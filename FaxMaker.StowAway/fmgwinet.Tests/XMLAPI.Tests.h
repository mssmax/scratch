//==============================================================================
//XMLAPI.Tests.H
//==============================================================================
#pragma once
#ifdef PERFORM_XMLAPI_TESTS 
//==============================================================================

#include "UnitTesting.h"
#include "xmlapi.h"
#include "downloadthread.h"
#include "uploadthread.h"
#include "CUtilities_mock.h"
#include "mfg.h"
#include "MSXML2objects_mocks.h"

//==============================================================================

class CXMLAPI_Test: public CXMLAPI
{
public:
	testing::NiceMock<IXMLDOMDocument2_Mock> m_xmlDoc;
	testing::NiceMock<IXMLDOMElement_Mock> m_xmlDocElement;

	CXMLAPI_Test(std::string path, CDownloadThread * pDwnT,
		CUploadThread * pUpT, IMtaConfig* pMta) : CXMLAPI( path, pDwnT, pUpT, pMta)
	{
		ON_CALL(m_xmlDoc, get_documentElement(_)).WillByDefault(testing::DoAll(testing::SetArgPointee<0>(&m_xmlDocElement),testing::Return(S_OK)));
	}

	//XML DOM	
	HRESULT CreateXMLDOMDocument(MSXML2::IXMLDOMDocumentPtr &ppDoc)
	{
		ppDoc= &m_xmlDoc;
		return S_OK;
	}
};

class CXMLAPI_Test_Fakes: public CXMLAPI_Test
{
public:
	CXMLAPI_Test_Fakes(std::string path, CDownloadThread * pDwnT,
		CUploadThread * pUpT, IMtaConfig* pMta) : CXMLAPI_Test( path, pDwnT, pUpT, pMta)
	{}
	// set return to IsValidXML()
	BOOL bIsValidXMLReturn;
	BOOL IsValidXML(LPCSTR lpszPath)
	{
		return bIsValidXMLReturn;
	}

	// set return to ExceedsMaxPathLength()
	BOOL bExceedsReturn;
	BOOL ExceedsMaxPathLength(std::string sXMLPath)
	{ 
		return bExceedsReturn;
	}
};

//==============================================================================

class xmlapi_fxt: public mfgfixture
{
public:
	NiceMock<CUtilities_Mock>	m_utils;	
	NiceMock<IReportsMock>		m_rep;
	NiceMock<CMtaConfigMock>	m_mta;
	NiceMock<CStatusInterfaceMock>  m_pSI;	

	std::string						m_sXmlPath;
	NiceMock<CMtaConfigMock>		m_Mta;  
	NiceMock<CDownloadThread_Mock>	m_DT;
	CUploadThread * 				m_pUT;

	xmlapi_fxt() : m_DT(&m_mta,&m_rep,&m_utils,&m_pSI)	
	{}	
};


class xmlapi_fxt_dt: public xmlapi_fxt
{
public:
	NiceMock<dirfilelist_c_mock> m_dfl;
	NiceMock<CStreamOnFileWithOffset_Mock> sof;	
	CXMLAPI_Test_Fakes *					m_pXmlapi;

	xmlapi_fxt_dt(): sof(0) 
	{}

	virtual void SetUp() 
	{			 
		m_pXmlapi = new CXMLAPI_Test_Fakes("\\xmlapi", &m_DT, m_pUT, &m_Mta);
		SetDirFileListMock(&m_dfl);	
		SetStreamOnFileWithOffsetMock(&sof);

		//set default values
		m_pXmlapi->m_dwXMLProcessResult = 0;
		m_pXmlapi->bExceedsReturn		= FALSE;	
		m_pXmlapi->bIsValidXMLReturn	= TRUE;
	}

	virtual void TearDown()
	{	
		delete m_pXmlapi;
	}
};

class xmlapi_fxt_dt_others: public xmlapi_fxt
{
public:
	CXMLAPI_Test *					m_pXmlapi;

	virtual void SetUp() 
	{			 
		m_pXmlapi = new CXMLAPI_Test("\\xmlapi", &m_DT, m_pUT, &m_Mta);	
	}

	virtual void TearDown()
	{	
		delete m_pXmlapi;
	}
};

class xmlapi_fxt_utprocess: public xmlapi_fxt
{
public:
	CXMLAPI_Test m_Xmlapi;
	testing::NiceMock<CFaxFrmtMessage_Mock> m_ffm;

	xmlapi_fxt_utprocess() : m_Xmlapi("\\xmlapi", &m_DT, m_pUT, &m_Mta)	
	{
		SetFaxFrmtMsgMock(&m_ffm);
	}

	virtual void SetUp()
	{}

	virtual void TearDown()
	{}
};




#endif //PERFORM_XMLAPI_TESTS

//END ==========================================================================