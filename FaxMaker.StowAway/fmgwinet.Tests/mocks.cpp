#include "StdAfx.h"
#include "mocks.h"


Cpop3maildropMock::Cpop3maildropMock()
{}

Cpop3maildropMock::~Cpop3maildropMock()
{}

//==============================================================================

CStatusInterfaceMock::CStatusInterfaceMock()
{}

CStatusInterfaceMock::~CStatusInterfaceMock()
{}


//==============================================================================

CMtaConfigMock::CMtaConfigMock()
{}

CMtaConfigMock::~CMtaConfigMock()
{}


//==============================================================================

CTnef2MimeMock::CTnef2MimeMock()
{}

CTnef2MimeMock::~CTnef2MimeMock()
{}


//==============================================================================

CTemplatesMock::CTemplatesMock()
{
	ON_CALL(*this, CreateFileFromTemplate(_,_,_)).WillByDefault(Return(TRUE));
}

CTemplatesMock::~CTemplatesMock()
{}


//==============================================================================

CEmailMessageMock::CEmailMessageMock()
{}

CEmailMessageMock::~CEmailMessageMock()
{}


//==============================================================================

CEmailMessageExMock::CEmailMessageExMock()
{}

CEmailMessageExMock::~CEmailMessageExMock()
{}

//==============================================================================

SmtpSenderMock::SmtpSenderMock()
{}

SmtpSenderMock::~SmtpSenderMock()
{}


//==============================================================================

IReportsMock::IReportsMock()
{
	ON_CALL(*this, CreateReport(_,_,_,_,_,_)).WillByDefault(Return(TRUE));
	ON_CALL(*this, CreateSmsReport(_,_,_,_,_,_,_,_)).WillByDefault(Return(TRUE));
	ON_CALL(*this, CreateMtaReport(_,_,_,_,_,_)).WillByDefault(Return(TRUE));
	ON_CALL(*this, CreateBodyReport(_,_,_,_)).WillByDefault(Return(TRUE));
	ON_CALL(*this, RenameMessage(_,_,_)).WillByDefault(Return(TRUE));
	ON_CALL(*this, CreateAdminMessage(_,_,_,_)).WillByDefault(Return(TRUE));
}

IReportsMock::~IReportsMock()
{}


//==============================================================================

CDownloadThreadMock::CDownloadThreadMock(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtils, IStatusInterface* pSI)
	: CDownloadThread(pMtaConfig, pReports, pUtils, pSI)
{}

CDownloadThreadMock::~CDownloadThreadMock()
{}


//==============================================================================

CUploadThreadMock::CUploadThreadMock(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface* pSi, IEmailMessageEx *pMsg)
	: CUploadThread(pMtaConfig, pUtil, pSi, pMsg)
{}

CUploadThreadMock::~CUploadThreadMock()
{}


//==============================================================================

IFmgwinetAppMock::IFmgwinetAppMock()
	:	downloadThreadMock(GetMtaConfig(), &reportsMock, GetUtilities(), &statusInterfaceMock),
		uploadThreadMock(GetMtaConfig(), GetUtilities(), &statusInterfaceMock)
{
	// CWinServiceApp
	ON_CALL(*this, StartThreads()).WillByDefault(Return(TRUE));
	ON_CALL(*this, StopThreads()).WillByDefault(Return(TRUE));
	ON_CALL(*this, IsService()).WillByDefault(Return(TRUE));
	ON_CALL(*this, ServiceStart()).WillByDefault(Return(TRUE));

	// CFmgwinetApp
	ON_CALL(*this, getArchivingDbType(_)).WillByDefault(Return(""));
}

IFmgwinetAppMock::~IFmgwinetAppMock()
{}

IMtaConfig* IFmgwinetAppMock::getmtaconfig()
{	return GetMtaConfig();		}	// returns mock of MtaConfig

CDownloadThread* IFmgwinetAppMock::getdownloadthread()
{	return &downloadThreadMock;	}	// returns mock of CDownloadThread

CUploadThread* IFmgwinetAppMock::getuploadthread()
{	return &uploadThreadMock;	}	// returns mock of CUploadThread

CDownloadThreadMock& IFmgwinetAppMock::GetDownloadThreadMock()
{	return downloadThreadMock;	}	// returns actual mock of CDownloadThread

CUploadThreadMock& IFmgwinetAppMock::GetUploadThreadMock()
{	return uploadThreadMock;	}	// returns actual mock of CUploadThread


//==============================================================================

IDirectorySearchMock::IDirectorySearchMock()
{
	ON_CALL(*this, SetSearchPreference(_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, ExecuteSearch(_,_,_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, AbandonSearch(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetFirstRow(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetNextRow(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetPreviousRow(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetNextColumnName(_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetColumn(_,_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, FreeColumn(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, CloseSearchHandle(_)).WillByDefault(Return(S_OK));
}

IDirectorySearchMock::~IDirectorySearchMock()
{}


//==============================================================================

IADsMock::IADsMock()
{
	ON_CALL(*this, get_Name(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, get_Class(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, get_GUID(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, get_ADsPath(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, get_Parent(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, get_Schema(_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetInfo()).WillByDefault(Return(S_OK));
	ON_CALL(*this, SetInfo()).WillByDefault(Return(S_OK));
	ON_CALL(*this, Get(_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, Put(_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetEx(_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, PutEx(_,_,_)).WillByDefault(Return(S_OK));
	ON_CALL(*this, GetInfoEx(_,_)).WillByDefault(Return(S_OK)); 
}

IADsMock::~IADsMock()
{}


//==============================================================================

CStreamOnFileWithOffset_Mock::CStreamOnFileWithOffset_Mock(ULONG ulOffset) : CStreamOnFileWithOffset(ulOffset)
{}

CStreamOnFileWithOffset_Mock::~CStreamOnFileWithOffset_Mock()
{}


//==============================================================================

CDownloadThread_Mock::CDownloadThread_Mock(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtil, IStatusInterface* pSI) 		
	: CDownloadThread(pMtaConfig,pReports, pUtil, pSI)
{
	ON_CALL(*this, IsEnding()).WillByDefault(Return(FALSE)); 
}

CDownloadThread_Mock::~CDownloadThread_Mock()
{}


//==============================================================================
/*
CXMLAPI_Mock::CXMLAPI_Mock(std::string path, CDownloadThread * pDwnT, CUploadThread * pUpT, IMtaConfig* pMta)
	: CXMLAPI(path, pDwnT, pUpT, pMta)
{}

CXMLAPI_Mock::~CXMLAPI_Mock()
{}
*/