#pragma once
#ifdef PERFORM_REPORTS_TESTS
#include "Reports.h"
#include "UnitTesting.h"

/*
 *  Fixtures.
 */

struct ReportsFixture : public mfgfixture
{
	CReports						*reports;
	faxmessage_t					faxMessage;
	NiceMock<CStatusInterfaceMock>	statusInterface;

	void SetUp()
	{
		reports = new CReports(&statusInterface);
		memset(&faxMessage, 0, sizeof(faxMessage));
		ON_CALL(m_faxfrmtmessage_mock, GetFaxMessage()).WillByDefault(Return(&faxMessage));
	}
};

#endif	// PERFORM_REPORTS_TESTS