// NOVA.PAS: Room5 - Room9 (Barre-An, the jungle planet, the pyramid's top level)
#include "nova.h"

static void Zap(void)
{
	for (x = 1; x <= 29; x++) if (R[x] == Prm) Van(x);
}

void Room5(void)    // Barre-An Planet
{
	switch (Prm)
	{
	case 13:    // Crossroads
		switch (Vb)
		{
		case 1: case 11: if (En(7)) RL(22); else { RL(93); MoveTo(8); } break;
		case 2: MoveTo(18); break;
		case 3: MoveTo(16); break;
		case 4: MoveTo(14); break;
		case 28: case 31: if (N1 == 81 || N1 == 88) { Score(5, 70); RB2(5, 6); Pause(); } break;
		case 41: if (N1 == 56) RL(394); break;
		}
		break;

	case 14:    // Dusty Road
		switch (Vb)
		{
		case 3: MoveTo(13); break;
		case 4: RC = 1; MoveTo(15); break;
		}
		break;

	case 15:    // Dusty Road (endless)
		switch (Vb)
		{
		case 3: Zap(); RC--; if (RC < 1) MoveTo(14); else MoveTo(15); break;
		case 4:
			Zap(); RC++; MoveTo(15);
			if (RC == 40) { Wr("You notice something unusual."); Wln(); }
			break;
		case 11: case 13:
			if (RC == 40)
			{
				RL(135); Explode(3); Play(1, 9999, 0); Play(9999, 1, 0);
				Walls(10); Explode(5); TFlag = 4;
				SetClear(Inv, 4);
				SET_ADD(Inv, 1); SET_ADD(Inv, 6); SET_ADD(Inv, 12); SET_ADD(Inv, 17);
				SET_ADD(Inv, 20); SET_ADD(Inv, 26); SET_ADD(Inv, 28); SET_ADD(Inv, 29);
				MoveTo(78);
				T[3] = 120; T[4] = 120; T[5] = 120; SetClear(PStat, 1); MugCon = 79; Bor(0, 0);
			}
			break;
		}
		break;

	case 16:    // Gravel Road
		switch (Vb)
		{
		case 2: case 11: RL(108); MoveTo(20); break;
		case 3: RC = 1; MoveTo(17); break;
		case 4: MoveTo(13); break;
		case 28: case 31: if (N1 == 81) RL(109); break;
		}
		break;

	case 17:    // Gravel Road (endless)
		switch (Vb)
		{
		case 3:
			Zap(); RC++;
			if (RC == 80 && !SetEmpty(Inv, 4)) { MoveTo(17); RS(17); SetClear(Inv, 4); }
			else MoveTo(17);
			break;
		case 4: Zap(); RC--; if (RC < 1) MoveTo(16); else MoveTo(17); break;
		}
		break;

	case 18:    // Paved Road
		switch (Vb)
		{
		case 1: MoveTo(13); break;
		case 2: RC = 1; MoveTo(19); break;
		case 4: case 11: MoveTo(21); break;
		}
		break;

	case 19:    // Paved Road (endless)
		switch (Vb)
		{
		case 1: Zap(); RC--; if (RC < 1) MoveTo(18); else MoveTo(19); break;
		case 2: if (RC == 60) RS(18); else { Zap(); RC++; MoveTo(19); } break;
		}
		break;

	case 20:    // General Store
		switch (Vb)
		{
		case 1: case 13: MoveTo(16); break;
		case 28: if (N1 == 132) RL(88); break;
		case 20: case 26: case 76:
			if ((N1 == 86 || N1 == 87) && LOC_HAS(20, 5)) { RS(14); R[5] = 20; Score(15, 20); }
			break;
		case 39: if (SET_IN(NounSet, 5) || SET_IN(NounSet, 30)) Score(15, 20); break;
		}
		break;

	case 21:    // Gravel Lot
		switch (Vb)
		{
		case 3: case 13: MoveTo(18); break;
		case 1: case 4: case 5: case 6: case 7: case 8: RL(27); break;
		case 2: case 9: case 51: if (N1 == Null || N1 == 62 || N1 == 71) { RL(234); MoveTo(22); } break;
		case 28: case 31: if (N1 == 81) { Score(5, 71); RB(1, 7); Pause(); } break;
		}
		break;

	case 22:    // Top of Ship (Barre-An)
		switch (Vb)
		{
		case 1: RL(235); MoveTo(21); break;
		case 10: case 11: case 51:
			if (N1 == Null || N1 == 62 || N1 == 64 || N1 == 71)
			{
				if (En(23)) { RL(236); MoveTo(2); Score(15, 72); }
				else RL(99);
			}
			break;
		case 24: case 28: case 31:
			switch (N1)
			{
			case 56: if (En(23)) RL(163); else RL(99); break;
			case 81: RB(1, 7); Pause(); break;
			}
			break;
		case 50:
			if (N1 == 78) { if (En(22)) { RL(94); Min(22); } else { RL(95); Add(22); } }
			break;
		case 41:
			if (N1 == 56) { if (En(22)) { RL(96); Add(23); Score(5, 73); } else RL(97); }
			break;
		case 53:
			if (N1 == 56) { if (En(23)) { RL(98); Min(23); } else Say("hatch", "closed"); }
			break;
		case 59: if (N1 == 49 || N1 == 122 || StrEq(VStr, "jump off")) MoveTo(21); break;
		}
		break;
	}
}

void Room6(void)    // Jungle Planet
{
	switch (Prm)
	{
	case 23:    // Deep Pit
		switch (Vb)
		{
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 51:
			RL(217); break;
		case 28: case 31:
			switch (N1) { case 73: RL(218); break; case 81: case 116: RS(80); break; }
			break;
		case 75: if (N1 == 73) MoveTo(34); break;
		}
		break;

	case 24:    // Top of Ship (Jungle planet)
		switch (Vb)
		{
		case 3: RL(235); MoveTo(25); break;
		case 10: case 11: case 51:
			if (N1 == Null || N1 == 62 || N1 == 64 || N1 == 71)
			{
				if (En(23)) { Score(5, 74); RL(236); MoveTo(2); }
				else RL(99);
			}
			break;
		case 24: case 28: if (N1 == 56) { if (En(23)) RL(163); else RL(99); } break;
		case 50:
			if (N1 == 78) { if (En(22)) { RL(94); Min(22); } else { RL(95); Add(22); } }
			break;
		case 41:
			if (N1 == 56) { if (En(22)) { RL(96); Add(23); } else RL(97); }
			break;
		case 53:
			if (N1 == 56) { if (En(23)) { RL(98); Min(23); } else Say("hatch", "closed"); }
			break;
		case 59: if (N1 == 49 || N1 == 122 || StrEq(VStr, "jump off")) MoveTo(25); break;
		}
		break;

	case 25:    // Sandy Clearing
		switch (Vb)
		{
		case 4: case 9: case 51: if (N1 == Null || N1 == 62 || N1 == 71) { RL(234); MoveTo(24); } break;
		case 1: MoveTo(26); Score(5, 75); break;
		case 2: case 3: case 5: case 6: case 7: case 8: RL(200); break;
		}
		break;

	case 26:    // Winding Path 1
		switch (Vb)
		{
		case 2: MoveTo(25); break;
		case 5: RS(65); DEAD(); break;
		case 59:
			if (N1 == 66)
			{
				if (StrEq(VStr, "jump in")) { RL(288); DEAD(); }
				else { RS(66); MoveTo(27); Score(25, 76); }
			}
			break;
		case 1: case 3: case 4: case 6: case 7: case 8: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		case 28: if (N1 == 66) RS(85); break;
		case 35: case 62: if (N1 == 66) { RL(288); DEAD(); } break;
		case 75: case 76: if (N1 == 66) RL(285); break;
		case 83: if (N1 == 66) RL(286); break;
		case 84: if (N1 == 66) { RL(287); DEAD(); } break;
		}
		break;

	case 27:    // Winding Path 2
		switch (Vb)
		{
		case 6: MoveTo(28); break;
		case 8: case 11: RS(65); DEAD(); break;
		case 1: case 2: case 3: case 4: case 5: case 7: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		case 59:
			if (N1 == 66)
			{
				if (StrEq(VStr, "jump in")) { RL(288); DEAD(); }
				else { RS(66); MoveTo(26); }
			}
			break;
		case 28: if (N1 == 66) RS(85); break;
		case 35: case 62: if (N1 == 66) { RL(288); DEAD(); } break;
		case 75: case 76: if (N1 == 66) RL(285); break;
		case 83: if (N1 == 66) RL(286); break;
		case 84: if (N1 == 66) { RL(287); DEAD(); } break;
		}
		break;

	case 28:    // Winding Path 3
		switch (Vb)
		{
		case 7: MoveTo(27); break;
		case 6: MoveTo(29); break;
		case 8: RS(67); DEAD(); break;
		case 1: case 2: case 3: case 4: case 5: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		case 28: if (N1 == 112) RS(86); break;
		case 24: if (N1 == 112) RL(553); break;
		}
		break;

	case 29:    // Winding Path 4
		switch (Vb)
		{
		case 7: MoveTo(28); break;
		case 5:
			if (En(29)) MoveTo(32);
			else { Col(10, 7); RS(72); Col(11, 7); T[12] = 146; Add(29); MoveTo(32); }
			break;
		case 9: case 51: if (N1 == Null || N1 == 112) { RL(201); MoveTo(30); } break;
		case 6: RS(69); Prm = 31; T[11] = 4; break;
		case 8: if (En(35) || En(130)) { RC = 1; MoveTo(59); Score(20, 77); } break;
		case 1: case 2: case 3: case 4: RL(200); break;
		case 28:
			switch (N1)
			{
			case 112: RL(238); break;
			case 120: RL(544); break;
			case 113: if (Md == 1) { RL(314); Add(130); } break;
			}
			break;
		case 59: if (N1 == 120) { RL(541); Prm = 31; T[11] = 4; } break;
		}
		break;
	}
}

void Room7(void)    // Jungle Planet
{
	switch (Prm)
	{
	case 30:    // Top of Tree
		switch (Vb)
		{
		case 9: RL(202); break;
		case 10: case 51: if (N1 == Null || N1 == 112) { RL(203); MoveTo(29); } break;
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: RL(200); break;
		case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28: if (N1 == 68) RS(79); break;
		case 18: case 17: RL(603); break;
		}
		break;

	case 31:    // Quicksand Pit
		switch (Vb)
		{
		case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: RL(206); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		case 39: case 42: case 76:
			if (N1 == 47)
			{
				if (En(78)) RL(549);
				else { T[11] = 0; RS(71); Prm = 29; Score(25, 78); }
			}
			break;
		case 28: switch (N1) { case 47: RL(239); break; case 120: RL(240); break; } break;
		}
		break;

	case 32:    // Winding Path 5
		switch (Vb)
		{
		case 1: MoveTo(33); break;
		case 8:
			MoveTo(29);
			if (En(35) && !En(40)) { RL(314); Add(40); }
			break;
		case 2: case 3: case 4: case 5: case 6: case 7: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		}
		break;

	case 33:    // Winding Path 6
		switch (Vb)
		{
		case 2: MoveTo(32); break;
		case 6:
			if (!En(28)) { RS(68); Add(28); MoveTo(34); } else MoveTo(34);
			break;
		case 1: case 3: case 4: case 5: case 7: case 8: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		}
		break;

	case 34:    // Front of Pyramid
		switch (Vb)
		{
		case 7: MoveTo(33); break;
		case 41: case 42: case 43: case 44: case 54: case 75: case 88:
			if (N1 == 56) RL(220);
			else if (N1 == 126)
			{
				if (HingeCon != Null)
				{
					Wr("The hinged mouth opens, revealing a "); WrFN(HingeCon); Wr(" inside."); Wln();
					Add(31); SetR(HingeCon, 34); Score(25, 79);
				}
				else { RL(226); Add(31); }
			}
			break;
		case 53:
			if (N1 == 56) RL(220);
			else if (N1 == 126)
			{
				RL(227);
				if (HingeCon != Null) SetR(HingeCon, Null);
				Min(31);
			}
			break;
		case 1: case 11:
			if (En(30)) { Score(25, 80); MoveTo(35); }
			else RL(219);
			break;
		case 39:
			if (IN133(NounSet, PyraCon) || (SET_IN(NounSet, 30) && PyraCon == 9)) { RL(225); Min(30); SFlag = 0; }
			else if (IN133(NounSet, HingeCon)) { RL(228); SFlag = 0; }
			break;
		case 2: case 3: case 4: case 5: case 6: case 8: RL(200); break;
		case 9: case 51: if (N1 == 112) RL(204); break;
		case 92: if (N1 == Null || N1 == 49 || N1 == 122) { RL(216); MoveTo(23); } break;
		case 31: if (N1 == 81 || N1 == 89 || N1 == 119) { RB(8, 7); Pause(); } break;
		case 28: case 24:
			switch (N1)
			{
			case 68: RS(87); break;
			case 89: RS(88); break;
			case 81: case 119: RB(8, 7); Pause(); break;
			case 64:
				if (PyraCon == Null) RL(241);
				else { Wr("The small slot contains a "); WrFN(PyraCon); Wr("."); Wln(); }
				break;
			case 56: if (En(30)) RL(244); else RL(245); break;
			case 126:
				if (En(31))
				{
					if (HingeCon == Null) RL(242);
					else { Wr("The open mouth contains a "); WrFN(HingeCon); Wr("."); Wln(); }
				}
				else RL(243);
				break;
			}
			break;
		}
		break;
	}
}

void Room8(void)    // Inside Pyramid (Top Level)
{
	switch (Prm)
	{
	case 35:    // Vestibule
		switch (Vb)
		{
		case 2: case 13: MoveTo(34); break;
		case 10: case 11: case 51:
			if (N1 == Null || N1 == 71)
			{
				if (En(32)) MoveTo(36);
				else { RS(84); Add(32); MoveTo(36); }
			}
			break;
		case 28: if (N1 == 56) { if (En(30)) RL(244); else RL(245); } break;
		}
		break;

	case 36:    // Descending Stairs
		switch (Vb)
		{
		case 10: case 11: case 51: if (N1 == Null || N1 == 71) MoveTo(37); break;
		case 9: case 13: MoveTo(35); break;
		}
		break;

	case 37:    // Halcyon Hall
		switch (Vb)
		{
		case 1: if (T[16] > 1) { RL(278); DEAD(); } else MoveTo(39); break;
		case 3: MoveTo(38); break;
		case 10: case 51: if (N1 == Null || N1 == 71) MoveTo(42); break;
		case 9: case 13: MoveTo(36); break;
		}
		break;

	case 38:    // Departure Room
		switch (Vb)
		{
		case 4: case 13: MoveTo(37); break;
		case 28:
			switch (N1)
			{
			case 73: if (LOC_HAS(38, 10)) { RL(253); RL(252); } else RL(253); break;
			case 10: if (LOC_HAS(38, 10)) RL(254); break;
			case 30: if (LOC_HAS(38, 17)) { SF(); Wr("It's a grey disk!"); Wln(); } break;
			}
			break;
		case 26: if (N1 == 10 && LOC_HAS(38, 10)) RL(255); break;
		case 42: case 44: if (N1 == 10 || N1 == 73) RL(256); break;
		case 75: if (N1 == 73) RL(257); break;
		case 47: case 83: case 84: RL(258); break;
		case 39:
			if (LOC_HAS(38, 10) && SET_IN(NounSet, 10)) { LOC_ADD(38, 17); RL(259); Score(15, 81); SFlag = 0; }
			else if (SET_IN(NounSet, 30) && LOC_HAS(38, 17)) R[17] = 38;
			break;
		}
		break;
	}
}

void Room9(void)    // Inside Pyramid (Top Level)
{
	switch (Prm)
	{
	case 39:    // Holey Hall
		switch (Vb)
		{
		case 2: case 13:
			if (T[15] > 1) { RL(269); MoveTo(37); Score(5, 82); } else MoveTo(37);
			break;
		case 4: case 11:
			if (T[15] > 1) { RS(95); DEAD(); } else MoveTo(40);
			break;
		}
		break;

	case 40:    // Pyramid Lobby
		switch (Vb)
		{
		case 1: case 11:
			if (T[14] > 1) { SetNoun(68, 12); MoveTo(41); }
			break;
		case 3: case 13:
			if (IN29(Inv, 18) || IN29(Inv, 4))
			{
				SET_DEL(Brief, 39); MoveTo(39);
				T[15] = 3; T[16] = 9; Col(10, 7); RS(93);
				for (i = 500; i <= 5500; i++) Sound(Rnd(i) + Rnd(i));
				i = 5500;
				for (j = 1; j <= 7000; j++) Sound(Rnd(i) + Rnd(i));
				for (j = i; j >= 500; j--) Sound(Rnd(j) + Rnd(j));
				NoSound();
			}
			else MoveTo(39);
			break;
		case 42: case 44: case 51: case 75: case 84: case 94:
			if (N1 == 68)
			{
				if (T[14] > 1) RL(246);
				else if (En(33)) { T[14] = 7; RL(261); Walls(8); }
				else { T[14] = 7; Score(20, 33); RS(89); Walls(8); }
			}
			break;
		case 28:
			switch (N1)
			{
			case 56: if (T[14] > 1) RL(370); else RL(92); break;
			case 68: RL(247); break;
			}
			break;
		}
		break;

	case 41:    // Treasure's Keeper
		switch (Vb)
		{
		case 2: case 13:
			if (T[14] > 1) { SetNoun(68, 13); MoveTo(40); }
			else RL(248);
			break;
		case 28: case 22:
			switch (N1)
			{
			case 56: if (T[14] > 1) RL(370); else RL(92); break;
			case 68:
				if (PodumCon == Null) RL(264);
				else { Wr("There is a "); WrFN(PodumCon); Wr(" on the podium."); Wln(); }
				break;
			case 18: if (LOC_HAS(41, 18) && !En(34)) RS(90); break;
			}
			break;
		case 33:
			if (N2 == 68)
				if (Pr == 1)
				{
					if (IN29(Mov, N1))
					{
						if (PodumCon == Null) { Van(N1); R[N1] = 41; PodumCon = N1; RL(266); }
						else RL(268);
					}
					else Crazy();
				}
			break;
		case 75: if (N1 == 68) RL(276); break;
		case 50: if (N1 == 68) { RL(277); Walls(8); T[14] = 7; } break;
		case 39: if (SET_IN(NounSet, 30) && LOC_HAS(41, 18)) R[18] = 41; break;
		}
		break;
	}
}
