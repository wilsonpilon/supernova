// DEFAULT.PAS: Default10 - Default14, DeadMain
#include "nova.h"

// Pascal set constant [a1..b1, a2..b2, ...] given as a list of (lo, hi) pairs ended by 0
static u8 InR(u8 v, const u8* r)
{
	while (*r) { if (v >= r[0] && v <= r[1]) return 1; r += 2; }
	return 0;
}

static const u8 k_On[]     = { 32, 37, 49, 49, 71, 71, 80, 80, 83, 87, 90, 90, 93, 93, 98, 98, 112, 113, 126, 126, 132, 132, 0 };
static const u8 k_Beside[] = { 32, 32, 35, 35, 37, 37, 49, 49, 51, 51, 54, 54, 56, 56, 62, 62, 66, 73, 80, 87, 89, 90, 94, 98,
                               104, 104, 112, 113, 116, 116, 120, 122, 125, 128, 132, 132, 0 };
static const u8 k_Under[]  = { 80, 81, 83, 86, 98, 98, 104, 104, 112, 112, 0 };
static const u8 k_Cont[]   = { 3, 3, 4, 4, 9, 9, 17, 17, 19, 19, 66, 66, 79, 79, 99, 99, 0 };   // [3,4,9,17,19,66,79,99] (mug)
static const u8 k_Satch[]  = { 3, 4, 6, 6, 8, 9, 12, 12, 17, 17, 19, 19, 0 };                    // [3,4,6,8,9,12,17,19]
static const u8 k_Holst[]  = { 3, 4, 8, 9, 12, 12, 17, 17, 19, 19, 0 };                          // [3,4,8,9,12,17,19]

static void PutInto(const u8* allowed, u8* con, const char* what, u16 full, u16 isMug)
{
	// shared by PUT ... INTO mug / satchel / holster
	if (*con == Null)
	{
		if (InR(N1, allowed))
		{
			if (isMug)
			{
				Wr("The "); WrFN(N1); Wr(" is now in the mug."); Wln();
				if (IN29(Mov, N1)) Van(N1);
				LOC_ADD(Prm, N1); *con = N1;
			}
			else
			{
				Van(N1); LOC_ADD(Prm, N1); *con = N1;
				Wr("The "); WrFN(N1); Wr(" is now in the "); Wr(what); Wr("."); Wln();
			}
		}
		else if (IN29(Mov, N1)) RL(137);
		else Crazy();
	}
	else RL(full);
}

void Default10(void)
{
	switch (Vb)
	{
	// PUT
	case 33:
		SFlag = 0;
		if (StrEq(VStr, "fill")) { x = N1; N1 = N2; N2 = x; }
		if ((IN29(Mov, N1) && IN29(Inv, N1)) || N1 == 66 || N1 == 79)
		{
			switch (N2)
			{
			case 64:
				if (Prm == 34)
				{
					if (N1 == 3 || N1 == 8 || N1 == 9)
					{
						if (Pr == 5)
						{
							if (PyraCon == Null)
							{
								Van(N1); SetR(N1, 34); PyraCon = N1;
								if (N1 == 9) { RS(83); Walls(8); Add(30); } else RL(224);
							}
							else RL(223);
						}
						else RL(222);
					}
					else RL(221);
				}
				break;
			case 126:
				if (En(31))
				{
					if (N1 == 3 || N1 == 8 || N1 == 9)
					{
						if (Pr == 1 || Pr == 5 || Pr == 7 || Pr == 10)
						{
							if (HingeCon == Null) { Van(N1); SetR(N1, 34); HingeCon = N1; RL(231); }
							else RL(230);
						}
						else NoSense();
					}
					else RL(229);
				}
				else RL(232);
				break;
			case 35:
				if (Prm == 1)
				{
					if (N1 == 16) { Van(16); Add(6); R[16] = 1; RL(144); Score(5, 25); }
					else { RL(143); Van(N1); SetR(N1, 1); }
				}
				break;
			case 120: RL(550); Van(N1); break;
			case 62:
				if (Prm == 22 || Prm == 24)
				{
					if (En(23)) { Van(N1); SetR(N1, 2); RL(134); } else RL(99);
				}
				break;
			case 66: if (Rnd(2) == 0) RL(375); else RL(376); Van(N1); break;
			case 42: RL(495); break;
			case 29: PutInto(k_Cont, &MugCon, "mug", 136, 1); break;
			case 5: PutInto(k_Satch, &SatchCon, "brown satchel", 138, 0); break;
			case 7: PutInto(k_Holst, &HolstCon, "holster", 581, 0); break;
			}
		}
		else { Wr("First you must have the "); WrFN(N1); Wr("."); Wln(); }

		if (!SFlag)
			switch (Pr)
			{
			case 1:
				if (InR(N2, k_On))
				{
					Van(N1); SetR(N1, Prm);
					Wr("The "); WrFN(N1); Wr(" is now on the "); WrFN(N2); Wr("."); Wln();
				}
				else RL(139);
				break;
			case 5: RL(139); break;
			case 7:
				if (InR(N2, k_Beside))
				{
					Van(N1); SetR(N1, Prm);
					Wr("The "); WrFN(N1); Wr(" is now beside the "); WrFN(N2); Wr("."); Wln();
				}
				else RL(139);
				break;
			case 10:
				if (InR(N2, k_Under))
				{
					Van(N1); SetR(N1, Prm);
					Wr("The "); WrFN(N1); Wr(" is now under the "); WrFN(N2); Wr("."); Wln();
				}
				else RL(139);
				break;
			default: Crazy();
			}
		break;
	}
}

void Default11(void)
{
	switch (Vb)
	{
	// PUSH and TURN ON
	case 44:
		if (StrEq(VStr, "turn on") || StrEq(VStr, "activat"))
		{
			switch (N1)
			{
			case 2:
				RL(460); Score(5, 117);
				switch (Rnd(4))
				{
				case 0: j = Rnd(250) + 420; for (i = 400; i <= j; i++) Play(400, i, 1); break;
				case 1:
					y = Rnd(35) + 2;
					for (x = 1; x <= 99; x++) { i = Rnd(9000) + 60; Play(i, i, y); Delay(y); }
					break;
				case 2:
					j = Rnd(230) + 21;
					for (x = 1; x <= j; x++)
					{
						i = Rnd(230) + 22;
						for (y = 1; y <= i; y++) Sound((u16)(x * y));
					}
					break;
				case 3:
					j = Rnd(9999) + 999;
					for (i = 1; i <= j; i++) Sound(Rnd(i) + i);
					break;
				}
				NoSound(); if (Region == 5) Sound(60);
				break;
			case 37: case 38: RL(373); break;
			case 98: case 48:
				if (Prm == SinkRm) Say("water", "flowing out");
				else { SinkRm = Prm; LOC_ADD(Prm, 79); RL(82); }
				break;
			case 127: if (!StrEq(NStr, "window")) RL(360); else RL(164); break;
			}
		}
		else
			switch (N1)
			{
			case 116: if (IN_RANGE(Prm, 42, 50)) RL(282); else RL(281); break;
			case 123: RL(472); break;
			default: RL(103);
			}
		break;

	// PULL and TURN OFF
	case 42:
		if (StrEq(VStr, "turn off") || StrEq(VStr, "deactiv"))
		{
			switch (N1)
			{
			case 98: case 48: case 79:
				if (Prm != SinkRm) Say("water", "off");
				else { SinkRm = Null; LOC_DEL(Prm, 79); RL(81); }
				break;
			case 127: if (!StrEq(NStr, "window")) RL(360); else RL(165); break;
			default: RL(165);
			}
		}
		else RL(103);
		break;

	// CLEAN
	case 94:
		switch (N1)
		{
		case 18:
			if (En(34)) RL(265);
			else
			{
				if (LOC_HAS(41, 18)) R[18] = 41;
				Score(15, 34);
				RS(91); SetNoun(18, 4);
			}
			break;
		case 126: if (StrEq(NStr, "bar")) RL(167); break;
		case 80: RL(166); break;
		case 98: RL(168); break;
		default: Wr("The "); Wr(NStr); Wr(" doesn't need to be cleaned."); Wln();
		}
		break;

	// RESTART
	case 86:
		Bor(1, 0); Cur(2);
		Wr("Are you sure you want to restart your game? ");
		CFlag = (char)(GetKey() & 0xFF);
		if (UpCase(CFlag) != 'Y') RL(169);
		else
		{
			RL(170); Delay(2000); Window(1, 1, 80, 25); ClrScr(); Bor(0, 0); NoSound();
			RestartGame();
		}
		Bor(0, 0);
		break;

	case 89: RL(171); break;      // SHOOT

	// TIE and UNTIE
	case 90:
		if (StrEq(VStr, "tie")) RL(172); else if (StrEq(VStr, "untie")) RL(173); else RL(437);
		break;

	// FIND
	case 73:
		if (IN29(Inv, N1)) { Wr("You are carrying the "); Wr(NStr); Wr("."); Wln(); }
		else { Wr("The "); Wr(NStr); Wr(" is close by, just look around."); Wln(); }
		break;
	}
}

void Default12(void)
{
	switch (Vb)
	{
	// HIT and BREAK
	case 54:
		if (N2 != Null)
		{
			if (IN29(Mov, N2) || N2 == 60)
			{
				if (N1 != N2)
				{
					if (Pr == 3 || Pr == 6)
						switch (N1)
						{
						case 1: if (Prm == 56 && LOC_HAS(56, 1)) RL(300); else RL(83); break;
						case 18:
							if (N2 == 10 || N2 == 11 || N2 == 29) { RS(97); Van(18); R[4] = Prm; }
							else RL(272);
							break;
						case 123: RL(416); break;
						default: RL(273);
						}
					else NoSense();
				}
				else Crazy();
			}
			else Crazy();
		}
		else
			switch (N1)    // N2 = Null
			{
			case 18: RL(274); break;
			default:
				if (Rnd(3) == 0) { Wr("At the last moment you decide to spare the "); Wr(NStr); Wr("'s life!"); Wln(); }
				else { Wr("Hitting the "); Wr(NStr); Wr(" doesn't help."); Wln(); }
			}
		break;

	// THROW
	case 48:
		if (N2 != Null)
		{
			if (IN29(Inv, N1))
			{
				if (IN29(Mov, N1))
				{
					if (N1 != N2)
					{
						if (IN_RANGE(Pr, 1, 5))
						{
							if (N1 == 29 && (MugCon == 79 || MugCon == 92 || MugCon == 99)) { MugCon = Null; RL(585); SFlag = 0; }
							if (N2 == 1 && Prm == 56 && LOC_HAS(56, 1)) { Van(N1); SetR(N1, 56); RL(301); }
							if (N2 == 49 && Prm == 56 && (N1 == 1 || N1 == 10 || N1 == 18 || N1 == 29)) { RS(112); Add(128); }
							if (N2 == 69 && Prm == 91)
							{
								RS(160); LOC_ADD(91, 123);
								LOC_DEL(91, 44); LOC_DEL(91, 129); T[23] = 15; Add(51); Van(N1); SetR(N1, 91);
							}
							if (N1 == 1 || IN_RANGE(N1, 22, 25)) { Van(N1); RL(430); }
							if (N2 == 46 && (Prm == 99 || Prm == 101))
							{
								RL(514); Blast();
								if (Prm == 99) T[27] = 2000; else T[28] = 2000;
								Van(N1);
							}
							switch (N2)
							{
							case 1: case 22: case 23: case 24: case 25: Van(N2); RL(431); break;
							case 42: Van(N1); RL(515); break;
							case 66: RL(375); Van(N1); break;
							case 111: RL(449); Van(N1); break;
							case 120: RL(550); Van(N1); break;
							case 123: RL(450); RL(451); Van(N1); SetR(N1, Prm); break;
							case 124: RL(452); RL(453); Van(N1); SetR(N1, Prm); break;
							case 127: if (Prm == 64) { if (Md == 3) { RL(537); Add(128); } else RL(538); } break;
							}
							if (N1 == 18 || N2 == 18) { Van(N1); SetR(N1, Prm); Van(18); RS(97); R[4] = Prm; }
							if (!SFlag)
							{
								Wr("The "); WrFN(N1); Wr(" collides with the "); WrFN(N2); Wr(", but nothing interesting happens."); Wln();
								Van(N1); SetR(N1, Prm);
							}
						}
						else NoSense();
					}
					else Crazy();
				}
				else Crazy();
			}
			else { Wr("First you must have the "); WrFN(N1); Wr("."); Wln(); }
		}
		else { Van(N1); SetR(N1, Prm); RL(275); }
		break;
	}
}

void Default13(void)
{
	switch (Vb)
	{
	// CLOSE
	case 53:
		switch (N1)
		{
		case 5: RL(289); break;
		case 56: RL(92); break;
		default: Wr("The "); Wr(NStr); Wr(" is not something that can be closed."); Wln();
		}
		break;

	// SCORE
	case 78:
		Col(11, 7);
		Wr("In "); Col(12, 9); WrNum(Tic); Col(11, 7);
		Wr(" moves you scored "); Col(12, 9); WrNum(Sc); Col(11, 7);
		Wr(" out of a possible 1000 points."); Wln();
		Wr("This score earns you the rank of "); Col(12, 9);
		if (IN_RANGE(Sc, 0, 9)) { Up("ABSOLUTE BEGINNER!"); Wln(); }
		else if (IN_RANGE(Sc, 10, 99)) Wr("Rookie");
		else if (IN_RANGE(Sc, 100, 179)) Wr("Struggling");
		else if (IN_RANGE(Sc, 180, 259)) Wr("Novice");
		else if (IN_RANGE(Sc, 260, 349)) Wr("Competent");
		else if (IN_RANGE(Sc, 350, 449)) Wr("Fair");
		else if (IN_RANGE(Sc, 450, 549)) Wr("Good");
		else if (IN_RANGE(Sc, 550, 649)) Wr("Great");
		else if (IN_RANGE(Sc, 650, 744)) Wr("Brilliant");
		else if (IN_RANGE(Sc, 745, 829)) Wr("Genius");
		else if (IN_RANGE(Sc, 830, 899)) Wr("Elite");
		else if (IN_RANGE(Sc, 900, 949)) Wr("Champion");
		else if (IN_RANGE(Sc, 950, 1000)) Wr("Galaxy Class");
		if (Sc > 9) { Wr(" Adventurer."); Wln(); }
		break;

	// REPAIR
	case 74: RL(357); break;

	// FOLLOW
	case 91:
		if (StrEq(VStr, "follow"))
			switch (N1)
			{
			case 123: RL(358); break;
			case 124: RL(359); break;
			default: Wr("Why, did the "); Wr(NStr); Wr(" leave?"); Wln();
			}
		else RL(432 + Rnd(5));   // for Scott in VOCAB!
		break;

	// WEAR
	case 72:
		if (N1 == 7 || N1 == 20 || N1 == 26 || N1 == 28)
		{
			if (IN29(Inv, N1))
			{
				if (!IN29(Wear, N1))
				{
					SET_ADD(Wear, N1); Wr("You are now wearing the "); WrFN(N1); Wr("."); Wln();
					if (IN_RANGE(Prm, 102, 105) && N1 == 20) RL(503);
				}
				else { Wr("You are already wearing the "); WrFN(N1); Wr("."); Wln(); }
			}
			else { Wr("First you must have the "); WrFN(N1); Wr("."); Wln(); }
		}
		else Crazy();
		break;

	// REMOVE
	case 19:
		if (IN29(Wear, N1))
		{
			SET_DEL(Wear, N1);
			Wr("You are no longer wearing the "); WrFN(N1); Wr("."); Wln();
			if (IN_RANGE(Prm, 102, 105)) RL(492);
		}
		else { Wr("You are not wearing the "); WrFN(N1); Wr(" to begin with."); Wln(); }
		break;

	// TYPE
	case 56:
		if (Here(37) || Here(52)) RL(273); else RL(361);
		break;
	}
}

void Default14(void)
{
	switch (Vb)
	{
	// SHOW / GIVE
	case 49:
		if (N2 == 46 || N2 == 111 || N2 == 123 || N2 == 124)
		{
			if (Pr == 1)
				switch (N2)
				{
				case 46: RL(447); break;
				case 111:
					if (N1 == 8) { SET_ADD(Inv, 29); Van(8); Score(10, 21); RS(11); T[1] = 21; Add(9); Add(16); }
					else RL(444);
					break;
				case 124: RL(445); break;
				case 123: RL(446); break;
				}
			else NoSense();
		}
		else Crazy();
		break;

	// JUMP
	case 59:
		if (N1 != Null)
		{
			if (StrEq(VStr, "jump over") || StrEq(VStr, "leap over") || StrEq(VStr, "jump across"))
			{
				if (IN_RANGE(N1, 1, 29) || N1 == 36 || N1 == 49 || N1 == 51 || N1 == 69 || N1 == 71 || N1 == 73 || N1 == 132)
				{ Wr("You land on the other side of the "); Wr(NStr); Wr("."); Wln(); }
				else Crazy();
			}
			else if (StrEq(VStr, "jump off") && En(8)) { RL(475); Min(8); }
			else if (N1 == 34 || N1 == 56 || N1 == 60 || N1 == 62 || N1 == 68 || N1 == 72 || N1 == 82 || N1 == 92) Crazy();
			else if (IN_RANGE(N1, 1, 29) && !IN29(Inv, N1)) { Wr("You land on the "); Wr(NStr); Wr("."); Wln(); }
			else if (StrEq(NStr, "quicksa")) { RL(582); Add(128); }
			else if (StrEq(NStr, "swamp")) { RL(583); Add(128); }
			else RL(476);
		}
		else if (StrEq(VStr, "jump off") && En(8)) { RL(475); Min(8); }
		else RL(539);
		break;

	// DIG
	case 92:
		switch (N1)
		{
		case 49: case 116: case 122: RL(556); break;
		case 120: RL(557); break;
		default: Crazy();
		}
		break;

	// TALK / GREET
	case 21:
		if (StrEq(VStr, "greet"))
			switch (N1)
			{
			case 111: RL(51); break;
			case 124: RL(46); break;
			case 123: RL(441); break;
			default: Crazy();
			}
		else if (N1 == 111 || N1 == 123 || N1 == 124)
		{
			Wr("To talk to the "); WrFN(N1); Wr(" enter:"); Wln();
			WrFN(N1); Wr(", < what you want to say goes here >"); Wln();
		}
		else Crazy();
		break;

	case 25: RS(245); break;
	}
}

void DeadMain(void)
{
	Wln();
	Gotoxy(1, 20); for (x = 5; x >= 1; x--) { Wln(); Delay(99); Sound(300 + (x * 50)); Bor(x, 7); }
	do x = Rnd(14) + 1; while (x == 4 || x == 12);
	Gotoxy(1, 15); Col(x, 15); Bak(4, 7);
	Wr("***************************************"); Wr("***************************************"); Wln();
	Delay(99); Sound(300); Bor(6, 0);
	Gotoxy(1, 16);
	for (x = 3; x >= 1; x--)
	{
		Wr("*                                      "); Wr("                                      *");
		Delay(99); Sound(100 + (x * 50)); Bor(x + 8, 15);
	}
	Gotoxy(1, 19);
	Wr("***************************************"); Wr("***************************************");
	Delay(99); Sound(100); Bor(14, 0);
	Gotoxy(30, 17); Col(16, 31); Wr("YOU HAVE DIED!!!"); Delay(99); Sound(50);
	Gotoxy(1, 20); Bak(0, 0); Col(11, 7); Bor(12, 7); Wln(); Delay(99); NoSound();
	Delay(999); i = Rnd(3) + 2;
	Tune(i, 3, 830); Tune(i, 3, 770); Tune(i, 3, 200); Tune(i, 3, 0); Delay(800);
	Tune(i, 6, 400); Tune(i, 5, 400); Tune(i, 5, 400);
	Tune(i, 3, 400); Tune(i, 3, 400); Tune(i, 2, 400); Tune(i, 3, 810);
	for (;;)
	{
		Col(11, 7); Cur(2);
		do
		{
			Wr("Would you like to "); Col(14, 15); Wr("S"); Col(11, 7);
			Wr("tart a new game, "); Col(14, 15); Wr("R"); Col(11, 7);
			Wr("estore or "); Col(14, 15); Wr("Q"); Col(11, 7); Wr("uit? ");
			CFlag = (char)(GetKey() & 0xFF); CFlag = UpCase(CFlag); Col(9, 15); WrC(CFlag); Wln();
			Play(500, 500, 40); Col(11, 7);
		} while (!(CFlag == 'S' || CFlag == 'R' || CFlag == 'Q'));
		Bor(0, 0); Cur(3);
		switch (CFlag)
		{
		case 'S':
			RL(170); Delay(1500); Window(1, 1, 80, 25); ClrScr();
			RestartGame();
			return;
		case 'R':
			Min(126); Restore();
			if (En(126)) return;
			break;
		case 'Q':
			Window(1, 1, 80, 25); ClrScr(); Col(3, 7);
			Wr("In "); WrNum(Tic); Wr(" moves you scored "); WrNum(Sc); Wr(" out of a possible 1000 points."); Wln(); Wln();
			Col(31, 16); Bak(1, 7); Wr("Good-bye!"); Wln(); Wln(); Cur(1);
			Halt();
			return;
		}
	}
}

void DEAD(void) { DeadMain(); }
void Call13(void) { Default13(); }
