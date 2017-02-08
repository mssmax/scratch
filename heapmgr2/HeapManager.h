#pragma once

struct HeapEntry : public SLIST_ENTRY
{
	BYTE m[1];
};

class CHeapManager;
class CHeapEntry
{
private:
	CHeapManager *m_pMgr;
	HeapEntry *m_pData;
public:
	CHeapEntry(HeapEntry *pData, CHeapManager *pMgr);
	~CHeapEntry();

	operator void*();
};

class CHeapManager
{
private:
	ULONG m_ulAllocDelta;
	ULONG m_ulSize;
	ULONG m_ulInitialSize;
	DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) SLIST_HEADER m_slAllocs;

	void PrepAllocs(ULONG ulNumAllocs)
	{		
		for (; ulNumAllocs > 0; ulNumAllocs--)
		{
			HeapEntry *pAlloc = static_cast<HeapEntry*>(_aligned_malloc(sizeof(HeapEntry) + m_ulSize, MEMORY_ALLOCATION_ALIGNMENT));
			InterlockedPushEntrySList(&m_slAllocs, pAlloc);
		}
	}

public:
	CHeapManager(ULONG ulNumAllocs, ULONG ulSize, ULONG ulAllocDelta = 10)
	{
		m_ulAllocDelta = ulAllocDelta;
		m_ulSize = ulSize;
		m_ulInitialSize = ulNumAllocs;
		InitializeSListHead(&m_slAllocs);
		PrepAllocs(ulNumAllocs);
	}
	virtual ~CHeapManager()
	{
		Recycle(FALSE);
	}

	CHeapEntry Alloc()
	{
		HeapEntry *entry = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		if (entry == NULL)
		{
			PrepAllocs(m_ulAllocDelta);
			entry = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		}
		return CHeapEntry(entry, this);
	}

	void Free(HeapEntry *p)
	{
		InterlockedPushEntrySList(&m_slAllocs, p);
	}

	void Recycle(BOOL bRealloc = TRUE)
	{
		HeapEntry* pEntry = static_cast<HeapEntry*>(InterlockedFlushSList(&m_slAllocs));
		if (bRealloc)
		{
			PrepAllocs(m_ulInitialSize);
		}

		while (pEntry)
		{
			HeapEntry *pNext = static_cast<HeapEntry*>(pEntry->Next);
			_aligned_free(pEntry);
			pEntry = pNext;
		}
	}

	USHORT Size()
	{
		return QueryDepthSList(&m_slAllocs);
	}
};

CHeapEntry::CHeapEntry(HeapEntry *pData, CHeapManager *pMgr)
{
	m_pMgr = pMgr;
	m_pData = pData;
}
CHeapEntry::~CHeapEntry()
{
	m_pMgr->Free(m_pData);
}

CHeapEntry::operator void*()
{
	return m_pData->m;
}
