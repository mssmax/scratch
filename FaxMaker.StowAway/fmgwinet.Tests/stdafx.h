//==============================================================================
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//==============================================================================
#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
//==============================================================================
#include <stdio.h>
#include <tchar.h>

#include <afxsock.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <mlang.h>
#pragma warning(disable:4786)

#include "gfihlps.h"
using namespace gfihlps;

#define NO_USERUTIL_LIB		// do not link with userutil.lib, in userutil.h
#define NO_USERUTIL_IMPORT
#define FMCFGDB_USEGFILOG
#include "fmgfidb_c.h"

#include <filename_gen.h>

#define HTMLSLAYER_API		// to omit the pragma comment htmlslayer.lib, in html_slayer.h

//------------------------------------------------------------------------------
// TESTS to perform...

#define PERFORM_LICENSETHREAD_TESTS
#define PERFORM_REPORTS_TESTS
#define PERFORM_DOWNLOADTHREAD_TESTS
#define PERFORM_RSXMESSAGE_TESTS
#define PERFORM_XMLAPI_TESTS
#define PERFORM_UPLOADTHREAD_TESTS
#define PERFORM_GSMTHREAD_TESTS
#define PERFORM_UPLOADTHREAD_TESTS
#define PERFORM_UTILITIES_TESTS


//==============================================================================