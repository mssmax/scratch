#include "stdafx.h"
#include "ESEEngine.h"


CDBEngine::CDBEngine()
{
	m_dbInstance = 0;
	m_dbID = 0;
	ZeroMemory(m_sDBName, sizeof(m_sDBName));
}


CDBEngine::~CDBEngine()
{
	CloseDatabase();
	Term();
}

JET_ERR CDBEngine::Init(LPCSTR lpszDatabasePath, LPCSTR lpszFileNamePrefix, LPCSTR lpszBackupPath)
{
	// TODO: add proper error handling
	std::string sTempDBPath(lpszFileNamePrefix);
	sTempDBPath.append(".edb");
	JET_ERR e = JetCreateInstance(&m_dbInstance, lpszFileNamePrefix);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramLogFilePath, 0, lpszDatabasePath);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramSystemPath, 0, lpszDatabasePath);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramBaseName, 0, lpszFileNamePrefix);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramCircularLog, 1, 0);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramCreatePathIfNotExist, 1, 0);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramMaxSessions, 1024, 0);
	e = JetSetSystemParameter(&m_dbInstance, 0, JET_paramTempPath, 0, sTempDBPath.c_str());
	if (lpszBackupPath)
	{
		// before restoring we need to delete any existing files
		// otherwise restore will fail with badSignature error
		DeleteAllFiles(lpszDatabasePath);
		e = JetRestoreInstance(m_dbInstance, lpszBackupPath, lpszDatabasePath, 0);
	}
	e = JetInit(&m_dbInstance);

	return e;
}

JET_ERR CDBEngine::Term()
{
	JET_ERR e = 0;

	if (g_dwSessionTLSID > 0)
	{
		JET_SESID id = reinterpret_cast<JET_SESID>(TlsGetValue(g_dwSessionTLSID));
		if (id)
		{
			JetEndSession(id, 0);
		}
		TlsFree(g_dwSessionTLSID);
		g_dwSessionTLSID = 0;
	}

	if (m_dbInstance != 0)
	{
		e = JetTerm(m_dbInstance);
		m_dbInstance = 0;
	}

	return e;
}

JET_SESID CDBEngine::GetSessionID()
{
	if (g_dwSessionTLSID == 0)
	{
		g_dwSessionTLSID = TlsAlloc();
	}

	JET_SESID id = reinterpret_cast<JET_SESID>(TlsGetValue(g_dwSessionTLSID));
	if (id == 0)
	{
		JET_ERR e = JetBeginSession(m_dbInstance, &id, 0, 0);
		TlsSetValue(g_dwSessionTLSID, reinterpret_cast<LPVOID>(id));
	}
	return id;
}

JET_INSTANCE CDBEngine::GetInstanceID()
{
	return m_dbInstance;
}

JET_DBID CDBEngine::GetDBID()
{
	return m_dbID;
}

JET_ERR CDBEngine::CreateDatabase(LPCSTR lpszDB, LPCSTR lpszSchema)
{
	JET_ERR e = JetCreateDatabase(GetSessionID(), lpszDB, 0, &m_dbID, JET_bitDbOverwriteExisting);
	if (e == 0)
	{
		StringCbCopyA(m_sDBName, sizeof(m_sDBName), lpszDB);
		internal::CDBSchema dbSchema;
		HRESULT hr = dbSchema.ReadSchema(lpszSchema);
		if (FAILED(hr))
		{
			return hr;
		}

		e = dbSchema.CreateSchema(GetSessionID(), m_dbID);
	}
	return e;
}

JET_ERR CDBEngine::OpenDatabase(LPCSTR lpszDB, BOOL bReadOnly)
{
	JET_ERR e = JetAttachDatabase(GetSessionID(), lpszDB, (bReadOnly) ? JET_bitDbReadOnly : 0);
	if (e < 0)
	{
		return e;
	}
	
	e = JetOpenDatabase(GetSessionID(), lpszDB, 0, &m_dbID, (bReadOnly) ? JET_bitDbReadOnly : 0);
	if (e < 0)
	{
		return e;
	}

	StringCbCopyA(m_sDBName, sizeof(m_sDBName), lpszDB);
	return e;
}

JET_ERR CDBEngine::CloseDatabase()
{
	JET_ERR e = 0;
	if (m_dbID)
	{
		e = JetCloseDatabase(GetSessionID(), m_dbID, 0);
		m_dbID = 0;
		e = JetDetachDatabase(GetSessionID(), m_sDBName);
		ZeroMemory(m_sDBName, sizeof(m_sDBName));
	}
	return e;
}

JET_ERR CDBEngine::BackupDatabase(LPCSTR lpszDestPath)
{
	return JetBackupInstance(m_dbInstance, lpszDestPath, JET_bitBackupAtomic, 0);
}

JET_ERR CDBEngine::RestoreDatabase(LPCSTR lpszBackupPath)
{
	char szDBPath[1024] = { 0 };
	char szDBName[1024] = { 0 };
	char szFileNamePrefix[128] = { 0 };

	JET_ERR e = JetGetDatabaseInfo(
		GetSessionID(),
		GetDBID(),
		szDBName,
		sizeof(szDBName),
		JET_DbInfoFilename);

	e = JetGetSystemParameter(
		m_dbInstance,
		GetSessionID(),
		JET_paramSystemPath,
		0,
		szDBPath,
		sizeof(szDBPath));

	e = JetGetSystemParameter(
		m_dbInstance, 
		GetSessionID(), 
		JET_paramBaseName, 
		0, 
		szFileNamePrefix,
		sizeof(szFileNamePrefix));

	CloseDatabase();
	Term();

	Init(szDBPath, szFileNamePrefix, lpszBackupPath);
	OpenDatabase(szDBName, FALSE);

	return 0;
}


JET_ERR CDBEngine::GetTable(LPCSTR lpszTableName, CJetTable &table)
{
	return table.Open(this, lpszTableName);
}

JET_ERR CDBEngine::GetTable(JET_TABLEID tableID, CJetTable &table)
{
	table.m_tblID = tableID;
	table.m_pDBEngine = this;
	return 0;
}


void CDBEngine::GetErrorString(int error, LPSTR szOutString, size_t outStrSize)
{
	JetGetSystemParameter(
		GetInstanceID(),
		GetSessionID(),
		JET_paramErrorToString,
		reinterpret_cast<JET_API_PTR*>(&error),
		szOutString,
		outStrSize);
}
