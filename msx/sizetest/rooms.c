// Hand translation of Supernova Room1, Room2, Room3, Room14 (NOVA.PAS) -- size test
#include "nova.h"

void Room1(void)
{
	switch (Prm) {
	case 1:
		switch (Vb) {
		case 2: case 13: Door(2, 50); break;
		case 11: RL(199); break;
		case 22: case 24: case 28:
			switch (N1) {
			case 37: case 38: case 55: case 44:
				if (Md != 0) RL(534); else RB(3, 11); break;
			case 103: RL(317); break;
			case 35: if (En(6)) RL(542); else RL(543); break;
			case 59:
				switch (Region) {
				case 1: RS(46); break;
				case 2: RS(63); break;
				case 4: RS(T[9] + 46); break;
				}
				break;
			}
			break;
		case 35:
			if (N1 == 16 || N1 == 35) {
				if (Here(16) && En(6)) {
					if (!En(4)) {
						Add(7); sound(440); RS(19); T[6] = 4;
						for (x = 1; x <= Rnd(30) + 9; x++) { y = Rnd(6000) + 60; play(y, y, 70); delay(9); }
						Score(10, 67);
					} else {
						Add(7); RL(149);
						for (x = 1; x <= Rnd(9) + 9; x++) { y = Rnd(9999); play(y, y, 30); delay(55); }
					}
				} else RL(141);
			}
			break;
		case 56:
			if (En(7) && En(6)) {
				if (T[6] > 0) {
					if (StrEq(Input, Identity)) { RS(21); T[6] = 0; Score(15, 4); play(400, 400, 200); }
					else RL(142);
				} else {
					if (StrEq(Input, "r2/20-8 d2/vega")) {
						if (!(Region == 2 || Region == 4)) { RS(28); Add(26); Min(27); play(600, 600, 99); }
						else RL(154);
					} else { Min(26); RS(39); Min(27); play(600, 600, 99); }
				}
			} else RL(179);
			break;
		case 44:
			if (En(7) && En(6)) {
				switch (N1) {
				case 43:
					play(750, 750, 200);
					if (Region == 4) {
						if (T[9] == 11) {
							Score(5, 68); Region = 2; T[13] = 3; TFlag = 2;
							StrCpy(Nn[126], "hinged mouth\\mouth\\hinge\\");
							RB(9, 11); Explode(30);
						} else RS(64);
					} else RL(198);
					break;
				case 40:
					play(900, 900, 200);
					if (En(26)) { RS(37); Add(27); } else RL(195);
					break;
				case 41:
					play(600, 600, 200);
					if (En(27)) {
						if (!En(22) && !En(23)) {
							Col(28, 23); RL(180); Col(11, 7);
							RS(38); T[8] = 7;
							for (x = 1; x <= 6; x++)
								for (y = 30; y <= 200; y++) { sound(y); delay(5); }
							nosound();
						} else { RS(40); play(500, 999, 3); Min(26); Min(27); }
					} else if (En(26)) RL(184); else RL(185);
					break;
				}
			} else RL(179);
			break;
		case 39:
			if (SetIn(NounSet, 16) && En(6)) {
				if (En(7)) RL(575); else { Min(6); RL(145); SFlag = 0; }
			}
			break;
		}
		break;
	}
}

void Room2(void)
{
	switch (Prm) {
	case 2:
		switch (Vb) {
		case 9: case 13: case 51:
			if (N1 == 0 || N1 == 62 || N1 == 64 || N1 == 71) {
				if (En(23)) {
					RL(237);
					switch (Region) { case 1: Move(22); break; case 2: Move(24); break; }
				} else RL(99);
			}
			break;
		case 1: case 11: Door(1, 50); break;
		case 2: Move(3); break;
		case 4: Move(4); break;
		case 24: case 28:
			if (StrEq(NStr, "hatch")) { if (En(23)) RL(163); else RL(99); }
			break;
		case 50:
			if (N1 == 78) {
				if (En(22)) { RL(160); Min(22); }
				else {
					RL(161); Add(22);
					if (T[8] > 0) { RS(40); play(500, 999, 3); T[8] = 0; Min(27); }
				}
			}
			break;
		case 41:
			if (StrEq(NStr, "hatch")) {
				if (Region != 4) {
					if (En(22)) {
						RL(96); Add(23);
						if (!En(44) && Region == 2) {
							Add(44); RL(327);
							for (i = 1; i <= 400; i++) sound(Rnd(3500) + 999);
							for (i = 7500; i >= 500; i--) { sound(Rnd(i) + i); sound(Rnd(3500) + 2000); }
							for (i = 1; i <= 300; i++) sound(Rnd(3500) + 2500);
							nosound();
						}
					} else RL(97);
				} else { RS(45); DEAD(); }
			}
			break;
		case 53:
			if (N1 == 56) { if (En(23)) { RL(98); Min(23); } else Say("hatch", "closed"); }
			break;
		}
		break;
	case 3:
		switch (Vb) {
		case 1: case 13: Move(2); break;
		case 3: Move(6); break;
		case 4: Move(7); break;
		case 28: case 31:
			if (N1 == 55 || N1 == 81) { RB2(4, 12); Pause(); }
			break;
		case 24: case 41: case 76:
			if (N1 == 55) { if (Md == 1 || Md == 4) RL(533); else { RS(22); DEAD(); } }
			break;
		}
		break;
	}
}

void Room3(void)
{
	switch (Prm) {
	case 4:
		switch (Vb) {
		case 3: case 13: Move(2); break;
		case 28: case 22:
			switch (N1) {
			case 90: RS(23); break;
			case 52: RS(26); break;
			case 38: RS(27); break;
			case 55: RS(31); break;
			case 81: case 119: RB(10, 14); Pause(); break;
			case 64:
				if (NicheCon != 0) { Wr("There is a "); Wr(FN(NicheCon)); Wr(" sitting on the niche."); Wln(); }
				else RL(152);
				break;
			}
			break;
		case 31: if (N1 == 81 || N1 == 119) { RB(10, 14); Pause(); } break;
		case 41: case 42: case 43: case 44: case 88:
			if (N1 == 55) RL(174);
			break;
		case 33:
			if (N2 == 64) {
				if (N1 == 3 || N1 == 4 || N1 == 8 || N1 == 9 || N1 == 12 || N1 == 17 || N1 == 29) {
					if (Pr == 1 || Pr == 5 || Pr == 10) {
						if (NicheCon == 0) { Van(N1); R[N1] = 4; NicheCon = N1; RL(147); }
						else RL(148);
					} else Crazy();
				} else RL(146);
			}
			break;
		case 54: if (N1 == 38 || N1 == 90) { RL(321); Serum = 1; } break;
		case 56:
			Serum = Serum + 1;
			if (Serum < 3) {
				if (NicheCon == 0) RS(24);
				else if (NicheCon != 29) {
					Wr("A small amount of serum drips onto the "); Wr(FN(NicheCon)); Wr(" and ");
					Wr("dries up."); Wln();
				} else {
					if (MugCon == 0) {
						MugCon = 92; RS(25); SetAdd(L[4], 92);
						if (StrEq(Input, "6")) Add(42); else Min(42);
					} else {
						if (MugCon == 79 || MugCon == 92 || MugCon == 99) {
							Wr("The serum mixes with the "); Wr(FN(MugCon)); Wr(" inside the mug."); Wln();
						} else {
							Wr("The serum drips on the "); Wr(FN(MugCon)); Wr(" inside the mug and");
							Wr(" quickly evaporates."); Wln();
						}
					}
				}
			} else RS(135);
			break;
		}
		break;
	case 5:
		switch (Vb) {
		case 2: case 13: Move(6); break;
		case 22: case 28: if (N1 == 98) RL(122); break;
		}
		break;
	case 6:
		switch (Vb) {
		case 1: case 11: Move(5); break;
		case 4: case 13: Move(3); break;
		case 28: switch (N1) { case 80: RL(159); break; case 69: RS(29); break; } break;
		case 51: if (N1 == 69) Vb = 35; break;
		}
		break;
	case 7:
		switch (Vb) {
		case 3: case 13: Move(3); break;
		case 22: case 28: if (N1 == 32) RL(155); break;
		case 24: if (N1 == 32) RL(156); break;
		case 41: case 53: if (N1 == 32) RL(157); break;
		case 39: if (SetIn(NounSet, 32)) RL(158); break;
		}
		break;
	}
}

void Room14(void)
{
	switch (Prm) {
	case 64:
		switch (Vb) {
		case 1: case 13: DOpen(99); RS(139); DEAD(); break;
		case 2: case 11: Move(65); break;
		case 28:
			switch (N1) {
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
		case 54:
			if (N1 == 127) { if (Md == 3) { RL(537); DEAD(); } else RL(538); }
			break;
		}
		break;
	case 65:
		switch (Vb) {
		case 1: Move(64); break;
		case 2: Move(66); break;
		case 4: Door(68, 20); break;
		case 28: if (N1 == 56) RL(92); break;
		}
		break;
	case 66:
		switch (Vb) {
		case 1: Move(65); break;
		case 2: Move(67); break;
		case 3: if (T[20] > 1) { Score(15, 91); Move(73); } else RL(92); break;
		case 4: Move(72); break;
		case 28:
			switch (N1) {
			case 56: if (T[20] > 1) RL(370); else RL(92); break;
			case 64: RL(335); break;
			}
			break;
		case 33:
			if (N2 == 64) {
				if (Pr == 5) {
					if (N1 == 12) { RL(336); T[20] = 3; DOpen(20); Score(5, 92); }
					else RL(137);
				}
			}
			break;
		}
		break;
	case 67:
		switch (Vb) {
		case 1: Move(66); break;
		case 2: case 11: Door(75, 5); break;
		case 4: Move(71); break;
		}
		break;
	case 71:
		switch (Vb) {
		case 3: Move(67); break;
		case 28:
			switch (N1) {
			case 59: if (CodeSet == 4) RL(353); else RL(355); break;
			case 116: RL(354); break;
			}
			break;
		case 31:
			if (N1 == 59) { if (CodeSet == 4) { RB2(11, 14); Pause(); } else RL(355); }
			break;
		}
		break;
	case 72:
		switch (Vb) {
		case 3: Move(66); break;
		case 28: case 24:
			switch (N1) {
			case 128: RL(560); break;
			case 37: case 38: RL(339); break;
			case 46: if (Md != 0) { Score(5, 93); RL(343); } else RS(145); break;
			case 55:
				if (En(46)) {
					if (RobotCon != 0) { Wr("The robot's panel contains a "); Wr(FN(RobotCon)); Wr("."); Wln(); }
					else RL(346);
				} else RL(344);
				break;
			}
			break;
		case 33:
			if (N2 == 55) {
				if (Pr == 5) {
					if (En(46)) {
						if (RobotCon == 0) {
							if (N1 == 3 || N1 == 4 || N1 == 8 || N1 == 12 || N1 == 17)
							{ Van(N1); RL(347); R[N1] = 72; RobotCon = N1; }
							else RL(137);
						} else RL(348);
					} else RL(344);
				}
			}
			break;
		case 41:
			if (N1 == 55 || N1 == 46) {
				if (RobotCon == 0) RL(558);
				else {
					R[RobotCon] = 72; Add(46); Score(25, 94);
					Wr("The panel slides open, revealing a "); Wr(FN(RobotCon)); Wr(" inside."); Wln();
				}
			}
			break;
		case 53: if (N1 == 55) { Van(RobotCon); Min(46); RL(344); } break;
		case 42: case 44:
			switch (N1) {
			case 37: case 38: case 44:
				RL(340); Walls(9); Gotoxy(43, Wherey() - 1); Col(28, 31);
				Wr("BOOM!"); Wln(); Explode(32); DEAD(); break;
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
