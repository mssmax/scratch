#include "..\IEtherFaxLine.h"
#include "EtherFaxLine.mock.h"
using testing::NiceMock;

extern NiceMock<EtherFaxLine_Mock>*	 g_EtherfaxlineMock;

class EtherFaxLineStub : public IEtherFaxLine
{

	fmerror RequestFaxTransfer(faxtrans_t* pft) { return FM_OK;};
	fmerror RequestReceiveFax(LPCSTR EtherFaxJobId) {return FM_OK;};
	long GetState()
	{
		return g_EtherfaxlineMock->GetState();
	}
	
	void setLineFreeCallback(LPLINEFREECALLBACK ppcb, LPVOID pparam) {};
	BOOL CanReceive() {return TRUE;};

	fmerror Initialize(lineconfig_t*  plc,etherfaxData_t*  pec)
	{
		return FM_OK;
	};
	string getPortName()
	{
		return "HELLO";
	};
	LPCSTR getAccountName() 
	{
		return g_EtherfaxlineMock->getAccountName();
	};
	LPCSTR getUserID()
	{
		return g_EtherfaxlineMock->getUserID();
	};
	LPCSTR getLineDIDNumber()
	{
		return g_EtherfaxlineMock->getLineDIDNumber();
	};

	volatile BOOL isInitialized;
	void Abort() {};

};