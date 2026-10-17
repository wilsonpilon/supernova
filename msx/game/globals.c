// Game state and Pascal runtime helpers (strings, sets)
#include "nova.h"

u8  Mov[4], Inv[4], Socket[4], Wear[4];
u8  Brief[14];
u8  NounSet[17];
u8  Events[32];
u8  PStat[1];
u8  NoNounOnly[12], OneNounMaybe[12], ToNounOnly[12], ToNounMaybe[12];
u8  MugCon, FoodCon, SatchCon, HolstCon, NicheCon, PyraCon, HingeCon, PodumCon, RobotCon;
u8  CodeSet, ScrnSet, Vb, N1, N2, Pr, Md, VNP, Prm, EFlag, FFlag, TFlag, list, Num;
u8  Region, AlienRm, FriendRm, SinkRm, Serum, Weight, Old, Old2, x, y, o;
i16 Code, Tic, StoreR, StoreS, StoreL, StoreC, RC, Sc, TestC, Floor, i, j;
u8  R[MMax + 1];
i16 T[TMax + 1];
u8  Mx[4];
char Input[132], Again[132], Line[132];
char Identity[30], VStr[30], NStr[30], PStr[30], LastNoun[30], Word[30];
char Drive, Log, CFlag;
u8  Maze, Crawl, Color, QFlag, SFlag;
char Text5[80];

static const char d1[] = "NORTH", d2[] = "SOUTH", d3[] = "EAST", d4[] = "WEST",
	d5[] = "NORTHEAST", d6[] = "NORTHWEST", d7[] = "SOUTHEAST", d8[] = "SOUTHWEST";
const char* const g_Dir[9] = { 0, d1, d2, d3, d4, d5, d6, d7, d8 };

u8 SetEmpty(const u8* s, u8 bytes) { while (bytes--) if (*s++) return 0; return 1; }
void SetClear(u8* s, u8 bytes) { while (bytes--) *s++ = 0; }

u8 Len(const char* s) { u8 n = 0; while (s[n]) n++; return n; }

u8 Pos(const char* sub, const char* s)
{
	u8 n = Len(sub), k, j;
	if (!n) return 0;
	for (k = 0; s[k]; k++)
	{
		j = 0;
		while (j < n && s[k + j] == sub[j]) j++;
		if (j == n) return k + 1;
	}
	return 0;
}

void Delete(char* s, u8 idx, u8 cnt)
{
	u8 l = Len(s), k;
	if (idx == 0 || idx > l) return;
	idx--;
	if (idx + cnt > l) cnt = l - idx;
	for (k = idx; k + cnt <= l; k++) s[k] = s[k + cnt];
}

void Insert(const char* sub, char* s, u8 idx, u8 max)
{
	u8 l = Len(s), n = Len(sub), k;
	if (idx == 0) idx = 1;
	if (idx > l + 1) idx = l + 1;
	if (l + n > max) n = max - l;          // Pascal truncates the result to the string's maximum
	idx--;
	for (k = l + 1; k > idx; k--) s[k + n - 1] = s[k - 1];
	for (k = 0; k < n; k++) s[idx + k] = sub[k];
}

void Copy(char* dst, const char* s, u8 idx, u8 cnt)
{
	u8 l = Len(s), k = 0;
	if (idx == 0) idx = 1;
	idx--;
	while (k < cnt && idx + k < l) { dst[k] = s[idx + k]; k++; }
	dst[k] = 0;
}

void StrCpy(char* d, const char* s) { while ((*d++ = *s++)); }

void StrCat(char* d, const char* s, u8 max)
{
	u8 l = Len(d);
	while (*s && l < max) d[l++] = *s++;
	d[l] = 0;
}

u8 StrEq(const char* a, const char* b) { while (*a == *b) { if (!*a) return 1; a++; b++; } return 0; }

char UpCase(char c) { return (c >= 'a' && c <= 'z') ? c - 32 : c; }
