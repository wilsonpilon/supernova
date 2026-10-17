// NOVA.PAS: Room21 - Room23 (planetship: lower levels, the catwalk, the reactor regulators)
#include "nova.h"

void Room21(void)    // Planetship
{
	switch (Prm)
	{
	case 94:    // Maintenance
		switch (Vb)
		{
		case 1: case 13: Door(84, 65); break;
		case 28:
			switch (N1)
			{
			case 51: RL(489); break;
			case 87:
				RL(490); Score(15, 110);
				if (!En(49)) { Score(10, 49); RL(396); LOC_ADD(94, 26); }
				else if (LOC_HAS(94, 26)) RL(397);
				break;
			}
			break;
		case 39:
			if (SET_IN(NounSet, 30))
			{
				if (LOC_HAS(94, 2)) R[2] = 94;
				if (LOC_HAS(94, 26)) R[26] = 94;
			}
			if (SET_IN(NounSet, 51))
			{
				RL(491); SET_DEL(NounSet, 51);
				if (!SetEmpty(NounSet, 17)) SFlag = 0;
			}
			break;
		}
		break;

	case 97:    // Cryogenic Chamber
		switch (Vb)
		{
		case 4: case 11: case 13: Door(93, 5); break;
		case 28: if (N1 == 69) RS(193); break;
		case 18: RS(194); Van(N1); break;
		}
		break;

	case 98:    // Identity Booth
		switch (Vb)
		{
		case 2: case 11:
			Door(99, 0);
			if (T[27] > 1) { Col(10, 7); RS(208); Delay(999); Blast(); DEAD(); }
			else
			{
				T[27] = 2000; Col(10, 7); RL(504); Play(22, 22, 999); RS(210);
				Delay(999); Blast(); RL(505);
			}
			break;
		case 4: case 13: Door(93, 5); break;
		case 28: case 31:
			switch (N1)
			{
			case 81: case 119: if (StrEq(NStr, "message")) RL(517); else RB2(13, 13); break;
			case 59: RL(517); break;
			}
			break;
		}
		break;

	case 99:    // Security Tunnel #1
		switch (Vb)
		{
		case 1: case 13: Door(98, 0); break;
		case 2: case 11: Score(15, 111); Door(100, 0); break;
		case 4: RL(506); break;
		case 41: case 88: if (N1 == 55) RL(509); break;
		case 28: switch (N1) { case 46: RS(216); break; case 55: RL(510); break; case 88: RL(511); break; } break;
		case 42: case 44: case 75: if (N1 == 46) { RL(513); T[27] = 2000; Add(7); } break;
		case 39: if (SET_IN(NounSet, 88)) { RL(513); T[27] = 2000; Add(7); } break;
		}
		break;

	case 100:    // Security Tunnel #2
		switch (Vb)
		{
		case 1: case 13: Door(99, 0); Col(10, 7); RS(208); Delay(999); Blast(); DEAD(); break;
		case 2: case 11:
			Door(101, 0);
			if (T[28] > 1) { Col(10, 7); RS(209); Delay(999); Blast(); DEAD(); }
			else
			{
				T[28] = 2000; Col(10, 7); RL(504); Play(22, 22, 999); RS(211);
				Delay(999); Blast(); RS(212); SET_ADD(PStat, 4); T[29] = 14;
			}
			break;
		case 41: case 88: if (N1 == 55) RL(512); break;
		}
		break;
	}
}

// 102-105: without the security suit's gloves/helmet (object 20) most verbs are refused
static u8 CatwalkBlocked(void)
{
	if ((IN_RANGE(Vb, 1, 14) || Vb == 16 || IN_RANGE(Vb, 20, 31) || IN_RANGE(Vb, 33, 44) || IN_RANGE(Vb, 48, 64) ||
		IN_RANGE(Vb, 73, 76) || IN_RANGE(Vb, 83, 85) || IN_RANGE(Vb, 88, 94)) && !IN29(Wear, 20))
	{
		RL(491);
		return 1;
	}
	return 0;
}

static void CatwalkCommon(void)
{
	switch (Vb)
	{
	case 24: case 25: case 28:
		switch (N1)
		{
		case 34: case 49: RL(493); break;
		case 131: if (StrEq(NStr, "catwalk")) RL(494); else { RS(195); RS(196); } break;
		}
		break;
	case 75: case 76: if (N1 == 42) RL(495); break;
	case 51: case 59: if (N1 == 42 || N1 == 54 || N1 == 131) { RL(496); DEAD(); } break;
	}
}

void Room22(void)    // Planetship (Lower Level)
{
	switch (Prm)
	{
	case 101:    // Security Tunnel #3
		switch (Vb)
		{
		case 1: case 13: Door(100, 0); break;
		case 2: case 11:
			Region = 5; Door(102, 0); Score(20, 112);
			if (!IN29(Wear, 20)) RL(492);
			break;
		case 41: case 88: if (N1 == 55) RL(509); break;
		case 28: switch (N1) { case 46: RS(216); break; case 55: RL(510); break; case 88: RL(511); break; } break;
		case 42: case 44: case 75: if (N1 == 46) { RL(513); T[28] = 2000; Add(7); } break;
		case 39: if (SET_IN(NounSet, 88)) { RL(513); T[28] = 2000; Add(7); } break;
		}
		break;

	case 102:    // North Catwalk
		if (CatwalkBlocked()) break;
		switch (Vb)
		{
		case 1: case 11: case 13: Region = Null; Door(101, 0); Col(10, 7); RS(209); Delay(999); Blast(); DEAD(); break;
		case 7: MoveTo(103); break;
		case 8: MoveTo(105); break;
		default: CatwalkCommon();
		}
		break;

	case 103:    // East Catwalk
		if (CatwalkBlocked()) break;
		switch (Vb)
		{
		case 6: MoveTo(102); break;
		case 8: MoveTo(104); break;
		default: CatwalkCommon();
		}
		break;

	case 104:    // South Catwalk
		if (CatwalkBlocked()) break;
		switch (Vb)
		{
		case 2: case 13: Region = Null; Door(106, 5); Score(20, 113); break;
		case 5: MoveTo(103); break;
		case 6: MoveTo(105); break;
		default: CatwalkCommon();
		}
		break;

	case 105:    // West Catwalk
		if (CatwalkBlocked()) break;
		switch (Vb)
		{
		case 5: MoveTo(102); break;
		case 7: MoveTo(104); break;
		default: CatwalkCommon();
		}
		break;
	}
}

static void RegulatorSockets(void)
{
	RL(525);
	for (x = 1; x <= 25; x++)
		if (IN29(Socket, x))
		{
			if (x == 22) { Wr("  An "); WrFN(x); Wr(" is in one of the sockets."); Wln(); }
			else { Wr("  A "); WrFN(x); Wr(" is in one of the sockets."); Wln(); }
		}
}

void Room23(void)    // Planetship (Lower Level)
{
	switch (Prm)
	{
	case 106:    // Junction
		switch (Vb)
		{
		case 1: Region = 5; Door(104, 5); if (!IN29(Wear, 20)) RL(492); break;
		case 2: RL(328); break;
		case 3: Door(108, 5); break;
		case 4: case 11: case 13: Door(107, 20); break;
		case 41: case 42: case 43: case 44: case 88: if (N1 == 56) RL(532); break;
		}
		break;

	case 107:    // Conference Room
		switch (Vb)
		{
		case 3: case 13: Door(106, 20); break;
		case 39: if (SET_IN(NounSet, 30) && LOC_HAS(107, 19)) R[19] = 107; break;
		}
		break;

	case 108:    // Reactor Regulators
		switch (Vb)
		{
		case 4: case 13: Door(106, 5); break;
		case 41:
			if (N1 == 55)
			{
				if (!En(65))
				{
					Add(65); LOC_ADD(108, 40); Score(5, 114);
					for (x = 22; x <= 25; x++) R[x] = 108;
					RL(519); RL(520);
				}
				else RL(521);
			}
			break;
		case 53: if (En(65)) RL(522); else RL(523); break;
		case 28: case 24:
			switch (N1)
			{
			case 55: if (En(65)) RegulatorSockets(); else RL(523); break;
			case 40: RegulatorSockets(); break;
			}
			break;
		case 33:
			if ((N2 == 40 || N2 == 55) && En(65))
			{
				i = 0; for (x = 1; x <= 25; x++) if (IN29(Socket, x)) i++;
				if (i < 4)
				{
					if (IN29(Inv, N1))
					{
						if (N1 == 1 || IN_RANGE(N1, 22, 25))
						{
							Wr("The "); WrFN(N1); Wr(" fits perfectly in an open socket."); Wln();
							Van(N1); SET_ADD(Socket, N1); R[N1] = 108;
							i = 0; for (x = 1; x <= 25; x++) if (IN29(Socket, x)) i++;
							if (i > 3 && IN29(Socket, 1) && !IN29(Socket, 25))
							{   // ####  You have won the game!!!!!  ####
								Col(10, 7); Delay(999); Static(); Static(); RS(221); Static(); Static();
								Delay(999); Explode(1); Sound(20); RS(222); Delay(999); RS(223);
								Pause(); Bor(0, 0); Wln(); Score(55, 115);
								Add(66); Prm = 109; SetClear(PStat, 1); PlayerInput(Line);
								Col(10, 15); for (x = 224; x <= 229; x++) RS(x);
								Wln(); Wln(); Pause(); Wln();
								Won();    // <<<-----  END OF GAME !!!
							}
						}
						else { Wr("The "); WrFN(N1); Wr(" won't fit in any of the sockets."); Wln(); }
					}
					else { Wr("First you must have the "); WrFN(N1); Wr("."); Wln(); }
				}
				else RL(526);
			}
			break;
		}
		break;
	}
}
