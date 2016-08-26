#include "MfcHelper.Mock.h"
#include "mfg.h"

CFileMock::CFileMock()
{
}

BOOL CFileMock::Open(
   LPCTSTR lpszFileName,
   UINT nOpenFlags,
   CFileException* pError 
)
{
	//todo desidered access
	DWORD dAccess=0;
	DWORD dwCreationDisposition;

	if (nOpenFlags & CFile::modeReadWrite) dAccess=GENERIC_ALL;
	else if (nOpenFlags & CFile::modeWrite) dAccess=GENERIC_WRITE;
	else dAccess=GENERIC_READ;
	if (nOpenFlags & CFile::modeCreate) {
		dwCreationDisposition=((dAccess & GENERIC_WRITE) || dAccess==GENERIC_ALL) ? CREATE_ALWAYS : TRUNCATE_EXISTING;
		if (nOpenFlags & CFile::modeNoTruncate) dwCreationDisposition=OPEN_ALWAYS;
	} else dwCreationDisposition=((dAccess & GENERIC_WRITE) || dAccess==GENERIC_ALL) ? CREATE_NEW : OPEN_EXISTING;
	
	LPMOCK_HANDLE mHandle=(LPMOCK_HANDLE)GetSystemCalls()->CreateFile(lpszFileName, dAccess, 0, NULL, dwCreationDisposition, 0 ,NULL);
	if (mHandle==INVALID_HANDLE_VALUE) return FALSE;
	
	Attach(mHandle->mFile.pMockFile->GetBuffer(), mHandle->mFile.pMockFile->GetBufferSize());
	return TRUE;
}