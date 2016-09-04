#include "stdafx.h"
#include "resource.h"
#include "MyCopyPaste.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CMyCopyPaste, CDialog)

CMyCopyPaste::CMyCopyPaste(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYCOPYPASTE, pParent)
{

}

CMyCopyPaste::~CMyCopyPaste()
{
}

void CMyCopyPaste::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CP_CONTENT, m_editContent);
	DDX_Control(pDX, IDC_CP_LIST, m_lstCPs);
	DDX_Control(pDX, IDC_EDITINPLACE, m_editInplace);
}

BEGIN_MESSAGE_MAP(CMyCopyPaste, CDialog)
	ON_EN_KILLFOCUS(IDC_EDITINPLACE, OnEditKillFocus)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CP_LIST, &CMyCopyPaste::OnLvnItemchangedCpList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CP_LIST, &CMyCopyPaste::OnLvnKeydownCpList)
	ON_NOTIFY(NM_DBLCLK, IDC_CP_LIST, &CMyCopyPaste::OnNMDblclkCpList)
END_MESSAGE_MAP()

BOOL CMyCopyPaste::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstCPs.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstCPs.InsertColumn(0, _T("Application"), 0, 250);
	m_lstCPs.InsertColumn(1, _T("Key name"), 0, 250);
	m_editInplace.SetParent(&m_lstCPs);

	ReloadData();

	m_pPrevForegroundWnd = GetForegroundWindow();
	SetForegroundWindow();

	m_lstCPs.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CMyCopyPaste::OnEditKillFocus()
{
	m_editInplace.ShowWindow(SW_HIDE);
	m_lstCPs.SetFocus();
}


CString CMyCopyPaste::GetPasteableText()
{
	return m_sPasteable;
}

void CMyCopyPaste::OnOK()
{
	if (m_editInplace.IsWindowVisible())
	{
		CString s;

		m_editInplace.GetWindowText(s);
		m_lstCPs.SetItemText(
			m_hitInfo.iItem,
			m_hitInfo.iSubItem,
			s);
		CJetTable tbl;
		CALL_JET(g_DB.GetTable("tb_copypaste", tbl));
		CALL_JET(tbl.BeginTransaction());
		CALL_JET(tbl.UpdateRow()
			.SetColumn(s_CPColumns[m_hitInfo.iSubItem], s)
			.Done()
		);
		CALL_JET(tbl.CommitTransaction());

		OnEditKillFocus();
	}
	else
	{
		UpdateData(TRUE);
		int nSel = m_lstCPs.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
		if (nSel >= 0)
		{
			CString s;
			m_editContent.GetWindowText(s);
			m_sPasteable = s;
			if (lstrcmp(m_vecText[nSel].c_str(), s) != 0)
			{
				m_sPasteable = s;
				m_vecText[nSel] = s;

				CJetTable tbl;
				CALL_JET(g_DB.GetTable("tb_copypaste", tbl));
				CALL_JET(tbl.BeginTransaction());
				CALL_JET(tbl.Move(nSel));
				CALL_JET(tbl.UpdateRow()
					.SetColumn("tb_cp_text", m_vecText[nSel].c_str())
					.Done()
				);
				CALL_JET(tbl.CommitTransaction());
			}
		}
		else
		{
			m_sPasteable = _T("");
		}

		m_pPrevForegroundWnd->SetForegroundWindow();
		m_pPrevForegroundWnd = NULL;
		CDialog::OnOK();
	}

EXIT:
	;
}

void CMyCopyPaste::OnCancel()
{
	if (m_editInplace.IsWindowVisible())
	{
		OnEditKillFocus();
	}
	else
	{
		m_pPrevForegroundWnd->SetForegroundWindow();
		m_pPrevForegroundWnd = NULL;
		CDialog::OnCancel();
	}
}

void CMyCopyPaste::ReloadData()
{
	m_lstCPs.DeleteAllItems();
	m_vecText.clear();
	CJetTable tbl;
	CALL_JET(g_DB.GetTable("tb_copypaste", tbl));
	CALL_JET(tbl.BeginTransaction());

	JET_ERR e = tbl.MoveFirst();
	for (int iItem = 0; e >= 0; iItem++)
	{
		m_lstCPs.InsertItem(iItem, _T(""));
		for (int iSubItem = 0; iSubItem < _countof(s_CPColumns); iSubItem++)
		{
			WCHAR szStr[1024] = { 0 };
			CALL_JET(tbl.GetColumn(s_CPColumns[iSubItem], szStr, _countof(szStr)));
			if (iSubItem < _countof(s_CPColumns) - 1)
			{
				m_lstCPs.SetItemText(iItem, iSubItem, szStr);
			}
			else
			{
				m_vecText.push_back(szStr);
			}
		}
		e = tbl.NextRow();
	}

	CALL_JET(tbl.CommitTransaction());
EXIT:
	;
}

void CMyCopyPaste::OnLvnItemchangedCpList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem >= 0 && pNMLV->iItem < m_vecText.size())
	{
		m_editContent.SetWindowText(m_vecText[pNMLV->iItem].c_str());
	}

	*pResult = 0;
}


void CMyCopyPaste::OnLvnKeydownCpList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDown = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);

	if (pLVKeyDown->wVKey == VK_DELETE)
	{
		int nSel = m_lstCPs.GetNextItem(-1, LVNI_SELECTED);
		if (nSel != -1)
		{
			int nRes = AfxMessageBox(_T("Are you sure you want to delete this record ?"), MB_ICONQUESTION | MB_YESNO);
			if (nRes == IDYES)
			{
				CJetTable tbl;
				g_DB.GetTable("tb_copypaste", tbl);
				CALL_JET(tbl.BeginTransaction());
				CALL_JET(tbl.Move(nSel));
				CALL_JET(tbl.DeleteRow());
				CALL_JET(tbl.CommitTransaction());
				ReloadData();
			}
		}
	}
	else if (pLVKeyDown->wVKey == VK_INSERT)
	{
		int nItem = m_lstCPs.GetItemCount();
		CString s;
		m_pPrevForegroundWnd->GetWindowText(s);
		m_lstCPs.InsertItem(nItem, s);
		CJetTable tbl;
		g_DB.GetTable("tb_copypaste", tbl);
		CALL_JET(tbl.BeginTransaction());
		CALL_JET(tbl.InsertRow()
			.SetColumn("tb_cp_app", s)
			.Done());
		CALL_JET(tbl.CommitTransaction());
		ReloadData();
	}

EXIT:
	*pResult = 0;
}


void CMyCopyPaste::OnNMDblclkCpList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	ZeroMemory(&m_hitInfo, sizeof(m_hitInfo));
	GetCursorPos(&m_hitInfo.pt);

	m_lstCPs.ScreenToClient(&m_hitInfo.pt);
	if (m_lstCPs.SubItemHitTest(&m_hitInfo) != -1)
	{
		CRect itemRect;
		if (m_hitInfo.iSubItem > 0)
			m_lstCPs.GetSubItemRect(m_hitInfo.iItem, m_hitInfo.iSubItem, LVIR_BOUNDS, itemRect);
		else
			m_lstCPs.GetItemRect(m_hitInfo.iItem, &itemRect, LVIR_LABEL);

		m_editInplace.SetWindowText(m_lstCPs.GetItemText(m_hitInfo.iItem, m_hitInfo.iSubItem));
		itemRect.InflateRect(3, 3);

		m_editInplace.MoveWindow(&itemRect);
		m_editInplace.ShowWindow(SW_SHOW);
		m_editInplace.SetFocus();
		m_editInplace.SetSel(MAKELONG(0, -1));
		m_editInplace.ShowCaret();
	}

	*pResult = 0;
}
