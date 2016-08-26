// heapmgr.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "..\heapmgr.h"


void * operator new(size_t size)
{
	return CHeap::Alloc(size);
}

void operator delete(void *p)
{
	CHeap::Free(p);
}

void * operator new[](size_t size)
{
	return CHeap::Alloc(size);
}

void operator delete[](void *p)
{
	CHeap::Free(p);
}
