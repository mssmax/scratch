
// T1000.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CT1000App:
// See T1000.cpp for the implementation of this class
//

class CT1000App : public CWinApp
{
protected:
	
	void RegisterAutoRun();
	void WriteDefaults();

public:
	CT1000App();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CT1000App theApp;