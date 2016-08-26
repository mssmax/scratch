#pragma once
#include <strsafe.h>
#define MONITOR_SLOTNAME _T("\\\\.\\mailslot\\gfifm\\mntrevents")

typedef enum
{
	FMTNONE = 0,
	FMTSERVER = 1,
	FMTMTA = 2,
	FMTATTENDANT = 3
} FMEMODULETYPE;

typedef enum
{
	CATNONE = 0,
	CATINFO = 1,
	CATWARNING = 2,
	CATERROR = 3
} FMECATEGORY;

#pragma pack(push, 1)
class FMEVENT
{
public:
	FMEVENT()
	{
		GetSystemTimeAsFileTime((FILETIME*)&EventTime);
		EventID = 0;
		EventCategory = CATNONE;
		EventModule = FMTNONE;
		ZeroMemory(EventSubModule, sizeof(EventSubModule));
		ZeroMemory(EventAdditionalInfo, sizeof(EventAdditionalInfo));
	}
	// represents the FILETIME structure with the time ( in UTC ) the event was submitted on
	__int64 EventTime;
	// ID of the event which is used to retrieve localizable text to be displayed in the monitor
	DWORD EventID;
	// Event category, such as error, warning or information, barely used right now, but introduced for the future
	FMECATEGORY EventCategory;
	// Represents the module which logged the event
	FMEMODULETYPE EventModule;
	// Literal string used by indivudual modules to distninguish themselves ( non-localizable )
	WCHAR EventSubModule[256];
	// Ady addtional informarmation the logging module deems useful
	WCHAR EventAdditionalInfo[2048];
};
#pragma pack(pop)

template <class MSGTYPE>
class CMailSlotClient
{
	TCHAR m_szSlotName[256];
public:
	CMailSlotClient(LPCTSTR szSlotName)
	{
		StringCbCopy(m_szSlotName, sizeof(m_szSlotName), szSlotName);
	}

	virtual ~CMailSlotClient()
	{
	}

	HRESULT SendMessage(MSGTYPE *pEvent)
	{
		DWORD dwErr = 0;
		HANDLE h = CreateFile(m_szSlotName, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, 0);
		if (h == INVALID_HANDLE_VALUE)
			return HRESULT_FROM_WIN32(GetLastError());

		DWORD dwDummy = 0;
		if (!WriteFile(h, pEvent, sizeof(MSGTYPE), &dwDummy, 0))
			dwErr = GetLastError();

		CloseHandle(h);

		return HRESULT_FROM_WIN32(dwErr);
	}
};

