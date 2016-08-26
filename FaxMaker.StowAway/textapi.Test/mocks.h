//other Mocked Classes
//==============================================================================

#pragma once

#include "utils.h"

class CUtilities_Mock : public CUtilities
{
public:
	MOCK_METHOD2(matchregexstring_, BOOL(LPCWSTR, LPCWSTR));
	MOCK_METHOD1(isFaxNumber_, long (LPCWSTR lpsznum));	
};

//------------------------------------------------------------------------------

class CFaxFrmtMessage_Fake 
{
public:
	long m_liFlags;

	CFaxFrmtMessage_Fake()
	{
		m_liFlags = 0;
	}

	CFaxFrmtMessage_Fake(long flags)
	{
		m_liFlags = flags;
	}
	
	virtual long GetFlags(void)
	{
		return m_liFlags;
	}

	virtual LPCSTR GetAttachmentPath(long i)
	{
		switch (i)
		{
		case 0: return "c:\\test.fmd"; 
		case 1: return "c:\\test.txt"; 
		default: return NULL;
		}
	}
};

//End mocks.h ==================================================================