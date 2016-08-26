//other Mocked Classes
//==============================================================================

#pragma once
#include "UnitTesting.h"

//==============================================================================

#include "pop3maildrop_c.h"

class Cpop3maildropMock : public Ipop3maildrop  
{
public:
	Cpop3maildropMock();
	virtual ~Cpop3maildropMock();

	MOCK_METHOD1(setmessage_deleted, int (int index));
	MOCK_METHOD2(getlastservererrorstring, int(LPSTR lpszbuf, int maxlen));
	MOCK_METHOD5(logon, int (LPCSTR lpszmailbox, LPCSTR lpszpassword, LPCSTR lpszserver, WORD port, BOOL bsecure) );
};

void SetIpop3maildropMock(Cpop3maildropMock *);

//==============================================================================

#include "StatusInterface.h"

struct CStatusInterfaceMock  : public IStatusInterface
{
	CStatusInterfaceMock();
	virtual ~CStatusInterfaceMock();

	MOCK_METHOD2(SetItems, void(long i, BOOL bout));

	MOCK_METHOD1(SetMain, void(LPCSTR lpsz));
	MOCK_METHOD1(SetUpload, void(LPCSTR lpsz));
	MOCK_METHOD1(SetDownload, void(LPCSTR lpsz));
	MOCK_METHOD1(SetEmailAddress, void(LPCSTR lpsz));
	MOCK_METHOD1(SetLastFile, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmGateway, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmStatus, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmLinesConfig, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmPendingSMS, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmSendingThread, void(LPCSTR lpsz));
	MOCK_METHOD1(SetGsmRecvThread, void(LPCSTR lpsz));
	MOCK_METHOD1(SetMain, void(DWORD id));
	MOCK_METHOD1(SetUpload, void(DWORD id));
	MOCK_METHOD1(SetDownload, void(DWORD id));
	MOCK_METHOD1(SetEmailAddress, void(DWORD id));
	MOCK_METHOD1(SetLastFile, void(DWORD id));
	MOCK_METHOD1(SetGsmSendingThread, void(DWORD id));
	MOCK_METHOD1(SetGsmRecvThread, void(DWORD id));
	MOCK_METHOD1(SetGsmGateway, void(DWORD id));
	MOCK_METHOD1(SetGsmStatus, void(DWORD id));
	MOCK_METHOD1(SetGsmLinesConfig, void(DWORD id));
	MOCK_METHOD1(SetGsmPendingSMS, void(DWORD id));

	void __cdecl AddToLog(DWORD id, ...){}
	void __cdecl AddToLog(LPCSTR lpszformat, ...){}

	MOCK_METHOD0(GetLogFile, LPCSTR());
	MOCK_METHOD0(updatefile, BOOL());
	MOCK_METHOD0(forcewrite, BOOL());
};

//==============================================================================

#include "MtaConfig.h"

#define mock_getset_long(var)    CString var; MOCK_METHOD0(get_##var, long());	 MOCK_METHOD1(set_##var, void(long val));
#define mock_getset_cstring(var) long var; MOCK_METHOD0(get_##var, CString());	 MOCK_METHOD1(set_##var, void(LPCSTR val));

class CMtaConfigMock : public IMtaConfig
{
public:
	CMtaConfigMock();
	virtual ~CMtaConfigMock();

	MOCK_METHOD0(Init, HRESULT());
	MOCK_METHOD0(Uninit, void());

	MOCK_METHOD2(file_loadconfig, BOOL (LPCSTR lpszpath, BOOL bfrommodulepath ));
	MOCK_METHOD1(loadconfig, BOOL(LPCSTR lpszmachinename));
	MOCK_METHOD1(saveconfig, BOOL(LPCSTR lpszmachinename));

	MOCK_METHOD0(getundeliverabledir , CString ());
	MOCK_METHOD0(isexchange, BOOL());
};

 //==============================================================================

#include "tnef2mime.h"

class CTnef2MimeMock : public tnef2mime::ITnef2Mime
{
public:
	CTnef2MimeMock();
	virtual ~CTnef2MimeMock();

	MOCK_METHOD2(Convert, HRESULT(IStream * pSource, IStream *pDest));
};

void SetTnef2MimeMock(CTnef2MimeMock  *);

//==============================================================================

#include "mfg.h"

#include "CDOobjects_mocks.h"
//==============================================================================

#include <Templates.h>

struct CTemplatesMock : public CTemplates
{
	CTemplatesMock();
	virtual ~CTemplatesMock();

	MOCK_METHOD3(CreateFileFromTemplate, BOOL(LPCSTR lpsztemplate, LPCSTR lpszdestfile, LPCWSTR ppcustomfields[][2]));
};

void SetCTemplatesMock(CTemplatesMock*);
//==============================================================================

#include "EmailMessageEx.h"

class CEmailMessageMock : virtual public IEmailMessage
{
public:
	CEmailMessageMock();
	virtual ~CEmailMessageMock();

	MOCK_METHOD0(NewMessage, HRESULT());
	MOCK_METHOD4(Create, HRESULT(LPCSTR from, LPCSTR sendTo, LPCSTR subject, LPCSTR body));
	MOCK_METHOD4(Create, HRESULT(LPCWSTR from, LPCWSTR sendTo, LPCWSTR subject, LPCWSTR body));
	MOCK_METHOD1(Create, HRESULT(LPCWSTR body));
	MOCK_METHOD1(Create, HRESULT(LPCSTR body));
	MOCK_METHOD1(SetFrom, void(LPCWSTR value));
	MOCK_METHOD0(GetFrom, LPCWSTR());
	MOCK_METHOD1(SetTo, void(LPCWSTR value));
	MOCK_METHOD0(GetTo, LPCWSTR());
	MOCK_METHOD1(SetSubject, void(LPCWSTR value));
	MOCK_METHOD0(GetSubject, LPCWSTR());
	MOCK_METHOD2(SetHeader, void(LPCSTR lpszName, LPCWSTR wszValue));
	MOCK_METHOD2(SetHeader, void(LPCSTR lpszName, LPCSTR lpszValue));
	MOCK_METHOD1(Save, HRESULT(IStream *pStream));
};

class CEmailMessageExMock :  public IEmailMessageEx, public CEmailMessageMock
{
public:
	CEmailMessageExMock();
	virtual ~CEmailMessageExMock();

	MOCK_METHOD3(CreateFromFiles, void(LPCSTR lpszBodyContentType, LPCSTR lpszBodyPath, IEmailMessageEx::attachmentsList_t *pAttachments));
	//MOCK_METHOD6(ConstructEmail, void(strw_c &, str_c *saddress, LPCWSTR lpszalias, LPCWSTR lpszaddress, LPCWSTR lpszdomain/* = NULL*/, BOOL bfilter/* = TRUE*/));
};


//==============================================================================

#include "SmtpSender.h"

class SmtpSenderMock :  public ISmtpSender
{
public:
	SmtpSenderMock();
	virtual ~SmtpSenderMock();

	MOCK_METHOD0(GetLastServerError, virtual char *());
	MOCK_METHOD1(Prepare, void(IEmailMessage *message));
	MOCK_METHOD2(Send, int(IEmailMessage *pMessage, SIZE_T bufferSize/*=650*/));
	MOCK_METHOD2(Send, int(IEmailMessage *pMessage, IStream *pOutputStream));
	MOCK_METHOD1(GetLastError,virtual char *(char *defaultMessage/*=NULL*/));
	MOCK_METHOD1(SetMailServer,int(const char * lpMailServer));
    MOCK_METHOD1(SetMailPort, int(int lMailPort));
    MOCK_METHOD1(SetSSLStartMode, int(int iSSLStartMode));
    MOCK_METHOD1(SetAuthMechanism, int(int iAuthMechanism));
	MOCK_METHOD1(SetUser, int(const char *lpUser));
    MOCK_METHOD1(SetPassword, int(const char *lpPassword));
	MOCK_METHOD1(ProcessQueue, int(const char* lpszQueueDir));
	MOCK_METHOD0(Interrupt, int());
};

void SetSmtpSenderMock(SmtpSenderMock *smtpSender);


//==============================================================================

#include "Reports.h"

struct IReportsMock : public IReports 
{
	IReportsMock();
	virtual ~IReportsMock();

	MOCK_METHOD6(CreateReport,       BOOL(report_type reporttype, LPCWSTR lpszsender, LPCWSTR lpszrecipient, LPCWSTR lpszsubject, LPCWSTR ppcustomfields[][2], LPCSTR lpszremoteid));
	MOCK_METHOD8(CreateSmsReport,    BOOL(report_type reporttype, LPCWSTR lpszrecipient, LPCWSTR lpszoriginalsubject, LPCWSTR lpszstatus, LPCWSTR lpszdescription, LPCWSTR lpsztext, LPCWSTR lpsztextLenght, LPCSTR lpszsmsnumbers));
	MOCK_METHOD6(CreateMtaReport,    BOOL(report_type reporttype, LPCWSTR lpszsender, LPCWSTR lpszrecipient, LPCWSTR lpszsubject, LPCWSTR lpszstatus, LPCWSTR lpszdescription));
	MOCK_METHOD4(CreateBodyReport,   BOOL(LPSTR lpszfilepath, long maxlen, LPCSTR lpsztemplate, LPCWSTR ppcustomfields[][2]));
	MOCK_METHOD3(RenameMessage,      BOOL(LPCSTR lpszsourcepath, BOOL bdeleteonfailure, LPCSTR lpszprefix));
	MOCK_METHOD4(CreateAdminMessage, BOOL(LPCSTR lpszdesription, LPCSTR lpszattachment, DWORD dwerrorcode, LPCSTR lpszSubject));
	MOCK_METHOD1(SetInDir,           void(LPCSTR lpszindir));
};


//==============================================================================

#include <DownloadThread.h>

/*
 *  Mock of CDownloadThread.  Only the methods used in CMainThread were mocked.
 */
struct CDownloadThreadMock : public CDownloadThread
{
	CDownloadThreadMock(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtils, IStatusInterface* pSI);
	virtual ~CDownloadThreadMock();

	// CThinThread
	MOCK_METHOD0(OverrideCycle, void());
};


//==============================================================================

#include <UploadThread.h>

/*
 *  Mock of CUploadThread.  Only the methods used in CMainThread were mocked.
 */
struct CUploadThreadMock : public CUploadThread
{
	CUploadThreadMock(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface* pSi, IEmailMessageEx *pMsg=NULL);
	virtual ~CUploadThreadMock();

	// CThinThread
	MOCK_METHOD0(OverrideCycle, void());
};


//==============================================================================

#include <fmgwinet.h>

/*
 *  Mock of CFmgwinetApp.  Only the methods used in CMainThread were mocked.
 */
class IFmgwinetAppMock : public IFmgwinetApp
{
public:
	IFmgwinetAppMock();
	virtual ~IFmgwinetAppMock();

	// CWinServiceApp
	MOCK_METHOD0(StartThreads, BOOL());
	MOCK_METHOD0(StopThreads,  BOOL());
	MOCK_METHOD0(IsService,    BOOL());
	MOCK_METHOD0(ServiceStart, BOOL());
	MOCK_METHOD0(ServiceStop,  void());

	// IFmgwinetApp
	MOCK_METHOD1(getArchivingDbType, LPCSTR(DLECFG_HANDLE dleconfhandle));

	IMtaConfig* getmtaconfig();
	CDownloadThread* getdownloadthread();
	CUploadThread* getuploadthread();

	// non overridden functions:
	CDownloadThreadMock& GetDownloadThreadMock();
	CUploadThreadMock&   GetUploadThreadMock();

protected:
	IReportsMock			reportsMock;
	CStatusInterfaceMock	statusInterfaceMock;
	CUploadThreadMock		uploadThreadMock;
	CDownloadThreadMock		downloadThreadMock;
};

IFmgwinetAppMock* GetFmgwinetAppMock();
void SetFmgwinetAppMock(IFmgwinetAppMock *);


//==============================================================================

#include <activeds.h>

struct IDirectorySearchMock : public IDirectorySearch 
{
	IDirectorySearchMock();
	~IDirectorySearchMock();

	IUNKNOWN_MOCK();
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, SetSearchPreference, HRESULT(PADS_SEARCHPREF_INFO pSearchPrefs, DWORD dwNumPrefs));
	MOCK_METHOD4_WITH_CALLTYPE(STDMETHODCALLTYPE, ExecuteSearch, HRESULT(LPWSTR pszSearchFilter, LPWSTR *pAttributeNames, DWORD dwNumberAttributes, /*[out]*/ PADS_SEARCH_HANDLE phSearchResult));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, AbandonSearch, HRESULT(ADS_SEARCH_HANDLE phSearchResult));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, GetFirstRow, HRESULT(ADS_SEARCH_HANDLE hSearchResult));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, GetNextRow, HRESULT(ADS_SEARCH_HANDLE hSearchResult));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, GetPreviousRow, HRESULT(ADS_SEARCH_HANDLE hSearchResult));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, GetNextColumnName, HRESULT(ADS_SEARCH_HANDLE hSearchHandle, /*[out]*/ LPWSTR *ppszColumnName));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, GetColumn, HRESULT(ADS_SEARCH_HANDLE hSearchResult, LPWSTR szColumnName, /*[out]*/ PADS_SEARCH_COLUMN pSearchColumn));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, FreeColumn, HRESULT(PADS_SEARCH_COLUMN pSearchColumn));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, CloseSearchHandle, HRESULT(ADS_SEARCH_HANDLE hSearchResult));
};

void SetIDirectorySearchMock(IDirectorySearchMock*);
IDirectorySearchMock* GetIDirectorySearchMock();


//==============================================================================

struct IADsMock : public IADs
{
	IADsMock();
	~IADsMock();

    IUNKNOWN_MOCK();
	IDISPATCH_MOCK();
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_Name, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_Class, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_GUID, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_ADsPath, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_Parent, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD1_WITH_CALLTYPE(STDMETHODCALLTYPE, get_Schema, HRESULT(/*[out]*/ BSTR *retval));
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, GetInfo, HRESULT());
	MOCK_METHOD0_WITH_CALLTYPE(STDMETHODCALLTYPE, SetInfo, HRESULT());
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, Get, HRESULT(BSTR bstrName, /*[out]*/ VARIANT *pvProp));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, Put, HRESULT(BSTR bstrName, VARIANT vProp));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, GetEx, HRESULT(BSTR bstrName, /*[out]*/ VARIANT *pvProp));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, PutEx, HRESULT(long lnControlCode, BSTR bstrName, VARIANT vProp));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, GetInfoEx, HRESULT(VARIANT vProperties, long lnReserved)); 
};

void SetIADsMock(IADsMock*);
IADsMock* GetIADsMock();

//==============================================================================

#include <StreamOnFile.h>

class CStreamOnFileWithOffset_Mock  : public CStreamOnFileWithOffset
{
public:
	CStreamOnFileWithOffset_Mock(ULONG ulOffset);
	virtual ~CStreamOnFileWithOffset_Mock();

	MOCK_METHOD5(InitA, HRESULT (LPCSTR lpszFileName, DWORD dwAccess, DWORD dwShareMode, DWORD dwCreatinDisposition, DWORD dwFlags));
	MOCK_METHOD3_WITH_CALLTYPE(STDMETHODCALLTYPE, Seek, HRESULT (LARGE_INTEGER dlibMove,DWORD dwOrigin,ULARGE_INTEGER *plibNewPosition));
	MOCK_METHOD2_WITH_CALLTYPE(STDMETHODCALLTYPE, Stat, HRESULT (STATSTG *pstatstg, DWORD grfStatFlag));
	MOCK_METHOD4_WITH_CALLTYPE(STDMETHODCALLTYPE, CopyTo, HRESULT (IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten));
	MOCK_METHOD0(Close, void ());
	MOCK_METHOD1(InitHandle, void(HANDLE hFile));
};

void SetStreamOnFileWithOffsetMock(CStreamOnFileWithOffset_Mock *);

//==============================================================================

//Download thread
#include "Downloadthread.h"

class CDownloadThread_Mock  : public CDownloadThread  
{
public:
	CDownloadThread_Mock();
	virtual ~CDownloadThread_Mock();

	CDownloadThread_Mock(IMtaConfig *pMtaConfig, IReports *pReports, IUtilities *pUtil, IStatusInterface* pSI) ;
	MOCK_METHOD0(IsEnding, BOOL());
};

#include "Uploadthread.h"

class CUploadThread_Mock  : public CUploadThread  
{
public:
	CUploadThread_Mock();
	virtual ~CUploadThread_Mock();

	CUploadThread_Mock(IMtaConfig *pMtaConfig, IUtilities *pUtil, IStatusInterface* pSi, IEmailMessageEx *pMsg)
		: CUploadThread(pMtaConfig, pUtil, pSi, pMsg)
	{}

	MOCK_METHOD0(IsEnding, BOOL());
};

//==============================================================================
/*
#include "xmlapi.h"

class CXMLAPI_Mock : public CXMLAPI
{
public:
	CXMLAPI_Mock(std::string path, CDownloadThread * pDwnT, CUploadThread * pUpT, IMtaConfig* pMta);
	virtual ~CXMLAPI_Mock();

	MOCK_METHOD1(IsValidXML, BOOL(LPCSTR lpszPath)); 
	MOCK_METHOD1(ExceedsMaxPathLength, BOOL(std::string sXMLPath));	
	MOCK_METHOD3(CreateXMLAPIReport,  void (LPCSTR lpszInputName, int error, LPCSTR lpszDescription));		
	MOCK_METHOD1(UT_ProcessXMLAPI, void (IGenericFaxFrmtMessage *pfm));
};
*/