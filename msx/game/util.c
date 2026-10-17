// ADPAR.PAS miscellaneous routines (resident part: console helpers, sounds, doors, text records, object bookkeeping)
#include "nova.h"
#define CH(s, k) ((s)[(k) - 1])

void SF(void) { SFlag = 1; }

void Cur(u8 n) { CursorShape(n); }
void Col(u8 a, u8 b) { (void)b; TextColor(a); }          // mono colour (b) is not used: always colour
void Bak(u8 a, u8 b) { (void)b; TextBackground(a); }
void Bor(u8 a, u8 b) { (void)b; Border(a); }

u8   En(u8 n) { return SET_IN(Events, n) != 0; }
void Add(u8 n) { SET_ADD(Events, n); }
void Min(u8 n) { SET_DEL(Events, n); }

void Score(i16 n, i16 pointer)
{
	if (!En((u8)pointer)) { Sc += n; Add((u8)pointer); }
}

void Cn(const char* s)
{
	Gotoxy(40 - (Len(s) / 2), Wherey());
	Wr(s); Wln();
}

void Pause(void)
{
	Col(15, 15); Wr("Press any key to continue...");
	CFlag = (char)(GetKey() & 0xFF);
	Col(11, 7); Wln();
}

// ---- sound
static const u16 g_NoteHz100[12] = { 3263, 3457, 3662, 3880, 4111, 4355, 4614, 4889, 5180, 5488, 5814, 6160 };

void Tune(i16 octave, i16 note, i16 duration)
{
	u32 f = g_NoteHz100[note - 1];
	i16 k;
	for (k = 1; k <= octave; k++) f *= 2;
	f = (f + 50) / 100;
	Sound((u16)f);
	if (duration != 0) { Delay(duration); NoSound(); }
}

void RegionSoundOn(void)
{
	if (Region == 4) Sound(20);
	if (Region == 5) Sound(60);
}

void Play(i16 start, i16 stop, i16 speed)
{
	i16 k;
	if (start <= stop) for (k = start; k <= stop; k++) { Sound(k); Delay(speed); }
	else for (k = start; k >= stop; k--) { Sound(k); Delay(speed); }
	NoSound(); RegionSoundOn();
}

void Explode(u8 duration)
{
	i16 k;
	for (k = duration * 999; k >= 20; k--) Sound(Rnd(k));
	NoSound();
}

void Walls(u8 duration)
{
	i16 k, n = duration * 999;
	for (k = 1; k <= n; k++) Sound(Rnd(35) + 20);
	NoSound();
}

void Static(void)
{
	i16 k, l, n;
	for (k = 1; k <= 50; k++)
		switch (Rnd(2))
		{
		case 0: n = Rnd(70) + 10; for (l = 1; l <= n; l++) Sound(Rnd(4000) + 3000); break;
		case 1: NoSound(); Delay(Rnd(29)); break;
		}
	NoSound(); if (Region == 5) Sound(60);
}

void Blast(void)
{
	u8 k;
	for (k = 1; k <= 40; k++) { Play(550 - k * 6, 600 + k * 6, 0); Play(600 + k * 6, 550 - k * 6, 0); }
	Explode(2);
}

void DOpen(u8 num)
{
	Wr("The door slides open..."); Wln();
	if (num != 0) Play(50, 125 - num, num);
	else { for (i = 3500; i <= 5000; i++) Sound(Rnd(4500) + i); NoSound(); }
	if (Region == 5) Sound(60);
}

void DClose(u8 num)
{
	Wr("The sliding door closes."); Wln();
	if (num != 0) Play(125 - num, 50, num);
	else { for (i = 5000; i >= 3500; i--) Sound(Rnd(4500) + i); NoSound(); }
	if (Region == 5) Sound(60);
}

void Door(u8 New, u8 num)
{
	if (En(7)) RL(22); else
	if (En(8)) RL(23); else { DOpen(num); MoveTo(New); DClose(num); }
}

// ---- time events
void WrFN(u8 n)
{
	const char* s = g_N[n];
	while (*s && *s != '\\') WrC(*s++);
}

void FN(u8 vnp, char* dst)
{
	const char* s = g_N[vnp];
	SF();
	while (*s && *s != '\\') *dst++ = *s++;
	*dst = 0;
}

void UpC(char c)
{
	char t[2];
	t[0] = c; t[1] = 0;
	Up(t);
}

void Up(const char* w)
{
	char b[136];
	u8 n;
	StrCpy(b, w);
	if (b[0] == 'l' && b[1] == 0) Insert("ook", b, 2, 131);          // Word+' ' check: 'l' alone
	else if (b[0] == 'l' && b[1] == ' ') Insert("ook", b, 2, 131);
	if (b[0] == 'e' && b[1] == 'x' && b[2] == 0) Insert("amine", b, 3, 131);
	else if (b[0] == 'e' && b[1] == 'x' && b[2] == ' ') Insert("amine", b, 3, 131);
	n = Len(b);
	for (x = 1; x <= n; x++) b[x - 1] = UpCase(b[x - 1]);
	Col(12, 15); Wr(b); Col(11, 7);
}

void Spaces(char* I)
{
	Insert(" ", I, 1, 131);
	StrCat(I, " ", 131);
}

void QFormat(char* I)
{
	u8 l;
	if (I[0] == '.' || I[0] == ' ') Delete(I, 1, 1);
	l = Len(I);
	if (l && (I[l - 1] == '.' || I[l - 1] == ' ')) Delete(I, l, 1);
}

void LowerCase(char* I)
{
	u8 n = Len(I);
	for (x = 1; x <= n; x++)
		if (CH(I, x) >= 'A' && CH(I, x) <= 'Z') CH(I, x) += 32;
}

u8 Here(u8 obj)
{
	u8 r = 0;
	if (LOC_HAS(Prm, obj)) r = 1;
	if (obj == 79 && MugCon == 79 && IN29(Inv, 29)) r = 1;
	if (obj <= MMax) if (R[obj] == Prm || IN29(Inv, obj)) r = 1;
	if (obj == 58 && (IN29(Inv, 3) || IN29(Inv, 4) || R[4] == Prm || R[3] == Prm)) r = 1;
	return r;
}

#include "mapper.h"
// the scratch area (page 3) holds the text record being displayed
AT(0xD000) u8 g_Scratch[1400];
#define g_Rec ((char*)g_Scratch)

static void WrRec(const char* s)
{
	Wr(s);
}

void RL(i16 pointer) { SF(); ReadText(TK_L + pointer, g_Rec); WrRec(g_Rec); Wln(); }
void RS(i16 pointer) { SF(); ReadText(TK_S + pointer, g_Rec); WrRec(g_Rec); Wln(); }
void RR(i16 pointer) { SF(); ReadText(TK_R + pointer, g_Rec); WrRec(g_Rec); Wln(); }
void RC1(i16 pointer, char* dst) { ReadText(TK_C + pointer, dst); }      // C1 clue (read into Text5 by DEFAULT.PAS)

void RB(u8 pointer, u8 colour)
{
	SF(); Col(colour, 7);
	ReadText(TK_M + pointer, g_Rec); WrRec(g_Rec); Wln();
	Col(11, 7);
}

void RB2(u8 pointer, u8 colour)
{
	SF(); Col(colour, 7);
	ReadText(TK_B + pointer, g_Rec); WrRec(g_Rec); Wln();
	Col(11, 7);
}

// ---- keyboard helper used by MoreThanOne
void Val(const char* s, i16* code, i16* err)
{
	i16 v = 0, k = 0;
	u8 neg = 0;
	if (s[0] == '-') { neg = 1; k = 1; }
	else if (s[0] == '+') k = 1;
	if (!s[k]) { *code = 0; *err = k + 1; return; }
	for (; s[k]; k++)
	{
		if (s[k] < '0' || s[k] > '9') { *code = 0; *err = k + 1; return; }
		v = v * 10 + (s[k] - '0');
	}
	*code = neg ? -v : v;
	*err = 0;
}

void MC(u8 whichChar, u8 watchRoom, u8 toRoom, i16 messageNum)
{
	if (Prm == watchRoom) RS(messageNum);
	switch (whichChar)    // 1 = Aliens, 2 = Scientist
	{
	case 1: LOC_DEL(AlienRm, 124); AlienRm = toRoom; LOC_ADD(AlienRm, 124); break;
	case 2: LOC_DEL(FriendRm, 123); FriendRm = toRoom; LOC_ADD(FriendRm, 123); break;
	}
	SFlag = 0;
}

void Van(u8 obj)
{
	if (obj == 0) return;    // Van(Null): only touched variables that are already Null
	if (obj <= MMax) SET_DEL(Inv, obj);
	SetR(obj, Null);
	LOC_DEL(Prm, obj);
	if (IN29(Wear, obj)) SET_DEL(Wear, obj);
	if (obj == SatchCon) SatchCon = Null;
	if (obj == MugCon) MugCon = Null;
	if (obj == 16) Min(6);
	if (obj == NicheCon) NicheCon = Null;
	if (obj == PyraCon) PyraCon = Null;
	if (obj == HingeCon) HingeCon = Null;
	if (obj == PodumCon) PodumCon = Null;
	if (obj == 16) { Min(37); Min(6); }
	if (obj == RobotCon) RobotCon = Null;
	if (IN29(Socket, obj)) SET_DEL(Socket, obj);
	if (obj == HolstCon) HolstCon = Null;
}

void Crazy(void) { SF(); RL(Rnd(7) + 127); }

void NoSense(void) { RL(190); }

void Say(const char* what1, const char* what2)
{
	SF();
	Wr("The "); Wr(what1); Wr(" is already "); Wr(what2); Wr("."); Wln();
}

// the transporter's rising whine (x runs to 800 here, so these are not the byte variables)
void TransportNoise(void)
{
	i16 xx, yy;
	for (yy = 60; yy >= 1; yy--) for (xx = 800; xx >= 1; xx--) Sound((u16)(xx * yy));
	NoSound();
}

// The Pascal source reassigned entries of the noun list with string literals. Overlay code cannot keep
// pointers into itself (page 2 is swapped), so the alternatives live here, in resident memory.
static const char* const g_NounAlt[] =
{
	"reactor regulat\\",
	"hinged mouth\\mouth\\hinge\\",
	"middle table\\middle\\",
	"bar\\",
	"glass ball\\ball\\glass\\",
	"dusty ball\\ball\\dusty\\",
	"sockets\\socket\\",
	"laser beam\\beam\\laser\\",
	"speaker\\",
	"cyan button\\cyan\\",
	"solar map\\map\\solar\\drawing\\",
	"keyhole\\",
	"podium\\alter\\column\\",
	"pyramid\\podium\\",
	"laser beam\\laser\\beam\\",
};

void SetNoun(u8 n, u8 alt) { g_N[n] = g_NounAlt[alt]; }
