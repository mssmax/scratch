#pragma once
#include "StdAfx.h"
#include "UnitTesting.h"
#include <GFIDB.h>


struct IGFIDBMock : public GFIMDB::IGFIDB
{
	IUNKNOWN_MOCK();

	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Open, HRESULT(long lAccess, BSTR bstrParams));
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Close,HRESULT());
};


struct IGFIDB3Mock : public GFIMDB::IGFIDB3
{
	IUNKNOWN_MOCK();

	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Open,  HRESULT(long lAccess, BSTR bstrParams));
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Close, HRESULT());

	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetDBValue, HRESULT(/*[in]*/ long lPropertyID, /*[out,retval]*/ VARIANT *pVal));

	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetRuleTypeIdx, HRESULT(/*[in]*/ LPWSTR ruletypename, /*[out,retval]*/ unsigned long *pIdx));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetUSN, HRESULT(/*[in]*/ unsigned long dwIdx, /*[out,retval]*/ unsigned long *dwUSN));
};


struct IGFIRULEDBMock : public GFIMDB::IGFIRULEDB
{
	NiceMock<IGFIDBMock>	gfiDBMock;
	NiceMock<IGFIDB3Mock>	gfiDB3Mock;

	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, AddRef,  ULONG());
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, Release, ULONG());

	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Read,      HRESULT(BSTR bstrRuleid, /*[out,retval]*/ GFIMDB::IGFIRULE **lppRule));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_EnumRules, HRESULT(BSTR bstrUserid, BSTR bstrRuleType, /*[out,retval]*/ GFIMDB::IEnumGFIRules **lppRuleEnum));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Write,     HRESULT(GFIMDB::IGFIRULE *lpRule, VARIANT_BOOL bFailExists));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Delete,    HRESULT(BSTR bstrRuleid));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetNewRule,HRESULT(BSTR bstrRuleType, /*[out,retval]*/ GFIMDB::IGFIRULE **lppRule));

	// fake QueryInterface()
	HRESULT STDMETHODCALLTYPE QueryInterface(/*in*/ REFIID riid, /*out*/ void **ppvObject)
	{
		if (riid == __uuidof(GFIMDB::IGFIDB)) {
			*ppvObject = &gfiDBMock;
			return S_OK;
		}

		else if (riid == __uuidof(GFIMDB::IGFIDB3)) {
			*ppvObject = &gfiDB3Mock;
			return S_OK;
		}

		return E_FAIL;
	}
};



/*
 *  A derived class of CGFIDB used for unit testing.
 */
struct CGFIDB_ex : public CGFIDB
{
private:
	NiceMock<IGFIDB3Mock>	gfiDB3Mock;

public:
	// com stuff
	HRESULT FinalConstruct()
	{	return S_OK;	}

	void FinalRelease()
	{}

	// unit-testing
	GFIMDB::IGFIDB3* CreateIGFIDB3PtrInstance(/*out*/HRESULT *hresult)
	{
		if (hresult != NULL)
			hresult = S_OK;

		return &gfiDB3Mock;
	}
};


/*
 *  Fixture of CGFIDB.
 */
class GFIDBFixture : public mfgfixture
{
protected:
	CComObject<CGFIDB_ex>	*gfiDB;
	IGFIRULEDBMock			rulesDBMock;

public:
	virtual void SetUp()
	{
		ASSERT_HRESULT_SUCCEEDED( CComObject<CGFIDB_ex>::CreateInstance(&gfiDB) );
		gfiDB->AddRef();

		EXPECT_CALL(rulesDBMock, AddRef());
		gfiDB->SetConfigDB( &rulesDBMock );
	}

	virtual void TearDown()
	{
		EXPECT_CALL(rulesDBMock, Release());
		gfiDB->Release();
	}
};



/*
 *  Fixture of CGFIDB that opens the DB.
 */
struct GFIDBOpenedFixture : public GFIDBFixture
{
	void SetUp()
	{
		GFIDBFixture::SetUp();
		gfiDB->Open(999, _bstr_t("test_dir_exists"));
	}

	void TearDown()
	{
		gfiDB->Close();
		GFIDBFixture::TearDown();
	}
};