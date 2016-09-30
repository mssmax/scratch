#include "stdafx.h"
#include <conio.h>

char szDBSchema[] = "xml://<schema version=\"666\">" \
"<table name=\"tb_system\">" \
"<column name=\"tb_sys_id\" type=\"int32\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_seq\" type=\"int32\" autoinc=\"yes\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_schema\" type=\"longtext\" size=\"256\" indexed=\"yes\"/>" \
"<column name=\"tb_sys_bin\" type=\"binary\"/>" \
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
		e = db.BackupDatabase("c:\\temp\\backup");
		CJetTable table;
		e = db.GetTable("tb_system", table);
		/**
		e = table.BeginTransaction();
		for (int count = 1000; count < 1020; count++)
		{
			char buf[512] = { 0 };
			sprintf(buf, "Inserted record with sys_id %d", count);
			e = table.InsertRow()
				.SetColumn("tb_sys_id", count)
				.SetColumn("tb_sys_schema", buf)
				.Done();
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
			int iVersion = 0;
			int iSeq = 0;
			IStreamPtr strmData;
			SYSTEMTIME curTime = { 0 };
			e = table.GetColumn("tb_sys_schema", szString, sizeof(szString));
			e = table.GetColumn("tb_sys_id", &iVersion);
			e = table.GetColumn("tb_sys_seq", &iSeq);
			printf("string[%s], version[%d], seq[%d]\n", szString, iVersion, iSeq);
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