#include <IBillingManager.h>


class BillingManagerStub : public IBillingManager
{
public:

	fmerror transactReceiveFax(long NosPages,LPCSTR LineKey, vector<string> &DIDs, LPCSTR DialledNumber=NULL, LPCSTR DiallingNumber=NULL) { return FM_OK;};
	fmerror transactSendFax(long NosPages, LPCSTR LineKey, vector<string> &DIDs, string &Token, LPCSTR DialledNumber=NULL, LPCSTR DiallingNumber=NULL)
	{
		return FM_OK;
	};

	fmerror issueRefund(LPCSTR Token,LPCSTR LineKey)
	{
		return FM_OK;
	};

	void SetLicenseKey(LPCSTR License, BOOL bBillingDisabled) {};

	LPCSTR getAccountName() {return "UnitTestingAccount";};

};