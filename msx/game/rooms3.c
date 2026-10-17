// NOVA.PAS: Room10 - Room14 (pyramid maze and bottom level, the antenna and the inner planet)
#include "nova.h"

// the verbs every shifting room answers the same way
static void ShiftCommon(void)
{
	switch (Vb)
	{
	case 41: case 42: case 43: case 44: case 50: case 55: if (N1 == 116) RL(325); break;
	case 28: if (N1 == 116) RL(547); break;
	}
}

void Room10(void)    // Pyramid Maze
{
	switch (Prm)
	{
	case 42:    // Ladder Room
		if (IN_RANGE(Vb, 1, 13))
		{
			if (Maze)
				switch (Vb) { case 9: case 13: MoveTo(37); break; case 6: MoveTo(43); break; case 3: case 8: MoveTo(42); break; }
			else
				switch (Vb) { case 9: case 13: MoveTo(37); break; }
		}
		else ShiftCommon();
		break;

	case 43:    // Shifting Room NW
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) { if (Vb == 7) MoveTo(42); }
			else switch (Vb) { case 2: MoveTo(44); break; case 8: MoveTo(45); break; }
		}
		else ShiftCommon();
		break;

	case 44:    // Shifting Room W
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) switch (Vb) { case 7: MoveTo(47); break; case 2: MoveTo(45); break; }
			else switch (Vb) { case 1: MoveTo(43); break; case 5: MoveTo(44); break; }
		}
		else ShiftCommon();
		break;

	case 45:    // Shifting Room SW
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) { if (Vb == 1 || Vb == 3) MoveTo(45); }
			else switch (Vb) { case 1: MoveTo(45); break; case 6: MoveTo(43); break; }
		}
		else ShiftCommon();
		break;

	case 46:    // Shifting Room N
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze)
				switch (Vb) { case 1: RS(98); DEAD(); break; case 7: Score(50, 84); MoveTo(50); break; }
			else
				switch (Vb)
				{
				case 1: RS(98); DEAD(); break;
				case 7: Score(50, 84); MoveTo(50); break;
				case 3: MoveTo(48); break;
				case 8: MoveTo(45); break;
				}
		}
		else ShiftCommon();
		break;

	case 47:    // Shifting Room S
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) { if (Vb == 6) MoveTo(44); }
			else switch (Vb) { case 3: MoveTo(49); break; case 1: MoveTo(47); break; case 4: MoveTo(45); break; }
		}
		else ShiftCommon();
		break;

	case 48:    // Shifting Room NE
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) switch (Vb) { case 7: MoveTo(49); break; case 3: case 4: MoveTo(48); break; }
			else switch (Vb) { case 4: MoveTo(46); break; case 1: case 2: MoveTo(48); break; }
		}
		else ShiftCommon();
		break;

	case 49:    // Shifting Room SE
		if (IN_RANGE(Vb, 1, 8))
		{
			if (Maze) { if (Vb == 5) MoveTo(48); }
			else switch (Vb) { case 4: MoveTo(47); break; case 2: MoveTo(45); break; }
		}
		else ShiftCommon();
		break;

	case 50:    // Stairway Room E
		switch (Vb)
		{
		case 6: MoveTo(46); break;
		case 4: case 10: case 13: case 51: MoveTo(51); break;
		case 41: case 42: case 43: case 44: case 50: case 55: if (N1 == 116) RL(325); break;
		}
		break;
	}
}

void Room11(void)    // Inside Pyramid (Bottom Level)
{
	switch (Prm)
	{
	case 51:    // Lower Stairway
		switch (Vb)
		{
		case 3: case 9: case 13: case 51: MoveTo(50); break;
		case 1: MoveTo(52); break;
		}
		break;

	case 52:    // Misty Passage
		switch (Vb)
		{
		case 2: if (T[19] > 1) { RS(129); DEAD(); } else MoveTo(51); break;
		case 4: if (En(35)) { RL(305); DEAD(); } else MoveTo(53); break;
		case 33:
			if (N1 == 11 && IN29(Inv, 11))
				if (Pr == 1 || Pr == 5 || Pr == 7 || Pr == 10)
					if (N2 == 34 || N2 == 49 || N2 == 64)
					{
						if (T[19] > 0) { Score(40, 39); Van(11); LOC_ADD(52, 11); T[19] = 0; RS(127); }
						else { Van(N1); R[N1] = 52; RL(586); }
					}
			break;
		case 39: if (SET_IN(NounSet, 11) && En(39)) RL(307); break;
		case 28: switch (N1) { case 11: RL(312); break; case 64: RL(313); break; } break;
		case 42: case 44: case 55: switch (N1) { case 11: RL(307); break; case 34: RL(308); break; } break;
		}
		break;

	case 53:    // Damp Passage
		switch (Vb)
		{
		case 2: MoveTo(54); if (En(35)) { RS(122); DEAD(); } break;
		case 3: if (T[18] > 1) { RL(305); DEAD(); } else MoveTo(52); break;
		case 59:
			if (N1 == 66)
			{
				RS(125); SET_DEL(Brief, 52); MoveTo(52); T[18] = 0; T[19] = 3;
				Col(10, 7); RL(306); Walls(12); Score(20, 85);
			}
			break;
		}
		break;

	case 54:    // Dark Passage
		switch (Vb)
		{
		case 1: MoveTo(53); break;
		case 2: MoveTo(55); break;
		}
		break;

	case 55:    // Circular Chamber
		switch (Vb)
		{
		case 1:
			SET_DEL(Brief, 54); MoveTo(54);
			if (En(35))
			{
				if (Crawl)
				{
					Col(10, 7); RS(119); RS(120); Pause(); LOC_ADD(53, 66); Score(20, 86);
					SET_DEL(Brief, 53); MoveTo(53); Col(10, 7); RS(121); Walls(12); T[18] = 4;
				}
				else { Col(10, 7); RS(118); DEAD(); }
			}
			break;
		case 2:
			if (Md == 1) RS(230);
			else if (Rnd(2) == 1) { Prm = 58; RL(563); }
			else { RS(100); DEAD(); }
			break;
		case 3: MoveTo(56); if (IN29(Inv, 11)) { RS(102); DEAD(); } break;
		case 4: if (Md == 1) RS(231); else { RS(99); DEAD(); } break;
		case 39:
			if (SET_IN(NounSet, 11)) { RS(101); SFlag = 0; }
			else if (SET_IN(NounSet, 30) && LOC_HAS(55, 11)) R[11] = 55;
			break;
		}
		break;
	}
}

void Room12(void)    // Inside Pyramid (Bottom Level)
{
	switch (Prm)
	{
	case 56:    // The Erusaert
		switch (Vb)
		{
		case 2: case 11: if (En(36)) { Score(5, 88); MoveTo(57); } break;
		case 4: case 13: if (T[17] > 4) { RS(108); DEAD(); } else MoveTo(55); break;
		case 22: case 28:
			switch (N1)
			{
			case 1: if (!En(35)) RL(292); break;
			case 34:
				if (LOC_HAS(56, 1)) RL(291);
				else if (!En(35)) { RS(103); LOC_ADD(56, 1); }
				break;
			case 49: RS(105); break;
			case 56: RL(370); break;
			case 66: RL(295); break;
			case 68: RS(104); break;
			case 110: RL(293); break;
			}
			break;
		case 26: if (N1 == 49 || N1 == 68) RL(295); break;
		case 39:
			if (SET_IN(NounSet, 1))
			{
				if (!En(35)) RL(300);
				else if (T[17] > 4) { RS(113); T[17] = 3; Van(1); SET_ADD(Inv, 1); Score(50, 87); }
			}
			else if (SET_IN(NounSet, 30)) RL(578);
			break;
		case 37: if (!SET_IN(NounSet, 1) && !En(35)) RL(300); break;
		case 35: case 51: case 62: if (N1 == 68) { RS(106); DEAD(); } break;
		case 59: RS(107); DEAD(); break;
		case 41: case 88:
			if (N1 == 68 || N1 == 110)
			{
				if (N2 == 4)
				{
					if (!En(35))
					{
						if (Here(16) && En(37))
						{ RS(117); T[17] = 6; LOC_ADD(56, 1); LOC_ADD(56, 56); Add(35); Min(37); }
						else { RS(111); DEAD(); }
					}
					else RL(273);
				}
				else RL(296);
			}
			break;
		case 33:
			if (N1 == 16)
			{
				if (Pr == 1 || Pr == 10)
				{
					if (N2 == 49 || N2 == 1)
					{
						Van(16); Add(37); R[16] = 56;
						if (!En(35)) { if (Pr == 1) RL(297); else RL(298); }
						else RL(297);
					}
					else if (N2 == 34 && Pr == 10) { Wr("Done."); Wln(); Van(16); SF(); R[16] = 56; }
					else if (N2 == 68) RL(299);
				}
			}
			else if (N2 == 68 || N2 == 110)
			{
				if (Pr == 1) RL(299);
				else if (Pr == 5)
				{
					if (N1 == 4)
					{
						if (!En(35))
						{
							if (Here(16) && En(37))
							{ RS(117); T[17] = 6; LOC_ADD(56, 1); Add(35); Min(37); }
							else { RS(111); DEAD(); }
						}
						else RL(273);
					}
					else RL(296);
				}
			}
			break;
		case 54: case 75: case 76:
			switch (N1)
			{
			case 1: if (LOC_HAS(56, 1)) RL(300); break;
			case 34: RL(304); break;
			}
			break;
		}
		break;
	}
}

static void BoothTransport(u8 score)
{
	RS(138);
	TransportNoise(); TFlag = 3; SET_DEL(Brief, 70);
	if (score) Score(10, 89);
	for (x = 1; x <= MMax; x++) if (R[x] == Prm) R[x] = 70;
	MoveTo(70);
}

static void BoothLook(void)
{
	switch (N1)
	{
	case 37: case 59: RS(133); break;
	case 38: RL(322); break;
	case 93: RL(319); break;
	case 94: RS(134); break;
	case 44: RL(320); break;
	}
}

void Room13(void)    // Inside Pyramid (Bottom Level)
{
	switch (Prm)
	{
	case 57:    // Solar Chamber
		switch (Vb)
		{
		case 1: case 13: MoveTo(56); break;
		case 28:
			switch (N1) { case 56: RL(370); break; case 72: case 82: case 116: RS(149); break; case 109: RS(150); break; }
			break;
		case 44: case 75: if (N1 == 103) { Prm = 58; RS(233); Walls(5); Static(); } break;
		}
		break;

	case 58:    // Eternal Trap
		switch (Vb)
		{
		case 9: case 13: case 51: RL(554); break;
		case 92: RL(555); break;
		}
		break;

	case 59:    // Overgrown Path
		switch (Vb)
		{
		case 5: RC--; if (RC < 1) { RL(309); MoveTo(29); } else RL(310); break;
		case 8: RC++; if (RC > 7) { RL(309); MoveTo(60); } else RL(310); break;
		case 1: case 2: case 3: case 4: case 6: case 7: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		}
		break;

	case 60:    // Antenna Building (Outside)
		switch (Vb)
		{
		case 4: case 11:
			Door(61, 25);
			if (!En(43)) { RS(137); Score(10, 43); }
			break;
		case 5: MoveTo(59); break;
		case 1: case 2: case 3: case 6: case 7: case 8: RL(200); break;
		case 28: switch (N1) { case 96: RL(323); break; case 115: RL(324); break; } break;
		case 41: case 42: case 43: case 44: case 54: case 75: case 88: if (N1 == 56) RL(315); break;
		case 9: case 51: if (N1 == 112) RL(204); else RL(316); break;
		}
		break;

	case 61:    // Monitor Outpost
		switch (Vb)
		{
		case 3: case 13: Door(60, 25); break;
		case 2: case 11: MoveTo(62); break;
		case 28: case 24: case 31:
			switch (N1) { case 37: case 59: RS(133); break; case 38: case 44: RL(322); break; case 93: RL(319); break; case 94: RS(134); break; }
			break;
		case 42: case 44: case 75: if (N1 == 37 || N1 == 38 || N1 == 44) RL(318); break;
		}
		break;

	case 62:    // Booth (Jungle Planet)
		switch (Vb)
		{
		case 1: case 13: MoveTo(61); break;
		case 42: case 44: case 75:
			switch (N1) { case 37: case 38: RL(318); break; case 44: BoothTransport(1); break; }
			break;
		case 28: case 24: case 31: BoothLook(); break;
		}
		break;

	case 63:    // Booth (Ringed Planet)
		switch (Vb)
		{
		case 1: case 13: RL(328); break;
		case 42: case 44: case 75:
			switch (N1) { case 37: case 38: RL(318); break; case 44: BoothTransport(0); break; }
			break;
		case 28: case 24: case 31: BoothLook(); break;
		case 39: if (SET_IN(NounSet, 30) && LOC_HAS(63, 20)) R[20] = 63; break;
		}
		break;
	}
}

void Room14(void)    // Inner Planet
{
	switch (Prm)
	{
	case 64:    // Lobby
		switch (Vb)
		{
		case 1: case 13: DOpen(99); RS(139); DEAD(); break;
		case 2: case 11: MoveTo(65); break;
		case 28:
			switch (N1)
			{
			case 35: RL(330); break;
			case 49: case 122: RL(334); break;
			case 72: case 116: Score(5, 90); RS(148); break;
			case 81: RB(5, 11); break;
			case 86: RL(333); break;
			case 127: RL(351); break;
			}
			break;
		case 31: if (N1 == 81) RB(5, 11); break;
		case 20: case 24: if (N1 == 127) RS(142); break;
		case 54: if (N1 == 127) { if (Md == 3) { RL(537); DEAD(); } else RL(538); } break;
		}
		break;

	case 65:    // North Corridor
		switch (Vb)
		{
		case 1: MoveTo(64); break;
		case 2: MoveTo(66); break;
		case 4: Door(68, 20); break;
		case 28: if (N1 == 56) RL(92); break;
		}
		break;

	case 66:    // Central Corridor
		switch (Vb)
		{
		case 1: MoveTo(65); break;
		case 2: MoveTo(67); break;
		case 3: if (T[20] > 1) { Score(15, 91); MoveTo(73); } else RL(92); break;
		case 4: MoveTo(72); break;
		case 28:
			switch (N1) { case 56: if (T[20] > 1) RL(370); else RL(92); break; case 64: RL(335); break; }
			break;
		case 33:
			if (N2 == 64)
				if (Pr == 5)
				{
					if (N1 == 12) { RL(336); T[20] = 3; DOpen(20); Score(5, 92); }
					else RL(137);
				}
			break;
		}
		break;

	case 67:    // South Corridor
		switch (Vb)
		{
		case 1: MoveTo(66); break;
		case 2: case 11: Door(75, 5); break;
		case 4: MoveTo(71); break;
		}
		break;

	case 71:    // Monitor Station
		switch (Vb)
		{
		case 3: MoveTo(67); break;
		case 28:
			switch (N1) { case 59: if (CodeSet == 4) RL(353); else RL(355); break; case 116: RL(354); break; }
			break;
		case 31:
			if (N1 == 59) { if (CodeSet == 4) { RB2(11, 14); Pause(); } else RL(355); }
			break;
		}
		break;

	case 72:    // Reactor Control
		switch (Vb)
		{
		case 3: MoveTo(66); break;
		case 28: case 24:
			switch (N1)
			{
			case 128: RL(560); break;
			case 37: case 38: RL(339); break;
			case 46: if (Md != Null) { Score(5, 93); RL(343); } else RS(145); break;
			case 55:
				if (En(46))
				{
					if (RobotCon != Null) { Wr("The robot's panel contains a "); WrFN(RobotCon); Wr("."); Wln(); }
					else RL(346);
				}
				else RL(344);
				break;
			}
			break;
		case 33:
			if (N2 == 55)
				if (Pr == 5)
				{
					if (En(46))
					{
						if (RobotCon == Null)
						{
							if (N1 == 3 || N1 == 4 || N1 == 8 || N1 == 12 || N1 == 17)
							{ Van(N1); RL(347); R[N1] = 72; RobotCon = N1; }
							else RL(137);
						}
						else RL(348);
					}
					else RL(344);
				}
			break;
		case 41:
			if (N1 == 55 || N1 == 46)
			{
				if (RobotCon == Null) RL(558);
				else
				{
					SetR(RobotCon, 72); Add(46); Score(25, 94);
					Wr("The panel slides open, revealing a "); WrFN(RobotCon); Wr(" inside."); Wln();
				}
			}
			break;
		case 53: if (N1 == 55) { Van(RobotCon); Min(46); RL(344); } break;
		case 42: case 44:
			switch (N1)
			{
			case 37: case 38: case 44:
				RL(340); Walls(9); Gotoxy(43, Wherey() - 1); Col(28, 31);
				Wr("BOOM!"); Wln(); Explode(32); DEAD();
				break;
			case 46:
				if (StrEq(VStr, "turn on") || StrEq(VStr, "activat") || StrEq(VStr, "turn off")) RL(341);
				else if (!En(94) && !En(93)) { RL(342); RL(559); }
				else RL(342);
				break;
			}
			break;
		}
		break;
	}
}
