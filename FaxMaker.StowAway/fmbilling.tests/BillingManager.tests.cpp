#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "..\billingmanager.h"
#include "HttpRequest.mock.h"
#include "PhoneNumberParser.mock.h"
#include <string>
#include <vector>
using namespace std;
//////////////////////////////////////////////
// Add using statments here:                //
//////////////////////////////////////////////

using testing::_;
using testing::Eq;
using testing::AnyOf;
using testing::Invoke;
using testing::Assign;
using testing::Return;
using testing::NiceMock;
using testing::StrCaseEq;
using testing::SetArgPointee;



//////////////////////////////////////////////
//TEST FIXTURE -  BillingManager	        //
//                                          //
//										    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////
NiceMock<HttpRequest_Mock>* g_pHttpRequestMock;
NiceMock<PhoneNumberParser_Mock>* g_pPhoneNumberParserMock;

class BillingManager_fixture : public mfgfixture
{


public:

	BillingManager* m_pBillingManager;
	
	//Called before every test.
    virtual void SetUp() 
    {
		g_pHttpRequestMock = new NiceMock<HttpRequest_Mock>();	
		g_pPhoneNumberParserMock = new NiceMock<PhoneNumberParser_Mock>();
		m_pBillingManager = new BillingManager("ETHERFAX","NOLICENSE",FALSE,"HELLO");
		
    };

    //Called after every test.
    virtual void TearDown()
    {
		delete g_pHttpRequestMock;
		delete g_pPhoneNumberParserMock;
		delete m_pBillingManager;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(BillingManager_fixture, transactReceiveFax_DialledNumberNotNullButGetCountryCodeFails_ReturnsFM_BILLINGNOCOUNTRYCODEFOUND)
{
	EXPECT_CALL(*g_pPhoneNumberParserMock,GetCountryCodeFromNum(_,_,_)).WillRepeatedly(Return(false));
	vector<string> dids;
	EXPECT_EQ(FM_BILLINGNOCOUNTRYCODEFOUND,m_pBillingManager->transactReceiveFax(1,"",dids,"035622562259"));
};

TEST_F(BillingManager_fixture, transactReceiveFax_NumbersOkHttpRequestOk_ReturnsFM_OK)
{
	EXPECT_CALL(*g_pPhoneNumberParserMock,GetCountryCodeFromNum(_,_,_)).WillRepeatedly(Return(true));
	vector<string> dids;
	EXPECT_EQ(FM_BILLINGNOCOUNTRYCODEFOUND,m_pBillingManager->transactReceiveFax(1,"",dids,"035622562259"));
};