//DownloadThread.Tests.H
//==============================================================================
#pragma once
#ifdef PERFORM_DOWNLOADTHREAD_TESTS 
//------------------------------------------------------------------------------
#include "stdafx.h"
#include "resource.h"
#include "mtaconfig.h"
#include "DownloadThread.h"
#include "mfg.h"
#include "mocks.h"
#include "CUtilities_mock.h"
#include "DownloadThread.Tests.h"
#include "UnitTesting.h"
//------------------------------------------------------------------------------
#define STREQ(str)	MatcherCast<LPCSTR>(StrEq(str))
#define CAST(str) MatcherCast<str>(_)
//==============================================================================
       
class CDownloadThread_Test : public CDownloadThread
{
public:		
	CDownloadThread_Test(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtil, IStatusInterface* pSI) : CDownloadThread(pMtaConfig,pReports, pUtil, pSI){}

	//XML DOM
	void Set_pML(IMultiLanguage *lp_ml)
	{
		m_spML = lp_ml;
	}
	
	//faking CreateInstanceProvider
	HRESULT SetInstanceProvider(CLSID id)
	{
		return S_OK;
	}

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

class CDownloadThread_Test_ProcessXML : public CDownloadThread
{
public:	
	CDownloadThread_Test_ProcessXML(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtil, IStatusInterface* pSI) : CDownloadThread(pMtaConfig,pReports, pUtil, pSI)
	{	
		setDefaults();
	}
	
	void setDefaults()
	{
		bIsEndingReturn = FALSE;
		bCheckIfMsgIsTNEFReturn= FALSE;
		bIsTnef = FALSE;
	}

	BOOL bIsEndingReturn;

	BOOL bCheckIfMsgIsTNEFReturn;
	BOOL bIsTnef;
	
	BOOL IsEnding()
	{ 
		return bIsEndingReturn;
	}

	HRESULT CheckIfMsgIsTNEF(CMimeMessage *pMsg, BOOL *pbIsTNEF)
	{
		*pbIsTNEF = bIsTnef;
		return bCheckIfMsgIsTNEFReturn;
	}

	void SetConvertor()
	{
		m_pConverter =  tnef2mime::GetNewTnef2Mime();
	}
};

//==============================================================================

extern int contenttype_index;

class downloadthread_gen_fxt: public mfgfixture
{
public:
	NiceMock<CUtilities_Mock>	m_utils;	
	NiceMock<IReportsMock>		m_rep;
	NiceMock<CMtaConfigMock>	m_mta;
	NiceMock<CStatusInterfaceMock>  m_pSI;	
};

class downloadthread_fxt: public downloadthread_gen_fxt
{
public:
	CDownloadThread *			m_dt;	

	virtual void SetUp()
	{			
		m_dt = new CDownloadThread(&m_mta,&m_rep,&m_utils,&m_pSI);	
	}

	virtual void TearDown()
	{	
		delete m_dt;
		contenttype_index = 0;
	}
};

class downloadthread_fxt_com: public downloadthread_gen_fxt
{
public:
	CDownloadThread_Test *				m_dt_t;	
	//NiceMock<IXMLDOMDocument2_Mock>		m_xmldom_mockp;

	virtual void SetUp()
	{
		m_dt_t = new CDownloadThread_Test(&m_mta,&m_rep,&m_utils,&m_pSI);		
		CoInitialize(NULL);	
	}

	virtual void TearDown()
	{		
		delete m_dt_t;
		CoUninitialize();	
	}
};

class dthread_fxt_processemail: public downloadthread_gen_fxt
{
public:
	CDownloadThread_Test_ProcessXML *	m_dt;	
	NiceMock<dirfilelist_c_mock> m_dfl;
	NiceMock<CStreamOnFileWithOffset_Mock> sof;

	dthread_fxt_processemail() : sof(0)
	{
	}

	virtual void SetUp()
	{			
		m_dt = new CDownloadThread_Test_ProcessXML(&m_mta,&m_rep,&m_utils,&m_pSI);	
		//m_dt->m_sXMLAPIFolder = "c:\\";
		SetDirFileListMock(&m_dfl);	
		SetStreamOnFileWithOffsetMock(&sof);	
		m_dt->setDefaults();		
	}

	virtual void TearDown()
	{	
		delete m_dt;
		contenttype_index = 0;
	}
};

#endif	//PERFORM_DOWNLOADTHREAD_TESTS
//End DownloadThread.Text.H ===========================================================