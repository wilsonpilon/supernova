#include "txt.h"

#define X0B  8     // left margin in bytes (16 px)
#define Y0   6     // top margin in lines

// Page-aligned lookup tables, kept in page 3 (C000h+) so they survive mapper swaps of page 2 (8000h-BFFFh)
//  planes: font transposed, plane r holds row r of each glyph, indexed by char code
//  tabA/B/C: glyph row byte -> output VRAM byte 0/1/2 for the current fg/bg
__at(0xC000) u8 g_Plane[8][256];
__at(0xC800) u8 g_TabA[256];
__at(0xC900) u8 g_TabB[256];
__at(0xCA00) u8 g_TabC[256];

static u8 s_Fg = 255, s_Bg = 255;
static const u8* s_Str;
static u16 s_Vram;
static u8 s_Len;
static u8 s_Row;

void Txt_SetColor(u8 fg, u8 bg)
{
	if (fg == s_Fg && bg == s_Bg)
		return;
	s_Fg = fg;
	s_Bg = bg;
	u8 t[4];
	t[0] = (bg << 4) | bg;
	t[1] = (bg << 4) | fg;
	t[2] = (fg << 4) | bg;
	t[3] = (fg << 4) | fg;
	for (u8 i = 0; i < 4; i++)
		Mem_Set(t[i], &g_TabA[i * 64], 64);
	for (u8 i = 0; i < 16; i++)
		Mem_Set(t[i & 3], &g_TabB[i * 16], 16);
	for (u8 i = 0; i < 64; i++)
		Mem_Set(t[i & 3], &g_TabC[i * 4], 4);
	// B = (b >> 4) & 3 -> runs of 16 values, repeating 4 times; fix: index (b>>4)&3
	// C = (b >> 2) & 3 -> runs of 4 values, repeating 16 times
}

// font: 255 glyphs x 8 bytes (FONT.BIN)
void Txt_Init(const u8* f)
{
	for (u16 c = 0; c < 255; c++)
		for (u8 r = 0; r < 8; r++)
			g_Plane[r][c] = f[c * 8 + r];
	s_Fg = 255;
	Txt_SetColor(7, 0);
}

// Blit s_Len chars of s_Str at VRAM address s_Vram (row 0 of the first cell).
// A VRAM line is 256 bytes, so address bits 14-15 (line / 64) go to R#14 for every raster.
static void Blit() __naked
{
	__asm
		ld		a, (_s_Len)
		or		a
		ret		z

		ld		c, #0xC0				// C = font plane page (g_Plane = C000h)
		xor		a
		ld		(_s_Row), a
	blit_row:
		ld		hl, (_s_Vram)
		ld		a, (_s_Row)
		add		a, h
		ld		h, a					// H = VRAM line of this raster
		di
		rlca
		rlca
		and		a, #3
		out		(#0x99), a
		ld		a, #0x8E
		out		(#0x99), a				// R#14 = A16..A14
		ld		a, l
		out		(#0x99), a
		ld		a, h
		and		a, #0x3F
		or		a, #0x40
		out		(#0x99), a
		ei
		ld		de, (_s_Str)
		ld		a, (_s_Len)
		ld		b, a
	1$:
		ld		a, (de)
		inc		de
		ld		h, c
		ld		l, a
		ld		a, (hl)
		ld		l, a
		ld		h, #0xC8					// H = tabA page (g_TabA = C800h; B = C900h, C = CA00h)
		ld		a, (hl)
		out		(#0x98), a
		inc		h
		ld		a, (hl)
		out		(#0x98), a
		inc		h
		ld		a, (hl)
		out		(#0x98), a
		djnz	1$
		inc		c
		ld		a, (_s_Row)
		inc		a
		ld		(_s_Row), a
		cp		a, #8
		jr		nz, blit_row
		di
		xor		a
		out		(#0x99), a
		ld		a, #0x8E
		out		(#0x99), a				// R#14 = 0 again
		ei
		ret
	__endasm;
}

void Txt_Draw(u8 col, u8 row, const c8* str)
{
	u8 len = 0;
	while (str[len]) len++;
	if (col + len > TXT_COLS) len = TXT_COLS - col;
	s_Len = len;
	s_Str = (const u8*)str;
	s_Vram = (u16)(Y0 + row * 8) * 256 + X0B + col * 3;
	Blit();
}

void Txt_ClearRow(u8 row, u8 bg)
{
	VDP_CommandHMMV(X0B * 2, Y0 + row * 8, TXT_COLS * 6, 8, (bg << 4) | bg);
	VDP_CommandWait();
}

void Txt_Scroll(u8 row0, u8 row1, u8 n)
{
	VDP_CommandHMMM(X0B * 2, Y0 + (row0 + n) * 8, X0B * 2, Y0 + row0 * 8, TXT_COLS * 6, (row1 - row0 + 1 - n) * 8);
	VDP_CommandWait();
}
