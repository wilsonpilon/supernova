// Basic types shared by the generated data files and the game code
#pragma once
#ifdef HOST_TEST
#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int16_t  i16;
#else
#include "msxgl.h"
// the Pascal source defines procedures named Min / Max
#undef Min
#undef Max
#endif

// variable placed at a fixed address (page 3, above the program); not zeroed at start-up
#ifdef HOST_TEST
#define AT(addr)
#else
#define AT(addr) __at(addr)
#endif
