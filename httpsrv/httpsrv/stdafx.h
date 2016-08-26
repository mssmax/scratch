#pragma once
#define _WIN32_WINNT 0x0500
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <WinCrypt.h>
#include <ObjBase.h>
#include <http.h>
using namespace System;
using namespace System::Net;
using namespace System::Text;
using namespace System::IO;
using namespace System::Security::Cryptography;
using namespace System::Runtime::InteropServices;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Xml;

#include "HttpHandlerIF.h"
