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
	DDX_Control(pDX, IDC_EDIT, m_ctrlEdit);
}

BEGIN_MESSAGE_MAP(CMyKeysDlg, CDialog)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LST_KEYS, &CMyKeysDlg::OnLvnKeydownLstKeys)
	ON_NOTIFY(NM_DBLCLK, IDC_LST_KEYS, &CMyKeysDlg::OnNMDblclkLstKeys)
	ON_EN_KILLFOCUS(IDC_EDIT, OnEditKillFocus)
END_MESSAGE_MAP()


void CMyKeysDlg::ReloadData()
{
	static char* s_Columns[] = {
		"tb_keys_app",
		"tb_keys_keyname",
		"tb_keys_user",
		"tb_keys_password"
	};

	m_lstKeys.DeleteAllItems();

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
			if (i == 0)
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
}

BOOL CMyKeysDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstKeys.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstKeys.InsertColumn(0, _T("Application"), 0, 180);
	m_lstKeys.InsertColumn(1, _T("Key name"), 0, 180);
	m_lstKeys.InsertColumn(2, _T("User"), 0, 180);
	m_lstKeys.InsertColumn(3, _T("Password"), 0, 180);

	m_ctrlEdit.SetParent(&m_lstKeys);

	ReloadData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CMyKeysDlg::OnLvnKeydownLstKeys(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (pLVKeyDown->wVKey == VK_DELETE)
	{
		int nSel = m_lstKeys.GetNextItem(-1, LVNI_SELECTED);
		if (nSel != -1)
		{
			CJetTable tbl;
			JET_ERR e = g_DB.GetTable("tb_keys", tbl);
			tbl.BeginTransaction();
			e = tbl.Move(nSel);
			e = tbl.DeleteRow();
			tbl.CommitTransaction();

			ReloadData();
		}
	}

	*pResult = 0;



}

void CMyKeysDlg::OnNMDblclkLstKeys(NMHDR *pNMHDR, LRESULT *pResult)
{
	LVHITTESTINFO hitInfo = { 0 };
	GetCursorPos(&hitInfo.pt);
		
	m_lstKeys.ScreenToClient(&hitInfo.pt);
	if (m_lstKeys.SubItemHitTest(&hitInfo) != -1)
	{
		CRect itemRect;
		if(hitInfo.iSubItem > 0)
			m_lstKeys.GetSubItemRect(hitInfo.iItem, hitInfo.iSubItem, LVIR_BOUNDS, itemRect);
		else
			m_lstKeys.GetItemRect(hitInfo.iItem, &itemRect, LVIR_LABEL);

		m_ctrlEdit.SetWindowText(m_lstKeys.GetItemText(hitInfo.iItem, hitInfo.iSubItem));
		itemRect.InflateRect(4, 4);

		m_ctrlEdit.MoveWindow(&itemRect);
		m_ctrlEdit.ShowWindow(SW_SHOW);
		m_ctrlEdit.SetFocus();
		m_ctrlEdit.SetSel(MAKELONG(0, -1));
		m_ctrlEdit.ShowCaret();
	}
	
	*pResult = 0;
}

void CMyKeysDlg::OnEditKillFocus()
{
	m_ctrlEdit.ShowWindow(SW_HIDE);
	m_lstKeys.SetFocus();
}

void CMyKeysDlg::OnCancel()
{
	if (m_ctrlEdit.IsWindowVisible())
	{
		OnEditKillFocus();
	}
	else
	{
		CDialog::OnCancel();
	}
}


void CMyKeysDlg::OnOK()
{
	if (m_ctrlEdit.IsWindowVisible())
	{
		OnEditKillFocus();
	}
	else
	{
		CDialog::OnOK();
	}
}
