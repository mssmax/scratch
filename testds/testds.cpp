#include "stdafx.h"
#include <shlwapi.h>


int _tmain(int argc, _TCHAR* argv[])
{
	DOMAIN_CONTROLLER_INFO *pGC = NULL;
	DWORD d = DsGetDcName(NULL, NULL, NULL, NULL, 0/* DS_GC_SERVER_REQUIRED*/, &pGC);
	//_tprintf(_T("GC name[%s]\n"), pGC->

	LPTSTR szSiteName = NULL;
	HANDLE hDS = NULL;
	DS_DOMAIN_TRUSTS *pDomainTrust = NULL;
	ULONG ulCount = 0;
	DsEnumerateDomainTrusts(NULL, DS_DOMAIN_DIRECT_INBOUND | DS_DOMAIN_DIRECT_OUTBOUND | DS_DOMAIN_IN_FOREST | DS_DOMAIN_PRIMARY | DS_DOMAIN_TREE_ROOT, &pDomainTrust, &ulCount);
	for(int i = 0; i < ulCount; i++)
	{
		_tprintf(_T("====== Domain: NB[%s], DNS[%s], flags[0x%X], ParentIdx[%u] ======\n"), 
			pDomainTrust[i].NetbiosDomainName, 
			pDomainTrust[i].DnsDomainName, 
			pDomainTrust[i].Flags,
			pDomainTrust[i].ParentIndex);


		DWORD dw = DsBind(NULL, pDomainTrust[i].DnsDomainName, &hDS);

		DS_NAME_RESULT	*pSites = NULL;
		DS_NAME_RESULT	*pDomains = NULL;
		DS_NAME_RESULT	*pServers = NULL;
		dw = DsListSites(hDS, &pSites);
		_tprintf(_T("Listing sites:\n"));
		for(int i = 0; i < pSites->cItems; i++)
		{
			_tprintf(_T("\tSite: name[%s], domain[%s] status[%u]\n"), pSites->rItems[i].pName,pSites->rItems[i].pDomain, pSites->rItems[i].status);

			_tprintf(_T("\t\t===========================================================================\n"));
			_tprintf(_T("\t\tListing domains for site:\n"));
			dw = DsListDomainsInSite(hDS, pSites->rItems[0].pName, &pDomains);
			for(int i = 0; i < pDomains->cItems; i++)
			{
				_tprintf(_T("\t\t\tDomain: [%s], name [%s], status [%u]\n"), pDomains->rItems[i].pDomain, pDomains->rItems[i].pName, pDomains->rItems[i].status);
			}
			_tprintf(_T("\t\t===========================================================================\n"));
			_tprintf(_T("\t\tListing servers for site:\n"));
			dw = DsListServersInSite(hDS, pSites->rItems[0].pName, &pServers);
			for(int i = 0; i < pServers->cItems; i++)
			{
				_tprintf(_T("\t\t\tServer: [%s], name [%s], status [%u]\n"), pServers->rItems[i].pDomain, pServers->rItems[i].pName, pServers->rItems[i].status);
			}
			_tprintf(_T("\t\t===========================================================================\n"));
		}
		DsUnBind(&hDS);


	}




	return 0;
}

