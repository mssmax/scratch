/*
 *  Pre-compiler header.
 */
#pragma once

#define STRICT
#define _WIN32_WINNT 0x0501		// Windows XP or later
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
// tests to perform
#define PERFORM_CBASE64_TESTS
#define PERFORM_GFIRULE_TESTS
#define PERFORM_ENUMGFIRULES_TESTS
#define PERFORM_GFIDB_TESTS

#include "UnitTesting.h"

// ATL stuff
#include <atlbase.h>
extern CComModule	_Module;
#include <atlcom.h>
#include <comdef.h>
#include <comutil.h>
#include <comip.h>
#include <fmcfgdb.h>

#include <gfihlps.h>
using namespace gfihlps;
#include <fmstore.h>
#include <fmdevcfg.h>
#include <msntypes.h>
#include <dbasecmn.h>
#include <gfi_log_c.h>
#include <fmcfgdb_common.h>
#include <FmDbRuleHdr.h>
#include <shared/gfidbdef.h>
#import <gfimdb2.dll> rename_namespace("GFIMDB")

// log stuff
extern gfi_log_c	g_gfilog;
#define LTRACE  g_gfilog.logerror
#define LTRACEW g_gfilog.logwarning
#define LTRACEI g_gfilog.loginfo