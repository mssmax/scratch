//==============================================================================
// Unit Testing for CRSxMessage
//==============================================================================
#pragma once
#ifdef PERFORM_RSXMESSAGE_TESTS
//==============================================================================

#include "stdafx.h"
#include "mfg.h"
#include "mocks.h"
#include "CUtilities_Mock.h"
#include "Rsxmessage.h"
#include "html_slayer.mock.h"
#include <mfg_htmlslayer.h>
#include "UnitTesting.h"

#define STREQ(str)	MatcherCast<LPCSTR>(StrEq(str))
#define CAST(str) MatcherCast<str>(_)
//==============================================================================

class CRSxMessage_Test : public CRSxMessage
{
public:
	CRSxMessage_Test(LPCSTR sPath, IMtaConfig *pMtaConfig, IReports *pReports, 
		        IUtilities *pUtils, IStatusInterface* pSI,
				IMultiLanguagePtr	spML) : CRSxMessage(sPath, pMtaConfig, pReports, 
		        pUtils, pSI, spML)
	{}

	HRESULT CreateCDOMessage(CDO::IMessagePtr &ptr)
	{
		ptr = gp_msg_mock;
		return S_OK;
	}
	
	//Setting the mock
	CDO::IMessage * gp_msg_mock;
	void SetMsgMock(CDO::IMessage * g_msg_mock)
	{
		gp_msg_mock = g_msg_mock;
	}
};

//==============================================================================

class rsxmessage_fxt: public mfgfixture
{
public:
	CRSxMessage_Test *			m_rsx;	

	NiceMock<CUtilities_Mock>	m_utils;	
	NiceMock<IReportsMock>		m_rep;
	NiceMock<CMtaConfigMock>	m_mta;
	NiceMock<CStatusInterfaceMock>  m_pSI;	
	IMultiLanguagePtr			m_spMl;	

	virtual void SetUp()
	{			
		m_rsx = new CRSxMessage_Test("", &m_mta,&m_rep,&m_utils,&m_pSI, m_spMl);	
	}

	virtual void TearDown()
	{	
		delete m_rsx;
	}
};


#endif //PERFORM_RSXMESSAGE_TESTS

//==============================================================================