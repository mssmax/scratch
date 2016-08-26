// findusers.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	if(argc < 3)
	{
		printf("Usage: findusers <searchtype> <searchstring> [<connectionstring>]");
		return 0;
	}

	LPCTSTR lpszConnectionString = "directorytype=ad";
	int iSearchType = atoi(argv[1]);
	if(argc > 3)
		lpszConnectionString = argv[3];


	CoInitialize(0);

	try
	{
		IGFIDBPtr spDB("GFIADCache.GFIUserDB");
		IGFIUserLocator2Ptr spLocator(spDB);

		spDB->Open(0, lpszConnectionString);

		int i = 2;
		int iCount = 0;
//		while(i < argc)
		{

			IEnumUOPtr spEnum(spLocator->FindUsersEx((SearchTypeEnum)iSearchType, argv[i++], VARIANT_FALSE));

			spEnum->Reset();
			IGFIUserPtr spUser = 0;
			while(spEnum->Next(1, &spUser, 0) == S_OK)
			{
				spUser->Open();
				_bstr_t bstrUserID(spUser->GetID());
				//_variant_t vDisplayName = spUser->GetProp("cn");

				char szSID[128] = "not present";
				try
				{
					_variant_t vObjectSID = spUser->GetProp("objectSID");
					SAFEARRAY *psa = V_ARRAY(&vObjectSID);
					PBYTE pv = 0;
					SafeArrayAccessData(psa, (LPVOID*)&pv);
					for(int i = 0; i < psa->rgsabound[0].cElements; i++)
						sprintf(szSID + i*2, "%02x", pv[i]);

					SafeArrayUnaccessData(psa);
				}
				catch(_com_error &)
				{
				}

				variant_t vMail;
				try
				{
					vMail = spUser->GetProp("mail");
				}
				catch(_com_error &)
				{
					vMail.SetString("not present");
				}
				

				_bstr_t bstrName;
				_bstr_t bstrLastName;

				try
				{
					_variant_t vName(spUser->GetProp("givenName"));
				
					bstrName = vName;
				}
				catch(_com_error &e)
				{
					bstrName = "not present";
				}

				try
				{
					_variant_t vLastName(spUser->GetProp("sn"));
					bstrLastName = vLastName;
				}
				catch(_com_error &e)
				{
					bstrLastName = "not present";
				}

				_bstr_t bstrMail(vMail);
				//_bstr_t bstrName(vDisplayName);

				printf("User:\n\tid[%s]\n\tsid[%s]\n\tdisplayName[%s]\n\temail[%s]\n", (LPCTSTR)bstrUserID, szSID, (LPCTSTR)bstrName, (LPCTSTR)bstrMail);
//				printf("%s,%s,%s,\n", (LPCTSTR)bstrName, (LPCTSTR)bstrLastName, (LPCTSTR)bstrMail);
#ifdef USE_ASSOCUO
				for(int i = 0; i < 2; i++)
				{
					printf("\tAssociated UOs[%d]:\n", i);
					DWORD dw = GetTickCount();
					IEnumUOPtr spAssocUO(spUser->GetAssociatedUOs(udbUOTypeSecurityGroups));
					dw = GetTickCount() - dw;
					printf("\tExtracted UO extracted in %d msec\n", dw);
					IGFIUserPtr spAssocUser;
					spAssocUO->Reset();
					while(spAssocUO->Next(1, &spAssocUser, 0) == S_OK)
					{
						_bstr_t bstrUserID(spAssocUser->GetID());
						try
						{
							spAssocUser->Open();
							_variant_t vDisplayName = spAssocUser->GetProp("cn");
							
							char szSID[128]="";
							try
							{
								_variant_t vObjectSID = spAssocUser->GetProp("objectSID");
								SAFEARRAY *psa = V_ARRAY(&vObjectSID);
								PBYTE pv = 0;
								SafeArrayAccessData(psa, (LPVOID*)&pv);
								for(int i = 0; i < psa->rgsabound[0].cElements; i++)
									sprintf(szSID + i*2, "%02x", pv[i]);
								
								SafeArrayUnaccessData(psa);
							}
							catch(_com_error &)
							{
								strcpy(szSID, "not present");
							}
							
							variant_t vMail;
							try
							{
								vMail = spAssocUser->GetProp("mail");
							}
							catch(_com_error &)
							{
								vMail.SetString("not present");
							}
							
							_bstr_t bstrMail(vMail);
							_bstr_t bstrName(vDisplayName);
							
							printf("\tUO:\n\t\tid[%s]\n\t\tsid[%s]\n\t\tdisplayName[%s]\n\t\temail[%s]\n", (LPCTSTR)bstrUserID, szSID, (LPCTSTR)bstrName, (LPCTSTR)bstrMail);
							spAssocUser.Release();
						}
						catch(_com_error &e)
						{
							printf("Error[0x%X, %s] when using %s\n", e.Error(), (LPCTSTR)e.Description(), (LPCSTR)bstrUserID);
						}
					}
				}
#endif
				spUser.Release();

				++iCount;
			}
		}
		printf("Finished enumerating users, total[%ld]\n", iCount);
	}
	catch(_com_error &e)
	{
		printf("Error[0x%X, %s]\n", e.Error(), (LPCTSTR)e.Description());
	}

	CoUninitialize();

	return 0;
}
