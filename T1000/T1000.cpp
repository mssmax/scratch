#include "stdafx.h"
#include "T1000.h"
#include "T1000Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CT1000App

BEGIN_MESSAGE_MAP(CT1000App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CT1000App construction

CT1000App::CT1000App()
{
}


// The one and only CT1000App object

CT1000App theApp;


// CT1000App initialization

BOOL CT1000App::InitInstance()
{
	CWinApp::InitInstance();

	RegisterAutoRun();

	WriteDefaults();

	CT1000Dlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CT1000App::RegisterAutoRun()
{
	HKEY hKey = 0;
	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR szProcName[_MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szProcName, _MAX_PATH);
		lRes = RegSetValueEx(hKey, _T("T1000"), 0, REG_SZ, reinterpret_cast<LPBYTE>(szProcName), _countof(szProcName));
		RegCloseKey(hKey);
	}
}

void CT1000App::WriteDefaults()
{
	HKEY hKey = 0;
	DWORD dwDisposition = 0;
	LONG lRes = RegCreateKeyEx(HKEY_CURRENT_USER, 
		_T("SOFTWARE\\T1000"), 
		0, 
		0, 
		0,
		KEY_WRITE, 
		0, &hKey,
		&dwDisposition);
	if (lRes == ERROR_SUCCESS)
	{
		if (dwDisposition == REG_CREATED_NEW_KEY)
		{
			HKEY hKeyTargets = 0;
			LPTSTR szDusk = _T("17:30");
			LPTSTR szDawn = _T("09:00");
			RegSetValueEx(hKey, _T("Dusk"), 0, REG_SZ, reinterpret_cast<LPBYTE>(szDusk), (sizeof(szDusk) + 1) * sizeof(TCHAR));
			RegSetValueEx(hKey, _T("Dawn"), 0, REG_SZ, reinterpret_cast<LPBYTE>(szDawn), (sizeof(szDawn) + 1) * sizeof(TCHAR));
			RegCreateKeyEx(hKey, _T("Targets"), 0, 0, 0, KEY_CREATE_SUB_KEY, 0, &hKeyTargets, 0);
			RegCloseKey(hKeyTargets);
		}
		RegCloseKey(hKey);
	}
}
