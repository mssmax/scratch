#include "stdafx.h"
#include <propbag.h>

extern int TestMe(LPCSTR name);

DECLARE_MOCK_FACTORY_IMPL(CPropBag);

namespace testing
{
	class CTests : public Test
	{
	public:
		NiceMock<CPropBagMock> mock;

		void SetUp()
		{
			EXPECT_CALL(CPropBag::m_sFactory, Create()).WillOnce(Return(&mock));
		}

		void TearDown()
		{
		}
	};

	TEST_F(CTests, TestMe_SetsOneToFirst)
	{
		// catch-all expectation
		EXPECT_CALL(mock, Set(_, _)).Times(AnyNumber());
		EXPECT_CALL(mock, Set(1, StrCaseEq(L"first")));
		ON_CALL(mock, Get(_)).WillByDefault(Return(L""));

		TestMe("something");
	}

	TEST_F(CTests, TestMe_SetsTwoToLast)
	{
		// catch-all expectation
		EXPECT_CALL(mock, Set(_, _)).Times(AnyNumber());
		EXPECT_CALL(mock, Set(2, StrCaseEq(L"last")));
		ON_CALL(mock, Get(_)).WillByDefault(Return(L""));

		TestMe("something");
	}
/**/
	TEST_F(CTests, TestMe_ReturnsFirstOnOne)
	{
		EXPECT_CALL(mock, Get(_)).Times(AnyNumber());
		ON_CALL(mock, Get(1))
			.WillByDefault(Return(L"first"));
		ON_CALL(mock, Get(2))
			.WillByDefault(Return(L"last"));
		ON_CALL(mock, Get(3))
			.WillByDefault(Return(L"third"));

		ASSERT_EQ(1, TestMe("something"));
	}

	TEST_F(CTests, TestMe_ReturnsGarbageOnOne)
	{
		EXPECT_CALL(mock, Get(_)).Times(AnyNumber());
		EXPECT_CALL(mock, Get(1))
			.WillOnce(Return(L"garbage"));
		ON_CALL(mock, Get(2)).WillByDefault(Return(L"last"));

		ASSERT_EQ(0, TestMe("something"));
	}

	TEST_F(CTests, TestMe_ReturnsLastOnTwo)
	{
		EXPECT_CALL(mock, Get(_)).Times(AnyNumber());

		ON_CALL(mock, Get(1))
			.WillByDefault(Return(L"first"));
		ON_CALL(mock, Get(2))
			.WillByDefault(Return(L"last"));
		ON_CALL(mock, Get(3))
			.WillByDefault(Return(L"third"));

		ASSERT_EQ(1, TestMe("something"));
	}

	TEST_F(CTests, TestMe_ReturnsGarbageOnTwo)
	{
		ON_CALL(mock, Get(1)).WillByDefault(Return(L"first"));
		EXPECT_CALL(mock, Get(_)).Times(AnyNumber());
		EXPECT_CALL(mock, Get(2))
			.WillRepeatedly(Return(L"garbage"));

		ASSERT_EQ(0, TestMe("something"));
	}
	/**/
}