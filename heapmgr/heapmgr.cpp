// heapmgr.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "heapmgr.h"

DWORD g_dwTlsIndex = 0;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:

			g_dwTlsIndex = TlsAlloc();
			if(g_dwTlsIndex == TLS_OUT_OF_INDEXES)
				return FALSE;

			TlsSetValue(g_dwTlsIndex, 0);
			return CHeap::CreateNewHeap();

			break;
		case DLL_THREAD_ATTACH:

			TlsSetValue(g_dwTlsIndex, 0);
			return CHeap::CreateNewHeap();

			break;
		case DLL_THREAD_DETACH:

			CHeap::DestroyHeap();

			break;
		case DLL_PROCESS_DETACH:
			
			CHeap::DestroyHeap();
			TlsFree(g_dwTlsIndex);
			break;
    }
    return TRUE;
}


BOOL CHeap::CreateNewHeap()
{
	CHeap *pHeap = (CHeap*)CHeap::StableAlloc(sizeof(CHeap));
	if(!pHeap)
	{
		SetLastError(ERROR_NOT_ENOUGH_MEMORY);
		return FALSE;
	}
	
	ZeroMemory(pHeap, sizeof(CHeap));

	pHeap->m_hHeap = HeapCreate(HEAP_NO_SERIALIZE, DEFAULT_HEAP_SIZE, 0);
	if(!pHeap->m_hHeap)
		return FALSE;

	pHeap->m_cs.Init();

	if(!TlsSetValue(g_dwTlsIndex, pHeap))
		return FALSE;
	
	return TRUE;
}

void CHeap::DestroyHeap()
{
	CHeap *pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);
	// it is possible that a thread is being detached for which
	// a heap was never created
	if(pHeap)
	{
		FreeMemorySegmentList(pHeap->m_SegListHead);
		pHeap->m_SegListHead = 0;
		// incrementing a variable like this will flip the lowest bit, aka deferred bit
		LONG l = InterlockedIncrement(&pHeap->m_lAllocCount);
		// now we check what is the resulting value
		// if it is 1 - it means our lowest bit is set only ( due to previous InterlockedIncrement )
		// and remaining bits are zero meaning there are no outstanding allocations and 
		// therefore the heap can be safely destroyed
		if(l == 1)
		{
			if(pHeap->m_hHeap)
			{
				HeapDestroy(pHeap->m_hHeap);
				pHeap->m_hHeap = NULL;
			}
		
			CHeap::StableFree(pHeap);
			TlsSetValue(g_dwTlsIndex, 0);
		}
	}
}

void *CHeap::StableAlloc(size_t size)
{
	return HeapAlloc(GetProcessHeap(), 0, size);
}

void CHeap::StableFree(void *p)
{
	HeapFree(GetProcessHeap(), 0, p);
}

void *CHeap::Alloc(size_t size)
{
	CHeap *pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);

	// a heap may not yet have been created for this thread
	// this can happen if the dll was loaded AFTER the thread was created
	if(!pHeap)
	{
		if(!CreateNewHeap())
			return NULL;
		pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);
	}

	// we increment the counter by two so as to bypass the lowest bit
	InterlockedExchangeAdd(&pHeap->m_lAllocCount, 2);

	pHeap->m_cs.Enter();

	PHEAP *p = (PHEAP*)HeapAlloc(pHeap->m_hHeap, HEAP_NO_SERIALIZE, size + sizeof(PHEAP));

	pHeap->m_cs.Leave();

	if(!p)
	{
		InterlockedExchangeAdd(&pHeap->m_lAllocCount, -2);
		return NULL;
	}

	p->pHeap = pHeap;

	return p + 1;
}

void CHeap::Free(void *mem)
{
	if(!mem)
		return;

	// when freeing we always use the heap that originally had the the memory allocated
	PHEAP *p = (PHEAP*)mem;

	p--;
	CHeap *pHeap = p->pHeap;

	pHeap->m_cs.Enter();

	HeapFree(pHeap->m_hHeap, HEAP_NO_SERIALIZE, p);

	pHeap->m_cs.Leave();

	// if the heap was marked for delayed deletion
	// we should delete it
	LONG lRef = InterlockedExchangeAdd(&pHeap->m_lAllocCount, -2);
	// InterlockedExchangeAdd returns the original value of the variable
	// if it's value is 3 it means that lowest bit is set ( by DLL_THREAD_DETACH notification ) 
	// and our allocation counter WAS 1, meaning it was the last allocation to be freed, and so the heap can be destroyed.
	if(lRef == 3)
	{
		HeapDestroy(pHeap->m_hHeap);
		CHeap::StableFree(pHeap);
		// there is intentionally no TlsSetValue to zero here
		// because otherwise we would destroy our own heap, besides, current thread does not have
		// access to other threads' TLS anyway and also, since we are here it means original thread has been destroyed
		// so not zeroing the tls value is not harmful
	}
}

void *CHeap::FastAlloc(size_t size)
{
	CHeap *pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);

	// a heap may not yet have been created for this thread
	// this can happen if the dll was loaded AFTER the thread was created
	if(!pHeap)
	{
		if(!CreateNewHeap())
			return NULL;
		pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);
	}

	MEMORY_SEGMENT *pSeg = pHeap->m_SegListHead;
	if(!pSeg || pSeg->dwCurrentPointer + size > pSeg->dwTotalBytes)
	{
		DWORD dwSizeNeeded = sizeof(MEMORY_SEGMENT) + max(DEFAULT_HEAP_SIZE, size);
		pSeg = (MEMORY_SEGMENT*)CHeap::Alloc(dwSizeNeeded);
		if(!pSeg)
			return NULL;
		pSeg->dwTotalBytes = dwSizeNeeded - sizeof(MEMORY_SEGMENT);
		pSeg->dwCurrentPointer = 0;
		pSeg->pNext = 0;
		if(pHeap->m_SegListHead)
			pSeg->pNext = pHeap->m_SegListHead;
		pHeap->m_SegListHead = pSeg;
	}

	void *p = pSeg->Segment + pSeg->dwCurrentPointer;
	pSeg->dwCurrentPointer += size;
	pHeap->m_lFastAllocCount++;

	return p;
}

void CHeap::FastFree(void *p)
{
	CHeap *pHeap = (CHeap*)TlsGetValue(g_dwTlsIndex);
	
	// remove all segments but first one
	if(--pHeap->m_lFastAllocCount == 0)
	{
		FreeMemorySegmentList(pHeap->m_SegListHead->pNext);
		pHeap->m_SegListHead->pNext = 0;
		pHeap->m_SegListHead->dwCurrentPointer = 0;
	}

}

void CHeap::FreeMemorySegmentList(MEMORY_SEGMENT *pHead)
{
	while(pHead)
	{
		MEMORY_SEGMENT *pNext = pHead->pNext;
		CHeap::Free(pHead);
		pHead = pNext;
	}
}


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
