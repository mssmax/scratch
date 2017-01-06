// uimgrDlg.h : header file
//

#if !defined(AFX_UIMGRDLG_H__31A70536_8045_4051_9B7D_B4F2C69AE151__INCLUDED_)
#define AFX_UIMGRDLG_H__31A70536_8045_4051_9B7D_B4F2C69AE151__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

using namespace std;

typedef vector<HWND> HWNDVECTOR;
#define SZ_APP_TITLE "Window tray bin"
/////////////////////////////////////////////////////////////////////////////
// CUIMgrDlg dialog

class CUIMgrDlg : public CDialog
{
// Construction
public:
	CUIMgrDlg(CWnd* pParent = NULL);	// standard constructor

	DWORD GetHotkey() { return MAKELPARAM(m_wHKModifiers, m_wHKVirtualCode); }
	void SetHotkey(DWORD dwHotKey) { m_wHKModifiers = LOWORD(dwHotKey); m_wHKVirtualCode = HIWORD(dwHotKey); }

// Dialog Data
	//{{AFX_DATA(CUIMgrDlg)
	enum { IDD = IDD_UIMGR_DIALOG };
	CHotKeyCtrl	m_HotKey;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIMgrDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ShowWindows(HWND hwnd, BOOL bShow);
	void FillMenu(CMenu *pMenu);
	HICON m_hIcon;
	BOOL m_bVisible;
	WORD m_wHKVirtualCode;
	WORD m_wHKModifiers;
	HWNDVECTOR m_hWindows;

	// Generated message map functions
	//{{AFX_MSG(CUIMgrDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnCallback(WPARAM, LPARAM);
	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnAppExit();
	afx_msg void OnCmdShow();
	afx_msg LRESULT OnHotKey(WPARAM wHotId, LPARAM lModifier);
	afx_msg void OnClearall();
	afx_msg void OnShowall();
	afx_msg void OnHideall();
	afx_msg BOOL OnQueryEndSession();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIMGRDLG_H__31A70536_8045_4051_9B7D_B4F2C69AE151__INCLUDED_)
