#pragma once
#include "afxcmn.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_lstKeys;
};