// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Include\fme.h"


int main()
{
	printf("something\n");
	CMailSlotClient<FMEVENT> mc(_T("\\\\.\\mailslot\\mscp"));
	FMEVENT evt;
	StringCbCopy(evt.EventSubModule, sizeof(evt.EventSubModule), _T("hahahahahahahaha"));
	mc.SendMessage(&evt);

    return 0;
}

