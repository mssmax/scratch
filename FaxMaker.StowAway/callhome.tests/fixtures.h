#pragma once

//Fixtures
namespace callhome {
	class CCallHomeCollectorBasicFixture : public ::testing::Test {

	protected:
		CPatchCheckerMock m_patchCheckerMock;
		CHDATAAREA memDataArea;
		CCallHomeCollectorBasicFixture() {
			SetPatchCheckerMock(&m_patchCheckerMock);
			ZeroMemory(&memDataArea, sizeof(CHDATAAREA));
		}
	};

	//=========================== CCallHomeCollector ===========================

	class CCallHomeCollectorBasicTest : public CCallHomeCollectorBasicFixture {
	public:
		FakeCallHomeCollector<CCallHomeCollector> o;
		CCallHomeCollectorBasicTest() : o(&memDataArea) {
		}
	};

	class CCallHomeCollectorIntermediateTest : public CCallHomeCollectorBasicTest {
	public:
		virtual void SetUp() {
			CallItemsInitializer holder;
			holder.InitCounter("aCounter");
			ASSERT_TRUE(o.Init(&holder));
		}
	};

	class CCallHomeCollectorFinalTest : public CCallHomeCollectorIntermediateTest {
	public:
		DYNAMIC_COUNTERS_GROUP_HANDLE hGroup;
		DynamicCountersGroup *pGroup;

		virtual void SetUp() {
			CCallHomeCollectorIntermediateTest::SetUp();
		
			ASSERT_TRUE(o.InitDynamicCountersGroup(&hGroup ,"aGroup", 0, NULL, LOCKTYPE_MODULE));

			map<string, DynamicCountersGroup*>::iterator iGroup=o.GetInstanceDynamicCountersGroups().find("aGroup.");
			ASSERT_FALSE(iGroup == o.GetInstanceDynamicCountersGroups().end())
				  << "group just added not found in the instance DynamicCountersGroups map";
			pGroup=iGroup->second;
		}
	};

	//=========================== CCallHomeMaster ===========================

	class CCallHomeMasterTest : public CCallHomeCollectorBasicFixture {

	private:
		FakeMasterCallHome *o;
	protected:
		HANDLE hEvent1,hEvent2,hEvent3;
		CallItemsInitializer itemsInitializer;

		CCallHomeMasterTest() : o(NULL) {
			itemsInitializer.InitCounter("aCounter");
			ZeroMemory(&memDataArea, sizeof(CHDATAAREA));
		}
		virtual ~CCallHomeMasterTest() {}
		bool Join(int nofe, DWORD msTimeout) {
			HANDLE events[3] = {hEvent1, hEvent2, hEvent3};
			DWORD ret=WaitForMultipleObjects(nofe, events, TRUE, msTimeout);
			return WAIT_FAILED!=ret;
		}

		virtual FakeMasterCallHome *CreateMasterCallHome() {
			return new FakeMasterCallHome(&memDataArea, NULL, NULL, NULL);
		}

		FakeMasterCallHome *NextObject() {
			//memDataArea.callTime
			FakeMasterCallHome *no=CreateMasterCallHome();
			if (o!=NULL) {
				SYSTEMTIME st;
				o->GetTime(st);
				no->SetTime(st);
				delete o;
			}
			
			return o=no;
		}
		FakeMasterCallHome *GetObj() {
			if (!o) NextObject();
			return o;
		}

		virtual void SetUp() {
			hEvent1=CreateEvent(NULL,FALSE,FALSE,NULL);
			hEvent2=CreateEvent(NULL,FALSE,FALSE,NULL);
			hEvent3=CreateEvent(NULL,FALSE,FALSE,NULL);

//			ASSERT_TRUE(NextObject());

		}
		virtual void TearDown() {
			delete o;
			CloseHandle(hEvent1);
			CloseHandle(hEvent2);
			CloseHandle(hEvent3);
			
		}
	};

	class CCallHomeMasterSchedulingTest : public CCallHomeMasterTest {
		virtual FakeMasterCallHome *CreateMasterCallHome() {
			return new FakeMasterCallHomeMockedTimer(&memDataArea, NULL, NULL, NULL);
		}

	};
};