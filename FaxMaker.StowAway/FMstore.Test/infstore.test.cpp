/*
 *  Unit tests for class dbinfstore_c.
 */

#include "stdafx.h"
#include "InfstoreUnitTest.h"


/*
 *   Creates an empty DB.
 */
TEST(dbinfstore, create_DBCreatedWithCorrectPathAndName)
{
	dbinfstore_c						infoStore;
	NiceMock<InfoStoreMockSystemCalls>	systemCalls;
	char								dbname[32],
										dbpath[MAX_PATH];

	infoStore.systemCalls = &systemCalls;

	EXPECT_EQ(TRUE, infoStore.init(sizeof(db_header), sizeof(db_record), TESTDB_ID));
	EXPECT_EQ(TRUE, infoStore.create(TESTDB_PATH, TESTDB_NAME));

	// check if DB's name is equal to TESTDB_NAME
	EXPECT_STREQ(TESTDB_NAME, infoStore.getnameex());
	EXPECT_EQ(TRUE,           infoStore.getname(dbname, 32));
	EXPECT_STREQ(TESTDB_NAME, dbname);

	// check if DB's path is equal to TESTDB_PATH
	EXPECT_STREQ(TESTDB_PATH, infoStore.getpathex());
	EXPECT_EQ(TRUE,           infoStore.getpath(dbpath, MAX_PATH));
	EXPECT_STREQ(TESTDB_PATH, dbpath);
}


/*
 *  Opens a DB whose path exists.
 */
TEST(dbinfstore, open_PathExits_DBOpened)
{
	dbinfstore_c						infoStore;
	NiceMock<InfoStoreMockSystemCalls>	systemCalls;
	NiceMock<hlp_c_ex_mock>				hlps;

	infoStore.systemCalls = &systemCalls;
	infoStore.hlps = &hlps;

	EXPECT_EQ(TRUE, infoStore.init(sizeof(db_header), sizeof(db_record), TESTDB_ID));
	EXPECT_EQ(TRUE, infoStore.open(TESTDB_PATH));
}


TEST(dbinfstore, open_PathDoesNotExits_DBNotOpened)
{
	dbinfstore_c						infoStore;
	NiceMock<InfoStoreMockSystemCalls>	systemCalls;
	NiceMock<hlp_c_ex_mock>				hlps;

	infoStore.systemCalls = &systemCalls;
	infoStore.hlps = &hlps;

	EXPECT_CALL(hlps, doesfileexist(testing::MatcherCast<LPCSTR>(_))).WillOnce(Return(FALSE));

	EXPECT_EQ(TRUE, infoStore.init(sizeof(db_header), sizeof(db_record), TESTDB_ID));
	EXPECT_EQ(FALSE, infoStore.open("file.does.not.exist"));
}


/*
 *  Sets and gets DB's header.
 */
TEST_F(dbinfstore_cTest, setGetHeader_HeaderStoredCorrectly)
{
	db_header	header,
				headerReceived = {0};

	// set header
	strcpy(header.header, TESTDB_HEADERSTR);
	header.version = TESTDB_VERSION;

	// save header t
	EXPECT_EQ(TRUE, infoStore->setheader(&header));

	ON_CALL(systemCalls, ReadFile(_, _, sizeof(db_header),  _, _))
		.WillByDefault(Invoke(&systemCalls, &InfoStoreMockSystemCalls::Fake_ReadFile_Returns_db_header));

	// get the header from the DB
	EXPECT_EQ(TRUE, infoStore->getheader(&headerReceived));

	// check if its correct
	EXPECT_EQ(header.version, headerReceived.version);
	EXPECT_STREQ(header.header, headerReceived.header);
}



/*
 *  Generic function that adds 3 records to the database.
 */
inline void AddThreeRecords(dbinfstore_c *infoStore, InfoStoreMockSystemCalls &systemCalls)
{
	db_record	rec1("5name", "surname5"),
				rec2("1name", "surname1"),
				rec3("3name", "surname3");

	// store these 3 records
	EXPECT_NE(-1, infoStore->addrecord(&rec1));
	EXPECT_NE(-1, infoStore->addrecord(&rec2));
	EXPECT_NE(-1, infoStore->addrecord(&rec3));
}


/*
 *  Add 3 records to the database using addrecord().
 */
TEST_F(dbinfstore_cTest, addrecord_3Records_RecordsStoredCorrectly)
{
	// add 3 records to the database
	AddThreeRecords(infoStore, systemCalls);

	// make sure they are stored correctly
	ItemsList<dbmainhdr_t, db_full_record> *itemsStored = systemCalls.GetItemsStored();

	EXPECT_EQ(3, itemsStored->itemsList.size());
	EXPECT_STREQ("5name", itemsStored->itemsList[0].record.name);
	EXPECT_STREQ("1name", itemsStored->itemsList[1].record.name);
	EXPECT_STREQ("3name", itemsStored->itemsList[2].record.name);
}



/*
 *  Write 3 records to the DB using setrecord() and check if we can sort them.
 */
TEST_F(dbinfstore_cTest, cansort_3records_ReturnsTRUE)
{
	db_record	rec1("name11", "surname11"),
				rec2("name2",  "surname2"),
				rec3("name3",  "surname3");

	EXPECT_NE(-1, infoStore->setrecord(&rec1, 0));
	EXPECT_NE(-1, infoStore->setrecord(&rec2, 1));
	EXPECT_NE(-1, infoStore->setrecord(&rec3, 2));

	// now we check if we are able to sort records:  we should be able to
	// sort as we have no deleted records.
	EXPECT_EQ(TRUE, infoStore->cansort());
}



/*
 *  Sort records in ascending order.
 */
TEST_F(dbinfstore_cTest, sort_3Records_RecordsSortedCorrectly)
{
	// add 3 records to the database
	AddThreeRecords(infoStore, systemCalls);

	// perform the sorting operation
	EXPECT_EQ(TRUE, infoStore->sort(&db_record::Compare));

	// make sure items are sorted correctly
	ItemsList<dbmainhdr_t, db_full_record> *itemsStored = systemCalls.GetItemsStored();
	EXPECT_EQ(3, itemsStored->itemsList.size());
	EXPECT_STREQ("1name", itemsStored->itemsList[0].record.name);
	EXPECT_STREQ("3name", itemsStored->itemsList[1].record.name);
	EXPECT_STREQ("5name", itemsStored->itemsList[2].record.name);
}



/*
 *  Make sure that there are 3 records in the DB.
 */
TEST_F(dbinfstore_cTest, getrecordcount_3Records_Returns3)
{
	db_record	record;

	// add 3 records to the database
	AddThreeRecords(infoStore, systemCalls);

	// iterates each record:  we should be able to iterate though 3 records
	infoStore->findinit();

	// for the first 3 records, findnext() should NOT return -1
	EXPECT_NE(-1, infoStore->findnext(&record));
	EXPECT_NE(-1, infoStore->findnext(&record));
	EXPECT_NE(-1, infoStore->findnext(&record));
	
	// there is not 4th record, so findnext() should return -1
	EXPECT_EQ(-1, infoStore->findnext(&record));

	// we should have 3 records registered in the database
	EXPECT_EQ(3, infoStore->getrecordcount());
	EXPECT_EQ(3, infoStore->getrecordcountex());
}



/*
 *  Make sure that the 2nd record is stored correctly.
 */
TEST_F(dbinfstore_cTest, getrecord_2ndRecord_ReturnsCorrectData)
{
	db_record	recordReceived;

	AddThreeRecords(infoStore, systemCalls);
	
	EXPECT_EQ(TRUE, infoStore->getrecord(&recordReceived, 1));
	EXPECT_STREQ("1name", recordReceived.name);
	EXPECT_STREQ("surname1", recordReceived.surname);
}



/*
 *  Delete 1st record and check if the 1st one has been really deleted.
 */
TEST_F(dbinfstore_cTest, delrecord_DeleteFirstRecord_RecDeleted)
{
	AddThreeRecords(infoStore, systemCalls);

	EXPECT_EQ(TRUE, infoStore->delrecord(0));
	EXPECT_EQ(true, systemCalls.PersistTmpRecordHeader());
	
	// is the 1st record realy deleted?
	EXPECT_EQ(TRUE, infoStore->isdeleted(0));

	// there should be at most 2 records
	EXPECT_EQ(2, infoStore->getrecordcount());
}



/*
 *  Deletes all 3 records and make sure that 3 records were deleted.
 */
TEST_F(dbinfstore_cTest, deleteallrecords_3Records_EveryRecDeleted)
{
	long totalDeletedRecords = 0;

	// add 3 records
	AddThreeRecords(infoStore, systemCalls);

	//////////////////////////////////////////////////////////////////////////
	// generic file writes 
	EXPECT_CALL(systemCalls, WriteFile(_, _, _, _, _)).Times(AnyNumber());

	// dbrecordhdr_t file writes
	EXPECT_CALL(systemCalls, WriteFile(_, _, sizeof(dbrecordhdr_t), _, _))
		.WillRepeatedly(Invoke(&systemCalls, &InfoStoreMockSystemCalls::Fake_WriteFile_Update_dbrecordhdr_t));
	//////////////////////////////////////////////////////////////////////////

	// delete all the records!
	EXPECT_EQ(TRUE, infoStore->deleteallrecords());

	// get the total amount of deleted records
	infoStore->getrecordcount(&totalDeletedRecords);
	EXPECT_EQ(3, totalDeletedRecords);

	// there should be no records
	EXPECT_EQ(0, infoStore->getrecordcount());

	// now we check if we are able to sort records:  we should NOT be able to
	// sort as we have at least one (in this case 3) deleted record.
	EXPECT_EQ(FALSE, infoStore->cansort());
}



/*
 *  [Linear] Search for records.
 */
TEST_F(dbinfstore_cTest, search_Record_RecordFound)
{
	// add 3 records
	db_record	rec1("5name", "surname5"),
				rec2("1name", "surname1"),
				rec3("3name", "surname3");

	// store these 3 records
	EXPECT_NE(-1, infoStore->addrecord(&rec1));
	EXPECT_NE(-1, infoStore->addrecord(&rec2));
	EXPECT_NE(-1, infoStore->addrecord(&rec3));

	// search for the 3rd record:  search function should return 2 (index/position)
	EXPECT_EQ(2,  infoStore->search(&rec3, &db_record::Compare));

	// search for other records
	EXPECT_EQ(0,  infoStore->search(&rec1, &db_record::Compare));
	EXPECT_EQ(1,  infoStore->search(&rec2, &db_record::Compare));
}



/*
 *  [Binary] Search for records.
 */
TEST_F(dbinfstore_cTest, search_SortedRecords_RecordFound)
{
	// add 3 records
	db_record	rec1("5name", "surname5"),
				rec2("1name", "surname1"),
				rec3("3name", "surname3");

	// store these 3 records
	EXPECT_NE(-1, infoStore->addrecord(&rec1));
	EXPECT_NE(-1, infoStore->addrecord(&rec2));
	EXPECT_NE(-1, infoStore->addrecord(&rec3));

	// perform the sorting operation
	EXPECT_EQ(TRUE, infoStore->sort(&db_record::Compare));

	// This is how records are now sorted:
	// 0) rec2
	// 1) rec3
	// 2) rec1

	// search for the rec3:  search function should return 1 (index/position)
	EXPECT_EQ(1,  infoStore->search(&rec3, &db_record::Compare, TRUE));

	// search for other records
	EXPECT_EQ(2,  infoStore->search(&rec1, &db_record::Compare, TRUE));
	EXPECT_EQ(0,  infoStore->search(&rec2, &db_record::Compare, TRUE));
}



TEST_F(dbinfstore_cTest, getrecordsize_dbrecord_ReturnsSizeOfDbrecord)
{
	AddThreeRecords(infoStore, systemCalls);

	// size of record with index 0 should be equal to that of db_record
	EXPECT_EQ(sizeof(db_record), infoStore->getrecordsize(0));
	EXPECT_EQ(sizeof(db_record), infoStore->getrecordsize(1));
	EXPECT_EQ(sizeof(db_record), infoStore->getrecordsize(2));

	// record with index 3 does not exist
	EXPECT_EQ(-1, infoStore->getrecordsize(3));
}



TEST_F(dbinfstore_cTest, compact_1RecordDeletedOutOf3_2RecordsStored)
{
	// add 3 records
	AddThreeRecords(infoStore, systemCalls);

	// delete the first record
	EXPECT_EQ(TRUE, infoStore->delrecord(0));
	EXPECT_EQ(true, systemCalls.PersistTmpRecordHeader());

	// compact database
	EXPECT_EQ(TRUE, infoStore->compact());
	systemCalls.GetItemsStored()->itemsList.pop_back();	// simulates setfilelength()

	// now we should have 2 records stored in the DB
	EXPECT_EQ(2, infoStore->getrecordcount());
}
