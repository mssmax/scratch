#include "StdAfx.h"
#include "EnumGFIRules.tests.h"

#ifdef PERFORM_ENUMGFIRULES_TESTS

TEST_F(CEnumGFIRulesFixture, Clone_ReturnsOK)
{
	IEnumGFIRules	*pEnumRules = NULL;

	EXPECT_CALL(mockedEnumRules, raw_Clone(_)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( enumRules->Clone(&pEnumRules) );
}


TEST_F(CEnumGFIRulesFixture, Reset_ReturnsOK)
{
	EXPECT_CALL(mockedEnumRules, raw_Reset()).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( enumRules->Reset() );
}


TEST_F(CEnumGFIRulesFixture, Skip_ReturnsOK)
{
	EXPECT_CALL(mockedEnumRules, raw_Skip(_)).WillOnce(Return(S_OK));
	EXPECT_HRESULT_SUCCEEDED( enumRules->Skip(19) );
}


TEST_F(CEnumGFIRulesFixture, Next_RulesList_ReturnsOK)
{
	ULONG		rulesFetched;
	const char	TOTAL_RULES = 5;
	IGFIRULE	*rule[TOTAL_RULES] = {0};

	EXPECT_CALL(mockedEnumRules, raw_Next(1, _, NULL))
		.Times(TOTAL_RULES)
		.WillRepeatedly(Invoke(&mockedEnumRules, &IEnumGFIRulesMock::raw_Next_Fake));
	
	EXPECT_HRESULT_SUCCEEDED( enumRules->Next(TOTAL_RULES, rule, &rulesFetched) );
	EXPECT_EQ(TOTAL_RULES, rulesFetched);
}


TEST_F(CEnumGFIRulesFixture, Next_InvalidParameters_HandlesThemCorrectly)
{
	const char	TOTAL_RULES = 5;
	
	EXPECT_EQ(ERROR_INVALID_PARAMETER, enumRules->Next(0,           (IGFIRULE**)1,    (ULONG*)1));
	EXPECT_EQ(ERROR_INVALID_PARAMETER, enumRules->Next(TOTAL_RULES, (IGFIRULE**)NULL, (ULONG*)1));

	// the 3rd parameter (pceltFetched) can be NULL:  Next() method will just
	// ignore it
	IGFIRULE	*rule[TOTAL_RULES] = {0};

	EXPECT_CALL(mockedEnumRules, raw_Next(1, _, NULL))
		.Times(TOTAL_RULES)
		.WillRepeatedly(Invoke(&mockedEnumRules, &IEnumGFIRulesMock::raw_Next_Fake));
	EXPECT_HRESULT_SUCCEEDED(enumRules->Next(TOTAL_RULES, rule, NULL));
}

#endif	// PERFORM_ENUMGFIRULES_TESTS