// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char* argv[])
{
	SECURITY_ATTRIBUTES sa = {0};
	HANDLE hPipe = CreateNamedPipe("\\\\.\\pipe\\mynamedpipe", PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 1, 0, 0, 1000, &sa);
	DWORD dwWri;

//	for(int i = 0; i < 2; i++)
	{

	BOOL b = ConnectNamedPipe(hPipe, NULL);

	char buf[] = "myfunnydata";
//	b = WriteFile(hPipe, buf, 12, &dwWri, 0);
//	b = WriteFile(hPipe, buf, 5, &dwWri, 0);
//	b = WriteFile(hPipe, buf, 5, &dwWri, 0);

	DisconnectNamedPipe(hPipe);
	}

	CloseHandle(hPipe);
	return 0;
}
