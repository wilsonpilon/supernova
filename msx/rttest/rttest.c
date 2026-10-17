#include "../game/nova.h"
AT(0xD000) u8 g_Scratch[1400];
static void Msg(const char* m) { while (*m) DOS_CharOutput(*m++); DOS_CharOutput(13); DOS_CharOutput(10); }
void main()
{
	ClearData();
	ScreenBegin();
	TextColor(11);
	Wr("Press a key (will switch to text mode)"); Wln();
	GetKey();
	__asm
		ld		a, #0
		ld		ix, #0x005F
		ld		iy, (0xFCC1 - 1)
		call	0x001C
	__endasm;
	Msg("Text mode reached via CHGMOD");
	Msg("Now DOS_Exit(0)");
	DOS_Exit(0);
	Msg("DOS_Exit RETURNED");
	for (;;) { }
}
