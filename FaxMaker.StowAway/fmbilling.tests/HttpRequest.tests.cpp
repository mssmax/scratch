#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "..\HttpRequest.h"


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
//TEST FIXTURE -  HttpRequest		        //
//                                          //
//										    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////


class HttpRequest_fixture : public mfgfixture
{


public:

	HttpRequest* pHttpRequest;
	
	//Called before every test.
    virtual void SetUp() 
    {
		pHttpRequest = new HttpRequest();	

    };

    //Called after every test.
    virtual void TearDown()
    {
		delete pHttpRequest;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(HttpRequest_fixture, CalculateHash_PassNULLParam_ReturnsEmpty)
{
	EXPECT_EQ(NULL,pSha->CalculateHash(NULL,0));
};