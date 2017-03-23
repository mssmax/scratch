#include "stdafx.h"
#include <atlbase.h>

int main(int argc, char *argv[])
{
	USES_CONVERSION;
	if (argc < 2)
	{
		printf("Usage: wget <remote file> [<local file>]\n");
		return 0;
	}

	WCHAR wszLocalPath[256] = { 0 };
	WCHAR *pwszLocalPath = wszLocalPath;
	if (argc == 2)
	{
		URL_COMPONENTS url = { 0 };
		url.dwStructSize = sizeof(url);
		url.lpszUrlPath = wszLocalPath;
		url.dwUrlPathLength = _countof(wszLocalPath);
		BOOL b = WinHttpCrackUrl(A2W(argv[1]), 0, 0, &url);
		WCHAR *p = wcsrchr(url.lpszUrlPath, '/');
		if (p)
		{
			pwszLocalPath = p + 1;
		}
	}
	else
	{
		StringCbCopy(wszLocalPath, sizeof(wszLocalPath), A2W(argv[2]));
	}

	CoInitialize(0);

	try
	{
		MSXML::IServerXMLHTTPRequestPtr spReq(__uuidof(MSXML::ServerXMLHTTP60));
		spReq->open("GET", argv[1]);

		printf("Downloading...\n");

		spReq->send();

		printf("Status %lu\n", spReq->status);
//		if (spReq->status == 200)
		{
			printf("Creating destination stream...\n");
			
			IStreamPtr spDstStrm;
			HRESULT hr = SHCreateStreamOnFile(pwszLocalPath, STGM_READWRITE | STGM_CREATE, &spDstStrm);
			if (FAILED(hr))
			{
				printf("Failed to open local file, err 0x%X", hr);
				return 0;
			}
			
			printf("Obtaining source stream...\n");
			
			IStreamPtr spSrcStrm = spReq->responseStream;
			ULARGE_INTEGER cb;
			cb.QuadPart = MAXULONGLONG;
			hr = spSrcStrm->CopyTo(spDstStrm, cb, 0, 0);
			if (FAILED(hr))
			{
				printf("Failed to copy source stream to the destination 0x%X\n", hr);
				return 0;
			}
		}
		printf("Downloading...ok\n");
	}
	catch (_com_error &e)
	{
		printf("COM error  0x%X, %s\n", e.Error(), (LPCSTR)e.Description());
	}

	CoUninitialize();
    return 0;
}

