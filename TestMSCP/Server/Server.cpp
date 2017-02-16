// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "..\Include\fme.h"

int main()
{
	HANDLE hPort = NULL;
	HANDLE hSlot = INVALID_HANDLE_VALUE;

	printf("gflkdgjdlgf\n");

	return 0;

	hSlot = CreateMailslot(_T("\\\\.\\mailslot\\mscp"), 0, MAILSLOT_WAIT_FOREVER, 0);
	if (hSlot == INVALID_HANDLE_VALUE)
	{
		printf("Failed to open mail slot, err %d\n", GetLastError());
		return 0;
	}

	hPort = CreateIoCompletionPort(hSlot, hPort, (ULONG_PTR)hSlot, 1);
	if (hPort == NULL)
	{
		printf("Failed to open IO completion port, err %d\n", GetLastError());
	}


	for (;;)
	{
		ULONG_PTR ulCompletionKey = (ULONG_PTR)hSlot;
		BYTE buffer[8192]= { 0 };
		DWORD dwRead = 0;
		OVERLAPPED ov = { 0 };
		BOOL b = ReadFile((HANDLE)ulCompletionKey, buffer, 8192, &dwRead, &ov);

		DWORD dwBytesTransferred = 0;
		LPOVERLAPPED lpov = NULL;
		b = GetQueuedCompletionStatus(hPort, &dwBytesTransferred, &ulCompletionKey, &lpov, INFINITE);
		if (b)
		{
			lpov+=2;
			FMEVENT *evt = (FMEVENT*)lpov;
			printf("Got packet\n");
		}
	}

	CloseHandle(hPort);
	CloseHandle(hSlot);

    return 0;
}

