#pragma once

//UNIT TESTING TIFF WRAPPER

#include <fmcommon.h>
#include <windows.h>
#include "ILogInterface.h"


class CTiffWrapper
{
public:
	CTiffWrapper(ILogInterface *log);
	~CTiffWrapper(void);

	fmerror ConvertToGroup3Fax(LPCSTR srcfile,LPCSTR destfile);

	static int CountPages(LPSTR lpszTiffFile);


	
};
