#include "stdafx.h"

extern int TestMe(LPCSTR value);

int main()
{
	int res = TestMe("something");
	return 0;
}

