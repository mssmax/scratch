#pragma once
#include "afxwin.h"

#ifdef _DEBUG
#define DEFAULT_MAIN_TIMER 15000
#define DEFAULT_MSGBOX_TIMEOUT_TIMER 10000
#else
#define DEFAULT_MAIN_TIMER (30L * 60 * 1000)
#define DEFAULT_MSGBOX_TIMEOUT_TIMER (15L * 60 * 1000)
#endif

// CT1000Dlg dialog
class CT1000Dlg : public CDialog
{
// Construction
public:
	CT1000Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_T1000_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	UINT m_uiMainTimer;
	UINT m_uiMsgTimeOutTimer;
	UINT m_uiUpdateTimer;
	DWORD m_dwCountdown;
	BOOL m_bVisible;
	BOOL m_bConsent;
	BOOL m_bFirstShot;

	BOOL GetDuskAndDawn(DWORD *pdwDawn, DWORD *pdwDusk);
	void GetTargets(std::vector<std::wstring> &targets);
	void CheckTargets();
	void ExecuteTargets(DWORD dwDawn, DWORD dwDusk, std::vector<std::wstring> &vTargets);

	void TerminateTargets(std::vector<std::wstring> &targets);
	void ResurrectTargets(std::vector<std::wstring> &targets);

	BOOL IsTargetAlive(LPCTSTR lpszName, DWORD *pdwProcID);

	void DisplayWarning(std::vector<std::wstring> &vTargets);
	void KillTimers();
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKeepalive();
	CListBox m_lstTargets;
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnIdCancel();
};
