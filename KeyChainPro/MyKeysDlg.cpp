#include "stdafx.h"
#include "resource.h"
#include "MyKeysDlg.h"

IMPLEMENT_DYNAMIC(CMyKeysDlg, CDialog)

CMyKeysDlg::CMyKeysDlg(CWnd* pParent /*=NULL*/, JET_TABLEID tblID /*=NULL*/)
	: CDialog(IDD_MYKEYS, pParent)
{
	m_bShowPlainPwds = FALSE;
	m_bReadOnly = FALSE;
	m_tblID = tblID;
	m_iSortOrder = 1;
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
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CMyKeysDlg::OnHdnItemclickLstKeys)
	ON_BN_CLICKED(IDC_BTNBACKUP, &CMyKeysDlg::OnClickedBtnbackup)
	ON_BN_CLICKED(IDC_BTNRESTORE, &CMyKeysDlg::OnClickedBtnrestore)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()


void CMyKeysDlg::ReloadData()
{
	m_lstKeys.DeleteAllItems();
	m_vecPasswords.clear();
	m_bShowPlainPwds = FALSE;
	LARGE_INTEGER liZero = { 0 };

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
		WCHAR szStr[1024] = { 0 };
		m_lstKeys.InsertItem(iItem, _T(""));
		for (int i = 0; i < _countof(s_KeysColumns); i++)
		{
			// TODO: a bit hacky since we assume the fourth column is password
			// we'll deal with it at some point
			if (i < 3)
			{
				e = tbl.GetColumn(s_KeysColumns[i], szStr, _countof(szStr));
				if (e != JET_errSuccess)
				{
					break;
				}
				m_lstKeys.SetItemText(iItem, i, szStr);
			}
			else
			{
				IStreamPtr spStrm;
				CreateStreamOnHGlobal(0, TRUE, &spStrm);
				CALL_JET(tbl.GetColumn(s_KeysColumns[i], spStrm));
				spStrm->Seek(liZero, STREAM_SEEK_SET, NULL);
				m_lstKeys.SetItemText(iItem, i, _T("********"));
				TCHAR szPassword[512] = { 0 };
				DecryptPassword(spStrm, szPassword);
				m_vecPasswords.push_back(szPassword);
				m_lstKeys.SetItemData(iItem, m_vecPasswords.size() - 1);
			}
		}
		e = tbl.NextRow();
	}

	CALL_JET(tbl.CommitTransaction());
	m_lstKeys.SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

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
	m_lstKeys.InsertColumn(3, _T("Password"), 0, 320);

	m_ctrlEdit.SetParent(&m_lstKeys);

	ReloadData();

	m_lstKeys.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	if(m_vecPasswords.size() == 1)
	{
		m_sKeyName = m_lstKeys.GetItemText(0, 1);
		m_sPassword = m_vecPasswords[0].c_str();
		EndDialog(IDOK);
	}

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
			int nRes = this->MessageBox(_T("Are you sure you want to delete this record ?"), _T("Delete record"), MB_ICONQUESTION | MB_YESNO);
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
		OnOK();
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

		if (m_hitInfo.iSubItem == 3 && !m_bShowPlainPwds)
		{
			m_ctrlEdit.SetPasswordChar(_T('*'));
		}
		else
		{
			m_ctrlEdit.SetPasswordChar(0);
		}

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
		if (m_hitInfo.iSubItem < 3)
		{
			m_lstKeys.SetItemText(
				m_hitInfo.iItem,
				m_hitInfo.iSubItem,
				sText);
		}
		else
		{
			m_lstKeys.SetItemText(
				m_hitInfo.iItem,
				m_hitInfo.iSubItem,
				(m_bShowPlainPwds) ? sText : _T("********"));
		}

		CJetTable tbl;
		CALL_JET(g_DB.GetTable("tb_keys", tbl));
		CALL_JET(tbl.BeginTransaction());
		CALL_JET(tbl.Move(m_hitInfo.iItem));
		if (m_hitInfo.iSubItem < 3)
		{
			CALL_JET(tbl.UpdateRow()
				.SetColumn(s_KeysColumns[m_hitInfo.iSubItem], sText)
				.Done());
		}
		else
		{
			IStreamPtr spStrm;
			CreateStreamOnHGlobal(0, TRUE, &spStrm);
			EncryptPassword(sText, spStrm);
			CALL_JET(tbl.UpdateRow()
				.SetColumn(s_KeysColumns[m_hitInfo.iSubItem], spStrm)
				.Done());
			m_vecPasswords[m_hitInfo.iItem] = sText;
		}
		CALL_JET(tbl.CommitTransaction());

		OnEditKillFocus();
	}
	else
	{
		// this code path is used when multiple entries are found
		// for a given app and we need to ask the user which password exactly is to be used
		int nSel = m_lstKeys.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
		if (nSel != -1)
		{
			m_sKeyName = m_lstKeys.GetItemText(nSel, 1);
			m_sPassword = m_vecPasswords[m_lstKeys.GetItemData(nSel)].c_str();
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

CString CMyKeysDlg::GetSelectedPassword()
{
	return m_sPassword;
}

void CMyKeysDlg::OnHdnItemclickLstKeys(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	// hacky to rely on password column being the last one, fix pending
	if (phdr->iItem == 3)
	{
		m_lstKeys.SetRedraw(FALSE);
		m_bShowPlainPwds = !m_bShowPlainPwds;
		for (size_t i = 0; i < m_vecPasswords.size(); i++)
		{
			m_lstKeys.SetItemText(i, 3, 
				(m_bShowPlainPwds) ? m_vecPasswords[m_lstKeys.GetItemData(i)].c_str() : _T("********"));
		}
		m_lstKeys.SetRedraw(TRUE);
	}
	else
	{
		struct _SORTPARAMS
		{
			CListCtrl *lstKeys;
			int iItem;
			int iSortOrder;
		} p = { &m_lstKeys, phdr->iItem, m_iSortOrder };
		
		m_iSortOrder *= -1;
		m_lstKeys.SortItemsEx([](LPARAM item1, LPARAM item2, LPARAM sortParam)
			{ 
				_SORTPARAMS *p = reinterpret_cast<_SORTPARAMS*>(sortParam);
				CString s1 = p->lstKeys->GetItemText(item1, p->iItem);
				CString s2 = p->lstKeys->GetItemText(item2, p->iItem);
				return s1.CompareNoCase(s2) * p->iSortOrder;
			}, 
			reinterpret_cast<LPARAM>(&p));
	}

	*pResult = 0;
}


void CMyKeysDlg::OnClickedBtnbackup()
{
	CString sBackupPath = AfxGetApp()->GetProfileString(_T("settings"), _T("BackupPath"));
	if (sBackupPath.IsEmpty())
	{
		BROWSEINFO brw = { 0 };
		brw.hwndOwner = GetSafeHwnd();
		brw.lpszTitle = _T("Select backup location");
		brw.ulFlags = BIF_RETURNONLYFSDIRS;
		LPITEMIDLIST pidl = SHBrowseForFolder(&brw);
		if (pidl)
		{
			SHGetPathFromIDList(pidl, sBackupPath.GetBuffer(_MAX_PATH));
			sBackupPath.ReleaseBuffer();
			
			CoTaskMemFree(pidl);

			AfxGetApp()->WriteProfileString(_T("settings"), _T("BackupPath"), sBackupPath);
		}
		else
		{
			return;
		}
	}

	CWaitCursor wc;
	USES_CONVERSION;	
	CALL_JET(g_DB.BackupDatabase(T2A(sBackupPath)));

EXIT:
	;
}


void CMyKeysDlg::OnClickedBtnrestore()
{
	CString sBackupPath = AfxGetApp()->GetProfileString(_T("settings"), _T("BackupPath"));
	if (sBackupPath.IsEmpty())
	{
		BROWSEINFO brw = { 0 };
		brw.hwndOwner = GetSafeHwnd();
		brw.lpszTitle = _T("Select backup location");
		brw.ulFlags = BIF_RETURNONLYFSDIRS;
		LPITEMIDLIST pidl = SHBrowseForFolder(&brw);
		if (pidl)
		{
			SHGetPathFromIDList(pidl, sBackupPath.GetBuffer(_MAX_PATH));
			sBackupPath.ReleaseBuffer();

			CoTaskMemFree(pidl);

			AfxGetApp()->WriteProfileString(_T("settings"), _T("BackupPath"), sBackupPath);
		}
		else
		{
			return;
		}
	}
	CString s;
	s.Format(_T("You are about to restore a keychain from this location '%s'\nThis operation will replace your current keychain\nAre you sure ?"), (LPCTSTR)sBackupPath);
	if(MessageBox(s, _T("Restore"), MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		CWaitCursor wait;
		USES_CONVERSION;
		CALL_JET(g_DB.RestoreDatabase(T2A(sBackupPath)));
		ReloadData();
	}

EXIT:
	;
}


BOOL CMyKeysDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	OpenHelp();
	return TRUE;
}
