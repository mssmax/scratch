#include "stdafx.h"
#include <afxwin.h>
#include "mfg.h"
using namespace gfihlps;
using ::testing::Test;
using testing::NiceMock;

//#pragma comment (lib, "tiff32.lib")
#include "tifffile.h"
#include "Bitmap_1bpp.h"
#include "tiffinfo.h"

extern LPTIFFINFO g_pti[MAXHANDLE_TIFF];

//TODO reset all structures in TearDown used by Tiff32 (i.e. close tiff files)
class Tiff32WriteTest : public mfgfixture
{
};

class Tiff32ReadTest : public mfgfixture
{
protected:
	static char *BN_1728x4tifFileBuffer;
	static const int BN_1728x4tifFileSize= 258;

	static char *BN_LowRes_1728x2tifFileBuffer;
	static const int BN_LowRes_1728x2tifFileSize= 248;

	static char *BNmulti_3x4_4x4_5x4tifFileBuffer;
	static const int BNmulti_3x4_4x4_5x4tifFileSize= 674;

	bool CreateTestFile(LPCTSTR lpFileName, LPCVOID pBuffer, DWORD bufferSize=0) {
		HANDLE hFile=GetSystemCallsMock().CreateFile(lpFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, 0, NULL);
		DWORD nofb;
		if (!GetSystemCallsMock().WriteFile(hFile, pBuffer, bufferSize>0 ? bufferSize : strlen((char *)pBuffer), &nofb, NULL)) return false;
		//ASSERT_EQ(bufferSize, nofb);
		return GetSystemCallsMock().CloseHandle(hFile);
	}

	virtual void SetUp() {
		ASSERT_TRUE(CreateTestFile(TEXT("BN_1728x4.tif"), BN_1728x4tifFileBuffer, BN_1728x4tifFileSize));
	}
	virtual void TearDown() {
		
	}
};
char *Tiff32ReadTest::BN_1728x4tifFileBuffer=
	"\x49\x49\x2a\x00\x22\x00\x00\x00\x00\x13\x51\x18\x58\x00\x13\x54\x3a\x1d\x0e\x87\x4c\x2c\x00\x0a\x6c\xd4\x00\x47\x43\xa1\xd0\xe9\x85\x90\x10\x00\x00\x01\x03\x00\x01\x00\x00\x00\xc0\x06\x00\x00\x01\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x02\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x01\x03\x00\x01\x00\x00\x00\x00\x00\x00\x00\x0a\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x11\x01\x04\x00\x01\x00\x00\x00\x08\x00\x00\x00\x12\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x15\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x16\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x17\x01\x04\x00\x01\x00\x00\x00\x1a\x00\x00\x00\x1a\x01\x05\x00\x01\x00\x00\x00\xe8\x00\x00\x00\x1b\x01\x05\x00\x01\x00\x00\x00\xf0\x00\x00\x00\x1c\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x28\x01\x03\x00\x01\x00\x00\x00\x02\x00\x00\x00\x31\x01\x02\x00\x0a\x00\x00\x00\xf8\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x18\x00\x00\x20\x00\x00\x00\x00\x18\x00\x00\x20\x00\x49\x72\x66\x61\x6e\x56\x69\x65\x77\x00";
char *Tiff32ReadTest::BN_LowRes_1728x2tifFileBuffer=
	"\x49\x49\x2a\x00\x18\x00\x00\x00\x00\x13\x51\x18\x58\x00\x13\x54\x3a\x1d\x0e\x87\x4c\x2c\x00\x00\x10\x00\x00\x01\x03\x00\x01\x00\x00\x00\xc0\x06\x00\x00\x01\x01\x03\x00\x01\x00\x00\x00\x02\x00\x00\x00\x02\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x01\x03\x00\x01\x00\x00\x00\x00\x00\x00\x00\x0a\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x11\x01\x04\x00\x01\x00\x00\x00\x08\x00\x00\x00\x12\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x15\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x16\x01\x03\x00\x01\x00\x00\x00\x02\x00\x00\x00\x17\x01\x04\x00\x01\x00\x00\x00\x0f\x00\x00\x00\x1a\x01\x05\x00\x01\x00\x00\x00\xde\x00\x00\x00\x1b\x01\x05\x00\x01\x00\x00\x00\xe6\x00\x00\x00\x1c\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x28\x01\x03\x00\x01\x00\x00\x00\x02\x00\x00\x00\x31\x01\x02\x00\x0a\x00\x00\x00\xee\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x01\x00\x00\x00\x49\x72\x66\x61\x6e\x56\x69\x65\x77\x00";
char *Tiff32ReadTest::BNmulti_3x4_4x4_5x4tifFileBuffer=
	"\x49\x49\x2a\x00\x12\x00\x00\x00\x00\x17\x40\x02\xe8\x00\x47\xc0\x04\xd6\x0f\x00\x00\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x01\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x02\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x01\x03\x00\x01\x00\x00\x00\x00\x00\x00\x00\x11\x01\x04\x00\x01\x00\x00\x00\x08\x00\x00\x00\x12\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x15\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x16\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x17\x01\x04\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x1a\x01\x05\x00\x01\x00\x00\x00\xcc\x00\x00\x00\x1b\x01\x05\x00\x01\x00\x00\x00\xd4\x00\x00\x00\x1c\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x29\x01\x03\x00\x02\x00\x00\x00\x00\x00\x01\x00\x31\x01\x02\x00\x0a\x00\x00\x00\xdc\x00\x00\x00\xf0\x00\x00\x00\x00\x00\x00\x18\x00\x00\x20\x00\x00\x00\x00\x18\x00\x00\x20\x00\x49\x72\x66\x61\x6e\x56\x69\x65\x77\x00\x00\x18\x40\x03\x08\x00\x5f\x00\x13\x56\x0f\x00\x00\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x01\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x02\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x01\x03\x00\x01\x00\x00\x00\x00\x00\x00\x00\x11\x01\x04\x00\x01\x00\x00\x00\xe6\x00\x00\x00\x12\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x15\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x16\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x17\x01\x04\x00\x01\x00\x00\x00\x0a\x00\x00\x00\x1a\x01\x05\x00\x01\x00\x00\x00\xaa\x01\x00\x00\x1b\x01\x05\x00\x01\x00\x00\x00\xb2\x01\x00\x00\x1c\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x29\x01\x03\x00\x02\x00\x00\x00\x00\x00\x02\x00\x31\x01\x02\x00\x0a\x00\x00\x00\xba\x01\x00\x00\xce\x01\x00\x00\x00\x00\x00\x18\x00\x00\x20\x00\x00\x00\x00\x18\x00\x00\x20\x00\x49\x72\x66\x61\x6e\x56\x69\x65\x77\x00\x00\x18\xc0\x06\x30\x01\x78\x00\x63\x00\x0f\x00\x00\x01\x03\x00\x01\x00\x00\x00\x05\x00\x00\x00\x01\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x02\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x03\x01\x03\x00\x01\x00\x00\x00\x03\x00\x00\x00\x06\x01\x03\x00\x01\x00\x00\x00\x00\x00\x00\x00\x11\x01\x04\x00\x01\x00\x00\x00\xc4\x01\x00\x00\x12\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x15\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x16\x01\x03\x00\x01\x00\x00\x00\x04\x00\x00\x00\x17\x01\x04\x00\x01\x00\x00\x00\x09\x00\x00\x00\x1a\x01\x05\x00\x01\x00\x00\x00\x88\x02\x00\x00\x1b\x01\x05\x00\x01\x00\x00\x00\x90\x02\x00\x00\x1c\x01\x03\x00\x01\x00\x00\x00\x01\x00\x00\x00\x29\x01\x03\x00\x02\x00\x00\x00\x00\x00\x03\x00\x31\x01\x02\x00\x0a\x00\x00\x00\x98\x02\x00\x00\x00\x00\x00\x00\x00\x00\x00\x18\x00\x00\x20\x00\x00\x00\x00\x18\x00\x00\x20\x00\x49\x72\x66\x61\x6e\x56\x69\x65\x77\x00";


class Tiff32ProcessTest : public Tiff32ReadTest {};

class tiffdecode_cTest : public Tiff32ReadTest
{
protected:
	HTIFF m_htiff;

	const char *m_fileBuffer;
	const int m_fileSize;

	tiffdecode_cTest(const char *fileBuffer, const int fileSize) : m_fileBuffer(fileBuffer), m_fileSize(fileSize)  {}


	virtual void SetUp()
	{
		ASSERT_TRUE(CreateTestFile(TEXT("dec_testfile.tif"), m_fileBuffer, m_fileSize));
		pageinfo_t pi;
		ASSERT_EQ(R_SUCCESS, tiffOpenFile("dec_testfile.tif", &m_htiff));
		ASSERT_EQ(R_SUCCESS, tiffFindFirstPage(m_htiff, &pi));
	}

	virtual void TearDown()
	{
		tiffCloseFile(m_htiff);
	}
};

class tiffdecode_cTest_HighResFile : public tiffdecode_cTest
{
protected:
	tiffdecode_cTest_HighResFile() :
		tiffdecode_cTest(Tiff32ReadTest::BN_1728x4tifFileBuffer, Tiff32ReadTest::BN_1728x4tifFileSize)
		{}
};

class tiffdecode_cTest_LowResFile : public tiffdecode_cTest
{
protected:
	tiffdecode_cTest_LowResFile() :
		tiffdecode_cTest(Tiff32ReadTest::BN_LowRes_1728x2tifFileBuffer, Tiff32ReadTest::BN_LowRes_1728x2tifFileSize)
		{}
};


TEST_F(Tiff32WriteTest, tiffAppendPageFromBitmap_BNBitmap_createsAFileOfMeaningfulSize)
{

	HTIFF htiff;
	
	ASSERT_EQ(R_SUCCESS, tiffCreateFile("test.tiff", &htiff));

	CBitmap bitMap;
	CDC memDC;
	bitMap.CreateBitmap(1728,4,1,1,NULL);
	memDC.SelectObject(bitMap);
	memDC.FillSolidRect(0, 0, 1727, 3, RGB(255,255,255));

	ASSERT_TRUE(tiffAppendPageFromBitmap(htiff, NULL, bitMap, 1728, 4));
	ASSERT_EQ(R_SUCCESS, tiffCloseFile(htiff));
	
	HANDLE hf=GetSystemCallsMock().CreateFile(TEXT("test.tiff"), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL); 
	ASSERT_NE(INVALID_HANDLE_VALUE, hf);
	DWORD nofb;
	BYTE buffer[512];
	ASSERT_TRUE(GetSystemCallsMock().ReadFile(hf, buffer, 512, &nofb, NULL));
	ASSERT_GT(nofb, 216);

}


TEST_F(Tiff32ReadTest, tiffOpenFile_inexistentFile_fails) {
	HTIFF htiff;
	ASSERT_EQ(R_ERROR, tiffOpenFile("inexistent.tif", &htiff));
}

TEST_F(Tiff32ReadTest, tiffFindFirstPage)
{
	HTIFF htiff;
	pageinfo_t firstPageInfo;
	ASSERT_EQ(R_SUCCESS, tiffOpenFile("BN_1728x4.tif", &htiff));
	ASSERT_EQ(R_SUCCESS, tiffFindFirstPage(htiff, &firstPageInfo));
	
	ASSERT_EQ(1728, (int)firstPageInfo.piImageWidth);
	ASSERT_EQ(4, (int)firstPageInfo.piImageLength);
	
}

TEST_F(Tiff32ReadTest, tiffFindNextPage_withNoExtraPages_fails)
{
	HTIFF htiff;
	pageinfo_t pageInfo;
	ASSERT_EQ(R_SUCCESS, tiffOpenFile("BN_1728x4.tif", &htiff));
	ASSERT_EQ(R_SUCCESS, tiffFindFirstPage(htiff, &pageInfo));
	ASSERT_EQ(R_ERROR, tiffFindNextPage(htiff, &pageInfo));

}

TEST_F(Tiff32ReadTest, tiffFindNextPage_onA3PagesTiff_retrievedPagesSizeAreCorrect)
{
	ASSERT_TRUE(CreateTestFile(TEXT("BNmulti.tif"), BNmulti_3x4_4x4_5x4tifFileBuffer, BNmulti_3x4_4x4_5x4tifFileSize));

	HTIFF htiff;
	ASSERT_EQ(R_SUCCESS, tiffOpenFile("BNmulti.tif", &htiff));

	pageinfo_t firstPageInfo;
	ASSERT_EQ(R_SUCCESS, tiffFindFirstPage(htiff, &firstPageInfo));
	ASSERT_EQ(3, (int)firstPageInfo.piImageWidth);
	ASSERT_EQ(4, (int)firstPageInfo.piImageLength);

	pageinfo_t secondPageInfo;
	ASSERT_EQ(R_SUCCESS, tiffFindNextPage(htiff, &secondPageInfo));
	ASSERT_EQ(4, (int)secondPageInfo.piImageWidth);
	ASSERT_EQ(4, (int)secondPageInfo.piImageLength);

	pageinfo_t thirdPageInfo;
	ASSERT_EQ(R_SUCCESS, tiffFindNextPage(htiff, &thirdPageInfo));
	ASSERT_EQ(5, (int)thirdPageInfo.piImageWidth);
	ASSERT_EQ(4, (int)thirdPageInfo.piImageLength);

	ASSERT_EQ(R_SUCCESS, tiffCloseFile(htiff));
}


TEST_F(Tiff32ProcessTest, tiffTruncate_lastPageOf3AsInput_outputHas1page)
{
	ASSERT_TRUE(CreateTestFile(TEXT("BNmulti.tif"), BNmulti_3x4_4x4_5x4tifFileBuffer, BNmulti_3x4_4x4_5x4tifFileSize));

	ASSERT_EQ(TRUE, tiffTruncate("BNmulti.tif", "truncated.tif", 2, 2, NULL));

	//check that only the 2nd page has been written
	HTIFF htiff;
	ASSERT_EQ(R_SUCCESS, tiffOpenFile("truncated.tif", &htiff));
	pageinfo_t pageInfo;
	ASSERT_EQ(R_SUCCESS, tiffFindFirstPage(htiff, &pageInfo));
	ASSERT_EQ(4, (int)pageInfo.piImageWidth);
	ASSERT_EQ(R_ERROR, tiffFindNextPage(htiff, &pageInfo));
}


TEST(Bitmap_1bpp, ResizeProportionally_doubleWidth_doubleHeight)
{
	CBitmap bitMap;
	CDC memDC;
	bitMap.CreateBitmap(5,10,1,1,NULL);
	memDC.SelectObject(bitMap);
	memDC.FillSolidRect(0, 0,10, 10, RGB(255,255,255));

	Bitmap_1bpp b;
	b.CreateBitmap(memDC.m_hDC, bitMap);
	b.ResizeProportionally(10);
	ASSERT(20, b.infoHeader.biHeight);
}


//======== tiffdecode_c tests ========
TEST_F(tiffdecode_cTest_HighResFile, readbit_fileDecoded_succeeds)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	BYTE b;
	ASSERT_TRUE(td.readbit(&b));
}

TEST_F(tiffdecode_cTest_HighResFile, pd_getscanlines_fileDecoded_returns4)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	ASSERT_EQ(4, td.pd_getscanlines());
}

TEST_F(tiffdecode_cTest_HighResFile, pd_getblackpixels_fileDecoded_returnsExpectedValues)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	EXPECT_EQ(9, tiffhlps_c::getblackpixels(&td,0));
	EXPECT_EQ(5, tiffhlps_c::getblackpixels(&td,1));
	EXPECT_EQ(0, tiffhlps_c::getblackpixels(&td,2));
	EXPECT_EQ(4, tiffhlps_c::getblackpixels(&td,3));
}


//watchdog for internal code limitation
TEST_F(tiffdecode_cTest_HighResFile, allocatepagedata_pageWidthArgIsNot1728_fails)
{
	tiffdecode_c td;
	EXPECT_FALSE(td.allocatepagedata(0, 1));
	EXPECT_FALSE(td.allocatepagedata(1727, 1));
	EXPECT_FALSE(td.allocatepagedata(1729, 1));
}

TEST_F(tiffdecode_cTest_HighResFile, allocatepagedata_pageWidthArgIs1728_succeeds)
{
	tiffdecode_c td;
	ASSERT_TRUE(td.allocatepagedata(1728, 1));
}



TEST_F(tiffdecode_cTest_LowResFile, readbit_fileDecoded_succeeds)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	BYTE b;
	ASSERT_TRUE(td.readbit(&b));
}

TEST_F(tiffdecode_cTest_LowResFile, pd_getscanlines_fileDecoded_returns4)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	ASSERT_EQ(4, td.pd_getscanlines());
}

TEST_F(tiffdecode_cTest_LowResFile, pd_getblackpixels_fileDecoded_returnsExpectedValues)
{
	tiffdecode_c td(m_htiff);
	ASSERT_TRUE(td.decodepage());
	EXPECT_EQ(9, tiffhlps_c::getblackpixels(&td,0));
	EXPECT_EQ(9, tiffhlps_c::getblackpixels(&td,1));
	EXPECT_EQ(5, tiffhlps_c::getblackpixels(&td,2));
	EXPECT_EQ(5, tiffhlps_c::getblackpixels(&td,3));
}


//watchdog for internal code limitation
TEST_F(tiffdecode_cTest_LowResFile, allocatepagedata_pageWidthArgIsNot1728_fails)
{
	tiffdecode_c td;
	EXPECT_FALSE(td.allocatepagedata(0, 1));
	EXPECT_FALSE(td.allocatepagedata(1727, 1));
	EXPECT_FALSE(td.allocatepagedata(1729, 1));
}

TEST_F(tiffdecode_cTest_LowResFile, allocatepagedata_pageWidthArgIs1728_succeeds)
{
	tiffdecode_c td;
	ASSERT_TRUE(td.allocatepagedata(1728, 1));
}


int _tmain(int argc, _TCHAR* argv[])
{
	for(int i = 0; i < MAXHANDLE_TIFF; i++) g_pti[i] = NULL;
	::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
