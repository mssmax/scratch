#pragma once

class CMyKeysDlg : public CDialog
{
	DECLARE_DYNAMIC(CMyKeysDlg)

public:
	CMyKeysDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyKeysDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYKEYSDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};