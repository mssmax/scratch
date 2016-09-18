#pragma once

class CInputDlg : public CDialog
{
	DECLARE_DYNAMIC(CInputDlg)
private:

	CString m_sTitle;
	BOOL m_bPassword;

public:
	CInputDlg(CWnd* pParent = NULL, LPCTSTR lpszTitle = NULL, BOOL bPasword = FALSE);   // standard constructor
	virtual ~CInputDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INPUTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_sInput;
};
