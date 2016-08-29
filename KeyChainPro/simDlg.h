#pragma once
#include "afxwin.h"

class CSimDlg : public CDialog
{
// Construction
public:
	CSimDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	BOOL m_bVisible;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	void PasteCredentials();
	void RecordCredentials();
	void FindKeyAndPaste();

	void SendString(LPCTSTR lpszString);
	void SendCode(WORD wCode);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnExitClick();
	afx_msg LRESULT OnCallback(WPARAM, LPARAM);

	CString m_sAppTitle;
	CString m_sPassword;
	CString m_sUserName;
	CString m_sKeyName;
	afx_msg void OnExit();
	afx_msg void OnMyKeys();
};
