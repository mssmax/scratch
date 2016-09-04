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
