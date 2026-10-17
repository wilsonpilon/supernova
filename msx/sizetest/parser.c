// Hand translation of Supernova engine pieces (NOVA.PAS Describe/Move, ADPAR.PAS parser, DEFAULT.PAS Default1) -- size test
#include "nova.h"

extern char RN[RMax + 1][20];
extern u8 PrefixTab;

static char* s_I;   // string being formatted (the "var I" of PreFormat)

static void QFormat(char* I)
{
	u8 l;
	if (I[0] == '.' || I[0] == ' ') Delete(I, 1, 1);
	l = 0; while (I[l]) l++;
	if (l && (I[l - 1] == '.' || I[l - 1] == ' ')) Delete(I, l, 1);
}

static void Spaces(char* I)
{
	u8 l = 0;
	while (I[l]) l++;
	Insert(" ", I, 1);
	I[l + 1] = ' '; I[l + 2] = 0;
}

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
	s_I = I;
	D("  ", 1); QFormat(I);
	FFlag = 0; if (I[0]) FFlag = 1;
	Spaces(I);
	Crawl = (Pos(" crawl ", I) > 0); D(" crawl ", 6);
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
	{
		u8 p;
		while ((p = Pos(",", I)) != 0) { Insert(" ", I, p); Delete(I, Pos(",", I), 1); }
	}
	D("..", 1); D(" .", 1); D("  ", 1);
	QFormat(I);
	if (!I[0]) {
		EFlag = 0;
		switch (FFlag) {
		case 2: RL(193); break;
		case 3: RL(186); break;
		default: Wr("Pardon me?"); Wln();
		}
	}
}

void LowerCase(char* I)
{
	for (x = 0; I[x]; x++)
		if (I[x] >= 'A' && I[x] <= 'Z') I[x] += 32;
}

void ChopSeven(char* I)
{
	char W[132];
	u8 k, l;
	if (!I[0]) return;
	l = 0; while (I[l]) l++;
	I[l] = ' '; I[l + 1] = 0;
	x = 1;
	do {
		k = 0;
		while (I[x - 1] != ' ' && I[x - 1] != '.' && I[x - 1] != ',') { W[k++] = I[x - 1]; x++; }
		W[k] = 0;
		if (k > 7) {
			y = Pos(W, I); x = x + (7 - k);
			Delete(I, y, k); Delete(W, 8, 130);
			Insert(W, I, y);
		}
		x++;
		l = 0; while (I[l]) l++;
	} while ((u8)(x - 1) != l);
	Delete(I, l, 1);
}

void FindMood(char* input, char* word, u8* md)
{
	char Temp1[54], Temp2[30], tmp[34];
	char counter;
	Spaces(input); x = 0;
	while (x < 4) {
		x++;
		counter = '1';
		StrCpy(Temp1, Aa[x]);
		while (Pos(&counter, Temp1) > 0) {   // (single-char string; simplification)
			Copy(Temp2, Temp1, 1, Pos(&counter, Temp1) - 1);
			tmp[0] = ' '; tmp[1] = 0; StrCat(tmp, Temp2); StrCat(tmp, " ");
			if (Pos(tmp, input) > 0) {
				StrCpy(word, Temp2);
				*md = x; if (*md == 2) *md = 1;
				x = 4; counter = '8';
				Delete(input, Pos(Temp2, input), 1 + 0);
			}
			Delete(Temp1, 1, Pos(&counter, Temp1));
			counter++;
		}
	}
	QFormat(input);
}

const char* FN(u8 vnp)
{
	static char Temp[30];
	SFlag = 1;
	Copy(Temp, Nn[vnp], 1, Pos("\\", Nn[vnp]) - 1);
	return Temp;
}

u8 Here(u8 obj)
{
	u8 r = 0;
	if (SetIn(L[Prm], obj)) r = 1;
	if (obj == 79 && MugCon == 79 && SetIn(Inv, 29)) r = 1;
	if (obj <= MMax) if (R[obj] == Prm || SetIn(Inv, obj)) r = 1;
	if (obj == 58 && (SetIn(Inv, 3) || SetIn(Inv, 4) || R[4] == Prm || R[3] == Prm)) r = 1;
	return r;
}

u8 Present(void)
{
	x = 0;
	if (!(Vb == 17 || Vb == 18 || Vb == 37 || Vb == 39)) {
		if (N1 != 0) {
			if (Here(N1)) {
				if (N2 != 0) {
					if (Here(N2)) return 1;
					Wr("You can't see any "); Wr(FN(N2)); Wr(" here."); Wln();
				} else return 1;
			} else { Wr("You can't see any "); Wr(FN(N1)); Wr(" here."); Wln(); }
		} else return 1;
	} else {
		for (x = 1; x <= NMax; x++)
			if (SetIn(NounSet, x) && !Here(x)) {
				Wr("You can't see any "); Wr(FN(x)); Wr(" here."); Wln();
				return 0;
			}
		return 1;
	}
	return 0;
}

static void Convert(u8* n, u8 Max)
{
	switch (Max) {
	case 1:
		switch (*n) {
		case 12: *n = 11; break; case 14: *n = 13; break; case 23: *n = 22; break; case 27: *n = 26; break;
		case 29: *n = 28; break; case 34: *n = 33; break; case 36: *n = 35; break; case 38: *n = 37; break;
		case 40: *n = 39; break; case 43: *n = 42; break; case 45: *n = 44; break;
		case 52: *n = 51; break; case 55: *n = 54; break; case 57: case 58: *n = 56; break;
		case 60: case 61: *n = 59; break; case 63: *n = 62; break;
		}
		break;
	case 2:
		switch (*n) {
		case 13: case 14: case 15: *n = 12; break; case 21: *n = 20; break; case 33: *n = 32; break;
		case 36: *n = 35; break; case 39: *n = 38; break; case 45: *n = 44; break;
		case 50: *n = 49; break; case 53: *n = 52; break; case 65: *n = 64; break;
		case 67: *n = 66; break; case 70: *n = 69; break; case 76: case 77: *n = 75; break;
		case 97: *n = 96; break; case 101: case 102: *n = 100; break; case 114: *n = 113; break;
		case 117: *n = 116; break; case 63: *n = 62; break;
		case 27: *n = 26; break; case 57: *n = 56; break; case 61: *n = 60; break;
		case 91: *n = 90; break; case 105: case 106: case 107: case 108: *n = 109; break; case 31: *n = 28; break;
		case 133: *n = 46; break; case 84: if (Prm > 21) *n = 128; break;
		case 115: if (Prm == 28) *n = 112; break;
		case 58: if (Prm == 52) *n = 64; break;
		}
		break;
	case 3:
		switch (*n) { case 2: *n = 1; break; case 4: *n = 3; break; case 8: *n = 7; break; }
		break;
	}
}

static void FindWord(char* I, u8* vnp, char* word, u8 Max)
{
	u8 j, ps;
	char Temp1[30], Temp2[30], tmp[34];
	QFormat(I); Spaces(I); j = 0;
	while (j < Mx[Max]) {
		j++;
		switch (Max) { case 1: StrCpy(Temp1, Vb_[j]); break; case 2: StrCpy(Temp1, Nn[j]); break; case 3: StrCpy(Temp1, Pp[j]); break; }
		ps = Pos("\\", Temp1);
		while (ps > 0) {
			Copy(Temp2, Temp1, 1, ps - 1);
			tmp[0] = ' '; tmp[1] = 0; StrCat(tmp, Temp2); StrCat(tmp, " ");
			{
				char cmp[34];
				Copy(cmp, I, 1, ps + 1);
				if (StrEq(cmp, tmp)) {
					*vnp = j;
					Convert(vnp, Max);
					StrCpy(word, Temp2);
					Delete(I, 1, ps);
					if (Max == 1) StrCpy(VStr, word); else if (Max == 2) StrCpy(NStr, word);
					j = Mx[Max];
					StrCpy(Temp1, "X");
				}
			}
			Delete(Temp1, 1, ps);
			ps = Pos("\\", Temp1);
		}
	}
	QFormat(I);
}

void Dictionary(u8 ifFound, u8 skipList)
{
	u8 stop = 1;
	VNP = 0; list = 1;
	while (list < 4 && stop == 1) {
		if (list == skipList) list++;
		else {
			FindWord(Input, &VNP, Word, list);
			if (VNP != 0) { EFlag = ifFound; list--; stop = 0; }
			list++;
		}
	}
	if (EFlag != ifFound) {
		char tmp[40];
		EFlag = 5; StrCat(Input, " ");
		Copy(Word, Input, 1, Pos(" ", Input) - 1);
		tmp[0] = ' '; tmp[1] = 0; StrCat(tmp, Word); StrCat(tmp, " ");
		if (Pos(tmp, " top directi next some from is under underne leaning but speak pay ") > 0) EFlag = ifFound;
		else if (Word[0] >= '0' && Word[0] <= '9') EFlag = 17;
		if (ifFound == 14) EFlag = 14;
		QFormat(Input);
	}
}

void Describe(u8 room)
{
	Col(9, 9); Wr(RN[Prm]); Col(11, 7); Wln();
	if (En(1) || (!En(1) && !SetIn(Brief, Prm))) {
		SetAdd(Brief, Prm);
		switch (Prm) {
		case 29: if (En(35)) { RS(130); RS(131); } else RR(29); break;
		case 34: if (En(30)) { RS(81); RS(82); } else RR(34); break;
		case 52: if (En(39)) RS(126); else RR(52); break;
		case 91: if (En(51)) { RS(157); RS(158); } else RR(91); break;
		default: RR(room);
		}
	}
	switch (Prm) {
	case 1: if (SetIn(L[1], 16)) RL(150); break;
	case 107: if (SetIn(L[107], 19)) RL(518); break;
	case 38: if (SetIn(L[38], 17)) RL(260); break;
	case 41: if (SetIn(L[41], 18) && !En(34)) RS(96); break;
	case 55: if (SetIn(L[55], 11)) RL(290); break;
	case 63: if (SetIn(L[63], 20)) RL(356); break;
	case 73: if (SetIn(L[73], 28)) RL(352); break;
	case 80: if (SetIn(L[80], 17)) RL(395); break;
	case 94: if (SetIn(L[94], 26)) RL(397); if (SetIn(L[94], 2)) RL(398); break;
	}
	for (o = 1; o <= MMax; o++)
		if (R[o] == Prm) {
			if (o == 6) { Wr("There is some "); Wr(FN(o)); Wr(" here."); }
			else {
				const char* w = FN(o);
				if (w[0] == 'a' || w[0] == 'e' || w[0] == 'i' || w[0] == 'o' || w[0] == 'u')
				{ Wr("There is an "); Wr(FN(o)); Wr(" here."); }
				else { Wr("There is a "); Wr(FN(o)); Wr(" here."); }
			}
			if ((o == 29 && MugCon != 0) || (o == 7 && HolstCon != 0) || (o == 5 && SatchCon != 0)) Wr("  (with something in it)");
			if (o == 5 && R[5] == 20) Wr("  (lying in some trash)");
			if (o == 7 && SetIn(L[12], 7) && R[7] == 12) Wr("  (on the shelf)");
			if (o == NicheCon) Wr("  (on the niche)");
			if (En(6) && o == 16) Wr("  (on the seat)");
			if (o == PyraCon) Wr("  (in the pyramid's slot)");
			if (o == HingeCon) Wr("  (inside the statue's mouth)");
			if (o == PodumCon) Wr("  (on top of the podium)");
			if (o == 16 && En(37)) Wr("  (under the crystal)");
			if (o == RobotCon) Wr("  (in the robot's panel)");
			if (SetIn(Socket, o)) Wr("  (in one of the regulator's sockets)");
			Wln();
		}
}

void MoveTo(u8 New)
{
	SFlag = 1;
	if (En(7)) RL(22);
	else if (En(8)) RL(23);
	else {
		if (MugCon != 0 && SetIn(Inv, 29)) { if (Prm != 8) SetDel(L[Prm], MugCon); SetAdd(L[New], MugCon); }
		if (SatchCon != 0 && SetIn(Inv, 5)) { SetDel(L[Prm], SatchCon); SetAdd(L[New], SatchCon); }
		if (HolstCon != 0 && SetIn(Inv, 7)) { SetDel(L[Prm], HolstCon); SetAdd(L[New], HolstCon); }
		Prm = New;
		Describe(Prm);
	}
}

void Default1(void)
{
	switch (Vb) {
	case 16:
		if (!SetEmpty(Inv, 4)) {
			Wr("You have in your possession..."); Wln();
			for (o = 1; o <= MMax; o++) {
				if (SetIn(Inv, o)) {
					if (o == 6) { Wr("  some "); Wr(FN(o)); } else { Wr("  a "); Wr(FN(o)); }
					if (SetIn(Wear, o)) Wr("  (being worn)");
					switch (o) {
					case 5: if (SatchCon != 0) Wr("  (contains something)"); break;
					case 7: if (HolstCon != 0) Wr("  (contains something)"); break;
					case 29: if (MugCon != 0) Wr("  (has something in it)"); break;
					}
					Wln();
				}
			}
		} else RL(28);
		break;
	case 18:
		if (!SetIn(NounSet, 30)) {
			for (o = 1; o <= NMax; o++)
				if (SetIn(NounSet, o)) {
					if (SetIn(Inv, o)) {
						Wr(FN(o)); Wr(":  ");
						if (SetIn(Wear, o)) { Wr("First you must remove the "); Wr(FN(o)); Wr("."); Wln(); }
						else { Wr("Dropped."); Wln(); SetDel(Inv, o); R[o] = Prm; }
					} else { Wr("You don't have the "); Wr(FN(o)); Wr("."); Wln(); }
				}
		} else {
			Min(17);
			for (o = 1; o <= MMax; o++)
				if (SetIn(Inv, o)) {
					Wr(FN(o)); Wr(":  "); Add(17);
					if (SetIn(Wear, o)) { Wr("First you must remove the "); Wr(FN(o)); Wr("."); Wln(); }
					else { Wr("Dropped."); Wln(); SetDel(Inv, o); R[o] = Prm; }
				}
			if (!En(17)) RL(303);
		}
		break;
	case 17:
		for (o = 1; o <= MMax; o++)
			if (SetIn(Inv, o) && !SetIn(NounSet, o)) {
				Wr(FN(o)); Wr(":  ");
				if (SetIn(Wear, o)) { Wr("First you must remove the "); Wr(FN(o)); Wr("."); Wln(); }
				else { Wr("Dropped."); Wln(); SetDel(Inv, o); R[o] = Prm; }
			}
		break;
	}
}
