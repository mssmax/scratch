#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "..\SHA256.h"


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
//TEST FIXTURE -  SHA256			        //
//                                          //
//										    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////


class SHA256_fixture : public mfgfixture
{


public:

	SHA256* pSha;
	
	//Called before every test.
    virtual void SetUp() 
    {
		pSha = new SHA256();	

    };

    //Called after every test.
    virtual void TearDown()
    {
		delete pSha;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(SHA256_fixture, CalculateHash_PassNULLParam_ReturnsEmpty)
{
	EXPECT_EQ(NULL,pSha->CalculateHash(NULL,0));
};


//This unit test will check that given a string value the returned hashed value is correct
//value obtained from http://www.online-convert.com/result/254a85969f0531e3e35f065d1eb35de8
TEST_F(SHA256_fixture, CalculateHash_PassValueString_ReturnsExpectedResultString)
{
	char input[] = "Hello From GFI Unit Testing";
	int len = strlen(input) ;
	EXPECT_STREQ("5VN9uFlxbD12YPQ7aW6S3rtIx7hGIHr1rtb/tnI3YwI=",pSha->CalculateHash(input,len));
};