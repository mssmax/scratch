#pragma once
#include <esent.h>
#include "JetTable.h"
#pragma comment(lib, "esent.lib")

extern DWORD g_dwSessionTLSID;

class CDBEngine
{
	friend class CJetTable;
private:
	JET_INSTANCE m_dbInstance;
	JET_DBID m_dbID;
	char m_sDBName[512];
public:
	CDBEngine();
	virtual ~CDBEngine();

	JET_SESID GetSessionID();
	JET_INSTANCE GetInstanceID();
	JET_DBID GetDBID();

	JET_ERR Init(LPCSTR lpszDatabasePath, LPCSTR lpszFileNamePrefix);
	JET_ERR Term();

	JET_ERR CreateDatabase(LPCSTR lpszDB, LPCSTR lpszSchema);

	JET_ERR OpenDatabase(LPCSTR lpszDB, BOOL bReadOnly);

	JET_ERR CloseDatabase();

	JET_ERR BackupDatabase(LPCSTR lpszDestPath);
	JET_ERR RestoreDatabase(LPCSTR lpszBackupPath);

	JET_ERR GetTable(LPCSTR lpszTableName, CJetTable &table);

	// for internal use only, may become documented in the future
	JET_ERR GetTable(JET_TABLEID tableID, CJetTable &table);

	void GetErrorString(int error, LPSTR szOutString, size_t outStrSize);
};