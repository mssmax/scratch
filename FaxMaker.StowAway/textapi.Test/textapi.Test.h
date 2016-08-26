//TextAPI.Text.H
//==============================================================================
#pragma once
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "textapi.h"
//------------------------------------------------------------------------------
using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::EndsWith;
using ::testing::Invoke;
using ::testing::AtLeast;
using ::testing::MatcherCast;
//==============================================================================

//Common Stuff

//hlps
#define expect_doesfileexist(no,ret) EXPECT_CALL(m_hlps_mock, doesfileexist(MatcherCast<LPCSTR>(_))).Times(AtLeast(no)); \
	ON_CALL(m_hlps_mock, doesfileexist(MatcherCast<LPCSTR>(_))).WillByDefault(Return(ret));
#define expect_changeextension(no) EXPECT_CALL(m_hlps_mock, changeextension(_,_)).Times(AtLeast(no));
#define expect_adjustfilename(no,ret) EXPECT_CALL(m_hlps_mock, adjustfilename(_,_)).Times(AtLeast(no)); \
	ON_CALL(m_hlps_mock, adjustfilename(_,_)).WillByDefault(Return(ret));

//textfile (in hlps)
#define expect_tf_create(no,ret) EXPECT_CALL(tf_c_m,create(MatcherCast<LPCSTR>(_),_)).Times(AtLeast(no)); \
	ON_CALL(tf_c_m, create(MatcherCast<LPCSTR>(_),_)).WillByDefault(Return(ret));
#define expect_tf_close(no) EXPECT_CALL(tf_c_m,close()).Times(AtLeast(no)); 

//systemcalls
#define expect_CreateFileRead(no,ret)  EXPECT_CALL(m_systemcalls_mock,CreateFile(_,GENERIC_READ,_,_,_,_,_)).Times(AtLeast(no)); \
	ON_CALL(m_systemcalls_mock, CreateFile(_,GENERIC_READ,_,_,_,_,_)).WillByDefault(Return(ret));   
#define expect_CreateFileWrite(no,ret)  EXPECT_CALL(m_systemcalls_mock,CreateFile(_,GENERIC_WRITE,_,_,_,_,_)).Times(AtLeast(no)); \
	ON_CALL(m_systemcalls_mock, CreateFile(_,GENERIC_WRITE,_,_,_,_,_)).WillByDefault(Return(ret));  

//FFM
#define expect_ffm_Open(no,ret) EXPECT_CALL(mffm,Open(_)).Times(AtLeast(no)).WillRepeatedly(Return(ret));
#define expect_ffm_Close(no) EXPECT_CALL(mffm,Close()).Times(AtLeast(no)); 
#define expect_ffm_GetFlags(no, ret) EXPECT_CALL(mffm,GetFlags()).Times(no);	\
	ON_CALL(mffm, GetFlags()).WillByDefault(Return(ret));
#define expect_ffm_SetFlags(times) EXPECT_CALL(mffm,SetFlags(_)).Times(times);
#define expect_ffm_GetSender(no, ret) EXPECT_CALL(mffm,GetSender()).Times(no);	\
	ON_CALL(mffm, GetSender()).WillByDefault(Return(ret));
#define expect_ffm_GetSubject(ret) EXPECT_CALL(mffm,GetSubject());		\
	ON_CALL(mffm, GetSubject()).WillByDefault(Return(ret));
#define expect_ffm_GetBodyFile(no, ret) EXPECT_CALL(mffm, GetBodyFile()).Times(no);   \
	ON_CALL(mffm, GetBodyFile()).WillByDefault(Return(ret));
#define expect_ffm_GetFaxMessage(no, ret) EXPECT_CALL(mffm, GetFaxMessage()).Times(no);  \
	ON_CALL(mffm, GetFaxMessage()).WillByDefault(Return(&ret));
#define expect_ffm_GetFilePath(no,ret)  EXPECT_CALL(mffm,GetFilePath()).Times(AtLeast(no)); \
	ON_CALL(mffm, GetFilePath()).WillByDefault(Return(ret)); 

//html_slayer
#define expect_htmlslayer_gettextapicmd(no,ret) \
	EXPECT_CALL(m_hs_mock, htmlslayer_gettextapicmd_(_,_)).Times(no);  \
	ON_CALL(m_hs_mock, htmlslayer_gettextapicmd_(_,_)).WillByDefault(Return(ret));

//text writer/reader
#define expect_textwriter_init(no,ret)  EXPECT_CALL(tw_m,Init(_,_)).Times(AtLeast(no)); \
	ON_CALL(tw_m, Init(_,_)).WillByDefault(Return(ret)); 

//------------------------------------------------------------------------------

class textapi_initfxt :  public mfgfixture
{
public:
	NiceMock<FMCFGDBLib::IGFIRULEDBMock> m_gfiruledbMock; 	
	NiceMock<GSMServicesInterface_Mock> gsmmock;
	NiceMock<Ccfgstr_glb_Mock> cfgstore;

	virtual void SetUp()		 
	{	
		setInit(FALSE);
		SetCfgstoreGlbMock(&cfgstore);
	};

	virtual void TearDown()	  {	};
};


class textApi_ProcessFile_str : public mfgfixture
{
public:
	virtual void SetUp()		 
	{	
		SetFaxFrmtMsgMock(&mffm);
	};

	virtual void TearDown()	  {	};

	NiceMock<CFaxFrmtMessage_Mock> mffm;

protected:
};

class textApi_ProcessFileTextFxtr : public mfgfixture
{
public:
	virtual void SetUp()		 
	{	
		SetFaxFrmtMsgMock(&mffm);
		SetHtmlSlayerMock(&m_hs_mock);		
		CoInitialize(NULL);		
	};

	virtual void TearDown()	 
	{	
		CoUninitialize();
	};

	NiceMock<CFaxFrmtMessage_Mock> mffm;
	NiceMock<CHtmlSlayer_Mock> m_hs_mock;	
	NiceMock<GSMServicesInterface_Mock>  gsmmock;	

protected:
};


//End TextAPI.Test.h ===========================================================

