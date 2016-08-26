#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "CAThreadPool.mock.h"
#include "EtherFaxLine.mock.h"
#include "..\LineManager.h"


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
//TEST FIXTURE -  CDeviceManager            //
//                                          //
//Dependancies: 1 - CAThreadPool(MOCKED)    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////

NiceMock<EtherFaxLine_Mock>*	 g_EtherfaxlineMock;
#define SERVERURL				 "https://na.connect.etherfax.net/web/api/"

class LineManager_fixture : public mfgfixture
{


public:

	LineManager* m_pLineMan;
	NiceMock<CAThreadPool_Mock>*     m_ThreadPool;
	


	//Called before every test.
    virtual void SetUp() 
    {
		m_ThreadPool = new NiceMock<CAThreadPool_Mock>();
		g_EtherfaxlineMock = new NiceMock<EtherFaxLine_Mock>();
		str_c license("Unit Testing License");
		m_pLineMan = new LineManager(m_ThreadPool,&license,NULL);

    };

    //Called after every test.
    virtual void TearDown()
    {
		delete m_pLineMan;
		delete g_EtherfaxlineMock;
		delete m_ThreadPool;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(LineManager_fixture, FindLineByPortName_FindNonExistingLine_ReturnsNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	EXPECT_EQ(NULL,m_pLineMan->FindLineByPortName("Line2"));
};


TEST_F(LineManager_fixture, FindLineByPortName_FindExistingLine_ReturnsNotNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	EXPECT_NE((IEtherFaxLine*) NULL,m_pLineMan->FindLineByPortName("Line101"));
	//EXPECT_TRUE(NULL != m_pLineMan->FindLineByPortName("Line101"));
};

TEST_F(LineManager_fixture, FindFreeLine_NoFreeLineAvailable_ReturnsNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	EXPECT_CALL(*g_EtherfaxlineMock,GetState()).WillRepeatedly(Return(STATE_SENDING));
	EXPECT_EQ(NULL,m_pLineMan->FindFreeLine());
};

TEST_F(LineManager_fixture, FindFreeLine_FreeLineAvailable_ReturnsNotNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	EXPECT_CALL(*g_EtherfaxlineMock,GetState()).WillRepeatedly(Return(STATE_IDLE));
	EXPECT_NE((IEtherFaxLine*)NULL,m_pLineMan->FindFreeLine());
};


//Not all lines are idle, the first 2 will be busy the rest will report as idle
TEST_F(LineManager_fixture, FindFreeLine_NotAllLinesAvailable_ReturnsNotNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	
	using ::testing::InSequence;
	{
		InSequence dummy;

		EXPECT_CALL(*g_EtherfaxlineMock,GetState()).Times(2).WillRepeatedly(Return(STATE_SENDING));
		EXPECT_CALL(*g_EtherfaxlineMock,GetState()).WillRepeatedly(Return(STATE_IDLE));
	

	}
	EXPECT_NE((IEtherFaxLine*)NULL,m_pLineMan->FindFreeLine());
};

TEST_F(LineManager_fixture, FindAccountFreeLine_FreeLineAvailable_ReturnsNotNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	EXPECT_CALL(*g_EtherfaxlineMock,getAccountName()).WillRepeatedly(Return("MyAccount"));
	EXPECT_CALL(*g_EtherfaxlineMock,getUserID()).WillRepeatedly(Return("MyUserID"));
	EXPECT_NE((IEtherFaxLine*)NULL,m_pLineMan->FindAccountFreeLine("MyAccount","MyUserID"));
};

TEST_F(LineManager_fixture, FindAccountFreeLine_FreeLineNotAvailable_ReturnsNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	EXPECT_CALL(*g_EtherfaxlineMock,GetState()).WillRepeatedly(Return(STATE_SENDING));
	EXPECT_EQ(NULL,m_pLineMan->FindAccountFreeLine("MyAccount","MyUserID"));
};

TEST_F(LineManager_fixture, FindAccountFreeLine_AccountNoMatch_ReturnsNULL)
{
	m_pLineMan->CreateNewLine("Line101","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line102","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line103","BillAcc1",SERVERURL);
	m_pLineMan->CreateNewLine("Line104","BillAcc1",SERVERURL);
	EXPECT_CALL(*g_EtherfaxlineMock,getAccountName()).WillRepeatedly(Return("MyAccount"));
	EXPECT_CALL(*g_EtherfaxlineMock,getUserID()).WillRepeatedly(Return("MyUserID"));
	EXPECT_EQ(NULL,m_pLineMan->FindAccountFreeLine("MyAccount2","MyUserID"));
};


TEST_F(LineManager_fixture, isPortSupported_PortShouldBeSupported_ReturnsTRUE)
{
	EXPECT_EQ(TRUE,m_pLineMan->isPortSupported("etherFAX123"));
};
TEST_F(LineManager_fixture, isPortSupported_PortShouldNOTBeSupported_ReturnsFalse)
{
	EXPECT_EQ(FALSE,m_pLineMan->isPortSupported("OFS123"));
};