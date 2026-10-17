// ADPAR.PAS title, initialisation, save / restore and the ending (overlay)
#include "nova.h"
#define CH(s, k) ((s)[(k) - 1])

void Won(void)
{
	const i16 W = 800, H = 400, Q = 200, Tt = 131;
	Wln();
	Gotoxy(1, 20); for (x = 1; x <= 5; x++) { Wln(); Delay(99); Sound(x * 50); Bor(x, 7); }
	Gotoxy(1, 15); Col(4, 15); Bak(1, 7);
	Wr("#######################################"); Wr("#######################################"); Wln();
	Delay(99); Sound(300); Bor(6, 0);
	Gotoxy(1, 16);
	for (x = 1; x <= 3; x++)
	{
		Wr("#                                      "); Wr("                                      #");
		Delay(99); Sound(300 + (x * 50)); Bor(x + 8, 15);
	}
	Gotoxy(1, 19);
	Wr("#######################################"); Wr("#######################################");
	Delay(99); Sound(500); Bor(14, 0);
	Gotoxy(26, 17); Col(31, 31);
	Wr("Y O U   H A V E   W O N ! !"); Delay(99); Sound(550);
	Gotoxy(1, 20); Bak(0, 0); Bor(2, 7); Wln(); Delay(99); NoSound();
	for (x = 1; x <= 12; x++) { Wln(); Delay(80); }
	Gotoxy(1, 9); Col(9, 9);
	Wr("                              S U P E R N O V A"); Wln(); Wln(); Col(11, 7);
	Wr("     Programmed by . . . . . . . . . . . . . . . . . . . . . Scott Miller"); Wln();
	Wr("     Story by  . . . . . . . . . . . . . . . . . . . . . . . Scott Miller"); Wln();
	Wr("     Additional story development. . . . . . . . . . . . . . . Terry Nagy"); Wln();
	Wln(); Wln();
	Vb = 78; Call13(); Wln(); Wln(); Col(3, 7);
	Wr("Press any key to quit...");
	Tune(2, 8, Q); Tune(2, 8, Q); Tune(3, 1, W);
	do
	{
		Tune(2, 8, Q); Tune(2, 8, Q);
		Tune(3, 1, H); Tune(2, 12, H); Tune(2, 10, H); Tune(2, 8, Q); Tune(2, 8, Q);
		Tune(3, 1, W); Tune(2, 8, Tt); Tune(2, 8, Tt); Tune(2, 8, Tt);
		if (KeyPressed()) goto JUMP;
		Tune(3, 1, H); Tune(2, 12, H); Tune(2, 10, H); Tune(2, 8, Tt); Tune(2, 8, Tt); Tune(2, 8, Tt);
		Tune(3, 3, W);
		if (KeyPressed()) goto JUMP;
		Tune(2, 10, Q); Tune(2, 10, Q);
		Tune(3, 3, H); Tune(3, 1, H); Tune(2, 12, H); Tune(2, 10, Q); Tune(2, 10, Q);
		if (KeyPressed()) goto JUMP;
		Tune(3, 3, W); Tune(2, 10, Tt); Tune(2, 10, Tt); Tune(2, 10, Tt);
		Tune(3, 3, H); Tune(3, 1, H); Tune(2, 12, H); Tune(2, 10, Tt); Tune(2, 10, Tt); Tune(2, 10, Tt);
		Tune(3, 1, W);
	} while (!KeyPressed());
JUMP:
	CFlag = (char)(GetKey() & 0xFF);
	Window(1, 1, 80, 25); ClrScr(); Gotoxy(2, 2); Col(31, 31); Bor(0, 0); Cur(1); Bak(2, 0);
	Wr("Congratulations!"); Wln();
	Halt();
}

void Title(void)
{
	ClrScr(); TextColor(7); Color = 1;
	Col(9, 9); Gotoxy(1, 1);
	Cn("S U P E R N O V A");
	Col(9, 7); Gotoxy(1, 3);
	Cn("Published by");
	Gotoxy(1, 5);
	Cn("APOGEE SOFTWARE PRODUCTIONS");
	Wln();
	Col(11, 7);
	Cn("This game is placed in the public domain for your enjoyment.   Please do");
	Cn("not abuse this product or the author's rights.");
	Wln();
	Cn("If you enjoy this game the author asks that you contribute $10 (by check).");
	Cn("This payment  will encourage the author  to create similar games  and will");
	Cn("help compensate him  for the several years work that went into  Supernova.");
	Cn("This fee will also register the payer for telephone support and clues.");
	Wln();
	Col(14, 15);
	Wr("Please make checks payable to:  Scott Miller"); Wln();
	Wln();
	Wr("Scott Miller      (214) 240-0614"); Wln();
	Wr("4206 Mayflower Drive"); Wln();
	Wr("Garland, TX 75043"); Wln();
	Wln();
	Wr("Also call for help:  Terry Nagy  (214) 271-3065"); Wln();
	Wln();
	Col(11, 7); Delay(7000);
	Cn("Thanks, enjoy the game...");

	Col(7, 7); Gotoxy(27, 25); Delay(999);
	Wr("Press any key to continue.");
	while (!KeyPressed()) { }
	CFlag = (char)(GetKey() & 0xFF); Bak(1, 0); ClrScr();
	// main SUPERNOVA title screen
	Bor(1, 0); Col(15, 15); Bak(4, 0);
	for (x = 1; x <= 80; x++) { Gotoxy(x, 1); WrC((char)205); Gotoxy(x, 24); WrC((char)205); }
	for (y = 1; y <= 24; y++) { Gotoxy(1, y); WrC((char)186); Gotoxy(80, y); WrC((char)186); }
	Gotoxy(1, 1); WrC((char)201); Gotoxy(80, 1); WrC((char)187);
	Gotoxy(1, 24); WrC((char)200); Gotoxy(80, 24); WrC((char)188);
	Bak(1, 0);
	Col(2, 7); Gotoxy(1, 10); Cn("Copyright 1987 Scott Miller");
	Col(14, 7); Gotoxy(1, 12); Cn("Version B");
	Col(7, 7); Gotoxy(1, 15);
	Cn("Programmed by Scott Miller");
	Cn("Story by Scott Miller and Terry Nagy");
	Gotoxy(1, 23); Col(3, 7);
	Cn("Press any key to continue.");
	do
	{
		Gotoxy(32, 8);
		TextColor((u8)Rnd(16));
		Wr("S U P E R N O V A");
	} while (!KeyPressed());
	CFlag = (char)(GetKey() & 0xFF);
}

void Init1(void)
{
	u8 l;
	for (;;)
	{
		Bor(0, 0); Bak(0, 0); ClrScr(); NoSound();
		Cur(3); Randomize();
		for (x = 1; x <= 20; x++) { Sound(x * 9); Delay(x * 2); }
		NoSound();
		Gotoxy(1, 9); y = 0; Col(14, 7); StrCpy(Identity, "");
		Cn("Please enter your identity code name:"); Col(12, 15);
		do i = Rnd(32767); while (!KeyPressed());
		do
		{
			CFlag = (char)(GetKey() & 0xFF);
			if (CFlag != 13)
			{
				if (CFlag != 8) { if (Len(Identity) < 29) { l = Len(Identity); Identity[l] = UpCase(CFlag); Identity[l + 1] = 0; } }
				else Delete(Identity, Len(Identity), 2);
			}
			{
				char b[36];
				Gotoxy(1, 11);
				StrCpy(b, " "); StrCat(b, Identity, 33); StrCat(b, " ", 34);
				Cn(b);
			}
			Sound(50); Delay(50); NoSound();
		} while (CFlag != 13);
		Col(10, 7); Gotoxy(1, 7);
		if (Identity[0])
		{
			char b[80];
			StrCpy(b, " IDENTITY CODE ACCEPTED--YOU MAY PROCEED "); StrCat(b, Identity, 79);
			Cn(b);
		}
		else { Col(25, 31); Bor(4, 7); Cn("IDENTITY CODE REJECTED--ABORTING SEQUENCE"); }
		LowerCase(Identity); ChopSeven(Identity);
		Delay(1500);
		if (Identity[0]) break;
	}
}

void Init2(void)
{
	Col(7, 15); Bak(1, 7);
	for (x = 1; x <= 80; x++) { Gotoxy(x, 1); WrC((char)205); Gotoxy(x, 24); WrC((char)205); }
	Gotoxy(80, 24); WrC((char)190); Gotoxy(1, 24); WrC((char)212);
	Gotoxy(1, 4); InsLine();
	for (x = 2; x <= 24; x++) { Gotoxy(1, x); WrC((char)179); Gotoxy(80, x); WrC((char)179); }
	Gotoxy(1, 4); WrC((char)198);
	for (x = 2; x <= 79; x++) { Gotoxy(x, 4); WrC((char)205); }
	WrC((char)181);
	Gotoxy(1, 1); WrC((char)213); Gotoxy(80, 1); WrC((char)184);
	Bak(4, 7); Col(14, 0); Gotoxy(2, 2);
	for (x = 1; x <= 78; x++) WrC(' ');
	Gotoxy(2, 2); Wr("Move");
	Gotoxy(68, 2); Wr("Score");
	Bak(7, 7); Gotoxy(2, 3);
	for (x = 1; x <= 78; x++) WrC(' ');
	Bak(5, 7); Col(15, 0);
	Gotoxy(2, 3); Wr("Player Condition:");
	Bak(0, 0);
	Gotoxy(1, 14); Col(14, 7);
	Cn("Working 14 hours a day in the core of some dusty, smelly mine");
	Cn("is not your idea of the perfect lifestyle.");
	Cn("Barre-An is a dust ball in space, its only salvation being that it is");
	Cn("rich in precious barre-an metal.  Or used to be.  Nowadays the mines");
	Cn("don't seem so generous, which is why you're looking for a more");
	Cn("profitable venture.");
	Cn("A break, that's all you ask for, maybe today you figure...");
	Wln();
}

void Init3(void)
{
	StrCpy(Line, "");
	StrCpy(Again, "z");
	StrCpy(LastNoun, "mug");
	Vb = Null;
	Prm = 8;
	Sc = 0;
	Tic = 0;
	SetClear(PStat, 1); SET_ADD(PStat, 6);
	SetClear(Events, 32);
	for (o = 1; o <= MMax; o++) R[o] = Null;
	SetClear(Inv, 4); SET_ADD(Inv, 3); SET_ADD(Inv, 8);
	SetClear(Mov, 4); for (o = 1; o <= MMax; o++) SET_ADD(Mov, o);
	AlienRm = Null;
	FriendRm = 91;
	SetClear(Brief, 14);
	SetClear(Wear, 4);
	MugCon = 99;
	FoodCon = 4;
	SatchCon = 6;
	HolstCon = Null;
	NicheCon = Null;
	SinkRm = Null;
	PyraCon = Null;
	Serum = Null;
	HingeCon = 9;
	PodumCon = 18;
	RobotCon = 12;
	SetClear(Socket, 4); for (o = 22; o <= 25; o++) SET_ADD(Socket, o);
	CodeSet = 7;
	ScrnSet = 1;
	Floor = 1;
	Region = 1;
	TFlag = 1;
	Old = 250;
	Old2 = Old;
	Maze = 1;
	Drive = 'A';
	StoreR = -1; StoreS = -1; StoreL = -1; StoreC = -1;
	Mx[1] = VMax; Mx[2] = NMax; Mx[3] = PMax;
	for (x = 1; x <= TMax; x++) T[x] = Null;
	T[2] = 0;
	T[3] = 70;    // Hunger
	T[4] = 26;    // Thirst
	T[5] = 85;    // Sleep (no relation to the T[2] sleep timer!)
	SetClear(NoNounOnly, 12); SetClear(OneNounMaybe, 12); SetClear(ToNounOnly, 12); SetClear(ToNounMaybe, 12);
	for (o = 1; o <= 8; o++) SET_ADD(NoNounOnly, o);
	SET_ADD(NoNounOnly, 15); SET_ADD(NoNounOnly, 16); SET_ADD(NoNounOnly, 30);
	for (o = 77; o <= 79; o++) SET_ADD(NoNounOnly, o);
	SET_ADD(NoNounOnly, 82); SET_ADD(NoNounOnly, 85); SET_ADD(NoNounOnly, 86); SET_ADD(NoNounOnly, 87); SET_ADD(NoNounOnly, 95);
	for (o = 9; o <= 14; o++) SET_ADD(OneNounMaybe, o);
	SET_ADD(OneNounMaybe, 25); SET_ADD(OneNounMaybe, 32); SET_ADD(OneNounMaybe, 46);
	SET_ADD(OneNounMaybe, 59); SET_ADD(OneNounMaybe, 60);
	for (o = 66; o <= 71; o++) SET_ADD(OneNounMaybe, o);
	SET_ADD(OneNounMaybe, 80); SET_ADD(OneNounMaybe, 81); SET_ADD(OneNounMaybe, 90); SET_ADD(OneNounMaybe, 91);
	SET_ADD(ToNounOnly, 33); SET_ADD(ToNounOnly, 49); SET_ADD(ToNounOnly, 64); SET_ADD(ToNounOnly, 88); SET_ADD(ToNounOnly, 93);
	SET_ADD(ToNounMaybe, 19);
	for (o = 41; o <= 44; o++) SET_ADD(ToNounMaybe, o);
	SET_ADD(ToNounMaybe, 48); SET_ADD(ToNounMaybe, 50); SET_ADD(ToNounMaybe, 53); SET_ADD(ToNounMaybe, 54);
	for (o = 74; o <= 76; o++) SET_ADD(ToNounMaybe, o);
	SET_ADD(ToNounMaybe, 89); SET_ADD(ToNounMaybe, 90); SET_ADD(ToNounMaybe, 92); SET_ADD(ToNounMaybe, 94);
	// all other verbs are OneNounOnly
	Window(2, 5, 79, 24); Gotoxy(1, 19);
}

// ---- save / restore (one .SAV file; the original wrote four .SM1-.SM4 files and asked for a disk drive)
// file = g_Loc (object locations), then STATE_SIZE bytes of the remaining variables
#define STATE_SIZE (18 + MMax + 8 + 2 * TMax + 4 + 1 + 32 + 4 + 4)

static u8* Put8(u8* p, u8 v) { *p++ = v; return p; }
static u8* Put16(u8* p, i16 v) { *p++ = (u8)v; *p++ = (u8)((u16)v >> 8); return p; }
static const u8* Get8(const u8* p, u8* v) { *v = *p++; return p; }
static const u8* Get16(const u8* p, i16* v) { *v = (i16)(p[0] | (p[1] << 8)); return p + 2; }

static void SaveName(const char* base, char* dst)
{
	StrCpy(dst, base); StrCat(dst, ".SAV", 12);
}

static void AskName(const char* prompt, const char* abort_)
{
	Wr(prompt); Wr(" ('"); Up("/"); Wr("' aborts "); Wr(abort_); Wr(")? ");
}

static u8 NameDialog(void)   // returns 0 if the user aborted; Input holds the base name (upper case)
{
	for (;;)
	{
		Col(14, 15); ReadLn(Input, 8); Col(11, 7);
		Cur(3);
		while (Pos(" ", Input) > 0) Delete(Input, Pos(" ", Input), 1);
		while (Pos(".", Input) > 0) Delete(Input, Pos(".", Input), 1);
		while (Pos(":", Input) > 0) Delete(Input, Pos(":", Input), 1);
		if (Pos("/", Input) > 0) return 0;
		LowerCase(Input);
		if (Pos("list", Input) > 0 || Pos("dir", Input) > 0 || Pos("file", Input) > 0)
		{
			Wln(); ListSaveFiles(); Wln(); Pause();
			Wr("Which file name? ");
			continue;
		}
		if (Len(Input) == 0) StrCpy(Input, "lastsave");
		for (x = 0; Input[x]; x++) Input[x] = UpCase(Input[x]);
		return 1;
	}
}

void Save(void)
{
	char fn[16];
	u8 st[STATE_SIZE];
	u8* p = st;
	SF(); NoSound();
	for (x = 1; x <= 20; x++) Wln();
	Bor(2, 7); Cur(2);
	Gotoxy(1, 2);
	AskName("SAVE under what file name", "SAVE");
	if (!NameDialog())
	{
		Wln(); Up("SAVE ABORTED"); Wln();
	}
	else
	{
		Wln(); Wln();
		Wr("The game file "); Up(Input); Wr(" is now being saved..."); Wln();
		p = Put8(p, Prm); p = Put8(p, Region); p = Put8(p, MugCon); p = Put8(p, FoodCon); p = Put8(p, SatchCon);
		p = Put8(p, NicheCon); p = Put8(p, PyraCon); p = Put8(p, HingeCon); p = Put8(p, PodumCon);
		p = Put8(p, AlienRm); p = Put8(p, FriendRm); p = Put8(p, SinkRm); p = Put8(p, Serum); p = Put8(p, TFlag);
		p = Put8(p, RobotCon); p = Put8(p, CodeSet); p = Put8(p, ScrnSet); p = Put8(p, HolstCon);
		for (x = 1; x <= MMax; x++) p = Put8(p, R[x]);
		p = Put16(p, Tic); p = Put16(p, Sc); p = Put16(p, RC); p = Put16(p, Floor);
		for (x = 1; x <= TMax; x++) p = Put16(p, T[x]);
		for (x = 0; x < 4; x++) p = Put8(p, Inv[x]);
		p = Put8(p, PStat[0]);
		for (x = 0; x < 32; x++) p = Put8(p, Events[x]);
		for (x = 0; x < 4; x++) p = Put8(p, Socket[x]);
		for (x = 0; x < 4; x++) p = Put8(p, Wear[x]);
		SaveName(Input, fn);
		Wln();
		if (FileWrite2(fn, (const u8*)g_Loc, sizeof(g_Loc), st, (u16)(p - st)))
		{
			Wr("Your present game location is now SAVED under the name "); Up(Input); Wr("."); Wln();
		}
		else { Col(12, 15); Wr("The game could not be saved!"); Wln(); Col(11, 7); }
	}
	Wln(); Wln(); Pause();
	Col(11, 7);
	Bor(0, 0); StrCpy(Line, "l");
	RegionSoundOn();
}

void Restore(void)
{
	char fn[16];
	u8 st[STATE_SIZE];
	const u8* p = st;
	SF(); NoSound();
	for (x = 1; x <= 25; x++) Wln();
	Bor(6, 7); Cur(2);
	Gotoxy(1, 2);
	AskName("RESTORE from what file name", "RESTORE");
	if (!NameDialog())
	{
		Wln(); Up("RESTORE ABORTED"); Wln(); Wln(); Pause(); Col(11, 7); Bor(0, 0); return;
	}
	Wln(); Wln();
	Wr("The game file "); Up(Input); Wr(" is now being restored..."); Wln();
	SaveName(Input, fn);
	if (FileRead2(fn, (u8*)g_Loc, sizeof(g_Loc), st, STATE_SIZE) != sizeof(g_Loc) + STATE_SIZE)
	{
		Col(12, 15); Wln(); Wln();
		{
			char b[60];
			StrCpy(b, "The file "); StrCat(b, Input, 20); StrCat(b, " does not exist!", 40);
			Cn(b);
		}
		Wr("\a"); Wln(); Delay(2000); Col(11, 7);
		Pause(); Bor(0, 0);
		return;
	}
	p = Get8(p, &Prm); p = Get8(p, &Region); p = Get8(p, &MugCon); p = Get8(p, &FoodCon); p = Get8(p, &SatchCon);
	p = Get8(p, &NicheCon); p = Get8(p, &PyraCon); p = Get8(p, &HingeCon); p = Get8(p, &PodumCon);
	p = Get8(p, &AlienRm); p = Get8(p, &FriendRm); p = Get8(p, &SinkRm); p = Get8(p, &Serum); p = Get8(p, &TFlag);
	p = Get8(p, &RobotCon); p = Get8(p, &CodeSet); p = Get8(p, &ScrnSet); p = Get8(p, &HolstCon);
	for (x = 1; x <= MMax; x++) p = Get8(p, &R[x]);
	p = Get16(p, &Tic); p = Get16(p, &Sc); p = Get16(p, &RC); p = Get16(p, &Floor);
	for (x = 1; x <= TMax; x++) p = Get16(p, &T[x]);
	for (x = 0; x < 4; x++) p = Get8(p, &Inv[x]);
	p = Get8(p, &PStat[0]);
	for (x = 0; x < 32; x++) p = Get8(p, &Events[x]);
	for (x = 0; x < 4; x++) p = Get8(p, &Socket[x]);
	for (x = 0; x < 4; x++) p = Get8(p, &Wear[x]);
	Add(126);
	Wln(); Wr("Your present game location is now RESTORED from the name "); Up(Input); Wr("."); Wln();
	Wln(); Wln(); Pause();
	Col(11, 7);
	Bor(0, 0);
	RegionSoundOn();
	if (Region > 1)
	{
		SetNoun(84, 0);
		SetNoun(126, 1);
	}
	else
	{
		SetNoun(84, 2);
		SetNoun(126, 3);
	}
	if (En(34)) SetNoun(18, 4);
	else SetNoun(18, 5);
	if (Prm > 79)
	{
		SetNoun(40, 6);
		SetNoun(82, 7);
		SetNoun(110, 8);
	}
	else
	{
		SetNoun(40, 9);
		SetNoun(82, 10);
		SetNoun(110, 11);
	}
	Min(128); StrCpy(Line, "l");
}
