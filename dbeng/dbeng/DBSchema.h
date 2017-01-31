#pragma once
namespace internal
{
	class CDBColumn
	{
	private:
		std::wstring m_sColumnName;
		UINT m_uiType;
		UINT m_uiSize;
		BOOL m_bIndexed;
		BOOL m_bAutoInc;
		BOOL m_bUnique;
		BOOL m_bTagged;
	public:
		CDBColumn()
		{
			m_uiType = 0;
			m_uiSize = 0;
			m_bIndexed = FALSE;
			m_bAutoInc = FALSE;
			m_bUnique = FALSE;
			m_bTagged = FALSE;
		}

		friend class CDBSchema;
	};

	class CDBTable
	{
	private:
		std::wstring m_sTableName;
		std::vector<CDBColumn> m_vecColumns;
	public:

		CDBTable()
		{}

		friend class CDBSchema;
	};

	class CDBSchema
	{
	private:
		ULONG m_ulSchemaVersion;
		std::vector<CDBTable> m_vecTables;
	private:
		HRESULT ParseSchema(IStream *pInput);
		HRESULT ParseElement(IXmlReader *pReader);
		HRESULT ParseTable(IXmlReader *pReader, CDBTable &tbl);
		HRESULT ParseColumn(IXmlReader *pReader, CDBTable &tbl);
		HRESULT GetAttribute(IXmlReader *pReader, LPCWSTR pwszAttrName, LPCWSTR *pwszAttrValue, UINT *uiAttrValueSize);
		JET_ERR CreateTable(JET_SESID sessionID, JET_DBID dbID, CDBTable &table);
		UINT MapStringToJetColType(LPCWSTR lpszStringType);
	public:
		CDBSchema();
		virtual ~CDBSchema();

		HRESULT ReadSchema(LPCSTR lpszXML);
		JET_ERR CreateSchema(JET_SESID sessionID, JET_DBID dbID);
	};
}
