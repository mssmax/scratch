#include "StdAfx.h"
#include "DBSchema.h"
#include <atlbase.h>

// TODO: Some review of error handling is needed

using namespace internal;

CDBSchema::CDBSchema()
{
	m_ulSchemaVersion = 0;
}


CDBSchema::~CDBSchema()
{
}

HRESULT CDBSchema::GetAttribute(IXmlReader *pReader, LPCWSTR pwszAttrName, LPCWSTR *pwszAttrValue, UINT *uiAttrValueSize)
{
	HRESULT hr = pReader->MoveToAttributeByName(pwszAttrName, NULL);
	if (FAILED(hr) || hr == S_FALSE)
	{
		*pwszAttrValue = L"";
		*uiAttrValueSize = 0;
		return hr;
	}

	hr = pReader->GetValue(pwszAttrValue, uiAttrValueSize);

	return hr;
}

UINT CDBSchema::MapStringToJetColType(LPCWSTR lpszStringType)
{
	if (_wcsicmp(lpszStringType, L"text") == 0)
		return JET_coltypText;
	else if (_wcsicmp(lpszStringType, L"longtext") == 0)
		return JET_coltypLongText;
	else if (_wcsicmp(lpszStringType, L"int32") == 0)
		return JET_coltypLong;
	else if (_wcsicmp(lpszStringType, L"datetime") == 0)
		return JET_coltypDateTime;
	else if (_wcsicmp(lpszStringType, L"binary") == 0)
		return JET_coltypLongBinary;

	// we failed to recognize the file
	return 0;
}

HRESULT CDBSchema::ReadSchema(LPCSTR lpszXML)
{
	HRESULT hr = S_OK;
	IStreamPtr spInput;
	if (_strnicmp(lpszXML, "xml://", 6) == 0)
	{
		hr = CreateStreamOnHGlobal(0, TRUE, &spInput);
		if (FAILED(hr))
		{
			return hr;
		}

		hr = spInput->Write(lpszXML + 6, lstrlenA(lpszXML) - 6, 0);
		if (FAILED(hr))
		{
			return hr;
		}

		LARGE_INTEGER liZero = { 0 };
		hr = spInput->Seek(liZero, STREAM_SEEK_SET, 0);
		if (FAILED(hr))
		{
			return hr;
		}
	}
	else
	{
		hr = SHCreateStreamOnFileA(lpszXML, STGM_READ, &spInput);
		if (FAILED(hr))
		{
			return hr;
		}
	}

	return ParseSchema(spInput);
}

HRESULT CDBSchema::ParseSchema(IStream *pInput)
{
	CComPtr<IXmlReader> spReader;
	HRESULT hr = CreateXmlReader(__uuidof(IXmlReader), reinterpret_cast<LPVOID*>(&spReader), NULL);
	if (FAILED(hr))
	{
		return hr;
	}

	hr = spReader->SetInput(pInput);
	if (FAILED(hr))
	{
		return hr;
	}

	for (;;)
	{
		XmlNodeType nodeType = XmlNodeType_None;
		hr = spReader->Read(&nodeType);
		if (FAILED(hr))
		{
			return hr;
		}

		if (hr == S_FALSE)
		{
			break;
		}

		if (nodeType == XmlNodeType_Element)
		{
			hr = ParseElement(spReader);
			if (FAILED(hr))
			{
				return hr;
			}
		}
	}
	return S_OK;
}

HRESULT CDBSchema::ParseElement(IXmlReader *pReader)
{
	LPCWSTR wszNodeName = 0;
	UINT wszNodeNameSize = 0;
	HRESULT hr = pReader->GetLocalName(&wszNodeName, &wszNodeNameSize);
	if (FAILED(hr))
	{
		return hr;
	}

	LPCWSTR wszAttrValue = 0;
	UINT uiAttrValueSize = 0;
	if (_wcsnicmp(wszNodeName, L"schema", wszNodeNameSize) == 0)
	{
		hr = GetAttribute(pReader, L"version", &wszAttrValue, &uiAttrValueSize);
		if (FAILED(hr))
		{
			return hr;
		}

		m_ulSchemaVersion = _wtol(wszAttrValue);
	}
	else if (_wcsnicmp(wszNodeName, L"table", wszNodeNameSize) == 0)
	{
		CDBTable tbl;
		hr = ParseTable(pReader, tbl);
		if (FAILED(hr))
		{
			return hr;
		}
		m_vecTables.push_back(tbl);
	}

	return S_OK;
}

HRESULT CDBSchema::ParseTable(IXmlReader *pReader, CDBTable &tbl)
{
	LPCWSTR wszTableName = 0;
	UINT uiTableNameSize = 0;
	HRESULT hr = GetAttribute(pReader, L"name", &wszTableName, &uiTableNameSize);
	if (FAILED(hr))
	{
		return hr;
	}

	tbl.m_sTableName = wszTableName;
	for (;;)
	{
		LPCWSTR wszNodeName = 0;
		UINT uiNodeSize = 0;
		XmlNodeType nodeType = XmlNodeType_None;
		hr = pReader->Read(&nodeType);
		if (FAILED(hr))
		{
			return hr;
		}

		if (hr == S_FALSE)
		{
			break;
		}

		pReader->GetLocalName(&wszNodeName, &uiNodeSize);
		if (nodeType == XmlNodeType_Element)
		{			
			if (_wcsnicmp(wszNodeName, L"column", uiNodeSize) == 0)
			{
				hr = ParseColumn(pReader, tbl);
				if (FAILED(hr))
				{
					return hr;
				}
			}
		}
		else if (nodeType == XmlNodeType_EndElement)
		{
			if (_wcsnicmp(wszNodeName, L"table", uiNodeSize) == 0)
			{
				break;
			}
		}
	}

	return S_OK;
}

HRESULT CDBSchema::ParseColumn(IXmlReader *pReader, CDBTable &tbl)
{
	CDBColumn col;
	LPCWSTR wszAttrValue = 0;
	UINT uiAttrSize = 0;
	HRESULT hr = GetAttribute(pReader, L"name", &wszAttrValue, &uiAttrSize);
	col.m_sColumnName = wszAttrValue;
	hr = GetAttribute(pReader, L"type", &wszAttrValue, &uiAttrSize);
	col.m_uiType = MapStringToJetColType(wszAttrValue);
	hr = GetAttribute(pReader, L"size", &wszAttrValue, &uiAttrSize);
	col.m_uiSize = _wtol(wszAttrValue);
	hr = GetAttribute(pReader, L"indexed", &wszAttrValue, &uiAttrSize);
	if (_wcsnicmp(wszAttrValue, L"yes", 3) == 0)
	{
		col.m_bIndexed = TRUE;
	}

	hr = GetAttribute(pReader, L"autoinc", &wszAttrValue, &uiAttrSize);
	if (_wcsnicmp(wszAttrValue, L"yes", 3) == 0)
	{
		col.m_bAutoInc = TRUE;
	}

	hr = GetAttribute(pReader, L"unique", &wszAttrValue, &uiAttrSize);
	if (_wcsnicmp(wszAttrValue, L"yes", 3) == 0)
	{
		col.m_bUnique = TRUE;
	}

	tbl.m_vecColumns.push_back(col);

	return hr;
}

JET_ERR CDBSchema::CreateSchema(JET_SESID sessionID, JET_DBID dbID)
{
	std::vector<CDBTable>::iterator it = m_vecTables.begin();
	for (; it != m_vecTables.end(); ++it)
	{
		JET_ERR e = CreateTable(sessionID, dbID, *it);
		if (e < 0)
		{
			return e;
		}
	}
	return 0;
}

JET_ERR CDBSchema::CreateTable(JET_SESID sessionID, JET_DBID dbID, CDBTable &table)
{
	USES_CONVERSION;
	JET_TABLEID tblID = 0;
	JET_ERR e = JetCreateTable(sessionID, dbID, W2A(table.m_sTableName.c_str()), 0, 0, &tblID);
	if (e < 0)
	{
		return e;
	}

	std::vector<CDBColumn>::iterator it = table.m_vecColumns.begin();
	for (; it != table.m_vecColumns.end(); ++it)
	{
		JET_COLUMNDEF columnDef = { 0 };
		columnDef.cbStruct = sizeof(columnDef);
		columnDef.coltyp = it->m_uiType;
		columnDef.cbMax = it->m_uiSize;
		columnDef.grbit = (it->m_bAutoInc) ? JET_bitColumnAutoincrement : 0;
		e = JetAddColumn(sessionID, tblID, W2A(it->m_sColumnName.c_str()), &columnDef, 0, 0, 0);
		if (e < 0)
		{
			return e;
		}
		if (it->m_bIndexed)
		{
			JET_INDEXCREATE idx = { 0 };
			idx.cbStruct = sizeof(idx);

			std::string sKeyName(W2A(it->m_sColumnName.c_str()));
			// this code uses a rather convoluted method of constructing the key
			// because Jet requires double NULL char at the end of the key used for indexing.
			// At the same time to avoid problems with aLine Quality check,
			// we resort to using strsafe functions instead of traditional snprintf
			char szKey[512] = "+";
			StringCbCatA(szKey, sizeof(szKey), sKeyName.c_str());
			sKeyName.append("_index");
			idx.szIndexName = &sKeyName[0];
			idx.szKey = szKey;
			idx.cbKey = lstrlenA(szKey) + 2;
			if (it->m_bUnique)
			{
				idx.grbit = JET_bitIndexUnique;
			}
			e = JetCreateIndex2(sessionID, tblID, &idx, 1);
			if (e < 0)
			{
				return e;
			}
		}
	}

	e = JetCloseTable(sessionID, tblID);
	return e;
}