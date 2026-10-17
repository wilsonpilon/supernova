// Size-test runtime interface: Pascal-ish helpers used by hand-translated Supernova code
#pragma once
typedef unsigned char u8;
typedef unsigned int  u16;
typedef signed int    i16;

#define VMax 95
#define NMax 133
#define MMax 29
#define RMax 109

// ---- game state (globals, as in the Pascal source)
extern u8  Vb, N1, N2, Pr, Md, Prm, Region, EFlag, FFlag, TFlag, list, Serum, CodeSet;
extern u8  NicheCon, MugCon, SatchCon, HolstCon, RobotCon, FoodCon, PyraCon, HingeCon, PodumCon;
extern u8  x, y, o, VNP, SFlag, Crawl;
extern i16 T[31], Sc, i;
extern u8  R[MMax + 1];
extern u8  L[RMax + 1][17];     // set of 1..NMax per room
extern u8  Events[32], Inv[4], Wear[4], Socket[4], Brief[14], NounSet[17], PStat[1];
extern char Input[132], Line[132], Word[30], Identity[30], VStr[30], NStr[30];
extern char Vb_[VMax + 1][30], Nn[NMax + 1][30], Pp[12][30], Aa[5][54];
extern u8  Mx[4];

// ---- Pascal-like set helpers
u8   SetIn(const u8* s, u8 n);
void SetAdd(u8* s, u8 n);
void SetDel(u8* s, u8 n);
u8   SetEmpty(const u8* s, u8 bytes);

// ---- Pascal-like string helpers (1-based, like pos/delete/insert/copy)
u8   Pos(const char* sub, const char* s);
void Delete(char* s, u8 idx, u8 cnt);
void Insert(const char* sub, char* s, u8 idx);
void Copy(char* dst, const char* s, u8 idx, u8 cnt);

// ---- engine services (stubs in the size test; not counted)
void Wr(const char* s);
void Wln(void);
void RL(i16 n); void RS(i16 n); void RR(i16 n);
void RB(u8 n, u8 colour); void RB2(u8 n, u8 colour);
void Col(u8 a, u8 b);
void Gotoxy(u8 xx, u8 yy); u8 Wherey(void);
void sound(u16 f); void nosound(void); void delay(u16 ms);
void play(u16 a, u16 b, u16 c);
u16  Rnd(u16 n);
void Explode(u8 d); void Walls(u8 d);
void Door(u8 n, u8 num); void DOpen(u8 n);
void Pause(void); void DEAD(void); void Say(const char* a, const char* b);
void Van(u8 n); void Crazy(void);
void Score(u8 n, u8 ptr);
void Move(u8 n);
const char* FN(u8 n);
u8   Here(u8 obj);
#define En(n)   SetIn(Events, (n))
#define Add(n)  SetAdd(Events, (n))
#define Min(n)  SetDel(Events, (n))
u8   StrEq(const char* a, const char* b);
void StrCpy(char* d, const char* s);
void StrCat(char* d, const char* s);
