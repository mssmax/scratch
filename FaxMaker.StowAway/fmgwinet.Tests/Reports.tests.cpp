#include "StdAfx.h"
#ifdef   PERFORM_REPORTS_TESTS
#include "reports.tests.h"


/******************************************************************************
 *
 *  Testing CreateReport().
 */

TEST_F(ReportsFixture, CreateReport_3Recipients_CreatesAdminReportWith3RecipReturnsTrue)
{
	LPCWSTR recipients = L"Recip1;Recip2;Recip3";	// 3 recipients (only adminnotification uses ';' as recipient seperator)

	// make sure CreateReport() has successfully added 3 recipients
	EXPECT_CALL(GetFaxFrmtMsgMock(), RecipAdd(_,_))
		.Times(3);

	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, L"Sender", recipients, L"Subject", NULL, "RemoteID"));
}


TEST_F(ReportsFixture, CreateReport_3Recipients_CreatesReportWith3RecipReturnsTrue)
{
	LPCWSTR					recipients = L"Recip1\1Recip2\1Recip3";	// 3 recipients
	CReports::report_type	reportTypes[] = {CReports::mtaerror, CReports::smserror, CReports::smssent, CReports::smsreceived};

	// test for all report_types except for adminnotification
	for (short i = 0;  i < (sizeof(reportTypes) / sizeof(CReports::report_type));  i++)
	{
		// make sure CreateReport() has successfully added 3 recipients
		EXPECT_CALL(GetFaxFrmtMsgMock(), RecipAdd(_,_))
			.Times(3);

		EXPECT_EQ(TRUE, reports->CreateReport(reportTypes[i], L"Sender", recipients, L"Subject", NULL, "RemoteID"));
	}
}


TEST_F(ReportsFixture, CreateReport_InvalidParameters_HandlesThemAppropriately)
{
	LPCWSTR ppcustomfields[1][2];

	// invalid report type
	EXPECT_EQ(FALSE, reports->CreateReport((CReports::report_type)100, L"x", L"x", L"x", ppcustomfields, "x"));

	// all NULL parameters should be correctly handled by CreateReport()
	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, NULL, L"x", L"x", ppcustomfields, "x"));
	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, L"x", NULL, L"x", ppcustomfields, "x"));
	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, L"x", L"x", NULL, ppcustomfields, "x"));
	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, L"x", L"x", L"x", NULL,           "x"));
	EXPECT_EQ(TRUE, reports->CreateReport(CReports::adminnotification, L"x", L"x", L"x", ppcustomfields, NULL));
}


/******************************************************************************
 *
 *  Testing CreateSmsReport().
 */

TEST_F(ReportsFixture, CreateSmsReport_3Recipients_CreatesSMSReportWith3RecipReturnsTrue)
{
	LPCWSTR recipients = L"Recip1\1Recip2\1Recip3";	// 3 recipients

	// make sure CreateReport() has successfully added 3 recipients
	EXPECT_CALL(GetFaxFrmtMsgMock(), RecipAdd(_,_))
		.Times(3);

	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, recipients, L"Subject", L"Status", L"Description", L"Text", L"10", "79123456"));
}


TEST_F(ReportsFixture, CreateSmsReport_InvalidParameters_HandlesThemAppropriately)
{
	// invalid report type
	EXPECT_EQ(FALSE, reports->CreateSmsReport((CReports::report_type)100, L"R", L"Sub", L"Stat", L"Des", L"Text", L"10", "123456"));

	// all NULL parameters should be correctly handled by CreateSmsReport()
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, NULL, L"Sub", L"Stat", L"Des", L"Text", L"10", "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", NULL,   L"Stat", L"Des", L"Text", L"10", "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", L"Sub", NULL,    L"Des", L"Text", L"10", "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", L"Sub", L"Stat", NULL,   L"Text", L"10", "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", L"Sub", L"Stat", L"Des", NULL,    L"10", "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", L"Sub", L"Stat", L"Des", L"Text", NULL,  "123456"));
	EXPECT_EQ(TRUE, reports->CreateSmsReport(CReports::smserror, L"R", L"Sub", L"Stat", L"Des", L"Text", L"10", NULL));
}


/******************************************************************************
 *
 *  Testing CreateBodyReport().
 */

TEST_F(ReportsFixture, CreateBodyReport_InvalidParameters_ReturnsFalse)
{
	LPCWSTR ppcustomfields[1][2];

	EXPECT_EQ(FALSE, reports->CreateBodyReport(NULL, 1, "T",  ppcustomfields));
	EXPECT_EQ(FALSE, reports->CreateBodyReport("F",  0, "T",  ppcustomfields));
	EXPECT_EQ(FALSE, reports->CreateBodyReport("F",  1, NULL, ppcustomfields));
	// ppcustomfields is handled by CreateFileFromTemplate()...
}


/******************************************************************************
 *
 *  Testing RenameMessage().
 */

TEST_F(ReportsFixture, RenameMessage_OperationFailureDeleteOnFailure_FaxFormatMessageDeleted)
{
	// simulate message renaming operation failure
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(FALSE));

	// faxfrmtDeleteMessage() will be called since delete on failure is TRUE
	EXPECT_CALL(GetGlobalFaxFmtMock(), faxfrmtDeleteMessage(_))
		.Times(1);

	EXPECT_EQ(FALSE, reports->RenameMessage("C:\\source.path", TRUE /*delete on fail*/, NULL));
}


TEST_F(ReportsFixture, RenameMessage_OperationFailureDoNotDeleteOnFailure_FaxFormatMessageRetained)
{
	// simulate message renaming operation failure
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(FALSE));

	// faxfrmtDeleteMessage() will *NOT* be called since delete on failure is FALSE
	EXPECT_CALL(GetGlobalFaxFmtMock(), faxfrmtDeleteMessage(_))
		.Times(0);

	EXPECT_EQ(FALSE, reports->RenameMessage("C:\\source.path", FALSE /*delete on fail*/, NULL));
}


TEST_F(ReportsFixture, RenameMessage_InvalidParameters_HandlesThemAppropriately)
{
	// first param (lpszsourcepath) cannot be NULL
	EXPECT_EQ(FALSE, reports->RenameMessage(NULL, TRUE, "P"));
	
	// last param (lpszprefix) can be NULL
	EXPECT_EQ(TRUE, reports->RenameMessage("S",  TRUE, NULL));
}


/******************************************************************************
 *
 *  Testing CreateAdminMessage().
 */

TEST_F(ReportsFixture, CreateAdminMessage_InvalidParameters_HandlesThemAppropriately)
{
	EXPECT_EQ(TRUE, reports->CreateAdminMessage(NULL,  "Attch", 0, "Sub"));
	EXPECT_EQ(TRUE, reports->CreateAdminMessage("Des", NULL,    0, "Sub"));
	EXPECT_EQ(TRUE, reports->CreateAdminMessage("Des", "Attch", 0, NULL));
}



#endif	// PERFORM_REPORTS_TESTS