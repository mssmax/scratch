//==============================================================================
// Unit Testing for CRSxMessage
//==============================================================================
#include "stdafx.h"

#ifdef PERFORM_RSXMESSAGE_TESTS
#include "rsxmessage.tests.h"

//==============================================================================
// CRSxMessage::GetRecipientType
//==============================================================================

TEST_F(rsxmessage_fxt, GetRecipientType_NoSeparator_ReturnPITYPE_FAX)
{
	bool b;
	m_mta.set_lotusnotes(NULL);
	
	ASSERT_EQ(PITYPE_FAX,m_rsx->GetRecipientType(L"",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_NotLotus_AddressContainSms_ReturnPITYPE_SMS)
{
	bool b;
	m_mta.set_lotusnotes(NULL);
	m_mta.set_smsdomain("@...");
	m_mta.set_faxdomain("@...");
	
	ASSERT_EQ(PITYPE_SMS,m_rsx->GetRecipientType(L"@sms",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_NotLotus__AddressContainFax_ReturnPITYPE_FAX)
{
	bool b;
	m_mta.set_lotusnotes(NULL);
	m_mta.set_smsdomain("@...");
	m_mta.set_faxdomain("@...");

	ASSERT_EQ(PITYPE_FAX,m_rsx->GetRecipientType(L"@fax",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_NotLotus_AddressEqualsSMSDomainName_ReturnPITYPE_SMS)
{
	bool b;
	m_mta.set_lotusnotes(NULL);
	m_mta.set_smsdomain("@sms");	
	m_mta.set_faxdomain("@...");
	
	ASSERT_EQ(PITYPE_SMS,m_rsx->GetRecipientType(L"@sms",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_NotLotus_AddressEqualsFAXDomainName_ReturnPITYPE_FAX)
{
	bool b;
	m_mta.set_lotusnotes(NULL);	
	m_mta.set_faxdomain("@fax");	
	m_mta.set_smsdomain("@...");
	
	ASSERT_EQ(PITYPE_FAX,m_rsx->GetRecipientType(L"@fax%",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_Lotus_AddressContainSms_ReturnPITYPE_SMS)
{
	bool b;
	m_mta.set_lotusnotes(1);
	m_mta.set_smsdomain("%...");
	m_mta.set_faxdomain("%...");
	
	ASSERT_EQ(PITYPE_SMS,m_rsx->GetRecipientType(L"%sms",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_Lotus__AddressContainFax_ReturnPITYPE_FAX)
{
	bool b;
	m_mta.set_lotusnotes(1);
	m_mta.set_smsdomain("%...");
	m_mta.set_faxdomain("%...");

	ASSERT_EQ(PITYPE_FAX,m_rsx->GetRecipientType(L"%fax",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_Lotus_AddressEqualsSMSDomainName_ReturnPITYPE_SMS)
{
	bool b;
	m_mta.set_lotusnotes(1);
	m_mta.set_smsdomain("%sms");	
	m_mta.set_faxdomain("%...");
	
	ASSERT_EQ(PITYPE_SMS,m_rsx->GetRecipientType(L"%sms",b));
}

TEST_F(rsxmessage_fxt, GetRecipientType_Lotus_AddressEqualsFAXDomainName_ReturnPITYPE_FAX)
{
	bool b;
	m_mta.set_lotusnotes(1);	
	m_mta.set_faxdomain("%fax");	
	m_mta.set_smsdomain("%...");
	
	ASSERT_EQ(PITYPE_FAX,m_rsx->GetRecipientType(L"%fax%",b));
}

//==============================================================================
// CRSxMessage::ExtractAlias()
//==============================================================================

TEST_F(rsxmessage_fxt, ExtractAlias_NotSeparator_ReturnAsInput)
{
	strw_c strd = L"<dd>";
	m_mta.set_lotusnotes(NULL);
	ASSERT_STREQ(strd,m_rsx->ExtractAlias(strd, TRUE).c_str());
}

TEST_F(rsxmessage_fxt, ExtractAlias_NotLotus_ReturnCorrectAlias)
{
	strw_c strd = L"<dd@testdomain.com>   ";
	m_mta.set_lotusnotes(NULL);
	ASSERT_STREQ(L"dd",m_rsx->ExtractAlias(strd, TRUE).c_str());
}

TEST_F(rsxmessage_fxt, ExtractAlias_Lotus_ReturnCorrectAlias)
{
	strw_c strd = L"<dd%testdomain.com>   ";
	m_mta.set_lotusnotes(1);
	ASSERT_STREQ(L"dd",m_rsx->ExtractAlias(strd, TRUE).c_str());
}

//==============================================================================
// CRSxMessage::convertfile2UTF8()
//==============================================================================

TEST_F(rsxmessage_fxt, convertfile2UTF8_AdjustFilenameFailed_ReturnFalse)
{	
	EXPECT_CALL(m_hlps_mock,adjustfilename(_,_,_)).WillRepeatedly(Return(FALSE));
	ASSERT_EQ(FALSE, m_rsx->convertfile2UTF8("",""));
}

TEST_F(rsxmessage_fxt, convertfile2UTF8_AdjustFilenameSucceed_ReturnTrue)
{	
	EXPECT_CALL(m_hlps_mock,adjustfilename(_,_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_systemcalls_mock, RegOpenKeyEx(_,_,_,_,_)).WillRepeatedly(Return(ERROR_SUCCESS));
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,_,_,_,_,_)).WillRepeatedly(Return(ERROR_SUCCESS));
	ASSERT_EQ(TRUE, m_rsx->convertfile2UTF8("",""));
}

//==============================================================================
// CRSxMessage::ProcessAlias()
//==============================================================================

TEST_F(rsxmessage_fxt, ProcessAlias_NoFaxNumber_ReturnFalse)
{
	LPCWSTR lpszAlias = L"abc;abc";
	personalinfo_t pinfo;
	m_mta.set_fieldseparators(";");

	ASSERT_EQ(FALSE, m_rsx->ProcessAlias(lpszAlias,pinfo));
}

//TEST_F(rsxmessage_fxt, ProcessAlias_WithFaxNumber_ReturnTrue)
//{
//	LPCWSTR lpszAlias = L"abc ; 0035621123452";
//	personalinfo_t pinfo;
// 	m_mta.set_fieldseparators(";");	                   
//	EXPECT_CALL(m_utils, IsFaxNumber(_)).WillRepeatedly(Return(TRUE));
//	ASSERT_EQ(TRUE, m_rsx->ProcessAlias(lpszAlias,pinfo));
//	ASSERT_STREQ("0035621123452", pinfo.faxnumber);
//}

//==============================================================================
// CRSxMessage::checkandrepairHTMLbodyfile()
//==============================================================================

TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_noHtmlExtraChecksIsSet_ReturnTrue)
{
	LPCSTR lpszpath = "";

	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(TRUE),	 
			Return(TRUE)
	));			
	ASSERT_EQ(TRUE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}


TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_TextExtractToMemReturnOk_ReturnTrue)
{
	LPCSTR lpszpath = "";

	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));			

	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);

	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,_)).WillOnce(Return(HSR_OK)); 

	ASSERT_EQ(TRUE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}

TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_2ndTextExtractToMemReturnNotOk_ReturnFalse)
{
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));			
	
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);

	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,_	)).WillRepeatedly(Return(HSR_NOTHTML)); 

	ASSERT_EQ(FALSE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}


TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_CantCreateFileName_ReturnFalse)
{
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));				
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS	)).WillRepeatedly(Return(HSR_NOTHTML)); 
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS|HSF_FORCEPARSE	)).WillRepeatedly(Return(HSR_OK)); 

	//-- Actual Test
	EXPECT_CALL(m_hlps_mock, adjustfilename(_,_,_)).WillOnce(Return(FALSE));
	ASSERT_EQ(FALSE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}


TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_TextReaderFromFileFails_ReturnFalse)
{
	//-- Building Env state
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));				
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS	)).WillRepeatedly(Return(HSR_NOTHTML)); 
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS|HSF_FORCEPARSE	)).WillRepeatedly(Return(HSR_OK)); 
	EXPECT_CALL(m_hlps_mock, adjustfilename(_,_,_)).WillOnce(Return(TRUE));

	//-- Actual Test
	CCreateTextReaderFromFile_Mock ctrf_m;
	SetCreateTextReaderFromFileObjMock(&ctrf_m);	
	EXPECT_CALL(ctrf_m,CreateTextReaderFromFile(MatcherCast<LPCSTR>(_),_)).WillOnce(Return(-1));
	ASSERT_EQ(FALSE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}

TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_CreateFileFails_ReturnFalse)
{
	//-- Building Env state
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));				
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS	)).WillRepeatedly(Return(HSR_NOTHTML)); 
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS|HSF_FORCEPARSE	)).WillRepeatedly(Return(HSR_OK)); 
	EXPECT_CALL(m_hlps_mock, adjustfilename(_,_,_)).WillOnce(Return(TRUE));
	CCreateTextReaderFromFile_Mock ctrf_m;
	SetCreateTextReaderFromFileObjMock(&ctrf_m);	

	//-- Actual Test
	NiceMock<CTextReader_Mock> m_tr_mock;
	EXPECT_CALL(ctrf_m,CreateTextReaderFromFile(MatcherCast<LPCSTR>(_),_))
		.WillOnce(DoAll(
			SetArgPointee<1>(&m_tr_mock)	,
			Return(S_OK) ));
	
	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return(INVALID_HANDLE_VALUE));
	ASSERT_EQ(FALSE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}

TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_TextWriterInitFails_ReturnFalse)
{
	//-- Building Env state
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	 
			Return(TRUE)
	));				
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS	)).WillRepeatedly(Return(HSR_NOTHTML)); 
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS|HSF_FORCEPARSE	)).WillRepeatedly(Return(HSR_OK)); 
	EXPECT_CALL(m_hlps_mock, adjustfilename(_,_,_)).WillOnce(Return(TRUE));
	CCreateTextReaderFromFile_Mock ctrf_m;
	SetCreateTextReaderFromFileObjMock(&ctrf_m);	
	NiceMock<CTextReader_Mock> m_tr_mock;
	EXPECT_CALL(ctrf_m,CreateTextReaderFromFile(MatcherCast<LPCSTR>(_),_))
		.WillOnce(DoAll(
			SetArgPointee<1>(&m_tr_mock)	,
			Return(S_OK) ));
	
	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return(HANDLE(1)));
	
	//-- Actual Test
	CTextWriter_Mock m_tw_mock;
	SetTextWriterMock(&m_tw_mock);
	EXPECT_CALL(m_tw_mock, Init(_,_)).WillOnce(Return(E_UNEXPECTED));
	ASSERT_EQ(FALSE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}

TEST_F(rsxmessage_fxt, checkandrepairHTMLbodyfile_Succeed_ReturnTrue)
{
	//-- Building Env state
	LPCSTR lpszpath = "";
	EXPECT_CALL(m_reg_mock, connect(STREQ("SOFTWARE\\GFI FAX & VOICE\\fmserver_advanced"),_,_)).WillRepeatedly(Return(TRUE));
	EXPECT_CALL(m_reg_mock, getlong(STREQ("no_html_extra_checks"), _))
		.WillOnce( DoAll(
			SetArgPointee<1>(2),	
			Return(TRUE)
	));				
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	SetHtmlSlayerMock(&m_hs_mock);
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS	)).WillRepeatedly(Return(HSR_NOTHTML)); 
	EXPECT_CALL(m_hs_mock, htmlslayer_textextract_tomemory_(_,_,_,	HSF_NOEXTRACTTEXT|HSF_EXTRACTNOFORMATTING|
		HSF_EXTRACTTAGS|HSF_FORCEPARSE	)).WillRepeatedly(Return(HSR_OK)); 
	EXPECT_CALL(m_hlps_mock, adjustfilename(_,_,_)).WillOnce(Return(TRUE));
	CCreateTextReaderFromFile_Mock ctrf_m;
	SetCreateTextReaderFromFileObjMock(&ctrf_m);	
	NiceMock<CTextReader_Mock> m_tr_mock;
	EXPECT_CALL(ctrf_m,CreateTextReaderFromFile(MatcherCast<LPCSTR>(_),_))
		.WillOnce(DoAll(
			SetArgPointee<1>(&m_tr_mock)	,
			Return(S_OK) ));
	
	EXPECT_CALL(m_systemcalls_mock,CreateFile(_,_,_,_,_,_,_)).WillOnce(Return(HANDLE(1)));
	NiceMock<CTextWriter_Mock> m_tw_mock;
	SetTextWriterMock(&m_tw_mock);
	
	//-- Actual Test
	EXPECT_CALL(m_tw_mock, Init(_,_)).WillOnce(Return(S_OK));
	ASSERT_EQ(TRUE, m_rsx->checkandrepairHTMLbodyfile(lpszpath));
}

//==============================================================================
// CRSxMessage::ExtractFMRecipient
//==============================================================================

TEST_F(rsxmessage_fxt, ExtractFMRecipient_EmailDoesntStartWithIMCEA_ReturnFalse)
{
	ASSERT_STREQ("notImcea@email.com", m_rsx->ExtractFMRecipient("notImcea@email.com").c_str());
}

TEST_F(rsxmessage_fxt, ExtractFMRecipient_NoMinusInEmail_ReturnEmpty)
{
	ASSERT_STREQ("", m_rsx->ExtractFMRecipient("IMCEA_").c_str());
}

TEST_F(rsxmessage_fxt, ExtractFMRecipient_NoAt_ReturnEmpty)
{
	ASSERT_STREQ("", m_rsx->ExtractFMRecipient("IMCEAaa-ab").c_str());
}

TEST_F(rsxmessage_fxt, ExtractFMRecipient_AtBeforeMinus_ReturnEmpty)
{
	ASSERT_STREQ("", m_rsx->ExtractFMRecipient("IMCEA@-").c_str());
}

TEST_F(rsxmessage_fxt, ExtractFMRecipient_GoodEmail_Return)
{
	m_mta.set_fieldseparators(";");
	ASSERT_STREQ("12345@FAX", m_rsx->ExtractFMRecipient("IMCEAFAX-12345@domain.com").c_str());
}

//==============================================================================
// CRSxMessage::do_cdo_parse_getemailaddress
//==============================================================================
TEST_F(rsxmessage_fxt, Getemailaddress_BadEmailInput_ReturnTrue)
{
	LPCWSTR lpszaddresses = L"sdfsd@email.com;dfs@email2.com"; 
	long index = 1; 
	strw_c saddress = "";
	ASSERT_EQ(FALSE, m_rsx->do_cdo_parse_getemailaddress(lpszaddresses, index, saddress));
}

TEST_F(rsxmessage_fxt, Getemailaddress_GoodEmailInput_ReturnTrue)
{
	LPCWSTR lpszaddresses = L"sdfsd@email.com,dfs@email2.com"; 
	long index = 1; 
	strw_c saddress = "";
	ASSERT_EQ(TRUE, m_rsx->do_cdo_parse_getemailaddress(lpszaddresses, index, saddress));
}

TEST_F(rsxmessage_fxt, Getemailaddress_GoodEmailInput_ReturnProperEmail)
{
	LPCWSTR lpszaddresses = L"abc1@email.com,abc2@email.com"; 
	long index = 1; 
	strw_c saddress = "";
	m_rsx->do_cdo_parse_getemailaddress(lpszaddresses, index, saddress);
	ASSERT_STREQ(L"abc2@email.com", saddress.getbuffer(0));
}

//==============================================================================
// CRSxMessage::ToFieldHasErrors
//==============================================================================
TEST_F(rsxmessage_fxt, ToFieldHasErrors_HasErrors_RetTrue)
{
	m_rsx->m_sToField = L"1@faxmaker.com, errors@";
	ASSERT_EQ(TRUE, m_rsx->ToFieldHasErrors());	
}

TEST_F(rsxmessage_fxt, ToFieldHasErrors_NoErrors_RetFalse)
{
	m_rsx->m_sToField = L"1@faxmaker.com";
	m_rsx->ToFieldHasErrors();	
}


//==============================================================================
// CRSxMessage::RenameSF_Files
//==============================================================================
TEST_F(rsxmessage_fxt, RenameSF_FaxRec_RenameFailed_RetFalse)
{
	m_rsx->m_bOneFaxRecip = TRUE;
	m_rsx->m_bOneSmsRecip = FALSE;
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(FALSE));
	ASSERT_EQ(FALSE, m_rsx->RenameSF_Files());
}

TEST_F(rsxmessage_fxt, RenameSF_SmsRec_RenameFailed_RetFalse)
{
	m_rsx->m_bOneFaxRecip = FALSE;
	m_rsx->m_bOneSmsRecip = TRUE;
	EXPECT_CALL(m_systemcalls_mock, MoveFileEx(_,_,_)).WillOnce(Return(FALSE));
	ASSERT_EQ(FALSE, m_rsx->RenameSF_Files());	
}
//==============================================================================
#endif //PERFORM_RSXMESSAGE_TESTS