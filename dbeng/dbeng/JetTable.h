#pragma once

class CDBEngine;

typedef enum
{
	OP_EQ = JET_bitSeekEQ,
	OP_GT = JET_bitSeekGT,
	OP_GE = JET_bitSeekGE,
	OP_LT = JET_bitSeekLT,
	OP_LE = JET_bitSeekLE
} SEEK_OPERAND;

typedef enum
{
	MODE_NONE,
	MODE_INSERT,
	MODE_UPDATE,
	MODE_SELECT
} MODUS_OPERANDI;

class CJetTable
{
	friend class CDBEngine;
private:
	BOOL m_bFWDirection;
	MODUS_OPERANDI m_enCurrentMode;
	JET_TABLEID m_tblID;
	JET_TABLEID m_tblBookmarks;
	JET_RECORDLIST m_lstBookmarks;

	JET_INDEXRANGE m_arrIndexRanges[64];
	int m_iIndexRangeSize;

	CDBEngine *m_pDBEngine;

	JET_ERR Open(CDBEngine *pEngine, LPCSTR lpszTableName);
	void Close();

	JET_ERR GetColInfo(LPCSTR pszColName, JET_COLUMNDEF* pColDef);
	JET_ERR SetColIndex(LPCSTR pszColName, JET_TABLEID tblID);

	JET_ERR Select(JET_TABLEID tblID, LPCSTR lpszColumnName, SEEK_OPERAND operand, const void* data, int size);

	JET_TABLEID GetCursor();
	JET_ERR CleanIndexRanges();
public:
	CJetTable();
	CJetTable(JET_TABLEID tbl, CDBEngine *pEngine);
	virtual ~CJetTable();

	operator JET_TABLEID()
	{
		return m_tblID;
	}

	CJetTable& operator=(const JET_TABLEID tblid)
	{
		m_tblID = tblid;
		return *this;
	}

	void Detach();

	JET_ERR BeginTransaction();
	JET_ERR CommitTransaction();
	JET_ERR RollbackTransaction();

	JET_ERR NextRow();

	JET_ERR MoveFirst();
	JET_ERR MoveNext();
	JET_ERR MovePrev();
	JET_ERR Move(long lRow);

	// TODO: temporary solution, this should probably be refactored into a separate class
	CJetTable& SetColumn(LPCSTR lpszColumnName, LPCSTR value);
	CJetTable& SetColumn(LPCSTR lpszColumnName, int value);
	CJetTable& SetColumn(LPCSTR lpszColumnName, LPSYSTEMTIME value);
	// intended for columns of type "binary"
	CJetTable& SetColumn(LPCSTR lpszColumnName, IStream *value);

	JET_ERR GetColumn(LPCSTR lpszColumnName, LPSTR pszBuffer, int size);
	JET_ERR GetColumn(LPCSTR lpszColumnName, int *pValue);
	JET_ERR GetColumn(LPCSTR lpszColumnName, LPSYSTEMTIME value);
	// intended for columns of type "binary"
	JET_ERR GetColumn(LPCSTR lpszColumnName, IStream *value);

	// for internal use only, may become documented in the future
	JET_ERR GetColumn(JET_COLUMNID colID, void *pvData, int size);

	JET_ERR Done();

	CJetTable& InsertRow();
	CJetTable& UpdateRow();
	JET_ERR DeleteRow();

	JET_ERR Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, int value);
	JET_ERR Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCSTR value);
	JET_ERR Select(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPSYSTEMTIME value);

	///////////////////////////////////////////////////////////////////////////////
	// TODO: subject to refactoring
	CJetTable& Select();

	CJetTable& Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, int value);
	CJetTable& Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPCSTR value);
	CJetTable& Where(LPCSTR lpszColumnName, SEEK_OPERAND operand, LPSYSTEMTIME value);

	CJetTable& ByRange(LPCSTR lpszColumnName, int lowValue, int upValue);
	CJetTable& ByRange(LPCSTR lpszColumnName, LPCSTR lowValue, LPCSTR upValue);
	CJetTable& ByRange(LPCSTR lpszColumnName, LPSYSTEMTIME lowValue, LPSYSTEMTIME upValue);
	///////////////////////////////////////////////////////////////////////////////
	JET_ERR Join();
};

