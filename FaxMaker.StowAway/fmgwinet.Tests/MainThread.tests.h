#pragma once
#include "StdAfx.h"
#include "UnitTesting.h"
#include <MainThread.h>
#include <mfg_afx.h>

#define EXCHVERSION		"Qwerty"
#define EXCHVERSION_W	L"Qwerty"

/*
 *  Extended CMainThread:  the new class that will be unit tested.
 */
struct CMainThreadEx : public CMainThread
{
	CMainThreadEx(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface *statusInterface)
		: CMainThread(pMtaConfig, pUtil, statusInterface)
	{}

	// Overridden method
	IFmgwinetApp* GetFmgwinetApp()
	{	return GetFmgwinetAppMock();	}

	// Set m_restartthreadstarted.  Used for unit testing.
	void SetRestartThreadStarted(BOOL val)
	{	m_restartthreadstarted = val;	}

	// getters created for unit testing
	int GetItemsInQueue()
	{	return m_itemsInQueue;			}
	
	int GetItemsOutPendingQueue()
	{	return m_itemsOutPendingQueue;	}
};



HRESULT STDMETHODCALLTYPE Fake_IADs_Get(BSTR bstrName, /*[out]*/ VARIANT *pvProp)
{
	if (CComBSTR(L"configurationNamingContext") == CComBSTR(bstrName)) {
		pvProp->bstrVal = L"testX";
		return S_OK;
	}

	return E_INVALIDARG;
}


HRESULT STDMETHODCALLTYPE Fake_IDirectorySearch_GetColumn(ADS_SEARCH_HANDLE hSearchResult, LPWSTR szColumnName, /*[out]*/ PADS_SEARCH_COLUMN pSearchColumn)
{
	pSearchColumn->pADsValues = new ADSVALUE;
	pSearchColumn->pADsValues->CaseExactString = EXCHVERSION_W;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Fake_IDirectorySearch_FreeColumn(PADS_SEARCH_COLUMN pSearchColumn)
{
	if (!pSearchColumn)
		return E_INVALIDARG;
	
	delete pSearchColumn->pADsValues;
	return S_OK;
}


/*
 *  Fixture
 */
class MainThreadFixture : public mfgfixture
{
public:
	CMainThreadEx					*mainThread;
	NiceMock<IFmgwinetAppMock>		fmgwinetAppMock;
	NiceMock<CStatusInterfaceMock>	statusInterfaceMock;
	NiceMock<CMtaConfigMock>		mtaConfigMock;
	NiceMock<IUtilitiesMock>		utilitiesMock;
	NiceMock<IDirectorySearchMock>	directorySearchMock;
	NiceMock<IADsMock>				aDsMock;
	NiceMock<CFileFindMock>			fileFindMock;
	
public:
	void SetUp()
	{
		SetFmgwinetAppMock(&fmgwinetAppMock);
		mainThread = new CMainThreadEx(&mtaConfigMock, &utilitiesMock, &statusInterfaceMock);

		SetUtilitiesMock(&utilitiesMock);
		SetIDirectorySearchMock(&directorySearchMock);
		SetIADsMock(&aDsMock);
		SetCFileFindMock(&fileFindMock);

		ON_CALL(GetSystemCallsMock(), CreateThread(_,_,_,_,_,_)).WillByDefault(Return((HANDLE)0x99));
		ON_CALL(aDsMock, Get(_,_)).WillByDefault(Invoke(Fake_IADs_Get));
		ON_CALL(directorySearchMock, GetColumn(_,_,_)).WillByDefault(Invoke(Fake_IDirectorySearch_GetColumn));
		ON_CALL(directorySearchMock, FreeColumn(_)).WillByDefault(Invoke(Fake_IDirectorySearch_FreeColumn));
	}

	void TearDown()
	{	delete mainThread;	}

protected:
	void PrepareAddTelemetyData();
};