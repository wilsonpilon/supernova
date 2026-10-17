// 80x25 text engine on SCREEN 7 (512x212, 16 colors), 6x8 font
#pragma once
#include "msxgl.h"

#define TXT_COLS 80
#define TXT_ROWS 25

extern __at(0xC000) u8 g_Plane[8][256];    // font planes (C000h), patched by the runtime for CP437 box characters

void Txt_Init(const u8* font);        // font: 8 bytes per glyph, glyph 0 first (2048 bytes)
void Txt_SetColor(u8 fg, u8 bg);
void Txt_Draw(u8 col, u8 row, const c8* str);       // draw string with current colors
void Txt_Scroll(u8 row0, u8 row1, u8 n);          // scroll rows row0..row1 up by n rows
void Txt_ClearRow(u8 row, u8 bg);
