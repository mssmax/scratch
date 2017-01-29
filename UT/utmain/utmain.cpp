#include "stdafx.h"
#include <PropBag.h>


int TestMe(LPCSTR name)
{
	CPropBag pb;

	pb.Set(1, L"first");
	pb.Set(2, L"last");

	std::wstring s = pb.Get(1);
	if (s != L"first")
		return 0;
	else
	{
		s = pb.Get(2);
		if (s != L"last")
			return 0;
		else
			return 1;
	}
}