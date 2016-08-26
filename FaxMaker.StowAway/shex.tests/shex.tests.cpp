// Shex.tests.cpp 
//==============================================================================

#include "stdafx.h"

using testing::_;
using testing::Return;
using testing::ReturnArg;
using testing::MatcherCast;
using testing::Invoke;
using testing::StrCaseEq;
using testing::EndsWith;



//==============================================================================

class shex_fxr : public mfgfixture
{
public:
	LPSTR prmlist[2];

	virtual void SetUp()		 
	{	
		LPSTR prm = ".doc";	
		prmlist[1] = prm;		
	};
	 
	 virtual void TearDown()	 
	 {	
	 };
};

//==============================================================================
LPSTR out = "ext";
DWORD GetPrivateProfileString_Fake(LPCSTR,LPCSTR,LPCSTR,LPSTR lpReturnedString,DWORD,LPCSTR)
{
	memcpy(lpReturnedString, "*!*", sizeof("*!*"));
	return 1;
}
  
//==============================================================================


//Not enough paramaters passed, return 0
TEST_F(shex_fxr, shexMain_notEnoughParam_returnZero)		
{	
	ASSERT_EQ (0, shexMain(1,NULL));
}

//Extension is not an allowed extension, and File extension not supported
TEST_F(shex_fxr, shexMain_ExtIsNotInAllowedAndNoConvertor_returnDOCCVTERR_EXTENSIONDENIED)		
{	
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq("*!*"),_,_,_)).WillOnce(Invoke(GetPrivateProfileString_Fake));
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq(""),_,_,_)).WillOnce(Return(0));
	
	ASSERT_EQ (DOCCVTERR_EXTENSIONDENIED, shexMain(2,prmlist));
}

//Extension is an allowed extension; Find FindExecutable fails
TEST_F(shex_fxr, shexMain_AllowedExt_returnDOCCVTERR_NOAPPLICATION)		
{	
	ASSERT_EQ (DOCCVTERR_NOAPPLICATION, shexMain(2,prmlist));
}


//Shell Execute fails
TEST_F(shex_fxr, shexMain_ShellExecuteFails_returnDOCCVTERR_NOAPPLICATION)		
{	
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq("*!*"),_,_,_)).WillOnce(Invoke(GetPrivateProfileString_Fake));
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq(""),_,_,_)).WillOnce(Invoke(GetPrivateProfileString_Fake));

	EXPECT_CALL(m_systemcalls_mock, ShellExecuteEx(_)).WillOnce(Return(FALSE));
	ASSERT_EQ (DOCCVTERR_NOAPPLICATION, shexMain(2,prmlist));
}

//Extension is not an allowed extension, and File extension is supported
TEST_F(shex_fxr, shexMain_ExtIsNotInAllowedAndNoConvertor_returnZero)		
{	
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq("*!*"),_,_,_)).WillOnce(Invoke(GetPrivateProfileString_Fake));
	EXPECT_CALL(m_systemcalls_mock, GetPrivateProfileString(_,_,StrCaseEq(""),_,_,_)).WillOnce(Invoke(GetPrivateProfileString_Fake));

	EXPECT_CALL(m_systemcalls_mock, ShellExecuteEx(_)).WillOnce(Return(TRUE));
	ASSERT_EQ (0, shexMain(2,prmlist));
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

//End shex.tests.cpp ===========================================================
