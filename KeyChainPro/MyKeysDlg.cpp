#include "stdafx.h"
#include "resource.h"
#include "MyKeysDlg.h"

IMPLEMENT_DYNAMIC(CMyKeysDlg, CDialog)

CMyKeysDlg::CMyKeysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYKEYS, pParent)
{

}

CMyKeysDlg::~CMyKeysDlg()
{
}

void CMyKeysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_KEYS, m_lstKeys);
}

BEGIN_MESSAGE_MAP(CMyKeysDlg, CDialog)
END_MESSAGE_MAP()

BOOL CMyKeysDlg::OnInitDialog()
{
	static char* s_Columns[] = {
		"tb_keys_app",
		"tb_keys_keyname",
		"tb_keys_user",
		"tb_keys_password"
	};

	CDialog::OnInitDialog();

	m_lstKeys.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstKeys.InsertColumn(0, _T("Application"), 0, 180);
	m_lstKeys.InsertColumn(1, _T("Key name"), 0, 180);
	m_lstKeys.InsertColumn(2, _T("User"), 0, 180);
	m_lstKeys.InsertColumn(3, _T("Password"), 0, 180);

	USES_CONVERSION;
	CJetTable tbl;
	JET_ERR e = g_DB.GetTable("tb_keys", tbl);
	e = tbl.BeginTransaction();
	for (int iItem = 0; e >= 0; iItem++)
	{
		char szStr[1024] = { 0 };
		for (int i = 0; i < _countof(s_Columns); i++)
		{
			e = tbl.GetColumn(s_Columns[i], szStr, sizeof(szStr));
			if(i == 0)
			{ 
				m_lstKeys.InsertItem(iItem, ConvA2W(szStr).c_str());
			}
			else
			{
				m_lstKeys.SetItemText(iItem, i, ConvA2W(szStr).c_str());
			}
		}
		e = tbl.NextRow();
	}

	e = tbl.CommitTransaction();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
