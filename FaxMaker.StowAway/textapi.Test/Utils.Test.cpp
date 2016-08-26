//Utils.Test.cpp
//==============================================================================
#include "stdafx.h"
#include "utils.h"
#include "mfg.h"

//==============================================================================
//Globals from textapi.cpp:
extern map<ULONG, wstring> g_MapLinesSymbolicLinks;

class textApi_utilsFxtre : public mfgfixture
{
public:
	virtual void SetUp()
	{
	}

	virtual void TearDown()
	{
	}
};

namespace F
{
	//==============================================================================
	// TEST CODE RELATED WITH isonlydigit()
	//==============================================================================
	TEST_F(textApi_utilsFxtre, isonlydigit_NumbersOnly_returnTrue)
	{
		ASSERT_EQ(TRUE, isonlydigit(L"1234"));
	}

	TEST_F(textApi_utilsFxtre, isonlydigit_MixedChar_returnFalse)
	{
		ASSERT_EQ(FALSE, isonlydigit(L"1a234"));
	}

	// TEST CODE RELATED WITH isFaxNumber()
	//==============================================================================

	TEST_F(textApi_utilsFxtre, isFaxNumber_lpsznumIsNULL_return0)
	{
		ASSERT_EQ(0, isFaxNumber(NULL));
	}

	TEST_F(textApi_utilsFxtre, isFaxNumber_lpsznumIsEmpty_return0)
	{
		ASSERT_EQ(0, isFaxNumber(L""));
	}

	TEST_F(textApi_utilsFxtre, IsFaxNumber_lpsznumContainUnallowedChars_return0)
	{
		ASSERT_EQ(0, isFaxNumber(L"2134bb56789"));
	}

	TEST_F(textApi_utilsFxtre, IsFaxNumber_lpsznumContainOnlyAllowedChars_return0)
	{
		ASSERT_EQ(1, isFaxNumber(L"213456789"));
	}
}
//End utils.test.cpp ===========================================================
