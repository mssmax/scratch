#include "stdafx.h"
#include "resource.h"
#include "MyKeysDlg.h"


// CMyKeysDlg dialog

IMPLEMENT_DYNAMIC(CMyKeysDlg, CDialog)

CMyKeysDlg::CMyKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYKEYSDLG, pParent)
{

}

CMyKeysDlg::~CMyKeysDlg()
{
}

void CMyKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyKeysDlg, CDialog)
END_MESSAGE_MAP()
