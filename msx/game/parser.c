// ADPAR.PAS parser and player input (overlay)
#include "nova.h"
#define CH(s, k) ((s)[(k) - 1])

static char* s_I;   // the string being formatted (the "var I" of PreFormat's nested D)

static void D(const char* A, u8 B)
{
	u8 p;
	while ((p = Pos(A, s_I)) != 0) Delete(s_I, p, B);
}

static void Strip(const char* what, u8 off, u8 cnt)
{
	u8 p;
	while ((p = Pos(what, s_I)) != 0) Delete(s_I, p + off, cnt);
}

void PreFormat(char* I)
{
	u8 p;
	s_I = I;
	D("  ", 1); QFormat(I);
	FFlag = 0; if (I[0]) FFlag = 1;
	Spaces(I);
	Crawl = Pos(" crawl ", I) > 0; D(" crawl ", 6);
	D(" walk ", 5); D(" run ", 4); D(" go ", 3); D(" travel ", 7);
	QFormat(I); if (!I[0] && FFlag == 1) FFlag = 2; Spaces(I);
	D(" of ", 3); D(" the ", 4); D(" very ", 5); D(" more ", 5); D(" and ", 4);
	D(" large ", 6); D(" big ", 4); D(" huge ", 5); D(" please ", 7);
	D(" small ", 6); D(" little ", 7); D(" tiny ", 5);
	D("'", 1); D("#", 1); D("!", 1); D("?", 1); D(" number ", 7);
	QFormat(I); if (!I[0] && FFlag == 1) FFlag = 3; Spaces(I);
	Strip("examine ", 2, 5);
	Strip(" into ", 3, 2);
	Strip(" onto ", 3, 2);
	Strip(" inside ", 3, 4);
	Strip(" within ", 1, 4);
	Strip("look ", 1, 3);
	Strip(". ", 1, 1);
	while ((p = Pos(",", I)) != 0) { Insert(" ", I, p, 131); Delete(I, Pos(",", I), 1); }
	D("..", 1); D(" .", 1); D("  ", 1);
	QFormat(I);
	if (!I[0])
	{
		EFlag = Null;
		switch (FFlag)
		{
		case 2: RL(193); break;
		case 3: RL(186); break;
		default: Wr("Pardon me?"); Wln();
		}
	}
}

void ChopSeven(char* I)
{
	char w[136];
	u8 wl;
	if (Len(I) > 0)
	{
		StrCat(I, " ", 131); x = 1;
		do
		{
			wl = 0;
			while (CH(I, x) != ' ' && CH(I, x) != '.' && CH(I, x) != ',') { w[wl++] = CH(I, x); x++; }
			w[wl] = 0;
			if (wl > 7)
			{
				y = Pos(w, I); x = x + (7 - wl);
				Delete(I, y, wl); Delete(w, 8, 130);
				Insert(w, I, y, 131);
			}
			x++;
		} while ((u8)(x - 1) != Len(I));
		Delete(I, Len(I), 1);
	}
}

void FindMood(char* input, char* word, u8* md)
{
	char t1[56], t2[32], pat[34];
	char counter;
	u8 p;
	Spaces(input); x = 0;
	while (x < AMax)
	{
		x++;
		counter = '1';
		StrCpy(t1, g_A[x]);
		pat[0] = counter; pat[1] = 0;
		while ((p = Pos(pat, t1)) > 0)
		{
			Copy(t2, t1, 1, p - 1);
			StrCpy(pat, " "); StrCat(pat, t2, 31); StrCat(pat, " ", 33);
			if (Pos(pat, input) > 0)
			{
				StrCpy(word, t2);
				*md = x; if (*md == 2) *md = 1;
				x = AMax; counter = '8';
				Delete(input, Pos(t2, input), Len(t2) + 1);
			}
			pat[0] = counter; pat[1] = 0;
			Delete(t1, 1, Pos(pat, t1));
			counter++;
			pat[0] = counter; pat[1] = 0;
		}
	}
	QFormat(input);
}

static void CantSee(u8 n)
{
	Wr("You can't see any "); WrFN(n); Wr(" here."); Wln();
}

u8 Present(void)
{
	x = 0;
	if (!(Vb == 17 || Vb == 18 || Vb == 37 || Vb == 39))
	{
		if (N1 != Null)
		{
			if (Here(N1))
			{
				if (N2 != Null)
				{
					if (Here(N2)) return 1;
					CantSee(N2);
					return 0;
				}
				return 1;
			}
			CantSee(N1);
			return 0;
		}
		return 1;
	}
	for (;;)
	{
		x++;
		if (x > NMax) return 1;
		if (SET_IN(NounSet, x))
			if (!Here(x)) { CantSee(x); return 0; }
	}
}

static void Convert(u8* n, u8 max)
{
	switch (max)
	{
	case 1:     // Verbs
		switch (*n)
		{
		case 12: *n = 11; break; case 14: *n = 13; break; case 23: *n = 22; break; case 27: *n = 26; break;
		case 29: *n = 28; break; case 34: *n = 33; break; case 36: *n = 35; break; case 38: *n = 37; break;
		case 40: *n = 39; break; case 43: *n = 42; break; case 45: *n = 44; break;
		case 52: *n = 51; break; case 55: *n = 54; break; case 57: case 58: *n = 56; break;
		case 60: case 61: *n = 59; break; case 63: *n = 62; break;
		}
		break;
	case 2:     // Nouns
		switch (*n)
		{
		case 13: case 14: case 15: *n = 12; break; case 21: *n = 20; break; case 33: *n = 32; break;
		case 36: *n = 35; break; case 39: *n = 38; break; case 45: *n = 44; break;
		case 50: *n = 49; break; case 53: *n = 52; break; case 65: *n = 64; break; case 67: *n = 66; break;
		case 70: *n = 69; break; case 76: case 77: *n = 75; break;
		case 97: *n = 96; break; case 101: case 102: *n = 100; break; case 114: *n = 113; break;
		case 117: *n = 116; break; case 63: *n = 62; break;
		case 27: *n = 26; break; case 57: *n = 56; break; case 61: *n = 60; break; case 91: *n = 90; break;
		case 105: case 106: case 107: case 108: *n = 109; break; case 31: *n = 28; break;
		case 133: *n = 46; break; case 84: if (Prm > 21) *n = 128; break;
		case 115: if (Prm == 28) *n = 112; break;
		case 58: if (Prm == 52) *n = 64; break;
		}
		break;
	case 3:     // Prepositions
		switch (*n)
		{
		case 2: *n = 1; break; case 4: *n = 3; break; case 8: *n = 7; break;
		}
		break;
	}
}

// Looks for one of the words of the list `max` (1 verbs, 2 nouns, 3 prepositions) at the start of I
// (I = ' ' + text). Entries are lists of synonyms, each ending in '\'; a match is " synonym ".
void FindWord(char* I, u8* vnp, char* word, u8 max)
{
	u8 j2 = 0, lim, n, k;
	const char* e;
	QFormat(I); Spaces(I);
	lim = Mx[max];
	while (j2 < lim)
	{
		j2++;
		e = max == 1 ? g_V[j2] : (max == 2 ? g_N[j2] : g_P[j2]);
		while (*e)
		{
			for (n = 0; e[n] != '\\'; n++) { }
			if (I[0] == ' ')
			{
				for (k = 0; k < n && I[1 + k] == e[k]; k++) { }
				if (k == n && I[1 + n] == ' ')
				{   // Match Found
					*vnp = j2;
					Convert(vnp, max);
					for (k = 0; k < n; k++) word[k] = e[k];
					word[n] = 0;
					Delete(I, 1, n + 1);
					if (max == 1) StrCpy(VStr, word);
					else if (max == 2) StrCpy(NStr, word);
					j2 = lim;
					break;
				}
			}
			e += n + 1;
		}
	}
	QFormat(I);
}

void Dictionary(u8 ifFound, u8 skipList)
{
	u8 stopLoopFlag, p;
	char w[40];
	VNP = Null; list = 1; stopLoopFlag = 1;
	while (list < 4 && stopLoopFlag == 1)
	{
		if (list == skipList) list++;
		else
		{
			FindWord(Input, &VNP, Word, list);
			if (VNP != Null) { EFlag = ifFound; list--; stopLoopFlag = 0; }
			list++;
		}
	}
	if (EFlag != ifFound)
	{
		EFlag = 5; StrCat(Input, " ", 131);
		p = Pos(" ", Input);
		Copy(Word, Input, 1, p - 1);
		StrCpy(w, " "); StrCat(w, Word, 38); StrCat(w, " ", 39);
		if (Pos(w, " top directi next some from is under underne leaning but speak pay ") > 0) EFlag = ifFound;
		else if (Word[0] >= '0' && Word[0] <= '9') EFlag = 17;
		if (ifFound == 14) EFlag = 14;
		QFormat(Input);
	}
}

// ---- text records (Pascal read the files R1,R2,S1,L1,C1,SM,B1; here they come from the text pools)
static u8 AskKey(const char* prompt, const char* valid)
{
	u8 k;
	for (;;)
	{
		Wr(prompt);
		CFlag = (char)(GetKey() & 0xFF);
		WrC(CFlag); Wln(); Wln();
		k = UpCase(CFlag);
		for (x = 0; valid[x]; x++) if (valid[x] == k) return k;
	}
}

void MoreThanOne(void)
{
	u8 k;
	if ((N1 == 58 || SET_IN(NounSet, 58) || N2 == 58) && Here(3) && Here(4))
	{
		Cur(2);
		k = AskKey("Which one, the R)usty or S)hiney key? ", "RS");
		x = (k == 'R') ? 3 : 4;
		if (N1 == 58) N1 = x;
		if (N2 == 58) N2 = x;
		if (SET_IN(NounSet, 58)) { SET_DEL(NounSet, 58); SET_ADD(NounSet, x); }
	}
	if ((N1 == 86 || SET_IN(NounSet, 86) || N2 == 86) && Prm == 8)
	{
		Cur(2);
		k = AskKey("Which one, the W)estern, M)iddle or E)astern table? ", "WME");
		x = (k == 'W') ? 83 : (k == 'M') ? 84 : 85;
		if (N1 == 86) N1 = x;
		if (N2 == 86) N2 = x;
		if (SET_IN(NounSet, 86)) { SET_DEL(NounSet, 86); SET_ADD(NounSet, x); }
	}
	if ((N1 == 44 || SET_IN(NounSet, 44) || N2 == 44) && (Prm == 1 || Prm == 70) && Vb != 28)
	{
		Cur(2);
		k = AskKey("Which one, the T)an, P)urple or C)yan button? ", "TPC");
		x = (k == 'T') ? 41 : (k == 'P') ? 43 : 40;
		if (N1 == 44) N1 = x;
		if (N2 == 44) N2 = x;
		if (SET_IN(NounSet, 44)) { SET_DEL(NounSet, 44); SET_ADD(NounSet, x); }
	}
	if (N1 == 58 && Here(3) && !Here(4)) N1 = 3;
	if (N2 == 58 && Here(3) && !Here(4)) N2 = 3;
	if (SET_IN(NounSet, 58) && Here(3) && !Here(4)) { SET_DEL(NounSet, 58); SET_ADD(NounSet, 3); }
	if (N1 == 58 && Here(4) && !Here(3)) N1 = 4;
	if (N2 == 58 && Here(4) && !Here(3)) N2 = 4;
	if (SET_IN(NounSet, 58) && Here(4) && !Here(3)) { SET_DEL(NounSet, 58); SET_ADD(NounSet, 4); }
}

void PrintWord(const char* w)
{
	Wr("--you can't use the word "); Up(w); Wr(" here."); Wln();
}

// Pascal val(): Code = value, TestC = 0 or the (1-based) position of the first bad character
static u8 IsNoun(u8 v, const u8* set) { return SET_IN(set, v) != 0; }

void Parser_Syntax(char* input)
{
	(void)input;
	StrCpy(Word, ""); Md = Null; Num = Null; Code = Null;
	Vb = Null; N1 = Null; N2 = Null; SetClear(NounSet, 17);
JUMP1:   // used if the player forgets the first noun
	FFlag = Null; Pr = Null;
JUMP2:   // used if the player forgets the second noun or preposition
	EFlag = Null;
	FindMood(Input, Word, &Md);
	if (Len(Input) > 0)
	{
		FindMood(Input, Word, &Num);
		if (Num == Null)
		{
			FindWord(Input, &Vb, Word, 1);
			if (Vb != Null)
			{
				if (Len(Input) == 0)
				{
					if (IsNoun(Vb, NoNounOnly) || IsNoun(Vb, OneNounMaybe)) EFlag = Legal;
					if (EFlag != Legal)
					{
						if (Vb == 35 && Here(35)) { EFlag = Legal; N1 = 35; StrCpy(NStr, "seat"); }
						if (Vb == 47 && Here(6)) { EFlag = Legal; N1 = 6; }
						if (Vb == 62 && Here(80)) { EFlag = Legal; N1 = 80; }
						if (Vb == 65 && Here(124)) { EFlag = Legal; N1 = 124; }
						if (EFlag != Legal) EFlag = 4;
					}
				}
				else if (IsNoun(Vb, NoNounOnly)) Dictionary(3, 9);
				else if (!(Vb == 17 || Vb == 18 || Vb == 37 || Vb == 39))   // get, drop and but branch off
				{
					if (!(Vb == 25 || Vb == 32 || Vb == 56 || IN_RANGE(Vb, 66, 69) || Vb == 71 || Vb == 80 || Vb == 81)) // characters and type
					{
						if (Vb != FFlag) FindWord(Input, &N1, Word, 2);
						if (N1 != Null) FN(N1, LastNoun);
						if (N1 != Null)
						{
							if (!StrEq(Word, "all"))
							{
								if (Len(Input) == 0)
								{
									if (IsNoun(Vb, ToNounOnly))
									{
										if (StrEq(VStr, "fill") && Prm == SinkRm && N1 == 29) { N2 = 79; Pr = 6; EFlag = Legal; }
										else if (Vb == 64 && Prm == 8 && (N1 == 29 || N1 == 99) && !En(21) && Here(8)) { Pr = 6; N2 = 8; EFlag = Legal; }
										else if (Vb == 88 && IN_RANGE(Prm, 9, 11) && N1 == 56 && Here(3)) { Pr = 6; N2 = 3; EFlag = Legal; }
										else EFlag = 15;
									}
									else EFlag = Legal;
								}
								else if (IsNoun(Vb, ToNounMaybe) || IsNoun(Vb, ToNounOnly))
								{
									FindWord(Input, &Pr, Word, 3);
									if (Pr != Null)
									{
										if (Len(Input) == 0)
										{
											if (Vb == 50 && (Pr == 1 || Pr == 9)) EFlag = Legal; else EFlag = 6;
										}
										else if (Vb != 50)
										{
											FindWord(Input, &N2, Word, 2);
											if (N2 != Null)
											{
												if (!StrEq(Word, "all"))
												{
													if (Len(Input) == 0) EFlag = Legal; else Dictionary(12, 9);
												}
												else EFlag = 16;
											}
											else Dictionary(11, 2);
										}
										else
										{
											Val(Input, &Code, &TestC);
											if (TestC == 0) EFlag = Legal;
											else { Delete(Input, 1, (u8)(TestC - 1)); Dictionary(14, 9); }
										}
									}
									else Dictionary(9, 3);
								}
								else { Dictionary(3, 9); if (list == 2) EFlag = 8; }
							}
							else EFlag = 16;
						}
						else Dictionary(10, 2);
					}
					else { QFormat(Input); EFlag = Legal; }   // special case for SAY, TYPE, characters
				}
				else   // special case for GET and DROP
					while (EFlag == Null)
					{
						N1 = Null;
						FindWord(Input, &N1, Word, 2);
						if (N1 != Null) FN(N1, LastNoun);
						if (N1 != Null)
						{
							if (!SET_IN(NounSet, N1))
							{
								SET_ADD(NounSet, N1);
								if (Len(Input) == 0) EFlag = Legal;
							}
							else EFlag = 13;
						}
						else Dictionary(10, 2);
					}
			}
			else Dictionary(7, 1);
		}
		else EFlag = 2;
	}
	else EFlag = 1;
	if (EFlag != Legal) { StrCpy(Line, ""); Play(199, 199, 55); Delay(70); Play(60, 60, 55); }
	switch (EFlag)
	{
	case 1: RL(186); break;
	case 2: Wr("The word "); Up(Word); Wr(" is too many adverbs."); Wln(); break;
	case 3: Wr("Illegal input"); PrintWord(Word); break;
	case 4:
		if (Vb == 56) RL(187);
		else
		{
			Wr("Noun missing--");
			switch (Vb)
			{
			case 35: case 62: Wr("what do you want to "); Up(Word); Wr(" on?"); Wln(); break;
			case 65: Wr("what do you want to "); Up(Word); Wr(" to?"); Wln(); break;
			default: Wr("what do you want to "); Up(Word); Wr("?"); Wln();
			}
			PlayerInput(Line);
			if (Len(Input) > 0) goto JUMP1;
		}
		break;
	case 5:
		if (Len(Word) > 1) { Wr("The word "); Up(Word); Wr(" is not used in this adventure."); Wln(); }
		else { Wr("The letter "); Up(Word); Wr(" is not used as shorthand in this parser."); Wln(); }
		break;
	case 6:
		Wr("Noun missing--what do you want to "); Up(VStr); Up(" the "); Up(NStr); Wr(" "); Up(PStr); Wr("?"); Wln();
		PlayerInput(Line); FFlag = Vb;
		if (Len(Input) > 0) goto JUMP2;
		break;
	case 7: Wr("Verb missing"); PrintWord(Word); break;
	case 8: RL(188); break;
	case 9: Wr("Preposition expected"); PrintWord(Word); break;
	case 10: Wr("Noun expected"); PrintWord(Word); break;
	case 11: Wr("Indirect noun expected"); PrintWord(Word); break;
	case 12: Wr("No more input expected"); PrintWord(Word); break;
	case 13: Wr("Illegal noun used--"); Up(Word); Wr(" referenced more than once."); Wln(); break;
	case 14: Wr("Number expected"); PrintWord(Word); break;
	case 15:
		Wr("Preposition and noun missing--");
		if (Vb == 33 || Vb == 48) { Wr("where do you want to "); Up(VStr); Wr(" the "); Up(NStr); Wr("?"); Wln(); }
		else if (Vb == 49)
		{
			Pr = 1;
			Wr("who do you want to "); Up(VStr); Wr(" the "); Up(NStr); Wr(" to?"); Wln();
		}
		else
		{
			Pr = 6;
			Wr("what do you want to "); Up(VStr); Wr(" the "); Up(NStr); Wr(" with?"); Wln();
		}
		PlayerInput(Line); FFlag = Vb;
		if (Len(Input) > 0) goto JUMP2;
		break;
	case 16: RL(189); break;
	case 17: RL(576); break;
	}
}

static void Key(const char* k)
{
	StrCat(Line, k, 131); QFlag = 1; Wr(k);
}

static void ScanKey(u8 hi)
{
	switch (hi)
	{
	case 59: Key("Save"); break;      case 71: Key("Northwest"); break;
	case 60: Key("Restore"); break;   case 73: Key("Northeast"); break;
	case 61: Key("R D"); break;       case 79: Key("Southwest"); break;
	case 62: Key("Look"); break;      case 81: Key("Southeast"); break;
	case 63: Key("Get all"); break;   case 82: Key("Down"); break;
	case 64: Key("Drop all"); break;  case 83: Key("Up"); break;
	case 65: Key("Score"); break;     case 104: QFlag = 1; RR(0); break;
	case 66: Key("Inventory"); break;
	case 67: Key("Wait"); break;
	case 68: StrCpy(Line, ""); Key("Repeat"); break;
	case 94: case 30: Key("by Scott Miller"); break;
	case 95: case 47: Key("Version A Dec 9, 85"); break;
	}
	if (IN_RANGE(Prm, 1, 7))
		switch (hi)
		{
		case 72: Key("Fore"); break; case 75: Key("Port"); break; case 77: Key("Starboard"); break; case 80: Key("Aft"); break;
		}
	else
		switch (hi)
		{
		case 72: Key("North"); break; case 75: Key("West"); break; case 77: Key("East"); break; case 80: Key("South"); break;
		}
}

void PlayerInput(char* line)
{
	u16 k;
	u8 lo, hi, p;
	(void)line;
	Wln();   // main space in game
	if (Len(Line) == 0)
	{
		Bak(4, 7); Col(14, 0);
		Window(1, 1, 80, 22);
		Gotoxy(8, 2); WrNum(Tic); Wr("   ");
		Gotoxy(35 - (Len(g_RN[Prm]) / 2), 2);
		Wr("      "); Wr(g_RN[Prm]); Wr("      ");
		Gotoxy(75, 2); Wr("    "); Gotoxy(75, 2); WrNum(Sc); Wr(" ");
		Bak(7, 7);
		Col(1, 0); Gotoxy(22, 3);
		if (SetEmpty(PStat, 1)) Wr("Healthy"); else Wr("  *    ");
		Col(15, 0); Gotoxy(33, 3);
		if (SET_IN(PStat, 2)) { Col(31, 16); Wr("Hungry"); } else Wr(" *    ");
		Col(4, 0); Gotoxy(43, 3);
		if (SET_IN(PStat, 3)) { Col(20, 16); Wr("Sick"); } else Wr(" *  ");
		Col(0, 0); Gotoxy(51, 3);
		if (SET_IN(PStat, 4)) { Col(16, 16); Wr("Injured"); } else Wr("   *   ");
		Col(6, 0); Gotoxy(62, 3);
		if (SET_IN(PStat, 5)) { Col(22, 16); Wr("Tired"); } else Wr("  *  ");
		Col(5, 0); Gotoxy(71, 3);
		if (SET_IN(PStat, 6)) { Col(21, 16); Wr("Thirsty"); } else Wr("   *   ");
		Bak(0, 0); Window(2, 5, 79, 24);
		if (En(66)) { Gotoxy(1, 20); return; }
		Gotoxy(1, 20); Col(28, 31); WrC((char)175); Wln();
		Cur(1);
		Col(14, 7); Gotoxy(3, 19);
		QFlag = 0;
		do
		{
			k = GetKey(); lo = k & 0xFF; hi = k >> 8;
			Sound(99); NoSound(); RegionSoundOn();
			if (lo == 8)    // backspace
			{
				if (Wherex() == 1 && Wherey() == 20) { Window(1, 1, 80, 25); Gotoxy(80, 23); }
				if (Len(Line) > 0) { WrC(8); WrC(' '); WrC(8); }
				Delete(Line, Len(Line), 2);
				Window(2, 5, 79, 24);
			}
			else if (lo == 13) QFlag = 1;
			else if (lo > 0 && Len(Line) < 110)
			{
				WrC((char)lo);
				p = Len(Line); Line[p] = (char)lo; Line[p + 1] = 0;
			}
			else ScanKey(hi);
		} while (QFlag != 1);
		Cur(3);
		Gotoxy(1, 19); Col(5, 7); WrC((char)175); Col(11, 7); Gotoxy(1, 20);
		if (Len(Line) > 76) Wln();
		LowerCase(Line); Spaces(Line);
		if (Pos(" r ", Line) > 0 || Pos(" repeat ", Line) > 0) PreFormat(Line);
		if (StrEq(Line, "r") || StrEq(Line, "repeat")) StrCpy(Line, Again); else StrCpy(Again, Line);
		Spaces(Line);
		while (Pos(" then ", Line) > 0)
		{
			x = Pos(" then ", Line); Delete(Line, x, 5); Insert(".", Line, x, 131);
		}
		ChopSeven(Line);
		PreFormat(Line);
	}
	if (Pos(".", Line) > 0)    // separates the line into single inputs
	{
		Copy(Input, Line, 1, Pos(".", Line));
		Delete(Line, 1, Pos(".", Line));
		Delete(Input, Pos(".", Input), 1);
		PreFormat(Input);
	}
	else { StrCpy(Input, Line); StrCpy(Line, ""); }
	Spaces(Input);
	while (Pos(" it ", Input) > 0)
	{
		x = Pos(" it ", Input) + 1; Delete(Input, x, 2); Insert(LastNoun, Input, x, 131);
		PreFormat(Input); ChopSeven(Input);
	}
	while (Pos(" them ", Input) > 0)
	{
		x = Pos(" them ", Input) + 1; Delete(Input, x, 4); Insert(LastNoun, Input, x, 131);
		PreFormat(Input); ChopSeven(Input);
	}
	QFormat(Input);
	Col(11, 7);
}
