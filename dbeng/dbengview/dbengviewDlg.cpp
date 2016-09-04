#include "stdafx.h"
#include "dbengview.h"
#include "dbengviewDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::wstring ConvA2W(LPCSTR lpsz)
{
	std::wstring ret;
	int size = MultiByteToWideChar(CP_ACP, 0, lpsz, -1, 0, 0);
	if (size)
	{
		ret.resize(size);
		MultiByteToWideChar(CP_ACP, 0, lpsz, -1, &ret[0], size);
		ret.resize(size - 1);
	}
	return ret;
}

std::string ConvW2A(LPCWSTR lpsz)
{
	std::string ret;
	int size = WideCharToMultiByte(CP_ACP, 0, lpsz, -1, 0, 0, 0, 0);
	if (size)
	{
		ret.resize(size);
		WideCharToMultiByte(CP_ACP, 0, lpsz, -1, &ret[0], size, 0, 0);
		ret.resize(size - 1);
	}
	return ret;
}


CDBEngViewDlg::CDBEngViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DBENGVIEW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDBEngViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_lstData);
	DDX_Control(pDX, IDC_COMBO_TABLES, m_cmbTables);
}

BEGIN_MESSAGE_MAP(CDBEngViewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnClickedOpen)
	ON_BN_CLICKED(IDC_CLOSE, OnClickedClose)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLES, &CDBEngViewDlg::OnSelchangeComboTables)
END_MESSAGE_MAP()


// CDBEngViewDlg message handlers

BOOL CDBEngViewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_lstData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDBEngViewDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDBEngViewDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDBEngViewDlg::OnClickedOpen()
{
	CFileDialog fd(TRUE);
	fd.GetOFN().Flags |= OFN_FILEMUSTEXIST;
	int res = fd.DoModal();
	if (res == IDOK)
	{
		CWaitCursor wait;
		CString sDBPath = fd.GetPathName();
		CString sDBFile = fd.GetFileName();
		sDBPath = sDBPath.Left(sDBPath.GetLength() - sDBFile.GetLength());
		JET_ERR e = g_DB.Init(ConvW2A(sDBPath).c_str(), "dbv");
		e = g_DB.OpenDatabase(ConvW2A(fd.GetPathName()).c_str(), FALSE);
		if (e < 0)
		{
			char szErr[1024] = { 0 };
			CString sErr;
			g_DB.GetErrorString(e, szErr, _countof(szErr));
			sErr.Format(_T("An error occurred while opening the database, [%S]"), szErr);
			AfxMessageBox(sErr, MB_OK | MB_ICONERROR);
			g_DB.Term();
		}
		else
		{
			SetWindowText(fd.GetPathName());
			EnumerateTables();
		}
	}
}


void CDBEngViewDlg::OnClickedClose()
{
	m_cmbTables.ResetContent();
	m_lstData.DeleteAllItems();
	for (; m_lstData.DeleteColumn(0);)
	{
	}

	g_DB.CloseDatabase();
	g_DB.Term();
	SetWindowText(_T("dbengview"));
}

void CDBEngViewDlg::EnumerateTables()
{
	JET_OBJECTLIST objInfo = { 0 };
	JET_ERR e = JetGetObjectInfo(
		g_DB.GetSessionID(),
		g_DB.GetDBID(),
		JET_objtypTable,
		0,
		0,
		reinterpret_cast<void*>(&objInfo),
		sizeof(objInfo),
		JET_ObjInfoListNoStats);
	CJetTable tbl;
	g_DB.GetTable(objInfo.tableid, tbl);
	for (; e >= 0; )
	{
		char szTableName[512] = { 0 };
		e = tbl.GetColumn(objInfo.columnidobjectname, szTableName, sizeof(szTableName));
		if(e >= 0)
		{ 
			m_cmbTables.AddString(ConvA2W(szTableName).c_str());
		}
		e = tbl.NextRow();
	}

	m_cmbTables.SetCurSel(0);
}


void CDBEngViewDlg::OnSelchangeComboTables()
{
	CString sTable;
	int idx = m_cmbTables.GetCurSel();
	if (idx != -1)
	{
		CWaitCursor wait;
		m_lstData.DeleteAllItems();
		for(; m_lstData.DeleteColumn(0);)
		{
		}
		m_cmbTables.GetLBText(idx, sTable);
		EnumerateColumnsAndData(sTable);
	}
}

void CDBEngViewDlg::EnumerateColumnsAndData(CString &sTable)
{
	USES_CONVERSION;
	JET_COLUMNLIST colList = { 0 };
	JET_ERR e = JetGetColumnInfo(
		g_DB.GetSessionID(),
		g_DB.GetDBID(),
		ConvW2A(sTable).c_str(),
		0,
		reinterpret_cast<void*>(&colList),
		sizeof(colList),
		JET_ColInfoList);

	CJetTable tbl;
	g_DB.GetTable(colList.tableid, tbl);
	tbl.BeginTransaction();
	std::vector<COLUMN_INFO> vecColumns;
	for (int colIdx = 0; e >= 0; colIdx++)
	{
		COLUMN_INFO colInfo;
		colInfo.sName.resize(256);
		e = tbl.GetColumn(colList.columnidcolumnname, &colInfo.sName[0], colInfo.sName.size());
		if (e >= 0)
		{
			LVCOLUMN lvCol = { 0 };
			lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
			lvCol.cx = 170;
			lvCol.fmt = LVCFMT_LEFT;
			lvCol.pszText = A2T(colInfo.sName.c_str());
			m_lstData.InsertColumn(colIdx, &lvCol);
		}

		e = tbl.GetColumn(colList.columnidcoltyp, &colInfo.lType, sizeof(colInfo.lType));
		if (e >= 0)
		{
			vecColumns.push_back(colInfo);
		}

		e = tbl.NextRow();
	}
	tbl.CommitTransaction();

	if (!vecColumns.empty())
	{
		PopulateData(sTable, vecColumns);
	}
}

// slightly fragile approach because we depend on the correlation between contents of the
// columns vector and the order of the headers in the list control
void CDBEngViewDlg::PopulateData(CString &sTable, std::vector<COLUMN_INFO> &vecColumns)
{
	IStreamPtr spStrm;

	CJetTable tbl;
	JET_ERR e = g_DB.GetTable(ConvW2A(sTable).c_str(), tbl);
	if (e < 0)
	{
		return;
	}
	e = tbl.BeginTransaction();
	int iItem = 0;
	for (; e >= 0; )
	{
		m_lstData.InsertItem(iItem, _T(""));
		for (size_t iSubItem = 0; iSubItem < vecColumns.size(); iSubItem++)
		{
			std::wstring sItemText;
			WCHAR szStrValue[1024] = { 0 };
			int lValue = 0;
			SYSTEMTIME timeValue = { 0 };
			switch (vecColumns[iSubItem].lType)
			{
			case JET_coltypText:
			case JET_coltypLongText:
				e = tbl.GetColumn(vecColumns[iSubItem].sName.c_str(), szStrValue, _countof(szStrValue));
				sItemText = szStrValue;
				break;
			case JET_coltypLong:
				e = tbl.GetColumn(vecColumns[iSubItem].sName.c_str(), &lValue);
				sItemText.resize(16);
				StringCbPrintf(&sItemText[0], sItemText.size(), _T("%d"), lValue);
				break;
			case JET_coltypBinary:
			case JET_coltypLongBinary:
				{
					CreateStreamOnHGlobal(0, TRUE, &spStrm);
					e = tbl.GetColumn(vecColumns[iSubItem].sName.c_str(), spStrm);
					if (e < 0)
					{
						sItemText = _T("<binary data>");
					}
					else if (e == JET_wrnColumnNull)
					{
						sItemText = _T("<null>");
					}
					else
					{
						STATSTG stat = { 0 };
						spStrm->Stat(&stat, STATFLAG_NONAME);
						std::vector<BYTE> vecBytes;
						vecBytes.resize(stat.cbSize.LowPart);
						ULONG ulRead = 0;
						spStrm->Read(&vecBytes[0], vecBytes.size(), &ulRead);
						DWORD dwSize = 0;
						if (CryptBinaryToString(
							&vecBytes[0],
							vecBytes.size(),
							CRYPT_STRING_HEX,
							0,
							&dwSize
						))
						{
							sItemText.resize(dwSize - 1);
							CryptBinaryToString(
								&vecBytes[0],
								vecBytes.size(),
								CRYPT_STRING_HEX,
								&sItemText[0],
								&dwSize);
						}
					}
				}
				break;
			case JET_coltypDateTime:
				e = tbl.GetColumn(vecColumns[iSubItem].sName.c_str(), &timeValue);
				sItemText.resize(512);
				{
					int numChars = GetDateFormat(
						LOCALE_INVARIANT,
						0,
						&timeValue,
						_T("dd/MM/yyyy "),
						&sItemText[0],
						sItemText.size());
					if (numChars > 0)
					{
						GetTimeFormat(
							LOCALE_INVARIANT,
							0,
							&timeValue,
							_T("HH:mm:ss"),
							&sItemText[numChars - 1],
							sItemText.size() - numChars);
					}
				}
				break;
			default:
				sItemText = L"<type unknown>";
				break;
			}

			m_lstData.SetItemText(iItem, iSubItem, sItemText.c_str());
		}
		
		iItem++;
		e = tbl.NextRow();
	}

	tbl.CommitTransaction();
}
