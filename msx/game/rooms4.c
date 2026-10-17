// NOVA.PAS: Room15 - Room20 (inner planet, the planetship)
#include "nova.h"

static void MoveObjects(u8 from, u8 to)    // for x:=1 to MMax do if R[x]=from then R[x]:=to
{
	for (x = 1; x <= MMax; x++) if (R[x] == from) R[x] = to;
}

void Room15(void)    // Inner Planet
{
	switch (Prm)
	{
	case 68:    // Decontamination
		switch (Vb)
		{
		case 3: case 13: if (En(45)) Door(65, 20); else RL(331); break;
		case 4: case 11: Door(69, 20); break;
		case 28: case 24: if (N1 == 131) RL(329); break;
		case 42: case 44: case 75:
			if (N1 == 44) { RL(332); Add(45); Play(20, 99, 7); Play(99, 99, 600); Play(99, 20, 7); Score(10, 95); }
			break;
		}
		break;

	case 69:    // Waiting Room
		switch (Vb)
		{
		case 2: case 11: Door(70, 50); break;
		case 3: case 13: Door(68, 20); break;
		case 28: case 24: switch (N1) { case 35: RL(330); break; case 131: RL(329); break; } break;
		}
		break;

	case 70:    // Transporter Booth
		switch (Vb)
		{
		case 1: case 13: Door(69, 50); break;
		case 28: case 24:
			switch (N1) { case 44: RS(140); break; case 93: RL(319); break; case 94: RS(141); break; }
			break;
		case 42: case 44: case 75:
			if (IN_RANGE(N1, 40, 43))
			{
				SET_DEL(Brief, 62); SET_DEL(Brief, 63); SET_DEL(Brief, 78);
				RS(138); Min(45);
				TransportNoise();
				switch (N1)
				{
				case 40: MoveObjects(70, 78); TFlag = 4; MoveTo(78); Score(15, 97); break;
				case 41: MoveObjects(70, 62); TFlag = 2; MoveTo(62); break;
				case 43: MoveObjects(70, 63); TFlag = 2; MoveTo(63); Score(10, 96); break;
				}
			}
			break;
		}
		break;
	}
}

static void Elevator(u8 msg, u8 to, u8 from)
{
	RS(msg); Play(20, 42, 20); Sound(42); Delay(5500); RL(338);
	Play(42, 20, 20); Prm = to; MoveObjects(from, to);
}

void Room16(void)    // Inner Planet
{
	switch (Prm)
	{
	case 73:    // Communications
		switch (Vb)
		{
		case 1: MoveTo(74); break;
		case 4: Door(66, 20); break;
		case 28:
			switch (N1)
			{
			case 48: RL(366); Wr("The dial is set at number "); WrNum(CodeSet); Wr("."); Wln(); break;
			case 115: RL(350); break;
			case 121: RL(367); break;
			case 127: RL(351); break;
			}
			break;
		case 50:
			if (N1 == 48)
			{
				if (Pr > Null)
				{
					if (Code > 0 && Code < 11) { RL(363); CodeSet = (u8)Code; if (Code == 4) Score(10, 98); }
					else RL(362);
				}
				else RL(365);
			}
			break;
		}
		break;

	case 74:    // Computer Station
		switch (Vb)
		{
		case 2: MoveTo(73); break;
		case 28:
			switch (N1)
			{
			case 37: RL(349); break;
			case 38: RL(322); break;
			case 52: RL(37); break;
			case 59: RB2(7, 10); Col(26, 23); RL(364); Score(5, 47); break;
			}
			break;
		case 31: if (N1 == 59) { RB2(7, 10); Col(26, 23); RL(364); Add(47); } break;
		}
		break;

	case 75:    // Elevator 1st Floor
		switch (Vb)
		{
		case 1: Door(67, 5); break;
		case 42: case 44: case 75: Elevator(143, 76, 75); break;
		}
		break;

	case 76:    // Elevator 2nd Floor
		switch (Vb)
		{
		case 1: Score(5, 99); Door(77, 5); break;
		case 42: case 44: case 75: Elevator(144, 75, 76); break;
		}
		break;

	case 77:    // Observatory
		switch (Vb)
		{
		case 2: Door(76, 5); break;
		case 28:
			switch (N1)
			{
			case 34: RL(562); break;
			case 104: if (StrEq(NStr, "eyepiec")) RL(561); else RS(146); break;
			}
			break;
		case 24:
			if (N1 == 104)
			{
				if (IN29(Wear, 20)) { Score(5, 100); RS(147); }
				else { RL(368); RL(369); }
			}
			break;
		}
		break;
	}
}

void Room17(void)    // Planetship
{
	switch (Prm)
	{
	case 78:    // Icy Platform
		switch (Vb)
		{
		case 3: case 11: Door(79, 25); break;
		case 28:
			switch (N1)
			{
			case 54: RL(379); break;
			case 93: RL(380); break;
			case 96: RL(381); break;
			case 103: RL(500); break;
			case 115: RL(350); break;
			case 118: RL(382); break;
			}
			break;
		case 59: if (N1 == 54) { RL(400); DEAD(); } break;
		}
		break;

	case 79:    // Confirmation Port
		switch (Vb)
		{
		case 4: case 13: Door(78, 25); break;
		case 28:
			switch (N1) { case 44: RL(383); break; case 64: RL(384); break; case 116: RL(600); break; }
			break;
		case 44: case 75:
			if (N1 == 44)
			{
				RS(156); Score(10, 50);
				Play(20, 99, 7); Play(99, 99, 600); Play(99, 20, 7);
			}
			break;
		case 33:
			if (N2 == 64)
				if (Pr == 5)
				{
					if (N1 == 17)
					{
						Explode(1); Sound(50); RS(151);
						for (i = 9000; i >= 1; i--)
							for (j = 1; j <= 4; j++) Sound(Rnd(i / 2 + 400) + i);
						NoSound();
						Van(17); LOC_ADD(80, 17); MoveTo(80); Add(7); T[22] = 3; T[26] = 91;
						Delay(900); Sound(70); RL(399); Delay(999); NoSound(); Score(25, 101);
						SetNoun(40, 6);
						SetNoun(82, 14);
						SetNoun(110, 8);
					}
					else RL(137);
				}
			break;
		}
		break;

	case 80:    // Inspection Lobby
		switch (Vb)
		{
		case 4: case 13: Door(81, 65); break;
		case 9: RL(523); break;
		case 28:
			switch (N1)
			{
			case 55: RL(385); break;
			case 86: if (LOC_HAS(80, 17)) RL(395); break;
			case 72: case 116: RS(148); break;
			}
			break;
		case 41: case 42: case 43: case 44: case 88: case 53: if (N1 == 55) RL(405); break;
		}
		break;
	}
}

void Room18(void)    // Planetship
{
	i16 k;
	switch (Prm)
	{
	case 81:    // Circular Corridor
		switch (Vb)
		{
		case 2: MoveTo(82); break;
		case 3: case 11: Door(80, 65); break;
		case 6: MoveTo(88); break;
		case 28: if (N1 == 72) RL(386); break;
		}
		break;

	case 82:    // Circular Corridor
		switch (Vb)
		{
		case 1: MoveTo(81); break;
		case 3: Door(96, 50); break;
		case 4: Door(93, 5); break;
		case 8: MoveTo(83); break;
		case 28: if (N1 == 72 || N1 == 103) RL(387); break;
		}
		break;

	case 83:    // Circular Corridor
		switch (Vb)
		{
		case 2: case 11: Door(95, 25); break;
		case 4: MoveTo(84); break;
		case 5: MoveTo(82); break;
		case 28: if (N1 == 72 || N1 == 115) RL(388); break;
		}
		break;

	case 93:    // Central Elevator
		switch (Vb)
		{
		case 3: case 11: case 13:
			if (Floor == 1) Door(82, 5);
			else if (IN_RANGE(Floor, 2, 5) || IN_RANGE(Floor, 30, 40) || IN_RANGE(Floor, 89, 95) || IN_RANGE(Floor, 170, 230) ||
				IN_RANGE(Floor, 333, 444) || IN_RANGE(Floor, 543, 600) || IN_RANGE(Floor, 793, 812)) RL(413);
			else if (Floor == 480)
			{
				if (En(64))
				{
					Door(98, 5); Delay(999); RS(217); Score(20, 102);
					for (x = 1; x <= 7; x++) { i = Rnd(7000) + 1500; j = Rnd(600) + 400; Play(i, i, j); }
					RL(516);
				}
				else RL(413);
			}
			else { Door(97, 5); Score(5, 103); }
			break;
		case 28: if (N1 == 44 || N1 == 52) RS(188); break;
		case 56:
			Val(Input, &i, &TestC);
			if (TestC == 0)
			{
				if (i > 0 && i < 837)
				{
					if (i != Floor)
					{
						j = i; i = Floor > i ? Floor - i : i - Floor; Floor = j; RL(411); Play(20, 36, 35);
						Sound(37);
						k = i / 4;
						for (x = 1; x <= 200; x++) Delay(k);
						RL(412); Play(36, 20, 35);
					}
					else RL(410);
				}
				else RL(410);
			}
			else Crazy();
			break;
		}
		break;

	case 95:    // Transmission Room
		switch (Vb)
		{
		case 1: case 13: Door(83, 25); break;
		case 28: if (N1 == 38 || N1 == 121) RS(191); break;
		case 42: case 44: case 75:
			if (N1 == 44)
			{
				Score(5, 104); RL(404); Play(1200, 1200, 99);
				if (En(48)) Min(48); else Add(48);
			}
			break;
		}
		break;

	case 96:    // Station Control
		switch (Vb)
		{
		case 4: case 13: Door(82, 50); break;
		case 28:
			switch (N1) { case 38: RS(192); break; case 59: Score(5, 105); RB2(11, 5); Pause(); break; }
			break;
		case 31: if (N1 == 59) { Score(5, 105); RB2(11, 5); Pause(); } break;
		}
		break;
	}
}

void Room19(void)    // Planetship
{
	switch (Prm)
	{
	case 84:    // Circular Corridor
		switch (Vb)
		{
		case 2: case 11: Door(94, 65); break;
		case 3: MoveTo(83); break;
		case 6: MoveTo(85); break;
		case 28: if (N1 == 72 || N1 == 46) RL(389); break;
		}
		break;

	case 85:    // Circular Corridor
		switch (Vb)
		{
		case 1: MoveTo(86); break;
		case 4: case 11: Door(92, 65); break;
		case 7: MoveTo(84); break;
		case 28: if (N1 == 72) RL(390); break;
		}
		break;

	case 86:    // Circular Corridor
		switch (Vb)
		{
		case 2: MoveTo(85); break;
		case 4: case 11:
			if (T[21] > 1) { Score(15, 107); MoveTo(91); if (En(52)) T[23] = 2; }
			else RL(92);
			break;
		case 5: MoveTo(87); break;
		case 28:
			switch (N1)
			{
			case 56: if (T[21] > 1) RL(370); break;
			case 64: RL(335); break;
			case 72: case 69: RL(391); break;
			}
			break;
		case 33:
			if (N2 == 64)
				if (Pr == 5)
				{
					if (N1 == 12) { RL(336); T[21] = 3; DOpen(15); Score(5, 106); }
					else RL(137);
				}
			break;
		}
		break;

	case 87:    // Circular Corridor
		switch (Vb)
		{
		case 1: case 11: Door(89, 65); break;
		case 3: MoveTo(88); break;
		case 8: MoveTo(86); break;
		case 28: if (N1 == 72) RL(392); break;
		}
		break;

	case 88:    // Circular Corridor
		switch (Vb)
		{
		case 1: case 11: Door(90, 65); break;
		case 4: MoveTo(87); break;
		case 7: MoveTo(81); break;
		case 28: if (N1 == 72) RL(392); break;
		}
		break;

	case 89:    // Reactors West
		switch (Vb)
		{
		case 2: case 13: Door(87, 65); break;
		case 3: MoveTo(90); break;
		case 28:
			switch (N1) { case 38: RL(414); break; case 59: RL(401); break; case 128: RS(189); break; }
			break;
		case 31:
			if (N1 == 59)
				switch (ScrnSet)
				{
				case 1: RB2(8, 6); break;      // orange crystal
				case 2: RB2(10, 10); break;    // green crystal
				case 3: RB2(12, 14); break;    // yellow crystal
				case 4:                        // blue crystal (cracked)
					RB2(14, 9);
					x = Wherex(); y = Wherey(); Gotoxy(40, 14); Col(25, 23); Wr("LACITIRC");
					Gotoxy(x, y); Score(20, 108);
					break;
				}
			break;
		case 44: case 75:
			if (N1 == 44)
			{
				RL(402); ScrnSet++; if (ScrnSet > 4) ScrnSet = 1;
				for (x = 1; x <= 30; x++) { i = Rnd(7000) + 999; Play(i, i, 50); Delay(Rnd(15) + 5); }
			}
			else if (N1 == 38 || N1 == 128) RL(403);
			break;
		case 42: if (N1 == 38 || N1 == 128) RL(403); break;
		}
		break;
	}
}

void Room20(void)    // Planetship
{
	switch (Prm)
	{
	case 90:    // Reactors East
		switch (Vb)
		{
		case 2: case 13: Door(88, 65); break;
		case 4: MoveTo(89); break;
		case 28:
			switch (N1) { case 38: RL(393); break; case 59: RL(402); break; case 128: RS(190); break; }
			break;
		case 31: if (N1 == 59) { Score(5, 109); RB2(9, Rnd(14) + 1); } break;
		case 42: case 44: case 75: if (N1 == 38 || N1 == 128) RL(403); break;
		}
		break;

	case 91:    // Emergency Override
		switch (Vb)
		{
		case 3: case 13:
			if (T[25] > 2) { Col(10, 7); RL(424); T[25] = Null; T[24] = Null; T[26] = 14; Add(52); }
			Door(86, 15);
			if (T[24] > 0 && !En(64)) { T[24] = Null; T[26] = 14; Add(52); }
			break;
		case 4: case 11: RL(92); break;
		case 28:
			switch (N1)
			{
			case 37: case 38: case 51: RL(393); break;
			case 44: RL(469); break;
			case 59: RL(468); break;
			case 64: RL(335); break;
			case 69: if (En(51)) RL(426); else if (Md == 1) RL(467); else RS(182); break;
			}
			break;
		case 31: if (N1 == 59) RL(468); break;
		case 24: if (N1 == 69) { if (En(51)) Crazy(); else RL(427); } break;
		case 75: if (N1 == 69) { if (En(51)) Crazy(); else RL(428); } break;
		case 54:
			if (N1 == 69)
			{
				if (En(51)) RL(426);
				else if (N2 == 0 || N2 == 60) RL(601);
				else if (!(N2 == 10 || N2 == 18 || N2 == 29)) RL(602);
				else
				{
					RS(160); LOC_ADD(91, 123); LOC_DEL(91, 44); LOC_DEL(91, 129); T[23] = 15;
					Add(51);
				}
			}
			break;
		case 42: case 44:
			if (N1 == 69 && !En(51)) RL(470);
			else if (N1 == 44) { RL(471); Play(20, 440, 1); Play(440, 440, 500); Play(440, 20, 1); }
			break;
		case 41: case 88: if (N1 == 69) { if (En(51)) Crazy(); else RL(467); } break;
		case 39: if (N1 == 95) RL(442); break;
		case 49:
			if (N2 == 123)
				if (Pr == 1)
				{
					if (N1 == 1)
					{
						if (T[24] > 1 && T[24] < 6) { T[24] = Null; T[25] = 7; RS(181); }
						else RL(429);
					}
					else RL(425);
				}
			break;
		case 33:
			if (N2 == 64)
				if (Pr == 5)
					if (N1 == 12) RL(577);
			break;
		}
		break;

	case 92:    // Climate Control
		switch (Vb)
		{
		case 3: case 13: Door(85, 65); break;
		case 28: case 31: if (N1 == 59) RS(152); break;
		}
		break;
	}
}
