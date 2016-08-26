#include "stdafx.h"
#include	"..\Factory.h"
#include "BillingManager.stub.h"
#include "EtherFaxLine.stub.h"

//Unit test factory that will return a BillingManager stub.

IBillingManager* BillingManagerFactory::NewBillingManager(LPCSTR ProviderCode,LPCSTR LicenseKey,BOOL bBillingDisabled,LPCSTR AccountName)
{
	return new BillingManagerStub();
}

//Unit Test factory that will return a new EtherFaxLine stub
IEtherFaxLine* NewLineFactory::CreateNewLine(LPCSTR PortName, LPCSTR TempDir, LPCSTR ServerUrl, IBillingManager* pBillingManager, CAThreadPool* pThreadPool, LPPORTINFOCALLBACK* portcallback)
{
	return new EtherFaxLineStub();
}

IAccountStatusCheck* AccountStatusCheckFactory::CreateAccountStatusCheck()
{
	return NULL;
}


