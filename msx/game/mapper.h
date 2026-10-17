// Page 2 (8000h-BFFFh) is a window onto mapper segments: overlays and text pools.
// g_Page2 shadows the segment currently mapped there so code can save / restore it cheaply.
#pragma once
#include "types.h"

extern u8 g_Page2;
void Page2Set(u8 seg);

// Scratch area in page 3 (kept out of page 2): text records, start-up file loading
extern AT(0xD000) u8 g_Scratch[1400];
