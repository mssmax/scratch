#include "stdafx.h"
#include <conio.h>
#include <vector>
#include <CBase64.h>

std::wstring CleanPasswords(std::wstring &s)
{
	std::wstring res(s);
	CharLowerW(&res[0]);
	size_t idx = 0;
	while ((idx = res.find(L"password=\"")) != std::wstring::npos)
	{
		size_t idx2 = res.find('\"', idx + wcslen(L"password=\""));
		res.replace(idx, idx2 - idx + 1, L"password=***");
	}
	return res;
}

/**
void main()
{
	TCHAR szWorkFolder[_MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szWorkFolder, _MAX_PATH);
	LPTSTR szSep = _tcsrchr(szWorkFolder, '\\');
	if (szSep)
		*szSep = 0;

	TCHAR szFileToLaunch[_MAX_PATH] = _T("http://go.gfi.com/?pageid=FMhelp#cshid=qsg");

	SHELLEXECUTEINFO info = { 0 };
	info.cbSize = sizeof(info);
	info.lpFile = szFileToLaunch;
	info.lpDirectory = szWorkFolder;
	info.nShow = SW_SHOWNORMAL;
	ShellExecuteEx(&info);


/**
	std::wstring cmdline = L"whatever.exe /PROXYPASSWORD=\"mypassword\", /UPGRADEPROXYPASSWORD=\"myotherpassword\"";
	wprintf(L"Before: %s\n", cmdline.c_str());
	cmdline = CleanPasswords(cmdline);
	wprintf(L"After: %s\n", cmdline.c_str());
}
/**/

/*
void main()
{
	HANDLE hToken = 0;
	BOOL b = OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken);
	BYTE buf[1024] = { 0 };
	PTOKEN_USER usr = reinterpret_cast<PTOKEN_USER>(buf);
	DWORD dwLen = sizeof(buf);
	b = GetTokenInformation(hToken, TokenUser, buf, sizeof(buf), &dwLen);


	BYTE sid[256] = { 0 };
	PSID mySid = reinterpret_cast<PSID>(sid);
	WCHAR refDom[256] = { 0 };
	DWORD dwMySid = sizeof(sid);
	DWORD dwRefDom = sizeof(refDom);
	SID_NAME_USE use;
	//b = LookupAccountName(NULL, L"max", mySid, &dwMySid, refDom, &dwRefDom, &use);
	//DWORD len = GetLengthSid(mySid);
	LPWSTR strSid;
	PSID baSid;
	b = ConvertStringSidToSid(L"LS", &baSid);

	ConvertSidToStringSid(baSid, &strSid);
	printf("%S\n", strSid);
}
*/

/**/
int main(int argc, char* argv[])
{
	std::string sPwd("my password");
	WCHAR szIn[512] = L"something is encrypted";
	WCHAR szOut[512] = { 0 };
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;
	HCRYPTKEY hCryptKey = 0;
	DWORD dwDataSize = 0;
	
	BYTE cbKey[1024] = { 0 };
	DWORD keyLen = sizeof(cbKey);

	BOOL b = 0;

	if (CryptAcquireContext(&hProv, 0, 0, PROV_RSA_AES, 0))
	{	
/**
		if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
		{			
			if (CryptHashData(hHash, reinterpret_cast<const BYTE*>(sPwd.c_str()), sPwd.length(), 0))
			{
				if (CryptDeriveKey(hProv, CALG_AES_256, hHash,	CRYPT_EXPORTABLE, &hCryptKey))
				{
					dwDataSize = (wcslen(szIn) + 1) * sizeof(WCHAR);
					if (CryptEncrypt(
						hCryptKey, 
						0, 
						TRUE, 
						0, 
						reinterpret_cast<BYTE*>(szIn), 
						&dwDataSize, 
						sizeof(szIn)))
					{
						char b64[512] = { 0 };
						CBase64 b;
						DWORD dwBSize = sizeof(b64);
						b.Encode(reinterpret_cast<BYTE*>(szIn), dwDataSize, b64, &dwBSize);
						printf("%s\n", b64);
					}
					b = CryptExportKey(hCryptKey, 0, OPAQUEKEYBLOB, 0, cbKey, &keyLen);
					HKEY hKey = 0;
					LONG l = RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Testing"), 0, 0, 0, KEY_ALL_ACCESS, 0, &hKey, 0);
					RegSetValueEx(hKey, _T("code"), 0, REG_BINARY, cbKey, keyLen);
					RegSetValueEx(hKey, _T("data"), 0, REG_BINARY, reinterpret_cast<BYTE*>(szIn), dwDataSize);
					RegCloseKey(hKey);
					CryptDestroyKey(hCryptKey);
				}
			}
			CryptDestroyHash(hHash);
		}
/**/
		HKEY hKey = 0;
		LONG l = RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Testing"), 0, KEY_ALL_ACCESS, &hKey);
		RegQueryValueEx(hKey, _T("code"), 0, 0, cbKey, &keyLen);
		dwDataSize = sizeof(szOut);
		RegQueryValueEx(hKey, _T("data"), 0, 0, reinterpret_cast<BYTE*>(szOut), &dwDataSize);
		RegCloseKey(hKey);

		hCryptKey = 0;
		b = CryptImportKey(hProv, cbKey, keyLen, 0, 0, &hCryptKey);
		if (CryptDecrypt(hCryptKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(szOut), &dwDataSize))
		{
			printf("%S\n", szOut);
		}

/**
		if (CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash))
		{
			if (CryptHashData(hHash, reinterpret_cast<const BYTE*>(sPwd.c_str()), sPwd.length(), 0))
			{
				if (CryptDeriveKey(hProv, CALG_AES_256, hHash, 0, &hCryptKey))
				{
					if (CryptDecrypt(hCryptKey, 0, TRUE, 0, reinterpret_cast<BYTE*>(szIn), &dwDataSize))
					{
						printf("%S\n", szIn);
					}
				}
			}
		}
/**/
		CryptReleaseContext(hProv, 0);
	}
	return 0;
}
/**/