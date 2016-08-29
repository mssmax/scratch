#include "stdafx.h"
#include "sim.h"
#include "simDlg.h"
#include "MyKeysDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_CALLBACK WM_APP + 1

#define HK_PASTE_CREDENTIALS 666
#define HK_RECORD_CREDENTIALS 667
#define HK_CHOOSE_KEY 668

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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimDlg::PasteCredentials()
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

	JET_ERR err = keysTable.Select("tb_keys_app", OP_EQ, T2A(sAppTitle));
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
	hr = CreateStreamOnHGlobal(0, TRUE, &spPassStrm);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to create stream for password, err[0x%X]"), hr);
		AfxMessageBox(s, MB_OK | MB_ICONERROR);
		return;
	}

	CALL_JET(keysTable.GetColumn("tb_keys_user", szUserName, sizeof(szUserName)));
	CALL_JET(keysTable.GetColumn("tb_keys_password", spPassStrm));

	CALL_JET(keysTable.CommitTransaction());

	hr = DecryptPassword(spPassStrm, szPassword);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to encrypt password, err[0x%X]"), hr);
		AfxMessageBox(s, MB_OK | MB_ICONERROR);
		return;
	}

	// Sleeps aren't the best way to control timing, but it appears
	// Windows is not capable of handling input fast enough ( probably due to thread switching ).
	SendString(A2T(szUserName));
	Sleep(50);
	SendCode(9);
	Sleep(50);
	SendString(szPassword);

EXIT:
	;
}

void CSimDlg::FindKeyAndPaste()
{

}

void CSimDlg::SendString(LPCTSTR lpszString)
{
	int iSize = lstrlen(lpszString);
	for (int i = 0; i < iSize; i++)
	{
		INPUT inp[2] = { 0 };
		inp[0].type = INPUT_KEYBOARD;
		inp[0].ki.wScan = lpszString[i];
		inp[0].ki.dwFlags = KEYEVENTF_UNICODE;

		inp[1].type = INPUT_KEYBOARD;
		inp[1].ki.wScan = lpszString[i];
		inp[1].ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;

		UINT res = SendInput(2, inp, sizeof(INPUT));
		Sleep(40);
	}
}

void CSimDlg::SendCode(WORD wCode)
{
	INPUT inp[2] = { 0 };
	ZeroMemory(inp, sizeof(inp));
	inp[0].type = INPUT_KEYBOARD;
	inp[0].ki.wVk = wCode;
	inp[0].ki.wScan = MapVirtualKey(wCode, 0);
	inp[1].type = INPUT_KEYBOARD;
	inp[1].ki.wVk = wCode;
	inp[1].ki.wScan = MapVirtualKey(wCode, 0);
	inp[1].ki.dwFlags = KEYEVENTF_KEYUP;
	UINT res = SendInput(_countof(inp), inp, sizeof(INPUT));
	Sleep(40);
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
		PasteCredentials();
	}
	else if (wParam == HK_RECORD_CREDENTIALS)
	{
		RecordCredentials();
	}
	else if (wParam == HK_CHOOSE_KEY)
	{
		FindKeyAndPaste();
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
		AfxMessageBox(s, MB_OK | MB_ICONERROR);
		return;
	}

	hr = EncryptPassword(m_sPassword, spPassStrm);
	if (FAILED(hr))
	{
		CString s;
		s.Format(_T("Failed to encrypt password, err[0x%X]"), hr);
		AfxMessageBox(s, MB_OK | MB_ICONERROR);
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

HRESULT CSimDlg::EncryptPassword(LPCTSTR lpszPassword, IStream *pStrm)
{
	HRESULT hr = S_OK;
	DATA_BLOB blbIn = { 0 };
	DATA_BLOB blbOut = { 0 };

	blbIn.cbData = lstrlen(lpszPassword) * sizeof(TCHAR);
	blbIn.pbData = (BYTE*)lpszPassword;

	BOOL b = CryptProtectData(
		&blbIn,
		0,
		0,
		0,
		0,
		0,
		&blbOut
	);

	if (!b)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	ULONG cbWri = 0;
	hr = pStrm->Write(blbOut.pbData, blbOut.cbData, &cbWri);
	if (FAILED(hr))
	{
		LocalFree(blbOut.pbData);
		return hr;
	}

	LocalFree(blbOut.pbData);

	LARGE_INTEGER liZero = { 0 };
	ULARGE_INTEGER liNewPos = { 0 };
	return pStrm->Seek(liZero, STREAM_SEEK_SET, &liNewPos);
}

HRESULT CSimDlg::DecryptPassword(IStream *pStrm, LPTSTR lpszPassword)
{
	HGLOBAL hGlb = 0;
	HRESULT hr = GetHGlobalFromStream(pStrm, &hGlb);
	if (FAILED(hr))
	{
		return hr;
	}
	DATA_BLOB blbIn = { 0 };
	DATA_BLOB blbOut = { 0 };
	STATSTG stat = { 0 };
	pStrm->Stat(&stat, STATFLAG_NONAME);
	blbIn.cbData = stat.cbSize.LowPart;
	blbIn.pbData = reinterpret_cast<BYTE*>(GlobalLock(hGlb));

	BOOL b = CryptUnprotectData(
		&blbIn,
		0,
		0,
		0,
		0,
		0,
		&blbOut
	);

	if (!b)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		GlobalUnlock(hGlb);
	}
	else
	{
		memcpy(lpszPassword, blbOut.pbData, blbOut.cbData);
		LocalFree(blbOut.pbData);
	}

	return hr;
}

void CSimDlg::OnMyKeys()
{
	CMyKeysDlg dlg;

	int iRes = dlg.DoModal();
}