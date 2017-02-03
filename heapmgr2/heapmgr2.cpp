#include "stdafx.h"
#include "HeapManager.h"

int main()
{
	CHeapManager mgr(3, 8192);

	{
		CHeapEntry p = mgr.Alloc();
		memcpy(p, "some string", 12);
		printf("Copied some stuff\n");
	}

	CHeapEntry p2 = mgr.Alloc();

	return 0;
}

