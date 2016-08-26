// textapi.Test.cpp : Defines the entry point for the console application.
//==============================================================================
#include "stdafx.h"
#include "textapi.Test.h"
//==============================================================================

CUtilities_Mock *g_Utils;

void SetUtils(CUtilities_Mock * l_Utils)
{
	g_Utils = l_Utils;
}

CUtilities * GetUtils()
{
	return g_Utils;
}

//==============================================================================
// TEST CODE RELATED WITH  Textapi_init() 
//==============================================================================
namespace F
{
	// Alread Loaded
	TEST_F(textapi_initfxt, textapi_init_alreadyInit_ReturnTrue)
	{
		setInit(TRUE);
		ASSERT_EQ(TRUE, textapi_init());
	}

	// Check if g_init is set true after init()
	TEST_F(textapi_initfxt, textapi_init_gsmLoadingSucceed_ReturnTrue)
	{
		setInit(FALSE);
		textapi_init();
		ASSERT_EQ(TRUE, GetInit());
	}

	//==============================================================================
	// TEST CODE RELATED WITH  textapi_uninit() 
	//==============================================================================

	// Check if g_init is set false after uninit()
	TEST(textapi_test, textapi_uninit_alreadyInit_ReturnTrue)
	{
		setInit(TRUE);
		NiceMock<GSMServicesInterface_Mock> gsmmock;

		textapi_uninit();
		ASSERT_EQ(FALSE, GetInit());
	}


	//==============================================================================
	// TEST CODE RELATED WITH textapiProcessFile(STR) 
	//==============================================================================

	//if open fails, return FM_ERROR
	TEST_F(textApi_ProcessFile_str, textapiProcessFile_ParamStr_OpenFails_ReturnFM_ERROR)
	{
		expect_ffm_Open(1, FALSE);
		ASSERT_EQ(FM_ERROR, textapiProcessFile("blabla"));
	}

	// if open succeed, a close is expected. 
	TEST_F(textApi_ProcessFile_str, textapiProcessFile_ParamStr_OpenSucceed_ReturnIrrelevant)
	{
		expect_ffm_Open(1, TRUE);
		expect_ffm_Close(1);
		expect_ffm_GetFlags(1, FAXMSG_F_PARSED);
		textapiProcessFile("blabla");
	}

	//==============================================================================
	// TEST CODE RELATED WITH textapiProcessFile(FFM) 
	//==============================================================================

	// State: Message already parsed. 
	TEST_F(textApi_ProcessFileTextFxtr, ProcessFileFFM_FAXMSG_F_PARSEDisSet_ReturnOK)
	{
		expect_ffm_GetFlags(1, FAXMSG_F_PARSED);
		ASSERT_EQ(FM_OK, textapiProcessFile(mffm));
	}

	// State:     Body is Rtf 
	TEST_F(textApi_ProcessFileTextFxtr, ProcessFileFFM_BodyRrfFlagSet_WithoutAttachements_returnOk)
	{
		expect_ffm_GetFlags(3, FAXMSG_F_BODYRTF);
		//1 and 2 asks for the FAXMSG_F_PARSED flag 
		//3. asks for the FAXMSG_F_BODYRTF flag

		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");

		ASSERT_EQ(FM_OK, textapiProcessFile(mffm));
	}

	// State:    Body File is Empty 
	TEST_F(textApi_ProcessFileTextFxtr, ProcessFileFFM_EmptyGetBodyFileBody_returnOk)
	{
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		expect_ffm_GetBodyFile(1, (LPCSTR)NULL);

		ASSERT_EQ(FM_OK, textapiProcessFile(mffm));
	}

	//State:   HTML body but there is no content
	TEST_F(textApi_ProcessFileTextFxtr, textapiProcessFileFMM_HtmlBodyFlagSet_returnOk)
	{
		//-- Building Env state	
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		LPCSTR str_bodyfile = "c:\\body.txt";
		expect_ffm_GetBodyFile(2, str_bodyfile);

		//-- Variations to test
		expect_ffm_GetFlags(6, FAXMSG_F_BODYHTML);
		expect_htmlslayer_gettextapicmd(1, 1);

		//-- Ending Test gracefully
		faxmessage_t fm;
		expect_ffm_GetFaxMessage(2, fm);
		memcpy(fm.body.filename, str_bodyfile, strlen(str_bodyfile));

		//-- Test
		ASSERT_EQ(FM_OK, textapiProcessFile(mffm));
	}

	HANDLE h1 = (HANDLE)1;
	HANDLE createfile1()
	{
		return h1;
	}
	HANDLE h2 = (HANDLE)2;
	HANDLE createfile2()
	{
		return h2;
	}

	//State:   HtmlBodyFlagSet not set 
	//         1st create file fails - return FM_FILEOPENFAILED  
	TEST_F(textApi_ProcessFileTextFxtr, textapiProcessFileFMM_HtmlBodyFlagNotSet_ReadFileFailed_returnFILEOPENFAILED)
	{
		//-- Building Env state	
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		LPCSTR str_bodyfile = "c:\\body.txt";
		expect_ffm_GetBodyFile(2, str_bodyfile);
		expect_ffm_GetFilePath(1, "");

		//-- Actual Test
		expect_CreateFileRead(1, INVALID_HANDLE_VALUE);
		ASSERT_EQ(FM_FILEOPENFAILED, textapiProcessFile(mffm));
	}

	//State:   HtmlBodyFlagSet not set 
	//         2nd create file fails - return FM_FILECREATEFAILED 
	TEST_F(textApi_ProcessFileTextFxtr, textapiProcessFileFMM_HtmlBodyFlagNotSet_CreateFileWriteFailed_returnFILECREATEFAILED)
	{
		//-- Building Env state
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		expect_ffm_GetBodyFile(2, "c:\\body.txt");
		expect_ffm_GetFilePath(1, "");

		//-- Actual Test
		HANDLE hd = (HANDLE)1;
		expect_CreateFileRead(1, hd);
		expect_CreateFileWrite(1, INVALID_HANDLE_VALUE);
		ASSERT_EQ(FM_FILECREATEFAILED, textapiProcessFile(mffm));
	}


	HRESULT Fake_CreateTextReaderFromFile_F(HANDLE hFile, CTextReader **ppReader)
	{
		return E_FAIL;
	}

	//State:   HtmlBodyFlagSet not set 
	//         GetCreateTextReaderFromFile fails 
	TEST_F(textApi_ProcessFileTextFxtr, textapiProcessFileFMM_CreateTextFileFailed_returnFILEOPENFAILED)
	{
		//-- Building Env state	
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		expect_ffm_GetBodyFile(2, "c:\\body.txt");
		expect_ffm_GetFilePath(1, "");
		HANDLE hd = (HANDLE)1;
		expect_CreateFileRead(1, hd);
		expect_CreateFileWrite(1, hd);

		//-- Actual Test
		CCreateTextReaderFromFile_Mock ctrf_m;
		SetCreateTextReaderFromFileObjMock(&ctrf_m);
		EXPECT_CALL(ctrf_m, CreateTextReaderFromFile(MatcherCast<HANDLE>(_), _)).WillOnce(Invoke(Fake_CreateTextReaderFromFile_F));
		ASSERT_EQ(FM_FILEOPENFAILED, textapiProcessFile(mffm));
	}


	CTextReader_Mock *g_trm;

	void SetTestTextReader(CTextReader_Mock *pl_trm)
	{
		g_trm = pl_trm;
	}

	HRESULT Fake_CreateTextReaderFromFile_OK(HANDLE hFile, CTextReader **ppReader)
	{
		*ppReader = g_trm;
		return S_OK;
	}


	//State:   HtmlBodyFlagSet not set 
	//         WriterInit fails 
	TEST_F(textApi_ProcessFileTextFxtr, textapiProcessFileFMM_WriterInitFailed_returnFILEOPENFAILED)
	{
		//-- Building Env state	
		expect_ffm_GetSender(2, L"sender-test@test.com");
		expect_ffm_GetSubject(L"subject_test");
		expect_ffm_GetBodyFile(2, "c:\\body.txt");
		expect_ffm_GetFilePath(1, "");
		HANDLE hd = (HANDLE)1;
		expect_CreateFileRead(1, hd);
		expect_CreateFileWrite(1, hd);
		CCreateTextReaderFromFile_Mock ctrf_m;
		SetCreateTextReaderFromFileObjMock(&ctrf_m);
		NiceMock<CTextWriter_Mock> tw_m;
		SetTextWriterMock(&tw_m);
		NiceMock<CTextReader_Mock> tr_m;
		SetTestTextReader(&tr_m);

		//-- Actual Test
		EXPECT_CALL(ctrf_m, CreateTextReaderFromFile(MatcherCast<HANDLE>(_), _)).WillOnce(testing::Invoke(Fake_CreateTextReaderFromFile_OK));
		expect_textwriter_init(1, E_FAIL);

		ASSERT_EQ(FM_FILEOPENFAILED, textapiProcessFile(mffm));
	}
}
//==============================================================================
// Test Fixture used for all the rest of the tests. 
//==============================================================================

class textApi_commonfxtre : public mfgfixture
{
	public:
	 virtual void SetUp()		 
	 {	
		 InitializeCriticalSection(GetCS());
     };
	 
	 virtual void TearDown()	 
	 {	
	 };

protected:
};

//==============================================================================
// TEST CODE RELATED WITH textapiIsTextAPITypeSender 
//==============================================================================
namespace F
{
	// Passed Empty Parameter
	TEST_F(textApi_commonfxtre, textapiIsTextAPITypeSender_empty_returnFalse)
	{
		ASSERT_EQ(FALSE, textapiIsTextAPITypeSender(NULL));
	}

	// Passed Bad Parameter 
	TEST_F(textApi_commonfxtre, textapiIsTextAPITypeSender_BadTypes_returnFalse)
	{
		ASSERT_EQ(FALSE, textapiIsTextAPITypeSender("bla"));
	}

	// Passed Good Parameters 
	TEST_F(textApi_commonfxtre, textapiIsTextAPITypeSender_GoodTypes_returnTrue)
	{
		ASSERT_EQ(TRUE, textapiIsTextAPITypeSender(TEXT_API_SENDER));
		ASSERT_EQ(TRUE, textapiIsTextAPITypeSender(HPF_API_SENDER));
		ASSERT_EQ(TRUE, textapiIsTextAPITypeSender(SAP_API_SENDER));
	}

	//==============================================================================
	// TEST CODE RELATED WITH textapiDoAttachBody 
	//==============================================================================
	// Passed Empty Parameter
	TEST_F(textApi_commonfxtre, textapiDoAttachBody_empty_returnFalse)
	{
		ASSERT_EQ(FALSE, textapiDoAttachBody(NULL));
	}

	// Passed Bad Parameter 
	TEST_F(textApi_commonfxtre, textapiDoAttachBody_GoodTypes_returnTrue)
	{
		ASSERT_EQ(TRUE, textapiDoAttachBody(TEXT_API_SENDER));
	}

	// Passed Good Parameters 
	TEST_F(textApi_commonfxtre, textapiDoAttachBody_BadTypes_returnFalse)
	{
		ASSERT_EQ(FALSE, textapiDoAttachBody(HPF_API_SENDER));
		ASSERT_EQ(FALSE, textapiDoAttachBody(SAP_API_SENDER));
	}

	//==============================================================================
	// TEST CODE RELATED WITH textapiGetSentReportUniqueFilePath 
	//==============================================================================

	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_EmptyPrms_returnFalse)
	{
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", NULL, "", "", 1, 1));
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", "", NULL, "", 1, 1));
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", "", "", NULL, 1, 1));
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", "", "", "", 0, 1));
	}


	//report not 1 or 2
	//	ASSERT_EQ (FALSE, textapiGetSentReportUniqueFilePath("","","",0,4));


	//TEXT_API_SENDER. Good report type requested. Returns true
	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_TextApi_ReportTypeOk_returnTrue)
	{
		char out[_MAX_PATH] = "";
		char in[_MAX_PATH] = "filename.txt";

		expect_changeextension(2);
		expect_adjustfilename(2, TRUE);
		ASSERT_EQ(TRUE, textapiGetSentReportUniqueFilePath("", TEXT_API_SENDER, in, out, _MAX_PATH, TEXTAPI_TYPE_OK));
		ASSERT_EQ(TRUE, textapiGetSentReportUniqueFilePath("", TEXT_API_SENDER, in, out, _MAX_PATH, TEXTAPI_TYPE_ERR));
	}

	//TEXT_API_SENDER. Bad report type requested. Returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_TextApi_BadReportType_returnTrue)
	{
		char out[_MAX_PATH] = "";
		char in[_MAX_PATH] = "filename.txt";
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", TEXT_API_SENDER, in, out, _MAX_PATH, 0));
	}

	//HPF_API_SENDER.  file does not  exists
	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_HpfApi_FileNotExist_returnTrue)
	{
		char out[_MAX_PATH] = "";
		char in[_MAX_PATH] = "filename.txt";
		expect_changeextension(1);
		expect_doesfileexist(1, FALSE);
		ASSERT_EQ(TRUE, textapiGetSentReportUniqueFilePath("", HPF_API_SENDER, in, out, _MAX_PATH, 0));
	}
	//HPF_API_SENDER.  file exists
	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_HpfApi_FileExist_returnFalse)
	{
		char out[_MAX_PATH] = "";
		char in[_MAX_PATH] = "filename.txt";
		expect_changeextension(1);
		expect_doesfileexist(1, TRUE);
		ASSERT_EQ(FALSE, textapiGetSentReportUniqueFilePath("", HPF_API_SENDER, in, out, _MAX_PATH, 0));
	}

	//SAP_API_SENDER.  file exists
	TEST_F(textApi_commonfxtre, textapiGetSentReportUniqueFilePath_Sap_returnTrue)
	{
		char out[_MAX_PATH] = "";
		char in[_MAX_PATH] = "filename.txt";
		ASSERT_EQ(TRUE, textapiGetSentReportUniqueFilePath("", SAP_API_SENDER, in, out, _MAX_PATH, 0));
	}

	//==============================================================================
	// TEST CODE RELATED WITH textapiGetSentReportBodyTemplate 
	//==============================================================================

	//Empty Params and TEXT_API_SENDER as input return FALSE
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_NullParam_returnFalse)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(NULL, out, 0, 0));
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(TEXT_API_SENDER, NULL, 0, 0));
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(NULL, NULL, 0, 0));
	}

	//TEXT_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_TextApi_returnFalse)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(TEXT_API_SENDER, out, 0, 0));
	}

	//HPF_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_HpfApi_GoodReportType_returnTrue)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(TRUE, textapiGetSentReportBodyTemplate(HPF_API_SENDER, out, _MAX_PATH, TEXTAPI_TYPE_OK));
		ASSERT_STREQ("data\\reports\\hpf_ok.fbt", out);
		ASSERT_EQ(TRUE, textapiGetSentReportBodyTemplate(HPF_API_SENDER, out, _MAX_PATH, TEXTAPI_TYPE_ERR));
		ASSERT_STREQ("data\\reports\\hpf_err.fbt", out);
	}

	//HPF_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_HpfApi_BadReportType_returnTrue)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(HPF_API_SENDER, out, _MAX_PATH, 0));
	}

	//SAP_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_SAPApi_GoodReportType_returnTrue)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(TRUE, textapiGetSentReportBodyTemplate(SAP_API_SENDER, out, _MAX_PATH, TEXTAPI_TYPE_OK));
		ASSERT_STREQ("data\\reports\\sap_ok.fbt", out);
		ASSERT_EQ(TRUE, textapiGetSentReportBodyTemplate(SAP_API_SENDER, out, _MAX_PATH, TEXTAPI_TYPE_ERR));
		ASSERT_STREQ("data\\reports\\sap_err.fbt", out);
	}

	//SAP_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiGetSentReportBodyTemplate_SAPApi_BadReportType_returnTrue)
	{
		char out[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiGetSentReportBodyTemplate(SAP_API_SENDER, out, _MAX_PATH, 0));
	}

	//==============================================================================
	// TEST CODE RELATED WITH textapiDoFinalReportWork 
	//==============================================================================

	//NULL Parameters
	TEST_F(textApi_commonfxtre, textapiDoFinalReportWork_NullParams_returnFalse)
	{
		char in[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiDoFinalReportWork(NULL, in, 1));
		ASSERT_EQ(FALSE, textapiDoFinalReportWork(TEXT_API_SENDER, NULL, 1));
	}

	//TEXT_API_SENDER, returns true
	TEST_F(textApi_commonfxtre, textapiDoFinalReportWork_TextApi_returnTrue)
	{
		char in[_MAX_PATH] = "";
		ASSERT_EQ(TRUE, textapiDoFinalReportWork(TEXT_API_SENDER, in, 1));
	}

	//HPF_API_SENDER, Create File Succeedreturns false
	TEST_F(textApi_commonfxtre, textapiDoFinalReportWork_HpfApi_CreateFileSucceed_returnTrue)
	{
		NiceMock<textfile_c_mock> tf_c_m;
		SetTextFileMock(&tf_c_m);

		expect_changeextension(1);
		expect_tf_create(1, TRUE);
		expect_tf_close(1);

		char in[_MAX_PATH] = "";
		ASSERT_EQ(TRUE, textapiDoFinalReportWork(HPF_API_SENDER, in, 1));
	}

	//HPF_API_SENDER. Create File fails, Returns false
	TEST_F(textApi_commonfxtre, textapiDoFinalReportWork_HpfApi_CreateFileFailed_returnTrue)
	{
		textfile_c_mock tf_c_m;
		SetTextFileMock(&tf_c_m);

		expect_changeextension(1);
		expect_tf_create(1, FALSE);

		char in[_MAX_PATH] = "";
		ASSERT_EQ(FALSE, textapiDoFinalReportWork(HPF_API_SENDER, in, 1));
	}

	//SAP_API_SENDER, returns false
	TEST_F(textApi_commonfxtre, textapiDoFinalReportWork_SapApi_returnTrue)
	{
		char in[_MAX_PATH] = "";
		ASSERT_EQ(TRUE, textapiDoFinalReportWork(SAP_API_SENDER, in, 1));
	}
}
//==============================================================================
// Main_Test
//==============================================================================

int _tmain(int argc, char ** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();

	return 0;
}
//End TextAPI.Test.cpp =========================================================