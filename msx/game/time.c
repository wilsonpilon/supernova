// ADPAR.PAS time events (overlay)
#include "nova.h"

void Time1(void)
{
	Tic++; Min(128); Col(13, 15);
	for (x = 1; x <= TMax; x++) T[x]--;
	if (T[2] == 1) { SET_DEL(PStat, 5); RL(1); }
	switch (T[3])    // Hunger
	{
	case 25: SET_ADD(PStat, 2); RL(2); break;
	case 12: RL(3); break;
	case 4: Bor(4, 7); RL(4); break;
	case 1: RL(124); DEAD(); break;
	}
	switch (T[4])    // Thirst
	{
	case 22: SET_ADD(PStat, 6); RL(5); break;
	case 11: RL(6); break;
	case 4: Bor(4, 7); RL(7); break;
	case 1: RL(125); DEAD(); break;
	}
	switch (T[5])    // Sleep
	{
	case 32: SET_ADD(PStat, 5); RL(8); break;
	case 14: RL(9); break;
	case 5: Bor(4, 7); RL(10); break;
	case 1: RL(126); DEAD(); break;
	default:
		if (T[5] >= 2 && T[5] <= 13)
		{
			x = Rnd(29) + 1;
			if (IN29(Inv, x) && !IN29(Wear, x))
			{
				Van(x); R[x] = Prm;
				Wr("A bout of weariness causes you to loose your grip on the ");
				WrFN(x); Wr("!"); Wln();
			}
		}
	}
	switch (T[29])   // Laser Injury
	{
	case 9: RS(214); break;
	case 4: RL(507); Bor(4, 7); break;
	case 1: RS(215); DEAD(); break;
	default:
		if (T[29] >= 2 && T[29] <= 11)
		{
			do x = Rnd(27) + 1; while (!(IN_RANGE(x, 1, 12) || IN_RANGE(x, 16, 20) || IN_RANGE(x, 22, 26) || x == 28));
			if (IN29(Inv, x) && !IN29(Wear, x))
			{
				Van(x); R[x] = Prm;
				if (Rnd(2) == 0)
				{
					Wr("A sudden stab of pain shoots up your side, you drop the ");
					WrFN(x); Wr("."); Wln();
				}
				else
				{
					Wr("The "); WrFN(x); Wr(" falls from your grip as you almost collapse from the"); Wln();
					Wr("extreme pain."); Wln();
				}
			}
		}
	}
	switch (T[12])   // Sickness
	{
	case 120: case 99: case 83: case 55: RL(207); break;
	case 65: SET_ADD(PStat, 3); RL(208); break;
	case 47: RL(209); break;
	case 30: RS(73); break;
	case 15: RS(74); break;
	case 4: Bor(4, 7); RS(75); break;
	case 1: RS(76); DEAD(); break;
	default:
		if (T[12] >= 2 && T[12] <= 29)
			if (Rnd(25) == 0 && !SetEmpty(Inv, 4) && !En(125))
			{
				RS(232);
				for (x = 1; x <= 29; x++)
					if (IN29(Inv, x) && !IN29(Wear, x)) { Van(x); R[x] = Prm; }
			}
	}
	Col(10, 7);
	if (IN_RANGE(Prm, 81, 88) && T[30] < 1 && Rnd(4) == 0)
	{
		if (IN29(Wear, 28))
		{
			T[30] = 9; RS(153 + Prm);
			for (i = 999; i <= 2300; i++) Sound(Rnd(i * 3) + i);
			for (i = 3000; i >= 20; i--) Sound(Rnd(i * 4) + i * 2);
			NoSound();
		}
		else
		{
			Wr("A small droid appears from the ");
			switch (Prm)
			{
			case 81: Wr("south"); break;     case 82: Wr("southwest"); break;
			case 83: Wr("west"); break;      case 84: Wr("northwest"); break;
			case 85: Wr("north"); break;     case 86: Wr("northeast"); break;
			case 87: Wr("east"); break;      case 88: Wr("southeast"); break;
			}
			Wr(" section of the corridor and flies"); Wln();
			RS(242); RS(243);
			for (i = 20; i <= 3000; i++) Sound(Rnd(i * 3) + i);
			NoSound();
			Delay(1500); DEAD();
		}
	}
}

void Time2A(void)
{
	Col(10, 7);   // Pre-Jungle Planet
	switch (T[1])
	{
	case 19: MC(1, 13, 13, 0); break;
	case 18: MC(1, 8, 8, 1); MC(1, 13, 8, 2); break;
	case 17: if (En(19)) { RS(9); T[1] = 11; } break;
	case 10: MC(1, 8, 9, 3); break;
	case 9: MC(1, 9, 0, 4); T[1] = Null; break;
	case 4: if (Prm == 8) { T[1] = 11; RS(5); } break;
	default:
		if (T[1] >= 11 && T[1] <= 16) { if (!En(19) && Prm == 8 && Rnd(2) == 1) RL(0); }
		else if (T[1] >= 5 && T[1] <= 7)
			if (En(2) && En(3)) { RB2(5 - (T[1] - 3), 10); Col(10, 7); }
	}
	if (T[7] == 1) { RS(35); DEAD(); }
	if (T[6] == 2 && En(7)) RL(140);
	if (T[6] == 1 && En(7)) { RS(20); DEAD(); }
	switch (T[8])    // Lift-off countdown
	{
	case 5: RL(181); break;
	case 4: RL(182); break;
	case 3: RL(191); break;
	case 2: RL(192); break;
	case 1:
		if (En(10))
		{
			Min(10); Min(26); Min(27); Explode(32);
			Sound(20); Bor(0, 0); Score(10, 122);
			SetNoun(84, 0);
			RB(7, 10); Delay(9999); Pause(); Col(10, 7); RS(43);
			SET_DEL(PStat, 2); SET_DEL(PStat, 5); SET_DEL(PStat, 6);
			Region = 4; T[3] = 150; T[4] = 125; T[5] = 230; T[8] = 0; T[9] = 1; T[10] = 5;
		}
		else { RS(44); Explode(32); DEAD(); }
		break;
	}
	if (T[9] < 1) T[9] = 15;
	if (Region == 4 && Prm == 1 && T[9] == 12) RS(62);
	if (IN_RANGE(T[10], 1, 4)) RL(194);
	if (Tic > 3 && Prm == 8 && !En(9) && !En(7)) { RS(6); Add(9); }
	else if (Tic > 5 && Prm == 8 && En(9) && !En(16) && !En(7)) { RS(7); Add(16); }
	switch (Prm)
	{
	case 1:
		if (Rnd(2) == 0 && T[9] > 12 && T[9] < 15 && Region == 4 && !En(129)) { RL(593); RL(594); Add(129); }
		else if (Rnd(20) == 0 && Region == 4) RL(592);
		break;
	case 7: if (Rnd(5) == 0) RL(595); break;
	case 8: if (Rnd(3) == 1) RL(24); else if (Rnd(3) == 1) RL(178); break;
	case 15: case 17: case 19:
		switch (Rnd(60))
		{
		case 1: RL(596); break;
		case 2: RL(597); break;
		case 3: case 4: if (RC > 40) { if (T[4] > 10) T[4] = 10; RL(598); } break;
		}
		break;
	case 20: if (Rnd(4) == 1) RL(25); break;
	}
	if (Prm == SinkRm) { RL(85); LOC_ADD(Prm, 79); }
}

void Time2B(void)
{
	Col(10, 7);   // Jungle Planet
	Maze = !Maze;
	if (IN_RANGE(Prm, 42, 49)) { Wr("Some of the walls shift positions."); Wln(); Walls(4); }
	if (Prm == 7) { if (Rnd(5) == 0) RL(595); }
	else if (IN_RANGE(Prm, 26, 29) || IN_RANGE(Prm, 32, 34) || Prm == 59 || Prm == 60)
	{
		switch (Rnd(40))
		{
		case 0: case 1: RL(210); break;
		case 2: case 3: RL(211); break;
		case 4: RS(77); break;
		case 5: case 6: RS(78); break;
		case 7: case 8: RL(212); break;
		case 9: RL(213); break;
		case 10: RL(214); break;
		case 11: RL(215); Play(6000, 6001, 20); break;
		}
	}
	else if (IN_RANGE(Prm, 42, 50)) { if (Rnd(7) == 0) RL(280); }
	if (Prm == 28 && Rnd(2) == 0) RL(233);
	if (Prm == SinkRm) { RL(85); LOC_ADD(Prm, 79); }
	if (T[11] == 2) RL(205);
	if (T[11] == 1) { RS(70); DEAD(); }
	if (T[13] == 1)
	{
		RB(11, 10);
		for (x = 1; x <= 3; x++) { Tune(4, 2, 200); Delay(99); Tune(4, 5, 200); Delay(99); }
		Pause();
	}
	if (T[14] == 2 && (Prm == 40 || Prm == 41)) RL(251);
	if (T[14] == 1)
		switch (Prm)
		{
		case 40: RL(249); Walls(8); break;
		case 41: RL(250); Walls(8); break;
		}
	if (T[15] == 1 && Prm == 39) { RS(94); DEAD(); }
	if (T[17] == 4) { RS(109); DEAD(); }
	if (T[17] == 1) { Add(36); if (Prm == 56) { RS(110); Walls(8); } }
	if (T[18] == 2) { RS(123); Walls(12); }
	if (T[18] == 1 && Prm == 53) { RS(124); Walls(12); DEAD(); }
	if (T[19] == 1) { RS(128); Walls(12); DEAD(); }
}

void Time2C(void)
{
	Col(11, 7);   // Inner Planet
	if (T[20] == 1)
		switch (Prm)
		{
		case 73: DClose(20); break;
		case 66: RL(337); Play(105, 50, 20); break;
		}
	Col(10, 7);
	if (Prm == 74 && !En(47)) { Wr("There is something flashing on the computer's screen."); Wln(); }
	if (Prm == 73 && CodeSet != 4)
	{
		Wr("There's an alarm sound coming over the radio."); Wln();
		for (x = 1; x <= 23; x++)
		{
			for (i = 450; i <= 999; i++) Sound(i);
			for (i = 999; i >= 450; i--) Sound(i);
		}
		NoSound();
	}
	switch (Rnd(50))
	{
	case 1: case 2: case 3: if (Here(38)) RL(588); break;
	case 4: case 5: RL(589); Explode(3); break;
	case 6: RS(244); for (x = 1; x <= 7; x++) Static(); break;
	default: if ((Prm == 64 || Prm == 73) && Rnd(9) == 0) { RL(590); RL(591); }
	}
}

static u8 Warn(i16 message, i16 ifTime, i16 said)
{
	u8 r = 0;
	if (!En((u8)said) && ifTime >= T[26])
	{
		if (said != 59) { Static(); RS(message); Static(); }
		else if (Prm > 99) { Static(); RS(message); Static(); }
		if (said == 59 && Prm < 100) { }
		else { r = 1; Add((u8)said); }
	}
	return r;
}

void Time2D(void)
{
	Col(11, 7);   // Planetship
	for (x = 0; x <= 2; x++) if (T[23 + x] > 2) Score(20, 118 + x);   // 60 total points
	if (En(64)) Score(10, 121);
	if (T[21] == 1)
		switch (Prm)
		{
		case 91: DClose(20); break;
		case 86: RL(337); Play(110, 50, 15); break;
		}
	Col(10, 7);
	if (Prm == 95 && !En(48))
	{
		Wr("A loud siren is sounding off..."); Wln();
		Play(300, 530, 6); Delay(200); Play(300, 530, 6);
	}
	if (Prm == 91 && !En(53))
	{
		Wr("An alarm is ringing..."); Wln();
		for (x = 1; x <= 23; x++) { Sound(400); Delay(99); Sound(940); Delay(60); NoSound(); }
	}
	if (T[22] == 1)
	{
		Wr("The door slides open..."); Play(50, 60, 65);
		Wr("then closes."); Wln(); Play(60, 50, 60);
		if (En(50)) RS(153);
		else if (SetEmpty(Inv, 4)) { RS(247); RS(248); }
		else { RS(154); RS(155); SetClear(Inv, 4); }
		Delay(2500); Wr("The door slides open..."); Play(50, 60, 65);
		Wr("then closes."); Wln(); Play(60, 50, 65);
	}
	switch (T[23])
	{
	case 13: if (Prm == 91) RL(438); MC(2, 91, 91, 161); Add(53); break;
	case 12: MC(2, 91, 91, 162); break;
	case 11:
		if (Prm == 86 || Prm == 91) DOpen(15);
		MC(2, 91, 86, 163); MC(2, 86, 86, 164);
		if (Prm == 86 || Prm == 91) DClose(15);
		break;
	case 10: MC(2, 86, 87, 165); MC(2, 87, 87, 166); break;
	case 9:
		if (Prm == 87 || Prm == 89) DOpen(65);
		MC(2, 87, 89, 167); MC(2, 89, 89, 168);
		if (Prm == 87 || Prm == 89) DClose(65);
		break;
	case 7: case 8: if (Prm == 89) { ScrnSet = 4; RL(417); } break;
	case 6:
		if (Prm == 87 || Prm == 89) DOpen(65);
		MC(2, 89, 87, 169); MC(2, 87, 87, 170);
		if (Prm == 87 || Prm == 89) DClose(65);
		break;
	case 5: MC(2, 87, 86, 171); MC(2, 86, 86, 172); break;
	case 4:
		if (T[21] < 2)
		{
			MC(2, 86, 91, 173); if (Prm == 86 || Prm == 91) DOpen(15);
			if (Prm == 86) RL(418);
			MC(2, 91, 91, 174); if (Prm == 86 || Prm == 91) DClose(15);
		}
		else if (Prm == 86) { RL(418); MC(2, 0, 91, 0); }
		break;
	case 1:
		if (Prm == 91) { RL(419); Van(123); DClose(55); } else Van(123);
		break;
	}
	if (T[23] > 1 && T[23] < 4 && Prm == 91) { T[24] = 7; T[23] = Null; }
	switch (T[24])
	{
	case 7: if (Prm == 91) RS(175); break;
	case 6: if (Prm == 91) { RS(176); RS(177); } break;
	case 5: if (Prm == 91) { RS(178); RS(179); } break;
	case 4: if (Prm == 91) RL(420); if (!IN29(Inv, 1)) T[24] = 2; break;
	case 2: case 3: if (Prm == 91 && IN29(Inv, 1)) RL(419 + T[24]); break;
	case 1:
		Van(123);
		if (Prm == 91) { RS(180); DOpen(55); RL(418); DClose(55); }
		break;
	}
	switch (T[25])
	{
	case 1: if (Prm == 91) RS(187); T[24] = 2; T[26] = 40; Add(64); break;
	default: if (IN_RANGE(T[25], 2, 5)) if (Prm == 91) RS(188 - T[25]);
	}
	if (En(64))
		if (!Warn(198, 38, 54))
		if (!Warn(199, 33, 55))
		if (!Warn(203, 30, 59))
		if (!Warn(200, 25, 57))
		if (!Warn(201, 20, 58))
		if (!Warn(202, 15, 56))
		if (!Warn(204, 10, 60))
		if (!Warn(205, 6, 61))
		if (!Warn(206, 3, 62))
		if (!Warn(207, 2, 63)) { }
	if (T[26] == 1) { RS(197); DEAD(); }
	if (T[27] == 1998 && Prm == 99) { RS(213); Blast(); DEAD(); }
	if (T[28] == 1998 && Prm == 101) { RS(213); Blast(); DEAD(); }
}

// ---- parser