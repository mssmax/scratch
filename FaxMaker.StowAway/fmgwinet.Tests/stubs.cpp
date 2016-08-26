// STUBS 
//==============================================================================
#include "stdafx.h"

///=============================================================================
// Stub of stdhlp_.c
///=============================================================================
#include "strhlp_c.h"

void strhlp_c::ltrim(LPSTR pbuf) { return;}
void strhlp_c::rtrim(LPSTR pbuf){ return;}
void strhlp_c::trim(LPSTR pbuf){ return;}
bool strhlp_c::ischarspace(char ch){ return true;}

void strhlp_c::ltrim(LPWSTR pbuf) { return;}
void strhlp_c::rtrim(LPWSTR pbuf) { return;}
void strhlp_c::trim(LPWSTR pbuf) { return;}
bool strhlp_c::ischarspace(wchar_t ch) { return true;} 

///=============================================================================
// Stub of pubcontacts.cpp
///=============================================================================

bool pubcontacts_find(LPCWSTR lpszemailaddress, personalinfo_t * pinfo){ return true;}

///=============================================================================
// Stub of filename_gen.cpp
///=============================================================================

HRESULT __stdcall CreateFileNameA(LPCSTR lpszExtension, LPSTR lpszBuf, int nBufferSize)
	{ return S_OK;}


///=============================================================================
// Stub of CMIMEENTITY
///=============================================================================

#include "mimelib.h"

//returns values:
BOOL bMultiPart = TRUE;
LPCSTR contenttype1 = "";
LPCSTR contenttype2 = "";
int contenttype_index = 0;
LPCWSTR filename = L"";

//stubs functions:
CHeaderFieldCollection CMimeEntity::GetHeader(LPCSTR lpszName){CHeaderFieldCollection a; return a;}
CHeaderFieldCollection CMimeEntity::GetHeaders(){ CHeaderFieldCollection a; return a;}
void CMimeEntity::SetHeader(LPCSTR lpszName, LPCWSTR wszValue){}
void CMimeEntity::SetHeader(LPCSTR lpszName, LPCSTR wszValue){}
void CMimeEntity::AddHeader(LPCSTR lpszName, LPCWSTR wszValue){}
void CMimeEntity::AddHeader(LPCSTR lpszName, LPCSTR wszValue){}
BOOL CMimeEntity::RemoveHeader(LPCSTR lpszName){ return TRUE;}
CMimeEntityCollection CMimeEntity::Children(){ CMimeEntityCollection a; return  a;}
BOOL CMimeEntity::IsMultipart(){ return bMultiPart;}
LPCWSTR CMimeEntity::FileName(){ return filename;}
void CMimeEntity::SetAttachmentName(LPCWSTR lpszFileName, BOOL bInline){}
LPCSTR CMimeEntity::ContentType()
{ 
	if (contenttype_index == 0) {contenttype_index++; return contenttype1;}
	if (contenttype_index == 1) return contenttype2;
	return "";
}
HRESULT CMimeEntity::GetData(IStream **ppStream){return S_OK;}
CMimeEntity * CMimeEntity::AddBodyPart(LPCSTR lpszContentType){ return NULL;}
BOOL CMimeEntity::RemoveBodyPart(size_t index){ return TRUE;}
LPCSTR CMimeEntity::Charset(){return ""; }


///=============================================================================
// Stub of CMimeEntityCollection
///=============================================================================

size_t childrenlength =  0;

CMimeEntityCollection::CMimeEntityCollection(){}
CMimeEntityCollection::~CMimeEntityCollection(){}
size_t CMimeEntityCollection::Length() const{ return childrenlength;}
CMimeEntity * CMimeEntityCollection::operator [](size_t index) const{ return NULL;}



///=============================================================================
// Stub of CMimeMessage
///=============================================================================

HRESULT hrCreate = S_OK;
HRESULT hrLoad = S_OK;

HRESULT CMimeMessage::Create(CMimeMessage **ppMsg, ULONG_PTR ulContext){return hrCreate;}
HRESULT CMimeMessage::Destroy(CMimeMessage *pMsg){return S_OK;}
HRESULT CMimeMessage::Load(IStream *pStream){return hrLoad;}
HRESULT CMimeMessage::Save(IStream *pStream, BOOL bStuffDot){return S_OK;}
CMimeEntity * CMimeMessage::AddAttachment(LPCSTR lpszContentType, LPCWSTR lpszFileName, BOOL bIsInline){ return NULL;}
BOOL CMimeMessage::get_MimeFormatted(){ return TRUE;}
void CMimeMessage::put_MimeFormatted(BOOL bMimeFormatted){}


///=============================================================================
// Stub of CHeaderFieldCollection
///=============================================================================

CHeaderFieldCollection::CHeaderFieldCollection(){}
CHeaderFieldCollection::~CHeaderFieldCollection(){}
size_t CHeaderFieldCollection::Length() const{ return 1; }
CHeaderField * CHeaderFieldCollection::operator [](size_t index) const{ return NULL;}


///=============================================================================
// Stub of CHeaderField
///=============================================================================
LPCWSTR CHeaderField::GetValue() { return L""; };

///=============================================================================
// Stubs of mimelib globals
///=============================================================================

BOOL bInitMimeLib= TRUE;

HRESULT __stdcall InitMimeLibA(LPCSTR lpszTempPath, ULONG_PTR *pContext){return bInitMimeLib;}
HRESULT __stdcall InitMimeLibW(LPCWSTR lpszTempPath, ULONG_PTR *pContext){return S_OK;}
HRESULT __stdcall UninitMimeLib(ULONG_PTR ulContext){ return S_OK;}


// TODO:  DELETE THIS
HRESULT __stdcall UserUtil_CountFAXmakerUsers(FMCFGDBLib::IGFIRULEDBPtr ipConfigDB, 
											  GFIUSERDBLib::IGFIUserLocatorPtr ipUserLocator, 
											  DWORD dwDirectoryType,	// AD or EX55
											  LPCWSTR lpwszRule,		// FMRULE_USERS or FMRULE_ADMINS
											  LPCWSTR lpwszNewUser,		// usually NULL
											  LPDWORD lpdwUserCount,	// returns count of users
											  LPDWORD lpdwGroupCount)	// returns count of groups
{	return S_OK;	}


HRESULT __stdcall UserUtil_IsFAXmakerUserList(LPCWSTR lpwszUserID, char** UserGroupIDs, FMCFGDBLib::IGFIRULEDBPtr pConfigDB, LPCWSTR lpwszRule, USERINFO **ppUserInfo, PBOOL pbUserInfoReturned)
{	return S_OK;	}

HRESULT __stdcall UserUtil_IsFAXmakerUserEx(LPCWSTR lpwszUserID, FMCFGDBLib::IGFIRULEDBPtr ipConfigDB, GFIUSERDBLib::IGFIUserLocatorPtr ipUserLocator, DWORD dwDirectoryType, LPCWSTR lpwszRule, USERINFO **ppUserInfo, PBOOL pbUserInfoReturned)
{	return S_OK;	}


///=============================================================================
// Stubs of fmarchiver
///=============================================================================

#include <fmarchiver.h>

BOOL __cdecl fmarchiver_archive_init()
{	return TRUE;	}

BOOL __cdecl fmarchiver_archive_term()
{	return TRUE;	}

BOOL __cdecl fmarchiver_archivefax_in(fmarchivefax_t * pfmarch)
{	return TRUE;	}

BOOL __cdecl fmarchiver_archivefax_out(fmarchivefax_t * pfmarch)
{	return TRUE;	}

BOOL __cdecl fmarchiver_archivesms_in(fmarchivesms_t * pfmarch)
{	return TRUE;	}

BOOL __cdecl fmarchiver_archivesms_out(fmarchivesms_t * pfmarch)
{	return TRUE;	}

BOOL __cdecl fmarchiver_manage_init()
{	return TRUE;	}

BOOL __cdecl fmarchiver_manage_term()
{	return TRUE;	}


///=============================================================================
// Stubs of Active Directory API
///=============================================================================

#include <activeds.h>
#include "mocks.h"

extern const IID IID_IDirectorySearch = __uuidof(IDirectorySearch);
extern const IID IID_IADs = __uuidof(IADs);

HRESULT WINAPI ADsOpenObject(LPCWSTR lpszPathName, LPCWSTR lpszUserName, LPCWSTR lpszPassword, DWORD dwReserved, REFIID riid, void FAR * FAR * ppObject)
{
	if (riid == IID_IDirectorySearch) {
		*ppObject = GetIDirectorySearchMock();
		return S_OK;
	}
	else if (riid == IID_IADs) {
		*ppObject = GetIADsMock();
		return S_OK;
	}
	
	return E_NOINTERFACE;
}


///=============================================================================
// Stubs of DLECFGGetSettings
///=============================================================================

#include "dleconf.h"

DLECFG DLECFGInit(LPDLECFG_HANDLE phndl, LPCTSTR lpszProdDir, LPCTSTR lpszHelpFile,
				  HWND hParentWnd, UINT nDLEID,  LPCTSTR szDBName)
{	return TRUE;	}

DLECFG DLECFGGetSettings(DLECFG_HANDLE hndl, DLECFGSettings* pSettings)
{	return TRUE;	}

DLECFG DLECFGInvokeDialog(DLECFG_HANDLE hndl, int& nRet)
{	return TRUE;	}

DLECFG DLECFGCommitChanges(DLECFG_HANDLE hndl)
{	return TRUE;	}

DLECFG DLECFGUninit(DLECFG_HANDLE hndl)
{	return TRUE;	}


///=============================================================================
// Stubs of POPS
///=============================================================================
void* IPWORKSSSL_CALL POPS_Create(PIPWORKSSSL_CALLBACK lpSink, void *lpContext, char *lpOemKey)
{}
int   IPWORKSSSL_CALL POPS_Destroy(void *lpObj){
	return 0;
}


//End ==========================================================================