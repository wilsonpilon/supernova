// Resident services table handed to overlays (overlays never link against the resident code)
#pragma once
typedef struct {
	void (*Print)(const char*);
	void (*PrintNum)(unsigned short);
	unsigned short (*ReadText)(unsigned short rec, char* dst);
} Api;
