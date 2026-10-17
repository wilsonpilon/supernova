// DEFAULT.PAS: Default0 - Default4
#include "nova.h"

void Default0(void)
{
	switch (Vb)
	{
	// N, S, E, W, etc.
	case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
		if (IN_RANGE(Prm, 14, 19)) RL(26);
		else if (Prm == 21) RL(27);
		else if (IN_RANGE(Prm, 42, 50)) RL(279);
		else if (Prm > 7) { Wr("There is not a path leading "); Up(g_Dir[Vb]); Wr("."); Wln(); }
		else { Wr("There is not passage in that direction."); Wln(); }
		break;

	// LOOK
	case 30:
		SET_DEL(Brief, Prm); Describe(Prm);
		if (Prm == 56 && T[17] > 4) { RS(114); RS(115); Pause(); }
		break;

	case 82: Save(); break;       // SAVE
	case 87: Restore(); break;    // RESTORE
	}
}

static void DropIt(void)
{
	WrFN(o); Wr(":  ");
	if (IN29(Wear, o)) { Wr("First you must remove the "); WrFN(o); Wr("."); Wln(); }
	else { Wr("Dropped."); Wln(); SET_DEL(Inv, o); R[o] = Prm; }
}

void Default1(void)
{
	switch (Vb)
	{
	// INVENTORY
	case 16:
		if (!SetEmpty(Inv, 4))
		{
			Wr("You have in your possession..."); Wln();
			for (o = 1; o <= MMax; o++)
				if (IN29(Inv, o))
				{
					if (o == 6) { Wr("  some "); WrFN(o); } else { Wr("  a "); WrFN(o); }
					if (IN29(Wear, o)) Wr("  (being worn)");
					switch (o)
					{
					case 5: if (SatchCon != Null) Wr("  (contains something)"); break;
					case 7: if (HolstCon != Null) Wr("  (contains something)"); break;
					case 29: if (MugCon != Null) Wr("  (has something in it)"); break;
					}
					Wln();
				}
		}
		else RL(28);
		break;

	// DROP
	case 18:
		if (!SET_IN(NounSet, 30))
		{
			for (o = 1; o <= NMax; o++)
				if (SET_IN(NounSet, o))
				{
					if (IN29(Inv, o)) DropIt();
					else { Wr("You don't have the "); WrFN(o); Wr("."); Wln(); }
				}
		}
		else
		{
			Min(17);   // DROP ALL
			for (o = 1; o <= MMax; o++)
				if (IN29(Inv, o)) { Add(17); DropIt(); }
			if (!En(17)) RL(303);
		}
		break;

	// DROP ALL BUT
	case 17:
		for (o = 1; o <= MMax; o++)
			if (IN29(Inv, o) && !SET_IN(NounSet, o)) DropIt();
		break;
	}
}

void Default2(void)
{
	switch (Vb)
	{
	// ROOM DESCRIPTION
	case 77:
		if (!En(1)) { Add(1); RL(14); }
		else { Min(1); RL(15); }
		break;

	// QUIT
	case 79:
		Bor(1, 0);
		Wr("In "); WrNum(Tic); Wr(" moves you scored "); WrNum(Sc); Wr(" out of a possible 1000 points."); Wln();
		Wr("If you want to continue this game at a later date you should first use the"); Wln();
		Wr("SAVE command."); Wln();
		Wr("Are you still sure you want to QUIT?  ");
		Cur(2);
		CFlag = (char)(GetKey() & 0xFF);
		if (UpCase(CFlag) == 'Y')
		{
			RL(12); Delay(2200);
			Window(1, 1, 80, 25); NoSound(); ClrScr(); Cur(1); Bor(0, 0); Gotoxy(2, 2);
			Col(31, 16); Bak(4, 7); Wr("Good-bye!"); Wln(); Halt();
		}
		else RL(13);
		Bor(0, 0);
		break;

	// SLEEP
	case 62:
		if (T[5] > 32 && T[8] < 1)
		{
			if (N1 == 19 || N1 == 27 || N1 == 49 || N1 == 69 || N1 == 80 || N1 == 113)
			{
				T[2] = 6; Add(7);
				Wr("You lie down on the "); Wr(NStr); Wr(" and fall asleep within a few minutes."); Wln();
			}
			else RL(16);
		}
		else if (N1 == 69 || N1 == 80)
		{
			T[5] = 230; SET_DEL(PStat, 5); Bor(0, 0); Add(7);
			switch (N1)
			{
			case 69:
				if (T[8] < 1) { RS(41); Add(128); }
				else { RS(42); Add(7); Add(10); }
				break;
			case 80:
				if (T[8] < 1)
				{
					if (!En(18)) RS(218);
					else switch (Rnd(2)) { case 0: RS(218); break; case 1: RS(219); break; }
				}
				else { RS(220); Add(128); }
				break;
			}
		}
		else { Wr("The "); WrFN(N1); Wr(" is not a suitable place to sleep."); Wln(); }
		break;

	// SIT
	case 35:
		if (N1 == 16 || N1 == 32 || N1 == 35 || N1 == 49 || N1 == 54 || N1 == 69 || N1 == 71 || N1 == 80 ||
			IN_RANGE(N1, 83, 86) || N1 == 98 || N1 == 110)
		{
			Add(7);
			if (N1 == 69)
			{
				if (T[8] < 1) { RS(41); Add(128); }
				else { RS(42); T[5] = 160; SET_DEL(PStat, 5); Add(7); Add(10); }
			}
			else { Wr("You sit down on the "); Wr(NStr); Wr("."); Wln(); }
		}
		else RL(17);
		break;

	// STAND
	case 32:
		if (En(7)) { Min(7); Min(2); Min(3); Min(19); T[6] = Null; RL(18); }
		else RL(19);
		break;

	// UP and CLIMB
	case 9: case 51:
		if (N1 == 32 || N1 == 35 || N1 == 54 || N1 == 68 || N1 == 71 || N1 == 80 || N1 == 81 ||
			IN_RANGE(N1, 83, 86) || N1 == 89 || N1 == 98 || N1 == 112 || N1 == 125)
		{
			Add(8);
			Wr("You manage to climb up the "); Wr(NStr); Wr("."); Wln();
		}
		else if (Vb == 9) { Wr("There is not a path in that direction."); Wln(); }
		else RL(20);
		break;

	// DOWN
	case 10:
		if (En(8)) { Min(8); RL(21); }
		else { Wr("There is not a path in that direction."); Wln(); }
		break;

	// IN and OUT
	case 11: case 13:
		Wr("Due to the multiple paths you will have to be more specific."); Wln();
		break;

	// WAIT
	case 15:
		Wr("Time passes"); Wln(); y = Wherey();
		for (x = 1; x <= 6; x++) { Gotoxy(11 + x, y - 1); Wr("."); Sound(x * 99); Delay(30); }
		NoSound(); Wln();
		RegionSoundOn();
		break;
	}
}

static void ExamineNothing(void)
{
	// "case random(20)" at the end of EXAMINE
	u16 r = Rnd(20);
	if (r <= 6) { Wr("You can't find anything unusual about the "); WrFN(N1); Wr("."); Wln(); }
	else if (r <= 14) { Wr("You see nothing special about the "); WrFN(N1); Wr("."); Wln(); }
	else if (r <= 17) { Wr("It looks like any other "); Wr(NStr); Wr(" you've ever seen."); Wln(); }
	else { Wr("It looks like an ordinary "); Wr(NStr); Wr("."); Wln(); }
}

void Default3(void)
{
	SFlag = 0;
	switch (Vb)
	{
	// EXAMINE
	case 28:
		switch (N1)
		{
		case 1: if (En(41)) RS(116); else { RS(116); RS(132); Score(5, 41); } break;
		case 2: RL(454); break;
		case 3: RL(45); break;
		case 4: RL(294); break;
		case 5:
			RL(105);
			if (SatchCon != Null)
			{
				if (SatchCon == 6) { Wr("The satchel contains "); WrFN(SatchCon); Wr("."); Wln(); }
				else { Wr("The satchel contains a "); WrFN(SatchCon); Wr("."); Wln(); }
			}
			break;
		case 6:
			RL(54);
			switch (FoodCon) { case 1: RL(55); break; case 2: RL(56); break; case 3: RL(57); break; case 4: RL(58); break; }
			break;
		case 7:
			RL(579);
			if (HolstCon == Null) RL(580);
			else { Wr("The gun strap contains a "); WrFN(HolstCon); Wr("."); Wln(); }
			break;
		case 8: RL(59); break;
		case 9: RL(374); break;
		case 10: RL(262); break;
		case 11: RL(311); break;
		case 12: RB2(6, 13); break;
		case 16: RL(151); break;
		case 17: RL(263); break;
		case 18: if (En(34)) { Score(5, 116); RS(92); } else RL(267); break;
		case 19: RB(12, 11); Pause(); break;
		case 20: RL(372); break;
		case 22: RL(527); if (IN29(Socket, 22)) RL(528); break;
		case 23: RL(529); if (IN29(Socket, 23)) RL(528); break;
		case 24: RL(530); if (IN29(Socket, 24)) RL(528); break;
		case 25: RL(531); if (IN29(Socket, 25)) RL(528); break;
		case 26: RL(455); break;
		case 28: if (StrEq(NStr, "badge")) RL(406); else if (Md == 1) RL(407); else RL(408); break;
		case 29:
			Wr("The mug is quite small.  ");
			if (MugCon == Null) RL(60); else RL(61);
			break;
		case 42: RS(195); RS(196); break;
		case 54: if (StrEq(NStr, "column")) { RS(195); RS(196); } break;
		case 56: RL(92); if (IN_RANGE(Prm, 81, 88)) RL(423); break;
		case 62:
			switch (Prm) { case 21: case 25: RS(15); break; case 22: case 24: RS(246); break; }
			break;
		case 99: RL(63); break;
		case 103:
			if (IN_RANGE(Prm, 24, 34) || Prm == 59 || Prm == 60) RL(317); else RL(326);
			break;
		case 113: RR(Prm); break;
		case 123: RS(159); break;
		case 124: RL(62); break;
		}
		if (!SFlag) ExamineNothing();
		break;

	// EAT
	case 47:
		if (N1 == 6)
		{
			RL(68); FoodCon--; T[3] = 175; SET_DEL(PStat, 2); Bor(0, 0);
			if (FoodCon == 0) { Van(6); RL(69); }
		}
		else { Wr("The "); Wr(NStr); Wr(" would not do much for your digestive system."); Wln(); }
		break;
	}
}

void Default4(void)
{
	switch (Vb)
	{
	// GET
	case 39:
		Weight = Null; Min(17);
		for (o = 1; o <= MMax; o++) if (IN29(Inv, o)) { Weight++; if (o == 11) Weight += 3; }
		if (!SET_IN(NounSet, 30))
		{
			for (o = 1; o <= NMax; o++)
				if (SET_IN(NounSet, o))
				{
					if (IN29(Mov, o))
					{
						WrFN(o); Wr(":  ");
						Weight++; if (o == 11) Weight += 3;
						if (Weight < 9)
						{
							if (!IN29(Inv, o))
							{
								if (IN29(Socket, o) && !IN29(Wear, 26)) RL(524);
								else
								{
									if (o == PyraCon && o == 9) Walls(8);
									Wr("Taken."); Wln(); Van(o); SET_ADD(Inv, o);
								}
							}
							else { Wr("You already have that."); Wln(); }
						}
						else
						{
							RL(587);
							if (o == 11) Weight -= 3;
						}
					}
					else
					{
						WrFN(o); Wr(":  ");
						if (N1 == 32 || N1 == 35 || N1 == 80 || IN_RANGE(N1, 83, 86) || N1 == 90 || N1 == 121) RL(502);
						else Crazy();
					}
				}
		}
		else
		{   // GET ALL
			for (o = 1; o <= MMax; o++)
				if (IN29(Mov, o))
					if (R[o] == Prm)
					{
						WrFN(o); Wr(":  "); Add(17);
						Weight++; if (o == 11) Weight += 3;
						if (Weight < 9)
						{
							if (IN29(Socket, o) && !IN29(Wear, 26)) RL(524);
							else { Wr("Taken."); Wln(); Van(o); SET_ADD(Inv, o); }
						}
						else
						{
							RL(587);
							if (o == 11) Weight -= 3;
						}
					}
			if (!En(17)) RL(29);
		}
		break;

	// GET ALL BUT
	case 37:
		Weight = Null; Min(17);
		for (o = 1; o <= MMax; o++) if (IN29(Inv, o)) { Weight++; if (o == 11) Weight += 3; }
		for (o = 1; o <= MMax; o++)
			if (IN29(Mov, o) && !SET_IN(NounSet, o))
				if (R[o] == Prm)
				{
					WrFN(o); Wr(":  ");
					Weight++; if (o == 11) Weight += 3;
					if (Weight < 9)
					{
						if (IN29(Socket, o) && !IN29(Wear, 26)) RL(524);
						else { Wr("Taken."); Wln(); Van(o); SET_ADD(Inv, o); Add(17); }
					}
					else
					{
						RL(587);
						if (o == 11) Weight -= 3;
					}
				}
		if (!En(17)) RL(29);
		break;
	}
}
