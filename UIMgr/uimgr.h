// uimgr.h : main header file for the UIMGR application
//

#if !defined(AFX_UIMGR_H__AA31CD67_11D4_459D_885B_C7EB498E5AE6__INCLUDED_)
#define AFX_UIMGR_H__AA31CD67_11D4_459D_885B_C7EB498E5AE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#define ID_MAINHOTKEY 0x1234
#define ID_EMERGENCYHOTKEY 0x4321
/////////////////////////////////////////////////////////////////////////////
// CUIMgrApp:
// See uimgr.cpp for the implementation of this class
//

class CUIMgrApp : public CWinApp
{
public:
	CUIMgrApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIMgrApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CUIMgrApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIMGR_H__AA31CD67_11D4_459D_885B_C7EB498E5AE6__INCLUDED_)
