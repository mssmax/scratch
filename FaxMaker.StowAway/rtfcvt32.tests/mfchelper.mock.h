#pragma once
#include <afx.h>
#include <afxwin.h>
#include <SystemCalls.h>
//#include <UnitTest.h>

#include "mfchelper.h"



class CFileMock : public CMemFile {
public:
	CFileMock();
	
	virtual BOOL Open(
	   LPCTSTR lpszFileName,
	   UINT nOpenFlags,
	   CFileException* pError = NULL 
	);


};


class MfcHelperMock :
	public MfcHelper
{

public:
	MfcHelperMock() {
	}
	virtual ~MfcHelperMock(void) {
	}
	virtual CFile *CreateFile() {
				
		return new CFileMock();
	}
	virtual BOOL CreateDc(CDC &dc, Device *devices, int devicesCount) {
		return dc.CreateCompatibleDC(NULL);
	}

};
