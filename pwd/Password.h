#pragma once
#include <string>
#pragma comment(lib, "crypt32.lib")

#define CRYPT_STRING_NOCRLF                 0x40000000

class CPassword
{
private:

	std::wstring ConvA2W(LPCSTR lpsz)
	{
		std::wstring ret;
		int size = MultiByteToWideChar(CP_ACP, 0, lpsz, -1, 0, 0);
		if (size)
		{
			ret.resize(size);
			MultiByteToWideChar(CP_ACP, 0, lpsz, -1, &ret[0], size);
			ret.resize(size - 1);
		}
		return ret;
	}

	std::string ConvW2A(LPCWSTR lpsz)
	{
		std::string ret;
		int size = WideCharToMultiByte(CP_ACP, 0, lpsz, -1, 0, 0, 0, 0);
		if (size)
		{
			ret.resize(size);
			WideCharToMultiByte(CP_ACP, 0, lpsz, -1, &ret[0], size, 0, 0);
			ret.resize(size - 1);
		}
		return ret;
	}

	void OutputDebugStringEx(char* format, ...)
	{
		va_list va;
		va_start(va, format);
		char str[1024] = { 0 };
		vsprintf_s(str, sizeof(str) / sizeof(str[0]), format, va);
		OutputDebugStringA(str);
		va_end(va);
	}

public:
	CPassword(DWORD dwReserved = 0)
	{
		UNREFERENCED_PARAMETER(dwReserved);
	}
	~CPassword()
	{}

	// ANSI wrappers for the main functions
	std::string Encrypt(LPCSTR lpszPassword)
	{
		return ConvW2A(Encrypt(ConvA2W(lpszPassword).c_str()).c_str());
	}
	std::string Decrypt(LPCSTR lpszBase64)
	{
		return ConvW2A(Decrypt(ConvA2W(lpszBase64).c_str()).c_str());
	}

	// This function takes a plain text password
	// encrypts it and converts the resulting data into a BASE64 string
	// suitable for saving in a DB or any other persistent storage
	std::wstring Encrypt(LPCWSTR lpszPassword)
	{
		DATA_BLOB blbIn = { 0 };
		DATA_BLOB blbOut = { 0 };
		blbIn.cbData = (lstrlenW(lpszPassword) + 1) * sizeof(WCHAR);
		blbIn.pbData = (BYTE*)lpszPassword;
		BOOL bRet = CryptProtectData(
			&blbIn,
			NULL,
			NULL,
			NULL,
			NULL,
			CRYPTPROTECT_LOCAL_MACHINE | CRYPTPROTECT_UI_FORBIDDEN,
			&blbOut
		);
		if (!bRet)
		{
			OutputDebugStringEx("CryptProtectData failed, error %d", GetLastError());
			return lpszPassword;
		}

		std::wstring sOutput;
		DWORD dwSize = 0;
		bRet = CryptBinaryToStringW(
			blbOut.pbData,
			blbOut.cbData,
			CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
			NULL,
			&dwSize);
		if (bRet)
		{
			sOutput.resize(dwSize);
			CryptBinaryToStringW(
				blbOut.pbData,
				blbOut.cbData,
				CRYPT_STRING_BASE64| CRYPT_STRING_NOCRLF,
				&sOutput[0],
				&dwSize);
		}
		else
		{
			OutputDebugStringEx("CryptBinaryToStringA failed, error %d", GetLastError());
			sOutput = lpszPassword;
		}

		LocalFree(blbOut.pbData);

		if (bRet)
		{
			// this code is there to remove CRLF from the output of CryptBinaryToString
			// since CRYPT_STRING_NOCRLF flag is not supported in W2K3 and XP
			for (std::wstring::iterator itr = sOutput.begin(); itr != sOutput.end();)
			{
				if (*itr == '\x0a' || *itr == '\x0d')
					sOutput.erase(itr);
				else
					itr++;
			}
		}
		return sOutput;
	}

	// this method takes a BASE64 string previsouly provided by the Encrypt method and decodes it into a plain text password
	std::wstring Decrypt(LPCWSTR lpszBase64)
	{
		DATA_BLOB blbEnc = { 0 };
		DATA_BLOB blbDec = { 0 };
		BOOL bRet = CryptStringToBinaryW(lpszBase64, 0, CRYPT_STRING_BASE64, NULL, &blbEnc.cbData, NULL, NULL);
		if (bRet)
		{
			blbEnc.pbData = static_cast<BYTE*>(_malloca(blbEnc.cbData));
			CryptStringToBinaryW(lpszBase64, 0, CRYPT_STRING_BASE64, blbEnc.pbData, &blbEnc.cbData, NULL, NULL);
		}
		else
		{
			OutputDebugStringEx("CryptStringToBinary failed with %d", GetLastError());
			return lpszBase64;
		}

		bRet = CryptUnprotectData(
			&blbEnc,
			NULL,
			NULL,
			NULL,
			NULL,
			CRYPTPROTECT_LOCAL_MACHINE | CRYPTPROTECT_UI_FORBIDDEN,
			&blbDec
		);

		_freea(blbEnc.pbData);

		std::wstring sRet;
		if (bRet)
		{
			sRet = reinterpret_cast<WCHAR*>(blbDec.pbData);
			LocalFree(blbDec.pbData);
		}
		else
		{
			OutputDebugStringEx("CryptUnprotectData failed with %d", GetLastError());
			// just return the original input as it seems not to be a valid encoded string in the first place
			sRet = lpszBase64;
		}

		return sRet;
	}
};