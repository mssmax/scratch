// prn_info.h : main header file for the PRN_INFO application
//

#if !defined(AFX_PRN_INFO_H__26828EF0_985F_4F1F_AD48_ED5E9AD1EAC4__INCLUDED_)
#define AFX_PRN_INFO_H__26828EF0_985F_4F1F_AD48_ED5E9AD1EAC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPrnInfoApp:
// See prn_info.cpp for the implementation of this class
//

class CPrnInfoApp : public CWinApp
{
public:
	CPrnInfoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrnInfoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPrnInfoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRN_INFO_H__26828EF0_985F_4F1F_AD48_ED5E9AD1EAC4__INCLUDED_)
