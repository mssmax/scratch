//==============================================================================
// Unit Testing for Class DownloadThread
//==============================================================================

#include "stdafx.h"
#ifdef PERFORM_DOWNLOADTHREAD_TESTS 
#include "DownloadThread.Tests.h"

//==============================================================================
// CDownloadThread::CheckIfMsgIsTNEF()
//==============================================================================
#include "mimelib.h"

extern BOOL bMultiPart;
extern LPCSTR contenttype1;
extern LPCSTR contenttype2;
extern int contenttype_index ;
extern LPCWSTR filename;
extern size_t childrenlength;


TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_1BodyPartWithMsTnef_BoolParamReturnTrue)
{
	LPCSTR l_contenttype = "application/ms-tnef";
	contenttype1 = l_contenttype;
	bMultiPart = FALSE; 
	
	CMimeMessage * msg = NULL;	BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(TRUE, res);
}

TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_1BodyPartWithOctetSrm_BoolParamReturnTrue)
{
	bMultiPart = FALSE; 
	LPCSTR l_contenttype = "application/octet-stream";
	contenttype2 = l_contenttype;
	LPCWSTR l_filename = L"winmail.dat";
	filename = l_filename;
	
	CMimeMessage * msg = NULL; BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(TRUE, res);
}

TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_MultiPartAndNot2Children_BoolParamReturnFALSE)
{
	bMultiPart = TRUE; 
	childrenlength =  3;
	
	CMimeMessage * msg = NULL; BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(FALSE, res);
}

TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_NoTextPlainContentn_BoolParamReturnFALSE)
{
	bMultiPart = TRUE; 
	childrenlength =  2;
	
	CMimeMessage * msg = NULL; BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(FALSE, res);
}

TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_NoApplicationMsTnefContent_BoolParamReturnFALSE)
{
	bMultiPart = TRUE; 
	childrenlength =  2;
	LPCSTR l_contenttype = "text/plain";
	contenttype1 = l_contenttype;

	CMimeMessage * msg = NULL; BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(FALSE, res);
}

TEST_F(downloadthread_fxt, CheckIfMsgIsTNEF_IsTNEF_BoolParamReturnTRUE)
{
	bMultiPart = TRUE; 
	childrenlength =  2;	//contenttype_index = 1;
	LPCSTR l_contenttype1 = "text/plain";
	contenttype1 = l_contenttype1;
	LPCSTR l_contenttype2 = "application/ms-tnef";
	contenttype2 = l_contenttype2;

	CMimeMessage * msg = NULL; BOOL res = FALSE;	
	m_dt->CheckIfMsgIsTNEF(msg, &res);
	ASSERT_EQ(TRUE, res);
}

//==============================================================================
// CDownloadThread::StartWork()
//==============================================================================
       
extern BOOL bInitMimeLib;

TEST_F(downloadthread_fxt, StartWork_bInitMimeLibFalse_ReturnFalse)
{
	bInitMimeLib = FALSE;
	ASSERT_EQ(FALSE,m_dt->StartWork());
}

TEST_F(downloadthread_fxt, StartWork_Tnef2MimeIsNull_ReturnFalse)
{
	bInitMimeLib = TRUE;
	ASSERT_EQ(FALSE,m_dt->StartWork());
}

TEST_F(downloadthread_fxt, StartWork_RegOpenFails_ReturnFalse)
{
	bInitMimeLib = TRUE;
	CTnef2MimeMock * ptnefmock = (CTnef2MimeMock *)1;
	SetTnef2MimeMock(ptnefmock);
	EXPECT_CALL(m_systemcalls_mock, RegOpenKeyEx(_,_,_,_,_)).WillOnce(Return(-1));
	ASSERT_EQ(TRUE,m_dt->StartWork());
}

TEST_F(downloadthread_fxt, StartWork_RegOQueryFails_ReturnFalse)
{
	bInitMimeLib = TRUE;
	CTnef2MimeMock * ptnefmock = (CTnef2MimeMock *)1;
	SetTnef2MimeMock(ptnefmock);
	EXPECT_CALL(m_systemcalls_mock, RegOpenKeyEx(_,_,_,_,_)).WillOnce(Return(ERROR_SUCCESS));
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,_,_,_,_,_)).WillOnce(Return(-1));
	ASSERT_EQ(TRUE,m_dt->StartWork());
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_EmptyFrom_ReturnFalse)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;

	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t bb = "";	
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(bb),Return(S_OK)));
	EXPECT_CALL(l_msg_mock, get_From(_)).WillOnce( DoAll(SetArgPointee<0>(bb),Return(S_OK)));
	
	BOOL bloop = FALSE;
	ASSERT_EQ(FALSE,m_dt_t->CheckForPossibleLoop("",bloop));
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_InvalidSender_ReturnFalse)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;

	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t sender = "dd.com";		
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(sender),Return(S_OK)));
	EXPECT_CALL(m_utils, ExtractEmail(_)).WillOnce(Return(L"dd.com"));
	
	BOOL bloop = FALSE;
	ASSERT_EQ(FALSE,m_dt_t->CheckForPossibleLoop("",bloop));
}
//==============================================================================
// CDownloadThread::CheckForPossibleLoop()
//==============================================================================

TEST_F(downloadthread_fxt, CheckForPossibleLoop_NoValidEmail_ReturnFalse)
{
	BOOL b;
	LPCSTR lpszfile = "";

	ASSERT_EQ(FALSE, m_dt->CheckForPossibleLoop(lpszfile,b));
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_InstallMode3_InvalidServerSender_ReturnFalse)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;
	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t sender = "d@d.com";		
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(sender),Return(S_OK)));
	EXPECT_CALL(m_utils, ExtractEmail(_)).WillRepeatedly(Return(L"dd.com"));

	m_mta.set_installmode(3);
	m_mta.set_serversender("");

	BOOL bloop = FALSE;
	ASSERT_EQ(FALSE,m_dt_t->CheckForPossibleLoop("",bloop));
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_SmsdomainsAsSender_ReturnParamTRUE)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;
	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t sender = "sender@sms.com";		
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(sender),Return(S_OK)));
	EXPECT_CALL(m_utils, ExtractEmail(_)).WillRepeatedly(Return(L"sender@sms.com"));

	m_mta.set_smsdomain("sms.com");	
	m_mta.set_faxdomain("fax.com");

	BOOL bloop = FALSE;
	ASSERT_EQ(TRUE,m_dt_t->CheckForPossibleLoop("",bloop));
	ASSERT_EQ(TRUE, bloop);
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_faxdomainsAsSender_ReturnParamTRUE)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;
	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t sender = "sender@fax.com";		
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(sender),Return(S_OK)));
	EXPECT_CALL(m_utils, ExtractEmail(_)).WillRepeatedly(Return(L"sender@fax.com"));

	m_mta.set_smsdomain("sms.com");	
	m_mta.set_faxdomain("fax.com");
	
	BOOL bloop = FALSE;
	ASSERT_EQ(TRUE,m_dt_t->CheckForPossibleLoop("",bloop));
	ASSERT_EQ(TRUE, bloop);
}

TEST_F(downloadthread_fxt_com, CheckForPossibleLoop_NoLoop_ReturnTRUE)
{
	NiceMock<IMessage_Mock> l_msg_mock;
	m_dt_t->SetMsgMock(&l_msg_mock);
	NiceMock<_Stream_Mock> l_stream_mock;
	EXPECT_CALL(l_msg_mock, raw_GetStream(_)).WillOnce( DoAll(SetArgPointee<0>(&l_stream_mock),	Return(S_OK)));
	_bstr_t sender = "sender@dor.com";		
	EXPECT_CALL(l_msg_mock, get_Sender(_)).WillOnce( DoAll(SetArgPointee<0>(sender),Return(S_OK)));
	EXPECT_CALL(m_utils, ExtractEmail(_)).WillRepeatedly(Return(L"sender@dor.com"));

	m_mta.set_smsdomain("sms.com");	
	m_mta.set_faxdomain("fax.com");	
	
	BOOL bloop = FALSE;
	ASSERT_EQ(TRUE,m_dt_t->CheckForPossibleLoop("",bloop));
	//ASSERT_EQ(FALSE, bloop);
}

//==============================================================================
// CDownloadThread::ProcessEmailsFromE2K7ForeignConnector()
//==============================================================================

TEST_F(dthread_fxt_processemail, ProcessEmail_ThreadIsEnding_Result1)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	
	m_dt->bIsEndingReturn = TRUE;
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(1, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_CantGetFilePath_Result2)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(FALSE));
	
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(2, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_fileStreamFails_Result2)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(FALSE));
	
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(2, m_dt->m_dwPEFromFcResult);	
}

extern HRESULT hrCreate;
extern HRESULT hrLoad;

TEST_F(dthread_fxt_processemail, ProcessEmail_MsgCreationFailed_Result3)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));

	hrCreate = -1;
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(3, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_LoadMimeFailed_Ressult4)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));	
	
	hrCreate = S_OK;
	hrLoad = -1;
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(4, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_CheckIfTnefFailed_Ressult5)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = S_OK;

	m_dt->bCheckIfMsgIsTNEFReturn = -1;
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(5, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_CreateTempFileFailed_Ressult6)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	//NiceMock<CStreamOnFileWithOffset_Mock> sof(0);
	//SetStreamOnFileWithOffsetMock(&sof);
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;	

	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return(INVALID_HANDLE_VALUE));
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(6, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_fileStreamFileSeekFailed_Ressult7)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;	
	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return((HANDLE)1));

	EXPECT_CALL(sof, Seek(_,_,_)).WillOnce(Return(-1));		
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(7, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_ConvertTNEFFailed_Ressult8)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;	
	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return((HANDLE)1));
	
	m_dt->bIsTnef = TRUE;
	CTnef2MimeMock ptnefmock; 
	SetTnef2MimeMock(&ptnefmock);
	m_dt->SetConvertor();
	EXPECT_CALL(ptnefmock, Convert(_,_)).WillOnce(Return(-1));
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(8, m_dt->m_dwPEFromFcResult);
}


TEST_F(dthread_fxt_processemail, ProcessEmail_NotTnef_FileStreamStatFailed_Result9)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));	
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;		
	m_dt->bIsTnef = FALSE;	
	
	EXPECT_CALL(sof, Stat(_,_)).WillOnce(Return(-1));	
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(9, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_FileStreamCopyToFailed_Result10)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;	
	m_dt->bIsTnef = FALSE;	
	STATSTG stg = {0};	
	EXPECT_CALL(sof, Stat(_,_)).WillOnce(DoAll(SetArgPointee<0>(stg),Return(S_OK)));

	EXPECT_CALL(sof, CopyTo(_,_,_,_)).WillOnce(Return(-1));	
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(10, m_dt->m_dwPEFromFcResult);
}

TEST_F(dthread_fxt_processemail, ProcessEmail_MoveFileToFailed_Result11)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	EXPECT_CALL(sof, InitA(_,_,_,_,_)).WillOnce(Return(S_OK));
	hrCreate = hrLoad = m_dt->bCheckIfMsgIsTNEFReturn = S_OK;	
	m_dt->bIsTnef = FALSE;	
	STATSTG stg = {0};	
	EXPECT_CALL(sof, Stat(_,_)).WillOnce(DoAll(SetArgPointee<0>(stg),Return(S_OK)));

	EXPECT_CALL(m_systemcalls_mock, MoveFile(_,_)).WillOnce(Return(FALSE));	
	m_dt->ProcessEmailsFromE2K7ForeignConnector();
	ASSERT_EQ(11, m_dt->m_dwPEFromFcResult);
}




//==============================================================================
// CDownloadThread::do_cdo_processmails()
//==============================================================================

TEST_F(dthread_fxt_processemail, CdoProcessEmail_NoRSxFiles_Return1)
{
	EXPECT_CALL(m_dfl, isempty()).WillOnce(Return(TRUE));
	m_dt->do_cdo_processmails();	
	ASSERT_EQ(1, m_dt->m_dwCdoProcessEmails);
}

TEST_F(dthread_fxt_processemail, CdoProcessEmail_ThreadIsEnding_Return2)
{
	EXPECT_CALL(m_dfl, isempty()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));	
	m_dt->bIsEndingReturn = TRUE;

	m_dt->do_cdo_processmails();	
	ASSERT_EQ(2, m_dt->m_dwCdoProcessEmails);
}

#endif
//End DownloadThread.Tests.cpp =================================================