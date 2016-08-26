#ifndef __INFSTORE_UNIT_TEST_H__
#define __INFSTORE_UNIT_TEST_H__

#include "stdafx.h"
#include "UnitTest.h"

/*
 *  Test Database constants.
 */
#define TESTDB_ID			1990
#define TESTDB_PATH			"test.db"
#define TESTDB_NAME			"Test DB"
#define TESTDB_VERSION		88
#define TESTDB_HEADERSTR	"Test HDR"


/*
 *  (Test) Database header.
 */
struct db_header
{
	int		version;
	char	header[10];
};


/*
 *  (Test) database record.
 */
struct db_record
{
	char	name[10],
			surname[10];

	db_record()
	{
		name[0]    = '\0';
		surname[0] = '\0';
	}

	db_record(const char *name, const char *surname)
	{
		strcpy(this->name,    name);
		strcpy(this->surname, surname);
	}

	// compares 2 db_records
	static long Compare(void *record1, void *record2)
	{
		if (!record1 || !record2)
			return -999;
		return strcmp(((db_record*)record1)->name, ((db_record*)record2)->name);
	}
};




/*
 *  'Full' database record, which is made up of a record header and the actual
 *  record.
 */
struct db_full_record
{
	dbrecordhdr_t	recordHeader;	// header for each record
	db_record		record;			// actual data:  auto-inits

	db_full_record()
	{	memset(&recordHeader, 0, sizeof(recordHeader));		}

	db_full_record(const char *name, const char *surname)
	{
		strcpy(record.name,    name);
		strcpy(record.surname, surname);
		memset(&recordHeader, 0, sizeof(recordHeader));
	}
};




/******************************************************************************
 *  A version of mocked system calls which is related to dbinfstore_c.
 *
 *  dbinfstore_c creates a database with the following structure:
 *    - Main header:    dbmainhdr_t
 *    - Your DB header: in our case it will be db_header
 *    - Records:        a list of db_full_records
 *
 *  db_full_records is further divided into 2 structures:
 *    - Record header:  dbrecordhdr_t which is a header for each record
 *    - Actual record:  in this case it will be db_record.
 ******************************************************************************/
class InfoStoreMockSystemCalls : public SystemCallsMockEx<dbmainhdr_t, db_full_record>
{
protected:
	dbrecordhdr_t	tmpRecordHeader;

public:
	InfoStoreMockSystemCalls() : SystemCallsMockEx()
	{
		ON_CALL(*this, ReadFile(_, _, sizeof(dbmainhdr_t),   _, _)).WillByDefault(Invoke(this, &SystemCallsMockEx::Fake_ReadFile_Returns_Header));
		ON_CALL(*this, ReadFile(_, _, sizeof(db_full_record),_, _)).WillByDefault(Invoke(this, &SystemCallsMockEx::Fake_ReadFile_Returns_Record));
		ON_CALL(*this, ReadFile(_, _, sizeof(dbrecordhdr_t), _, _)).WillByDefault(Invoke(this, &InfoStoreMockSystemCalls::Fake_ReadFile_Returns_dbrecordhdr_t));
		ON_CALL(*this, ReadFile(_, _, sizeof(db_record),     _, _)).WillByDefault(Invoke(this, &InfoStoreMockSystemCalls::Fake_ReadFile_Returns_db_record));

		ON_CALL(*this, WriteFile(_, _, sizeof(db_full_record),_, _)).WillByDefault(Invoke(this, &SystemCallsMockEx::Fake_WriteFile_Record));
		ON_CALL(*this, WriteFile(_, _, sizeof(dbrecordhdr_t), _, _)).WillByDefault(Invoke(this, &InfoStoreMockSystemCalls::Fake_WriteFile_dbrecordhdr_t));
		ON_CALL(*this, WriteFile(_, _, sizeof(db_record),     _, _)).WillByDefault(Invoke(this, &InfoStoreMockSystemCalls::Fake_WriteFile_db_record));

		// set file pointer
		ON_CALL(*this, SetFilePointer(_, _, NULL, FILE_BEGIN)).WillByDefault(Invoke(this, &InfoStoreMockSystemCalls::Fake_SetFilePointer));

		// sets the header
		dbmainhdr_t mainHeader = {0};
		strncpy(mainHeader.signature, DB_SIGNATURE_REV_2002, sizeof(DB_SIGNATURE_REV_2002));
		mainHeader.version     = DB_VERSION;
		mainHeader.headersize  = sizeof(db_header);
		mainHeader.recordsize  = sizeof(db_record);
		itemsStored.SetHeader(mainHeader);
	}

	

	/*
	 *  Simulates ReadFile and returns db_header structure.
	 */
	BOOL Fake_ReadFile_Returns_db_header(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToRead != sizeof(db_header)  ||  !lpBuffer)
			return FALSE;

		db_header	header;

		// set header
		strcpy(header.header, TESTDB_HEADERSTR);
		header.version = TESTDB_VERSION;

		if (!lpBuffer)
			return FALSE;

		memcpy(lpBuffer, &header, sizeof(db_header));

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = sizeof(db_header);

		return TRUE;
	}



	/*
	 *  Simulates ReadFile and returns dbrecordhdr_t (header of a record)
	 */
	BOOL Fake_ReadFile_Returns_dbrecordhdr_t(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToRead != sizeof(dbrecordhdr_t)  ||  !lpBuffer)
			return FALSE;

		db_full_record	fullRecord;
		if (!itemsStored.ReadItem(&fullRecord, false))	// we are reading partial record, so we do not modify the index/pointer
			return FALSE;

		memcpy(lpBuffer, &fullRecord.recordHeader, sizeof(dbrecordhdr_t));

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = sizeof(dbrecordhdr_t);

		return TRUE;
	}


	/*
	 *  Simulates ReadFile and returns db_record.
	 */
	BOOL Fake_ReadFile_Returns_db_record(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToRead != sizeof(db_record)  ||  !lpBuffer)
			return FALSE;

		db_full_record	fullRecord;
		if (!itemsStored.ReadItem(&fullRecord, false))	// we are reading partial record, so we do not modify the index/pointer
			return FALSE;

		memcpy(lpBuffer, &fullRecord.record, sizeof(db_record));

		if (lpNumberOfBytesRead)
			*lpNumberOfBytesRead = sizeof(db_record);

		return TRUE;
	}



	/*
	 *  Simulates SetFilePointer() on ItemList recordsToBeRead.  This function
	 *  is called by dbinfstore_c::pointtorecord() and dbinfstore_c::getrecorddump()
	 *  set the file pointer prior than retreiving the stored record.
	 *
	 *  Thus this function, using lDistanceToMove, will calculate which item
	 *  (aka record) needs to be read afterwards.
	 */
	DWORD Fake_SetFilePointer(HANDLE hFile, LONG lDistanceToMove, PLONG lpDistanceToMoveHigh, DWORD dwMoveMethod)
	{
		LONG index = 0;

		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_EQ(FILE_BEGIN, dwMoveMethod);

		if (hFile != DEFAULT_HANDLE)
			return INVALID_SET_FILE_POINTER;

		if (lDistanceToMove > sizeof(dbmainhdr_t) + sizeof(db_header)) {
			index = (LONG) ( ((float)(lDistanceToMove - sizeof(dbmainhdr_t) /*size of main header*/ - sizeof(db_header) /*size of dbheader*/))
						    / ((float)(sizeof(dbrecordhdr_t) + sizeof(db_record))) );
		}

		itemsStored.index = index;
		return 0;
	}



	/*
	 *  Simulates WriteFile and *temporary* saves dbrecordhdr_t.
	 */
	BOOL Fake_WriteFile_dbrecordhdr_t(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToWrite != sizeof(dbrecordhdr_t)  ||  !lpBuffer)
			return FALSE;

		memcpy(&tmpRecordHeader, lpBuffer, sizeof(dbrecordhdr_t));

		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

		return TRUE;
	}


	/*
	 *  Simulates WriteFile and saves dbrecordhdr_t in ItemsList.
	 */
	BOOL Fake_WriteFile_Update_dbrecordhdr_t(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToWrite != sizeof(dbrecordhdr_t)  ||  !lpBuffer)
			return FALSE;

		memcpy(&tmpRecordHeader, lpBuffer, sizeof(dbrecordhdr_t));
		
		if (!PersistTmpRecordHeader())
			return FALSE;

		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

		return TRUE;
	}


	/*
	 *  Simulates WriteFile and saves db_record in ItemsList.
	 */
	BOOL Fake_WriteFile_db_record(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
	{
		EXPECT_EQ(DEFAULT_HANDLE, hFile);
		EXPECT_TRUE(lpBuffer != NULL);

		if (hFile != DEFAULT_HANDLE  ||  nNumberOfBytesToWrite != sizeof(db_record)  ||  !lpBuffer)
			return FALSE;

		db_full_record fullRecord;

		fullRecord.recordHeader = tmpRecordHeader;
		memcpy(&fullRecord.record, lpBuffer, sizeof(db_record));

		if (!itemsStored.WriteItem(&fullRecord))
			return FALSE;

		if (lpNumberOfBytesWritten)
			*lpNumberOfBytesWritten = nNumberOfBytesToWrite;

		return TRUE;
	}



	bool PersistTmpRecordHeader()
	{
		db_full_record fullRecord;
		
		if (!itemsStored.GetItem(itemsStored.index, fullRecord))
			return false;
		
		memcpy(&fullRecord.recordHeader, &tmpRecordHeader, sizeof(tmpRecordHeader));
		itemsStored.ModifyItem(itemsStored.index, fullRecord);
		return true;
	}
};




/******************************************************************************
 *  CfgStore fixture for automated testing...
 ******************************************************************************/
class dbinfstore_cTest : public testing::Test
{
protected:
	dbinfstore_c						*infoStore;
	NiceMock<InfoStoreMockSystemCalls>	systemCalls;
	NiceMock<hlp_c_ex_mock>				hlps;

	void SetUp()
	{
		SetHlpsMock(&hlps);

		infoStore = new dbinfstore_c;
		infoStore->systemCalls = &systemCalls;

		// initialise infoStore
		EXPECT_EQ(TRUE,  infoStore->init(sizeof(db_header), sizeof(db_record), TESTDB_ID));

		// open infoStore
		EXPECT_CALL(systemCalls, ReadFile(_, _, _, _, _)).WillOnce(Invoke(&systemCalls, &InfoStoreMockSystemCalls::Fake_ReadFile_Returns_Header));
		EXPECT_EQ(TRUE, infoStore->open(TESTDB_PATH));

		EXPECT_CALL(systemCalls, ReadFile(_, _, _, _, _)).Times(AnyNumber());
	}

	void TearDown()
	{	delete infoStore;	}
};

#endif