// PC test runtime: runs the game logic with plain stdin/stdout so the translation can be checked
// against walkthroughs. Build: see host/build.sh. Text records are read from the generated pools.
#include "../game/nova.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

static FILE* g_Log;

void Wr(const char* s)
{
	for (; *s; s++)
	{
		if (*s == '\a') continue;
		if (*s == 8) continue;
		putchar(*s);
	}
}
void WrC(char c) { if (c == 8 || c == 7) return; putchar(c); }
void WrNum(i16 n) { printf("%d", n); }
void Wln(void) { putchar('\n'); }
void TextColor(u8 c) { (void)c; }
void TextBackground(u8 c) { (void)c; }
void Gotoxy(u8 xx, u8 yy) { (void)xx; (void)yy; }
u8   Wherex(void) { return 5; }
u8   Wherey(void) { return 20; }
void Window(u8 a, u8 b, u8 c, u8 d) { (void)a; (void)b; (void)c; (void)d; }
void ClrScr(void) { printf("\n[clrscr]\n"); }
void InsLine(void) { }
void Sound(u16 hz) { (void)hz; }
void NoSound(void) { }
void Delay(u16 ms) { (void)ms; }
void CursorShape(u8 n) { (void)n; }
void Border(u8 c) { (void)c; }
void ScreenBegin(void) { }
void ScreenFlush(void) { }

static int g_Eof;

u16 GetKey(void)
{
	int c = getchar();
	if (c == EOF) { fflush(stdout); fprintf(stderr, "\n[EOF on input]\n"); exit(g_Eof ? 3 : 0); }
	if (c == '\n') c = 13;
	return (u16)c;
}
u8 KeyPressed(void) { return 1; }

static unsigned long g_Seed = 12345;
void Randomize(void) { }
u16 Rnd(u16 n)
{
	g_Seed = g_Seed * 1103515245UL + 12345UL;
	return n ? (u16)((g_Seed >> 16) % n) : 0;
}

void ReadLn(char* s, u8 maxlen)
{
	u8 n = 0;
	int c;
	while ((c = getchar()) != EOF && c != '\n') if (n < maxlen) s[n++] = (char)c;
	s[n] = 0;
}

void Halt(void) { fflush(stdout); exit(0); }

u8 FileWrite2(const char* name, const u8* b1, u16 l1, const u8* b2, u16 l2)
{
	FILE* f = fopen(name, "wb");
	if (!f) return 0;
	fwrite(b1, 1, l1, f); fwrite(b2, 1, l2, f); fclose(f);
	return 1;
}
u16 FileRead2(const char* name, u8* b1, u16 l1, u8* b2, u16 l2)
{
	u16 n;
	FILE* f = fopen(name, "rb");
	if (!f) return 0;
	n = (u16)fread(b1, 1, l1, f);
	if (n == l1) n += (u16)fread(b2, 1, l2, f);
	fclose(f);
	return n;
}
static u16 FileRead(const char* name, u8* buf, u16 len)
{
	u16 n;
	FILE* f = fopen(name, "rb");
	if (!f) return 0;
	n = (u16)fread(buf, 1, len, f); fclose(f);
	return n;
}
void ListSaveFiles(void) { }

// ---- text pools (same files and decoder as the MSX build)
static u8  g_Pool[16][16384];
static u16 g_PoolFirst[16];
static u8  g_PoolCount;
static u8  g_Tok[2048];
static int g_Loaded;

static void LoadPools(void)
{
	u8 idx[64];
	char name[64];
	int i;
	const char* dir = getenv("NOVA_DATA");
	if (!dir) dir = "data";
	sprintf(name, "%s/TXT.IDX", dir);
	if (!FileRead(name, idx, sizeof(idx))) { fprintf(stderr, "cannot read %s\n", name); exit(2); }
	sprintf(name, "%s/TXT.TOK", dir);
	if (!FileRead(name, g_Tok, sizeof(g_Tok))) { fprintf(stderr, "cannot read %s\n", name); exit(2); }
	g_PoolCount = idx[0];
	for (i = 0; i < g_PoolCount; i++)
	{
		g_PoolFirst[i] = idx[1 + 2 * i] | (idx[2 + 2 * i] << 8);
		sprintf(name, "%s/TXT%d.BIN", dir, i);
		if (!FileRead(name, g_Pool[i], 16384)) { fprintf(stderr, "cannot read %s\n", name); exit(2); }
	}
	g_Loaded = 1;
}

u16 ReadText(u16 id, char* dst)
{
	u8 p;
	u16 i, n, off;
	const u8* r;
	char* d = dst;
	if (!g_Loaded) LoadPools();
	p = g_PoolCount;
	while (p && id < g_PoolFirst[--p]) {}
	i = id - g_PoolFirst[p];
	if (i >= (u16)(g_Pool[p][0] | (g_Pool[p][1] << 8))) { dst[0] = 0; return 0; }
	off = g_Pool[p][2 + 2 * i] | (g_Pool[p][3 + 2 * i] << 8);
	r = g_Pool[p] + off;
	n = r[0] | (r[1] << 8);
	r += 2;
	while (*r)
	{
		u8 c = *r++;
		u16 tp = g_Tok[2 * c] | (g_Tok[2 * c + 1] << 8);
		if (tp)
		{
			u8 l = g_Tok[tp];
			memcpy(d, &g_Tok[tp + 1], l);
			d += l;
		}
		else *d++ = (char)c;
	}
	*d = 0;
	return n;
}

static void OnTrap(int sig)
{
	fprintf(stderr, "\n[trap %d] Prm=%d Vb=%d N1=%d N2=%d Pr=%d Md=%d EFlag=%d Input='%s' Line='%s'\n",
		sig, Prm, Vb, N1, N2, Pr, Md, EFlag, Input, Line);
	exit(9);
}

int main(int argc, char** argv)
{
	signal(SIGILL, OnTrap); signal(SIGSEGV, OnTrap);
	const char* s = getenv("NOVA_SEED");
	(void)argc; (void)argv; (void)g_Log;
	if (s) g_Seed = strtoul(s, 0, 10);
	GameMain();
	return 0;
}
