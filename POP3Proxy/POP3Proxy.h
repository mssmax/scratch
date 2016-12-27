#pragma once

#define PROXY_S_OK 0
#define PROXY_E_CANNOT_START_THREAD -1

#pragma warning(disable : 4786)
#include <list>
#include <string>
using namespace std;


class CPOP3Proxy  
{
public:
	CPOP3Proxy();
	virtual ~CPOP3Proxy();

	int Start(int nProxyPort, const char* szServer, int nServerPort);
	void Stop();

	void AddDirectory(LPCTSTR lpszDir);

protected:
	
	static unsigned __stdcall MainServerThread(void *);
	static void __cdecl ClientThread(void*);

	void StartClientThread(SOCKET clientSock);
	void DoJob(SOCKET clientSocket, const char* szUser, const char* szPass);
	void SendFile(const char* szPath, SOCKET clientSock);
	void SendFileLines(const char* szPath, SOCKET clientSock, long lNumLines);
	void DoDeferredDelete();

	void GetResponse(SOCKET sock, char* szBuffer, int nSize);

	virtual int OnSTATCmd(SOCKET, char *szArgBuf);
	virtual int OnLISTCmd(SOCKET, char *szArgBuf);
	virtual int OnRETRCmd(SOCKET, char *szArgBuf);
	virtual int OnDELECmd(SOCKET, char *szArgBuf);
	virtual int OnNOOPCmd(SOCKET, char *szArgBuf);
	virtual int OnRSETCmd(SOCKET, char *szArgBuf);
	virtual int OnTOPCmd(SOCKET, char *szArgBuf);

protected:
	
	HANDLE m_hServerThread;
	BOOL m_bShutdown;
	int m_nServerPort;
	int m_nProxyPort;
	string m_sServerAddress;
	SOCKET m_ProxySocket;
	
	list<string> m_DeferredDeleteList;
	list<string> m_Directories;
};


typedef struct
{
	SOCKET clientSocket;
	CPOP3Proxy *pProxy;
} CLIENT_PARAM;
