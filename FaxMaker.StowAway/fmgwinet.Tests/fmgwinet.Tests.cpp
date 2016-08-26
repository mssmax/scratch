// fmgwinet.Tests.cpp

#include "stdafx.h"
#include "mocks.h"

//==============================================================================
// Main_Test
//==============================================================================

int _tmain(int argc, char ** argv)
{
	NiceMock<CTemplatesMock>	*templatesMock;

	::testing::InitGoogleTest(&argc, argv);

	// setup mocks
	SetCTemplatesMock( templatesMock  = new NiceMock<CTemplatesMock>       );
	
	// run tests
	int rtn = RUN_ALL_TESTS();

	// free mocks
	delete templatesMock;

	return rtn;
}
