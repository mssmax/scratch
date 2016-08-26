// ioshim.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ioshim.h"
#include "detours.h"
#include <strsafe.h>

BOOL (WINAPI *OrigCloseHandle)(HANDLE hFile) = CloseHandle;
BOOL WINAPI MyCloseHandle(HANDLE hFile);

BOOL (WINAPI *OrigWriteFile)(
	HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    ) = WriteFile;

HANDLE (WINAPI *OrigCreateFileW)(
   LPCWSTR lpFileName,
   DWORD dwDesiredAccess,
   DWORD dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   DWORD dwCreationDisposition,
   DWORD dwFlagsAndAttributes,
   HANDLE hTemplateFile) = CreateFileW;

BOOL WINAPI MyWriteFile(
	HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    );

HANDLE WINAPI MyCreateFileW(
   LPCWSTR lpFileName,
   DWORD dwDesiredAccess,
   DWORD dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   DWORD dwCreationDisposition,
   DWORD dwFlagsAndAttributes,
   HANDLE hTemplateFile);


#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	TCHAR szText[512] = {0};
	LONG l;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		l = DetourAttach((PVOID*)&OrigCloseHandle, MyCloseHandle);
		StringCbPrintf(szText, sizeof(szText), _T("Attached to closehandle [0x%X]"), l);
		OutputDebugString(szText);

		l = DetourAttach((PVOID*)&OrigCreateFileW, MyCreateFileW);
		StringCbPrintf(szText, sizeof(szText), _T("Attached to createfile[0x%X]"), l);
		OutputDebugString(szText);
		l = DetourAttach((PVOID*)&OrigWriteFile, MyWriteFile);
		StringCbPrintf(szText, sizeof(szText), _T("Attached to writefile[0x%X]"), l);
		OutputDebugString(szText);
		l = DetourTransactionCommit();
		StringCbPrintf(szText, sizeof(szText), _T("commited[0x%X]"), l);
		OutputDebugString(szText);

		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach((PVOID*)&OrigCloseHandle, MyCloseHandle);
		DetourDetach((PVOID*)&OrigCreateFileW, MyCreateFileW);
		DetourDetach((PVOID*)&OrigWriteFile, MyWriteFile);
		DetourTransactionCommit();
		break;
	}
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

#define MAX_BUFFER 102400
HANDLE g_hHandle = INVALID_HANDLE_VALUE;
BYTE g_byBuffer[MAX_BUFFER] = {0};
DWORD g_dwNumBytes = 0;

void DummyEntry()
{
	OutputDebugString(_T("The Dummy function was called"));
}

BOOL Flush()
{
	if(g_dwNumBytes > 0)
	{
		DWORD dwWri;
		if(!OrigWriteFile(g_hHandle, g_byBuffer, g_dwNumBytes, &dwWri, 0))
		{
			return FALSE;
		}
	}
	g_dwNumBytes = 0;

	return TRUE;
}

BOOL WINAPI MyCloseHandle(HANDLE hFile)
{
	if(hFile == g_hHandle)
	{
		Flush();
		g_hHandle = INVALID_HANDLE_VALUE;
	}

	return OrigCloseHandle(hFile);
}


BOOL WINAPI MyWriteFile(
	HANDLE hFile,
    LPCVOID lpBuffer,
    DWORD nNumberOfBytesToWrite,
    LPDWORD lpNumberOfBytesWritten,
    LPOVERLAPPED lpOverlapped
    )
{
	if(hFile == g_hHandle)
	{
		// check if the buffer is full
		if(g_dwNumBytes >= MAX_BUFFER)
		{
			if(!Flush())
				return FALSE;
		}

		DWORD dwCurPos = 0;
		while(nNumberOfBytesToWrite > 0)
		{
			DWORD dwCopy = min(nNumberOfBytesToWrite, MAX_BUFFER - g_dwNumBytes);
			memcpy(g_byBuffer + g_dwNumBytes, (PBYTE)lpBuffer + dwCurPos, dwCopy);
			dwCurPos += dwCopy;
			g_dwNumBytes += dwCopy;
			nNumberOfBytesToWrite -= dwCopy;
			if(g_dwNumBytes >= MAX_BUFFER)
			{
				if(!Flush())
					return FALSE;
			}
		}

		return TRUE;
	}
	else
		return OrigWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

HANDLE WINAPI MyCreateFileW(
   LPCWSTR lpFileName,
   DWORD dwDesiredAccess,
   DWORD dwShareMode,
   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
   DWORD dwCreationDisposition,
   DWORD dwFlagsAndAttributes,
   HANDLE hTemplateFile)
{
	HANDLE h = OrigCreateFileW(lpFileName, 
		dwDesiredAccess, 
		dwShareMode, 
		lpSecurityAttributes, 
		dwCreationDisposition, 
		dwFlagsAndAttributes, 
		hTemplateFile);

	if(h != INVALID_HANDLE_VALUE)
	{
		LPCTSTR psz = _tcsrchr(lpFileName, '\\');
		psz++;
		if(!lstrcmpi(psz, _T("database1.sos")) && (dwDesiredAccess & GENERIC_WRITE))
		{
			g_hHandle = h;
		}
	}

	return h;
}
