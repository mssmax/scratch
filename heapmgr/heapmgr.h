#ifndef __midl

#pragma once

#include <wtypes.h>

#define DEFAULT_HEAP_SIZE 65535

#ifdef HEAPMGR_EXPORTS
	#define HEAPMGR_API __declspec(dllexport)
#else
	#define HEAPMGR_API __declspec(dllimport)
#endif



class HEAPMGR_API CHeap
{
private:

	class HEAPMGR_API CThinLock
	{
	private:
		
		volatile LONG m_lTicket;
		volatile LONG m_lQueue;
		volatile LONG m_lContentionCount;
		volatile DWORD m_dwLastOwningThread;
				
	public:
				
		void Init()
		{
			m_lTicket = 0;
			m_lQueue = 1;
			m_lContentionCount = 0;
			m_dwLastOwningThread = 0;
		}
		
		void Enter()
		{
			LONG lTicket = InterlockedIncrement(&m_lTicket);
			if(lTicket != m_lQueue)
				InterlockedIncrement(&m_lContentionCount);
			long lCounter = 0;
			while(lTicket != m_lQueue)
			{
				if(lCounter++ > 4000)
				{
					Sleep(0);
					lCounter = 0;
				}
			}
			m_dwLastOwningThread = GetCurrentThreadId();
		}
		
		void Leave()
		{
			m_lQueue++;
		}
	} m_cs;

	HANDLE m_hHeap;
	// =======================================================================================
	/*
		The m_lAllocCount field is actually a compound one. The lowest bit indicates whether
		the heap is to be destroyed by the last deallocator ( a thread that returns last piece
		of memory into the heap ), while the remaining bits are used for allocation count
		( i.e. how many outstanding allocations are currently there in the heap ).
		The allocation count/deferred bit are needed for sitations where a thread that
		originally created the heap is being destroyed ( DLL_THREAD_DETACH notification received )
		but there are still live allocations present ( e.g. long-living variables, such as configuration settings )
		The idea is that we set the deferred bit if there are still allocations, however,
		if they were represented by two variables we could have a race condition where they are not consistently
		updated against each other. Using such breakdown of bits on a single variable we can introduce
		atomicity when it comes to updating the deferred bit if the count is not zero.		
	*/
	// =======================================================================================
	volatile LONG m_lAllocCount;
	
	// used by FastAlloc and FastFree
	volatile LONG m_lFastAllocCount;

	struct MEMORY_SEGMENT
	{
		MEMORY_SEGMENT *pNext;
		DWORD dwTotalBytes;
		DWORD dwCurrentPointer;
		BYTE Segment[1];
	} *m_SegListHead;


	static void FreeMemorySegmentList(MEMORY_SEGMENT *pHead);

public:


	static BOOL CreateNewHeap();

	static void DestroyHeap();

	// allocates/frees memory from a thread's private heap
	static void *Alloc(size_t size);
	static void Free(void *p);

	// allocates/frees memory from the process heap
	static void *StableAlloc(size_t size);
	static void StableFree(void *p);

	// used for temporary objects that a thread needs while processing
	static void *FastAlloc(size_t size);
	static void FastFree(void *p);
};

typedef DECLSPEC_ALIGN(MEMORY_ALLOCATION_ALIGNMENT) struct
{
	CHeap *pHeap;
} PHEAP;

extern void * __cdecl operator new(size_t size);
extern void __cdecl operator delete(void *p);
extern void * __cdecl operator new[](size_t size);
extern void __cdecl operator delete[](void *p);

#include <string>
#include <vector>
#include <map>
#include <list>
#include <deque>

/*
template<class Type>
class fastallocator : public std::allocator<Type>
{
	public:

	template <class U> struct rebind { typedef fastallocator<U>
                                         other; };
	pointer allocate(size_type _Count, const void* _Hint)
	{
		return (pointer)CHeap::FastAlloc(_Count * sizeof(Type));
	}

	void deallocate(pointer _Ptr, size_type _Count)
	{
		CHeap::FastFree(_Ptr);
	}
};
/**/

template <typename T> class fastallocator;

template <> class fastallocator<void>
{
public:
    typedef void* pointer;
    typedef const void* const_pointer;
    // reference to void members are impossible.
    typedef void value_type;
    template <class U> 
        struct rebind { typedef fastallocator<U> other; };
};    

namespace fastalloc{
    inline void destruct(char *){}
    inline void destruct(wchar_t*){}
    template <typename T> 
        inline void destruct(T *t){t->~T();}
} // namespace
    
template <typename T>
class fastallocator
{
public:
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef T value_type;

    template <class U> 
        struct rebind { typedef fastallocator<U> other; };
    fastallocator(){}
    pointer address(reference x) const {return &x;}
    const_pointer address(const_reference x) const {return &x;}
    pointer allocate(size_type size, fastallocator<void>::const_pointer hint = 0)
    {
        return static_cast<pointer>(CHeap::FastAlloc(size*sizeof(T)));
    }
    //for Dinkumware:
    char *_Charalloc(size_type n)
	{
		return static_cast<char*>(CHeap::FastAlloc(n));
	}
    // end Dinkumware

    template <class U> fastallocator(const fastallocator<U>&){}
    void deallocate(pointer p, size_type n)
    {
		CHeap::FastFree(p);
    }
    void deallocate(void *p, size_type n)
    {
		CHeap::FastFree(p);
    }
    size_type max_size() const throw() 
	{
		return size_t(-1) / sizeof(value_type);
	}
    void construct(pointer p, const T& val)
    {
        new(static_cast<void*>(p)) T(val);
    }
    void construct(pointer p)
    {
        new(static_cast<void*>(p)) T();
    }
    void destroy(pointer p)
	{
		fastalloc::destruct(p);
	}
    static void dump(){};
};

template <typename T, typename U>
inline bool operator==(const fastallocator<T>&, const fastallocator<U>){return true;}

template <typename T, typename U>
inline bool operator!=(const fastallocator<T>&, const fastallocator<U>){return false;}



typedef std::basic_string<char, std::char_traits<char>, fastallocator<char> > fstring;
typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, fastallocator<wchar_t> > fwstring;

template<class Type>
class fvector : public std::vector<Type, fastallocator<Type> >
{
};

template <
   class Key, 
   class Type, 
   class Traits = std::less<Key> >
class fmap : public std::map<Key, Type, Traits, fastallocator<Type> >
{
};


template <class Type>
class flist : public std::list<Type, fastallocator<Type> >
{
};

template <class Type>
class fdeque : public std::deque<Type, fastallocator<Type> >
{
};

#endif