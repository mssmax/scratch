// uimgrDlg.cpp : implementation file
//

#include "stdafx.h"
#include "uimgr.h"
#include "uimgrDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define WM_CALLBACK (WM_APP + 1)


WORD TranslateModifiers(WORD wModifier)
{
	WORD wOutput = 0;
	if(wModifier & HOTKEYF_ALT)
		wOutput |= MOD_ALT;
	if(wModifier & HOTKEYF_CONTROL)
		wOutput |= MOD_CONTROL;
	if(wModifier & HOTKEYF_SHIFT)
		wOutput |= MOD_SHIFT;
	if(wModifier & HOTKEYF_EXT)
		wOutput |= MOD_WIN;

	return wOutput;
}

WORD TranslateModifiers2(WORD wModifier)
{
	WORD wOutput = 0;
	if(wModifier & MOD_ALT)
		wOutput |= HOTKEYF_ALT;
	if(wModifier & MOD_CONTROL)
		wOutput |= HOTKEYF_CONTROL;
	if(wModifier & MOD_SHIFT)
		wOutput |= HOTKEYF_SHIFT;
	if(wModifier & MOD_WIN)
		wOutput |= HOTKEYF_EXT;

	return wOutput;
}

/////////////////////////////////////////////////////////////////////////////
// CUIMgrDlg dialog

CUIMgrDlg::CUIMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUIMgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUIMgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bVisible = FALSE;
	m_wHKModifiers = 0;
	m_wHKVirtualCode = 0;
}

void CUIMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUIMgrDlg)
	DDX_Control(pDX, IDC_HOTKEY1, m_HotKey);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUIMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CUIMgrDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CALLBACK, OnCallback)
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_CMD_SHOW, OnCmdShow)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_BN_CLICKED(IDC_CLEARALL, OnClearall)
	ON_BN_CLICKED(IDC_SHOWALL, OnShowall)
	ON_BN_CLICKED(IDC_HIDEALL, OnHideall)
	ON_COMMAND(IDC_HIDEALLMNU, OnHideall)
	ON_COMMAND(IDC_SHOWALLMNU, OnShowall)
	ON_WM_QUERYENDSESSION()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIMgrDlg message handlers

BOOL CUIMgrDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetWindowText(SZ_APP_TITLE);
	
	// TODO: Add extra initialization here
	NOTIFYICONDATA nim = {0};
	nim.cbSize = sizeof(nim);
	nim.hWnd = GetSafeHwnd();
	nim.hIcon = m_hIcon;
	nim.uCallbackMessage = WM_CALLBACK;
	nim.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	lstrcpyn(nim.szTip, SZ_APP_TITLE, 64);
	Shell_NotifyIcon(NIM_ADD, &nim);
	if(m_wHKModifiers != 0 && m_wHKVirtualCode != 0)
	{
		RegisterHotKey(GetSafeHwnd(), ID_MAINHOTKEY, m_wHKModifiers, m_wHKVirtualCode);
		WORD wModifier = TranslateModifiers2(m_wHKModifiers);
		m_HotKey.SetHotKey(m_wHKVirtualCode, wModifier);
	}

	// register Ctrl+Alt+Shift+Z as emergency hotkey which will bring up the dialog
	// if the tray icon is inaccessible for some reason
	RegisterHotKey(GetSafeHwnd(), ID_EMERGENCYHOTKEY, MOD_ALT | MOD_SHIFT | MOD_CONTROL, 0x5A);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CUIMgrDlg::OnCallback(WPARAM wParam, LPARAM lParam)
{
	CMenu menu;
	CMenu* pSubMenu = 0;
	CPoint pos;
	CMenu *pSubMenuWin = 0;
	switch(lParam)
	{
		case WM_LBUTTONDBLCLK:
			m_bVisible = TRUE;
			ShowWindow(SW_SHOW);
			SetForegroundWindow();
			break;
		case WM_RBUTTONDOWN:
		case WM_LBUTTONDOWN:
			menu.LoadMenu(IDR_MAINMENU);
			GetCursorPos(&pos);
			SetForegroundWindow();
			pSubMenu = menu.GetSubMenu(0);
			pSubMenuWin = pSubMenu->GetSubMenu(3);
			FillMenu(pSubMenuWin);
			pSubMenu->SetDefaultItem(0, TRUE);
			if(lParam == WM_LBUTTONDOWN)
				pSubMenu->TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pos.x, pos.y, this);
			else
				pSubMenuWin->TrackPopupMenu(TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pos.x, pos.y, this);
			break;
	}
	return 0;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CUIMgrDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CUIMgrDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


BOOL CUIMgrDlg::DestroyWindow() 
{
	NOTIFYICONDATA nim = {0};
	nim.hWnd = GetSafeHwnd();
	Shell_NotifyIcon(NIM_DELETE, &nim);

	UnregisterHotKey(GetSafeHwnd(), ID_MAINHOTKEY);

	// release all captured windows
	for(int i = 0; i < m_hWindows.size(); i++)
	{
		ShowWindows(m_hWindows[i], TRUE);
	}

	return CDialog::DestroyWindow();
}

void CUIMgrDlg::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
{
	if(!m_bVisible)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	CDialog::OnWindowPosChanging(lpwndpos);	
}



void CUIMgrDlg::OnCancel() 
{
	m_bVisible = FALSE;	
	ShowWindow(SW_HIDE);

	WORD wVirtCode = 0;
	WORD wModifier = 0;
	m_HotKey.GetHotKey(wVirtCode, wModifier);

	wModifier = TranslateModifiers(wModifier);

	if(wVirtCode != m_wHKVirtualCode || wModifier != m_wHKModifiers)
	{
		m_wHKVirtualCode = wVirtCode;
		m_wHKModifiers = wModifier;
		RegisterHotKey(GetSafeHwnd(), ID_MAINHOTKEY, m_wHKModifiers, m_wHKVirtualCode);
	}

//	CDialog::OnCancel();
}

void CUIMgrDlg::OnOK() 
{
	CDialog::OnOK();
}

void CUIMgrDlg::OnAppExit() 
{
	OnOK();	
}

void CUIMgrDlg::OnCmdShow() 
{
	OnCallback(0, WM_LBUTTONDBLCLK);
}

HWND g_hForegroundWnd;

BOOL CALLBACK MyEnumer(HWND hwnd, LPARAM lParam)
{
	TCHAR szClassName[MAX_PATH];

	GetClassName(hwnd, szClassName, MAX_PATH);

	if(!strnicmp(szClassName, "MS-SDI", 6)) // bloody Excel and Access
	{
		HWND anotherHwnd = (HWND)GetWindowLong(hwnd, 0);
		if(anotherHwnd == g_hForegroundWnd)
		{
			ShowWindow(hwnd, (lParam) ? SW_SHOW : SW_HIDE);
		}
	}

	return TRUE;
}

HWND ShowSpecialCases(HWND hwnd, BOOL bShow)
{
	g_hForegroundWnd = hwnd;

	EnumWindows(MyEnumer, bShow);
//	if(hwndToHide)
//		ShowWindow(hwndToHide, (bShow) ? SW_SHOW : SW_HIDE);

	return 0;
}

LRESULT CUIMgrDlg::OnHotKey(WPARAM wHotId, LPARAM lModifier)
{
	if(wHotId == ID_MAINHOTKEY)
	{
		CPoint pos;
		GetCursorPos(&pos);
		CWnd *pActiveWnd = GetForegroundWindow();
		if(pActiveWnd)
		{
//			CString sText;
//			pActiveWnd->GetWindowText(sText);
			HWND hwnd = pActiveWnd->GetSafeHwnd();

			HWNDVECTOR::iterator it = find(m_hWindows.begin(), m_hWindows.end(), hwnd);
			if(it == m_hWindows.end())
				m_hWindows.push_back(hwnd);

			ShowSpecialCases(hwnd, FALSE);
			ShowWindows(hwnd, FALSE);
		}
	}
	else if(wHotId == ID_EMERGENCYHOTKEY)
	{
		// show ourselves
		OnCallback(0, WM_LBUTTONDBLCLK);
	}

	return 0;
}

void CUIMgrDlg::FillMenu(CMenu *pMenu)
{
	for(int i = 0; i < m_hWindows.size(); i++)
	{
		if(::IsWindow(m_hWindows[i]))
		{
			CString sBuffer;
			int nTextLength = 0;
			LPTSTR pszBuf = sBuffer.GetBuffer(nTextLength = (::GetWindowTextLength(m_hWindows[i]) + 1));
			::GetWindowText(m_hWindows[i], pszBuf, nTextLength);
			UINT nFlags = MF_STRING | MF_ENABLED | MF_BYPOSITION;

			if(::IsWindowVisible(m_hWindows[i]))
				nFlags |= MF_CHECKED;
			pMenu->InsertMenu(i, nFlags, (UINT_PTR)m_hWindows[i], pszBuf);

			sBuffer.ReleaseBuffer();
		}
		else
		{
			m_hWindows.erase(m_hWindows.begin() + i);
			i--;
		}
	}
}

BOOL CUIMgrDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	for(int i = 0; i < m_hWindows.size(); i++)
	{
		if(m_hWindows[i] == (HWND)wParam)
		{
			ShowSpecialCases(m_hWindows[i], TRUE);
			ShowWindows(m_hWindows[i], TRUE);

			return TRUE;
		}
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CUIMgrDlg::OnClearall() 
{
	// release all captured windows
	OnShowall();

	m_hWindows.clear();
}

void CUIMgrDlg::OnShowall() 
{
	// release all captured windows
	for(int i = 0; i < m_hWindows.size(); i++)
	{
		ShowWindows(m_hWindows[i], TRUE);
	}
}

void CUIMgrDlg::OnHideall() 
{
	// release all captured windows
	for(int i = 0; i < m_hWindows.size(); i++)
	{
		ShowWindows(m_hWindows[i], FALSE);
	}
}

void CUIMgrDlg::ShowWindows(HWND hwnd, BOOL bShow)
{
	::ShowWindow(hwnd, (bShow) ? SW_SHOW : SW_HIDE);
	hwnd = ::GetWindow(hwnd, GW_OWNER);
	if(hwnd)
		::ShowWindow(hwnd, (bShow) ? SW_SHOW : SW_HIDE);
}

BOOL CUIMgrDlg::OnQueryEndSession() 
{
	if (!CDialog::OnQueryEndSession())
		return FALSE;
	
	if(m_hWindows.size() > 0)
	{
		// ask the user whether he wants to end the session
		int i = MessageBox("You have some windows hidden.\r\nDo you really want to end session ?", "Window Tray Bin", MB_YESNO);
		if(i != IDYES)
			return FALSE;
	}
	
	return TRUE;
}
