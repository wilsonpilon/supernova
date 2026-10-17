// NOVA.PAS: Describe, Move and the main program.
#include "nova.h"

void Describe(u8 room)
{
	char w[30];
	Col(9, 9); Wr(g_RN[Prm]); Col(11, 7); Wln();
	if (En(1) || (!En(1) && !SET_IN(Brief, Prm)))
	{
		SET_ADD(Brief, Prm);
		switch (Prm)    // alternate room descriptions
		{
		case 29: if (En(35)) { RS(130); RS(131); } else RR(29); break;
		case 34: if (En(30)) { RS(81); RS(82); } else RR(34); break;
		case 52: if (En(39)) RS(126); else RR(52); break;
		case 91: if (En(51)) { RS(157); RS(158); } else RR(91); break;
		default: RR(room);
		}
	}
	switch (Prm)    // for initial object's description
	{
	case 1: if (LOC_HAS(1, 16)) RL(150); break;
	case 107: if (LOC_HAS(107, 19)) RL(518); break;
	case 38: if (LOC_HAS(38, 17)) RL(260); break;
	case 41: if (LOC_HAS(41, 18) && !En(34)) RS(96); break;
	case 55: if (LOC_HAS(55, 11)) RL(290); break;
	case 63: if (LOC_HAS(63, 20)) RL(356); break;
	case 73: if (LOC_HAS(73, 28)) RL(352); break;
	case 80: if (LOC_HAS(80, 17)) RL(395); break;
	case 94: if (LOC_HAS(94, 26)) RL(397); if (LOC_HAS(94, 2)) RL(398); break;
	}
	for (o = 1; o <= MMax; o++)    // print the objects that are lying in the room
		if (R[o] == Prm)
		{
			if (o == 6) { Wr("There is some "); WrFN(o); Wr(" here."); }
			else
			{
				FN(o, w);
				if (w[0] == 'a' || w[0] == 'e' || w[0] == 'i' || w[0] == 'o' || w[0] == 'u')
				{ Wr("There is an "); WrFN(o); Wr(" here."); }
				else { Wr("There is a "); WrFN(o); Wr(" here."); }
			}
			if ((o == 29 && MugCon != Null) || (o == 7 && HolstCon != Null) || (o == 5 && SatchCon != Null))
				Wr("  (with something in it)");
			if (o == 5 && R[5] == 20) Wr("  (lying in some trash)");
			if (o == 7 && LOC_HAS(12, 7) && R[7] == 12) Wr("  (on the shelf)");
			if (o == NicheCon) Wr("  (on the niche)");
			if (En(6) && o == 16) Wr("  (on the seat)");
			if (o == PyraCon) Wr("  (in the pyramid's slot)");
			if (o == HingeCon) Wr("  (inside the statue's mouth)");
			if (o == PodumCon) Wr("  (on top of the podium)");
			if (o == 16 && En(37)) Wr("  (under the crystal)");
			if (o == RobotCon) Wr("  (in the robot's panel)");
			if (IN29(Socket, o)) Wr("  (in one of the regulator's sockets)");
			Wln();
		}
}

void MoveTo(u8 New)
{
	SF();
	if (En(7)) RL(22); else
	if (En(8)) RL(23); else
	{
		if (MugCon != Null && IN29(Inv, 29))
		{ if (Prm != 8) LOC_DEL(Prm, MugCon); LOC_ADD(New, MugCon); }
		if (SatchCon != Null && IN29(Inv, 5)) { LOC_DEL(Prm, SatchCon); LOC_ADD(New, SatchCon); }
		if (HolstCon != Null && IN29(Inv, 7)) { LOC_DEL(Prm, HolstCon); LOC_ADD(New, HolstCon); }
		Prm = New;
		Describe(Prm);
	}
}

static void TimeEvents(void)
{
	Time1();
	switch (TFlag)
	{
	case 1: Time2A(); break;
	case 2: Time2B(); break;
	case 3: Time2C(); break;
	case 4: Time2D(); break;
	}
}

static void RoomRoutine(void)
{
	if (Prm == 1) Room1();
	else if (Prm == 2 || Prm == 3) Room2();
	else if (IN_RANGE(Prm, 4, 7)) Room3();
	else if (IN_RANGE(Prm, 8, 12)) Room4();
	else if (IN_RANGE(Prm, 13, 22)) Room5();
	else if (IN_RANGE(Prm, 23, 29)) Room6();
	else if (IN_RANGE(Prm, 30, 34)) Room7();
	else if (IN_RANGE(Prm, 35, 38)) Room8();
	else if (IN_RANGE(Prm, 39, 41)) Room9();
	else if (IN_RANGE(Prm, 42, 50)) Room10();
	else if (IN_RANGE(Prm, 51, 55)) Room11();
	else if (Prm == 56) Room12();
	else if (IN_RANGE(Prm, 57, 63)) Room13();
	else if (IN_RANGE(Prm, 64, 67) || Prm == 71 || Prm == 72) Room14();
	else if (IN_RANGE(Prm, 68, 70)) Room15();
	else if (IN_RANGE(Prm, 73, 77)) Room16();
	else if (IN_RANGE(Prm, 78, 80)) Room17();
	else if (IN_RANGE(Prm, 81, 83) || Prm == 93 || Prm == 95 || Prm == 96) Room18();
	else if (IN_RANGE(Prm, 84, 89)) Room19();
	else if (IN_RANGE(Prm, 90, 92)) Room20();
	else if (Prm == 94 || IN_RANGE(Prm, 97, 100)) Room21();
	else if (IN_RANGE(Prm, 101, 105)) Room22();
	else if (IN_RANGE(Prm, 106, 108)) Room23();
}

static void DefaultRoutine(void)
{
	switch (Vb)
	{
	case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 30: case 82: case 87: Default0(); break;
	case 16: case 17: case 18: Default1(); break;
	case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 32: case 35: case 51: case 77: case 79: case 62: Default2(); break;
	case 28: case 47: Default3(); break;
	case 37: case 39: Default4(); break;
	case 46: case 64: Default5(); break;
	case 66: case 67: case 68: case 69: case 71: Default6(); break;
	case 31: case 65: case 75: case 80: case 81: case 83: case 84: case 93: case 95: Default7(); break;
	case 41: case 50: case 88: Default8(); break;
	case 20: case 22: case 24: case 26: case 70: case 76: Default9(); break;
	case 33: Default10(); break;
	case 42: case 44: case 73: case 86: case 89: case 90: case 94: Default11(); break;
	case 48: case 54: Default12(); break;
	case 19: case 53: case 56: case 72: case 74: case 78: case 91: Default13(); break;
	case 21: case 25: case 49: case 59: case 92: Default14(); break;
	}
}

void SetR(u8 obj, u8 room)
{
	if (obj >= 1 && obj <= MMax) R[obj] = room;
}

static void GameRun(void)
{
	Title();
	Init1(); Init2(); Init3();
	Describe(Prm);
	for (;;)
	{
		PlayerInput(Line);
		if (Len(Input) > 0) Parser_Syntax(Input);
		if (EFlag == Legal)
		{
			MoreThanOne();
			if (Present())
			{
				if (T[2] > 1)
				{
					RL(11); TimeEvents();
				}
				else
				{
					SFlag = 0;
					if (Vb == 71 && Prm == 55 && !En(35)) Vb = 1;
					RoomRoutine();
					if (!SFlag) DefaultRoutine();
					if (En(128)) DEAD();
					Min(128);
					if (!(IN_RANGE(Vb, 77, 79) || Vb == 82 || Vb == 86 || Vb == 87 || Vb == 95)) TimeEvents();
				}
			}
		}
	}
}

void RestartGame(void)
{
	Vocab_Init();
	GameRun();
}

void GameMain(void)
{
	Vocab_Init();
	ScreenBegin();
#ifndef HOST_TEST
	if (!OvlLoadAll())
	{
		Halt();
	}
#endif
	GameRun();
}
