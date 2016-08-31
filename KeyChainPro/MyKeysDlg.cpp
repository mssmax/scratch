#include "stdafx.h"
#include "resource.h"
#include "MyKeysDlg.h"

IMPLEMENT_DYNAMIC(CMyKeysDlg, CDialog)

CMyKeysDlg::CMyKeysDlg(CWnd* pParent /*=NULL*/, JET_TABLEID tblID /*=NULL*/)
	: CDialog(IDD_MYKEYS, pParent)
{
	m_tblID = tblID;
	if (m_tblID != 0)
	{
		m_bReadOnly = TRUE;
	}
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
	m_lstKeys.DeleteAllItems();

	USES_CONVERSION;
	CJetTable tbl;
	JET_ERR e = 0;
	if (m_tblID != 0)
	{
		CALL_JET(g_DB.GetTable(m_tblID, tbl));
	}
	else
	{
		CALL_JET(g_DB.GetTable("tb_keys", tbl));
	}
	e = tbl.BeginTransaction();
	CALL_JET(e);
	for (int iItem = 0; e >= 0; iItem++)
	{
		char szStr[1024] = { 0 };
		for (int i = 0; i < _countof(s_Columns); i++)
		{
			CALL_JET(tbl.GetColumn(s_Columns[i], szStr, sizeof(szStr)));
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

	CALL_JET(tbl.CommitTransaction());

EXIT:
	if(m_bReadOnly)
	{
		tbl.Detach();
	}
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

	m_lstKeys.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;  
}


void CMyKeysDlg::OnLvnKeydownLstKeys(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (m_bReadOnly)
	{
		goto EXIT;
	}

	if (pLVKeyDown->wVKey == VK_DELETE)
	{
		int nSel = m_lstKeys.GetNextItem(-1, LVNI_SELECTED);
		if (nSel != -1)
		{
			int nRes = AfxMessageBox(_T("Are you sure you want to delete this record ?"), MB_ICONQUESTION | MB_YESNO);
			if (nRes == IDYES)
			{
				CJetTable tbl;
				CALL_JET(g_DB.GetTable("tb_keys", tbl));
				CALL_JET(tbl.BeginTransaction());
				CALL_JET(tbl.Move(nSel));
				CALL_JET(tbl.DeleteRow());
				CALL_JET(tbl.CommitTransaction());

				ReloadData();
			}
		}
	}

EXIT:
	;

	*pResult = 0;
}

void CMyKeysDlg::OnNMDblclkLstKeys(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (m_bReadOnly)
	{
		goto EXIT;
	}

	ZeroMemory(&m_hitInfo, sizeof(m_hitInfo));
	GetCursorPos(&m_hitInfo.pt);
		
	m_lstKeys.ScreenToClient(&m_hitInfo.pt);
	if (m_lstKeys.SubItemHitTest(&m_hitInfo) != -1)
	{
		CRect itemRect;
		if(m_hitInfo.iSubItem > 0)
			m_lstKeys.GetSubItemRect(m_hitInfo.iItem, m_hitInfo.iSubItem, LVIR_BOUNDS, itemRect);
		else
			m_lstKeys.GetItemRect(m_hitInfo.iItem, &itemRect, LVIR_LABEL);

		m_ctrlEdit.SetWindowText(m_lstKeys.GetItemText(m_hitInfo.iItem, m_hitInfo.iSubItem));
		itemRect.InflateRect(3, 3);

		m_ctrlEdit.MoveWindow(&itemRect);
		m_ctrlEdit.ShowWindow(SW_SHOW);
		m_ctrlEdit.SetFocus();
		m_ctrlEdit.SetSel(MAKELONG(0, -1));
		m_ctrlEdit.ShowCaret();
	}

EXIT:
	;

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
		CString sText;
		m_ctrlEdit.GetWindowText(sText);
		m_lstKeys.SetItemText(m_hitInfo.iItem, m_hitInfo.iSubItem, sText);

		CJetTable tbl;
		CALL_JET(g_DB.GetTable("tb_keys", tbl));
		CALL_JET(tbl.BeginTransaction());
		CALL_JET(tbl.Move(m_hitInfo.iItem));
		CALL_JET(tbl.UpdateRow()
			.SetColumn(s_Columns[m_hitInfo.iSubItem], ConvW2A(sText).c_str())
			.Done());
		CALL_JET(tbl.CommitTransaction());

		OnEditKillFocus();
	}
	else
	{
		int nSel = m_lstKeys.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
		if (nSel != -1)
		{
			m_sKeyName = m_lstKeys.GetItemText(nSel, 1);
		}
		CDialog::OnOK();
	}

EXIT:
	;
}

CString CMyKeysDlg::GetSelectedKeyName()
{
	return m_sKeyName;
}