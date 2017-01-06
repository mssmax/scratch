// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#pragma pack(push, 1)
typedef struct
{
	WCHAR szJobID[39]; // 38 is the size of a GUID + zero-terminator
	WCHAR szUserName[61];
} JOBINFO;

#pragma pack(pop)

int main(int argc, char* argv[])
{

	size_t t = sizeof(JOBINFO);

	HANDLE hPipe = CreateFile("\\\\.\\pipe\\mynamedpipe", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, 0);

	BYTE buf[10]; 
	DWORD dwMode = PIPE_READMODE_MESSAGE;
	BOOL b = SetNamedPipeHandleState(hPipe, &dwMode, NULL, NULL);
	DWORD dwRead;

	b = ReadFile(hPipe, buf, 10, &dwRead, 0);

	CloseHandle(hPipe);

	return 0;
}
