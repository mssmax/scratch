// testjoin.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	NETSETUP_JOIN_STATUS status = NetSetupUnknownStatus;
	LPWSTR pName = NULL;
	LPCWSTR szCompName = NULL;
	if(argc > 1)
		szCompName = argv[1];
	DWORD dw = NetGetJoinInformation(szCompName, &pName, &status);
	_tprintf(_T("Domain: [%s], join info[%d]\n"), pName, status);

	LPBYTE pBuf = NULL;
	NetGetDCName(NULL, NULL, &pBuf);

	_tprintf(_T("DC name[%s]\n"), pBuf);

	return 0;
}

