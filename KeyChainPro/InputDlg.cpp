#include "stdafx.h"
#include "resource.h"
#include "InputDlg.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CInputDlg, CDialog)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/, LPCTSTR lpszTitle, BOOL bPasword)
	: CDialog(IDD_INPUTDLG, pParent)
	, m_sInput(_T(""))
{
	m_sTitle = lpszTitle;
	m_bPassword = bPasword;
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUTTEXT, m_sInput);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialog)
END_MESSAGE_MAP()


BOOL CInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_sTitle);
	if (m_bPassword)
	{
		GetDlgItem(IDC_INPUTTEXT)->SendMessage(EM_SETPASSWORDCHAR, '*', 0);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
