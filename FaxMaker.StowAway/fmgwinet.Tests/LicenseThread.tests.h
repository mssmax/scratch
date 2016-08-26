#pragma once
#ifdef PERFORM_LICENSETHREAD_TESTS
#include "StdAfx.h"
#include "UnitTesting.h"
#include "LicenseThread.h"
#include "UserInfo.h"
#include <mfg_gfikey_decoder.h>


/*
 *  Fixtures.
 */

class LicenseThreadFixture : public mfgfixture
{
public:
	CLicenseThread				*licenseThread;
	NiceMock<fmgfidb_c_mock>	*fmgfidbMock;		// used by CLicenseThread via CUserInfo
	NiceMock<IReportsMock>		reportsMock;
	
	NiceMock<gfikey_decoder_ex_c_wrapper_mock>	gfikeyDecoderMock;

	// mocks used by fmgfidbMock
	NiceMock<FMCFGDBLib::IGFIDBMock>			gfidbMock;
	NiceMock<FMCFGDBLib::IGFIRULEDBMock>		gfiruledbMock;
	NiceMock<FMCFGDBLib::IFAXmakerMock>			faxmakerMock;
	NiceMock<GFIUSERDBLib::IGFIDBMock>			userdbMock;
	NiceMock<GFIUSERDBLib::IGFIUserLocatorMock>	userlocatorMock;
	
public:
	void SetUp()
	{
		fmgfidbMock = new NiceMock<fmgfidb_c_mock>(&gfidbMock, &gfiruledbMock, &faxmakerMock, &userdbMock, &userlocatorMock);
		CUserInfo::SetGfidb(fmgfidbMock);

		licenseThread = new CLicenseThread(&reportsMock);
		set_gfikey_decoder_wrapper_mock(&gfikeyDecoderMock);
	}

	void TearDown()
	{
		CUserInfo::freedb();	// frees fmgfidb
		delete licenseThread;
	}

protected:
	void SimulatePurchaseDate(WORD year, WORD month, WORD day);
	CLicenseThread::ExpiryType TestMaintenanceContractExpiryDate(/*purchase date         */ WORD year, WORD month, WORD day,
												 /*months till MC expires*/ WORD maintenanceContractMonths);
};

#endif	// PERFORM_LICENSETHREAD_TESTS