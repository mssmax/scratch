#include <windows.h>
#include <gmock/gmock.h>
using testing::NiceMock;
#include "..\IHttpRequest.h"

class HttpRequest_Mock;

extern NiceMock<HttpRequest_Mock>* g_pHttpRequestMock;

class HttpRequest_Mock
{
public:

	HttpRequest_Mock()
	{

	};

	MOCK_METHOD5(Request,fmerror(LPCSTR, long,std::string,IProxySettings*,ILogInterface*));
	MOCK_METHOD6(RequestEx,fmerror(LPCWSTR,LPCSTR, long,std::string,IProxySettings*,ILogInterface*));
	
};

class HttpRequestMockWrapper :public IHttpRequest
{
public:
	fmerror Request(LPCSTR pData, long DataLen, std::string &response, IProxySettings* pProxySettings,ILogInterface *pLog)
	{
		return g_pHttpRequestMock->Request(pData,DataLen,response,pProxySettings,pLog);
	};

	fmerror RequestEx(LPCWSTR ServerURL, LPCSTR pData, long DataLen, std::string &response, IProxySettings* pProxySettings,ILogInterface *pLog)
	{
		return g_pHttpRequestMock->RequestEx(ServerURL,pData, DataLen,response,pProxySettings,pLog);
	};
};