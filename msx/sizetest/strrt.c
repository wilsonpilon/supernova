// Runtime helpers (this is the resident "Pascal runtime" cost)
#include "nova.h"
static const u8 g_Bit[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

u8 SetIn(const u8* s, u8 n)  { return (s[n >> 3] & g_Bit[n & 7]) != 0; }
void SetAdd(u8* s, u8 n)     { s[n >> 3] |= g_Bit[n & 7]; }
void SetDel(u8* s, u8 n)     { s[n >> 3] &= ~g_Bit[n & 7]; }
u8 SetEmpty(const u8* s, u8 bytes) { while (bytes--) if (*s++) return 0; return 1; }

static u8 Len(const char* s) { u8 n = 0; while (s[n]) n++; return n; }

u8 Pos(const char* sub, const char* s)
{
	u8 n = Len(sub);
	if (!n) return 0;
	for (u8 k = 0; s[k]; k++)
	{
		u8 j = 0;
		while (j < n && s[k + j] == sub[j]) j++;
		if (j == n) return k + 1;
	}
	return 0;
}

void Delete(char* s, u8 idx, u8 cnt)
{
	u8 l = Len(s);
	if (idx == 0 || idx > l) return;
	idx--;
	if (idx + cnt > l) cnt = l - idx;
	for (u8 k = idx; k + cnt <= l; k++) s[k] = s[k + cnt];
}

void Insert(const char* sub, char* s, u8 idx)
{
	u8 l = Len(s), n = Len(sub);
	if (idx == 0) idx = 1;
	if (idx > l + 1) idx = l + 1;
	idx--;
	for (u8 k = l + 1; k > idx; k--) s[k + n - 1] = s[k - 1];
	for (u8 k = 0; k < n; k++) s[idx + k] = sub[k];
}

void Copy(char* dst, const char* s, u8 idx, u8 cnt)
{
	u8 l = Len(s), k = 0;
	if (idx == 0) idx = 1;
	idx--;
	while (k < cnt && idx + k < l) { dst[k] = s[idx + k]; k++; }
	dst[k] = 0;
}

u8 StrEq(const char* a, const char* b) { while (*a == *b) { if (!*a) return 1; a++; b++; } return 0; }
void StrCpy(char* d, const char* s) { while ((*d++ = *s++)); }
void StrCat(char* d, const char* s) { while (*d) d++; while ((*d++ = *s++)); }
