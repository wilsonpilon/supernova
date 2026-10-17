#include "textpool.h"
#include "dos.h"
#include "dos_mapper.h"
#include "game/mapper.h"

#define POOL_ADDR 0x8000

static u8  g_PoolCount;
static u16 g_PoolFirst[TXT_MAXPOOLS];
static u8  g_PoolSeg[TXT_MAXPOOLS];

// Token table (TXT.TOK), kept in page 3 (not touched by page-2 swaps).
// g_TokBuf holds the file; g_TokHi/g_TokLo[c] = address of the length-prefixed expansion of code c
// (Hi = 0: literal byte). Both are page aligned for the asm decoder.
#define TOK_BUF_SIZE 700           // expansions only (TXT.TOK is 512 bytes of pointers + the expansions)
__at(0xCB00) static u8 g_TokHi[256];
__at(0xCC00) static u8 g_TokLo[256];
__at(0xCD00) static u8 g_TokBuf[TOK_BUF_SIZE];

// Expands the 0-terminated compressed string at src (page 2) into dst.
static void Decode(const u8* src, char* dst) __naked
{
	src; dst; // HL, DE
__asm
dec_loop:
	ld		a, (hl)
	inc		hl
	or		a
	jr		z, dec_done
	ld		c, a
	ld		b, #0xCB			// g_TokHi
	ld		a, (bc)
	or		a
	jr		z, dec_lit
	push	hl
	ld		h, a
	inc		b					// g_TokLo
	ld		a, (bc)
	ld		l, a
	ld		c, (hl)				// length
	inc		hl
	ld		b, #0
	ldir
	pop		hl
	jr		dec_loop
dec_lit:
	ld		a, c
	ld		(de), a
	inc		de
	jr		dec_loop
dec_done:
	ld		(de), a				// terminating 0
	ret
__endasm;
}

static u16 ReadFileLen(const char* name, void* dst, u16 size)
{
	u8 f = DOS_OpenHandle(name, O_RDONLY);
	if (g_DOS_LastError)
		return 0;
	u16 n = DOS_ReadHandle(f, dst, size);
	DOS_CloseHandle(f);
	return n;
}

static bool ReadFile(const char* name, void* dst, u16 size)
{
	return ReadFileLen(name, dst, size) != 0;
}

bool TextPool_Load()
{
	u8 idx[1 + 2 * TXT_MAXPOOLS + 12];
	u16 tokLen;
	if (!ReadFile("TXT.IDX", idx, sizeof(idx)))
		return false;
	tokLen = ReadFileLen("TXT.TOK", g_Scratch, sizeof(g_Scratch));
	if (tokLen <= 512 || tokLen - 512 > TOK_BUF_SIZE)
		return false;
	Mem_Copy(g_Scratch + 512, g_TokBuf, tokLen - 512);
	for (u16 c = 0; c < 256; c++)
	{
		u16 p = ((const u16*)g_Scratch)[c];
		if (p)
			p = p - 512 + (u16)g_TokBuf;
		g_TokHi[c] = p >> 8;
		g_TokLo[c] = p & 0xFF;
	}
	g_PoolCount = idx[0];
	if (g_PoolCount > TXT_MAXPOOLS)
		return false;

	char name[] = "TXT0.BIN";
	u8 save = g_Page2;
	bool ok = true;
	for (u8 i = 0; i < g_PoolCount && ok; i++)
	{
		g_PoolFirst[i] = idx[1 + 2 * i] | (idx[2 + 2 * i] << 8);
		DOS_Segment seg;
		if (!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &seg))
		{
			ok = false;
			break;
		}
		g_PoolSeg[i] = seg.Number;
		name[3] = '0' + i;
		Page2Set(seg.Number);
		ok = ReadFile(name, (void*)POOL_ADDR, 16384);
	}
	Page2Set(save);
	return ok;
}

u16 TextPool_Read(u16 id, char* dst)
{
	u8 p = g_PoolCount;
	while (p && id < g_PoolFirst[--p]) {}

	u8 save = g_Page2;
	Page2Set(g_PoolSeg[p]);
	u16 i = id - g_PoolFirst[p];
	u16 n = 0;
	if (i < *(const u16*)POOL_ADDR)
	{
		u16 off = ((const u16*)POOL_ADDR)[1 + i];
		const u8* r = (const u8*)(POOL_ADDR + off);
		n = *(const u16*)r;
		r += 2;
		Decode(r, dst);
	}
	else
		dst[0] = 0;
	Page2Set(save);
	return n;
}
