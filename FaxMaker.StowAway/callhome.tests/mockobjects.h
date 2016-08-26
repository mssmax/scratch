#pragma once
#include "PatchChecker.h"
#include "XmlCreator.h"
#include "WinHttpClient.h"
#include "utils.h"

namespace callhome {
	class CPatchCheckerMock : public CPatchCheckerBase {
	public:
		MOCK_METHOD0(Check, HRESULT());
	};

	class MockCallHomeXml : public CCallHomeXml {
	public:
		MOCK_METHOD4(CreateTelemetryXml, bool(LPCWSTR, WinHttpClient::httpVerb_t, BYTE, unsigned int));
		MOCK_METHOD0(Initialize, bool());
	};

	class MockHttpClient : public WinHttpClient {
	public:
		MOCK_METHOD4(SendAsyncRequest, bool(LPCWSTR, httpVerb_t, BYTE *, unsigned int));
		MOCK_METHOD0(RetrieveSystemProxySettings, bool());
		MOCK_METHOD0(Initialize, bool());
	};


	class MockTimer : public CTimer {
	public:
		MockTimer() : CTimer(NULL, NULL)  {}
		MOCK_METHOD2(Schedule, BOOL(DWORD, DWORD));
		MOCK_METHOD0(Cancel, void());
	};

	void SetPatchCheckerMock(CPatchCheckerMock *ptr);
};