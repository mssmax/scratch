
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <tchar.h>
#include <strsafe.h>
#include <WinCrypt.h>
#include <comdef.h>
#include <vector>
#include <string>
#include <ESEEngine.h>

extern CDBEngine g_DB;

std::wstring ConvA2W(LPCSTR lpsz);
std::string ConvW2A(LPCWSTR lpsz);

HRESULT EncryptPassword(LPCTSTR lpszPassword, IStream *pStrm);
HRESULT DecryptPassword(IStream *pStrm, LPTSTR lpszPassword);

#define CALL_JET(err) \
	if(err < 0) \
    { \
		CHAR szErrorMsg[1024] = {0}; \
		g_DB.GetErrorString(err, szErrorMsg, sizeof(szErrorMsg)); \
		CString s; \
		s.Format(_T("Keys database returned an error [%S]"), szErrorMsg); \
		::MessageBox(::GetForegroundWindow(), s, _T("Error"), MB_OK); \
		goto EXIT; \
    }

static char* s_Columns[] = {
	"tb_keys_app",
	"tb_keys_keyname",
	"tb_keys_user",
	"tb_keys_password"
};

