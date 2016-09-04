#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CMyCopyPaste : public CDialog
{
	DECLARE_DYNAMIC(CMyCopyPaste)

public:
	CMyCopyPaste(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyCopyPaste();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYCOPYPASTE };
#endif

protected:

	void ReloadData();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::vector<std::wstring> m_vecText;
	CWnd *m_pPrevForegroundWnd;
	LVHITTESTINFO m_hitInfo;
	CString m_sPasteable;

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEditKillFocus();

	CString GetPasteableText();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CEdit m_editContent;
	CListCtrl m_lstCPs;
	CEdit m_editInplace;
	afx_msg void OnLvnItemchangedCpList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownCpList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkCpList(NMHDR *pNMHDR, LRESULT *pResult);
};
