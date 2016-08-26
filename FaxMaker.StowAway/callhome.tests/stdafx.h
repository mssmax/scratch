// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						



#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxwin.h>         // MFC core and standard components


//#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>


#include "gfihlps.h"
using namespace gfihlps;
#define FMCFGDB_USEGFILOG
#include "fmgfidb_c.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"


#define LTRACE g_gfilog.logerror
#define LTRACEW g_gfilog.logwarning
#define LTRACEI g_gfilog.loginfo