#include "StdAfx.h"
#ifdef PERFORM_LICENSETHREAD_TESTS
#include "LicenseThread.tests.h"

// current date
const WORD	CURRENT_YEAR	= 2012,
			CURRENT_MONTH	= 12,
			CURRENT_DAY		= 25;


void GetCurrentSystemTime(LPSYSTEMTIME currentTime)
{
	if (!currentTime)
		return;

	memset(currentTime, 0, sizeof(SYSTEMTIME));
	currentTime->wDay	= CURRENT_DAY;
	currentTime->wMonth	= CURRENT_MONTH;
	currentTime->wYear	= CURRENT_YEAR;
	// we do not care about other values...
}


void LicenseThreadFixture::SimulatePurchaseDate(WORD year, WORD month, WORD day)
{
	EXPECT_CALL(gfikeyDecoderMock, get_purchasedate_year(_))
		.WillOnce( DoAll(
			SetArgPointee<0>(year),
			Return(TRUE)
	));

	EXPECT_CALL(gfikeyDecoderMock, get_purchasedate_month(_))
		.WillOnce( DoAll(
			SetArgPointee<0>(month),
			Return(TRUE)
	));

	EXPECT_CALL(gfikeyDecoderMock, get_purchasedate_day(_))
		.WillOnce( DoAll(
			SetArgPointee<0>(day),
			Return(TRUE)
	));
}


CLicenseThread::ExpiryType LicenseThreadFixture::TestMaintenanceContractExpiryDate( /*purchase date         */ WORD year, WORD month, WORD day,
																					/*months till MC expires*/ WORD maintenanceContractMonths)
{
	EXPECT_CALL(m_systemcalls_mock, GetLocalTime(_))
		.WillOnce(Invoke(GetCurrentSystemTime));

	EXPECT_CALL(gfikeyDecoderMock, is_maintenancecontract_enabled(_))
		.WillOnce( DoAll(
			SetArgPointee<0>( TRUE ),	// assume the user has a maintenance contract
			Return(TRUE)
	));

	// set license purchase date
	SimulatePurchaseDate(year, month, day);

	EXPECT_CALL(gfikeyDecoderMock, get_maintenancecontract_months(_))
		.WillOnce( DoAll(
			SetArgPointee<0>( maintenanceContractMonths ),
			Return(TRUE)
	));

	licenseThread->DoWork();
	return licenseThread->GetMaintenanceContractExpiryType();
}



/******************************************************************************
 *
 *  Testing DoWork().
 */

TEST_F(LicenseThreadFixture, DoWork_ExpiredMC_ReturnsExpired)
{
	CLicenseThread::ExpiryType maintenanceContractExpiry;

	// User have bought the license 3 years ago:
	//    Date of purchase:      25/12/2009
	//    Maintenance contract:  2 Months
	//    Current Date:          25/12/2012
	maintenanceContractExpiry =
		TestMaintenanceContractExpiryDate(/*purchase date         */ CURRENT_YEAR - 3, CURRENT_MONTH, CURRENT_DAY,
										  /*months till MC expires*/ 2);

	// MC should be expired
	EXPECT_EQ(CLicenseThread::EXPIRED, maintenanceContractExpiry);
}


TEST_F(LicenseThreadFixture, DoWork_AlmostExpiredMC_ReturnsAlmostExpired)
{
	CLicenseThread::ExpiryType maintenanceContractExpiry;

	// User have bought the license almost 2 months ago:
	//    Date of purchase:      03/11/2012
	//    Maintenance contract:  2 Months
	//    Current Date:          25/12/2012
	// (Will expire within 9 days -- the limit of 'almost expired')
	maintenanceContractExpiry =
		TestMaintenanceContractExpiryDate(/*purchase date         */ CURRENT_YEAR, CURRENT_MONTH - 1, /*day*/ 3,
										  /*months till MC expires*/ 2);

	// MC should be almost expired
	EXPECT_EQ(CLicenseThread::ALMOST_EXPIRED, maintenanceContractExpiry);
}


TEST_F(LicenseThreadFixture, DoWork_NotExpiredMC_ReturnsNotExpired)
{
	CLicenseThread::ExpiryType maintenanceContractExpiry;

	// User have bought the license almost 2 months ago:
	//    Date of purchase:      04/11/2012
	//    Maintenance contract:  2 Months
	//    Current Date:          25/12/2012
	// (Will expire within 10 days -- NOT almost expired)
	maintenanceContractExpiry =
		TestMaintenanceContractExpiryDate(/*purchase date         */ CURRENT_YEAR, CURRENT_MONTH - 1, /*day*/ 4,
										  /*months till MC expires*/ 2);

	// MC should NOT be expired
	EXPECT_EQ(CLicenseThread::NONE, maintenanceContractExpiry);
}



/******************************************************************************
 *
 *  Testing SendExpiryNotification().
 */

TEST_F(LicenseThreadFixture, SendExpiryNotification_expiredKeyExists_ReturnsTrue)
{
	EXPECT_TRUE( licenseThread->SendExpiryNotification(5 /*days ago*/) );
}


TEST_F(LicenseThreadFixture, SendExpiryNotification_expiredKeyDoesNotExist_ReturnsTrue)
{
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,StrEq("expired"),_,_,_,_))
		.WillOnce(Return(ERROR_FILE_NOT_FOUND));

	EXPECT_TRUE( licenseThread->SendExpiryNotification(5 /*days ago*/) );
}


TEST_F(LicenseThreadFixture, SendExpiryNotification_RegSetValueExFailure_ReturnsFalse)
{
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,StrEq("expired"),_,_,_,_))
		.WillOnce(Return(ERROR_FILE_NOT_FOUND));

	EXPECT_CALL(m_systemcalls_mock, RegSetValueEx(_,StrEq("expired"),_,_,_,_))
		.WillOnce(Return(1));

	EXPECT_FALSE( licenseThread->SendExpiryNotification(5 /*days ago*/) );
}



/******************************************************************************
 *
 *  Testing SendWarningNotification().
 */

TEST_F(LicenseThreadFixture, SendWarningNotification_expiredKeyExists_ReturnsTrue)
{
	EXPECT_TRUE( licenseThread->SendWarningNotification(5 /*days left*/) );
}


TEST_F(LicenseThreadFixture, SendWarningNotification_expiredKeyDoesNotExist_ReturnsTrue)
{
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,StrEq("expiring"),_,_,_,_))
		.WillOnce(Return(ERROR_FILE_NOT_FOUND));

	EXPECT_TRUE( licenseThread->SendWarningNotification(5 /*days left*/) );
}


TEST_F(LicenseThreadFixture, SendWarningNotification_RegSetValueExFailure_ReturnsFalse)
{
	EXPECT_CALL(m_systemcalls_mock, RegQueryValueEx(_,StrEq("expiring"),_,_,_,_))
		.WillOnce(Return(ERROR_FILE_NOT_FOUND));

	EXPECT_CALL(m_systemcalls_mock, RegSetValueEx(_,StrEq("expiring"),_,_,_,_))
		.WillOnce(Return(1));

	EXPECT_FALSE( licenseThread->SendWarningNotification(5 /*days left*/) );
}

#endif	// PERFORM_LICENSETHREAD_TESTS