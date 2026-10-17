// DEFAULT.PAS: Default5 - Default9
#include "nova.h"

#define IN4(v, a, b, c, d) ((v) == (a) || (v) == (b) || (v) == (c) || (v) == (d))

static void Quench(void)    // T[4]:=130; Bor(0,0); PStat:=PStat-[6]
{
	T[4] = 130; Bor(0, 0); SET_DEL(PStat, 6);
}

void Default5(void)
{
	switch (Vb)
	{
	// PURCHASE
	case 64:
		if (Prm == 8)
		{
			if (N1 == 100) RS(10);
			else if (N1 == 29 || N1 == 99 || Pr == 11)
			{
				if (Pr == 6 || Pr == 9)
				{
					if (N2 == 8 || N2 == 111)
					{
						if (Here(8)) { SET_ADD(Inv, 29); Van(8); Score(15, 21); RS(11); T[1] = 21; Add(9); Add(16); }
						else RL(39);
					}
					else RL(40);
				}
				else if (Pr == 11)
				{
					if (IN4(N1, 29, 99, 111, 124) && IN4(N2, 29, 99, 111, 124))
					{
						if ((N1 == 111) != (N2 == 111)) RL(196);
						else if ((N1 == 124) != (N2 == 124)) RL(197);
						else Crazy();
					}
					else Crazy();
				}
				else NoSense();
			}
			else { if (N2 != 124) RL(34); else RL(35); }
		}
		else if (Prm == AlienRm) RL(35);
		else if (Prm == FriendRm) RL(36);
		else RL(38);
		break;

	// DRINK
	case 46:
		if (N1 == Null)
		{
			if (Here(29))
			{
				if (Prm == 8 && !IN29(Inv, 29)) RL(67);
				else
				{
drink_jump:
					if (MugCon != Null)
					{
						switch (MugCon)
						{
						case 99: MugCon = Null; Quench(); RL(64); break;
						case 79: MugCon = Null; Quench(); RL(66); Van(79); break;
						case 92:
drink_jump1:
							MugCon = Null;
							if (T[12] < 2) RL(162);
							else if (En(42))
							{
								RS(136); Van(92); T[12] = Null; Bor(0, 0); SET_DEL(PStat, 3); Score(20, 124);
							}
							else RL(162);
							break;
						}
					}
					else RL(65);
				}
			}
			else if (Here(98))   // for the sink
			{
				if (SinkRm == Prm) { Quench(); RL(66); }
				else RL(67);
			}
			else RL(67);
		}
		else
			switch (N1)
			{
			case 29: case 99: if (IN29(Inv, 29)) goto drink_jump; else RL(87); break;
			case 98: if (SinkRm == Prm) { Quench(); RL(66); } else RL(67); break;
			case 121: case 66: RS(12); Add(128); break;
			case 92: goto drink_jump1;
			case 79:
				if (Here(29) && MugCon == 79) { Quench(); MugCon = Null; Van(79); RL(66); }
				else if (Here(79)) { Quench(); RL(66); }
				else RL(67);
				break;
			default: Wr("The "); Wr(NStr); Wr(" would not quench your thirst."); Wln();
			}
		break;
	}
}

static u8 InWord(const char* w)    // Default6's nested function Word(W)
{
	char b[40];
	StrCpy(b, " "); StrCat(b, w, 30); StrCat(b, " ", 35);
	return Pos(b, Input) > 0;
}

#define WORD(s) InWord(s)

void Default6(void)
{
	switch (Vb)
	{
	// TALKING TO CHARACTERS
	case 66: case 67: case 68: case 69: case 71:
		Spaces(Input);
		if (Vb == 66)    // bartender
		{
			if (Here(111))
			{
				if (WORD("follow")) RL(46);
				else if (WORD("go") || WORD("east") || WORD("south")) RL(567);
				else if (WORD("hi") || WORD("hello")) RL(51);
				else if (WORD("infocom")) RL(498);
				else if (WORD("scott") || WORD("miller")) RL(499);
				else if (WORD("what") || WORD("why") || WORD("where") || WORD("who") || WORD("how") || WORD("which") || WORD("when")) RL(463);
				else if ((WORD("ill") || WORD("give") || WORD("buy") || WORD("get me") || WORD("purchas")) && (WORD("drink") || WORD("glunk"))) RL(47);
				else if (WORD("ambross") || WORD("pink") || WORD("bloody") || WORD("foeboz") || WORD("ztulctw")) RL(565);
				else if (WORD("kill") || WORD("fight") || WORD("hit")) RL(566);
				else if (WORD("give")) RL(564);
				else if (WORD("help") || WORD("tell me")) RL(479);
				else switch (Rnd(3)) { case 0: RL(50); break; case 1: RL(480); break; case 2: RL(481); break; }
			}
			else RL(52);
		}
		if (Vb == 67)    // two aliens
		{
			if (Here(124))
			{
				if (WORD("follow")) RL(48);
				else if (WORD("go") || WORD("south") || WORD("east")) RL(568);
				else if (WORD("crystal")) RL(484);
				else if (WORD("coordin") || WORD("where") || WORD("world") || WORD("sagan") || WORD("planet") || WORD("locatio")) RL(485);
				else if (WORD("what") || WORD("why") || WORD("who") || WORD("tell me") || WORD("how") || WORD("which") || WORD("when")) RL(464);
				else switch (Rnd(3)) { case 0: RL(49); break; case 1: RL(482); break; case 2: RL(483); break; }
			}
			else RL(53);
		}
		if (Vb == 68)    // scientist
		{
			if (Here(123))
			{
				if (WORD("follow")) RL(440);
				else if (WORD("hi") || WORD("hello") || WORD("bye")) RL(441);
				else if (WORD("wristba")) RL(442);
				else if (WORD("card") || WORD("green")) RL(459);
				else if (WORD("give")) RL(456);
				else if (WORD("go") || WORD("east") || WORD("west")) RL(457);
				else if (WORD("help")) RL(462);
				else if (WORD("ship") || WORD("planets")) RL(497);
				else if (WORD("star") || WORD("superno")) RL(486);
				else if ((WORD("crystal") || WORD("clear")) && (T[24] > 1 && T[24] < 6) && IN29(Inv, 1))
				{ T[24] = Null; T[25] = 7; RS(181); }
				else if ((WORD("crystal") || WORD("clear")) && (T[23] > 1 || T[24] > 5)) RL(446);
				else if (WORD("crystal") || WORD("clear")) RL(487);
				else if ((WORD("who") || WORD("what")) && WORD("you")) RL(461);
				else if (WORD("what") || WORD("why") || WORD("where") || WORD("who") || WORD("how") || WORD("which") || WORD("when") || WORD("tell me")) RL(458);
				else switch (Rnd(3)) { case 0: RL(443); break; case 1: RL(477); break; case 2: RL(478); break; }
			}
			else RL(439);
		}
		if (Vb == 69)    // robot
		{
			if (Here(46)) RL(508); else RL(584);
		}
		if (Vb == 71)    // computer
		{
			if (Here(37)) RL(545); else RL(546);
		}
		break;
	}
}

void Default7(void)
{
	switch (Vb)
	{
	// READ
	case 31:
		switch (N1)
		{
		case 12: RB2(6, 13); break;
		case 19: RB(12, 7); Pause(); break;
		default: Wr("There is nothing on the "); WrFN(N1); Wr(" to read."); Wln();
		}
		break;

	// TOUCH
	case 75:
		if (IN29(Wear, 26)) RL(571);
		else
			switch (N1)
			{
			case 1: case 22: case 23: case 24: case 25: if (IN29(Socket, N1)) RL(569); else RL(570); break;
			case 79: RL(552); break;
			case 120: RL(551); break;
			case 34: case 103: case 115: Wr("You can't reach the "); Wr(NStr); Wr("."); Wln(); break;
			default: Wr("You feel nothing unusual about the "); Wr(NStr); Wr("."); Wln();
			}
		break;

	// TASTE
	case 84:
		switch (N1)
		{
		case 6: RL(377); break;
		case 29: case 92: case 99: if (MugCon == 99) RL(70); else RL(71); break;
		case 111: RL(466); break;
		default: Wr("It tastes like a "); Wr(NStr); Wr(" should taste."); Wln();
		}
		break;

	// SMELL
	case 83:
		switch (N1)
		{
		case 6: RL(378); break;
		case 29: case 99: if (MugCon == 99) RL(72); else RL(599); break;
		case 111: RL(466); break;
		default:
			if (IN_RANGE(Prm, 42, 50)) RL(572);
			else { Wr("It smells like an ordinary "); Wr(NStr); Wr("."); Wln(); }
		}
		break;

	// LISTEN
	case 65:
		switch (N1)
		{
		case 42: RL(501); break;
		case 62: if (Region == 4) RL(573); else RL(574); break;
		case 111: RL(73); break;
		default: Wr("The "); Wr(NStr); Wr(" is not making any sound."); Wln();
		}
		break;

	// HELLO
	case 80:
		if (StrEq(Word, "hi") || StrEq(Word, "hello")) { Wr("Hello to you."); Wln(); }
		else RL(74);
		break;

	// YES/NO/MAYBE
	case 81: RL(Rnd(6) + 75); break;

	// COMMANDS
	case 95: RB(4, 11); Pause(); RB(6, 11); Pause(); break;

	// ATTACK
	case 93:
		if (Pr == 6)
		{
			if (IN29(Mov, N2) || N2 == 60)
			{
				switch (N1)
				{
				case 60:
					Cur(2); Wr("Are you sure? "); CFlag = (char)(GetKey() & 0xFF); Cur(3); Wln();
					if (UpCase(CFlag) == 'Y') { RL(488); Add(128); }
					else { Wr("Whew!"); Wln(); }
					break;
				case 123: RL(465); break;
				case 124:
					switch (Rnd(6))
					{
					case 0: RL(175); break;
					case 1: RL(176); Add(7); break;
					case 2: RL(177); Van(N2); SetR(N2, Prm); break;
					case 3: RS(33); T[3] = 26; T[4] = 23; T[5] = 33; break;
					case 4: RS(34); Van(N2); Prm = 13; Add(7); break;
					case 5: RS(36); T[7] = 9; SET_ADD(PStat, 4); Add(7); break;
					}
					break;
				default: Wr("The "); WrFN(N1); Wr(" offers no resistance."); Wln();
				}
			}
			else Crazy();
		}
		else NoSense();
		break;
	}
}

void Default8(void)
{
	u8 n;
	i16 k;
	switch (Vb)
	{
	// TURN
	case 50:
		if (Pr == Null)    // e.g. turn knob
		{
			switch (N1)
			{
			case 48:
turn_jump:
				if (Prm == SinkRm) { SinkRm = Null; LOC_DEL(Prm, 79); RL(81); }
				else if (Prm == 5 || Prm == 12) { SinkRm = Prm; LOC_ADD(Prm, 79); RL(82); }
				else RL(540);
				break;
			default: RL(83);
			}
		}
		else if (Input[0] == 0)    // e.g. turn sink on
		{
			switch (N1)
			{
			case 2:
				RL(460); Score(5, 123);
				switch (Rnd(4))
				{
				case 0: k = Rnd(99) + 50; for (i = 20; i <= k; i++) Play(20, i, 1); break;
				case 1:
					for (i = Rnd(5000) + 4500; i >= 20; i--) { Sound(i); Delay(1); Sound(31); }
					break;
				case 2:
					i = Rnd(9999) + 1; x = 0;
					do { j = Rnd(9999); Play(j, i, 0); i = Rnd(9999) + 1; Play(i, j, 0); x++; } while (!(x > 9));
					break;
				case 3:
					for (x = 1; x <= 200; x++) { i = Rnd(9999) + 1; Play(i, i, 1); Delay(16); }
					break;
				}
				NoSound(); if (Region == 5) Sound(60);
				break;
			case 37: case 38: RL(373); break;
			case 48: case 79: case 98: goto turn_jump;
			case 46: RL(360); break;
			case 127: if (!StrEq(NStr, "window")) RL(360); break;
			case 131: RL(84); break;
			default: Crazy();
			}
		}
		else if (Pr == 1)    // e.g. turn dial to 123
		{
			if (N1 == 48) { /* case Prm of 79: nothing */ }
			else Crazy();
		}
		else NoSense();
		break;

	// OPEN and UNLOCK
	case 41: case 88:
		if (N1 == 5) RL(289);
		else if (N2 == Null)
		{
			switch (N1)    // if N1 only
			{
			case 56: RL(409); break;
			default: RL(100);
			}
		}
		else if (Pr == 6)
		{
			if (N1 == 56)
			{
				if (Prm == 9 || Prm == 11) { if (N2 == 3) RL(101); else Crazy(); }
				else if (Prm == 10) { if (N2 == 3) { RL(102); Add(11); } else Crazy(); }
				else RL(103);
			}
			else if (N1 == 32 || N1 == 38 || N1 == 45 || N1 == 55 || N1 == 62 || N1 == 68 || N1 == 69 || N1 == 78 || N1 == 89 || N1 == 90 || N1 == 119) RL(103);
			else Crazy();
		}
		else NoSense();
		break;
	}
	(void)n;
}

static void SatchContents(void)
{
	if (SatchCon != Null) { Wr("The satchel contains "); WrFN(SatchCon); Wr("."); Wln(); }
	else RL(110);
}

void Default9(void)
{
	switch (Vb)
	{
	// LOOK INSIDE
	case 24:
		switch (N1)
		{
		case 5: SatchContents(); break;
		case 7:
			if (HolstCon == Null) RL(580);
			else { Wr("The gun strap contains a "); WrFN(HolstCon); Wr("."); Wln(); }
			break;
		case 18: if (En(34)) RL(270); else RL(271); break;
		case 29:
			if (MugCon != Null) { Wr("The mug contains "); WrFN(MugCon); Wr("."); Wln(); }
			else RL(111);
			break;
		case 35: if (StrEq(NStr, "toilet")) RL(114); else RL(113); break;
		case 10: case 26: case 28: case 32: case 62: case 64: case 69: case 98: case 113:
			Wr("The "); Wr(NStr); Wr(" is empty."); Wln(); break;
		case 119: RL(112); break;
		case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 115: case 118:
			if (Prm == 63) RL(113); else RL(115); break;
		default: RL(113);
		}
		break;

	// LOOK UNDER, LOOK BEHIND and REACH IN
	case 20: case 26: case 76:
		switch (N1)
		{
		case 5: SatchContents(); break;
		case 80: if (!En(127)) { RS(16); Add(127); } else RL(116); break;
		case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 115: case 118:
			if (Prm == 63) RL(113); else RL(115); break;
		default: RL(116);
		}
		break;

	// LOOK ON TOP
	case 22:
		switch (N1)
		{
		case 66: RL(117); break;
		case 80: RL(118); break;
		case 99: RL(119); break;
		case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 115: case 118:
			if (Prm == 63) RL(113); else RL(115); break;
		default: RL(120);
		}
		break;

	// CLUE
	case 70:
		if (Sc < 25) { RL(302); break; }
		if (!En(15))
		{
			Add(15); RS(30);
			Cur(2);
			Wr("Do you still wish to see the clue? "); CFlag = (char)(GetKey() & 0xFF); Wln();
			if (UpCase(CFlag) == 'N') { Wr("OK, no clue will be shown."); Wln(); break; }
		}
		Cn("---Here is your clue---"); Sc -= 25;
		if (Prm != StoreC) { StoreC = Prm; RC1(Prm, Text5); }
		Col(12, 9); Cn(Text5); Col(11, 7);
		break;
	}
}
