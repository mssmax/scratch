#pragma once

// the following code assumes you have included windows.h, tchar.h and strsafe.h
inline void OutputDebugStringEx(LPCTSTR fmt, ...)
{
	TCHAR szBuf[512] = { 0 };
	va_list va;
	va_start(va, fmt);
	StringCbVPrintf(szBuf, sizeof(szBuf), fmt, va);
	va_end(va);
	OutputDebugStr(szBuf);
}