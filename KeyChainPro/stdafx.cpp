#include "stdafx.h"
#pragma comment(lib, "dbeng.lib")
CDBEngine g_DB;

std::wstring ConvA2W(LPCSTR lpsz)
{
	std::wstring ret;
	int size = MultiByteToWideChar(CP_UTF8, 0, lpsz, -1, 0, 0);
	if (size)
	{
		ret.resize(size);
		MultiByteToWideChar(CP_UTF8, 0, lpsz, -1, &ret[0], size);
		ret.resize(size - 1);
	}
	return ret;
}

std::string ConvW2A(LPCWSTR lpsz)
{
	std::string ret;
	int size = WideCharToMultiByte(CP_UTF8, 0, lpsz, -1, 0, 0, 0, 0);
	if (size)
	{
		ret.resize(size);
		WideCharToMultiByte(CP_UTF8, 0, lpsz, -1, &ret[0], size, 0, 0);
		ret.resize(size - 1);
	}
	return ret;
}

HCRYPTKEY g_hKey = 0;

HRESULT EncryptPassword(LPCTSTR lpszPassword, IStream *pStrm)
{
	HRESULT hr = S_OK;
	BYTE byData[1024] = { 0 };
	DWORD dwDataLen = (lstrlen(lpszPassword) + 1) * sizeof(TCHAR);
	CopyMemory(byData, lpszPassword, dwDataLen);

	BOOL b = CryptEncrypt(
		g_hKey,
		0,
		TRUE,
		0,
		(BYTE*)byData,
		&dwDataLen,
		sizeof(byData)
	);

	if (!b)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	ULONG cbWri = 0;
	hr = pStrm->Write(byData, dwDataLen, &cbWri);

	LARGE_INTEGER liZero = { 0 };
	ULARGE_INTEGER liNewPos = { 0 };
	return pStrm->Seek(liZero, STREAM_SEEK_SET, &liNewPos);
}

HRESULT DecryptPassword(IStream *pStrm, LPTSTR lpszPassword)
{
	HRESULT hr = S_OK;
	BYTE byData[1024] = { 0 };
	ULONG ulRead = 0;
	hr = pStrm->Read(byData, sizeof(byData), &ulRead);

	BOOL b = CryptDecrypt(
		g_hKey,
		0,
		TRUE,
		0,
		byData,
		&ulRead
		);

	if (!b)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	else
	{
		memcpy(lpszPassword, byData, ulRead);
	}

	return hr;
}
