#include "stdafx.h"
#pragma comment(lib, "dbeng.lib")
CDBEngine g_DB;

std::wstring ConvA2W(LPCSTR lpsz)
{
	std::wstring ret;
	int size = MultiByteToWideChar(CP_ACP, 0, lpsz, -1, 0, 0);
	if (size)
	{
		ret.resize(size);
		MultiByteToWideChar(CP_ACP, 0, lpsz, -1, &ret[0], size);
		ret.resize(size - 1);
	}
	return ret;
}

std::string ConvW2A(LPCWSTR lpsz)
{
	std::string ret;
	int size = WideCharToMultiByte(CP_ACP, 0, lpsz, -1, 0, 0, 0, 0);
	if (size)
	{
		ret.resize(size);
		WideCharToMultiByte(CP_ACP, 0, lpsz, -1, &ret[0], size, 0, 0);
		ret.resize(size - 1);
	}
	return ret;
}

HRESULT EncryptPassword(LPCTSTR lpszPassword, IStream *pStrm)
{
	HRESULT hr = S_OK;
	DATA_BLOB blbIn = { 0 };
	DATA_BLOB blbOut = { 0 };

	blbIn.cbData = lstrlen(lpszPassword) * sizeof(TCHAR);
	blbIn.pbData = (BYTE*)lpszPassword;

	BOOL b = CryptProtectData(
		&blbIn,
		0,
		0,
		0,
		0,
		0,
		&blbOut
		);

	if (!b)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	ULONG cbWri = 0;
	hr = pStrm->Write(blbOut.pbData, blbOut.cbData, &cbWri);
	if (FAILED(hr))
	{
		LocalFree(blbOut.pbData);
		return hr;
	}

	LocalFree(blbOut.pbData);

	LARGE_INTEGER liZero = { 0 };
	ULARGE_INTEGER liNewPos = { 0 };
	return pStrm->Seek(liZero, STREAM_SEEK_SET, &liNewPos);
}

HRESULT DecryptPassword(IStream *pStrm, LPTSTR lpszPassword)
{
	HGLOBAL hGlb = 0;
	HRESULT hr = GetHGlobalFromStream(pStrm, &hGlb);
	if (FAILED(hr))
	{
		return hr;
	}
	DATA_BLOB blbIn = { 0 };
	DATA_BLOB blbOut = { 0 };
	STATSTG stat = { 0 };
	pStrm->Stat(&stat, STATFLAG_NONAME);
	blbIn.cbData = stat.cbSize.LowPart;
	blbIn.pbData = reinterpret_cast<BYTE*>(GlobalLock(hGlb));

	BOOL b = CryptUnprotectData(
		&blbIn,
		0,
		0,
		0,
		0,
		0,
		&blbOut
		);

	if (!b)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		GlobalUnlock(hGlb);
	}
	else
	{
		memcpy(lpszPassword, blbOut.pbData, blbOut.cbData);
		LocalFree(blbOut.pbData);
	}

	return hr;
}
