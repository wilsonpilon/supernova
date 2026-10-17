// NOVA.PAS: Room1 - Room4 (the player's ship, the tavern)
#include "nova.h"

void Room1(void)    // The Player's Ship
{
	u8 n;
	switch (Prm)
	{
	case 1:    // Bridge
		switch (Vb)
		{
		case 2: case 13: Door(2, 50); break;
		case 11: RL(199); break;
		case 22: case 24: case 28:
			switch (N1)
			{
			case 37: case 38: case 55: case 44: if (Md != Null) RL(534); else RB(3, 11); break;
			case 103: RL(317); break;
			case 35: if (En(6)) RL(542); else RL(543); break;
			case 59:
				switch (Region) { case 1: RS(46); break; case 2: RS(63); break; case 4: RS(T[9] + 46); break; }
				break;
			}
			break;
		case 35:
			if (N1 == 16 || N1 == 35)
			{
				if (Here(16) && En(6))
				{
					if (!En(4))
					{
						Add(7); Sound(440); RS(19); T[6] = 4;
						n = Rnd(30) + 9;
						for (x = 1; x <= n; x++) { y = Rnd(6000) + 60; Play(y, y, 70); Delay(9); }
						Score(10, 67);
					}
					else
					{
						Add(7); RL(149);
						n = Rnd(9) + 9;
						for (x = 1; x <= n; x++) { y = Rnd(9999); Play(y, y, 30); Delay(55); }
					}
				}
				else RL(141);
			}
			break;
		case 56:
			if (En(7) && En(6))
			{
				if (T[6] > 0)
				{
					if (StrEq(Input, Identity)) { RS(21); T[6] = Null; Score(15, 4); Play(400, 400, 200); }
					else RL(142);
				}
				else if (StrEq(Input, "r2/20-8 d2/vega"))
				{
					if (!(Region == 2 || Region == 4)) { RS(28); Add(26); Min(27); Play(600, 600, 99); }
					else RL(154);
				}
				else { Min(26); RS(39); Min(27); Play(600, 600, 99); }
			}
			else RL(179);
			break;
		case 44:
			if (En(7) && En(6))
				switch (N1)
				{
				case 43:    // purple
					Play(750, 750, 200);
					if (Region == 4)
					{
						if (T[9] == 11)
						{
							Score(5, 68); Region = 2; T[13] = 3; TFlag = 2;
							SetNoun(126, 1);
							RB(9, 11); Explode(30);
						}
						else RS(64);
					}
					else RL(198);
					break;
				case 40:    // cyan
					Play(900, 900, 200);
					if (En(26)) { RS(37); Add(27); }
					else RL(195);
					break;
				case 41:    // tan
					Play(600, 600, 200);
					if (En(27))
					{
						if (!En(22) && !En(23))
						{
							Col(28, 23); RL(180); Col(11, 7);
							RS(38); T[8] = 7;
							for (x = 1; x <= 6; x++)
								for (y = 30; y <= 200; y++) { Sound(y); Delay(5); }
							NoSound();
						}
						else { RS(40); Play(500, 999, 3); Min(26); Min(27); }
					}
					else if (En(26)) RL(184);
					else RL(185);
					break;
				}
			else RL(179);
			break;
		case 39:
			if (SET_IN(NounSet, 16) && En(6))
			{
				if (En(7)) RL(575);
				else { Min(6); RL(145); SFlag = 0; }
			}
			break;
		}
		break;
	}
}

void Room2(void)    // The Player's Ship
{
	switch (Prm)
	{
	case 2:    // Forward Corridor
		switch (Vb)
		{
		case 9: case 13: case 51:
			if (N1 == 0 || N1 == 62 || N1 == 64 || N1 == 71)
			{
				if (En(23))
				{
					RL(237);
					switch (Region) { case 1: MoveTo(22); break; case 2: MoveTo(24); break; }
				}
				else RL(99);
			}
			break;
		case 1: case 11: Door(1, 50); break;
		case 2: MoveTo(3); break;
		case 4: MoveTo(4); break;
		case 24: case 28:
			if (StrEq(NStr, "hatch")) { if (En(23)) RL(163); else RL(99); }
			break;
		case 50:
			if (N1 == 78)
			{
				if (En(22)) { RL(160); Min(22); }
				else
				{
					RL(161); Add(22);
					if (T[8] > 0) { RS(40); Play(500, 999, 3); T[8] = 0; Min(27); }
				}
			}
			break;
		case 41:
			if (StrEq(NStr, "hatch"))
			{
				if (Region != 4)
				{
					if (En(22))
					{
						RL(96); Add(23);
						if (!En(44) && Region == 2)
						{
							Add(44); RL(327);
							for (i = 1; i <= 400; i++) Sound(Rnd(3500) + 999);
							for (i = 7500; i >= 500; i--) { Sound(Rnd(i) + i); Sound(Rnd(3500) + 2000); }
							for (i = 1; i <= 300; i++) Sound(Rnd(3500) + 2500);
							NoSound();
						}
					}
					else RL(97);
				}
				else { RS(45); DEAD(); }
			}
			break;
		case 53:
			if (N1 == 56) { if (En(23)) { RL(98); Min(23); } else Say("hatch", "closed"); }
			break;
		}
		break;

	case 3:    // Rear Corridor
		switch (Vb)
		{
		case 1: case 13: MoveTo(2); break;
		case 3: MoveTo(6); break;
		case 4: MoveTo(7); break;
		case 28: case 31: if (N1 == 55 || N1 == 81) { RB2(4, 12); Pause(); } break;
		case 24: case 41: case 76:
			if (N1 == 55) { if (Md == 1 || Md == 4) RL(533); else { RS(22); DEAD(); } }
			break;
		}
		break;
	}
}

void Room3(void)    // The Player's Ship
{
	switch (Prm)
	{
	case 4:    // Ship's Lab
		switch (Vb)
		{
		case 3: case 13: MoveTo(2); break;
		case 28: case 22:
			switch (N1)
			{
			case 90: RS(23); break;
			case 52: RS(26); break;
			case 38: RS(27); break;
			case 55: RS(31); break;
			case 81: case 119: RB(10, 14); Pause(); break;
			case 64:
				if (NicheCon != Null) { Wr("There is a "); WrFN(NicheCon); Wr(" sitting on the niche."); Wln(); }
				else RL(152);
				break;
			}
			break;
		case 31: if (N1 == 81 || N1 == 119) { RB(10, 14); Pause(); } break;
		case 41: case 42: case 43: case 44: case 88: if (N1 == 55) RL(174); break;
		case 33:
			if (N2 == 64)
			{
				if (N1 == 3 || N1 == 4 || N1 == 8 || N1 == 9 || N1 == 12 || N1 == 17 || N1 == 29)
				{
					if (Pr == 1 || Pr == 5 || Pr == 10)
					{
						if (NicheCon == Null) { Van(N1); SetR(N1, 4); NicheCon = N1; RL(147); }
						else RL(148);
					}
					else Crazy();
				}
				else RL(146);
			}
			break;
		case 54: if (N1 == 38 || N1 == 90) { RL(321); Serum = 1; } break;
		case 56:
			Serum++;
			if (Serum < 3)
			{
				if (NicheCon == Null) RS(24);
				else if (NicheCon != 29)
				{ Wr("A small amount of serum drips onto the "); WrFN(NicheCon); Wr(" and dries up."); Wln(); }
				else if (MugCon == Null)
				{
					MugCon = 92; RS(25); LOC_ADD(4, 92);
					if (StrEq(Input, "6")) Add(42); else Min(42);
				}
				else if (MugCon == 79 || MugCon == 92 || MugCon == 99)
				{ Wr("The serum mixes with the "); WrFN(MugCon); Wr(" inside the mug."); Wln(); }
				else
				{ Wr("The serum drips on the "); WrFN(MugCon); Wr(" inside the mug and quickly evaporates."); Wln(); }
			}
			else RS(135);
			break;
		}
		break;

	case 5:    // Sanitary Facilities
		switch (Vb)
		{
		case 2: case 13: MoveTo(6); break;
		case 22: case 28: if (N1 == 98) RL(122); break;
		}
		break;

	case 6:    // Sleep Chamber
		switch (Vb)
		{
		case 1: case 11: MoveTo(5); break;
		case 4: case 13: MoveTo(3); break;
		case 28: switch (N1) { case 80: RL(159); break; case 69: RS(29); break; } break;
		case 51: if (N1 == 69) Vb = 35; break;
		}
		break;

	case 7:    // Cargo Area
		switch (Vb)
		{
		case 3: case 13: MoveTo(3); break;
		case 22: case 28: if (N1 == 32) RL(155); break;
		case 24: if (N1 == 32) RL(156); break;
		case 41: case 53: if (N1 == 32) RL(157); break;
		case 39: if (SET_IN(NounSet, 32)) RL(158); break;
		}
		break;
	}
}

void Room4(void)    // The Tavern
{
	switch (Prm)
	{
	case 8:    // Tavern
		switch (Vb)
		{
		case 2: case 13:
			if (En(7)) RL(22); else { RL(93); MoveTo(13); }
			break;
		case 3: case 9: case 11: case 51:
			if (En(7)) RL(22);
			else if (N1 == Null || N1 == 71) { RL(283); MoveTo(9); }
			else if (N1 == 126) RL(473);
			break;
		case 31: if (N1 == 81 || N1 == 119) RB(2, 9); break;
		case 35:
			switch (N1)
			{
			case 83:
				if (IN_RANGE(T[1], 4, 7) || IN_RANGE(T[1], 11, 17)) RS(8);
				else { Add(19); Add(7); RL(30); }
				break;
			case 35: case 84: case 85:
				Add(7); RL(30);
				if (N1 == 35 || N1 == 84) Add(2); else Min(2);
				break;
			}
			break;
		case 65:
			if (N1 == 124)
			{
				if (En(2))
				{
					if (IN_RANGE(T[1], 5, 7) || IN_RANGE(T[1], 11, 18))
					{
						if (Md == 1 || En(18))
						{
							RL(31); Score(20, 3);
							if (!En(18)) { Add(18); T[1] = 8; }
						}
						else RL(32);
					}
					else RL(41);
				}
				else RL(33);
			}
			break;
		case 28:
			switch (N1) { case 56: RL(371); break; case 81: case 119: RB(2, 9); break; case 111: RL(44); break; }
			break;
		case 41: if (N1 == 56) RL(394); break;
		case 39: if (SET_IN(NounSet, 29) && !En(21)) RL(86); break;
		case 54: case 93: if (N1 == 111) { RS(32); DEAD(); } break;
		case 42: case 44: case 75: if (N1 == 111) RL(474); break;
		case 59: if (N1 == 126) RL(473); break;
		case 20: if (N1 == 126) RL(178); break;
		}
		break;

	case 9:    // West Hallway
		switch (Vb)
		{
		case 1: case 2: RL(89); break;
		case 3: case 11: MoveTo(10); break;
		case 28: if (N1 == 56) RL(92); break;
		case 4: case 10: case 13: case 51:
			if (N1 == Null || N1 == 71) { RL(284); MoveTo(8); }
			break;
		case 54: if (N1 == 56 && T[1] < 1) RS(13); else RL(123); break;
		case 41: case 88:
			if (N1 == 56) switch (N2) { case 0: RL(107); break; case 3: RL(101); break; default: RL(137); }
			break;
		}
		break;

	case 10:    // Central Hallway
		switch (Vb)
		{
		case 1: RL(89); break;
		case 4: case 13: MoveTo(9); break;
		case 3: MoveTo(11); break;
		case 28: if (N1 == 56) { if (En(11)) RL(370); else RL(92); } break;
		case 2: case 11:
			if (En(11)) { RL(90); Score(5, 69); MoveTo(12); Min(11); }
			else RL(89);
			break;
		case 41: case 88:
			if (N1 == 56) switch (N2) { case 0: RL(107); break; case 3: break; default: RL(137); }
			break;
		case 54: if (N1 == 56) RL(123); break;
		}
		break;

	case 11:    // East Hallway
		switch (Vb)
		{
		case 1: case 2: RL(89); break;
		case 4: case 13: MoveTo(10); break;
		case 28: if (N1 == 56) RL(92); break;
		case 41: case 88:
			if (N1 == 56) switch (N2) { case 0: RL(107); break; case 3: RL(101); break; default: RL(137); }
			break;
		case 54: if (N1 == 56) RL(123); break;
		case 53: if (N1 == 56) { RL(92); Min(11); } break;
		}
		break;

	case 12:    // Suite #4
		switch (Vb)
		{
		case 1: case 13:
			if (En(7)) RL(22);
			else if (En(11)) { RL(91); MoveTo(10); Min(11); }
			else RL(92);
			break;
		case 41: case 88: if (N1 == 56) { RL(104); Add(11); } break;
		case 22: case 28: case 76:
			switch (N1)
			{
			case 56: if (En(11)) RL(370); else RL(92); break;
			case 80: if (Md != Null) RL(535); else RL(536); break;
			case 87: if (LOC_HAS(12, 7)) { RL(106); R[7] = 12; Score(5, 24); } break;
			case 98: RL(121); break;
			}
			break;
		case 53: if (N1 == 56) { RL(92); Min(11); } break;
		case 39: if (SET_IN(NounSet, 7) || SET_IN(NounSet, 30)) Score(5, 24); break;
		}
		break;
	}
}
