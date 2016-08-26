#include "StdAfx.h"
#include "GFIRULE.tests.h"

#ifdef PERFORM_GFIRULE_TESTS

TEST_F(CGFIRULEFixture, AddUser_UserID_ReturnsOK)
{
	_bstr_t userID("userID");

	EXPECT_CALL(mockedRule, raw_AddUser(_)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( rule->AddUser(userID) );
}


TEST_F(CGFIRULEFixture, DeleteUser_UserID_ReturnsOK)
{
	_bstr_t userID("userID");

	EXPECT_CALL(mockedRule, raw_DeleteUser(_)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( rule->DeleteUser(userID) );
}


TEST_F(CGFIRULEFixture, EnumUser_ValidParameters_ReturnsOK)
{
	IEnumUsers *lpUserEnum;

	EXPECT_CALL(mockedRule, raw_EnumUsers(_)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( rule->EnumUsers(&lpUserEnum) );
}


TEST_F(CGFIRULEFixture, SetValue_APPRINTA4_ReturnsOK)
{
	_bstr_t		bstrValueID(VALUEID_AP_PRINT_A4);
	VARIANT		value    = {0};
	CBase64Mock	*cbase64 = (CBase64Mock	*) GetICBase64();

	EXPECT_CALL(*cbase64,   Encode(_, _, MatcherCast<LPWSTR>(_), _)).WillOnce(Return(GFICRYPTO_SUCCESS));
	EXPECT_CALL(mockedRule, raw_SetValue(_, _)).WillOnce(Return(S_OK));

	EXPECT_HRESULT_SUCCEEDED( rule->SetValue(bstrValueID, value) );
}


TEST_F(CGFIRULEFixture, SetValue_ETHERFAXACCOUNT_ReturnsOK)
{
	_bstr_t		bstrValueID(VALUEID_ETHERFAX_ACCOUNT);
	VARIANT		value    = {0};

	EXPECT_CALL(mockedRule, raw_SetValue(_, _)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( rule->SetValue(bstrValueID, value) );
}


TEST_F(CGFIRULEFixture, GetValue_ETHERFAXACCOUNT_ReturnsOK)
{
	_bstr_t		bstrValueID(VALUEID_ETHERFAX_ACCOUNT);
	VARIANT		value    = {0};

	EXPECT_CALL(mockedRule, raw_GetValue(_, _)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( rule->GetValue(bstrValueID, &value) );
}


/*
 *  Fake raw_GetValue().  Allocates a bstr and returns S_OK.
 */
HRESULT __stdcall raw_GetValue_Fake(BSTR bstrValueID, /*[out,retval]*/ VARIANT *value)
{
	if (!value)	return E_FAIL;
	value->bstrVal = SysAllocString(L"return str");
	return S_OK;
}


TEST_F(CGFIRULEFixture, GetValue_APPRINTA4_ReturnsOK)
{
	_bstr_t		bstrValueID(VALUEID_AP_PRINT_A4);
	VARIANT		value    = {0};
	CBase64Mock	*cbase64 = (CBase64Mock	*) GetICBase64();

	EXPECT_CALL(mockedRule, raw_GetValue(_, _)).WillOnce(Invoke(raw_GetValue_Fake));
	EXPECT_CALL(*cbase64,   Decode(MatcherCast<LPCWSTR>(_), _, _)).WillOnce(Return(GFICRYPTO_SUCCESS));

	value.byref = new autoprinter_t;
	EXPECT_HRESULT_SUCCEEDED( rule->GetValue(bstrValueID, &value) );
	delete value.byref;
	SysFreeString(value.bstrVal);
}



/*
 *  Check for NULL parameters.
 */
TEST_F(CGFIRULEFixture, AddUser_InvalidParameter_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, rule->AddUser(NULL));
}


TEST_F(CGFIRULEFixture, DeleteUser_InvalidParameter_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, rule->DeleteUser(NULL));
}


TEST_F(CGFIRULEFixture, EnumUser_InvalidParameter_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, rule->EnumUsers(NULL));
}


TEST_F(CGFIRULEFixture, SetValue_InvalidParameter_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, rule->SetValue(NULL, VARIANT()));
}


TEST_F(CGFIRULEFixture, GetValue_InvalidParameter_ReturnsError)
{
	VARIANT		value = {0};

	EXPECT_EQ(E_INVALIDARG, rule->GetValue(NULL,              &value));
	EXPECT_EQ(E_INVALIDARG, rule->GetValue(_bstr_t("ValueID"), NULL));
}

#endif	// PERFORM_GFIRULE_TESTS