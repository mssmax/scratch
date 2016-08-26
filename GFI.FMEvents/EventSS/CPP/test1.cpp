// test1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "fme.h"
#include "strsafe.h"


int main()
{
	CMailSlotClient<FMEVENT> sbtr(MONITOR_SLOTNAME);

	FMEVENT evt;
	evt.EventID = 0xdeadbeaf;
	evt.EventModule = FMTSERVER;
	StringCbCopy(evt.EventSubModule, sizeof(evt.EventSubModule), _T("muahahaa"));
	StringCbCopy(evt.EventAdditionalInfo, sizeof(evt.EventAdditionalInfo), _T("additional info for chef"));

	sbtr.SendMessageW(&evt);

	return 0;
}

