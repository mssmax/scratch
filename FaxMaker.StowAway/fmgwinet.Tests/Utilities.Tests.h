//==============================================================================
// Testing CUtilities
//==============================================================================
#pragma once
#ifdef PERFORM_UTILITIES_TESTS
//==============================================================================
#include "stdafx.h"
#include "mfg.h"
#include "mocks.h"
#include "UnitTesting.h"

//==============================================================================

class util_fxt: public mfgfixture
{
public:
	CUtilities  util;

	virtual void SetUp()
	{			
	}

	virtual void TearDown()
	{		
	}
};


#endif //PERFORM_UTILITIES_TESTS
//==============================================================================