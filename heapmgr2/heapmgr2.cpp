#include "stdafx.h"
#include "HeapManager.h"

#define _SECOND 10000000

int main()
{
/**/
	CHeapManager mgr(3, 8192);

	{
		CHeapEntry p = mgr.Alloc();
		memcpy(p, "some string", 12);
		printf("Copied some stuff\n");
	}

	CHeapEntry p2 = mgr.Alloc();

	USHORT sz = mgr.Size();
	mgr.Recycle();
/**/
	return 0;
}