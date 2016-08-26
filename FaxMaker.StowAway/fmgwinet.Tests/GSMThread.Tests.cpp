//////////////////////////////////////////////
// Add header files here:                   //
//////////////////////////////////////////////

#include "stdafx.h"

#ifdef PERFORM_GSMTHREAD_TESTS

#include "..\GSMServicesThread.h"
//Mocks
#include "mocks.h"
#include "ILogInterface.stub.h"
#include "GSMServicesInterface.Mock.h"
#include "TelStore.Mock.h"

//////////////////////////////////////////////
//TESTABLE VERSION OF CGSMSERVICESTHREAD    //
//////////////////////////////////////////////

class CGSMServicesThreadEx: public CGSMServicesThread
{

public:

    CGSMServicesThreadEx() : CGSMServicesThread((CMtaConfig*)&m_FakeMTAConfig, (CReports*)&m_FakeReports, NULL)
    {
    };

    virtual ~CGSMServicesThreadEx() {};


    virtual inline ILogInterface* FactoryConstructLogHandler()
    {
        return new CLogStub();
    };

    virtual inline GSMServicesLibInterface::libinterface* FactoryConstructSMSEngine()
    {
        return new NiceMock<GSMServicesInterface_Mock>();
    };

    virtual inline IStatusInterface* FactoryConstructUIInterface()
    {
        return new NiceMock<CStatusInterfaceMock>();
    };

    virtual inline fmgfidb_c* FactoryConstructDBInterface()
    {
        return NULL;
    };

    virtual inline CTelStore*  FactoryConstructTellStoreInterface()
    {
        return new  CTelStore();
    };

private:

    NiceMock<IReportsMock>   m_FakeReports;
    NiceMock<CMtaConfigMock> m_FakeMTAConfig;
}; 

////////////////////////////////////////////////////
//TEST FIXTURE - CGSMServicesThread               //
//                                                //
//Dependencies:  1 - Log                 (Stubbed //
//               2 - GSMServiesInterface (MOCKED  //
//               3 - IStatusInterface    (Stubbed)//
//               4 - fmstore             (MOCKED) //
//               5 - tellstore           (MOCKED) //
//               6 - fmgidb              (MOCKED) //
//                                                //
////////////////////////////////////////////////////

class CGSMServicesThread_fixture : public mfgfixture
{

public:
    
     CGSMServicesThreadEx *m_ptrGSMThread;

protected:

    // Called before every test.
    virtual void SetUp() 
    {
        m_ptrGSMThread = new CGSMServicesThreadEx();
    };

    // Called after every test.
    virtual void TearDown()
    {
        delete m_ptrGSMThread;
    };

};

///////////////////////////////////////////////
//Unit tests - parm1:  Name of test fixture. //
//             parm2:  State Under Test.     //
//             parm3:  ExpectedResult        //
//                                           //
///////////////////////////////////////////////

#endif //PERFORM_GSMTHREAD_TESTS