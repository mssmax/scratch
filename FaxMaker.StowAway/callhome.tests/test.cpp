#include "StdAfx.h"
#include "InterprocessMaps.h"
using namespace std;
#include "mockobjects.h"
#include "callhome-internal.h"
#include "fakeobjects.h"
#include "fixtures.h"
using testing::Return;
using testing::AllOf;
using testing::Gt;
using testing::Le;
using ::testing::_;


class ChEnvironment : public ::testing::Environment {
public:
	virtual ~ChEnvironment() {}
	virtual void SetUp() {
		ASSERT_TRUE(SUCCEEDED(::CoInitializeEx(NULL, COINIT_MULTITHREADED)));
	}
	virtual void TearDown() {
		::CoUninitialize();
	}
};


ACTION_P2(ReturnFromAsyncCall,retVal,hEvent)
{
	SetEvent(hEvent);
	return retVal;
}


namespace callhome {
	
	//=========================== CCallHomeCollector tests ===========================

	TEST_F(CCallHomeCollectorBasicTest, Init_withIntsAndStrings) {
		CallItemsInitializer holder;
		holder.Set("aStringItem","a string value");
		holder.Set("anIntItem",3);
		o.Init(&holder);

		ASSERT_STREQ("aStringItem", o.GetStrings().begin()->first);
		EXPECT_STREQ("a string value", o.GetStrings().begin()->second);
		ASSERT_STREQ("anIntItem", o.GetInts().begin()->first);
		EXPECT_EQ(3, o.GetInts().begin()->second);
	}

	TEST_F(CCallHomeCollectorBasicTest, Init_withCounters) {
		CallItemsInitializer holder;
		holder.InitCounter("aCounter");
		o.Init(&holder);

		ASSERT_STREQ("aCounter", o.GetCounters().begin()->first);
		EXPECT_EQ(0, o.GetCounters().begin()->second);
	}

	TEST_F(CCallHomeCollectorIntermediateTest, SetValues) {
		CallItemsHolder holder;
		holder.Set("aStringItem","a string value");
		holder.Set("anIntItem",3);
		o.SetValues(holder);

		ASSERT_STREQ("aStringItem", o.GetStrings().begin()->first);
		EXPECT_STREQ("a string value", o.GetStrings().begin()->second);
		ASSERT_STREQ("anIntItem", o.GetInts().begin()->first);
		EXPECT_EQ(3, o.GetInts().begin()->second);
	}

	TEST_F(CCallHomeCollectorBasicTest, SetValues_withReset) {
		CallItemsInitializer initializer;
		initializer.Set("aGroup.aNumber", 1);
		o.Init(&initializer);

		CallItemsHolder items;
		items.Set("aGroup.anotherNumber", 2);
		items.Reset("aGroup",callhome::ResetFlags::RESET_ALL_BRANCH | callhome::ResetFlags::RESET_REMOVING);
		o.SetValues(items);

		ASSERT_EQ(1, o.GetInts().size());
		EXPECT_EQ(2, o.GetInts().begin()->second);
		ASSERT_STREQ("aGroup.anotherNumber", o.GetInts().begin()->first);
	}

	TEST_F(CCallHomeCollectorIntermediateTest, SetValue_ints) {
		o.SetValue("anIntItem",3);
		o.SetValue("anotherIntItem",4);

		ASSERT_EQ(o.GetInts().size(), 2);
		DWORD intValue;
		ASSERT_TRUE(o.GetInts().GetValue("anIntItem", intValue));
		ASSERT_EQ(intValue, 3);
		ASSERT_TRUE(o.GetInts().GetValue("anotherIntItem", intValue));
		ASSERT_EQ(intValue, 4);
	}

	TEST_F(CCallHomeCollectorIntermediateTest, SetValue_strings) {
		o.SetValue("aStringItem","value1");
		o.SetValue("anotherStringItem","value2");

		ASSERT_EQ(o.GetStrings().size(), 2);
		string_value_t stringValue;
		ASSERT_TRUE(o.GetStrings().GetValue("aStringItem", stringValue));
		ASSERT_STREQ(stringValue, "value1");
		ASSERT_TRUE(o.GetStrings().GetValue("anotherStringItem", stringValue));
		ASSERT_STREQ(stringValue, "value2");
	}

	TEST_F(CCallHomeCollectorIntermediateTest, IncreaseCounter) {
		DWORD counterValue;
		o.IncreaseCounter("aCounter", 1);
		ASSERT_TRUE(o.GetCounters().GetValue("aCounter", counterValue));
		ASSERT_EQ(1, counterValue);
		o.IncreaseCounter("aCounter", 2);
		ASSERT_TRUE(o.GetCounters().GetValue("aCounter", counterValue));
		ASSERT_EQ(3, counterValue);
	}


	TEST_F(CCallHomeCollectorIntermediateTest, InitDynamicCountersGroup) {
		DYNAMIC_COUNTERS_GROUP_HANDLE hGroup;
		ASSERT_TRUE(o.InitDynamicCountersGroup(&hGroup ,"aGroup", 0, NULL, LOCKTYPE_MODULE));
		DynamicCountersGroupState groupState;
		ASSERT_TRUE(o.GetDynamicCountersGroups().GetValue("aGroup.", groupState));
		ASSERT_FALSE(o.GetInstanceDynamicCountersGroups().find("aGroup.") == o.GetInstanceDynamicCountersGroups().end())
			  << "group just added not found in the instance DynamicCountersGroups map";
	}


	TEST_F(CCallHomeCollectorFinalTest, IncreaseDynamicCounter) {
		o.IncreaseDynamicCounter(hGroup, "aDynCounter", 1);
		DWORD counterValue;
		ASSERT_TRUE(pGroup->counters.GetValue("aDynCounter", counterValue));
		ASSERT_EQ(1, counterValue);
	}

	TEST_F(CCallHomeCollectorBasicTest, CheckPatches_firstTime) {
		EXPECT_CALL(m_patchCheckerMock, Check())
			.WillOnce(Return(S_OK));
		EXPECT_CALL(*o.GetHttpMan(), SendAsyncRequest(_, _, _, _))
			.Times(0);

		ASSERT_TRUE(o.Init(NULL));
		ASSERT_TRUE(o.CheckPatches(NULL, NULL, NULL));
	}

	TEST_F(CCallHomeCollectorBasicTest, CheckPatches_afterOneDayOfInitWithAutocheckDisabled) {
		EXPECT_CALL(m_patchCheckerMock, Check())
			.WillOnce(Return(S_OK));
		EXPECT_CALL(*o.GetHttpMan(), SendAsyncRequest(_, _, _, _))
			.WillOnce(Return(S_OK));

		o.dynconfAutocheckEnabled=false;
		o.SetTime(20, 0, 0);

		ASSERT_TRUE(o.Init(NULL));

		o.AddDaysToTime(1);


		ASSERT_TRUE(o.CheckPatches(NULL, NULL, NULL));
	}

	TEST_F(CCallHomeCollectorBasicTest, CheckPatches_afterTwoDaysOfInitWithAutocheckDisabled) {
		EXPECT_CALL(m_patchCheckerMock, Check())
			.WillOnce(Return(S_OK));
		EXPECT_CALL(*o.GetHttpMan(), SendAsyncRequest(_, _, _, _))
			.Times(0);

		o.dynconfAutocheckEnabled=false;
		o.SetTime(20, 0, 0);

		ASSERT_TRUE(o.Init(NULL));

		o.AddDaysToTime(2);

		ASSERT_TRUE(o.CheckPatches(NULL, NULL, NULL));
	}

	TEST_F(CCallHomeCollectorBasicTest, CheckPatches_afterOneDaysOfInitWithAutocheckEnabled) {
		EXPECT_CALL(m_patchCheckerMock, Check())
			.WillOnce(Return(S_OK));
		EXPECT_CALL(*o.GetHttpMan(), SendAsyncRequest(_, _, _, _))
			.Times(0);

		o.dynconfAutocheckEnabled=true;
		o.SetTime(20, 0, 0);

		ASSERT_TRUE(o.Init(NULL));

		o.AddDaysToTime(2);

		ASSERT_TRUE(o.CheckPatches(NULL, NULL, NULL));
	}


	//=========================== CCallHomeMaster tests ===========================

	TEST_F(CCallHomeMasterSchedulingTest, Init_firstDayBeforeMidnight_scheduleCheck) {
		GetObj()->SetTime(23, 59, 56);
		EXPECT_CALL(*((MockTimer*)(GetObj()->GetTimer())), Schedule(AllOf(Gt(0), Le(4000)) , _))
	        .WillOnce(Return(TRUE));

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));
	}
	
	TEST_F(CCallHomeMasterSchedulingTest, Init_secondDayBeforeCalltime_scheduleCheck) {
		GetObj()->SetTime(0, 1, 0);
		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));

		GetObj()->SetTime(memDataArea.callTime);
		GetObj()->AddDaysToTime(1);
		GetObj()->AddMsToTime(-100);
		NextObject();
		
		EXPECT_CALL(*((MockTimer*)(GetObj()->GetTimer())), Schedule(AllOf(Gt(0), Le(4000)) , _))
	        .WillOnce(Return(TRUE));

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));
	}

	TEST_F(CCallHomeMasterTest, Init_firstDayBeforeMidnight_checkedOnlyPatches) {
		GetObj()->SetTime(23, 59, 56);
		EXPECT_CALL(m_patchCheckerMock, Check())
	        .WillOnce(Return(S_OK));
		EXPECT_CALL(*GetObj()->GetHttpAuto(), SendAsyncRequest(_, _, _, _))
	        .Times(0);

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));
		//GetObj()->IncreaseCounter("aCounter", 1);
		Sleep(3500);
	}



	TEST_F(CCallHomeMasterTest, Init_firstAndSecondDayBeforeMidnight_triggerPatchesCheckAndTelemetry) {
		GetObj()->SetTime(23, 59, 56);
		EXPECT_CALL(m_patchCheckerMock, Check())
	        .WillOnce(Return(S_OK));
		EXPECT_CALL(*GetObj()->GetHttpAuto(), SendAsyncRequest(_, _, _, _))
	        .Times(0);

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));
		
		Sleep(3500);

		//GetObj()->IncreaseCounter("aCounter", 1);
		
		GetObj()->AddDaysToTime(1);
		NextObject();
		
		EXPECT_CALL(m_patchCheckerMock, Check())
	        .WillOnce(ReturnFromAsyncCall(S_OK, &hEvent1));
		EXPECT_CALL(*GetObj()->GetHttpAuto(), SendAsyncRequest(_, _, _, _))
	        .WillOnce(ReturnFromAsyncCall(true, &hEvent2));

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));

		//GetObj()->IncreaseCounter("aCounter", 1);
		ASSERT_TRUE(Join(2, 3500));
	}

	TEST_F(CCallHomeMasterTest, Init_secondDayBeforeCalltime_triggerPatchesCheckAndTelemetry) {
		GetObj()->SetTime(0, 1, 0);
		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));

		GetObj()->SetTime(memDataArea.callTime);
		GetObj()->AddDaysToTime(1);
		GetObj()->AddMsToTime(-100);
		NextObject();
		
		EXPECT_CALL(m_patchCheckerMock, Check())
	        .WillOnce(ReturnFromAsyncCall(S_OK, &hEvent1));
		EXPECT_CALL(*GetObj()->GetHttpAuto(), SendAsyncRequest(_, _, _, _))
	        .WillOnce(ReturnFromAsyncCall(true, &hEvent2));

		ASSERT_TRUE(GetObj()->Init(&itemsInitializer));

		ASSERT_TRUE(Join(2, 1000));
	}
};


//=========================== CInterprocess* tests ===========================

TEST(CInterprocessStaticMapTest, SetValue_beyondMaxItems) {
	CInterprocessStaticMap<DWORD, 3> m;
	CInterprocessStaticMap<DWORD, 3>::MAP_STORAGE mstorage={};
	ASSERT_TRUE(m.Init(&mstorage));
	DWORD value;
	ASSERT_TRUE(m.SetValue("key1", value));
	ASSERT_TRUE(m.SetValue("key2", value));
	ASSERT_TRUE(m.SetValue("key3", value));
	ASSERT_FALSE(m.SetValue("key4", value));
	ASSERT_EQ(3, m.size());
}

TEST(CInterprocessCountersMapTest, IncrementDynamic) {
	CInterprocessCountersMap m;
	CInterprocessCountersMap::MAP_STORAGE mstorage={};
	ASSERT_TRUE(m.Init(&mstorage));
	m.IncrementDynamic("aDynCounter", 1);
	m.IncrementDynamic("aDynCounter", 2);
	ASSERT_EQ(1, m.size());
	DWORD value;
	ASSERT_TRUE(m.GetValue("aDynCounter", value));
	ASSERT_EQ(3, value);
}

//=========================== main ===========================

int main(int argc, char** argv) {
	if(argc==1) {
		char *defArgv[]={argv[0], "--gtest_filter=-CCallHomeMasterTest.*"};
		int defArgc=2;
		::testing::InitGoogleMock(&defArgc, defArgv);
	} else {
		::testing::InitGoogleMock(&argc, argv);
	}
	::testing::Environment* const env = ::testing::AddGlobalTestEnvironment(new ChEnvironment());
	return RUN_ALL_TESTS();
}