#include <windows.h>
#include <gmock/gmock.h>
#include <string>
using namespace::std;



class EtherFaxLine_Mock
{
public:

	EtherFaxLine_Mock()
	{

	};

	MOCK_METHOD0(getPortName,string());
	MOCK_METHOD0(GetState,long());
	MOCK_METHOD0(getAccountName,LPCSTR());
	MOCK_METHOD0(getUserID,LPCSTR());
	MOCK_METHOD0(getLineDIDNumber,LPCSTR());
};