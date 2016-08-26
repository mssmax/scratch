#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "..\PhoneNumberParser.h"
#include "PhoneNumberUtil.mock.h"
#include "ILogInterface.stub.h"
#include <string>
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
using testing::StrictMock;
using testing::SetArgPointee;

//////////////////////////////////////////////
//TEST FIXTURE -  PhoneNumberParser         //
//                                          //
//										    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////

NiceMock<PhoneNumberUtil_Mock>* g_phoneUtilMock;

class PhoneNumberParser_fixture : public mfgfixture
{


public:

	
	//Called before every test.
    virtual void SetUp() 
    {
		g_phoneUtilMock = new NiceMock<PhoneNumberUtil_Mock>();
	};

    //Called after every test.
    virtual void TearDown()
    {
		delete g_phoneUtilMock;
	};

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_PassNULLParamPhoneNumber_ReturnsFalse)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_EQ(false,phoneParser.parseCountryCodeNumber(NULL,number,new CLogStub()));
};

TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberParsedCorrectlyImmediately_ReturnsTrue)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillOnce(Return(1));
	EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).Times(4).WillRepeatedly(Return(true));
	EXPECT_EQ(true,phoneParser.parseCountryCodeNumber("+35622052246",number,new CLogStub()));
};

TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberNeverParsedCorrectly_ReturnsFalse)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillRepeatedly(Return(1));
	EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).WillRepeatedly(Return(false));
	EXPECT_EQ(false,phoneParser.parseCountryCodeNumber("1234",number,new CLogStub()));
};

TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberIsIntLacksLeadingPlus_ReturnsTrue)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillRepeatedly(Return(1));
	EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).WillOnce(Return(false)).WillRepeatedly(Return(true));
	EXPECT_EQ(true,phoneParser.parseCountryCodeNumber("35622052246",number,new CLogStub()));
};

TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberIsIridium_ReturnsTrue)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillRepeatedly(Return(1));
	EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).WillRepeatedly(Return(false));
	EXPECT_EQ(true,phoneParser.parseCountryCodeNumber("+88122565565",number,new CLogStub()));
};
TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberInmarsat_ReturnsTrue)
{
	string number;
	PhoneNumberParser phoneParser;
	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillRepeatedly(Return(1));
	EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).WillRepeatedly(Return(false));
	EXPECT_EQ(true,phoneParser.parseCountryCodeNumber("87022565565",number,new CLogStub()));
};
//TEST_F(PhoneNumberParser_fixture, parseCountryCodeNumber_NumberIsIridium_ReturnsTrue)
//{
//	string number;
//	EXPECT_CALL(*g_phoneUtilMock,Parse(_,_,_)).WillRepeatedly(Return(1));
//
//	using ::testing::InSequence;
//	{
//		InSequence dummy;
//
//		EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).Times(2).WillRepeatedly(Return(false));
//		EXPECT_CALL(*g_phoneUtilMock,IsValidNumber(_)).WillRepeatedly(Return(true));
//	
//
//	}
//
//	
//	EXPECT_EQ(true,PhoneNumberParser::parseCountryCodeNumber("87022565565",number,new CLogStub()));
//};