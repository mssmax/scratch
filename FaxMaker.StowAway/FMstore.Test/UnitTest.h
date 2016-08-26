#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__

/*
 *  Generic set of classes and structures used for unit testing.
 */

#include "stdafx.h"
#include <mfg.h>
#include <vector>

using ::testing::_;
using ::testing::AtLeast;
using ::testing::AnyNumber;
using ::testing::Return;
using ::testing::Invoke;
using ::testing::InSequence;
using ::testing::TypedEq;
using ::testing::NiceMock;
using std::vector;


/******************************************************************************
 *  A generic version of mocked system calls.
 *
 *  Makes sure that each file opened is eventaully closed, otherwise it will
 *  display a warning.  If a file is not close, it will display a warning.
 ******************************************************************************/
class SystemCallsMock2 : public SystemCalls
{
private:
	int	openedFileHandles;	// total number of opened file handles:  manipulated by Fake_CreateFile and Fake_CloseHandle
	#define	DEFAULT_HANDLE				((HANDLE)(LONG_PTR)123)	// default file handle used by fake functions
	#define	DEFAULT_MUTEX_HANDLE		((HANDLE)(LONG_PTR)456)
	#define DEFAULT_FILE_MAPPING_HANDLE	((LPVOID)789)

public:
	SystemCallsMock2()
    {
		ON_CALL(*this, CreateFileA(_, _, _, _, _,_,_)).WillByDefault(Invoke(this, &SystemCallsMock2::Fake_CreateFileA));
		ON_CALL(*this, CloseHandle(_)).WillByDefault(Invoke(this, &SystemCallsMock2::Fake_CloseHandle));
		ON_CALL(*this, ReadFile(_, _, _, _, _)).WillByDefault(Invoke(this, &SystemCallsMock2::Fake_ReadFile));
		ON_CALL(*this, WriteFile(_, _, _, _, _)).WillByDefault(Invoke(this, &SystemCallsMock2::Fake_WriteFile));
		ON_CALL(*this, SetFilePointer(_, _, _, _)).WillByDefault(Return(123));
		ON_CALL(*this, GetFileTime(_, _, _, _)).WillByDefault(Return(TRUE));
		ON_CALL(*this, CreateMutexA(_, _, _)).WillByDefault(Return(DEFAULT_MUTEX_HANDLE));
		ON_CALL(*this, InitializeSecurityDescriptor(_, _)).WillByDefault(Return(TRUE));
		ON_CALL(*this, SetSecurityDescriptorDacl(_, _, _, _)).WillByDefault(Return(TRUE));
		ON_CALL(*this, WaitForSingleObject(DEFAULT_MUTEX_HANDLE, _)).WillByDefault(Return(WAIT_OBJECT_0));
		ON_CALL(*this, ReleaseMutex(DEFAULT_MUTEX_HANDLE)).WillByDefault(Return(TRUE));
		ON_CALL(*this, CreateFileMappingA(_, _, _, _, _, _)).WillByDefault(Return(DEFAULT_FILE_MAPPING_HANDLE));
		ON_CALL(*this, MapViewOfFile(_, _, _, _, _)).WillByDefault(Return(DEFAULT_FILE_MAPPING_HANDLE));

		openedFileHandles = 0;
    }

	~SystemCallsMock2()
	{	EXPECT_EQ(0, openedFileHandles);	}

	MOCK_METHOD7(CreateFileA,	HANDLE(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile));
	MOCK_METHOD1(CloseHandle,	BOOL(HANDLE hObject));
	MOCK_METHOD5(ReadFile,		BOOL(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped));
	MOCK_METHOD5(WriteFile,		BOOL(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped));
	MOCK_METHOD4(SetFilePointer,DWORD(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod));
	MOCK_METHOD2(GetFileSizeEx, BOOL(HANDLE hFile, PLARGE_INTEGER lpFileSize));
	MOCK_METHOD4(GetFileTime,	BOOL(HANDLE hFile, LPFILETIME lpCreationTime, LPFILETIME lpLastAccessTime, LPFILETIME lpLastWriteTime));

	MOCK_METHOD3(CreateMutexA,	HANDLE(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR));
	MOCK_METHOD2(InitializeSecurityDescriptor,	BOOL(PSECURITY_DESCRIPTOR pSecurityDescriptor, DWORD dwRevision));
	MOCK_METHOD4(SetSecurityDescriptorDacl,		BOOL(PSECURITY_DESCRIPTOR pSecurityDescriptor, BOOL bDaclPresent, PACL pDacl, BOOL bDaclDefaulted));
	MOCK_METHOD2(WaitForSingleObject,			DWORD(HANDLE hHandle, DWORD dwMilliseconds));
	MOCK_METHOD1(ReleaseMutex,	BOOL(HANDLE hMutex));
	
	MOCK_METHOD6(CreateFileMappingA, HANDLE(HANDLE hFile, LPSECURITY_ATTRIBUTES lpFileMappingAttributes, DWORD flProtect, DWORD dwMaximumSizeHigh, DWORD dwMaximumSizeLow, LPCSTR lpName));
	MOCK_METHOD5(MapViewOfFile,      LPVOID(HANDLE hFileMappingObject, DWORD dwDesiredAccess, DWORD dwFileOffsetHigh, DWORD dwFileOffsetLow, SIZE_T dwNumberOfBytesToMap));


	virtual HANDLE Fake_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
	{
		openedFileHandles++;
		return DEFAULT_HANDLE;
	}


	virtual BOOL Fake_CloseHandle(HANDLE hObject)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hObject);
		EXPECT_GT(openedFileHandles, 0);	// expect that openedFileHandles > 0

		if ((hObject != DEFAULT_HANDLE)  ||  (openedFileHandles <= 0))
			return FALSE;

		openedFileHandles--;
		return TRUE;
	}


	virtual BOOL Fake_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE)
			return FALSE;

		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

		return TRUE;
	}


	virtual BOOL Fake_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE)
			return FALSE;

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = nNumberOfBytesToRead;

		return TRUE;
	}
};





/******************************************************************************
 *  An extended version of mocked system calls that has more features related
 *  to filesystem system calls.
 *
 *  This class will assume that you are writing to file that is made up of 2
 *  sections:
 *    - Header
 *    - Records
 ******************************************************************************/
template <class HeaderType, class ItemType>  struct ItemsList;

template <class HeaderType, class ItemType>
class SystemCallsMockEx : public SystemCallsMock2
{
protected:
	ItemsList<HeaderType, ItemType>	itemsStored;

public:
	SystemCallsMockEx() : SystemCallsMock2()
	{}

	virtual ItemsList<HeaderType, ItemType>* GetItemsStored()
	{	return &itemsStored;		}


	/*
	 *  Simulates ReadFile and returns header structure.
	 *
	 *  You need to set header (itemsStored.SetHeader()) before you call this
	 *  function.
	 */
	BOOL Fake_ReadFile_Returns_Header(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToRead != sizeof(HeaderType)  ||  !lpBuffer)
			return FALSE;

		memcpy(lpBuffer, itemsStored.GetHeader(), sizeof(HeaderType));

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = sizeof(HeaderType);

		return TRUE;
	}



	/*
	 *  Simulates ReadFile and returns a stored record structure.
	 */
	BOOL Fake_ReadFile_Returns_Record(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToRead != sizeof(ItemType)  ||  !lpBuffer)
			return FALSE;

		if (!itemsStored.ReadItem(lpBuffer))
			return FALSE;

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = sizeof(ItemType);

		return TRUE;
	}



	/*
	 *  Simulates WriteFile and saves record into ItemsList.
	 */
	BOOL Fake_WriteFile_Record(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToWrite != sizeof(db_full_record)  ||  !lpBuffer)
			return FALSE;

		if (!itemsStored.WriteItem(lpBuffer))
			return FALSE;

		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

		return TRUE;
	}
};





/******************************************************************************
 *  List of items that will be used to simalate multiple-item reading and
 *  writing to/from the filesystem.
 *
 *  Used by SystemCallsMockEx.
 ******************************************************************************/
template <class HeaderType, class ItemType>
struct ItemsList
{
	HeaderType			header;
	vector<ItemType>	itemsList;	// list of items/records that will be used during the simulation
	long				index;		// index of the next item to be read/written

	ItemsList()
	{	index = 0;		}

	void Clear()
	{
		itemsList.clear();
		index = 0;
	}

	void SetHeader(HeaderType &header)
	{	memcpy(&this->header, &header, sizeof(HeaderType));	}

	HeaderType* GetHeader()
	{	return &this->header;	}


	void AddItem(ItemType &item)
	{	itemsList.push_back(item);	}


	bool GetItem(int position, ItemType &item)
	{
		if ((position < 0)  ||  (position >= (long)itemsList.size()))
			return NULL;

		memcpy(&item, &(itemsList[position]), sizeof(ItemType));
		return true;
	}

	
	/*
	 *  Simulate read from file system.
	 */
	bool ReadItem(void *itemBuffer, bool modifyItemIndex = true)
	{
		if (!itemBuffer  ||  (index >= (long)itemsList.size()))
			return false;

		memcpy(itemBuffer, &(itemsList[index]), sizeof(ItemType));
		if (modifyItemIndex) index++;

		return true;
	}


	/*
	 *  Simulate write to the file system.
	 */
	bool WriteItem(const void *itemBuffer)
	{
		if (!itemBuffer)
			return false;

		ItemType  item;
		memcpy(&item, (ItemType *)itemBuffer, sizeof(ItemType));
		
		if (!ModifyItem(index, item))
			return false;

		index++;
		return true;
	}


	/*
	 *  Modifies (or adds) an item at the given position/index.
	 */
	bool ModifyItem(int position, ItemType &item)
	{
		if ((position < 0)  ||  (position > (long)itemsList.size()))
			return false;

		// append at the end of the list
		if (position >= (long)itemsList.size())
			itemsList.push_back(item);

		// else we are overwriting the item in the middle
		else
			itemsList[position] = item;	// copy data to itemsList

		return true;
	}
};



/*******************/



#endif