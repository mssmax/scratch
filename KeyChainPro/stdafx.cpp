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
