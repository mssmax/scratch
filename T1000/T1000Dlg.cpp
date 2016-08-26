
// T1000Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "T1000.h"
#include "T1000Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CT1000Dlg::CT1000Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_T1000_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_uiMainTimer = 0;
	m_uiMsgTimeOutTimer = 0;
	m_uiUpdateTimer = 0;
	m_dwCountdown = 0;
	m_bVisible = FALSE;
	m_bConsent = FALSE;
	m_bFirstShot = TRUE;
}

void CT1000Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TARGETLIST, m_lstTargets);
}

BEGIN_MESSAGE_MAP(CT1000Dlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_KEEPALIVE, &CT1000Dlg::OnBnClickedKeepalive)
	ON_WM_WINDOWPOSCHANGING()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDCANCEL, &CT1000Dlg::OnIdCancel)
END_MESSAGE_MAP()


// CT1000Dlg message handlers

BOOL CT1000Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CheckTargets();

	SYSTEMTIME t = { 0 };
	GetLocalTime(&t);
	int nInitialInterval = DEFAULT_MAIN_TIMER;
	// NOTE: A bit of an ugly solution to make sure our timer is aligned on half hour boundary
#ifndef _DEBUG
	nInitialInterval = DEFAULT_MAIN_TIMER - ((t.wMinute * 1000 * 60) % DEFAULT_MAIN_TIMER);
#endif
	m_uiMainTimer = SetTimer(1, abs(nInitialInterval), 0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CT1000Dlg::OnPaint()
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

BOOL CT1000Dlg::GetDuskAndDawn(DWORD *pdwDawn, DWORD *pdwDusk)
{
	HKEY hKey = 0;
	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("SOFTWARE\\T1000"),
		0,
		KEY_QUERY_VALUE,
		&hKey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR szDusk[_MAX_PATH] = { 0 };
		TCHAR szDawn[_MAX_PATH] = { 0 };
		DWORD dwDusk = sizeof(szDusk);
		DWORD dwDawn = sizeof(szDawn);
		RegQueryValueEx(hKey, _T("Dusk"), 0, 0, reinterpret_cast<LPBYTE>(szDusk), &dwDusk);
		RegQueryValueEx(hKey, _T("Dawn"), 0, 0, reinterpret_cast<LPBYTE>(szDawn), &dwDawn);
		RegCloseKey(hKey);
		DWORD dwHH = 0;
		DWORD dwMM = 0;
		LPCTSTR lpszSeparator = _tcschr(szDusk, ':') + 1;
		dwHH = _tcstol(szDusk, NULL, 10);
		dwMM = _tcstol(lpszSeparator, NULL, 10);
		*pdwDusk = (dwHH * 60) + dwMM;
		lpszSeparator = _tcschr(szDawn, ':') + 1;
		dwHH = _tcstol(szDawn, NULL, 10);
		dwMM = _tcstol(lpszSeparator, NULL, 10);
		*pdwDawn = (dwHH * 60) + dwMM;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void CT1000Dlg::GetTargets(std::vector<std::wstring> &targets)
{
	HKEY hKey = 0;
	LONG lRes = RegOpenKeyEx(HKEY_CURRENT_USER,
		_T("SOFTWARE\\T1000\\Targets"),
		0,
		KEY_QUERY_VALUE,
		&hKey);
	if (lRes == ERROR_SUCCESS)
	{
		TCHAR szTarget[_MAX_PATH] = { 0 };
		TCHAR szDummy[_MAX_PATH] = { 0 };
		DWORD dwIndex = 0;
		DWORD dwSize = sizeof(szTarget);
		DWORD dwDummySize = _countof(szDummy);
		DWORD dwType = 0;
		while (RegEnumValue(hKey, dwIndex, szDummy, &dwDummySize, 0, &dwType, reinterpret_cast<LPBYTE>(szTarget), &dwSize) == ERROR_SUCCESS)
		{
			if (dwType == REG_EXPAND_SZ)
			{
				TCHAR szTemp[_MAX_PATH] = { 0 };
				ExpandEnvironmentStrings(szTarget, szTemp, _countof(szTemp));
				StringCbCopy(szTarget, sizeof(szTarget), szTemp);
			}
			targets.push_back(szTarget);

			dwSize = sizeof(szTarget);
			dwDummySize = _countof(szDummy);
			dwType = 0;
			++dwIndex;
		}
		RegCloseKey(hKey);
	}
}

void CT1000Dlg::CheckTargets()
{
	DWORD dwDawn = 0;
	DWORD dwDusk = 0;
	if (GetDuskAndDawn(&dwDawn, &dwDusk))
	{
		if (dwDawn < dwDusk)
		{
			std::vector<std::wstring> vTargets;
			GetTargets(vTargets);
			std::vector<std::wstring>::iterator it = vTargets.begin();
			m_lstTargets.ResetContent();
			for (UINT idx = 0; it != vTargets.end(); ++it, ++idx)
			{
				m_lstTargets.AddString(it->c_str());
			}
			ExecuteTargets(dwDawn, dwDusk, vTargets);
		}
	}
}

void CT1000Dlg::ExecuteTargets(DWORD dwDawn, DWORD dwDusk, std::vector<std::wstring> &vTargets)
{
	SYSTEMTIME curTime = { 0 };
	GetLocalTime(&curTime);
	DWORD dwCurPeriod = (curTime.wHour * 60) + curTime.wMinute;
	if ((dwCurPeriod > dwDawn) && (dwCurPeriod < dwDusk))
	{
		// if we're entering the working period again, we better off hide the window and shut down the aux timers
		KillTimers();
		OnClose();
		ResurrectTargets(vTargets);
	}
	else
	{
		if(!m_bConsent)
			DisplayWarning(vTargets);

		if (m_bConsent)
		{
			TerminateTargets(vTargets);
			OnClose();
		}
	}
}

void CT1000Dlg::TerminateTargets(std::vector<std::wstring> &vTargets)
{
	std::vector<std::wstring>::iterator it = vTargets.begin();
	for (; it != vTargets.end(); ++it)
	{
		DWORD dwProcessID = 0;
		if (IsTargetAlive(it->c_str(), &dwProcessID))
		{
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
			if (hProcess)
			{
				TerminateProcess(hProcess, 0);
				CloseHandle(hProcess);
			}
		}
	}
}

void CT1000Dlg::ResurrectTargets(std::vector<std::wstring> &vTargets)
{
	std::vector<std::wstring>::iterator it = vTargets.begin();
	for (; it != vTargets.end(); ++it)
	{
		if (!IsTargetAlive(it->c_str(), 0))
		{
			STARTUPINFO startInfo = { 0 };
			PROCESS_INFORMATION procInfo = { 0 };
			if (CreateProcess(0, &(*it)[0], 0, 0, FALSE, 0, 0, 0, &startInfo, &procInfo))
			{
				CloseHandle(procInfo.hProcess);
				CloseHandle(procInfo.hThread);
			}
		}
	}
}

BOOL CT1000Dlg::IsTargetAlive(LPCTSTR lpszName, DWORD *pdwProcID)
{
	std::wstring sProcessName(lpszName);
	size_t idx = sProcessName.rfind(_T('\\'), sProcessName.length());

	if (idx == std::wstring::npos)
	{
		idx = 0;
	}
	else
	{
		++idx;
	}
	HANDLE hProcesses = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcesses != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(pe);
		if (Process32First(hProcesses, &pe))
		{
			do
			{
				if (_tcsnicmp(pe.szExeFile, sProcessName.c_str() + idx, lstrlen(pe.szExeFile)) == 0)
				{
					if (pdwProcID)
						*pdwProcID = pe.th32ProcessID;
					return TRUE;
				}
				pe.dwSize = sizeof(pe);
			} while (Process32Next(hProcesses, &pe));
		}

		CloseHandle(hProcesses);
	}
	return FALSE;
}

void CT1000Dlg::DisplayWarning(std::vector<std::wstring> &vTargets)
{
	std::vector<std::wstring>::iterator it = vTargets.begin();
	BOOL bShowWarning = FALSE;
	for (; it != vTargets.end(); ++it)
	{
		if (IsTargetAlive(it->c_str(), 0))
		{
			bShowWarning = TRUE;
			break;
		}
	}

	if (!bShowWarning)
		return;

	m_dwCountdown = GetTickCount() + DEFAULT_MSGBOX_TIMEOUT_TIMER;
	// this timer governs when 
	if(!m_uiMsgTimeOutTimer)
		m_uiMsgTimeOutTimer = SetTimer(2, DEFAULT_MSGBOX_TIMEOUT_TIMER, 0);
	// this timer is used to update the UI with the countdown
	if(!m_uiUpdateTimer)
		m_uiUpdateTimer = SetTimer(3, 1000, 0);
	m_bVisible = TRUE;
	m_bConsent = FALSE;
	ShowWindow(SW_SHOWNORMAL);
	SetForegroundWindow();
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CT1000Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CT1000Dlg::OnTimer(UINT uiTimerID)
{
	// if the window is visible it means we're waiting to user's consent or timeout to expire
	if (uiTimerID == m_uiMainTimer)
	{
		if (m_bFirstShot)
		{
			KillTimer(m_uiMainTimer);
			m_uiMainTimer = SetTimer(1, DEFAULT_MAIN_TIMER, 0);
			m_bFirstShot = FALSE;
		}
		CheckTargets();
	}
	else if (uiTimerID == m_uiMsgTimeOutTimer)
	{
		KillTimers();
		
		m_bConsent = TRUE;
		CheckTargets();
		OnClose();
	}
	else if (uiTimerID == m_uiUpdateTimer)
	{
		TCHAR szTime[128] = { 0 };
		DWORD dwTTL = m_dwCountdown - GetTickCount();
		StrFromTimeInterval(szTime, _countof(szTime), dwTTL, 4);
		CString str;
		// there is no space between "in" and "%s" because StrFromTimeInterval function always puts a leading space
		str.Format(_T("The following targets will be terminated in%s"), szTime);
		SetDlgItemText(IDC_FOREWARNING, str);
	}
	CDialog::OnTimer(uiTimerID);
}


void CT1000Dlg::OnBnClickedKeepalive()
{
	OnClose();
}


void CT1000Dlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	if(!m_bVisible)
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
	else
		lpwndpos->flags |= SWP_SHOWWINDOW;

	CDialog::OnWindowPosChanging(lpwndpos);
}

void CT1000Dlg::KillTimers()
{
	KillTimer(m_uiUpdateTimer);
	m_uiUpdateTimer = 0;

	KillTimer(m_uiMsgTimeOutTimer);
	m_uiMsgTimeOutTimer = 0;

	m_dwCountdown = 0;
	m_bConsent = FALSE;
}

void CT1000Dlg::OnClose()
{
	KillTimers();
	m_bConsent = FALSE;
	m_bVisible = FALSE;
	ShowWindow(SW_HIDE);
}


HBRUSH CT1000Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (pWnd->GetDlgCtrlID() == IDC_FOREWARNING)
	{
		pDC->SetTextColor(RGB(255, 0, 0));
	}
	
	return hbr;
}


void CT1000Dlg::OnIdCancel()
{
	OnClose();
}
