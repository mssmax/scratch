#include "stdafx.h"
#include "POP3Proxy.h"



CPOP3Proxy::CPOP3Proxy()
{
	m_hServerThread = NULL;
	m_bShutdown = FALSE;
	m_ProxySocket = 0;
	m_nProxyPort = 110;
	m_nServerPort = 0;
}

CPOP3Proxy::~CPOP3Proxy()
{

}

void CPOP3Proxy::AddDirectory(LPCTSTR lpszDir)
{
	string sTemp(lpszDir);
	sTemp += "\\";
	m_Directories.push_back(sTemp);
}

int CPOP3Proxy::Start(int nProxyPort, const char* szServerName, int nServerPort)
{
	unsigned uThrdId;
	m_sServerAddress = szServerName;
	m_nServerPort = nServerPort;
	m_nProxyPort = nProxyPort;
	m_bShutdown = FALSE;
	m_hServerThread = (HANDLE)_beginthreadex(NULL, 0, MainServerThread, this, 0, &uThrdId);
	if(m_hServerThread == 0)
		return PROXY_E_CANNOT_START_THREAD;
	else 
		return PROXY_S_OK;
}


void CPOP3Proxy::Stop()
{
	m_bShutdown = TRUE;
	closesocket(m_ProxySocket);
	WaitForSingleObject(m_hServerThread, INFINITE);
}

unsigned CPOP3Proxy::MainServerThread(void *pvParam)
{
	CPOP3Proxy* pThis = (CPOP3Proxy*)pvParam;

	WSADATA wsaData = {0};
	if(WSAStartup(0x101, &wsaData) != 0)
		_endthreadex(-1);
	
	sockaddr_in saddr;
	pThis->m_ProxySocket = socket(AF_INET, SOCK_STREAM, 0);
	if(pThis->m_ProxySocket == INVALID_SOCKET)
		_endthreadex(-2);
	
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(pThis->m_nProxyPort);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if(bind(pThis->m_ProxySocket, (sockaddr*)&saddr, sizeof(saddr)) != 0)
		_endthreadex(-3);

	if(listen(pThis->m_ProxySocket, 5) != 0)
		_endthreadex(-4);

	while(!pThis->m_bShutdown)
	{
		sockaddr_in caddr;
		int iSize = sizeof(caddr);
		SOCKET clientSocket = accept(pThis->m_ProxySocket, (sockaddr*)&caddr, &iSize);
		if(pThis->m_bShutdown)
			break;
		printf("Accepted connection on %s\n", inet_ntoa(caddr.sin_addr));
		pThis->StartClientThread(clientSocket);
	}

	WSACleanup();
	_endthreadex(0);
	return 0;
}

void CPOP3Proxy::StartClientThread(SOCKET clientSock)
{
	CLIENT_PARAM *pParam = new CLIENT_PARAM;
	pParam->clientSocket = clientSock;
	pParam->pProxy = this;
	_beginthread(ClientThread, 0, pParam);
}

void CPOP3Proxy::ClientThread(void* pvParam)
{
	CLIENT_PARAM *pParam = (CLIENT_PARAM*)pvParam;
	CPOP3Proxy *pThis = pParam->pProxy;
	SOCKET clientSocket = pParam->clientSocket;
	delete pParam;

	char szMainBuffer[2048];
	char szBuffer[512];
	char szUserName[128] = "";
	char szPassword[128] = "";
	char szServerName[128] = "";
	const char *pcszCmd = "USER";
	int nBytes = 0;
	int nTargetServerPort = 110;

	pThis->m_DeferredDeleteList.clear();

	lstrcpy(szBuffer, "+OK POP3Proxy ready\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	// now we should try to receive username and servername from the client
	BOOL bAuthorized = FALSE;
	int iStage = 1; // 1: username needed, 2: password needed
	while(!bAuthorized)
	{
		szMainBuffer[0] = 0;
		while((nBytes = recv(clientSocket, szBuffer, 512, 0)) > 0)
		{
			szBuffer[nBytes] = 0;
			lstrcat(szMainBuffer, szBuffer);
			char *pTemp = strstr(szMainBuffer, "\r\n");
			if(pTemp)
			{
				*pTemp = 0;
				break;
			}
		}

		if(nBytes == SOCKET_ERROR)
		{
			closesocket(clientSocket);
			_endthread();
		}

		if(_strnicmp(szMainBuffer, "QUIT", 4) == 0)
		{
			lstrcpy(szBuffer, "+OK POP3Proxy wishes you all the best! Bye\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			closesocket(clientSocket);
			_endthread();
		}

		if(_strnicmp(szMainBuffer, pcszCmd, 4) != 0)
		{
			printf("ERR Unknown command during authorization stage: [%s]\n", szMainBuffer);
			lstrcpy(szBuffer, "-ERR Unknown command during authorization stage\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			continue;
		}
		if(iStage == 1)
		{
			// the first command is USER so we must extract username and servername from parameters
			if(lstrlen(szMainBuffer) < 5)
			{
				printf(szBuffer, "ERR Missing name argument: [%s]\n", szMainBuffer);
				lstrcpy(szBuffer, "-ERR Missing name argument\r\n");
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				continue;
			}
			lstrcpyn(szUserName, szMainBuffer + 5, 128);
/*
			char *pStr = szMainBuffer + 5;
			char *pSlash = strchr(pStr, '/');
			if(!pSlash)
			{
				lstrcpy(szBuffer, "-ERR username must be in format user/server[:port]\r\n");
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				continue;
			}
			*pSlash++ = 0;

			lstrcpyn(szUserName, pStr, 128);

			char *pColon = strchr(pSlash, ':');
			if(pColon)
			{
				*pColon = 0;
				nTargetServerPort = atol(pColon + 1);
			}
			lstrcpyn(szServerName, pSlash, 128);
*/
			pcszCmd = "PASS";
			iStage = 2;
			lstrcpy(szBuffer, "+OK Send PASS command\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		}
		else if(iStage == 2) // password
		{
			if(lstrlen(szMainBuffer) > 4)
				lstrcpyn(szPassword, szMainBuffer + 5, 128);
			else
				lstrcpy(szPassword, "");
			// here we will not send +OK in order to first try to authorize on the real server
			// and if it is ok then send +OK
			bAuthorized = TRUE;
		}
		else
		{
			lstrcpy(szBuffer, "-ERR Internal proxy error ( wrong stage )\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			closesocket(clientSocket);
			_endthread();
		}
	}
	printf("Username: '%s'\n", szUserName);
	printf("Password: '%s'\n", szPassword);
	printf("Server - %s:%d\n", szServerName, nTargetServerPort); 

	pThis->DoJob(clientSocket, szUserName, szPassword);
	pThis->DoDeferredDelete();
	
	shutdown(clientSocket, SD_BOTH);
	closesocket(clientSocket);

	_endthread();
}

void CPOP3Proxy::DoJob(SOCKET clientSocket, const char* szUser, const char* szPass)
{
	// connect to server
	// see if we are authorized
	// and then send either good or bad answer to the client
	// BUT for now we will emulate a pop3 server
	char szBuffer[512] = "";
	char szMainBuffer[2048] = "";
	int nBytes;
	
	lstrcpy(szBuffer, "+OK You are authorized\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

	for(;;)
	{
		szMainBuffer[0] = 0;
		while((nBytes = recv(clientSocket, szBuffer, 512, 0)) > 0)
		{
			szBuffer[nBytes] = 0;
			lstrcat(szMainBuffer, szBuffer);
			char *pTemp = strstr(szMainBuffer, "\r\n");
			if(pTemp)
			{
				*pTemp = 0;
				break;
			}
		}
		if(nBytes == SOCKET_ERROR)
			return;

		char *szArgBuf;// = szMainBuffer;
		if(lstrlen(szMainBuffer) >= 4)
			szArgBuf = szMainBuffer + 4;
		else
			szArgBuf = "";
		if(!_strnicmp(szMainBuffer, "QUIT", 4))
		{
			lstrcpy(szBuffer, "+OK Bye\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			printf("Client closed connection\n");
			return;
		}
		else if(!_strnicmp(szMainBuffer, "STAT", 4))
		{
			printf("STAT\n");
			OnSTATCmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "LIST", 4))
		{
			printf("LIST '%s'\n", szArgBuf);
			OnLISTCmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "RETR", 4))
		{
			printf("RETR '%s'\n", szArgBuf);
			OnRETRCmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "DELE", 4))
		{
			printf("DELE '%s'\n", szArgBuf);
			OnDELECmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "NOOP", 4))
		{
			printf("NOOP\n");
			OnNOOPCmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "RSET", 4))
		{
			printf("RSET\n");
			OnRSETCmd(clientSocket, szArgBuf);
		}
		else if(!_strnicmp(szMainBuffer, "TOP", 3))
		{
			printf("TOP '%s'\n", szArgBuf);
			OnTOPCmd(clientSocket, szArgBuf);
		}
		else
		{
			printf("Unknown command - %s\n", szMainBuffer);
			lstrcpy(szBuffer, "-ERR Unknown or unsupported command\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		}
	}
}

int CPOP3Proxy::OnLISTCmd(SOCKET clientSocket, char* szArgBuf)
{
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH] = "";
	char szBuffer[512] = "";
	BOOL bOKSent = FALSE;

	if(lstrlen(szArgBuf) < 1) // global list
	{
		list<string>::iterator it = m_Directories.begin();
		long lMsgNum = 1;
		for(; it != m_Directories.end(); it++)
		{
			lstrcpy(szPath, (*it).c_str());
			lstrcat(szPath, "\\*.*");
			HANDLE hFind = FindFirstFile(szPath, &fd);
			if(hFind == INVALID_HANDLE_VALUE)
			{
				lstrcpy(szBuffer, "-ERR Cannot list messages\r\n");
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				return 0;
			}
			if(!bOKSent)
			{
				lstrcpy(szBuffer, "+OK\r\n");
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				bOKSent = TRUE;
			}
			do
			{
				if(fd.cFileName[0] != '.')
				{
					_snprintf(szBuffer, 128, "%d %d\r\n", lMsgNum++, fd.nFileSizeLow);
					send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				}
			} while(FindNextFile(hFind, &fd));

			FindClose(hFind);
		}
		_snprintf(szBuffer, 512, ".\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	}
	else
	{
		long lCurrentMsg = 0;
		long lMsgNum = atol(szArgBuf);
		HANDLE hFind;
		list<string>::iterator it = m_Directories.begin();
		for(; it != m_Directories.end(); it++)
		{

			lstrcpy(szPath, (*it).c_str());
			lstrcat(szPath, "\\*.*");
			hFind = FindFirstFile(szPath, &fd);
			if(hFind == INVALID_HANDLE_VALUE)
			{
				lstrcpy(szBuffer, "-ERR Cannot list messages\r\n");
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
				return 0;
			}
			lstrcpy(szBuffer, "+OK\r\n");
			do
			{
				if(fd.cFileName[0] != '.')
					lCurrentMsg++;
				if(lCurrentMsg == lMsgNum)
					goto sendit;
			} while(FindNextFile(hFind, &fd));
		}
sendit:
		if(lCurrentMsg == lMsgNum)
			_snprintf(szBuffer, 512, "+OK %ld %ld\r\n", lCurrentMsg, fd.nFileSizeLow);
		else
			_snprintf(szBuffer, 512, "-ERR No such message\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

		FindClose(hFind);
	}
	return 1; // success
}

int CPOP3Proxy::OnSTATCmd(SOCKET clientSocket, char* szArgBuf)
{
	DWORD dwTotalSize = 0;
	DWORD dwTotalCount = 0;
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH] = "";
	char szBuffer[512] = "";
	list<string>::iterator it(m_Directories.begin());

	for(; it != m_Directories.end(); it++)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, "\\*.*");
		HANDLE hFind = FindFirstFile(szPath, &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			lstrcpy(szBuffer, "-ERR Cannot list messages\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			return 0;
		}

		do
		{
			if(fd.cFileName[0] != '.')
			{
				dwTotalCount++;
				dwTotalSize += fd.nFileSizeLow;
			}
		} while(FindNextFile(hFind, &fd));

		FindClose(hFind);

	}

	_snprintf(szBuffer, 512, "+OK %d %d\r\n", dwTotalCount, dwTotalSize);
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

	return 1; // success
}

int CPOP3Proxy::OnRETRCmd(SOCKET clientSocket, char* szArgBuf)
{
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH] = "";
	char szBuffer[512] = "";

	if(lstrlen(szArgBuf) < 1)
	{
		lstrcpy(szBuffer, "-ERR Missing argument\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return 0;
	}

	long lCurrentMsg = 0;
	long lMsgNum = atol(szArgBuf);
	list<string>::iterator it(m_Directories.begin());
	for(; it != m_Directories.end(); it++)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, "\\*.*");
		HANDLE hFind = FindFirstFile(szPath, &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			lstrcpy(szBuffer, "-ERR Cannot retr messages\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			return 0;
		}

		do
		{
			if(fd.cFileName[0] != '.')
				lCurrentMsg++;
			if(lCurrentMsg == lMsgNum)
			{
				FindClose(hFind);
				goto sendit;
			}
		} while(FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
sendit:
	if(lCurrentMsg == lMsgNum)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, fd.cFileName);
		SendFile(szPath, clientSocket);
	}
	else
	{
		lstrcpy(szBuffer, "-ERR Message not found\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	}

	return 1; // success
}

int CPOP3Proxy::OnDELECmd(SOCKET clientSocket, char* szArgBuf)
{
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH] = "";
	char szBuffer[512] = "";

	if(lstrlen(szArgBuf) < 1)
	{
		lstrcpy(szBuffer, "-ERR Missing argument\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return 0;
	}

	long lMsgNum = atol(szArgBuf);
	list<string>::iterator it(m_Directories.begin());

	long lCurrentMsg = 0;
	for(; it != m_Directories.end(); it++)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, "\\*.*");
		HANDLE hFind = FindFirstFile(szPath, &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			lstrcpy(szBuffer, "-ERR Cannot list messages\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			return 0;
		}

		do
		{
			if(fd.cFileName[0] != '.')
				lCurrentMsg++;
			if(lCurrentMsg == lMsgNum)
			{
				FindClose(hFind);
				goto delit;
			}
		} while(FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
delit:
	if(lCurrentMsg == lMsgNum)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, fd.cFileName);
		m_DeferredDeleteList.push_back(szPath);
		_snprintf(szBuffer, 512, "+OK Message %ld deleted\r\n", lCurrentMsg);
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0); 
	}
	else
	{
		lstrcpy(szBuffer, "-ERR Message not found\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	}
	return 1; // success
}

int CPOP3Proxy::OnNOOPCmd(SOCKET clientSocket, char* szArgBuf)
{
	const char *szBuffer = "+OK\r\n";
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	return 1; // success
}

int CPOP3Proxy::OnRSETCmd(SOCKET clientSocket, char* szArgBuf)
{
	const char* szBuffer = "+OK Messages unmarked\r\n";
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	m_DeferredDeleteList.clear();
	return 1; // success
}

int CPOP3Proxy::OnTOPCmd(SOCKET clientSocket, char *szArgBuf)
{
	WIN32_FIND_DATA fd;
	char szPath[MAX_PATH] = "";
	char szBuffer[512] = "";

	if(lstrlen(szArgBuf) < 1)
	{
		lstrcpy(szBuffer, "-ERR Missing argument\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return 0;
	}

	long lCurrentMsg = 0;
	long lMsgNum = -1;
	long lNumLines = -1;
	//= atol(szArgBuf);
	if(sscanf(szArgBuf, "%d %d", &lMsgNum, &lNumLines) < 2)
	{
		lstrcpy(szBuffer, "-ERR Missing argument ");
		lstrcat(szBuffer, szArgBuf);
		lstrcat(szBuffer, "\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return 0;
	}

	list<string>::iterator it(m_Directories.begin());
	for(; it != m_Directories.end(); it++)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, "\\*.*");
		HANDLE hFind = FindFirstFile(szPath, &fd);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			lstrcpy(szBuffer, "-ERR Cannot retr messages\r\n");
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
			return 0;
		}

		do
		{
			if(fd.cFileName[0] != '.')
				lCurrentMsg++;
			if(lCurrentMsg == lMsgNum)
			{
				FindClose(hFind);
				goto sendit;
			}
		} while(FindNextFile(hFind, &fd));

		FindClose(hFind);
	}
sendit:
	if(lCurrentMsg == lMsgNum)
	{
		lstrcpy(szPath, (*it).c_str());
		lstrcat(szPath, fd.cFileName);

		SendFileLines(szPath, clientSocket, lNumLines);
	}
	else
	{
		lstrcpy(szBuffer, "-ERR Message not found\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	}

	return 1; // success
}


void CPOP3Proxy::SendFile(const char* szPath, SOCKET clientSocket)
{
	char szBuffer[512];
	char szTempBuffer[512];
	FILE* file = fopen(szPath, "rb");
	if(file == NULL)
	{
		lstrcpy(szBuffer, "-ERR Error during send\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return;
	}
	lstrcpy(szBuffer, "+OK\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	while(fgets(szBuffer, 500, file))
	{
		int pos = lstrlen(szBuffer);
		if(szBuffer[pos - 1] != 0x0a && szBuffer[pos - 2] != 0x0d)
			lstrcat(szBuffer, "\r\n");
		if(szBuffer[0] == '.')
		{
			lstrcpy(szTempBuffer, ".");
			lstrcat(szTempBuffer, szBuffer);
			send(clientSocket, szTempBuffer, lstrlen(szTempBuffer), 0);
		}
		else
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	}
	lstrcpy(szBuffer, ".\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

	fclose(file);
}

void CPOP3Proxy::SendFileLines(const char* szPath, SOCKET clientSocket, long lNumLines)
{
	char szBuffer[512];
	char szTempBuffer[512];
	FILE* file = fopen(szPath, "rb");
	if(file == NULL)
	{
		lstrcpy(szBuffer, "-ERR Error during send\r\n");
		send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
		return;
	}
	lstrcpy(szBuffer, "+OK\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);
	long lNumLinesSent = 0;

	// first send the headers
	while(fgets(szBuffer, 500, file))
	{
		int pos = lstrlen(szBuffer);
		if(szBuffer[pos - 1] != 0x0a && szBuffer[pos - 2] != 0x0d)
			lstrcat(szBuffer, "\r\n");
		if(szBuffer[0] == '.')
		{
			lstrcpy(szTempBuffer, ".");
			lstrcat(szTempBuffer, szBuffer);
			send(clientSocket, szTempBuffer, lstrlen(szTempBuffer), 0);
		}
		else
			send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

		// reached the end of headers
		if(lstrlen(szBuffer) == 2 && szBuffer[0] == 0x0D && szBuffer[1] == 0x0A)
			break;
	}

	for(int i = 0; i < lNumLines; i++)
	{
		if(fgets(szBuffer, 500, file))
		{
			if(lNumLinesSent++ >= lNumLines)
				break;

			int pos = lstrlen(szBuffer);
			if(szBuffer[pos - 1] != 0x0a && szBuffer[pos - 2] != 0x0d)
				lstrcat(szBuffer, "\r\n");
			if(szBuffer[0] == '.')
			{
				lstrcpy(szTempBuffer, ".");
				lstrcat(szTempBuffer, szBuffer);
				send(clientSocket, szTempBuffer, lstrlen(szTempBuffer), 0);
			}
			else
				send(clientSocket, szBuffer, lstrlen(szBuffer), 0);


		}
		else
			break;
	}
	lstrcpy(szBuffer, ".\r\n");
	send(clientSocket, szBuffer, lstrlen(szBuffer), 0);

	fclose(file);
}


void CPOP3Proxy::DoDeferredDelete()
{
	list<string>::iterator it(m_DeferredDeleteList.begin());
	for(; it != m_DeferredDeleteList.end(); it++)
		DeleteFile((*it).c_str());
}


void CPOP3Proxy::GetResponse(SOCKET sock, char *szBuffer, int nSize)
{
	char szTempBuffer[512];
	int nBytes;
	int nTotalBytes = 0;
	szBuffer[0] = 0;
	while((nBytes = recv(sock, szTempBuffer, 512, 0)) > 0)
	{
		szTempBuffer[nBytes] = 0;
		nTotalBytes += nBytes;
		if(nTotalBytes > nSize)
		{
			szBuffer[0] = 0;
			break;
		}
		lstrcat(szBuffer, szTempBuffer);
		if(strstr(szBuffer, "\r\n"))
			break;
	}
	if(nBytes == SOCKET_ERROR)
		szBuffer[0] = 0;
}