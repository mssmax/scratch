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
	ON_WM_HELPINFO()
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

BOOL CInputDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HRSRC hRsrc = FindResource(0, MAKEINTRESOURCE(IDR_HTML_HELP), RT_HTML);
	HGLOBAL hGlb = LoadResource(0, hRsrc);
	DWORD dwSize = SizeofResource(0, hRsrc);
	BYTE *byData = static_cast<BYTE*>(LockResource(hGlb));
	IStreamPtr spOutput;
	TCHAR szFileName[512] = { 0 };
	GetModuleFileName(0, szFileName, _countof(szFileName));
	*(_tcsrchr(szFileName, '\\') + 1) = 0;
	StringCbCat(szFileName, sizeof(szFileName), _T("help.html"));
	HRESULT hr = SHCreateStreamOnFile(szFileName, STGM_WRITE | STGM_CREATE, &spOutput);
	if (SUCCEEDED(hr))
	{
		spOutput->Write(byData, dwSize, 0);
		spOutput.Release();
	}

	ShellExecute(
		AfxGetMainWnd()->GetSafeHwnd(),
		_T("open"),
		szFileName,
		0,
		0,
		SW_SHOW
	);

	return TRUE;
}
