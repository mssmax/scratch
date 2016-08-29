#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class CMyKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyKeysDlg)

public:
	CMyKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyKeysDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYKEYS };
#endif

protected:

	LVHITTESTINFO m_hitInfo;

	void ReloadData();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstKeys;
	afx_msg void OnLvnKeydownLstKeys(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkLstKeys(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditKillFocus();
	CEdit m_ctrlEdit;
	virtual void OnCancel();
	virtual void OnOK();
};