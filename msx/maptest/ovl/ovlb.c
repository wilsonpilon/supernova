#include "api.h"
static unsigned short Fib(unsigned char n);
unsigned short OvlB_Main(const Api* api)
{
	api->Print("Overlay B running from page 2 (8000h). Fib(12) =");
	api->PrintNum(Fib(12));
	return 0xB0B0;
}
static unsigned short Fib(unsigned char n) { return n < 2 ? n : Fib(n - 1) + Fib(n - 2); }
