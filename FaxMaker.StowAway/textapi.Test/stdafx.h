// stdafx.h 
//==============================================================================
#pragma once

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						
//==============================================================================
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#include "systemcalls.h"
#include "gfihlps.h"
using namespace gfihlps;

//------------------------------------------------------------------------------
//to omit pragma comments:
#define HTMLSLAYER_API		//to omit the pragma comment htmlslayer.lib, in html_slayer.h 
#define NO_USERUTIL_LIB	

//------------------------------------------------------------------------------
#define FMCFGDB_USEGFILOG

#include <mfg.h>
#include <mfg_gsm.h>
#include <mfg_htmlslayer.h>
#include "mocks.h"
#include "textwriter.mock.h"
#include "textreader.mock.h"
#include "html_slayer.mock.h"
#include "fmgfidb_c.mock.h"
#include "userutil.mock.h"

//END ==========================================================================
