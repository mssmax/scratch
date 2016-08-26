//==============================================================================
// Testing CUtilities
//==============================================================================

#include "stdafx.h"

#ifdef PERFORM_UTILITIES_TESTS

#include "Utilities.Tests.h"

//==============================================================================
// CUtilities::isFaxNumber()
//==============================================================================

TEST_F(util_fxt, isFaxNumber_lpsznumIsNULL_return0)
{
  	ASSERT_EQ(0, util.IsFaxNumber(NULL));
}

TEST_F(util_fxt, isFaxNumber_lpsznumIsEmpty_return0)
{
  	ASSERT_EQ(0, util.IsFaxNumber(L""));
}

TEST_F(util_fxt, IsFaxNumber_lpsznumContainUnallowedChars_return0)
{
  	ASSERT_EQ(0, util.IsFaxNumber(L"2134bb56789"));
}

TEST_F(util_fxt, IsFaxNumber_lpsznumContainOnlyAllowedChars_return0)
{
  	ASSERT_EQ(1, util.IsFaxNumber(L"213456789"));
}

//==============================================================================
// CUtilities::MoveToIgnored()  
// - Nothing to test, as it has only 1 if statement, and it doesn't affect the 
//   final return.
//==============================================================================


//==============================================================================
// CRSxMessage::ExtractEmail()
//==============================================================================
TEST_F(util_fxt, ExtractEmail_NoAtChar_ReturnAsInput)
{
	ASSERT_STREQ(L"dd",util.ExtractEmail(L"dd").c_str());
}

TEST_F(util_fxt, ExtractEmail_AtChar_ReturnCorrectEmailAddress)
{
	strw_c strd = L"<sdd@dd.com>   ";	
	ASSERT_STREQ(L"sdd@dd.com",util.ExtractEmail(strd).c_str());
}


#endif //PERFORM_UTILITIES_TESTS

//==============================================================================