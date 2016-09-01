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
	DDX_Control(pDX, IDC_CP_CONTENT, m__editContent);
	DDX_Control(pDX, IDC_CP_LIST, m_lstCPs);
	DDX_Control(pDX, IDC_EDITINPLACE, m_editInplace);
}

BEGIN_MESSAGE_MAP(CMyCopyPaste, CDialog)
END_MESSAGE_MAP()

BOOL CMyCopyPaste::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_lstCPs.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_lstCPs.InsertColumn(0, _T("Application"), 0, 180);
	m_lstCPs.InsertColumn(1, _T("Key name"), 0, 180);
	m_editInplace.SetParent(&m_lstCPs);
	
	ReloadData();

	m_pPrevForegroundWnd = GetForegroundWindow();
	SetForegroundWindow();

	m_lstCPs.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

CString CMyCopyPaste::GetPasteableText()
{
	UpdateData(TRUE);
	int nSel = m_lstCPs.GetNextItem(-1, LVNI_FOCUSED | LVNI_SELECTED);
	if (nSel >= 0)
	{
		return ConvA2W(m_vecText[nSel].c_str()).c_str();
	}
	else
	{
		return _T("");
	}
}

void CMyCopyPaste::OnOK()
{
	m_pPrevForegroundWnd->SetForegroundWindow();
	m_pPrevForegroundWnd = NULL;
	CDialog::OnOK();
}

void CMyCopyPaste::OnCancel()
{
	m_pPrevForegroundWnd->SetForegroundWindow();
	m_pPrevForegroundWnd = NULL;
	CDialog::OnCancel();
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
			char szStr[1024] = { 0 };
			CALL_JET(tbl.GetColumn(s_CPColumns[iSubItem], szStr, sizeof(szStr)));
			if (iSubItem < _countof(s_CPColumns))
			{
				m_lstCPs.SetItemText(iItem, iSubItem, ConvA2W(szStr).c_str());
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