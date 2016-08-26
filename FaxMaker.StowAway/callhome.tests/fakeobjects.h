#pragma once

namespace callhome {
	template<class T>
	class FakeCallHomeCollector : public T {
		//friend class CCallHomeCollectorFixture;
	public:
		//costructor for the subclass of CCallHomeCollector
		FakeCallHomeCollector(CHDATAAREA *pMem) : pMemDataArea(pMem), dynconfIsAdvancedRequestsAllowed(true), dynconfAutocheckEnabled(true)
		{
			InitFake();
		}
		
		//costructor for the subclass of CMasterCallhome
		FakeCallHomeCollector(CHDATAAREA *pMem, BeforeCallingCb bcCb, NotificationCb nCb, LPVOID lpParam) : pMemDataArea(pMem), dynconfIsAdvancedRequestsAllowed(true), dynconfAutocheckEnabled(true), T(bcCb, nCb, lpParam)
		{
			InitFake();
		}

		virtual ~FakeCallHomeCollector() {
		}
	
	protected:
		CHDATAAREA *pMemDataArea;
		MockCallHomeXml mockXml;
		SYSTEMTIME fakeTime;

		void InitFake() {
			httpMan=new MockHttpClient();
		}

		virtual BOOL ReadConfiguration() {
			version="16.0";
			installId="1ccc8a4a-e9c7-4b29-afad-3de7bd349851";
			dwBuildNum=20120828;
			gfiKeyUrl=L"http://fake.fake";
			return TRUE;
		}

		virtual BOOL ReadDynamicConfiguration(_bstr_t &licenseKey) {
			isAdvancedRequestsAllowed=dynconfIsAdvancedRequestsAllowed;
			licenseKey="fakeliceseddssdfasfd";
			//Read Proxy Settings...
			proxyServer=L"http://fakeproxy.fake";
			proxyPort=80;
			proxyEnabled=true;
			proxyCredentialsEnabled=false;
			proxyAuthScheme=WINHTTP_AUTH_SCHEME_BASIC;
			//proxyUsername//proxyPassword

			autocheckEnabled=dynconfAutocheckEnabled;
			return TRUE;
		}

		virtual CCallHomeXml &GetCallHomeXml() {
			return mockXml;
		}

		virtual WinHttpClient *CreateHttpClient() {
			return new MockHttpClient();
		}
		
		virtual HRESULT GetAdminEmails(std::wstring *emails) {
			*emails=L"fake@fakeland";
			return S_OK;
		}

		virtual BOOL InitDataArea() {
			m_pDataArea=pMemDataArea;
			//ZeroMemory(m_pDataArea, sizeof(CHDATAAREA));
			m_pDataArea->dwVersion = 1;
			InitMaps();
			return TRUE;
		}
		
		virtual void UninitDataArea() {
		}

	public:
		//public member variables added for tests
		bool dynconfIsAdvancedRequestsAllowed;
		bool dynconfAutocheckEnabled;

		//setters and utility added for tests
		void SetTime(WORD hours, WORD minutes, WORD seconds) { 
			SYSTEMTIME st;
			GetLocalTime(&st);
			st.wHour=hours;
			st.wMinute=minutes;
			st.wSecond=seconds;
			SetTime(st);
		}
		void SetTime(DWORD msFromMidnight) { 
			msFromMidnight /= 1000;  
			int seconds = msFromMidnight % 60;  
			int minutes = (msFromMidnight % 3600) / 60;  
			int hours = msFromMidnight / 3600;  
			SetTime(hours, minutes, seconds);
		}
		void AddDaysToTime(int days) {
			AddMsToTime(days*CDate::msInDay);
		}
		void AddMsToTime(__int64 ms) {
			FILETIME ft;
			SystemTimeToFileTime (&fakeTime,&ft);
			ULARGE_INTEGER u; 
			memcpy(&u, &ft, sizeof(u));
			u.QuadPart+=10000*ms;
			memcpy(&ft, &u, sizeof(ft));
			FileTimeToSystemTime(&ft, &fakeTime);
		}

		//accessor for protected members of CCallHomeCollector added for tests
		ints_map &GetInts() {return ints; }
		strings_map &GetStrings() {return strings; }
		counters_map &GetCounters() {return counters; }
		dynamicCountersGroup_map &GetDynamicCountersGroups() {return dynamicCountersGroups; }
		map<string, DynamicCountersGroup*> &GetInstanceDynamicCountersGroups() {return dynGroups; }
		void SetTime(const SYSTEMTIME &sysTime) { fakeTime=sysTime; }

		MockHttpClient *GetHttpMan() {return (MockHttpClient*)httpMan; }
		CTimer *GetTimer() {return pTimer; }
		CTimer *GetDynGroupsTimer() {return pDynGroupsTimer; }


		//getter overridden with higher accessibility
		virtual void GetTime(SYSTEMTIME &now) {
			now=fakeTime;
		}
	};

	class FakeMasterCallHome : public FakeCallHomeCollector<CMasterCallHome> {
	public:
		
		FakeMasterCallHome(CHDATAAREA *pMem, BeforeCallingCb bcCb, NotificationCb nCb, LPVOID lpParam) : FakeCallHomeCollector<CMasterCallHome>(pMem, bcCb, nCb, lpParam) {
			httpAuto=new MockHttpClient();
		}
		

		virtual ~FakeMasterCallHome() {
		}

		//accessor for private members of CCallHomeMaster
		MockHttpClient *GetHttpAuto() {return (MockHttpClient*)httpAuto; }
	};

	class FakeMasterCallHomeMockedTimer : public FakeMasterCallHome {
	public:
		FakeMasterCallHomeMockedTimer(CHDATAAREA *pMem, BeforeCallingCb bcCb, NotificationCb nCb, LPVOID lpParam) : FakeMasterCallHome(pMem, bcCb, nCb, lpParam) {
			pTimer=new testing::NiceMock<MockTimer>();
			pDynGroupsTimer=new testing::NiceMock<MockTimer>();
		}
	};

		//this listener is also a workaround for the thread-unsafe nature of googlemock on windows
	/*class ChTestingListener {
	private:
		HANDLE hEvent1,hEvent2;
	public:
		int beforeCallingCbCallsCount;
		int notificationCbCallsCount;

		ChTestingListener() : beforeCallingCbCallsCount(0), notificationCbCallsCount(0) {
			hEvent1=CreateEvent(NULL,FALSE,FALSE,NULL);
			hEvent2=CreateEvent(NULL,FALSE,FALSE,NULL);
		}

		~ChTestingListener() {
			CloseHandle(hEvent1);
			CloseHandle(hEvent2);
		}
		

		virtual VOID BeforeCallingCb(CallItemsHolder &holder) {
			++beforeCallingCbCallsCount;
			SetEvent(hEvent1);
		}
		virtual VOID NotificationCb(Notification &notification)  {
			++notificationCbCallsCount;
			SetEvent(hEvent2);
		}

		bool Join(DWORD msTimeout) {
			HANDLE events[2] = {hEvent1, hEvent2};
			DWORD ret=WaitForMultipleObjects(2, events, TRUE, msTimeout);
			return WAIT_FAILED!=ret;
		}

		static VOID BeforeCallingCb(CallItemsHolder &holder, PVOID lpParam) {
			((ChTestingListener*)lpParam)->BeforeCallingCb(holder);
		}
		static VOID NotificationCb(Notification &notification, PVOID lpParam) {
			((ChTestingListener*)lpParam)->NotificationCb(notification);
		}
	};*/
};