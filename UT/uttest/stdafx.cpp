#include "stdafx.h"
#include <conio.h>

int main(int argc, char **argv)
{
	testing::InitGoogleMock(&argc, argv);
	int i = RUN_ALL_TESTS();
	_getch();
	return i;
}