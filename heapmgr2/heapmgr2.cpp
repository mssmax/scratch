#include "stdafx.h"
#include "HeapManager.h"

int main()
{
	CHeapManager mgr(8192, 3);

	{
		CHeapAlloc p = mgr.Alloc();
		memcpy(p, "some string", 12);
		printf("Copied some stuff\n");
	}

	CHeapAlloc p2 = mgr.Alloc();

	return 0;
}

