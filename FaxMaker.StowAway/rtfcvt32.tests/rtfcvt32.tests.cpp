#include "stdafx.h"

#include "RichMain.h"
#include <SystemCalls.h>
#include "MfcHelper.Mock.h"
#include "Rtf2FaxApp.h"
#include "mfg.h"
#include "MainFrm.h"

#include "RtfOleDoc.h"
#include "CntrItem.h"
#include "RtfOleView.h"
using ::testing::Test;
using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;
using ::testing::AtLeast;
using ::testing::NiceMock;
#pragma comment (lib, "tiff32.lib")


int AFXAPI AfxWinMain(CWinApp *pApp, HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPTSTR lpCmdLine, int nCmdShow);


class FakeRtf2FaxApp : public CRtf2FaxApp
{
	DECLARE_DYNCREATE(FakeRtf2FaxApp)
public:
	MfcHelperMock m_mfcHelper;
	NiceMock<TiffFileMock> m_mockTiffFile;
	HANDLE m_hInitEvent;

	FakeRtf2FaxApp() : CRtf2FaxApp(&m_mfcHelper, &m_mockTiffFile)
	{
		m_hThread=0;
		m_hInitEvent=::CreateEvent(NULL, FALSE, FALSE, NULL);
	}

	virtual ~FakeRtf2FaxApp() {}
	
	virtual BOOL InitInstance() {
		BOOL retVal=CRtf2FaxApp::InitInstance();
		::SetEvent(m_hInitEvent);
		return retVal; 
	}
	
	virtual void LoadConfig()
	{
		m_RichMain.SetDefaultConfig();
	}


	//added for test purpose
	bool RichEditCtrlTextContains(LPCTSTR text)
	{

		FINDTEXTEX ft;
		ft.chrg.cpMin = 0;
		ft.chrg.cpMax = -1;
		ft.lpstrText = text;
		return m_RichEditView->GetRichEditCtrl().FindText(FR_DOWN, &ft)>=0;
	}
};

IMPLEMENT_DYNCREATE(FakeRtf2FaxApp, CWinApp)

class Rtf2FaxApp_fixture : public mfgfixture
{

protected:

	bool CreateTestFile(LPCTSTR lpFileName, const char *pBuffer, DWORD bufferSize=0) {
		HANDLE hFile=GetSystemCallsMock().CreateFile(lpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		DWORD nofb;
		if (!GetSystemCallsMock().WriteFile(hFile, pBuffer, bufferSize>0 ? bufferSize : strlen(pBuffer), &nofb, NULL)) return false;
		//ASSERT_EQ(bufferSize, nofb);
		return GetSystemCallsMock().CloseHandle(hFile);
	}

	Rtf2FaxApp_fixture() {
	}

	virtual ~Rtf2FaxApp_fixture() {
	}

	virtual void SetUp() {

		FakeRtf2FaxApp *fakeMade =(FakeRtf2FaxApp *)AfxBeginThread(RUNTIME_CLASS(FakeRtf2FaxApp));// AfxGetApp();
		ASSERT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(GetObj()->m_hInitEvent,15000));
	}

	FakeRtf2FaxApp *GetObj() { return (FakeRtf2FaxApp *)AfxGetApp(); }

	virtual void TearDown() {
		CWinThread *th=AfxGetThread();
		CWinApp *wa=AfxGetApp();
		AfxGetThread()->PostThreadMessage(WM_QUIT,0,0);
		// wait until thread terminates
		ASSERT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(AfxGetThread()->m_hThread,10000));
	}
};

class Rtf2FaxAppCustom_fixture : public Rtf2FaxApp_fixture
{
protected:
	coverpage_template_t m_cpt;

	virtual void SetUp()
	{
		Rtf2FaxApp_fixture::SetUp();
		//one page long doc 
		ASSERT_TRUE(CreateTestFile(
			_T("test.rtf"),
			"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Calibri;}} {\\*\\generator Msftedit 5.41.21.2510;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang9\\fs22 Test\\par \\b\\i Test\\b0\\i0\\f1\\par }"
		));
		//cover with custom field
		ASSERT_TRUE(CreateTestFile(
			_T("body.rtf"),
			"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Calibri;}} {\\*\\generator Msftedit 5.41.21.2510;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang9\\fs22 Title\\par\\f1[[<custom_field>]]\\par }"
		));
	}
};

class Rtf2FaxAppTemplates_fixture : public Rtf2FaxApp_fixture
{
protected:
	coverpage_template_t m_cpt;

	virtual void SetUp()
	{
		Rtf2FaxApp_fixture::SetUp();
		ASSERT_TRUE(CreateTestFile(
			_T("body.rtf"),
			"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Calibri;}}{\\*\\generator Msftedit 5.41.21.2510;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang9\\fs22 Test\\par\\page Test\\page\\b\\i Test\\b0\\i0\\f1\\par}"
		));
		ASSERT_TRUE(CreateTestFile(
			_T("cover.rtf"),
			"{\\rtf1\\ansi\\deff0\\deflang1033{\\fonttbl{\\f0\\froman\\fprq2 Times New Roman;}{\\f1\\fswiss\\fprq2 Arial;}{\\f2\\fswiss\\fprq2\\fcharset0 Arial;}}\\viewkind4\\uc1\\pard\\f0\\fs20\\par\\par\\b\\f1\\fs96 Fax:\\tab\\tab\\tab\\tab\\fs24 Time:\\tab\\tab <Time>\\par\\tab\\tab\\tab\\tab\\tab\\tab Date:\\tab\\tab <Date>\\par\\tab\\tab\\tab\\tab\\tab\\tab ________________\\f2 _\\f1 ____\\par\\par\\tab\\tab\\tab\\tab\\tab\\tab To:\\tab\\tab <R_Company>\\par\\tab\\tab\\tab\\tab\\tab\\tab\\f2 _____________________\\par\\f1\\tab\\tab\\tab\\tab\\tab\\tab\\par\\tab\\tab\\tab\\tab\\tab\\tab From:\\tab\\tab <S_Company>\\par\\tab\\tab\\tab\\tab\\tab\\tab\\tab\\tab\\par\\tab\\tab\\tab\\tab\\tab\\tab\\tab\\tab\\par\\b0\\f0\\fs20\\par\\par\\f1\\fs24 Message:\\par\\par<Message>\\par}"
		));

		memset(&m_cpt, 0, sizeof coverpage_template_t);
		strcpy_s(m_cpt.sourcefilepath, sizeof(m_cpt.sourcefilepath), "cover.rtf");
		strcpy_s(m_cpt.bodytextfilepath, sizeof(m_cpt.bodytextfilepath), "body.rtf");
		wcscpy_s(m_cpt.subject, sizeof(m_cpt.subject), L"subject");
		wcscpy_s(m_cpt.recipinfo.company, sizeof(m_cpt.recipinfo.company), L"company2");
		wcscpy_s(m_cpt.senderinfo.company, sizeof(m_cpt.recipinfo.company), L"company1");
	}
};

ACTION_P(ReturnFalseIfArgIsNULLOr, s)
{
	return (arg0!=NULL && _tcscmp(s, arg0)!=0);
}

TEST_F(Rtf2FaxApp_fixture, ConvertRTF_1pageDoc_createsATiffAndAppends1Page) {
	ASSERT_TRUE(CreateTestFile(
		_T("test.rtf"),
		"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Calibri;}} {\\*\\generator Msftedit 5.41.21.2510;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang9\\fs22 Test\\par \\b\\i Test\\b0\\i0\\f1\\par }"
	));
	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	ASSERT_EQ(FM_OK, GetObj()->ConvertRTF("test.rtf", "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxApp_fixture, ConvertRTF_3pagesDoc_createsATiffAndAppends3Pages) {
	ASSERT_TRUE(CreateTestFile(
		_T("test.rtf"),
		"{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\\fnil\\fcharset0 Arial;}{\\f1\\fnil\\fcharset0 Calibri;}}{\\*\\generator Msftedit 5.41.21.2510;}\\viewkind4\\uc1\\pard\\sa200\\sl276\\slmult1\\lang9\\fs22 Test\\par\\page Test\\page\\b\\i Test\\b0\\i0\\f1\\par}"
	));
	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(3);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	ASSERT_EQ(FM_OK, GetObj()->ConvertRTF("test.rtf", "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxApp_fixture, ConvertRTF_wrongFileType_returnsRTF_RTFNOTVALID) {
	ASSERT_TRUE(CreateTestFile(
		_T("test2.rtf"),
		"this is a txt file"
	));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(0);

	ASSERT_EQ(RTF_RTFNOTVALID, GetObj()->ConvertRTF("test2.rtf", "test.tiff", FALSE, FALSE));
}



TEST_F(Rtf2FaxAppCustom_fixture, ConvertRTFCustom_1pageRtfAndACustomField_createsATiffAndAppendsAtLeast1Page) {
	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(AtLeast(1));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	LPCSTR fields[]={"<custom_field>", NULL};
	LPCSTR values[]={"custom_value", NULL};
	ASSERT_EQ(FM_OK, GetObj()->ConvertRTFCustom("test.rtf", "test.tiff", FALSE, FALSE, "body.rtf", fields, values));
}

TEST_F(Rtf2FaxAppCustom_fixture, ConvertRTFCustom_txtFileAndACustomField_createsATiffAndAppendsAtLeast1Page) {
	ASSERT_TRUE(CreateTestFile(
		_T("test.txt"),
		"this is a txt file"
	));
	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(AtLeast(1));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	LPCSTR fields[]={"<custom_field>", NULL};
	LPCSTR values[]={"custom_value", NULL};
	ASSERT_EQ(FM_OK, GetObj()->ConvertRTFCustom("test.txt", "test.tiff", FALSE, FALSE, "body.rtf", fields, values));
}


//WARNING: this test uses RichEditCtrlTextContains that access internal fields. ConvertRTFCustom is not really used till now (Dec 2012)
TEST_F(Rtf2FaxAppCustom_fixture, ConvertRTFCustom_1pageDocAndACustomField_customFieldIsReplacedWithItsValue) {
	LPCSTR fields[]={"<custom_field>", NULL};
	LPCSTR values[]={"custom_value", NULL};
	ASSERT_EQ(FM_OK, GetObj()->ConvertRTFCustom("test.rtf", "test.tiff", FALSE, FALSE, "body.rtf", fields, values));

	ASSERT_TRUE(GetObj()->RichEditCtrlTextContains(_T("[[custom_value]]")));
}

TEST_F(Rtf2FaxApp_fixture, ConvertRTF_inexistentFile_returnsRTF_FILENOTFOUND) {
	//to call doesfileexist is not mandatory, replace it with ON_CALL(..).WillByDefault(...)
	EXPECT_CALL(GetHlpsMock(), doesfileexist(testing::An<LPCTSTR>())).WillRepeatedly(ReturnFalseIfArgIsNULLOr(_T("test.rtf")));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(0);

	ASSERT_EQ(RTF_FILENOTFOUND, GetObj()->ConvertRTF("test.rtf", "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxApp_fixture, ConvertTXT_txtDoc_createTiff) {
	ASSERT_TRUE(CreateTestFile(
		_T("test.txt"),
		"this is a txt file"
	));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(AtLeast(1));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	ASSERT_EQ(FM_OK, GetObj()->ConvertTXT("test.txt", "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxAppCustom_fixture, ConvertRTFCustom_inexistentFile_returnsRTF_FILENOTFOUND) {
	//to call doesfileexist is not mandatory, replace it with ON_CALL(..).WillByDefault(...)
	EXPECT_CALL(GetHlpsMock(), doesfileexist(testing::An<LPCTSTR>())).WillRepeatedly(ReturnFalseIfArgIsNULLOr(_T("test2.rtf")));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(0);

	LPCSTR fields[]={"<custom_field>", NULL};
	LPCSTR values[]={"custom_value", NULL};
	ASSERT_EQ(RTF_FILENOTFOUND, GetObj()->ConvertRTFCustom("test2.rtf", "test.tiff", FALSE, FALSE, "body.rtf", fields, values));
}

TEST_F(Rtf2FaxAppTemplates_fixture, ConvertTPT_3pagesDoc_createsATiffAndAppendsAtLeast3Pages) {
	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(AtLeast(3));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	ASSERT_EQ(FM_OK, GetObj()->ConvertTPT(&m_cpt, "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxAppTemplates_fixture, ConvertTPTFile_3pagesDoc_createsATiffAndAppendsAtLeast3Pages) {
	ASSERT_TRUE(CreateTestFile(
		_T("template.cpt"),
		(const char *)&m_cpt,
		sizeof(coverpage_template_t)
	));

	InSequence s;
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(1);
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffAppendPageFromBitmap(_, _, _, _, _))
		.Times(AtLeast(3));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCloseFile(_))
		.Times(1);

	ASSERT_EQ(FM_OK, GetObj()->ConvertTPTFile("template.cpt", "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxAppTemplates_fixture, ConvertTPT_inexistentCoverFile_returnsRTF_FILENOTFOUND) {
	strcpy_s(m_cpt.sourcefilepath, sizeof(m_cpt.sourcefilepath), "cover2.rtf");
	//to call doesfileexist is not mandatory, replace it with ON_CALL(..).WillByDefault(...)
	EXPECT_CALL(GetHlpsMock(), doesfileexist(testing::An<LPCTSTR>())).WillRepeatedly(ReturnFalseIfArgIsNULLOr(_T("cover2.rtf")));
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(0);

	ASSERT_EQ(RTF_FILENOTFOUND, GetObj()->ConvertTPT(&m_cpt, "test.tiff", FALSE, FALSE));
}

TEST_F(Rtf2FaxAppTemplates_fixture, ConvertTPT_wrongCoverFileType_returnsRTF_RTFNOTVALID) {
	ASSERT_TRUE(CreateTestFile(
		_T("cover2.rtf"),
		"this is a txt file"
	));
	strcpy_s(m_cpt.sourcefilepath, sizeof(m_cpt.sourcefilepath), "cover2.rtf");
	EXPECT_CALL(GetObj()->m_mockTiffFile, tiffCreateFile(_, _))
		.Times(0);

	ASSERT_EQ(RTF_RTFNOTVALID, GetObj()->ConvertTPT(&m_cpt, "test.tiff", FALSE, FALSE));
}



int main(int argc, char** argv)
{
	//AfxOleInit();

	::testing::InitGoogleMock(&argc, argv);
	//::testing::Environment* const env = ::testing::AddGlobalTestEnvironment(new _Environment());
    return RUN_ALL_TESTS();
}
