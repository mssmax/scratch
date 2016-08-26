#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "..\base64.h"


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
//TEST FIXTURE -  Base64			        //
//                                          //
//										    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////


class base64_fixture : public mfgfixture
{


public:
	
	//Called before every test.
    virtual void SetUp() 
    {
		

    };

    //Called after every test.
    virtual void TearDown()
    {
	
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(base64_fixture, base64_encode_PassNULLParam_ReturnsEmpty)
{
	EXPECT_STREQ("",base64::base64_encode(NULL,0).c_str());
};

TEST_F(base64_fixture, base64_encode_PassZeroLength_ReturnsEmpty)
{
	unsigned char input[] = "Hello From GFI Unit Testing";
	EXPECT_STREQ("",base64::base64_encode(input,0).c_str());
};


//This unit test will check that given a string value the returned encoded value is correct
// you can check results at http://www.base64encode.org/
TEST_F(base64_fixture, base64_encode_PassValueString_ReturnsExpectedResultString)
{
	unsigned char input[] = "Hello From GFI Unit Testing";
	int len = strlen((char*)input);
	EXPECT_STREQ("SGVsbG8gRnJvbSBHRkkgVW5pdCBUZXN0aW5n",base64::base64_encode(input,len).c_str());
};


TEST_F(base64_fixture, base64_decode_PassNULLParam_ReturnsEmpty)
{
	string* ptr = NULL;
	EXPECT_STREQ("",base64::base64_decode(*ptr).c_str());
};


//This unit test will check that given a string value the returned decoded value is correct
// you can check results at http://www.base64encode.org/
TEST_F(base64_fixture, base64_decode_PassValueString_ReturnsExpectedResultString)
{
	char input[] = "SGVsbG8gRnJvbSBHRkkgVW5pdCBUZXN0aW5n";
	EXPECT_STREQ("Hello From GFI Unit Testing",base64::base64_decode(input).c_str());
};