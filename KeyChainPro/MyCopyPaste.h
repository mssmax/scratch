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
	std::vector<std::string> m_vecText;
	CWnd *m_pPrevForegroundWnd;

	DECLARE_MESSAGE_MAP()
public:
	CString GetPasteableText();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	CEdit m__editContent;
	CListCtrl m_lstCPs;
	CEdit m_editInplace;
};
