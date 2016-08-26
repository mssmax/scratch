#include "stdafx.h"
#include "mockobjects.h"

namespace callhome {
	CPatchCheckerMock *g_pPatchCheckerMock;

	void SetPatchCheckerMock(CPatchCheckerMock *ptr)
	{
		g_pPatchCheckerMock=ptr;
	}

	CPatchCheckerBase *GetNewPatchChecker(LPCSTR lpstrProductID,DWORD dwBuildNumber, bool async)
	{
		return g_pPatchCheckerMock;
	}

	void DeletePatchChecker(CPatchCheckerBase *)
	{
	}
};