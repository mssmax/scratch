#include "StdAfx.h"
#include "ESEEngine.h"
#include "JetTable.h"

CJetTable::CJetTable()
{
	m_tblID = 0;
	m_tblBookmarks = 0;
	m_pDBEngine = 0;
	m_bFWDirection = TRUE;
	m_enCurrentMode = MODE_NONE;

	m_iIndexRangeSize = 0;
	ZeroMemory(m_arrIndexRanges, sizeof(m_arrIndexRanges));
	ZeroMemory(&m_lstBookmarks, sizeof(m_lstBookmarks));
}

CJetTable::CJetTable(JET_TABLEID tbl, CDBEngine *pEngine)
{
	m_tblID = tbl;
	m_tblBookmarks = 0;
	m_pDBEngine = pEngine;
	m_bFWDirection = TRUE;
	m_enCurrentMode = MODE_NONE;

	m_iIndexRangeSize = 0;
	ZeroMemory(m_arrIndexRanges, sizeof(m_arrIndexRanges));
	ZeroMemory(&m_lstBookmarks, sizeof(m_lstBookmarks));
}


CJetTable::~CJetTable()
{
	Close();
}

JET_ERR CJetTable::Open(CDBEngine *pEngine, LPCSTR lpszTableName)
{
	m_pDBEngine = pEngine;
	JET_ERR e = JetOpenTable(
		m_pDBEngine->GetSessionID(),
		m_pDBEngine->m_dbID,
		lpszTableName,
		0,
		0,
		0,
		&m_tblID
	);

	return e;
}

void CJetTable::Detach()
{
	m_tblID = 0;
}

void CJetTable::Close()
{
	CleanIndexRanges();
	if (m_tblID != 0)
	{
		JetCloseTable(m_pDBEngine->GetSessionID(), m_tblID);
		m_tblID = 0;
	}

	if (m_tblBookmarks != 0)
	{
		JetCloseTable(m_pDBEngine->GetSessionID(), m_tblBookmarks);
		m_tblBookmarks = 0;
		if (m_lstBookmarks.tableid != 0)
		{
			JetCloseTable(m_pDBEngine->GetSessionID(), m_lstBookmarks.tableid);
			ZeroMemory(&m_lstBookmarks, sizeof(m_lstBookmarks));
		}
	}

	m_bFWDirection = TRUE;
	m_enCurrentMode = MODE_NONE;
	m_pDBEngine = 0;
}

JET_ERR CJetTable::BeginTransaction()
{
	m_bFWDirection = TRUE;
	return JetBeginTransaction(m_pDBEngine->GetSessionID());
}

JET_ERR CJetTable::CommitTransaction()
{
	m_bFWDirection = TRUE;
	return JetCommitTransaction(m_pDBEngine->GetSessionID(), JET_bitCommitLazyFlush);
}

JET_ERR CJetTable::RollbackTransaction()
{
	m_bFWDirection = TRUE;
	JET_ERR e = JetRollback(m_pDBEngine->GetSessionID(), JET_bitRollbackAll);
	// we nullify table id because during rollback the table is automatically closed by ESE
	m_tblID = 0;

	return e;
}

JET_ERR CJetTable::MoveFirst()
{
	m_bFWDirection = TRUE;
	return JetMove(m_pDBEngine->GetSessionID(), m_tblID, JET_MoveFirst, 0);
}

JET_ERR CJetTable::MoveNext()
{
	return JetMove(m_pDBEngine->GetSessionID(), m_tblID, JET_MoveNext, 0);
}

JET_ERR CJetTable::MovePrev()
{
	return JetMove(m_pDBEngine->GetSessionID(), m_tblID, JET_MovePrevious, 0);
}

JET_ERR CJetTable::Move(long lRow)
{
	return JetMove(m_pDBEngine->GetSessionID(), m_tblID, lRow, 0);
}

JET_ERR CJetTable::NextRow()
{
	if (m_lstBookmarks.tableid != 0)
	{
		JET_ERR e = 0;

		BYTE byBookmark[1024] = { 0 };
		ULONG ulActual = 0;
		e = JetRetrieveColumn(
			m_pDBEngine->GetSessionID(),
			m_lstBookmarks.tableid,
			m_lstBookmarks.columnidBookmark,
			byBookmark,
			sizeof(byBookmark),
			&ulActual,
			0,
			0);

		e = JetGotoBookmark(m_pDBEngine->GetSessionID(),
			m_tblID,
			byBookmark,
			ulActual
		);

		e = JetMove(
			m_pDBEngine->GetSessionID(),
			m_lstBookmarks.tableid,
			JET_MoveNext,
			0);

		return e;
	}
	else
	{
		return (m_bFWDirection) ? MoveNext() : MovePrev();
	}
}

JET_ERR CJetTable::GetColInfo(LPCSTR pszColName, JET_COLUMNDEF* pColDef)
{
	return JetGetTableColumnInfo(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		pszColName,
		pColDef,
		sizeof(JET_COLUMNDEF),
		JET_ColInfo
	);
}

JET_TABLEID CJetTable::GetCursor()
{
	JET_INDEXRANGE *pRange = &m_arrIndexRanges[m_iIndexRangeSize];
	JET_TABLEID tblID = 0;
	JET_ERR e = 0;
	pRange->cbStruct = sizeof(JET_INDEXRANGE);
	pRange->grbit = JET_bitRecordInIndex;
	e = JetDupCursor(m_pDBEngine->GetSessionID(), m_tblID, &pRange->tableid, 0);
	tblID = pRange->tableid;
	++m_iIndexRangeSize;
	return (e >= 0) ? tblID : 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
// TODO: The following set of functions require additional work wrt error handling
//////////////////////////////////////////////////////////////////////////////////////////
CJetTable& CJetTable::SetColumn(LPCSTR lpszColumnName, LPCSTR value)
{
	JET_COLUMNDEF column = { 0 };
	GetColInfo(lpszColumnName, &column);
	JET_ERR e = JetSetColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		value,
		lstrlenA(value) + sizeof(CHAR),
		JET_bitSetOverwriteLV,
		0);

	return *this;
}

CJetTable& CJetTable::SetColumn(LPCSTR lpszColumnName, LPCWSTR value)
{
	JET_COLUMNDEF column = { 0 };
	GetColInfo(lpszColumnName, &column);
	std::string s(ConvW2A(value, CP_UTF8));
	JET_ERR e = JetSetColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		s.c_str(),
		s.length() + 1,
		JET_bitSetOverwriteLV,
		0);

	return *this;
}


CJetTable& CJetTable::SetColumn(LPCSTR lpszColumnName, int value)
{
	JET_COLUMNDEF column = { 0 };
	GetColInfo(lpszColumnName, &column);

	JET_ERR e = JetSetColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		&value,
		sizeof(value),
		0,
		0);

	return *this;
}

CJetTable& CJetTable::SetColumn(LPCSTR lpszColumnName, LPSYSTEMTIME value)
{
	double dblTime = 0;
	SystemTimeToVariantTime(value, &dblTime);
	JET_COLUMNDEF column = { 0 };
	GetColInfo(lpszColumnName, &column);

	JET_ERR e = JetSetColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		&dblTime,
		sizeof(double),
		0,
		0);

	return *this;
}

CJetTable& CJetTable::SetColumn(LPCSTR lpszColumnName, IStream *value)
{
	JET_ERR e = 0;
	JET_COLUMNDEF column = { 0 };
	GetColInfo(lpszColumnName, &column);
	
	JET_GRBIT bit = JET_bitSetAppendLV;
	if (m_enCurrentMode == MODE_UPDATE)
	{
		bit |= JET_bitSetOverwriteLV;
	}

	BYTE buffer[8192] = { 0 };
	ULONG ulRead = 0;
	HRESULT hr = S_OK;
	for (; hr == S_OK ;)
	{
		hr = value->Read(buffer, sizeof(buffer), &ulRead);
		if (FAILED(hr) || ulRead == 0)
		{
			break;
		}

		e = JetSetColumn(
			m_pDBEngine->GetSessionID(),
			m_tblID,
			column.columnid,
			buffer,
			ulRead,
			bit,
			0
		);
		// reset the bit back to "append" in case of an update operation so that data is appended
		bit = JET_bitSetAppendLV;
	}

	return *this;
}


//////////////////////////////////////////////////////////////////////////////////////////
JET_ERR CJetTable::SetColIndex(LPCSTR pszColName, JET_TABLEID tblID)
{
	char szIndex[512] = { 0 };
	StringCbPrintfA(szIndex, sizeof(szIndex), "%s_index", pszColName);
	// TODO: look into whether the direction flag should be flipped here
	m_bFWDirection = TRUE;
	return JetSetCurrentIndex(m_pDBEngine->GetSessionID(), tblID, szIndex);
}

JET_ERR CJetTable::GetColumn(JET_COLUMNID colID, void *pvData, int size)
{
	return  JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		colID,
		pvData,
		size,
		0,
		0,
		0
	);
}


JET_ERR CJetTable::GetColumn(LPCSTR lpszColumnName, LPSTR pszBuffer, int size)
{
	JET_COLUMNDEF column = { 0 };
	JET_ERR e = GetColInfo(lpszColumnName, &column);
	if (e < 0)
	{
		return e;
	}

	e = JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		pszBuffer,
		size,
		0,
		0,
		0
	);

	return e;
}

JET_ERR CJetTable::GetColumn(LPCSTR lpszColumnName, LPWSTR pszBuffer, int size)
{
	JET_COLUMNDEF column = { 0 };
	JET_ERR e = GetColInfo(lpszColumnName, &column);
	if (e < 0)
	{
		return e;
	}

	ULONG ulActual = 0;
	e = JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		nullptr,
		0,
		&ulActual,
		0,
		0
		);

	if (e < 0)
	{
		return e;
	}
	else
	{
		std::string s;
		s.resize(ulActual);
		e = JetRetrieveColumn(
			m_pDBEngine->GetSessionID(),
			m_tblID,
			column.columnid,
			&s[0],
			ulActual,
			0,
			0,
			0
			);
		if (e < 0)
		{
			return e;
		}
		StringCchCopy(pszBuffer, size, ConvA2W(s.c_str(), CP_UTF8).c_str());
	}

	return e;
}

JET_ERR CJetTable::GetColumn(LPCSTR lpszColumnName, int *pValue)
{
	JET_COLUMNDEF column = { 0 };
	JET_ERR e = GetColInfo(lpszColumnName, &column);
	if (e < 0)
	{
		return e;
	}

	e = JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		pValue,
		sizeof(int),
		0,
		0,
		0
	);

	return e;
}

JET_ERR CJetTable::GetColumn(LPCSTR lpszColumnName, LPSYSTEMTIME value)
{
	JET_COLUMNDEF column = { 0 };
	JET_ERR e = GetColInfo(lpszColumnName, &column);
	if (e < 0)
	{
		return e;
	}

	double dblTime = 0;

	e = JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		&dblTime,
		sizeof(double),
		0,
		0,
		0
	);

	VariantTimeToSystemTime(dblTime, value);

	return e;
}

JET_ERR CJetTable::GetColumn(LPCSTR lpszColumnName, IStream *value)
{
	JET_COLUMNDEF column = { 0 };
	JET_ERR e = GetColInfo(lpszColumnName, &column);

	if (e < 0)
	{
		return e;
	}

	// first we need to figure out the size of the column
	ULONG ulTotalSize = 0;
	LONG ulRemaining = 0;
	e = JetRetrieveColumn(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		column.columnid,
		0,
		0,
		&ulTotalSize,
		0,
		0
	);

	if (e < 0)
	{
		return e;
	}

	ulRemaining = static_cast<LONG>(ulTotalSize);

	JET_RETINFO retInfo = { 0 };
	retInfo.cbStruct = sizeof(retInfo);
	retInfo.itagSequence = 1;
	for (; e >= 0 && ulRemaining > 0;)
	{
		BYTE buffer[0x2000] = { 0 };
		ULONG ulCurrent = 0;
		e = JetRetrieveColumn(
			m_pDBEngine->GetSessionID(),
			m_tblID,
			column.columnid,
			buffer,
			sizeof(buffer),
			&ulCurrent,
			0,
			&retInfo
		);
		if (ulRemaining >= 0)
		{
			ULONG ulBytesToWrite = (ulCurrent > sizeof(buffer)) ? sizeof(buffer) : ulCurrent;
			HRESULT hr = value->Write(buffer, ulBytesToWrite, 0);
		}
		ulRemaining -= sizeof(buffer);
		retInfo.ibLongValue += sizeof(buffer);
	}

	return e;
}


CJetTable& CJetTable::InsertRow()
{
	m_enCurrentMode = MODE_INSERT;
	JET_ERR e = JetPrepareUpdate(m_pDBEngine->GetSessionID(), m_tblID, JET_prepInsert);
	return *this;
}

CJetTable& CJetTable::UpdateRow()
{
	m_enCurrentMode = MODE_UPDATE;
	JET_ERR e = JetPrepareUpdate(m_pDBEngine->GetSessionID(), m_tblID, JET_prepReplace);
	return *this;
}

JET_ERR CJetTable::DeleteRow()
{
	m_enCurrentMode = MODE_NONE;
	JET_ERR e = JetDelete(m_pDBEngine->GetSessionID(), m_tblID);
	return e;
}

JET_ERR CJetTable::Done()
{
	JET_ERR e = 0;
	if (m_enCurrentMode == MODE_SELECT)
	{
		if (m_iIndexRangeSize > 1)
		{
			m_lstBookmarks.cbStruct = sizeof(m_lstBookmarks);
			e = JetIntersectIndexes(
				m_pDBEngine->GetSessionID(),
				m_arrIndexRanges,
				m_iIndexRangeSize,
				&m_lstBookmarks,
				0
			);

			if (e < 0)
			{
				return e;
			}

			e = JetMove(m_pDBEngine->GetSessionID(),
				m_lstBookmarks.tableid,
				JET_MoveFirst,
				0);

			if (e < 0)
			{
				return e;
			}

			e = NextRow();
		}
		else // assume just one table was duplicated
		{
			JetCloseTable(m_pDBEngine->GetSessionID(), m_tblID);
			m_tblID = m_arrIndexRanges[0].tableid;
			m_arrIndexRanges[0].tableid = 0;
			m_iIndexRangeSize = 0;
		}
	}
	else
	{
		e = JetUpdate(m_pDBEngine->GetSessionID(), m_tblID, 0, 0, 0);
		if (e < 0)
		{
			JET_ERR e2 = JetPrepareUpdate(m_pDBEngine->GetSessionID(), m_tblID, JET_prepCancel);
		}
	}

	m_enCurrentMode = MODE_NONE;

	return e;
}

//////////////////////////////////////////////////////////////////////////////////

JET_ERR CJetTable::CleanIndexRanges()
{
	JET_ERR e = 0;
	for (int i = 0; i < m_iIndexRangeSize; i++)
	{
		e = JetCloseTable(m_pDBEngine->GetSessionID(), m_arrIndexRanges[i].tableid);
	}

	ZeroMemory(m_arrIndexRanges, sizeof(m_arrIndexRanges));
	m_iIndexRangeSize = 0;

	return e;
}

JET_ERR CJetTable::Select(JET_TABLEID tblID, LPCSTR lpszColumnName, SEEK_OPERAND operand, const void* data, int size)
{
	JET_SESID sessID = m_pDBEngine->GetSessionID();
	JET_ERR e = SetColIndex(lpszColumnName, tblID);
	if (e < 0)
	{
		return e;
	}

	e = JetMakeKey(
		sessID,
		tblID,
		data,
		size,
		JET_bitNewKey
	);

	if (e < 0)
	{
		return e;
	}

	e = JetSeek(sessID, tblID, operand | JET_bitSetIndexRange);

	if ((operand == OP_LE) || (operand == OP_LT))
	{
		m_bFWDirection = FALSE;
	}

	return e;
}

JET_ERR CJetTable::Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, int value)
{
	return Select(m_tblID, lpszColumnName, operand, &value, sizeof(value));
}

JET_ERR CJetTable::Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCSTR value)
{
	return Select(m_tblID, lpszColumnName, operand, value, lstrlenA(value));
}

JET_ERR CJetTable::Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCWSTR value)
{
	std::string s = ConvW2A(value);
	return Select(m_tblID, lpszColumnName, operand, s.c_str(), s.size());
}

JET_ERR CJetTable::Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPSYSTEMTIME value)
{
	double dblTime = 0;
	SystemTimeToVariantTime(value, &dblTime);
	return Select(m_tblID, lpszColumnName, operand, &dblTime, sizeof(double));
}

/////////////////////////////////////////////////////////////////////////////////
// TODO: subject to refactoring

CJetTable& CJetTable::Select()
{
	m_enCurrentMode = MODE_SELECT;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//TODO: add error checking - possibly exception throwin
CJetTable& CJetTable::Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, int value)
{
	JET_ERR e = 0;

	e = Select(GetCursor(), lpszColumnName, operand, &value, sizeof(value));

	return *this;
}

CJetTable& CJetTable::Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCSTR value)
{
	JET_ERR e = 0;

	e = Select(GetCursor(), lpszColumnName, operand, value, lstrlenA(value));

	return *this;
}

CJetTable& CJetTable::Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCWSTR value)
{
	JET_ERR e = 0;

	e = Select(GetCursor(), lpszColumnName, operand, value, lstrlen(value));

	return *this;
}

CJetTable& CJetTable::Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPSYSTEMTIME value)
{
	JET_ERR e = 0;

	double dblTime = 0;
	SystemTimeToVariantTime(value, &dblTime);
	e = Select(GetCursor(), lpszColumnName, operand, &dblTime, sizeof(double));

	return *this;
}

CJetTable& CJetTable::ByRange(LPCSTR lpszColumnName, int lowValue, int upValue)
{
	JET_ERR e = 0;
	JET_TABLEID tblid = GetCursor();

	e = Select(tblid, lpszColumnName, OP_GE, &lowValue, sizeof(int));
	if (e < 0)
	{
		return *this;
	}

	e = JetMakeKey(
		m_pDBEngine->GetSessionID(),
		tblid,
		&upValue,
		sizeof(int),
		JET_bitNewKey
	);

	e = JetSetIndexRange(
		m_pDBEngine->GetSessionID(),
		tblid,
		JET_bitRangeInclusive | JET_bitRangeUpperLimit
	);

	return *this;
}

CJetTable& CJetTable::ByRange(LPCSTR lpszColumnName, LPCWSTR lowValue, LPCWSTR upValue)
{
	return ByRange(
		lpszColumnName,
		ConvW2A(lowValue).c_str(),
		ConvW2A(upValue).c_str()
		);
}

CJetTable& CJetTable::ByRange(LPCSTR lpszColumnName, LPCSTR lowValue, LPCSTR upValue)
{
	JET_ERR e = 0;
	JET_TABLEID tblid = GetCursor();
	JET_SESID sessID = m_pDBEngine->GetSessionID();
	e = SetColIndex(lpszColumnName, tblid);
	
	if (e < 0)
	{
		return *this;
	}

	e = JetMakeKey(
		sessID,
		tblid,
		lowValue,
		lstrlenA(lowValue),
		JET_bitNewKey | JET_bitPartialColumnStartLimit
	);

	if (e < 0)
	{
		return *this;
	}

	e = JetSeek(sessID, tblid, JET_bitSeekGE);
	if (e < 0)
	{
		return *this;
	}

	e = JetMakeKey(
		m_pDBEngine->GetSessionID(),
		tblid,
		upValue,
		lstrlenA(upValue),
		JET_bitNewKey | JET_bitPartialColumnEndLimit //| JET_bitSubStrLimit
	);

	e = JetSetIndexRange(
		m_pDBEngine->GetSessionID(),
		tblid,
		JET_bitRangeUpperLimit | JET_bitRangeInclusive
	);

	return *this;
}


CJetTable& CJetTable::ByRange(LPCSTR lpszColumnName, LPSYSTEMTIME lowValue, LPSYSTEMTIME upValue)
{
	JET_ERR e = 0;
	JET_TABLEID	tblid = GetCursor();

	double dblStartTime = 0;
	double dblEndTime = 0;
	SystemTimeToVariantTime(upValue, &dblEndTime);
	e = Select(tblid, lpszColumnName, OP_GE, &dblStartTime, sizeof(double));
	if (e < 0)
	{
		return *this;
	}

	e = JetMakeKey(
		m_pDBEngine->GetSessionID(),
		tblid,
		&dblEndTime,
		sizeof(double),
		JET_bitNewKey
	);

	e = JetSetIndexRange(
		m_pDBEngine->GetSessionID(),
		tblid,
		JET_bitRangeInclusive | JET_bitRangeUpperLimit
	);

	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////

/* This is just a test function - do not use */
JET_ERR CJetTable::Join()
{
	JET_ERR e = 0;
	BYTE byColList[2048] = { 0 };
	JET_COLUMNLIST *colList = reinterpret_cast<JET_COLUMNLIST*>(byColList);
	e = JetGetTableColumnInfo(
		m_pDBEngine->GetSessionID(),
		m_tblID,
		0,
		colList,
		sizeof(byColList),
		JET_ColInfoList
	);

	if (e < 0)
	{
		return e;
	}

	std::vector<JET_COLUMNDEF> vecColumnDefs;
	std::vector<JET_COLUMNID> vecColumnIDs;
	for (; e >= 0;)
	{
		JET_COLUMNDEF colDef = { 0 };
		colDef.cbStruct = sizeof(colDef);
		char szColName[256] = { 0 };
		e = JetRetrieveColumn(
			m_pDBEngine->GetSessionID(),
			colList->tableid,
			colList->columnidcolumnname,
			szColName,
			sizeof(szColName),
			0,
			0,
			0
		);

		e = GetColInfo(szColName, &colDef);
		vecColumnDefs.push_back(colDef);

		e = JetMove(m_pDBEngine->GetSessionID(), colList->tableid, JET_MoveNext, 0);
	}

	e = JetCloseTable(m_pDBEngine->GetSessionID(), colList->tableid);

	vecColumnIDs.resize(vecColumnDefs.size());
	JET_TABLEID tblid = 0;
	e = JetOpenTempTable(
		m_pDBEngine->GetSessionID(),
		&vecColumnDefs[0],
		vecColumnDefs.size(),
		0,
		&tblid,
		&vecColumnIDs[0]);

	CJetTable tempTable(tblid, m_pDBEngine);
	tempTable.InsertRow()
		.SetColumn("tb_sys_seq", 1)
		.Done();
	return e;
}

BOOL CJetTable::IsEmpty()
{
	JET_ERR e = JetMove(m_pDBEngine->GetSessionID(), m_tblID, JET_MoveFirst, 0);
	if (e == JET_errSuccess)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}