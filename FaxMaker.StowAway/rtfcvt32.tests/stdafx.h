// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#include <afxwin.h>         // MFC core and standard components

#include <afxcmn.h>			// MFC support for Windows 95 Common Controls

#include <afxrich.h>		// MFC rich edit classes

#include "gfihlps.h"
using namespace gfihlps;

typedef struct rtfcustom_s
{   
public:
	LPCSTR lpszrtfpath;
	LPCSTR lpszbodypath;
	LPCSTR * ppfields;
	LPCSTR * ppvalues;

public:
	hlp_implement_struct(rtfcustom_s);

} rtfcustom_t;

#define TYPE_TXT		0
#define TYPE_RTF		1
#define TYPE_TPT		2
#define TYPE_TPTSTRUCT	3
#define TYPE_CUSTOM		4
