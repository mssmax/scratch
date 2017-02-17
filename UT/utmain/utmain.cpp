#include "stdafx.h"
#include <PropBag.h>


int TestMe(LPCSTR name)
{
	CPropBag pb;

	pb.Set(1, L"first");
	pb.Set(2, L"last");
	pb.Set(3, L"third");

	std::wstring s = pb.Get(1);
	if (s != L"first")
		return 0;

	s = pb.Get(2);
	if (s != L"last")
		return 0;

	s = pb.Get(3);
	if (s != L"third")
		return 0;

	return 1;
}