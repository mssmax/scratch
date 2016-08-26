#include "StdAfx.h"
#include "GFIDB.tests.h"
#include "GFIRULE.tests.h"

using testing::MatcherCast;

#ifdef PERFORM_GFIDB_TESTS

TEST_F(GFIDBFixture, Open_NullDBName_ReturnsOK)
{
	EXPECT_CALL(m_hlps_mock, doesfileexist(MatcherCast<LPCSTR>(_))).WillOnce(Return(TRUE));
	EXPECT_EQ(S_OK, gfiDB->Open(999, NULL));
	gfiDB->Close();
}


TEST_F(GFIDBFixture, Open_DBNameExists_ReturnsOK)
{
	EXPECT_EQ(S_OK, gfiDB->Open(999, _bstr_t("test_dir_exists")));
	gfiDB->Close();
}


TEST_F(GFIDBFixture, Open_DBNameDoesNotExist_ReturnsFailure)
{
	EXPECT_CALL(m_hlps_mock, doesdirexist(MatcherCast<LPCWSTR>(_))).WillOnce(Return(FALSE));
	EXPECT_EQ(HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND), gfiDB->Open(999, _bstr_t("test_dir_does_not_exist")));
}


TEST_F(GFIDBFixture, Close_DBOpened_ReturnsOK)
{
	gfiDB->Open(999, _bstr_t("test_dir_exists"));
	EXPECT_EQ(S_OK, gfiDB->Close());
}

/******************************************************************************/

/*
 *  Test methods' behaviour when the DB is not opened.
 */
TEST_F(GFIDBFixture, Close_DBNotOpened_ReturnsError)
{
	EXPECT_EQ(E_FAIL, gfiDB->Close());
}


TEST_F(GFIDBFixture, WriteRule_DBNotOpened_ReturnsError)
{
	CComObject<CGFIRULE> dummyRule;
	EXPECT_EQ(E_FAIL, gfiDB->Write(&dummyRule, VARIANT_TRUE /*bFailExists*/));
}


TEST_F(GFIDBFixture, ReadRule_DBNotOpened_ReturnsError)
{
	IGFIRULE	*rule = NULL;
	_bstr_t		ruleID("RuleID");

	EXPECT_EQ(E_FAIL, gfiDB->Read(ruleID, &rule));
}


TEST_F(GFIDBFixture, GetNewRule_DBNotOpened_ReturnsError)
{
	IGFIRULE	*rule = NULL;
	_bstr_t		ruleType("RuleType");

	EXPECT_EQ(E_FAIL, gfiDB->GetNewRule(ruleType, &rule));
}


TEST_F(GFIDBFixture, Delete_DBNotOpened_ReturnsError)
{
	_bstr_t		ruleID("RuleID");

	EXPECT_EQ(E_FAIL, gfiDB->Delete(ruleID));
}


TEST_F(GFIDBFixture, EnumRules_DBNotOpened_ReturnsError)
{
	_bstr_t			userID("UserID"),
					ruleType("RuleType");
	IEnumGFIRules	*ruleEnum = NULL;

	EXPECT_EQ(E_FAIL, gfiDB->EnumRules(userID, ruleType, &ruleEnum));
}


TEST_F(GFIDBFixture, SetValue_DBNotOpened_ReturnsError)
{
	_bstr_t	valueID("ValueID");
	VARIANT	value = {0};

	EXPECT_EQ(E_FAIL, gfiDB->SetValue(valueID, value));
}


TEST_F(GFIDBFixture, GetValue_DBNotOpened_ReturnsError)
{
	_bstr_t			valueID("ValueID");
	VARIANT			value = {0};
	
	EXPECT_EQ(E_FAIL, gfiDB->GetValue(valueID, &value));
}


TEST_F(GFIDBFixture, CommitValues_DBNotOpened_ReturnsError)
{
	EXPECT_EQ(E_FAIL, gfiDB->CommitValues());
}

/******************************************************************************/

/*
 *  Test methods' behaviour when we pass NULL parameters to methods and DB is
 *  now opened.
 */
TEST_F(GFIDBOpenedFixture, WriteRule_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->Write(NULL, VARIANT_TRUE /*bFailExists*/));
}

TEST_F(GFIDBOpenedFixture, ReadRule_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->Read(NULL,              NULL));
	EXPECT_EQ(E_INVALIDARG, gfiDB->Read(_bstr_t("RuleID"), NULL));
}


TEST_F(GFIDBOpenedFixture, GetNewRule_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->GetNewRule(NULL,                NULL));
	EXPECT_EQ(E_INVALIDARG, gfiDB->GetNewRule(_bstr_t("RuleType"), NULL));
}


TEST_F(GFIDBOpenedFixture, Delete_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->Delete(NULL));
}


TEST_F(GFIDBOpenedFixture, EnumRules_NullParameters_ReturnsError)
{
	// UserID and RuleType variables are passed to and only used by GFIRuleDB.
	// Therefore testing these parameters is beyond our scope.
	EXPECT_EQ(E_INVALIDARG, gfiDB->EnumRules(_bstr_t("UserID"), _bstr_t("RuleType"), NULL));
}

TEST_F(GFIDBFixture, SetValue_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->SetValue(NULL, VARIANT()));
}

TEST_F(GFIDBOpenedFixture, GetValue_NullParameters_ReturnsError)
{
	EXPECT_EQ(E_INVALIDARG, gfiDB->GetValue(NULL,               (VARIANT*)1));
	EXPECT_EQ(E_INVALIDARG, gfiDB->GetValue(_bstr_t("ValueID"), NULL));
}


/******************************************************************************/

/*
 *  DB is now opened.
 */
TEST_F(GFIDBOpenedFixture, WriteRule_DBOpened_ReturnsOK)
{
	CComObject<CGFIRULE> dummyRule;

	EXPECT_CALL(rulesDBMock, raw_Write(_,_)).WillOnce(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->Write(&dummyRule, VARIANT_TRUE /*bFailExists*/));
}


TEST_F(GFIDBOpenedFixture, ReadRule_DBOpened_ReturnsOK)
{
	IGFIRULE	*rule = NULL;
	_bstr_t		ruleID("RuleID");

	EXPECT_CALL(rulesDBMock, raw_Read(_,_)).WillOnce(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->Read(ruleID, &rule));	// rule: it is a pointer to CComObject<CGFIRULE>
	EXPECT_TRUE(rule != NULL);

	rule->Release();
}


TEST_F(GFIDBOpenedFixture, GetNewRule_DBOpened_ReturnsOK)
{
	IGFIRULE	*rule = NULL;
	_bstr_t		ruleType("RuleType");

	EXPECT_CALL(rulesDBMock, raw_GetNewRule(_,_)).WillOnce(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->GetNewRule(ruleType, &rule));	// rule: it is a pointer to CComObject<CGFIRULE>

	rule->Release();
}


TEST_F(GFIDBOpenedFixture, Delete_DBOpened_ReturnsOK)
{
	_bstr_t		ruleID("RuleID");

	EXPECT_CALL(rulesDBMock, raw_Delete(_)).WillOnce(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->Delete(ruleID));
}


TEST_F(GFIDBOpenedFixture, EnumRules_DBOpened_ReturnsOK)
{
	_bstr_t			userID("UserID"),
					ruleType("RuleType");
	IEnumGFIRules	*ruleEnum = NULL;

	EXPECT_CALL(rulesDBMock, raw_EnumRules(_,_,_)).WillOnce(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->EnumRules(userID, ruleType, &ruleEnum));
	EXPECT_TRUE(ruleEnum != NULL);

	ruleEnum->Release();
}


TEST_F(GFIDBOpenedFixture, SetValue_DBOpened_ReturnsOK)
{
	_bstr_t	valueID("ValueID");
	VARIANT	value = {0};

	value.vt     = VT_INT;
	value.intVal = 1945;
	EXPECT_EQ(S_OK, gfiDB->SetValue(valueID, value));
}


/*
 *  Fake implementation of IGFIRULEDB.
 */
class IGFIRULEDBFake
{
public:
	NiceMock<IGFIRULEMock>	ruleMock;
	static const int		VALUE = 1945;

	HRESULT __stdcall raw_Read(BSTR bstrRuleid,  /*[out,retval]*/ GFIMDB::IGFIRULE **lppRule)
	{
		*lppRule = &ruleMock;
		return S_OK;
	}

	/*
	 *  GetValue() will simulate a read from the database and will return the value
	 *  of 1945 that is of type VT_INT.
	 */
	HRESULT __stdcall raw_GetValue(BSTR bstrValueID, /*[out,retval]*/ VARIANT *value)
	{
		// if bstrValueID == "value"
		if (VARCMP_EQ == VarBstrCmp(bstrValueID, _bstr_t("value"), GetUserDefaultLCID(), 0)) {
			value->vt     = VT_INT;
			value->intVal = VALUE;
			return S_OK;
		}

		// if bstrValueID == "type" (type of value)
		else if (VARCMP_EQ == VarBstrCmp(bstrValueID, _bstr_t("type"), GetUserDefaultLCID(), 0)) {
			value->vt     = VT_INT;
			value->intVal = VT_INT;
			return S_OK;
		}

		return E_FAIL;
	}
};


/*
 *  GetValue() will return the value of ValueID, that is 1945 (of type INT).
 *  The value is un-cached therefore it needs to read it from the DB.
 */
TEST_F(GFIDBOpenedFixture, GetValue_UnCached_ReturnsOK)
{
	_bstr_t			valueID("ValueID");
	VARIANT			value = {0};
	IGFIRULEDBFake	rulesDBFake;

	EXPECT_CALL(rulesDBMock, raw_Read(_,_))
		.WillOnce(Invoke(&rulesDBFake, &IGFIRULEDBFake::raw_Read));
	EXPECT_CALL(rulesDBFake.ruleMock, raw_GetValue(_,_))
		.Times(2)
		.WillRepeatedly(Invoke(&rulesDBFake, &IGFIRULEDBFake::raw_GetValue));
	EXPECT_EQ(S_OK, gfiDB->GetValue(valueID, &value));

	// make sure it returned the correct value
	EXPECT_EQ(value.intVal, IGFIRULEDBFake::VALUE);
}


/*
 *  First time get value is invoke, the returned data is cached in memory.  This
 *  test will make sure that the returned data of the first read equals that of
 *  the second read (cached) equals that of IGFIRULEDBFake::VALUE (1945).
 */
TEST_F(GFIDBOpenedFixture, GetValue_Cached_ReturnsOK)
{
	_bstr_t			valueID("ValueID");
	VARIANT			value1 = {0},
					value2 = {0};
	IGFIRULEDBFake	rulesDBFake;

	EXPECT_CALL(rulesDBMock, raw_Read(_,_))
		.WillRepeatedly(Invoke(&rulesDBFake, &IGFIRULEDBFake::raw_Read));
	EXPECT_CALL(rulesDBFake.ruleMock, raw_GetValue(_,_))
		.Times(2)
		.WillRepeatedly(Invoke(&rulesDBFake, &IGFIRULEDBFake::raw_GetValue));
	
	EXPECT_EQ(S_OK, gfiDB->GetValue(valueID, &value1));
	EXPECT_EQ(S_OK, gfiDB->GetValue(valueID, &value2));

	// make sure it returned the correct value
	EXPECT_EQ(value1.intVal, IGFIRULEDBFake::VALUE);
	EXPECT_EQ(value2.intVal, IGFIRULEDBFake::VALUE);
}


TEST_F(GFIDBOpenedFixture, CommitValues_DBOpened_ReturnsOK)
{
	_bstr_t			valueID;
	VARIANT			value = {0};
	char			tmp[10];
	IGFIRULEDBFake	rulesDBFake;
	const int		ITEMS_SAVED = 3;	// we are going to save x items

	value.vt = VT_INT;

	// set x values (these are stored in cache)
	for (int i = 0; i < ITEMS_SAVED; i++)
	{
		_snprintf(tmp, 10, "ValueID%d", i);
		valueID      = tmp;
		value.intVal = 1945 + i;
		gfiDB->SetValue(valueID, value);
	}

	// commit these x items
	EXPECT_CALL(rulesDBMock, raw_Read(_,_))
		.Times(ITEMS_SAVED)
		.WillRepeatedly(Invoke(&rulesDBFake, &IGFIRULEDBFake::raw_Read));
	EXPECT_CALL(rulesDBMock, raw_Write(_,_))
		.Times(ITEMS_SAVED)
		.WillRepeatedly(Return(S_OK));
	EXPECT_EQ(S_OK, gfiDB->CommitValues());
}

#endif	// PERFORM_GFIDB_TESTS