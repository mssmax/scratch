#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CMyKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyKeysDlg)

public:
	CMyKeysDlg(CWnd* pParent = NULL, JET_TABLEID tblID = NULL);   // standard constructor
	virtual ~CMyKeysDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYKEYS };
#endif

protected:

	JET_TABLEID m_tblID;
	CString m_sKeyName;
	CString m_sPassword;
	LVHITTESTINFO m_hitInfo;
	BOOL m_bReadOnly;
	CEdit m_ctrlEdit;
	CListCtrl m_lstKeys;
	std::vector<std::wstring> m_vecPasswords;
	BOOL m_bShowPlainPwds;

	void ReloadData();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	afx_msg void OnLvnKeydownLstKeys(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstKeys(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditKillFocus();
	virtual void OnCancel();
	virtual void OnOK();

	CString GetSelectedKeyName();
	CString GetSelectedPassword();
	afx_msg void OnHdnItemclickLstKeys(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedBtnbackup();
	afx_msg void OnClickedBtnrestore();
};