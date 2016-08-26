#include "StdAfx.h"
#include "GFIDB.tests.h"
#include "GFIRULE.tests.h"

/*
 *  Variables and getters methods used for unit testing.
 */

gfi_log_c	g_gfilog;
CComModule	_Module;
CBase64Mock	*base64Mock;
NiceMock<fmstore_c_mock<globalconfig_t, config_t>>	*fmstoreMock;

ICBase64* GetICBase64()
{	return base64Mock;		}

ifmstore_c<globalconfig_t, config_t>*	GetFmStore()
{	return fmstoreMock;		}

///////////////////////////////////////////////////////////////////////////////


int main(int argc, char** argv)
{
	testing::InitGoogleMock(&argc, argv);

	// initialise shared variables
	ASSERT_HRESULT_SUCCEEDED( CoInitialize(NULL) );
	_Module.Init(NULL, GetModuleHandle(0));

	base64Mock  = new CBase64Mock;
	fmstoreMock = new NiceMock<fmstore_c_mock<globalconfig_t, config_t>>;
	_pAtlModule = &_Module;

	// run tests
	int rtn = RUN_ALL_TESTS();

	// destroy shared variables
	delete base64Mock;
	delete fmstoreMock;
	CoUninitialize();

	return rtn;
}