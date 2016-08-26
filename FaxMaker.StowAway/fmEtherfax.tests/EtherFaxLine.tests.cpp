#include "stdafx.h"
#include "mfg.h"

//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////
#include "ILogInterface.stub.h"
#include "..\EtherFaxLine.h"
#include "fmdevcfg.h"
#include "CAThreadPool.mock.h"


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
//TEST FIXTURE -  EtherFaxLine        //
//                                          //
//Dependancies: 1 - CAThreadPool(MOCKED)    //
//										    //
//                                          //
//                                          //
//////////////////////////////////////////////


class EtherFaxLine_fixture : public mfgfixture
{


public:

	EtherFaxLine* m_pEtherFaxLine;
	NiceMock<CAThreadPool_Mock>*     m_ThreadPool;

	//Called before every test.
    virtual void SetUp() 
    {
		m_ThreadPool = new NiceMock<CAThreadPool_Mock>();
		char szTemp[512];
		szTemp[0]=0;
		GetTempPathA(512,szTemp);
		m_pEtherFaxLine = new EtherFaxLine("UNIT_TEST_PORT_NAME",szTemp,"https://na.connect.etherfax.net/web/api/",new CLogStub(),NULL,m_ThreadPool,NULL);

    };

    //Called after every test.
    virtual void TearDown()
    {
		delete m_pEtherFaxLine;
		delete m_ThreadPool;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             param3: ExpectedResult        //
//                                           //
///////////////////////////////////////////////


TEST_F(EtherFaxLine_fixture, Initialize_PassNULLParamLineConfig_ReturnsInvalidParam)
{
	etherfaxData_t ec;
	EXPECT_EQ(FM_INVALIDPARAM,m_pEtherFaxLine->Initialize(NULL,&ec));
};

TEST_F(EtherFaxLine_fixture, Initialize_PassNULLParamEtherFaxConfigConfig_ReturnsInvalidParam)
{
	lineconfig_t lc;
	EXPECT_EQ(FM_INVALIDPARAM,m_pEtherFaxLine->Initialize(&lc,NULL));
};

TEST_F(EtherFaxLine_fixture, RequestFaxTransfer_LineIsNotEnabledForSending_ReturnsFmerror)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options = lc.options & ~LINECFG_CANSEND;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	faxtrans_t ft;
	EXPECT_EQ(FM_ERROR,m_pEtherFaxLine->RequestFaxTransfer(&ft));
};

TEST_F(EtherFaxLine_fixture, RequestFaxTransfer_LineIsEnabledForSending_ReturnsFmok)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options |= LINECFG_CANSEND;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	faxtrans_t ft;
	EXPECT_EQ(FM_OK,m_pEtherFaxLine->RequestFaxTransfer(&ft));
};

TEST_F(EtherFaxLine_fixture, RequestReceiveFax_LineIsNotEnabledForReceiving_ReturnsFmerror)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options = lc.options & ~LINECFG_CANRECEIVE;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	EXPECT_EQ(FM_ERROR,m_pEtherFaxLine->RequestReceiveFax("GUID"));
};

TEST_F(EtherFaxLine_fixture, RequestReceiveFax_LineIsEnabledForReceiving_ReturnsFmok)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options |= LINECFG_CANRECEIVE;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	EXPECT_EQ(FM_OK,m_pEtherFaxLine->RequestReceiveFax("GUID"));
};

TEST_F(EtherFaxLine_fixture, CanReceive_LineCannotReceive_ReturnsFalse)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options = lc.options & ~LINECFG_CANRECEIVE;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	EXPECT_EQ(FALSE,m_pEtherFaxLine->CanReceive());
};

TEST_F(EtherFaxLine_fixture, CanReceive_LineCanReceive_ReturnsTrue)
{
	lineconfig_t lc;
	etherfaxData_t ec;
	lc.options |= LINECFG_CANRECEIVE;
	m_pEtherFaxLine->Initialize(&lc,&ec);
	EXPECT_EQ(TRUE,m_pEtherFaxLine->CanReceive());
};


