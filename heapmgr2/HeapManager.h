#pragma once

struct HeapEntry : public SLIST_ENTRY
{
	BYTE m[1];
};

class CHeapManager;
class CHeapAlloc
{
private:
	CHeapManager *m_pMgr;
	HeapEntry *m_pData;
public:
	CHeapAlloc(HeapEntry *pData, CHeapManager *pMgr);
	~CHeapAlloc();

	operator void*();
};

class CHeapManager
{
private:
	ULONG m_ulAllocDelta;
	ULONG m_ulSize;
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
		InitializeSListHead(&m_slAllocs);
		PrepAllocs(ulNumAllocs);
	}
	virtual ~CHeapManager()
	{
		HeapEntry *pAlloc = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		while (pAlloc)
		{
			_aligned_free(pAlloc);
			pAlloc = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		}
	}

	CHeapAlloc Alloc()
	{
		HeapEntry *entry = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		if (entry == NULL)
		{
			PrepAllocs(m_ulAllocDelta);
			entry = static_cast<HeapEntry*>(InterlockedPopEntrySList(&m_slAllocs));
		}
		return CHeapAlloc(entry, this);
	}

	void Free(HeapEntry *p)
	{
		InterlockedPushEntrySList(&m_slAllocs, p);
	}

	void Recycle()
	{

	}
};

CHeapAlloc::CHeapAlloc(HeapEntry *pData, CHeapManager *pMgr)
{
	m_pMgr = pMgr;
	m_pData = pData;
}
CHeapAlloc::~CHeapAlloc()
{
	m_pMgr->Free(m_pData);
}

CHeapAlloc::operator void*()
{
	return m_pData->m;
}
