#include "stdafx.h"
#include <conio.h>

char szDBSchema[] = "xml://<schema version=\"666\">" \
"<table name=\"tb_system\">" \
"<column name=\"tb_sys_id\" type=\"int32\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_seq\" type=\"int32\" autoinc=\"yes\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_schema\" type=\"longtext\" size=\"256\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_bin\" type=\"binary\"/>" \
"<column name=\"tb_sys_tag\" type=\"text\" tagged=\"yes\"/>" \
"</table>" \
"</schema>";

int main()
{
	CDBEngine db;

	JET_ERR e = db.Init("DBS", "mes");

	printf("Engine init[%d]\n", e);
	if (GetFileAttributes(_T("dbs\\testdatabase.jet")) == -1)
	{
		e = db.CreateDatabase("dbs\\testdatabase.jet", szDBSchema);
	}
	else
	{
		e = db.OpenDatabase("dbs\\testdatabase.jet", FALSE);
	}
	printf("DB opened[%d]\n", e);
	{
		CJetTable table;
		e = db.GetTable("tb_system", table);
		/**
		e = table.BeginTransaction();
		for (int count = 1000; count < 1020; count++)
		{
			char buf[512] = { 0 };
			sprintf(buf, "Inserted record with sys_id %d", count);
			table.InsertRow();
			e = table
				.SetColumn("tb_sys_id", count)
				.SetColumn("tb_sys_schema", buf);
			for (int k = 1; k <= 5; k++)
			{
				char s[256] = { 0 };
				sprintf(s, "tagged %d", k + count);
				e = table.SetColumn("tb_sys_tag", s, 0);
			}
			e = table.Done();
		}
		e = table.CommitTransaction();

		goto EXIT;

		/**/
		//e = table.Select()
		//	.ByRange("tb_sys_schema", "f", "fa")
		//	.ByRange("tb_sys_seq", 5, 10)
		//	.Where("tb_sys_seq", OP_GE, 10)
		//	.Done();
		
		/*
		CONCEPT: Join method

		Get table -> set conditions
		Get another table -> set conditions
		Call Join with the two tables to produce a third, temporary table that represents the join result
		The third table likely has to combine all the columns of the two tables being joined
		*/

		//e = table.Join();
		e = table.BeginTransaction();
		for (; e >= 0;)
		{
			char szString[1024] = { 0 };
			char szSchema[1024] = { 0 };
			int iVersion = 0;
			int iSeq = 0;
			IStreamPtr strmData;
			SYSTEMTIME curTime = { 0 };
			BYTE byKey[256] = { 0 };
			ULONG ulActual = 0;
			e = JetRetrieveKey(db.GetSessionID(), table, byKey, sizeof(byKey), &ulActual, 0);
			e = table.GetColumn("tb_sys_schema", szSchema, sizeof(szSchema));
			printf("schema[%s]\n", szSchema);
			for (int k = 1; e == 0; k++)
			{
				e = table.GetColumn("tb_sys_tag", szString, sizeof(szString), k);
				if(e == 0)
					printf("\ttag[%s]\n", szString);
			}
			e = table.NextRow();
		}
		char szErrStr[1024] = { 0 };
		db.GetErrorString(e, szErrStr, sizeof(szErrStr));
		e = table.CommitTransaction();
		/**/
	}
EXIT:
	return 0;
}	