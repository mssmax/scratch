#include "stdafx.h"

#ifdef PERFORM_UPLOADTHREAD_TESTS

#include "UploadThread.Tests.h"

using ::testing::_;
using ::testing::Return;
using ::testing::HasSubstr;
using ::testing::DoAll;
using ::testing::StrEq;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::ElementsAre;
using ::testing::Pointee;
using ::testing::Contains;
using ::testing::AllOf;
using ::testing::Invoke;

//==============================================================================
// CUploadThread::GetFaxSenderNumber
//==============================================================================
     
TEST_F(CUploadThread_fxt, GetFaxSenderNumber_noquotedaddressspecAndFaxNumberContainsNumbers_returnsPlainNumber)
{
	m_config.set_noquotedaddressspec(1);
	personalinfo_s pi;
	strcpy(pi.faxnumber, "+999 123AAA");
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_STREQ(L"999123", LPCWSTR(sNumber));
}

TEST_F(CUploadThread_fxt, GetFaxSenderNumber_quotedaddressspecAndFaxNumberContainsNumbers_returnsQuotedNumber)
{
	m_config.set_noquotedaddressspec(0);
	personalinfo_s pi;
	strcpy(pi.faxnumber, "+999 123AAA");
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_STREQ(L"\"+999 123\"", LPCWSTR(sNumber));
}

TEST_F(CUploadThread_fxt, GetFaxSenderNumber_noquotedaddressspecRemoteIdContainsNumbers_returnsQuotedNumber)
{
	m_config.set_noquotedaddressspec(1);
	personalinfo_s pi={};
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));

	faxmessage_t fm;
	strcpy(fm.remoteid, "#456BBB");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_STREQ(L"456", LPCWSTR(sNumber));
}

TEST_F(CUploadThread_fxt, GetFaxSenderNumber_quotedaddressspecRemoteIdContainsNumbers_returnsQuotedNumber)
{
	m_config.set_noquotedaddressspec(0);
	personalinfo_s pi={};
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));

	faxmessage_t fm;
	strcpy(fm.remoteid, "#456BBB");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_STREQ(L"\"#456\"", LPCWSTR(sNumber));
}

TEST_F(CUploadThread_fxt, GetFaxSenderNumber_faxNumberAndRemoteIdContainNumbers_returnsFaxNumber)
{
	personalinfo_s pi;
	strcpy(pi.faxnumber, "+999 123AAA");
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));

	faxmessage_t fm;
	strcpy(fm.remoteid, "456BBB");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_THAT(LPCWSTR(sNumber), HasSubstr(L"123"));
}

TEST_F(CUploadThread_fxt, GetFaxSenderNumber_faxNumberAndRemoteIdDoNotContainNumbers_returnsEmptyString)
{
	personalinfo_s pi;
	strcpy(pi.faxnumber, "AAA");
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));

	faxmessage_t fm;
	strcpy(fm.remoteid, "BBB");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	
	strw_c sNumber(1024);
	m_ut.GetFaxSenderNumber(&m_ffm, sNumber);
	ASSERT_STREQ(L"", LPCWSTR(sNumber));
}

//==============================================================================
// CUploadThread::BuildEmailSender
//==============================================================================
    
TEST_F(CUploadThread_fxt, BuildEmailSender_withNumber_)
{
	m_config.set_installmode(2);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("sender@server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The Sender"));
	//EXPECT_CALL(m_msg, ConstructEmail(_, _, StrEq(L"server@sender.net"), StrEq(L"123456"), StrEq(L"faxm.net"), _));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The Sender\" <123456@faxm.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<123456@faxm.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_withoutNumber_)
{
	m_config.set_installmode(2);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("sender@server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The Sender\" <fax@faxm.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<fax@faxm.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_mailboxModeWithValidServerSender_)
{
	m_config.set_installmode(3);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("sender@server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The Sender\" <sender@server.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<sender@server.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_mailboxModeWithPop3AndMailbox_)
{
	m_config.set_installmode(3);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("");
	m_config.set_mailbox("jo");
	m_config.set_pop3("mail.server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The Sender\" <jo@mail.server.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<jo@mail.server.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_mailboxModeWithNoServerSenderNorMailboxConfigured_)
{
	m_config.set_installmode(3);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("");
	m_config.set_mailbox("");
	m_config.set_pop3("");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"fax@faxmaker.com", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<fax@faxmaker.com>"));
}

//the sender contains "SMS"
TEST_F(CUploadThread_fxt, BuildEmailSender_SmsSenderWithNumber_)
{
	m_config.set_installmode(2);
	m_config.set_faxdomain("faxm.net");
	m_config.set_smsdomain("smsm.net");
	m_config.set_serversender("sender@server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The SMS Sender"));
	//EXPECT_CALL(m_msg, ConstructEmail(_, _, StrEq(L"server@sender.net"), StrEq(L"123456"), StrEq(L"faxm.net"), _));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The SMS Sender\" <123456@smsm.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<123456@smsm.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_SmsSenderWithoutNumber_)
{
	m_config.set_installmode(2);
	m_config.set_faxdomain("faxm.net");
	m_config.set_smsdomain("smsm.net");
	m_config.set_serversender("sender@server.net");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The SMS Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(), sAddr, smtpCommands);

	ASSERT_STREQ(L"\"The SMS Sender\" <sms@smsm.net>", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<sms@smsm.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailSender_mailboxModeSmsSenderWithNoServerSenderNorMailboxConfigured_)
{
	m_config.set_installmode(3);
	m_config.set_faxdomain("faxm.net");
	m_config.set_serversender("");
	m_config.set_mailbox("");
	m_config.set_pop3("");
	ON_CALL(m_ffm, GetSender()).WillByDefault(Return(L"The SMS Sender"));
	strw_c sAddr(1024);
	str_c smtpCommands;

	m_ut.BuildEmailSender(&m_ffm, strw_c(L"123456"), sAddr, smtpCommands);

	ASSERT_STREQ(L"sms@smsmaker.com", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("MAIL FROM:<sms@smsmaker.com>"));
}

//==============================================================================
// CUploadThread::BuildEmailRecipients
//==============================================================================

TEST_F(CUploadThread_fxt, BuildEmailRecipients_twoRecipients_)
{
	EXPECT_CALL(m_ffm, RecipFindInit()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_ffm, RecipFindNext(_, _, _))
		.WillOnce(CopyWCStringToArg0AndReturnTRUE(L"email1@server.net"))
		.WillOnce(CopyWCStringToArg0AndReturnTRUE(L"email2@server.net"))
		.WillRepeatedly(Return(FALSE));
	
	strw_c sAddr(1024);
	str_c smtpCommands;
	m_ut.BuildEmailRecipients(&m_ffm, sAddr, smtpCommands);

	ASSERT_STREQ(L"email1@server.net,email2@server.net", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("RCPT TO:<email1@server.net>"));
	ASSERT_THAT(smtpCommands, HasSubstr("RCPT TO:<email2@server.net>"));
}

TEST_F(CUploadThread_fxt, BuildEmailRecipients_oneRecipient_)
{
	EXPECT_CALL(m_ffm, RecipFindInit()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_ffm, RecipFindNext(_, _, _))
		.WillOnce(CopyWCStringToArg0AndReturnTRUE(L"email@server.net"))
		.WillRepeatedly(Return(FALSE));
	
	strw_c sAddr(1024);
	str_c smtpCommands;
	m_ut.BuildEmailRecipients(&m_ffm, sAddr, smtpCommands);

	ASSERT_STREQ(L"email@server.net", sAddr);
	ASSERT_THAT(smtpCommands, HasSubstr("RCPT TO:<email@server.net>"));
}

//==============================================================================
// CUploadThread::BuildEmailMessage
//==============================================================================

TEST_F(CUploadThread_fxt, BuildEmailMessage_1of2AttachmentsHasSpecialGuid_AddItFromStagingDirAndDeleteIt)
{
	//input:
	ON_CALL(m_ffm, GetFilePath()).WillByDefault(Return("c:\\dir\\rep.sfm"));
	EXPECT_CALL(m_ffm, GetAttachmentPath(_)).WillRepeatedly(Return((LPCSTR)0));
	EXPECT_CALL(m_ffm, GetAttachmentPath(0)).WillRepeatedly(Return("c:\\dir\\att.txt.3534113FB4474CC48D9B51A3B7DA1717"));
	EXPECT_CALL(m_ffm, GetAttachmentPath(1)).WillRepeatedly(Return("c:\\dir\\att.txt"));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("c:\\dir\\bod.txt"));
	//output:
	EXPECT_CALL(m_msg, CreateFromFiles(
		HasSubstr("text/plain"),
		StrEq("c:\\dir\\bod.txt"),
		Pointee(AllOf(Contains("c:\\dir\\att.txt"), Contains("c:\\dir\\staging\\att.txt")))
	)).Times(1);
	//cleaning:
	EXPECT_CALL(GetSystemCallsMock(), DeleteFile(StrEq("c:\\dir\\staging\\att.txt"))).Times(1);
	m_ut.BuildEmailMessage(&m_ffm);
}

TEST_F(CUploadThread_fxt, BuildEmailMessage_htmlBody_usesHtmlContentType)
{
	//input:
	ON_CALL(m_ffm, GetFilePath()).WillByDefault(Return("c:\\dir\\rep.sfm"));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("c:\\dir\\bod.txt"));
	ON_CALL(m_ffm, GetFlags()).WillByDefault(Return(FAXMSG_F_BODYHTML));
	//output:
	EXPECT_CALL(m_msg, CreateFromFiles(
		HasSubstr("text/html"),
		StrEq("c:\\dir\\bod.txt"),
		Pointee(ElementsAre())
	)).Times(1);
	m_ut.BuildEmailMessage(&m_ffm);
}

TEST_F(CUploadThread_fxt, BuildEmailMessage_rtfBody_usesRichtextContentType)
{
	//input:
	ON_CALL(m_ffm, GetFilePath()).WillByDefault(Return("c:\\dir\\rep.sfm"));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("c:\\dir\\bod.txt"));
	ON_CALL(m_ffm, GetFlags()).WillByDefault(Return(FAXMSG_F_BODYRTF));
	//output:
	EXPECT_CALL(m_msg, CreateFromFiles(
		HasSubstr("text/richtext"),
		StrEq("c:\\dir\\bod.txt"),
		Pointee(ElementsAre())
	)).Times(1);
	m_ut.BuildEmailMessage(&m_ffm);
}

TEST_F(CUploadThread_fxt, BuildEmailMessage_cannotMakeDirectoryAnd1of2AttachmentsHasSpecialGuid_addItFromOriginalLocation)
{
	//input:
	ON_CALL(m_ffm, GetFilePath()).WillByDefault(Return("c:\\dir\\rep.sfm"));
	ON_CALL(GetSystemCallsMock(), CreateDirectory(_, _)).WillByDefault(SetLastErrorAndReturnFALSE(ERROR_CANNOT_MAKE));
	EXPECT_CALL(m_ffm, GetAttachmentPath(_)).WillRepeatedly(Return((LPCSTR)0));
	EXPECT_CALL(m_ffm, GetAttachmentPath(0)).WillRepeatedly(Return("c:\\dir\\att1.txt.3534113FB4474CC48D9B51A3B7DA1717"));
	EXPECT_CALL(m_ffm, GetAttachmentPath(1)).WillRepeatedly(Return("c:\\dir\\att2.txt"));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("c:\\dir\\bod.txt"));
	//output:
	EXPECT_CALL(m_msg, CreateFromFiles(
		HasSubstr("text/plain"),
		StrEq("c:\\dir\\bod.txt"),
		Pointee(AllOf(Contains("c:\\dir\\att1.txt.3534113FB4474CC48D9B51A3B7DA1717"), Contains("c:\\dir\\att2.txt")))
	)).Times(1);
	//cleaning:
	EXPECT_CALL(GetSystemCallsMock(), DeleteFile(_)).Times(0);
	m_ut.BuildEmailMessage(&m_ffm);
}

TEST_F(CUploadThread_fxt, BuildEmailMessage_fromAnAttachmentWithSpecialGuidCannotMoveFile_addItFromOriginalLocation)
{
	//input:
	ON_CALL(m_ffm, GetFilePath()).WillByDefault(Return("c:\\dir\\rep.sfm"));
	ON_CALL(GetSystemCallsMock(), MoveFileEx(_, _, _)).WillByDefault(SetLastErrorAndReturnFALSE(ERROR_SHARING_VIOLATION));
	EXPECT_CALL(m_ffm, GetAttachmentPath(_)).WillRepeatedly(Return((LPCSTR)0));
	EXPECT_CALL(m_ffm, GetAttachmentPath(0)).WillRepeatedly(Return("c:\\dir\\att1.txt.3534113FB4474CC48D9B51A3B7DA1717"));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("c:\\dir\\bod.txt"));
	//output:
	EXPECT_CALL(m_msg, CreateFromFiles(
		HasSubstr("text/plain"),
		StrEq("c:\\dir\\bod.txt"),
		Pointee(ElementsAre("c:\\dir\\att1.txt.3534113FB4474CC48D9B51A3B7DA1717"))
	)).Times(1);
	//cleaning:
	EXPECT_CALL(GetSystemCallsMock(), DeleteFile(_)).Times(0);
	m_ut.BuildEmailMessage(&m_ffm);
}

//==============================================================================
// CUploadThread::ConstructEmail
//==============================================================================

TEST_F(CUploadThread_fxt, ConstructEmail_aliasAddressAndDomainInp_)
{
	strw_c semail;
	str_c saddress;
	m_ut.ConstructEmail(semail, &saddress, L"alias", L"mailbox", L"domain.net");
	ASSERT_STREQ(L"\"alias\" <mailbox@domain.net>", semail);
	ASSERT_STREQ("mailbox@domain.net", saddress);
}

TEST_F(CUploadThread_fxt, ConstructEmail_aliasAddressWithAtSignAndDomainInp_filtersAtSign)
{
	strw_c semail;
	str_c saddress;
	m_ut.ConstructEmail(semail, &saddress, L"alias", L"strange@mailbox", L"domain.net");
	ASSERT_STREQ(L"\"alias\" <strangemailbox@domain.net>", semail);
	ASSERT_STREQ("strangemailbox@domain.net", saddress);
}

TEST_F(CUploadThread_fxt, ConstructEmail_aliasAndAddressInp_)
{
	strw_c semail;
	str_c saddress;
	m_ut.ConstructEmail(semail, &saddress, L"alias", L"mailbox@domain.net");
	ASSERT_STREQ(L"\"alias\" <mailbox@domain.net>", semail);
	ASSERT_STREQ("mailbox@domain.net", saddress);
}

TEST_F(CUploadThread_fxt, ConstructEmail_aliasWithSpecialCharsAndAddressInp_filtersSpecialChars)
{
	strw_c semail;
	m_ut.ConstructEmail(semail, NULL, L"\\strange<@>alias/", L"mailbox@domain.net");
	ASSERT_STREQ(L"\"strangealias\" <mailbox@domain.net>", semail);
}

TEST_F(CUploadThread_fxt, ConstructEmail_aliasWithCommasAndAddressInp_replacesCommasWithDots)
{
	strw_c semail;
	m_ut.ConstructEmail(semail, NULL, L"alias,with,commas", L"mailbox@domain.net");
	ASSERT_STREQ(L"\"alias.with.commas\" <mailbox@domain.net>", semail);
}

TEST_F(CUploadThread_fxt, ConstructEmail_addressInp_)
{
	strw_c semail;
	str_c saddress;
	m_ut.ConstructEmail(semail, &saddress, NULL, L"mailbox@domain.net");
	ASSERT_STREQ(L"mailbox@domain.net", semail);
	ASSERT_STREQ("mailbox@domain.net", saddress);
}

TEST_F(CUploadThread_fxt, ConstructEmail_addressWithSpecialCharsAndDomainNoFilterInp_)
{
	strw_c semail;
	str_c saddress;
	m_ut.ConstructEmail(semail, &saddress, L"alias", L"/strange/mailbox\\", L"domain.net", FALSE);
	ASSERT_STREQ(L"\"alias\" </strange/mailbox\\@domain.net>", semail);
	ASSERT_STREQ("/strange/mailbox\\@domain.net", saddress);
}


//==============================================================================
// CUploadThread::DoSmtpWork
//==============================================================================

//TEST_F(CUploadThreadMiddleLevTest, DoSmtpWork_customlLongSetFor2of5FaxFm_ProcessXMLAPIIsCalledTwice)
//{
//	ExpectDirFileListCallsSequence(5);
//	EXPECT_CALL(m_ffm, GetCustomLong()).Times(5)
//		.WillOnce(Return(1)).WillOnce(Return(1))
//		.WillRepeatedly(Return(0));
//	NiceMock<XMLAPI_Mock> xmlapi;
//	EXPECT_CALL(xmlapi, UT_ProcessXMLAPI(_)).Times(2);
//	EXPECT_CALL(m_ut, ProcessFaxFrmtMessage(_)).Times(3);
//	m_ut.DoSmtpWork(FALSE);
//}

TEST_F(CUploadThreadMiddleLevTest, DoSmtpWork_smtpProcessQueueReturnsZero_itIsNotCalledAgain)
{
	ExpectDirFileListCallsSequence(2);
	EXPECT_CALL(m_ut, ProcessFaxFrmtMessage(_)).Times(2);
	EXPECT_CALL(m_smtp, ProcessQueue(_)).Times(1);
	m_ut.DoSmtpWork(FALSE);
}

TEST_F(CUploadThreadMiddleLevTest, DoSmtpWork_smtpProcessQueueReturnsErrorAndMessageIndexIncreasedPartially_smtpSenderProcessQueueIsCalledAgain)
{
	ExpectDirFileListCallsSequence(2);
	EXPECT_CALL(m_ut, ProcessFaxFrmtMessage(_)).Times(2);
	EXPECT_CALL(m_smtp, ProcessQueue(_)).Times(2)
		.WillOnce(DoAll(IncreaseMessageIndexBy(&m_ut, 1),Return(161)))
		.WillOnce(Return(0));
	m_ut.DoSmtpWork(FALSE);
}


//==============================================================================
// CUploadThread::PreScan
//==============================================================================


TEST_F(CUploadThreadMiddleLevTest, PreScan_3of5FaxFMDoNotHaveRecipient_moveThemToUnprocDir)
{
	ExpectDirFileListCallsSequence(5);
	{
		InSequence s;
		EXPECT_CALL(m_ffm, RecipFindInit()).Times(3).WillRepeatedly(Return(FALSE));
		EXPECT_CALL(m_ffm, RecipFindInit()).WillOnce(Return(TRUE));
		EXPECT_CALL(m_ffm, RecipFindNext(_, _, _))
			.WillOnce(Return(TRUE));
			//.WillOnce(Return(FALSE));
		EXPECT_CALL(m_ffm, RecipFindInit()).WillOnce(Return(TRUE));
		EXPECT_CALL(m_ffm, RecipFindNext(_, _, _))
			.WillOnce(Return(TRUE));
			//.WillOnce(Return(FALSE));
	}

	EXPECT_CALL(GetGlobalFaxFmtMock(), faxfrmtMoveMessage(_, _)).Times(3);
	m_ut.PreScan();
}


#endif
