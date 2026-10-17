// Supernova (c) 1985-1990 Scott Miller / Apogee Software, Ltd. - GNU GPL v2 or later.
// MSX2 port: game state, Pascal-runtime helpers and the declarations shared by all game modules.
// Names follow the Pascal source (NOVA.PAS / ADPAR.PAS / DEFAULT.PAS) so the code can be compared.
#pragma once

#include "types.h"
#include "mapper.h"
#include "../data/vocab_data.h"
#include "../data/textdef.h"

#define VMax 95
#define NMax 133
#define PMax 11
#define AMax 4
#define TMax 30
#define RMax 109
#define MMax 29
#define Null 0
#define Legal 255

// ---- Pascal sets: bit (n-1) of byte (n-1)>>3 (same layout as the vocab_data location sets)
#define SET_IN(s, n)   (((s)[((n) - 1) >> 3] >> (((n) - 1) & 7)) & 1)
#define SET_ADD(s, n)  ((s)[((n) - 1) >> 3] |= (u8)(1 << (((n) - 1) & 7)))
#define SET_DEL(s, n)  ((s)[((n) - 1) >> 3] &= (u8)~(1 << (((n) - 1) & 7)))
u8   SetEmpty(const u8* s, u8 bytes);
void SetClear(u8* s, u8 bytes);

// `x in Inv` etc. for the sets of 1..MMax: elements outside 1..29 are never members (Pascal semantics)
#define IN29(s, n) ((n) >= 1 && (n) <= MMax && SET_IN(s, n))

#define IN133(s, n) ((n) >= 1 && (n) <= NMax && SET_IN(s, n))

// ---- game state (globals of NOVA.PAS; the Pascal code relies on x,y,o,i,j being shared)
extern u8  Mov[4], Inv[4], Socket[4], Wear[4];      // sets of 1..MMax
extern u8  Brief[14];                                // set of 1..RMax
extern u8  NounSet[17];                              // set of 1..NMax
extern u8  Events[32];                               // set of 1..255
extern u8  PStat[1];                                 // set of 1..6: 2 Hungry 3 Sick 4 Injured 5 Tired 6 Thirsty
extern u8  NoNounOnly[12], OneNounMaybe[12], ToNounOnly[12], ToNounMaybe[12]; // sets of 1..VMax
extern u8  MugCon, FoodCon, SatchCon, HolstCon, NicheCon, PyraCon, HingeCon, PodumCon, RobotCon;
extern u8  CodeSet, ScrnSet, Vb, N1, N2, Pr, Md, VNP, Prm, EFlag, FFlag, TFlag, list, Num;
extern u8  Region, AlienRm, FriendRm, SinkRm, Serum, Weight, Old, Old2, x, y, o;
extern i16 Code, Tic, StoreR, StoreS, StoreL, StoreC, RC, Sc, TestC, Floor, i, j;
extern u8  R[MMax + 1];                              // room each movable object is in
extern i16 T[TMax + 1];                              // timers
extern u8  Mx[4];                                    // m[1..3] = VMax, NMax, PMax
extern char Input[132], Again[132], Line[132];       // Str130
extern char Identity[30], VStr[30], NStr[30], PStr[30], LastNoun[30], Word[30]; // Str29
extern char Drive, Log, CFlag;
extern u8  Maze, Crawl, Color, QFlag, SFlag;
extern char Text5[80];                               // clue (C1) record
extern const char* const g_Dir[9];                   // Dir[1..8]

// ---- Pascal string helpers (1-based like Pascal; `max` = maximum string length)
u8   Len(const char* s);
u8   Pos(const char* sub, const char* s);
void Delete(char* s, u8 idx, u8 cnt);
void Insert(const char* sub, char* s, u8 idx, u8 max);
void Copy(char* dst, const char* s, u8 idx, u8 cnt);
void StrCpy(char* d, const char* s);
void StrCat(char* d, const char* s, u8 max);          // d := d + s (truncated to max)
u8   StrEq(const char* a, const char* b);
char UpCase(char c);

// ---- runtime services (rt_host.c for the PC test build, rt_msx.c for the MSX)
void Wr(const char* s);          // write(s)
void WrC(char c);                // write(c)
void WrNum(i16 n);               // write(n)
void Wln(void);                  // writeln
void TextColor(u8 c);            // 0-15, +16 = blink
void TextBackground(u8 c);       // 0-7
void Gotoxy(u8 xx, u8 yy);
u8   Wherex(void);
u8   Wherey(void);
void Window(u8 x1, u8 y1, u8 x2, u8 y2);
void ClrScr(void);
void InsLine(void);
void Sound(u16 hz);
void NoSound(void);
void Delay(u16 ms);
u16  GetKey(void);               // blocks; (scancode << 8) | ascii, like BIOS int 16h
u8   KeyPressed(void);
u16  Rnd(u16 n);                 // random(n)
void Randomize(void);
void ReadLn(char* s, u8 maxlen); // readln(string) with an edit buffer limit
u16  ReadText(u16 id, char* dst);// resident text reader (textpool): copies record `id`, returns its length
void CursorShape(u8 n);          // 1 underline, 2 block, 3 invisible
void Border(u8 colour);
u8   FileWrite2(const char* name, const u8* b1, u16 l1, const u8* b2, u16 l2);   // returns 1 on success
u16  FileRead2(const char* name, u8* b1, u16 l1, u8* b2, u16 l2);                // returns bytes read (l1 + l2 if complete), 0 if missing
void ListSaveFiles(void);        // writes the names of the saved games
u8   OvlLoadAll(void);           // loads the code overlays into mapper segments
void ClearData(void);            // zero the uninitialised variables (MSX start-up code does not)
void ScreenFlush(void);          // make the screen consistent (pending scrolls); GetKey does it too
void ScreenBegin(void);          // game start: video setup / palette
void Halt(void);

// ---- ADPAR.PAS helpers
void SF(void);
void Cur(u8 n);
void Col(u8 a, u8 b);
void Bak(u8 a, u8 b);
void Bor(u8 a, u8 b);
u8   En(u8 n);
void Add(u8 n);
void Min(u8 n);
void Score(i16 n, i16 pointer);
void Cn(const char* s);
void Pause(void);
void Tune(i16 octave, i16 note, i16 duration);
void Play(i16 start, i16 stop, i16 speed);
void RegionSoundOn(void);        // the background drone of regions 4 and 5
void Explode(u8 duration);
void Walls(u8 duration);
void Static(void);
void Blast(void);
void DOpen(u8 num);
void DClose(u8 num);
void Door(u8 New, u8 num);
void Time1(void);
void Time2A(void); void Time2B(void); void Time2C(void); void Time2D(void);
void Up(const char* w);          // writes w in capitals (red) - the Pascal function returned ''
void UpC(char c);
void Spaces(char* I);
void QFormat(char* I);
void PreFormat(char* I);
void LowerCase(char* I);
void ChopSeven(char* I);
void FindMood(char* input, char* word, u8* md);
void FN(u8 vnp, char* dst);      // first noun name of n[vnp]
void WrFN(u8 n);                 // write(FN(n))
void RC1(i16 pointer, char* dst);// reads clue record `pointer` (C1) into dst
u8   Here(u8 obj);
u8   Present(void);
void FindWord(char* I, u8* vnp, char* word, u8 max);
void Dictionary(u8 ifFound, u8 skipList);
void RL(i16 pointer); void RS(i16 pointer); void RR(i16 pointer);
void RB(u8 pointer, u8 colour); void RB2(u8 pointer, u8 colour);
void Won(void);
void PlayerInput(char* line);
void Title(void);
void Init1(void); void Init2(void); void Init3(void);
void Save(void); void Restore(void);
void MoreThanOne(void);
void PrintWord(const char* w);   // Print(Word): the Pascal function returned ''
void Parser_Syntax(char* input);
void Val(const char* s, i16* code, i16* err);   // Pascal val()
void TransportNoise(void);
void MC(u8 whichChar, u8 watchRoom, u8 toRoom, i16 messageNum);
void Van(u8 obj);
void Crazy(void);
void NoSense(void);
void Say(const char* what1, const char* what2);
void SetNoun(u8 n, u8 alt);       // n[n]:='...' (alt = index into the resident table of alternatives)

// ---- NOVA.PAS
void GameMain(void);
void RestartGame(void);          // Pascal: execute('nova.com')
void SetR(u8 obj, u8 room);      // r[obj]:=room, ignoring objects that are not movable (the Pascal code wrote past the array)
void Describe(u8 room);
void MoveTo(u8 New);             // Pascal: Move
void Room1(void);  void Room2(void);  void Room3(void);  void Room4(void);  void Room5(void);
void Room6(void);  void Room7(void);  void Room8(void);  void Room9(void);  void Room10(void);
void Room11(void); void Room12(void); void Room13(void); void Room14(void); void Room15(void);
void Room16(void); void Room17(void); void Room18(void); void Room19(void); void Room20(void);
void Room21(void); void Room22(void); void Room23(void);

// ---- DEFAULT.PAS
void Default0(void);  void Default1(void);  void Default2(void);  void Default3(void);
void Default4(void);  void Default5(void);  void Default6(void);  void Default7(void);
void Default8(void);  void Default9(void);  void Default10(void); void Default11(void);
void Default12(void); void Default13(void); void Default14(void); void DeadMain(void);
void DEAD(void);
void Call13(void);

// Pascal `x in [a..b]`
#define IN_RANGE(v, a, b) ((v) >= (a) && (v) <= (b))
