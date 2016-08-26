// fmbilling.tests.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "mfg.h"
#include "windows.h"
#include <conio.h>

using namespace testing;

int _tmain(int argc, char* argv[])
{
	testing::InitGoogleMock(&argc, argv);
    int rtn = RUN_ALL_TESTS();
	getch();
	return rtn;
}


