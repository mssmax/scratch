//==============================================================================
//XMLAPI Tests
//==============================================================================
#include "stdafx.h"
#ifdef PERFORM_XMLAPI_TESTS 
#include "xmlapi.Tests.h"

//==============================================================================
// XMLAPI::DT_ProcessXMLAPI()
//==============================================================================
    
TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_xmlfolderLengthZero_ReturnFalse)
{	
	m_pXmlapi->m_sXMLAPIFolder = "";
	ASSERT_EQ(FALSE, m_pXmlapi->DT_ProcessXMLAPI());	
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_ThreadIsEnding_Result1)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));		
	EXPECT_CALL(m_DT, IsEnding()).WillOnce(Return(TRUE));

//	m_pXmlapi->bIsEndingReturn = TRUE;
	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(1, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_CantGetFilePath_Result2)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(FALSE));
	
	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(2, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_ExceedXpathCreateFaxFails_Result3)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	m_pXmlapi->bExceedsReturn = TRUE;

	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(3, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_NotValidXMLFails_Result4)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));

	m_pXmlapi->bIsValidXMLReturn = FALSE;

	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(4, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_CreateFaxFails_Result5)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	NiceMock<CFaxFrmtMessage_Mock> ffmm;
	SetFaxFrmtMsgMock(&ffmm);
	EXPECT_CALL(ffmm, Create(_)).WillOnce(Return(FALSE));

	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(5, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_MoveFileFails_Result6)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	NiceMock<CFaxFrmtMessage_Mock> ffmm;
	SetFaxFrmtMsgMock(&ffmm);
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(FALSE));
	faxmessage_t fm;
	EXPECT_CALL(ffmm, GetFaxMessage()).WillOnce(Return(&fm));
	
	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(6, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_AddAttFails_Result7)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	NiceMock<CFaxFrmtMessage_Mock> ffmm;
	SetFaxFrmtMsgMock(&ffmm);
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(TRUE));
	faxmessage_t fm;
	EXPECT_CALL(ffmm, GetFaxMessage()).WillOnce(Return(&fm));
	EXPECT_CALL(ffmm, AddAttachment(_)).WillOnce(Return(-1));
	
	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(7, m_pXmlapi->m_dwXMLProcessResult);
}

TEST_F(xmlapi_fxt_dt, ProcessXMLAPI_MoveSfmFile_Result8)
{
	EXPECT_CALL(m_dfl, findfirst()).WillOnce(Return(TRUE));
	EXPECT_CALL(m_dfl, findnext()).WillOnce(Return(FALSE));
	EXPECT_CALL(m_dfl, getcurrentfilepath(_,_)).WillOnce(Return(TRUE));
	NiceMock<CFaxFrmtMessage_Mock> ffmm;
	SetFaxFrmtMsgMock(&ffmm);
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(TRUE));	
	faxmessage_t fm;
	EXPECT_CALL(ffmm, GetFaxMessage()).WillOnce(Return(&fm));
	EXPECT_CALL(ffmm, AddAttachment(_)).WillOnce(Return(1));

	m_pXmlapi->DT_ProcessXMLAPI();
	ASSERT_EQ(8, m_pXmlapi->m_dwXMLProcessResult);
}

//==============================================================================
// XMLAPI::IsValidXML()
//==============================================================================

TEST_F(xmlapi_fxt_dt_others, IsValidXML_CantLoad_Return_TRUE)
{	
//	m_pXmlapi->Set_pxmldom(&m_xmldom_mockp);

	EXPECT_CALL(m_pXmlapi->m_xmlDoc, put_async(_)).WillOnce(Return(S_OK));	
	EXPECT_CALL(m_pXmlapi->m_xmlDoc, raw_load(_,_)).WillOnce(Return(E_FAIL));	
	
	ASSERT_EQ(FALSE, m_pXmlapi->IsValidXML(""));
}

TEST_F(xmlapi_fxt_dt_others, IsValidXML_LoadSucceed_Return_TRUE)
{	
//	m_pXmlapi->Set_pxmldom(&m_xmldom_mockp);
	
	VARIANT_BOOL b = VARIANT_TRUE;
	EXPECT_CALL(m_pXmlapi->m_xmlDoc, raw_load(_,_)).WillOnce( DoAll(
			SetArgPointee<1>(b),	 
			Return(S_OK)
	));	

	ASSERT_EQ(TRUE, m_pXmlapi->IsValidXML(""));
}

TEST_F(xmlapi_fxt_dt_others, IsValidXML_LoadSucceedButReturnFalse_Return_TRUE)
{	
	//m_pXmlapi->Set_pxmldom(&m_xmldom_mockp);
	
	VARIANT_BOOL b = VARIANT_FALSE;
	EXPECT_CALL(m_pXmlapi->m_xmlDoc, raw_load(_,_)).WillOnce( DoAll(
			SetArgPointee<1>(b),	 
			Return(S_OK)
		));	

	MSXML2::IXMLDOMParseError * parseerror = NULL;
	EXPECT_CALL(m_pXmlapi->m_xmlDoc, get_parseError(_)).WillOnce( DoAll(
			SetArgPointee<0>(parseerror),
			Return(S_OK))
		);
	//alternative 	
	//NiceMock<IXMLDOMParseError_Mock> parseerror;
	//EXPECT_CALL(xmldom_mock, get_parseError(_)).WillOnce( DoAll(
	//		SetArgPointee<0>(&parseerror),
	//		Return(S_OK))
	//	);

	ASSERT_EQ(FALSE, m_pXmlapi->IsValidXML(""));
}

//==============================================================================
// XMLAPI::CreateXMLAPIReport()
//==============================================================================
#define STREQ(str)	MatcherCast<LPCSTR>(StrEq(str))

TEST_F(xmlapi_fxt_dt, CreateXMLAPIReport_sStatusFileIsCorrent_ExpectedCall)
{
	EXPECT_CALL(m_systemcalls_mock, CreateFile(STREQ("\\xmlapi\\test.xml.status"), _, _, _, _, _, _)).WillOnce(Return(INVALID_HANDLE_VALUE));
	m_pXmlapi->CreateXMLAPIReport("test",0,"");
}

//==============================================================================
// XMLAPI::ExceedsMaxPathLength()
//==============================================================================
 

TEST_F(xmlapi_fxt_dt_others, ExceedsMaxPathLength_Condition1Succeed_ReturnTrue)
{
	m_pXmlapi->m_sXMLAPIFolder = "c:\\123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789";
	ASSERT_EQ(TRUE, m_pXmlapi->ExceedsMaxPathLength(""));
}

TEST_F(xmlapi_fxt_dt_others, ExceedsMaxPathLength_Condition2Succeed_ReturnTrue)
{	
	ASSERT_EQ(TRUE, m_pXmlapi->ExceedsMaxPathLength("c:\\123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456789_123456\\file.xml"));
}

TEST_F(xmlapi_fxt_dt_others, ExceedsMaxPathLength_NoConditionSucceed_ReturnFalse)
{
	ASSERT_EQ(FALSE, m_pXmlapi->ExceedsMaxPathLength(""));
}

//==============================================================================
// XMLAPI::UT_ProcessXMLAPI
//==============================================================================

#define EXPECT_CREATE_AND_ADD_ELEMENT(parent, el) \
	NiceMock<IXMLDOMElement_Mock> el; \
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_createElement(BSTREq(L#el), _)).WillOnce(DoAll(SetArgPointee<1>(&el), testing::Return(S_OK))); \
	EXPECT_CALL(parent, raw_appendChild(Eq(&el), _)).WillOnce(DoAll(SetArgPointee<1>(&el), testing::Return(S_OK)))

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_xmlFileAlreadyExistsButCantBeParsed_nothingIsSaved)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_load(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_FALSE),	 
			Return(S_OK)
	));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_save(_)).Times(0);
	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}


MATCHER_P(VariantBSTREq, wcs, "") {
	return ((arg.vt & VT_TYPEMASK) == VT_BSTR) && (wcscmp(arg.bstrVal, wcs)==0);
}

MATCHER_P(BSTREq, wcs, "") {
	return (wcs==NULL || arg==NULL) ? wcs==arg : (wcscmp(arg, wcs)==0);
}

MATCHER_P(BSTRHasSubstr, wcs, "") {
	return (wcs==NULL || arg==NULL) ? false : (wcsstr(arg, wcs)!=NULL);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_xmlFileAlreadyExists_loadsItAndAddNodeWithAllMandatoryFields)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return(""));

	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_load(VariantBSTREq(L"\\xmlapi\\file.xml.status"),_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));
	
	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);

	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_save(VariantBSTREq(L"\\xmlapi\\file.xml.status"))).Times(1);
	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}

ACTION_P(ReturnFalseIfArg0Is, s)
{
	return (strcmp(s, arg0)!=0);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_xmlFileDoesNotExists_createsItAndAddNodeWithAllMandatoryFields)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return(""));

	EXPECT_CALL(GetHlpsMock(), doesfileexist(An<LPCSTR>())).WillRepeatedly(ReturnFalseIfArg0Is("\\xmlapi\\file.xml.status"));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_load(_,_)).Times(0);
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_loadXML(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));
	
	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);

	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_save(VariantBSTREq(L"\\xmlapi\\file.xml.status"))).Times(1);
	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_faxFMhasNotEmptyCustomString_usesItAsUidInTheXML)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return("someuid"));
	EXPECT_CALL(GetHlpsMock(), doesfileexist(An<LPCSTR>())).WillRepeatedly(ReturnFalseIfArg0Is("\\xmlapi\\file.xml.status"));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_loadXML(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));

	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);

	EXPECT_CREATE_AND_ADD_ELEMENT(fax, uid);
	EXPECT_CALL(uid, put_text(BSTREq(L"someuid"))).Times(1);

	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_faxFMhasNotEmptyFaxnumber_usesItAsRecipientInTheXML)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return(""));
	personalinfo_s pi;
	strcpy(pi.faxnumber, "123456");
	ON_CALL(m_ffm, GetSenderInfo()).WillByDefault(Return(&pi));
	EXPECT_CALL(GetHlpsMock(), doesfileexist(An<LPCSTR>())).WillRepeatedly(ReturnFalseIfArg0Is("\\xmlapi\\file.xml.status"));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_loadXML(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));

	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);

	EXPECT_CREATE_AND_ADD_ELEMENT(fax, recipient);
	EXPECT_CALL(recipient, put_text(BSTREq(L"123456"))).Times(1);

	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_faxFMhasAttachment_usesItAsFaxfileInTheXML)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return(""));
	ON_CALL(m_ffm, GetAttachmentPath(_)).WillByDefault(Return("faximage"));
	EXPECT_CALL(GetHlpsMock(), doesfileexist(An<LPCSTR>())).WillRepeatedly(ReturnFalseIfArg0Is("\\xmlapi\\file.xml.status"));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_loadXML(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));

	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);

	EXPECT_CREATE_AND_ADD_ELEMENT(fax, faxfile);
	EXPECT_CALL(faxfile, put_text(BSTRHasSubstr(L"faximage"))).Times(1);

	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}

TEST_F(xmlapi_fxt_utprocess, ProcessXMLAPI_attachmentCannotBeRead_xmlIsSavedWithEmptyDescription)
{
	faxmessage_t fm;
	strcpy(fm.customfile.filename, "file");
	ON_CALL(m_ffm, GetFaxMessage()).WillByDefault(Return(&fm));
	ON_CALL(m_ffm, GetCustomString()).WillByDefault(Return(""));
	EXPECT_CALL(m_ffm, GetBodyFile()).WillOnce(Return("bod.txt"));
	EXPECT_CALL(GetHlpsMock(), doesfileexist(An<LPCSTR>())).WillRepeatedly(ReturnFalseIfArg0Is("\\xmlapi\\file.xml.status"));
	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_loadXML(_,_)).WillOnce(DoAll(
			SetArgPointee<1>(VARIANT_TRUE),
			Return(S_OK)
	));

	EXPECT_CREATE_AND_ADD_ELEMENT(m_Xmlapi.m_xmlDocElement, fax);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, errorcode);
	EXPECT_CREATE_AND_ADD_ELEMENT(fax, description);
	EXPECT_CALL(description, put_text(BSTREq(L""))).Times(1);
	ON_CALL(GetCreateTextReaderFromFileObjMock(), CreateTextReaderFromFile(MatcherCast<HANDLE>(_), _)).WillByDefault(Return(E_FAIL));

	EXPECT_CALL(m_Xmlapi.m_xmlDoc, raw_save(VariantBSTREq(L"\\xmlapi\\file.xml.status"))).Times(1);

	m_Xmlapi.UT_ProcessXMLAPI(&m_ffm);
}


#endif

//END ==========================================================================