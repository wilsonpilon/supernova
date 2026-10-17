// MSX2 runtime for the game: a Turbo-Pascal-CRT-like 80x25 text window system on SCREEN 7
// (see txt.c for the glyph blitter), keyboard, PSG sound, timing and file services.
//
// Scrolling is the expensive operation (the VDP moves ~220 KB/s), so lines that scroll the main
// text window are first drawn in off-screen VRAM lines below the frame and the window is scrolled
// once, by all of them, when the screen has to be consistent again (Flush).
#include "nova.h"
#include "mapper.h"
#include "../txt.h"
#include "dos.h"
#include "dos_mapper.h"
#include "../textpool.h"

// the BIOS variables below change in the interrupt handler
#define JIFFY()     (*(volatile u16*)0xFC9E)
#define KEY_READY() (*(volatile u16*)0xF3FA != *(volatile u16*)0xF3F8)

// ---- geometry
#define PX(col)   (16 + (col) * 6)       // pixel x of a 0-based text column
#define PY(row)   (6 + (row) * 8)        // pixel y of a 0-based text row
#define MAX_PENDING 5                    // rows 26..30 (0-based) of the VRAM page lie below the visible 212 lines

// ---- window / cursor state (all coordinates 1-based and window-relative, like Pascal's CRT)
static u8 wx1 = 1, wy1 = 1, wx2 = 80, wy2 = 25;
static u8 ww = 80, wh = 25;
static u8 cx = 1, cy = 1;
static u8 g_Fg = 7, g_Bg = 0, g_Blink = 0;
static u8 g_Defer;                       // the window is the main text window (lines may be deferred)
static u8 g_Pending;                     // lines scrolled but not yet moved on the screen
static u8 g_CursorShape = 3;

u8 g_Page2;

void Page2Set(u8 seg)
{
	g_Page2 = seg;
	DOSMapper_SetPage2(seg);
}

static void SetWindow(void)
{
	ww = wx2 - wx1 + 1;
	wh = wy2 - wy1 + 1;
	g_Defer = (wx1 == 2 && wy1 == 5 && wx2 == 79 && wy2 == 24);
}

// ---- VDP helpers
static void FillRows(u8 col0, u8 ncols, u8 row0, u8 nrows)
{
	VDP_CommandWait();
	VDP_CommandHMMV(PX(col0), PY(row0), ncols * 6, nrows * 8, g_Bg * 17);
	VDP_CommandWait();
}

static void CopyRows(u8 col0, u8 ncols, u8 srow, u8 drow, u8 nrows)
{
	VDP_CommandWait();
	VDP_CommandHMMM(PX(col0), PY(srow), PX(col0), PY(drow), ncols * 6, nrows * 8);
	VDP_CommandWait();
}

// ---- blinking text (Pascal colours 16-31): kept as regions that are redrawn while waiting for a key
#define BL_MAX 8
#define BL_TEXT 30
typedef struct { u8 col, row, len, fg, bg; char text[BL_TEXT]; } BlinkRegion;
static BlinkRegion g_Bl[BL_MAX];
static u8 g_BlN;
static u8 g_Phase = 1;
static u16 g_PhaseJ;

static void BlKill(u8 col0, u8 col1, u8 row)    // remove regions overlapping columns col0..col1 of `row` (absolute 0-based)
{
	u8 i = 0;
	while (i < g_BlN)
	{
		BlinkRegion* r = &g_Bl[i];
		if (r->row == row && r->col <= col1 && (u8)(r->col + r->len - 1) >= col0)
			g_Bl[i] = g_Bl[--g_BlN];
		else
			i++;
	}
}

static void BlClearWindow(void)
{
	u8 i = 0;
	while (i < g_BlN)
	{
		BlinkRegion* r = &g_Bl[i];
		if (r->row + 1 >= wy1 && r->row + 1 <= wy2 && r->col + 1 >= wx1 && r->col + 1 <= wx2)
			g_Bl[i] = g_Bl[--g_BlN];
		else
			i++;
	}
}

static void BlShiftUp(void)    // the main window scrolled up by one line
{
	u8 i = 0;
	while (i < g_BlN)
	{
		BlinkRegion* r = &g_Bl[i];
		if (r->row + 1 >= wy1 && r->row + 1 <= wy2 && r->col + 1 >= wx1 && r->col + 1 <= wx2)
		{
			if (r->row + 1 == wy1) { g_Bl[i] = g_Bl[--g_BlN]; continue; }
			r->row--;
		}
		i++;
	}
}

// ---- drawing
static void DrawCells(u8 col, u8 prow, const char* s, u8 n, u8 fg, u8 bg)
{
	char b[82];
	u8 i;
	for (i = 0; i < n; i++) b[i] = s[i];
	b[n] = 0;
	Txt_SetColor(fg, bg);
	Txt_Draw(col, prow, b);
}

// physical (0-based) row of post-scroll window row r
static u8 PhysRow(u8 r)
{
	u8 v = r + g_Pending;
	if (v > wh) return wy2 + 1 + (v - wh);    // virtual rows start at 0-based row 26
	return wy1 - 1 + v - 1;
}

static void DrawRun(u8 col, const char* s, u8 n)    // col: 0-based absolute column, at the cursor's row
{
	u8 arow = wy1 - 1 + cy - 1;
	BlKill(col, col + n - 1, arow);
	DrawCells(col, PhysRow(cy), s, n, g_Fg, g_Bg);
	if (g_Blink && n <= BL_TEXT && g_BlN < BL_MAX)
	{
		BlinkRegion* r = &g_Bl[g_BlN++];
		u8 i;
		r->col = col; r->row = arow; r->len = n; r->fg = g_Fg; r->bg = g_Bg;
		for (i = 0; i < n; i++) r->text[i] = s[i];
	}
}

static void Flush(void)
{
	u8 k = g_Pending;
	if (!k) return;
	if (k < wh)
		CopyRows(wx1 - 1, ww, wy1 - 1 + k, wy1 - 1, wh - k);
	CopyRows(wx1 - 1, ww, wy2 + 2, wy2 - k, k);
	g_Pending = 0;
}

static void NewLine(void)
{
	cx = 1;
	if (cy < wh) { cy++; return; }
	BlShiftUp();
	if (g_Defer)
	{
		if (g_Pending == MAX_PENDING) Flush();
		g_Pending++;
		FillRows(wx1 - 1, ww, wy2 + 1 + g_Pending, 1);
	}
	else
	{
		if (wh > 1) CopyRows(wx1 - 1, ww, wy1, wy1 - 1, wh - 1);
		FillRows(wx1 - 1, ww, wy2 - 1, 1);
	}
}

void ScreenFlush(void) { Flush(); }

void Wr(const char* s)
{
	u8 n, room;
	while (*s)
	{
		u8 c = (u8)*s;
		if (c == '\n') { NewLine(); s++; continue; }
		if (c == '\r') { cx = 1; s++; continue; }
		if (c == 8) { if (cx > 1) cx--; s++; continue; }
		if (c < 32) { s++; continue; }
		room = ww - cx + 1;
		n = 0;
		while (s[n] && (u8)s[n] >= 32 && n < room) n++;
		DrawRun(wx1 - 1 + cx - 1, s, n);
		cx += n; s += n;
		if (cx > ww) NewLine();
	}
}

void WrC(char c)
{
	char t[2];
	t[0] = c; t[1] = 0;
	Wr(t);
}

void WrNum(i16 n)
{
	char t[8];
	u8 k = 0, neg = 0;
	u16 v;
	if (n < 0) { neg = 1; v = (u16)(-n); } else v = (u16)n;
	do { t[k++] = '0' + v % 10; v /= 10; } while (v);
	if (neg) t[k++] = '-';
	{
		char o[8];
		u8 i;
		for (i = 0; i < k; i++) o[i] = t[k - 1 - i];
		o[k] = 0;
		Wr(o);
	}
}

void Wln(void) { NewLine(); }

void TextColor(u8 c) { g_Fg = c & 15; g_Blink = c >= 16; }
void TextBackground(u8 c) { g_Bg = c & 7; }

void Gotoxy(u8 xx, u8 yy)
{
	cx = xx < 1 ? 1 : (xx > ww ? ww : xx);
	cy = yy < 1 ? 1 : (yy > wh ? wh : yy);
}
u8 Wherex(void) { return cx; }
u8 Wherey(void) { return cy; }

void Window(u8 a, u8 b, u8 c, u8 d)
{
	Flush();
	wx1 = a; wy1 = b; wx2 = c; wy2 = d;
	SetWindow();
	cx = cy = 1;
}

void ClrScr(void)
{
	Flush();
	BlClearWindow();
	FillRows(wx1 - 1, ww, wy1 - 1, wh);
	cx = cy = 1;
}

void InsLine(void)
{
	u8 r;
	Flush();
	for (r = wh - 1; r >= cy; r--)    // bottom-up, so the rows do not overwrite each other
		CopyRows(wx1 - 1, ww, wy1 - 1 + r - 1, wy1 - 1 + r, 1);
	FillRows(wx1 - 1, ww, wy1 - 1 + cy - 1, 1);
}

void Border(u8 colour) { VDP_SetColor(colour & 15); }

// ---- cursor and blink
static void CursorDraw(u8 on)
{
	u8 col, row;
	if (g_CursorShape == 3 || cx > ww) return;
	col = wx1 - 1 + cx - 1;
	row = PhysRow(cy);
	if (!on)
		DrawCells(col, row, " ", 1, g_Fg, g_Bg);
	else if (g_CursorShape == 1)
		DrawCells(col, row, "_", 1, g_Fg, g_Bg);
	else
		DrawCells(col, row, " ", 1, g_Fg, g_Fg);
}

static void BlinkDraw(void)
{
	u8 i;
	for (i = 0; i < g_BlN; i++)
	{
		BlinkRegion* r = &g_Bl[i];
		DrawCells(r->col, r->row, r->text, r->len, g_Phase ? r->fg : r->bg, r->bg);
	}
}

void CursorShape(u8 n) { g_CursorShape = n; }

// ---- keyboard
static const u8 g_ScanOfCode[] =
{
	0xF1, 59, 0xF2, 60, 0xF3, 61, 0xF4, 62, 0xF5, 63, 0xF6, 64, 0xF7, 65, 0xF8, 66, 0xF9, 67, 0xFA, 68,
	0x1E, 72, 0x1F, 80, 0x1D, 75, 0x1C, 77, 0x0B, 71, 0x18, 73, 0x12, 82, 0x7F, 83, 0, 0
};

// Reads the next character of the BIOS keyboard buffer (FBF0h-FC17h) without calling the BIOS
static u8 ReadKeyBuffer(void)
{
	volatile u16* get = (volatile u16*)0xF3FA;
	u16 p;
	u8 c;
	__asm
		di
	__endasm;
	p = *get;
	c = *(volatile u8*)p;
	p++;
	if (p >= 0xFC18) p = 0xFBF0;
	*get = p;
	__asm
		ei
	__endasm;
	return c;
}

u8 KeyPressed(void)
{
	return KEY_READY();
}

u16 GetKey(void)
{
	u8 c;
	const u8* t;
	u8 curOn = 0;
	Flush();
	g_Phase = 1; g_PhaseJ = JIFFY();
	CursorDraw(1); curOn = 1;
	while (!KEY_READY())
	{
		if ((u16)(JIFFY() - g_PhaseJ) >= 16)
		{
			g_PhaseJ = JIFFY();
			g_Phase = !g_Phase;
			CursorDraw(g_Phase);
			BlinkDraw();
		}
	}
	if (g_BlN && !g_Phase) { g_Phase = 1; BlinkDraw(); }
	if (curOn) CursorDraw(0);
	c = ReadKeyBuffer();
	for (t = g_ScanOfCode; t[0]; t += 2)
		if (t[0] == c) return (u16)t[1] << 8;
	return c;
}

void ReadLn(char* s, u8 maxlen)
{
	u8 n = 0;
	u16 k;
	for (;;)
	{
		k = GetKey();
		if ((k & 0xFF) == 13) break;
		if ((k & 0xFF) == 8) { if (n) { n--; WrC(8); WrC(' '); WrC(8); } }
		else if ((k & 0xFF) >= 32 && n < maxlen) { s[n++] = (char)k; WrC((char)k); }
	}
	s[n] = 0;
	Wln();
}

// ---- timing, random numbers
static u16 g_LastSoundJ = 0xFFFF;

void Delay(u16 ms)
{
	u16 frames, start;
	if (ms >= 200) Flush();
	frames = (g_RG09SAV & 2) ? ms / 20 : ms / 17;
	if (!frames) return;
	g_LastSoundJ = 0xFFFF;
	start = JIFFY();
	while ((u16)(JIFFY() - start) < frames) { }
}

static u16 g_Rnd = 0x1D2B;

void Randomize(void) { g_Rnd ^= JIFFY() * 31 + 1; if (!g_Rnd) g_Rnd = 0x1D2B; }

u16 Rnd(u16 n)
{
	g_Rnd ^= g_Rnd << 7;
	g_Rnd ^= g_Rnd >> 9;
	g_Rnd ^= g_Rnd << 8;
	if (n == 0) return 0;
	if (n < 256) return ((u16)(u8)(g_Rnd >> 8) * n) >> 8;
	return g_Rnd % n;
}

// ---- PSG sound: channel A square wave. Pitch changes are applied at most once per frame so the long
// random "noise" loops of the original (thousands of sound() calls) stay cheap.
static u8 s_PsgReg, s_PsgVal;

static void PsgOut(void) __naked
{
	__asm
		di
		ld		a, (_s_PsgReg)
		out		(#0xA0), a
		ld		a, (_s_PsgVal)
		out		(#0xA1), a
		ei
		ret
	__endasm;
}

static void PsgWrite(u8 reg, u8 val)
{
	s_PsgReg = reg;
	s_PsgVal = val;
	PsgOut();
}

void Sound(u16 hz)
{
	u16 j = JIFFY(), period;
	if (j == g_LastSoundJ) return;
	g_LastSoundJ = j;
	if (hz < 28) period = 4095;
	else
	{
		period = (u16)(111861UL / hz);
		if (period < 1) period = 1;
	}
	PsgWrite(0, (u8)period);
	PsgWrite(1, (u8)(period >> 8));
	PsgWrite(7, 0xBE);
	PsgWrite(8, 12);
}

void NoSound(void)
{
	g_LastSoundJ = 0xFFFF;
	PsgWrite(8, 0);
}

// ---- text records and files
u16 ReadText(u16 id, char* dst) { return TextPool_Read(id, dst); }

// A saved game is written / read as two blocks (the object locations and the rest of the state)
u8 FileWrite2(const char* name, const u8* b1, u16 l1, const u8* b2, u16 l2)
{
	u8 f = DOS_CreateHandle(name, O_WRONLY, 0);
	u16 n1, n2;
	if (g_DOS_LastError) return 0;
	n1 = DOS_WriteHandle(f, b1, l1);
	n2 = DOS_WriteHandle(f, b2, l2);
	DOS_CloseHandle(f);
	return !g_DOS_LastError && n1 == l1 && n2 == l2;
}

u16 FileRead2(const char* name, u8* b1, u16 l1, u8* b2, u16 l2)
{
	u8 f = DOS_OpenHandle(name, O_RDONLY);
	u16 n;
	if (g_DOS_LastError) return 0;
	n = DOS_ReadHandle(f, b1, l1);
	if (n == l1) n += DOS_ReadHandle(f, b2, l2);
	DOS_CloseHandle(f);
	return g_DOS_LastError ? 0 : n;
}

void ListSaveFiles(void)
{
	DOS_FIB* fib = DOS_FindFirstEntry("*.SAV", 0);
	u8 any = 0;
	while (fib && !g_DOS_LastError)
	{
		char b[16];
		u8 i = 0;
		while (fib->Filename[i] && fib->Filename[i] != '.' && i < 8) { b[i] = fib->Filename[i]; i++; }
		b[i] = 0;
		Wr("    * "); Wr(b); Wln();
		any = 1;
		fib = DOS_FindNextEntry();
	}
	if (!any) { Wr("There are no saved games on this disk."); Wln(); }
}

static u8 g_Page2Orig;

void Halt(void)
{
	NoSound();
	Page2Set(g_Page2Orig);
	// SCREEN 0 through CALSLT (the BIOS is not visible from MSX-DOS), then back to the DOS prompt
	__asm
		ld		a, #0
		ld		ix, #0x005F
		ld		iy, (0xFCC1 - 1)
		call	0x001C
	__endasm;
	DOS_Exit(0);
}

// ---- start-up

// The MSXgl startup code copies initialised data but does not clear the uninitialised variables:
// zero them before anything runs (call first thing in main).
void ClearData(void) __naked
{
	__asm
		ld		hl, #s__DATA
		ld		bc, #l__DATA
		ld		a, b
		or		a, c
		ret		z
		xor		a
		ld		(hl), a
		dec		bc
		ld		a, b
		or		a, c
		ret		z
		ld		e, l
		ld		d, h
		inc		de
		ldir
		ret
	__endasm;
}
static const u16 g_Pal[16] = {
	RGB16(0,0,0), RGB16(0,0,5), RGB16(0,5,0), RGB16(0,5,5),
	RGB16(5,0,0), RGB16(5,0,5), RGB16(5,2,0), RGB16(5,5,5),
	RGB16(2,2,2), RGB16(2,2,7), RGB16(2,7,2), RGB16(2,7,7),
	RGB16(7,2,2), RGB16(7,2,7), RGB16(7,7,2), RGB16(7,7,7),
};

// CP437 glyphs missing from the MSX font (6x8 cells; bit 7 = left pixel)
typedef struct { u8 code; u8 rows[8]; } Glyph;
static const Glyph g_Glyphs[] =
{
	{ 175, { 0x00, 0x90, 0x48, 0x24, 0x48, 0x90, 0x00, 0x00 } },    // »
	{ 179, { 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20 } },    // │
	{ 181, { 0x20, 0x20, 0xE0, 0x20, 0x20, 0xE0, 0x20, 0x20 } },    // ╡
	{ 184, { 0x00, 0x00, 0xE0, 0x20, 0x20, 0xE0, 0x20, 0x20 } },    // ╕
	{ 186, { 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48 } },    // ║
	{ 187, { 0x00, 0x00, 0xF0, 0x08, 0x08, 0x18, 0x48, 0x48 } },    // ╗
	{ 188, { 0x48, 0x48, 0x18, 0x08, 0x08, 0xF0, 0x00, 0x00 } },    // ╝
	{ 190, { 0x20, 0x20, 0xE0, 0x20, 0x20, 0xE0, 0x00, 0x00 } },    // ╛
	{ 198, { 0x20, 0x20, 0x3C, 0x20, 0x20, 0x3C, 0x20, 0x20 } },    // ╞
	{ 200, { 0x48, 0x48, 0x4C, 0x40, 0x40, 0x7C, 0x00, 0x00 } },    // ╚
	{ 201, { 0x00, 0x00, 0x7C, 0x40, 0x40, 0x4C, 0x48, 0x48 } },    // ╔
	{ 205, { 0x00, 0x00, 0xFC, 0x00, 0x00, 0xFC, 0x00, 0x00 } },    // ═
	{ 212, { 0x20, 0x20, 0x3C, 0x20, 0x20, 0x3C, 0x00, 0x00 } },    // ╘
	{ 213, { 0x00, 0x00, 0x3C, 0x20, 0x20, 0x3C, 0x20, 0x20 } },    // ╒
};

static void Die(const char* m)
{
	while (*m) DOS_CharOutput(*m++);
	DOS_CharOutput(13); DOS_CharOutput(10);
	g_Page2 = g_Page2Orig;
	Halt();
}

static u8 DiskFile(const char* name, u8* dst, u16 len)
{
	u8 f = DOS_OpenHandle(name, O_RDONLY);
	if (g_DOS_LastError) return 0;
	DOS_ReadHandle(f, dst, len);
	DOS_CloseHandle(f);
	return 1;
}

void ScreenBegin(void)
{
	u8 i, r;
	if (!DOSMapper_Init())
	{
		const char* m = "This game needs a memory mapper and MSX-DOS 2.\r\n";
		while (*m) DOS_CharOutput(*m++);
		DOS_Exit(1);
	}
	g_Page2 = g_Page2Orig = DOSMapper_GetPage2();
	if (*(volatile u16*)0x0006 < 0xD900) Die("Not enough free memory (the TPA must reach D900h).");
	VDP_SetMode(VDP_MODE_SCREEN7);
	VDP_SetLineCount(VDP_LINE_212);
	VDP_SetColor(0);
	VDP_EnableSprite(FALSE);
	for (i = 0; i < 16; i++) VDP_SetPaletteEntry(i, g_Pal[i]);
	VDP_CommandHMMV(0, 0, 512, 256, 0);
	VDP_CommandWait();
	if (!DiskFile("FONT.BIN", g_Scratch, 2048)) Die("FONT.BIN missing");
	Txt_Init(g_Scratch);
	for (i = 0; i < sizeof(g_Glyphs) / sizeof(g_Glyphs[0]); i++)
		for (r = 0; r < 8; r++) g_Plane[r][g_Glyphs[i].code] = g_Glyphs[i].rows[r];
	g_CLIKSW = 0;
	for (i = 0; i < 10; i++) { g_FNKSTR[i][0] = 0xF1 + i; g_FNKSTR[i][1] = 0; }
	SetWindow();
	if (!TextPool_Load()) Die("Text files (TXT*.BIN) missing");
}
