#include "msxgl.h"
#include "txt.h"
#include "dos.h"
#include "dos_mapper.h"

// CGA palette (RGB 3 bits each)
static const u16 g_Pal[16] = {
	RGB16(0,0,0), RGB16(0,0,5), RGB16(0,5,0), RGB16(0,5,5),
	RGB16(5,0,0), RGB16(5,0,5), RGB16(5,2,0), RGB16(5,5,5),
	RGB16(2,2,2), RGB16(2,2,7), RGB16(2,7,2), RGB16(2,7,7),
	RGB16(7,2,2), RGB16(7,2,7), RGB16(7,7,2), RGB16(7,7,7),
};

static const c8 g_Line[] = "The quick brown fox jumps over the lazy dog. SUPERNOVA text engine on SCREEN 7!";
static const u8 g_Cols[] = { 11, 10, 14, 12, 15, 9, 3, 7 };

static void U16ToStr(u16 v, c8* out)
{
	c8 t[6]; u8 n = 0;
	do { t[n++] = '0' + v % 10; v /= 10; } while (v);
	u8 i = 0;
	while (n) out[i++] = t[--n];
	out[i] = 0;
}

static void Report(u8 row, const c8* label, u16 v)
{
	c8 num[8];
	Txt_ClearRow(row, 1);
	Txt_Draw(1, row, label);
	U16ToStr(v, num);
	Txt_Draw(60, row, num);
}

void main()
{
	VDP_SetMode(VDP_MODE_SCREEN7);
	VDP_SetLineCount(VDP_LINE_212);
	VDP_SetColor(0);
	VDP_EnableSprite(FALSE);
	for (u8 i = 0; i < 16; i++)
		VDP_SetPaletteEntry(i, g_Pal[i]);
	VDP_CommandHMMV(0, 0, 512, 212, 0);
	VDP_CommandWait();
	Txt_Init();

	// Page-2 survival check: map a fresh mapper segment over 8000h-BFFFh and trash it.
	// If the font/lookup tables lived in page 2 the text below would be garbage.
	u8 savedP2 = 0;
	if (DOSMapper_Init())
	{
		DOS_Segment seg;
		savedP2 = DOSMapper_GetPage(2);
		if (DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &seg))
		{
			DOSMapper_SetPage2(seg.Number);
			Mem_Set(0xAA, (void*)0x8000, 0x4000);
		}
	}

	// A: full screen redraw, 25 lines of 78 chars, mixed colors (color change on every line)
	u16 t0 = g_JIFFY;
	for (u8 y = 0; y < TXT_ROWS; y++)
	{
		Txt_SetColor(g_Cols[y & 7], (y == 1 || y == 2) ? 4 : 0);
		Txt_Draw(0, y, g_Line);
	}
	u16 tA = g_JIFFY - t0;

	// B: 20 scrolls of window rows 4..23 by 1 line (no drawing)
	t0 = g_JIFFY;
	for (u8 n = 0; n < 20; n++)
		Txt_Scroll(4, 23, 1);
	u16 tB = g_JIFFY - t0;

	// C: 20 scrolls by 6 lines
	t0 = g_JIFFY;
	for (u8 n = 0; n < 20; n++)
		Txt_Scroll(4, 23, 6);
	u16 tC = g_JIFFY - t0;

	// D: realistic turn: scroll by 6 + draw 6 colored lines, x10
	t0 = g_JIFFY;
	for (u8 n = 0; n < 10; n++)
	{
		Txt_Scroll(4, 23, 6);
		for (u8 l = 0; l < 6; l++)
		{
			Txt_SetColor(g_Cols[l], 0);
			Txt_Draw(1, 18 + l, g_Line);
		}
	}
	u16 tD = g_JIFFY - t0;

	// E: 20 clear rows
	t0 = g_JIFFY;
	for (u8 n = 0; n < 20; n++)
		Txt_ClearRow(10, 0);
	u16 tE = g_JIFFY - t0;

	// F: redraw 25 lines, same color (no table rebuild)
	t0 = g_JIFFY;
	Txt_SetColor(11, 0);
	for (u8 y = 0; y < TXT_ROWS; y++)
		Txt_Draw(0, y, g_Line);
	u16 tF = g_JIFFY - t0;

	// G: 100 color changes, no drawing
	t0 = g_JIFFY;
	for (u8 n = 0; n < 100; n++)
		Txt_SetColor(g_Cols[n & 7], 0);
	u16 tG = g_JIFFY - t0;

	Txt_SetColor(15, 1);
	Report(0, "A: redraw 25x78 (25 lines, color/line)   frames", tA);
	Report(1, "B: 20 x scroll window 20 rows by 1       frames", tB);
	Report(2, "C: 20 x scroll window by 6 rows          frames", tC);
	Report(3, "D: 10 x (scroll 6 + draw 6 lines)        frames", tD);
	Report(4, "E: 20 x clear row                         frames", tE);
	Report(5, "F: redraw 25x78 same color (no rebuild)   frames", tF);
	Report(6, "G: 100 x SetColor change, no drawing      frames", tG);
	Txt_SetColor(13, 0);
	Txt_Draw(1, 8, "Blink test (palette 13 toggles red/blue): BLINKING");

	if (savedP2) DOSMapper_SetPage2(savedP2);
	u8 f = 0;
	while (!Keyboard_IsKeyPressed(KEY_ESC))
	{
		Halt();
		if ((++f & 31) == 0)
			VDP_SetPaletteEntry(13, (f & 32) ? RGB16(7,0,0) : RGB16(0,0,7));
	}
	BIOS_Exit(0);
}
