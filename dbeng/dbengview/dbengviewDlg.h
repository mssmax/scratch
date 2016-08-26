
// dbengviewDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

typedef struct
{
	std::string sName;
	long lType;
} COLUMN_INFO;

class CDBEngViewDlg : public CDialog
{
// Construction
public:
	CDBEngViewDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DBENGVIEW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	void EnumerateTables();
	void EnumerateColumnsAndData(CString &sTable);
	void PopulateData(CString &sTable, std::vector<COLUMN_INFO> &vecColumns);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstData;
	CComboBox m_cmbTables;
	afx_msg void OnClickedOpen();
	afx_msg void OnClickedClose();
	afx_msg void OnSelchangeComboTables();
};
