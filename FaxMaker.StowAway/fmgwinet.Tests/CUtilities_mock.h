//CUtilities_Mock
//==============================================================================

#pragma once
#include "UnitTesting.h"

//==============================================================================
#include "Utilities.h"

class CUtilities_Mock : public IUtilities
{
public:
	CUtilities_Mock();
	~CUtilities_Mock();

	MOCK_METHOD0(setlanguage, BOOL());	
	MOCK_METHOD0(IsExchange, BOOL());
	MOCK_METHOD2(IsMessageUnicode,  BOOL (LPCWSTR lpszText, long bDisableUnicode));
	MOCK_METHOD5(smtplog, void(BOOL bSmtpOut, LPCWSTR lpszsender, LPCWSTR lpszrecipient, LPCWSTR lpszsubject, LPCWSTR lpszstatus));
	MOCK_METHOD1(IsFaxNumber, BOOL(LPCWSTR lpsznum));
	MOCK_METHOD1(ExtractEmail, std::wstring(LPCWSTR lpszAddress));
	MOCK_METHOD1(MoveFileToIgnored, BOOL(LPCSTR lpszfile));
	MOCK_METHOD2(extractemailsfromfields, HRESULT(LPCSTR lpBuffer, std::vector<std::string> &emails ));
	MOCK_METHOD3(ConvertFileFromEncoding, HRESULT(DWORD dwCodePage, LPCSTR lpszSource, LPCSTR lpszDestination));
	MOCK_METHOD1(MoveToBad, void (LPCSTR lpszName));
	MOCK_METHOD1(CopyToSourceArchive, void (LPCSTR lpszFileToCopy));
};

typedef CUtilities_Mock IUtilitiesMock;

void SetUtilitiesMock(IUtilitiesMock *);

//==============================================================================
