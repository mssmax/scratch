// fmEtherfax.tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mfg.h"
#include "windows.h"
#include <conio.h>

using testing::_;

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleMock(&argc,argv);
    int rtn = RUN_ALL_TESTS();
	getch();
	return rtn;

}

