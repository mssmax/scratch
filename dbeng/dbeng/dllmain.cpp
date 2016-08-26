#include "stdafx.h"

DWORD g_dwSessionTLSID = 0;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_dwSessionTLSID = TlsAlloc();
		if (g_dwSessionTLSID == TLS_OUT_OF_INDEXES)
		{
			return FALSE;
		}
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
	{
		JET_SESID sessionId = reinterpret_cast<JET_SESID>(TlsGetValue(g_dwSessionTLSID));
		if (sessionId != 0)
		{
			JetEndSession(sessionId, 0);
		}
	}
	break;
	case DLL_PROCESS_DETACH:
		TlsFree(g_dwSessionTLSID);
		break;
	}
	return TRUE;
}

