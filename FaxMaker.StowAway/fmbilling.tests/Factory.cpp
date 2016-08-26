#include "stdafx.h"
#include "..\factory.h"
#include "ILogInterface.stub.h"
#include "PhoneNumberParser.mock.h"
#include "HttpRequest.mock.h"
#include "ProxySettings.stub.h"

//UNIT TESTING FACTORY
//will return stubs or mocks

ILogInterface* BillingManagerFactory::getLogger()
{
	return new CLogStub();
}

IProxySettings* BillingManagerFactory::getProxySettingsLoader()
{
	return new ProxySettings_Stub();
}

IHttpRequest* BillingManagerFactory::getHttpRequester()
{
	return new HttpRequestMockWrapper();
}

PhoneNumberParser* BillingManagerFactory::getPhoneNumberParser()
{
	return new PhoneNumberParserWrapper();
}

