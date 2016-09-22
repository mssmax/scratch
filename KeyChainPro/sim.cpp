#include "stdafx.h"
#include "sim.h"
#include "simDlg.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CSimApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CSimApp::CSimApp()
{
	m_hCryptProv = 0;
}

CSimApp theApp;

BOOL CSimApp::InitInstance()
{
	CWinApp::InitInstance();

	SetRegistryKey(_T("KeyChainPro"));

	RegisterAutoRun();

	// TODO: Consider refactoring this part of code into InitKey
	if (!CryptAcquireContext(&m_hCryptProv, 0, 0, PROV_RSA_AES, 0))
	{
		if (GetLastError() == NTE_BAD_KEYSET)
		{
			if (!CryptAcquireContext(&m_hCryptProv, 0, 0, PROV_RSA_AES, CRYPT_NEWKEYSET))
			{
				return FALSE;

			}
		}
		else
		{
			return FALSE;
		}
	}

	if (!InitKey())
	{
		return FALSE;
	}

	JET_ERR e = InitDatabase();
	if (e < 0)
	{
		CString s;
		s.Format(L"Keychain database cannot be opened or created, err[%d]", e);
		AfxMessageBox(s, MB_OK);
		return FALSE;
	}

	CSimDlg dlg;
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

	return FALSE;
}

BOOL CSimApp::ExitInstance()
{
	if (m_hCryptProv)
	{
		CryptReleaseContext(m_hCryptProv, 0);
		m_hCryptProv = 0;
	}

	return TRUE;
}

void CSimApp::RegisterAutoRun()
{
	HKEY hKey = 0;
	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_WRITE, &hKey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR szProcName[_MAX_PATH] = { 0 };
		GetModuleFileName(NULL, szProcName, _MAX_PATH);
		lRes = RegSetValueEx(hKey, _T("KeyChainPro"), 0, REG_SZ, reinterpret_cast<LPBYTE>(szProcName), _countof(szProcName));
		RegCloseKey(hKey);
	}
}

BOOL CSimApp::InitKey()
{
	CInputDlg dlg(AfxGetMainWnd(), _T("Enter master password"), TRUE);
	if (dlg.DoModal() == IDOK)
	{
		TCHAR s[512] = { 0 };
		StringCbCopy(s, sizeof(s), dlg.m_sInput);
		HCRYPTHASH hHash = 0;
		if (CryptCreateHash(m_hCryptProv, CALG_SHA_256, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, reinterpret_cast<BYTE*>(s), lstrlen(s) * sizeof(TCHAR), 0))
			{
				if (CryptDeriveKey(m_hCryptProv, CALG_AES_256, hHash, CRYPT_EXPORTABLE, &g_hKey))
				{
					CryptDestroyHash(hHash);
					return TRUE;
				}
			}
			CryptDestroyHash(hHash);
		}
	}

	return FALSE;
}


LPCSTR lpszSchema = "xml://<schema version=\"1\">" \
"<table name=\"tb_keys\">" \
	"<column name=\"tb_keys_app\" type=\"text\" indexed=\"yes\"/>" \
	"<column name=\"tb_keys_keyname\" type=\"text\" size=\"128\" indexed=\"yes\" unique=\"yes\"/>" \
	"<column name=\"tb_keys_user\" type=\"text\" indexed=\"yes\"/>" \
	"<column name=\"tb_keys_password\" type=\"binary\"/>" \
"</table>" \
"<table name=\"tb_copypaste\">" \
	"<column name=\"tb_cp_app\" type=\"text\" indexed=\"yes\"/>" \
	"<column name=\"tb_cp_keyname\" type=\"text\" size=\"128\" indexed=\"yes\"/>" \
	"<column name=\"tb_cp_text\" type=\"longtext\"/>" \
"</table>" \
"</schema>";


JET_ERR CSimApp::InitDatabase()
{
	USES_CONVERSION;
	CString sPath(_T("keychain"));

	CreateDirectory(sPath, NULL);
	JET_ERR e = g_DB.Init(T2A(sPath), "kcp");
	if (e < 0)
	{
		return e;
	}

	sPath.Append(_T("\\keys.edb"));

	DWORD dwAttr = GetFileAttributes(sPath);
	if (dwAttr == -1)
	{
		e = g_DB.CreateDatabase(T2A(sPath), lpszSchema);
		if (e < 0)
		{
			return e;
		}
	}
	else
	{
		e = g_DB.OpenDatabase(T2A(sPath), FALSE);
	}

	return e;
}