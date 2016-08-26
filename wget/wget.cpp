// wget.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: wget <remote file> <local file>\n");
		return 0;
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
			HRESULT hr = SHCreateStreamOnFileA(argv[2], STGM_READWRITE | STGM_CREATE, &spDstStrm);
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

