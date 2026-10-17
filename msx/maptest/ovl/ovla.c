#include "api.h"
// entry point MUST be the first function: the overlay is linked at 8000h
unsigned short OvlA_Main(const Api* api)
{
	char buf[1400];
	api->Print("Overlay A running from page 2 (8000h).");
	api->ReadText(364, buf);        // a line record: swaps page 2 away from this very code and back
	api->Print(buf);
	api->ReadText(1090, buf);       // a text block record (B)
	buf[40] = 0;
	api->Print(buf);
	return 0xA0A0;
}
