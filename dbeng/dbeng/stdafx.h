#pragma once

#include "targetver.h"
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <comdef.h>
#include <string.h>
#include <esent.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <xmllite.h>
#pragma comment(lib, "xmllite.lib")
#include <string>
#include <vector>
#include "DBSchema.h"

std::wstring ConvA2W(LPCSTR lpsz, UINT cp = CP_UTF8);

std::string ConvW2A(LPCWSTR lpsz, UINT cp = CP_UTF8);
void DeleteAllFiles(LPCSTR lpszPath);