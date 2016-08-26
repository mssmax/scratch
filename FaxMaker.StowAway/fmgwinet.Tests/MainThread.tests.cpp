#include "StdAfx.h"

#ifdef PERFORM_MAINTHREAD_TESTS

#include "MainThread.tests.h"

// start threads only
TEST_F(MainThreadFixture, restart_StartThreadsOnly_ReturnsTrue)
{
	EXPECT_CALL(mtaConfigMock, loadconfig(_)).WillOnce(Return(TRUE));
	EXPECT_EQ(TRUE, mainThread->restart(TRUE /*start*/, TRUE));
}


// restart main thread (MTA is a service)
TEST_F(MainThreadFixture, restart_RestartMTAService_ReturnsTrue)
{
	EXPECT_CALL(fmgwinetAppMock, IsService()).WillOnce(Return(TRUE));
	EXPECT_EQ(TRUE, mainThread->restart(FALSE /*restart*/, TRUE));
}


// restart main thread (MTA is a process)
TEST_F(MainThreadFixture, restart_RestartMTAProcess_ReturnsTrue)
{
	EXPECT_CALL(fmgwinetAppMock, IsService())
		.Times(2)
		.WillRepeatedly(Return(FALSE));
	EXPECT_EQ(TRUE, mainThread->restart(FALSE /*restart*/, TRUE));
}


// restart main thread for the second time (MTA is a process)
TEST_F(MainThreadFixture, restart_Restart2ndTimeMTAProcess_ReturnsTrue)
{
	mainThread->SetRestartThreadStarted(TRUE);	// simulate 2nd restart operation

	EXPECT_CALL(fmgwinetAppMock, IsService())
		.Times(3)
		.WillRepeatedly(Return(FALSE));
	EXPECT_CALL(mtaConfigMock, loadconfig(_)).WillOnce(Return(TRUE));

	EXPECT_EQ(TRUE, mainThread->restart(FALSE /*restart*/, TRUE));
}


TEST_F(MainThreadFixture, checkqueueprocesscommands_ReturnsTrue)
{
	EXPECT_CALL(GetFmgwinetAppMock()->GetDownloadThreadMock(), OverrideCycle());
	EXPECT_CALL(GetFmgwinetAppMock()->GetUploadThreadMock(),   OverrideCycle());
	EXPECT_EQ(TRUE, mainThread->checkqueueprocesscommands());
}


TEST_F(MainThreadFixture, GetExchVersions_ADGetSuccess_ReturnsOK)
{
	str_c versStr;

	EXPECT_CALL(directorySearchMock, GetNextRow(_))
		.WillOnce(Return(S_OK))
		.WillOnce(Return(S_OK))
		.WillOnce(Return(S_ADS_NOMORE_ROWS));

	ASSERT_HRESULT_SUCCEEDED( mainThread->GetExchVersions(versStr) );
	EXPECT_STREQ(EXCHVERSION, (LPCSTR)versStr);
}


TEST_F(MainThreadFixture, GetExchVersions_ADGetFailure_ReturnsError)
{
	str_c versStr;

	EXPECT_CALL(aDsMock, Get(_,_)).WillOnce(Return(E_FAIL));
	EXPECT_HRESULT_FAILED( mainThread->GetExchVersions(versStr) );
}


TEST_F(MainThreadFixture, AddTelemetyData_InstallModeIs1_ReturnOK)
{
	PrepareAddTelemetyData();
	EXPECT_HRESULT_SUCCEEDED( mainThread->AddTelemetyData() );
}


TEST_F(MainThreadFixture, AddTelemetyData_InstallModeIs0_ReturnError)
{
	mtaConfigMock.set_installmode(0);
	EXPECT_HRESULT_FAILED( mainThread->AddTelemetyData() );
}


TEST_F(MainThreadFixture, StartWork_ReturnsTrue)
{
	PrepareAddTelemetyData();

	EXPECT_CALL(mtaConfigMock, loadconfig(_)).WillOnce(Return(TRUE));
	EXPECT_EQ(TRUE, mainThread->StartWork());
}


TEST_F(MainThreadFixture, StartWork_StartThreadFailure_ReturnsFalse)
{
	PrepareAddTelemetyData();

	EXPECT_CALL(mtaConfigMock, loadconfig(_)).WillOnce(Return(TRUE));
	EXPECT_CALL(fmgwinetAppMock, StartThreads()).WillOnce(Return(FALSE));
	EXPECT_EQ(FALSE, mainThread->StartWork());
}


TEST_F(MainThreadFixture, DoWork_7FilesInQueue_7FilesFound)
{
	mtaConfigMock.set_indir("in");
	mtaConfigMock.set_outdir("out");

	EXPECT_CALL(fileFindMock, FindFile(StrEq("in\\*.sfm"), _)).WillOnce(Return(TRUE));
	EXPECT_CALL(fileFindMock, FindFile(StrEq("out\\*.sfm"), _)).WillOnce(Return(TRUE));
	EXPECT_CALL(fileFindMock, FindFile(StrEq("out\\pending\\*.sfm"), _)).WillOnce(Return(TRUE));
	EXPECT_CALL(fileFindMock, FindNextFile())
		.Times(7)					/////////////////////////////////
		.WillOnce(Return(TRUE))		// 3 items in the IN queue (1 FindFile() + 2 FindNextFile())
		.WillOnce(Return(TRUE))		//
		.WillOnce(Return(FALSE))	/////////////////////////////////
		.WillOnce(Return(TRUE))		// 2 item in the OUT queue
		.WillOnce(Return(FALSE))	/////////////////////////////////
		.WillOnce(Return(TRUE))		// 2 items in the PENDING queue
		.WillOnce(Return(FALSE));	/////////////////////////////////

	// for checkqueueprocesscommands()
	EXPECT_CALL(GetFmgwinetAppMock()->GetDownloadThreadMock(), OverrideCycle());
	EXPECT_CALL(GetFmgwinetAppMock()->GetUploadThreadMock(),   OverrideCycle());

	mainThread->DoWork();
	EXPECT_EQ(3, mainThread->GetItemsInQueue());
	EXPECT_EQ(4, mainThread->GetItemsOutPendingQueue());
}


TEST_F(MainThreadFixture, DoWork_0FilesInQueue_0FilesFound)
{
	mtaConfigMock.set_indir("in");
	mtaConfigMock.set_outdir("out");

	EXPECT_CALL(fileFindMock, FindFile(StrEq("in\\*.sfm"), _)).WillOnce(Return(FALSE));
	EXPECT_CALL(fileFindMock, FindFile(StrEq("out\\*.sfm"), _)).WillOnce(Return(FALSE));
	EXPECT_CALL(fileFindMock, FindFile(StrEq("out\\pending\\*.sfm"), _)).WillOnce(Return(FALSE));

	// for checkqueueprocesscommands()
	EXPECT_CALL(GetFmgwinetAppMock()->GetDownloadThreadMock(), OverrideCycle());
	EXPECT_CALL(GetFmgwinetAppMock()->GetUploadThreadMock(),   OverrideCycle());

	mainThread->DoWork();
	EXPECT_EQ(0, mainThread->GetItemsInQueue());
	EXPECT_EQ(0, mainThread->GetItemsOutPendingQueue());
}


/******************************************************************************/

void MainThreadFixture::PrepareAddTelemetyData()
{
	mtaConfigMock.set_installmode(1);

	EXPECT_CALL(directorySearchMock, GetNextRow(_))
		.WillOnce(Return(S_OK))
		.WillOnce(Return(S_OK))
		.WillOnce(Return(S_ADS_NOMORE_ROWS));
}

#endif //PERFORM_MAINTHREAD_TESTS