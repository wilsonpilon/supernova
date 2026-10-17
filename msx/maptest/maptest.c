// Test: MSX-DOS 2 + RAM mapper. Load code overlays from disk into mapper segments,
// swap them into page 2 (8000h-BFFFh) and call them through an API table.
// Also tests the real game text pools (tools/mkdata.js output) through the resident reader.
#include "msxgl.h"
#include "dos.h"
#include "dos_mapper.h"
#include "ovl/api.h"
#include "../textpool.c"
#include "../data/textdef.h"
#include "../data/vocab_data.c"

#define OVL_ADDR 0x8000

static void Print(const char* s) { while (*s) DOS_CharOutput(*s++); DOS_CharOutput('\r'); DOS_CharOutput('\n'); }

static void PrintNum(u16 v)
{
	char t[6]; u8 n = 0;
	do { t[n++] = '0' + v % 10; v /= 10; } while (v);
	while (n) DOS_CharOutput(t[--n]);
	DOS_CharOutput('\r'); DOS_CharOutput('\n');
}

static void PrintHex(u16 v)
{
	for (i8 s = 12; s >= 0; s -= 4) { u8 d = (v >> s) & 15; DOS_CharOutput(d < 10 ? '0' + d : 'A' + d - 10); }
	DOS_CharOutput('\r'); DOS_CharOutput('\n');
}

static const Api g_Api = { Print, PrintNum, TextPool_Read };

static u8 LoadOverlay(const char* name, u8 seg)
{
	DOSMapper_SetPage2(seg);
	u8 f = DOS_OpenHandle(name, O_RDONLY);
	if (g_DOS_LastError) return 0;
	u16 n = DOS_ReadHandle(f, (void*)OVL_ADDR, 16384);
	DOS_CloseHandle(f);
	return n != 0;
}

// print the first `max` characters of a record, then its length
static void Show(const char* what, u16 id)
{
	static char tbuf[1400];
	u16 n = TextPool_Read(id, tbuf);
	Print(what);
	if (n > 60) tbuf[60] = 0;
	Print(tbuf);
	PrintNum(n);
}

void main()
{
	DOS_Segment segA, segB;
	Print("MAPPER OVERLAY + TEXT POOL TEST");
	if (!DOSMapper_Init()) { Print("No mapper extended BIOS!"); DOS_Exit(1); }
	Print("Mapper segments total/free:");
	PrintNum(g_DOS_VarTable->NumSeg);
	PrintNum(g_DOS_VarTable->FreeSeg);

	if (!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &segA)) { Print("alloc A failed"); DOS_Exit(1); }
	if (!DOSMapper_Alloc(DOS_ALLOC_USER, DOS_SEGSLOT_PRIM, &segB)) { Print("alloc B failed"); DOS_Exit(1); }
	u8 okA = LoadOverlay("OVLA.BIN", segA.Number);
	u8 okB = LoadOverlay("OVLB.BIN", segB.Number);
	Print(okA && okB ? "Overlays loaded" : "OVERLAY LOAD FAILED");

	u16 t0 = g_JIFFY;
	bool okT = TextPool_Load();
	Print(okT ? "Text pools loaded, frames:" : "TEXT LOAD FAILED");
	PrintNum(g_JIFFY - t0);
	Print("Free segments now:");
	PrintNum(g_DOS_VarTable->FreeSeg);

	// resident reads with page 2 holding overlay A
	DOSMapper_SetPage2(segA.Number);
	Show("Room 1 (R1+R2):", TK_R + 1);
	Show("Line 5:", TK_L + 5);
	Show("Special 3:", TK_S + 3);
	Show("Clue 5:", TK_C + 5);
	Show("B block 1:", TK_B + 1);
	Show("Last record:", TK_TOTAL - 1);
	Show("Past the end (expect empty, 0):", TK_TOTAL);
	Print("page 2 still overlay A? (1=yes):"); PrintNum(DOSMapper_GetPage2() == segA.Number);

	// read every record: byte total must be 116572 (mod 65536 = 51036)
	static char tbuf[1400];
	u16 sum = 0, nonEmpty = 0, longest = 0;
	t0 = g_JIFFY;
	for (u16 id = 0; id < TK_TOTAL; id++)
	{
		u16 n = TextPool_Read(id, tbuf);
		sum += n;
		if (n) nonEmpty++;
		if (n > longest) longest = n;
	}
	Print("All records read, frames:"); PrintNum(g_JIFFY - t0);
	Print("byte sum mod 65536 (expect 51036):"); PrintNum(sum);
	Print("non-empty / longest:"); PrintNum(nonEmpty); PrintNum(longest);

	// vocabulary tables
	Vocab_Init();
	Print(g_V[1]); Print(g_V[95]); Print(g_N[133]); Print(g_P[11]); Print(g_A[4]); Print(g_RN[24]); Print(g_RN[109]);
	Print(LOC_HAS(62, 56) && LOC_HAS(63, 20) && !LOC_HAS(61, 20) && !LOC_HAS(17, 56) && LOC_HAS(17, 131) ? "sets OK" : "sets BAD");
	g_N[126] = "hinged mouth\mouth\hinge\\"; LOC_ADD(17, 5); LOC_DEL(17, 131);
	Print(LOC_HAS(17, 5) && !LOC_HAS(17, 131) ? "set ops OK" : "set ops BAD");

	u16 (*entry)(const Api*) = (u16 (*)(const Api*))OVL_ADDR;
	u16 r;
	DOSMapper_SetPage2(segA.Number); r = entry(&g_Api); Print("A returned (hex):"); PrintHex(r);
	DOSMapper_SetPage2(segB.Number); r = entry(&g_Api); Print("B returned (hex):"); PrintHex(r);
	Print("DONE");
	DOS_Exit(0);
}
