#pragma once
#include "StdAfx.h"
#include "UnitTesting.h"
#include <EnumGFIRules.h>
#include "GFIRULE.tests.h"

/*
 *  Mock of IEnumGFIRules.    There is no need to mock wrapper methods (e.g.
 *  Clone()) as they are just wrapper methods generated in gfimdb2.tlh file.
 */
struct IEnumGFIRulesMock : public GFIMDB::IEnumGFIRules
{
	IUNKNOWN_MOCK();

    // Raw methods provided by interface
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Clone, HRESULT(IEnumGFIRules **ppenum));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Next,  HRESULT(/*[in]*/ unsigned long celt, /*[out]*/ GFIMDB::IGFIRULE **rgelt, /*[in,out]*/ unsigned long *pceltFetched));
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Reset, HRESULT());
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_Skip,  HRESULT(unsigned long celt));

	/*
	 *  Fakes raw_Next().  Returns 1 instance of IGFIRULE (IGFIRULEMock).
	 */
	HRESULT STDMETHODCALLTYPE raw_Next_Fake(/*[in]*/     unsigned long celt, 
											/*[out]*/    GFIMDB::IGFIRULE **rgelt,
											/*[in,out]*/ unsigned long *pceltFetched)
	{
		if (celt != 1)
			return E_FAIL;

		*rgelt = new NiceMock<IGFIRULEMock>;
		return S_OK;
	}
};


/*
 *  Fixture of CEnumGFIRules.
 */
class CEnumGFIRulesFixture : public testing::Test
{
protected:
	CComObject<CEnumGFIRules>	*enumRules;
	NiceMock<IEnumGFIRulesMock>	mockedEnumRules;

public:
	void SetUp()
	{
		ASSERT_HRESULT_SUCCEEDED( CComObject<CEnumGFIRules>::CreateInstance(&enumRules) );
		enumRules->AddRef();

		enumRules->setenum( &mockedEnumRules );
	}

	void TearDown()
	{	enumRules->Release();	}
};