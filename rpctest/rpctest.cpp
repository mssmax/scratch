// rpctest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rpctest.h"


#ifdef THE_SERVER
#include "rpctest_s.c"
#else
#include "rpctest_c.c"
#endif


class CTest : public IPropertyBag
{
	DWORD dwCount;
public:

	CTest() : dwCount(0) {}

	STDMETHOD_(ULONG, AddRef)()
	{
		printf("AddRef called %d\n", dwCount++);
		return 1;
	}

	STDMETHOD_(ULONG, Release)()
	{
		printf("Release called %d\n", dwCount--);
		return 1;
	}

	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppv)
	{
		if(riid == IID_IUnknown)
		{
			*ppv = (IUnknown*)this;
			AddRef();
			return S_OK;
		}
		else if(riid == IID_IPropertyBag)
		{
			*ppv = (IPropertyBag*)this;
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHOD(Read)(LPCOLESTR lpszName, VARIANT *v, IErrorLog *p)
	{
		printf("Read called\n");
		return S_OK;
	}
	STDMETHOD(Write)(LPCOLESTR lpszName, VARIANT *v)
	{
		printf("Write called %S\n", lpszName);
		return S_OK;
	}
};


#ifndef THE_SERVER

RPC_BINDING_HANDLE g_hBinding;

DWORD WINAPI MyThread(LPVOID hBinding)
{
	int i = 0;
	UINT status = 0;
	while(1)
	{
		RpcTryExcept 
		{
//			char buf[1024];
//			sprintf(buf, "simple string %d %d %d", GetCurrentThreadId(), GetCurrentProcessId(), i++);

			CTest test;

			Test(g_hBinding, &test);
			printf("------------------------\n");
		}
		RpcExcept(1)
		{
			status = RpcBindingReset(g_hBinding);
			status = RpcExceptionCode();
			printf("Error %ld[0x%X]\n", status, status);
		}
		RpcEndExcept

		Sleep(800);
	}

	return 0;
}
#endif


RPC_STATUS CALLBACK MyCallback(RPC_IF_HANDLE h, void *)
{
	printf("Security callback called\n");
	return RPC_S_OK;
}

int main(int argc, char* argv[])
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	RPC_STATUS status;
	unsigned char* protSeq = (unsigned char*)"ncalrpc";
	unsigned char* endPoint = (unsigned char*)argv[1];
	RPC_BINDING_VECTOR  *bindVector = NULL;

	// on Windows 2008 R2 onwards, RPC runtime does not initialize pNdrOleExtensionsTable which is used 
	// for marshalling interface pointers ( e.g. when passing an IStream as a parameter to an RPC interface )
	// which results in a crash in RPCRT4.dll when an RPC call is made because the above table is filled with NULLs
	// by calling NdrOleAllocate before calling any other functions we force RPCRT to initialize the table properly
	
	if(argc > 2)
		LPVOID p = NdrOleAllocate(1);

#ifdef THE_SERVER


/**/

		int i = 1;
		//for(int i = 1; i < argc; i++)
		{
			
			status = RpcServerRegisterIf(rpctest_v1_0_s_ifspec, NULL, NULL);
			if(status)
				printf("Failed to register if %d\n", status);
			
			printf("Registering endpoint %s\n", argv[i]);
			status = RpcServerUseProtseqEp(protSeq, 20, (unsigned char*)argv[i], NULL);
			if(status)
				printf("Failed to use protseqep %d\n", status);
			
				/**
				
				  status = RpcServerUseProtseq(protSeq, 20, NULL);
				  if(status)
				  printf("Failed to use protseqep %d\n", status);
				  
					status = RpcServerRegisterIf2(rpctest_v1_0_s_ifspec, NULL, NULL, 0, RPC_C_LISTEN_MAX_CALLS_DEFAULT, -1, NULL);
					if(status)
					printf("Failed to query binding vector %d\n", status);
			/**/
		}
		
	do
	{
		status = RpcServerInqBindings(&bindVector);
		if(status)
			printf("Failed to inquiry bindings %d\n", status);
		
		status = RpcEpRegister(rpctest_v1_0_s_ifspec, bindVector, NULL, NULL);
		if(status)
			printf("Failed to register endpoint %d\n", status);
		
		/**
		status = RpcServerRegisterAuthInfoA(NULL, RPC_C_AUTHN_DEFAULT, NULL, NULL);
		if(status)
		printf("Failed to register auth info %d\n", status);
		
		  
		/**/
		status = RpcServerListen(1, RPC_C_LISTEN_MAX_CALLS_DEFAULT, TRUE);
		if(status)
			printf("Failed to start listening %d\n", status);
		
		printf("Enter EP to unregister\n");
		unsigned char szEP[1024];
		scanf("%s", szEP);
		if(szEP[0] == 'q')
			break;

		if(szEP[0] == 'r')
		{
			status = RpcServerUseProtseqEp(protSeq, 20, szEP+1, NULL);
			if(status)
				printf("Failed to use protseqep %d\n", status);
		}
		else
		{

			RPC_IF_ID id = {0};
			RPC_EP_INQ_HANDLE inquiry = NULL;

			status = RpcIfInqId(rpctest_v1_0_s_ifspec, &id);
			status = RpcMgmtEpEltInqBegin(NULL, RPC_C_EP_MATCH_BY_IF, &id, RPC_C_VERS_EXACT, 0, &inquiry);
			if(!status)
			{
				for(;;)
				{
					RPC_IF_ID ifID;
					RPC_BINDING_HANDLE hBinding = NULL;
					status = RpcMgmtEpEltInqNextA(inquiry, &ifID, &hBinding, NULL, NULL);
					if(status == RPC_X_NO_MORE_ENTRIES)
						break;
					
					unsigned char *pszBinding = NULL;
					RpcBindingToStringBinding(hBinding, &pszBinding);
					printf("%s\n", pszBinding);

					unsigned char *pszEP = 0;
					RpcStringBindingParse(pszBinding, NULL, NULL, NULL, &pszEP, NULL);
					if(!stricmp((LPCSTR)szEP+1, (LPCSTR)pszEP))
					{
						RPC_BINDING_VECTOR newVec = { 1, { hBinding }};
						status = RpcEpUnregister(rpctest_v1_0_s_ifspec, &newVec , NULL);
						if(status)
							printf("RpcMgmtEpUnregister returned %d\n", status);
						status = RpcStringFree(&pszBinding);
						status = RpcBindingFree(&hBinding);
						break;
					}

						
					RpcStringFree(&pszBinding);
					RpcBindingFree(&hBinding);
				}
				status = RpcMgmtEpEltInqDone(&inquiry);
			}
		}
	}
	while(1);
	
	status = RpcMgmtStopServerListening(NULL);
/**/
	status = RpcEpUnregister(rpctest_v1_0_s_ifspec, bindVector, NULL);
/**/
	status = RpcServerUnregisterIf(rpctest_v1_0_s_ifspec, NULL, TRUE);
	if(bindVector)
		RpcBindingVectorFree(&bindVector);

#else
/**/

	printf("Connecting to endpoint %s\n", endPoint);

	unsigned char *pszBinding = NULL;
	status = RpcStringBindingComposeA(NULL, protSeq, NULL, endPoint, NULL, &pszBinding);
	if(status)
		printf("Failed to compose %d\n", status);

	status = RpcBindingFromStringBinding(pszBinding, &g_hBinding);
	if(status)
		printf("Failed to create binding %d\n", status);

	status = RpcBindingSetAuthInfoA(g_hBinding, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_AUTHN_DEFAULT, NULL, 0);
	if(status)
		printf("Failed to create binding %d\n", status);


	RpcStringFree(&pszBinding);

	for(int i = 0; i < 1; i++)
	{
		CloseHandle(CreateThread(NULL, 0, MyThread, 0, 0, 0));
	}

	getch();

	status = RpcBindingFree(&g_hBinding);
/**/

/**
	RPC_EP_INQ_HANDLE inquiry = NULL;
	RPC_IF_ID id = {0};

	status = RpcIfInqId(rpctest_v1_0_c_ifspec, &id);

	status = RpcMgmtEpEltInqBegin(NULL, RPC_C_EP_MATCH_BY_IF, &id, RPC_C_VERS_EXACT, 0, &inquiry);
	if(!status)
	{
		for(;;)
		{
			RPC_IF_ID ifID;
			RPC_BINDING_HANDLE hBinding = NULL;
			status = RpcMgmtEpEltInqNextA(inquiry, &ifID, &hBinding, NULL, NULL);
			if(status == RPC_X_NO_MORE_ENTRIES)
				break;

			unsigned char *pszBinding = NULL;
			RpcBindingToStringBindingA(hBinding, &pszBinding);
			printf("%s\n", pszBinding);
			RpcTryExcept
			{
				Test(hBinding, pszBinding);
			}
			RpcExcept(1)
			{
				printf("Exception invoking binding 0x%X\n", RpcExceptionCode());
			}
			RpcEndExcept

			RpcStringFree(&pszBinding);
			RpcBindingFree(&hBinding);
		}
		status = RpcMgmtEpEltInqDone(&inquiry);
	}

/**/
#endif

	CoUninitialize();

	return 0;
}


#ifdef THE_SERVER

/*
void Test(handle_t handle, unsigned char * pszString)
{
//	printf("called server [%s], %x, %x\n", pszString, GetCurrentThreadId(), GetCurrentProcessId());
}
*/

void Test(handle_t handle, IUnknown *p)
{
	char buf[1024];

	printf("Server side called\n");

	IPropertyBag *pBag = 0;
	HRESULT hr = p->QueryInterface(IID_IPropertyBag, (LPVOID*)&pBag);
	printf("QueryInterface called 0x%X\n", hr);
	VARIANT v = {0};
	hr = pBag->Write(L"sdsfsdF", &v);
	printf("Write called 0x%X\n", hr);
	pBag->Release();

}

#endif

/*********************************************************************/
/*                MIDL allocate and free                             */
/*********************************************************************/

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t len)
{
    return(malloc(len));
}

void __RPC_USER MIDL_user_free(void __RPC_FAR * ptr)
{
    free(ptr);
}
