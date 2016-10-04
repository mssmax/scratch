#include "stdafx.h"

std::wstring ConvA2W(LPCSTR lpsz, UINT cp)
{
	std::wstring ret;
	int size = MultiByteToWideChar(cp, 0, lpsz, -1, 0, 0);
	if (size)
	{
		ret.resize(size);
		MultiByteToWideChar(cp, 0, lpsz, -1, &ret[0], size);
		ret.resize(size - 1);
	}
	return ret;
}

std::string ConvW2A(LPCWSTR lpsz, UINT cp)
{
	std::string ret;
	int size = WideCharToMultiByte(cp, 0, lpsz, -1, 0, 0, 0, 0);
	if (size)
	{
		ret.resize(size);
		WideCharToMultiByte(cp, 0, lpsz, -1, &ret[0], size, 0, 0);
		ret.resize(size - 1);
	}
	return ret;
}

void DeleteAllFiles(LPCSTR lpszPath)
{
	WIN32_FIND_DATAA find = { 0 };
	std::string sPath(lpszPath);
	sPath.append("*.*");
	HANDLE hFind = FindFirstFileA(sPath.c_str(), &find);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				sPath = lpszPath;
				sPath.append(find.cFileName);
				DeleteFileA(sPath.c_str());
			}
		} while (FindNextFileA(hFind, &find));
		FindClose(hFind);
	}
}