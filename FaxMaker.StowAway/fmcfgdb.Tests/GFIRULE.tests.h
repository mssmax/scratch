#pragma once
#include "StdAfx.h"
#include "UnitTesting.h"
#include <CBase64.h>

/*
 *  Mock of CBase64.
 */
struct CBase64Mock : ICBase64
{
	MOCK_METHOD3(Decode, int(LPCSTR szBase64, BYTE* lpOutputBuf, DWORD* cbOutputBuf));
	MOCK_METHOD3(Decode, int(LPCWSTR szBase64, BYTE* lpOutputBuf, DWORD* cbOutputBuf));
	MOCK_METHOD4(Encode, int(BYTE* lpBuf, DWORD lpszBuf, LPWSTR szBase64, DWORD* cchBase64));
	MOCK_METHOD4(Encode, int(BYTE* lpBuf, DWORD lpszBuf, LPSTR szBase64, DWORD* cchBase64));
};


/*
 *  Mock of IGFIRULE.  There is no need to mock wrapper methods (e.g. SetValue())
 *  as they are just wrapper methods generated in gfimdb2.tlh file.
 */
struct IGFIRULEMock : public GFIMDB::IGFIRULE	// defined at gfimdb2.tlh
{
	IUNKNOWN_MOCK();

	// Raw methods provided by interface
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_SetValue,  HRESULT(BSTR bstrValueID, VARIANT value));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetValue,  HRESULT(BSTR bstrValueID, /*[out,retval]*/ VARIANT *value));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_GetValueEx,HRESULT(BSTR bstrValueID, GUID *iid, /*[out,retval]*/ VARIANT *value));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_EnumUsers, HRESULT(/*[out,retval]*/ GFIMDB::IEnumUsers **lppUserEnum));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_AddUser,   HRESULT(BSTR bstrUserid));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, raw_DeleteUser,HRESULT(BSTR bstrUserid));
};



/*
 *  Fixture of CGFIRULE.
 */
#include <GFIRULE.h>

class CGFIRULEFixture : public testing::Test
{
protected:
	CComObject<CGFIRULE>	*rule;
	NiceMock<IGFIRULEMock>	mockedRule;

public:
	void SetUp()
	{
		ASSERT_HRESULT_SUCCEEDED( CComObject<CGFIRULE>::CreateInstance(&rule) );
		rule->AddRef();

		rule->setrule( &mockedRule );
	}

	void TearDown()
	{	rule->Release();	}
};