#include <windows.h>
#include <gmock/gmock.h>
#include "..\PhoneNumberParser.h"
using testing::NiceMock;

class PhoneNumberParser_Mock;

extern NiceMock<PhoneNumberParser_Mock>* g_pPhoneNumberParserMock;

class PhoneNumberParser_Mock 
{
public:

	PhoneNumberParser_Mock()
	{

	};

	MOCK_METHOD3(parseCountryCodeNumber, bool(LPCSTR, string&,ILogInterface*));
	MOCK_METHOD3(GetCountryCodeFromNum,bool(LPCSTR, LPSTR,size_t));
	
};

class PhoneNumberParserWrapper : public PhoneNumberParser
{
	bool parseCountryCodeNumber(LPCSTR phoneNumber, string &FullNumber,ILogInterface *pLog)
	{
		return g_pPhoneNumberParserMock->parseCountryCodeNumber(phoneNumber,FullNumber,pLog);
	}
	bool GetCountryCodeFromNum(LPCSTR phoneNumber, LPSTR countryCode, size_t countryCodeSize)
	{
		return g_pPhoneNumberParserMock->GetCountryCodeFromNum(phoneNumber,countryCode,countryCodeSize);
	}
	

};