// msipreview.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <conio.h>

int main(int argc, char* argv[])
{

	if(argc != 2 && argc != 3)
	{
		printf("Usage: msipreview database.msi [dialogname]\n");
		return 0;
	}

	MSIHANDLE hDatabase = 0;
	UINT err = 0;
	if((err = MsiOpenDatabase(argv[1], MSIDBOPEN_READONLY, &hDatabase)) != ERROR_SUCCESS)
	{
		printf("Failed to open database[%s], err[%d]\n", argv[1], err);
		return 0;
	}

	if(argc == 3) // two arguments - database and dialog name -> we can preview
	{
		MSIHANDLE hPreview = 0;
		if((err = MsiEnableUIPreview(hDatabase, &hPreview)) != ERROR_SUCCESS)
		{
			printf("Failed to enable preview, err[%d]\n", err);
			MsiCloseHandle(hDatabase);
			return 0;
		}

		if((err = MsiPreviewDialog(hPreview, argv[2])) != ERROR_SUCCESS)
		{
			printf("Error showing dialog, err[%d]\n", err);
		}

		getch();

		MsiCloseHandle(hPreview);
	}
	else if(argc == 2) // just the database name -> output all the dialogs in that database
	{
		MSIHANDLE hView = 0;
		err = MsiDatabaseOpenView(hDatabase, "SELECT Dialog FROM Dialog", &hView);
		if(err != ERROR_SUCCESS)
		{
			printf("Failed to open view, err[%d]\n", err);
		}
		else
		{
			err = MsiViewExecute(hView, 0);
			if(err != ERROR_SUCCESS)
			{
				printf("Failed to execute view, err[%d]\n", err);
			}
			else
			{
				printf("Available dialogs:\n\n");
				for(;;)
				{
					MSIHANDLE hRecord = 0;
					err = MsiViewFetch(hView, &hRecord);
					if(err != ERROR_SUCCESS)
					{
						if(err != ERROR_NO_MORE_ITEMS)
							printf("Error while fetching a record, err[%d]\n", err);
						break;
					}

					TCHAR szBuf[1024] = {0};
					DWORD dwSize = 1024;
					err = MsiRecordGetString(hRecord, 1, szBuf, &dwSize);
					if(err != ERROR_SUCCESS)
					{
						if(err == ERROR_MORE_DATA)
						{
							printf("The buffer was too small, the size required is [%d]\n", dwSize);
							continue;
						}
						else
							break;
					}
					
					printf("%s\n", szBuf);

					MsiCloseHandle(hRecord);
				}
			}

			MsiViewClose(hView);
		}
	}
	


	MsiCloseHandle(hDatabase);


	return 0;
}
