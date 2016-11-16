#include "stdafx.h"
#include "sim.h"
#include "simDlg.h"
#include "MyKeysDlg.h"
#include "MyCopyPaste.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_CALLBACK WM_APP + 1

#define HK_PASTE_CREDENTIALS 666
#define HK_RECORD_CREDENTIALS 667
#define HK_CHOOSE_KEY 668
#define HK_COPYPASTER 669
#define HK_PASTEPWD 670
#define HK_SHOWKEYS 671
#define HK_PASSWORD2CLIP 672

CSimDlg::CSimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_SIM_DIALOG, pParent)
	, m_sAppTitle(_T(""))
	, m_sPassword(_T(""))
	, m_sUserName(_T(""))
	, m_sKeyName(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bVisible = FALSE;
}

void CSimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_APPTITLE, m_sAppTitle);
	DDX_Text(pDX, IDC_PASSWORD, m_sPassword);
	DDX_Text(pDX, IDC_USERNAME, m_sUserName);
	DDX_Text(pDX, IDC_KEYNAME, m_sKeyName);
}

BEGIN_MESSAGE_MAP(CSimDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_WINDOWPOSCHANGING()
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_BN_CLICKED(IDC_EXIT, OnExitClick)
	ON_MESSAGE(WM_CALLBACK, OnCallback)
	ON_COMMAND(IDC_EXIT, OnExit)
	ON_COMMAND(IDC_MYKEYS, OnMyKeys)
	ON_COMMAND(ID_1_HELP, &CSimDlg::OnHelp)
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()


// CSimDlg message handlers

BOOL CSimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	BOOL b = RegisterHotKey(GetSafeHwnd(), HK_PASTE_CREDENTIALS, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'V');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for pasting failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_RECORD_CREDENTIALS, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'R');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for recording failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_CHOOSE_KEY, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'K');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for key navigation failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_COPYPASTER, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'W');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for copy/paster failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_PASTEPWD, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'P');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for password only failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_SHOWKEYS, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'L');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for show keys failed"));
	}

	b = RegisterHotKey(GetSafeHwnd(), HK_PASSWORD2CLIP, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 'C');
	if (!b)
	{
		OutputDebugString(_T("Sim: Registering the hotkey for password2clip failed"));
	}

	NOTIFYICONDATA nimData = { 0 };
	nimData.cbSize = sizeof(nimData);
	nimData.hWnd = GetSafeHwnd();
	nimData.hIcon = m_hIcon;
	StringCbCopy(nimData.szTip, sizeof(nimData.szTip), _T("KeyChain Pro"));
	nimData.uCallbackMessage = WM_CALLBACK;
	nimData.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	b = Shell_NotifyIcon(NIM_ADD, &nimData);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CSimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimDlg::PasteCredentials(BOOL bPasswordOnly)
{
	USES_CONVERSION;
	HRESULT hr;
	IStreamPtr spPassStrm;
	CString sAppTitle;
	CWnd *wnd = GetForegroundWindow();
	wnd->GetWindowText(sAppTitle);
	CHAR szUserName[256] = { 0 };
	TCHAR szPassword[256] = { 0 };
	CJetTable keysTable;
	CALL_JET(g_DB.GetTable("tb_keys", keysTable));
	CALL_JET(keysTable.BeginTransaction());

	/*JET_ERR err = keysTable.Select()
		.Where("tb_keys_app", OP_EQ, T2A(sAppTitle))
		.Done();
	*/
	JET_ERR err = keysTable.Select("tb_keys_app", OP_EQ, T2A(sAppTitle));
	//JET_ERR err = keysTable.Select()
	//	.ByRange("tb_keys_app", T2A(sAppTitle), T2A(sAppTitle))
	//	.Done();
	if (err < 0)
	{ 
		CHAR szErrorMsg[1024] = { 0 }; 
		g_DB.GetErrorString(err, szErrorMsg, sizeof(szErrorMsg)); 
		CString s; 
		s.Format(_T("Searching for app [%s] resulted in error [%S].\n\nWould you like to record credentials ?"), sAppTitle, szErrorMsg); 
		int msgRes = ::MessageBox(wnd->GetSafeHwnd(), s, _T("Error"), MB_YESNO);
		if (msgRes == IDYES)
		{
			RecordCredentials();
		}

		keysTable.CommitTransaction();

		return; 
	}
	else
	{
		ULONG ulRecCount = 0;
		err = JetIndexRecordCount(g_DB.GetSessionID(), keysTable, &ulRecCount, 0);
		if (err >= 0)
		{
			if (ulRecCount > 1)
			{
				CMyKeysDlg dlg(wnd, keysTable);
				int iRes = dlg.DoModal();
				if (iRes == IDOK)
				{
					CALL_JET(keysTable.Select(
						"tb_keys_keyname", 
						OP_EQ, 
						ConvW2A(dlg.GetSelectedKeyName()).c_str()));
				}
				else
				{
					goto EXIT;
				}
			}
		}
		else
		{
			CALL_JET(err);
		}
	}

	hr = CreateStreamOnHGlobal(0, TRUE, &spPassStrm);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to create stream for password, err[0x%X]"), hr);
		::MessageBox(wnd->GetSafeHwnd(), s, _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	CALL_JET(keysTable.GetColumn("tb_keys_user", szUserName, sizeof(szUserName)));
	CALL_JET(keysTable.GetColumn("tb_keys_password", spPassStrm));

	CALL_JET(keysTable.CommitTransaction());

	hr = DecryptPassword(spPassStrm, szPassword);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to decrypt password, err[0x%X]"), hr);
		::MessageBox(wnd->GetSafeHwnd(), s, _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	// Sleeps aren't the best way to control timing, but it appears
	// Windows is not capable of handling input fast enough ( probably due to thread switching ).
	if (!bPasswordOnly)
	{
		SendString(A2T(szUserName), TRUE);
		Sleep(200);
		SendCode(9);
		Sleep(200);
	}
	SendString(szPassword, FALSE);

EXIT:
	;
}

void CSimDlg::FindKeyAndPaste()
{
}

void CSimDlg::SendString(LPCTSTR lpszString, BOOL bDoPause)
{
//	BlockInput(TRUE);
	int iSize = lstrlen(lpszString);
	for (int i = 0; i < iSize; i++)
	{
		// skipping line feed character because it gets translated into CRLF on its own
		// causing extra new line to be spilled in the in process
		// this is mainly required by copy-paster funcionality
		if (lpszString[i] == 0x0a)
		{
			continue;
		}
		INPUT inp[2] = { 0 };
		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.wScan = lpszString[i];
		inp[0].ki.dwFlags = KEYEVENTF_UNICODE;

		UINT res = SendInput(1, inp, sizeof(INPUT));
		if (bDoPause)
		{
			Sleep(50);
		}

		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.wScan = lpszString[i];
		inp[0].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;

		res = SendInput(1, inp, sizeof(INPUT));
		if (bDoPause)
		{
			Sleep(50);
		}
	}
//	BlockInput(FALSE);
}

void CSimDlg::SendCode(WORD wCode, BOOL bDoPause)
{
	INPUT inp[2] = { 0 };
	ZeroMemory(inp, sizeof(inp));
	inp[0].type = INPUT_KEYBOARD;
	inp[0].ki.wVk = wCode;
	inp[0].ki.wScan = MapVirtualKey(wCode, 0);
	UINT res = SendInput(1, inp, sizeof(INPUT));
	if (bDoPause)
	{
		Sleep(80);
	}
	inp[0].type = INPUT_KEYBOARD;
	inp[0].ki.wVk = wCode;
	inp[0].ki.wScan = MapVirtualKey(wCode, 0);
	inp[0].ki.dwFlags = KEYEVENTF_KEYUP;
	res = SendInput(1, inp, sizeof(INPUT));
}

void CSimDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if (!m_bVisible)
	{
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	}
	else
	{
		lpwndpos->hwndInsertAfter = HWND_TOPMOST;
		lpwndpos->flags |= SWP_SHOWWINDOW;
	}

	CDialog::OnWindowPosChanging(lpwndpos);
}


LRESULT CSimDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	if (wParam == HK_PASTE_CREDENTIALS)
	{
		PasteCredentials(FALSE);
	}
	else if (wParam == HK_PASTEPWD)
	{
		PasteCredentials(TRUE);
	}
	else if (wParam == HK_RECORD_CREDENTIALS)
	{
		RecordCredentials();
	}
	else if (wParam == HK_CHOOSE_KEY)
	{
		FindKeyAndPaste();
	}
	else if (wParam == HK_COPYPASTER)
	{
		OnCopyPaster();
	}
	else if (wParam == HK_SHOWKEYS)
	{
		CWnd *wnd = GetForegroundWindow();
		CMyKeysDlg dlg(wnd);
		dlg.DoModal();
	}
	else if (wParam == HK_PASSWORD2CLIP)
	{
		Password2Clip();
	}

	return 0;
}

void CSimDlg::RecordCredentials()
{
	CWnd *wnd = GetForegroundWindow();

	wnd->GetWindowText(m_sAppTitle);
	m_sKeyName = m_sAppTitle;
	m_sUserName.Empty();
	m_sPassword.Empty();
	UpdateData(FALSE);

	m_bVisible = TRUE;
	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	SetForegroundWindow();
	CEdit *keyName = reinterpret_cast<CEdit*>(GetDlgItem(IDC_KEYNAME));
	keyName->SetFocus();
	keyName->SetSel(MAKELONG (0, -1), FALSE);
}

void CSimDlg::OnOK()
{
	UpdateData(TRUE);
	
	m_bVisible = FALSE;
	ShowWindow(SW_HIDE);
	IStreamPtr spPassStrm;

	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &spPassStrm);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to create stream for password, err[0x%X]"), hr);
		MessageBox(s, _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	hr = EncryptPassword(m_sPassword, spPassStrm);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to encrypt password, err[0x%X]"), hr);
		MessageBox(s, _T("Error"), MB_OK | MB_ICONERROR);
		return;
	}

	USES_CONVERSION;
	JET_ERR e = 0;
	// let's save the entry into database
	CJetTable keysTable;
	CALL_JET(g_DB.GetTable("tb_keys", keysTable));
	CALL_JET(keysTable.BeginTransaction());

	CALL_JET(keysTable.InsertRow()
		.SetColumn("tb_keys_app", T2A(m_sAppTitle))
		.SetColumn("tb_keys_keyname", T2A(m_sKeyName))
		.SetColumn("tb_keys_user", T2A(m_sUserName))
		.SetColumn("tb_keys_password", spPassStrm)
		.Done());

	CALL_JET(keysTable.CommitTransaction());

EXIT:
	;
}

void CSimDlg::OnCancel()
{
	m_bVisible = FALSE;
	ShowWindow(SW_HIDE);
}

void CSimDlg::OnExitClick()
{
	NOTIFYICONDATA nimData = { 0 };
	nimData.cbSize = sizeof(nimData);
	nimData.hWnd = GetSafeHwnd();
	Shell_NotifyIcon(NIM_DELETE, &nimData);

	CDialog::OnCancel();
}

afx_msg LRESULT CSimDlg::OnCallback(WPARAM, LPARAM lParam)
{
	CMenu menu;
	CMenu *pSubMenu = 0;
	CPoint pos;
	if (lParam == WM_RBUTTONDOWN || lParam == WM_LBUTTONDOWN)
	{
		GetCursorPos(&pos);
		menu.LoadMenu(IDR_MAINFRAME);
		pSubMenu = menu.GetSubMenu(0);
		SetForegroundWindow();
		pSubMenu->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			pos.x,
			pos.y,
			this);
	}

	return 0;
}

void CSimDlg::OnExit()
{
	OnExitClick();
}

void CSimDlg::OnMyKeys()
{
	CMyKeysDlg dlg;
	dlg.DoModal();
}

void CSimDlg::OnCopyPaster()
{
	CMyCopyPaste dlg;
	int iRes = dlg.DoModal();
	if (iRes == IDOK)
	{
		CString s = dlg.GetPasteableText();
		if (s.GetLength() > 0)
		{
			// TODO: consider using Clipboard for better reliability
			SendString(s, FALSE);
		}
	}
}

void CSimDlg::OnHelp()
{
	HRSRC hRsrc = FindResource(0, MAKEINTRESOURCE(IDR_HTML_HELP), RT_HTML);
	HGLOBAL hGlb = LoadResource(0, hRsrc);
	DWORD dwSize = SizeofResource(0, hRsrc);
	BYTE *byData = static_cast<BYTE*>(LockResource(hGlb));
	IStreamPtr spOutput;
	TCHAR szFileName[512] = { 0 };
	GetModuleFileName(0, szFileName, _countof(szFileName));
	*(_tcsrchr(szFileName, '\\') + 1) = 0;
	StringCbCat(szFileName, sizeof(szFileName), _T("help.html"));
	HRESULT hr = SHCreateStreamOnFile(szFileName, STGM_WRITE | STGM_CREATE, &spOutput);
	if (SUCCEEDED(hr))
	{
		spOutput->Write(byData, dwSize, 0);
		spOutput.Release();
	}
/**/
	ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),
		_T("open"),
		szFileName,
		0,
		0,
		SW_SHOW
	);
/**/
}


void CSimDlg::OnEndSession(BOOL bEnding)
{
	if (bEnding)
	{
		// user logs of or the system is shutting down -> shut down the database cleanly
		g_DB.CloseDatabase();
	}
	CDialog::OnEndSession(bEnding);
}

void CSimDlg::Password2Clip()
{
	CWnd *wnd = GetForegroundWindow();
	CMyKeysDlg dlg(wnd);
	if (dlg.DoModal() == IDOK)
	{
		// TODO: error handling
		USES_CONVERSION;
		CString sPwd = dlg.GetSelectedPassword();
		OpenClipboard();
		EmptyClipboard();
		size_t len = (sPwd.GetLength() + 1) * sizeof(TCHAR);
		HGLOBAL hGlb = GlobalAlloc(GMEM_MOVEABLE, len);
		StringCbCopy(reinterpret_cast<LPWSTR>(GlobalLock(hGlb)), len, sPwd);
		GlobalUnlock(hGlb);
		SetClipboardData(CF_UNICODETEXT, hGlb);
		CloseClipboard();
	}
}