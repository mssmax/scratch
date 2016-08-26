// prn_infoDlg.h : header file
//

#if !defined(AFX_PRN_INFODLG_H__5103EB85_D027_475A_A952_EE4B833C58ED__INCLUDED_)
#define AFX_PRN_INFODLG_H__5103EB85_D027_475A_A952_EE4B833C58ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPrnInfoDlg dialog

class CPrnInfoDlg : public CDialog
{
// Construction
public:
	CPrnInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPrnInfoDlg)
	enum { IDD = IDD_PRN_INFO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrnInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPrnInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRN_INFODLG_H__5103EB85_D027_475A_A952_EE4B833C58ED__INCLUDED_)
