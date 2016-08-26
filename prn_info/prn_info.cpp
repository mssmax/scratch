// prn_info.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "prn_info.h"
#include "prn_infoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrnInfoApp

BEGIN_MESSAGE_MAP(CPrnInfoApp, CWinApp)
	//{{AFX_MSG_MAP(CPrnInfoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrnInfoApp construction

CPrnInfoApp::CPrnInfoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrnInfoApp object

CPrnInfoApp theApp;
BOOL g_bSettings = FALSE;


class correctdevmode_c
{
public:
	correctdevmode_c()
	{
		m_pdm = NULL;
	}
	
	~correctdevmode_c()
	{
		deallocate();
	}

public:
	BOOL allocateandfixdevmodestruct(LPSTR szDevice, DEVMODE *pdevmode);

	LPDEVMODE getdevmode() { return m_pdm; }

protected:
	void deallocate()
	{
		LPBYTE pbuffer;
		
		if ((pbuffer = (LPBYTE)m_pdm) != NULL)
		{
			delete [] pbuffer; m_pdm = NULL;
		}
	}

protected:
	LPDEVMODE m_pdm;
};

/////////////////////////////////////////////////////////////////////////////
// CPrnInfoApp initialization

void DumpPrinterInfo(HDC hdc, LPCSTR szDevice)
{
	FILE *f = NULL;

	f = fopen("printerinfo.txt", "a+");
	if(!f)
	{
		AfxMessageBox("Failed to open printerinfo.txt file for writing");
		return;
	}
	
	fprintf(f, "\nSettings change[%s]\n", (g_bSettings) ? "TRUE" : "FALSE");

	fprintf(f, "\nGathering information for printer[%s]\n\n", szDevice);
	DWORD dwValue = 0;

	dwValue = GetDeviceCaps(hdc, HORZRES);
	fprintf(f, "Devcap[%s], value[%d]\n", "HORSZRES", dwValue);

	dwValue = GetDeviceCaps(hdc, VERTRES);
	fprintf(f, "Devcap[%s], value[%d]\n", "VERTRES", dwValue);

	dwValue = GetDeviceCaps(hdc, PHYSICALWIDTH);
	fprintf(f, "Devcap[%s], value[%d]\n", "PHYSICALWIDTH", dwValue);

	dwValue = GetDeviceCaps(hdc, PHYSICALHEIGHT);
	fprintf(f, "Devcap[%s], value[%d]\n", "PHYSICALHEIGHT", dwValue);

	dwValue = GetDeviceCaps(hdc, LOGPIXELSX);
	fprintf(f, "Devcap[%s], value[%d]\n", "LOGPIXELSX", dwValue);

	dwValue = GetDeviceCaps(hdc, LOGPIXELSY);
	fprintf(f, "Devcap[%s], value[%d]\n", "LOGPIXELSY", dwValue);

	dwValue = GetDeviceCaps(hdc, PHYSICALOFFSETX);
	fprintf(f, "Devcap[%s], value[%d]\n", "PHYSICALOFFSETX", dwValue);

	dwValue = GetDeviceCaps(hdc, PHYSICALOFFSETY);
	fprintf(f, "Devcap[%s], value[%d]\n", "PHYSICALOFFSETY", dwValue);

	dwValue = GetDeviceCaps(hdc, TECHNOLOGY);
	fprintf(f, "Devcap[%s], value[0x%X]\n", "TECHNOLOGY", dwValue);

	dwValue = GetDeviceCaps(hdc, DRIVERVERSION);
	fprintf(f, "Devcap[%s], value[%d]\n", "DRIVERVERSION", dwValue);

	dwValue = GetDeviceCaps(hdc, HORZSIZE);
	fprintf(f, "Devcap[%s], value[%d]\n", "HORZSIZE", dwValue);

	dwValue = GetDeviceCaps(hdc, VERTSIZE);
	fprintf(f, "Devcap[%s], value[%d]\n", "VERTSIZE", dwValue);

	dwValue = GetDeviceCaps(hdc, RASTERCAPS);
	fprintf(f, "Devcap[%s], value[0x%X]\n", "RASTERCAPS", dwValue);

	dwValue = GetDeviceCaps(hdc, TEXTCAPS);
	fprintf(f, "Devcap[%s], value[0x%X]\n", "TEXTCAPS", dwValue);

	dwValue = GetDeviceCaps(hdc, SCALINGFACTORX);
	fprintf(f, "Devcap[%s], value[0x%X]\n", "SCALINGFACTORX", dwValue);

	dwValue = GetDeviceCaps(hdc, SCALINGFACTORY);
	fprintf(f, "Devcap[%s], value[0x%X]\n", "SCALINGFACTORY", dwValue);

	dwValue = GetDeviceCaps(hdc, ASPECTX);
	fprintf(f, "Devcap[%s], value[%d]\n", "ASPECTX", dwValue);

	dwValue = GetDeviceCaps(hdc, ASPECTY);
	fprintf(f, "Devcap[%s], value[%d]\n", "ASPECTY", dwValue);

	dwValue = GetDeviceCaps(hdc, ASPECTXY);
	fprintf(f, "Devcap[%s], value[%d]\n", "ASPECTXY", dwValue);

	fclose(f);
}

BOOL CPrnInfoApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	InitCommonControls();

	CPrnInfoDlg dlg1(FALSE);
	dlg1.DoModal();


	CPrintDialog dlg(FALSE);

	if(dlg.DoModal() == IDOK)
	{
		if (dlg.m_pd.hDevMode != NULL) 
		{
			LPBYTE ptr = NULL;
			// copy devmode structure
			if ((ptr = (LPBYTE)GlobalLock(dlg.m_pd.hDevMode)) == NULL)  goto cleanup;
			
			((DEVMODE*)ptr)->dmSize = sizeof DEVMODE;
			((DEVMODE*)ptr)->dmDriverExtra = 0;
			DEVMODE dm = {0};
			memcpy(&dm,(DEVMODEW*)ptr, sizeof DEVMODE);
			
			GlobalUnlock(dlg.m_pd.hDevMode);
			// copy devnames structure
			if ((ptr = (LPBYTE)GlobalLock(dlg.m_pd.hDevNames)) == NULL)  goto cleanup;
			
			DEVNAMES dn = {0};
			
			memcpy(&dn, ptr, sizeof DEVNAMES);

			char szDriver[128] = {0};
			char szDevice[128] = {0};
			
			strncpy(szDriver, (LPCTSTR)ptr + dn.wDriverOffset, 127); 
			strncpy(szDevice, (LPCTSTR)ptr + dn.wDeviceOffset, 127);
//			strncpy(ap->output, W2A((LPCTSTR)ptr + dn.wDriverOffset), 127); ap->output[127] = '\0';
			
			GlobalUnlock(dlg.m_pd.hDevNames);

			correctdevmode_c cdm;

			if (!cdm.allocateandfixdevmodestruct(szDevice, &dm)) 
			{
				AfxMessageBox("Allocateandfixdevmodestruct failed"); 
				goto cleanup;
			}

			HDC hdc = 0;
			if ((hdc = CreateDC(szDriver, szDevice, NULL, cdm.getdevmode())) == NULL)
			{
				DWORD dwlasterror = GetLastError();
				CString s;
				s.Format("Failed to open printer[%s], err %d", szDevice, dwlasterror);
				AfxMessageBox(s);
			}

			DumpPrinterInfo(hdc, szDevice);

			DeleteDC(hdc);
		}

	}
cleanup:

	return FALSE;
}

class closehandle_c
{
public:
	closehandle_c() { m_handle = INVALID_HANDLE_VALUE; }
	closehandle_c(HANDLE h) { sethandle(h); }
	~closehandle_c() { closehandle(); }

public:
	void sethandle(HANDLE h) { m_handle = h; }
	void closehandle()
	{
		if (m_handle != INVALID_HANDLE_VALUE) ClosePrinter(m_handle);
		
		m_handle = INVALID_HANDLE_VALUE;
	}

protected:
	HANDLE m_handle;
};


BOOL correctdevmode_c::allocateandfixdevmodestruct(LPSTR szDevice, DEVMODE *pdevmode)
{
	LPBYTE pbuffer = NULL;
	LPDEVMODE pnewdm = NULL;
	HANDLE hprinter;
	long bytesneeded;

	deallocate();

	if (!OpenPrinter(szDevice, &hprinter, NULL)) return FALSE;

	closehandle_c ch_printer(hprinter);

	if ((bytesneeded = DocumentProperties(NULL, hprinter, szDevice, NULL, NULL, 0)) < 0) return FALSE;

	// allocate
	try { pbuffer = new BYTE[bytesneeded]; } catch(...) { pbuffer = NULL; }

	// out of memory
	if (pbuffer == NULL) return FALSE;

	// cast
	pnewdm = (LPDEVMODE)pbuffer;

	// get default settings
	if (DocumentProperties(NULL, hprinter, szDevice, pnewdm, NULL, DM_OUT_BUFFER) != IDOK) {
		delete [] pbuffer; return FALSE;
	}

	// cast global
	m_pdm = (LPDEVMODE)pbuffer;

	// copy data to new devmode structure
	pnewdm->dmOrientation = pdevmode->dmOrientation;
	pnewdm->dmPaperSize = pdevmode->dmPaperSize;
	pnewdm->dmPaperLength = pdevmode->dmPaperLength;
	pnewdm->dmPaperWidth = pdevmode->dmPaperWidth;
	pnewdm->dmScale = pdevmode->dmScale;
	pnewdm->dmCopies = pdevmode->dmCopies;
	pnewdm->dmDefaultSource = pdevmode->dmDefaultSource;
	pnewdm->dmPrintQuality = pdevmode->dmPrintQuality;
	pnewdm->dmColor = pdevmode->dmColor;
	pnewdm->dmDuplex = pdevmode->dmDuplex;
	pnewdm->dmYResolution = pdevmode->dmYResolution;
	pnewdm->dmTTOption = pdevmode->dmTTOption;
	pnewdm->dmCollate = pdevmode->dmCollate;
	memcpy(pnewdm->dmFormName, pdevmode->dmFormName, CCHFORMNAME);
	pnewdm->dmFields = pdevmode->dmFields;


	// fix devmode structure
	if (pnewdm->dmFields & DM_ORIENTATION)
	{
		// or portrait or landscape
		if (pnewdm->dmOrientation != DMORIENT_PORTRAIT && pnewdm->dmOrientation != DMORIENT_LANDSCAPE)
		{
			pnewdm->dmOrientation = DMORIENT_PORTRAIT;
		}
	}
	else
	{
		pnewdm->dmOrientation = 0;
	}


	if (pnewdm->dmFields & DM_PAPERSIZE)
	{
		if (pnewdm->dmPaperSize < DMPAPER_FIRST || pnewdm->dmPaperSize > DMPAPER_LAST)
		{
			pnewdm->dmPaperSize = DMPAPER_LETTER;
		}
		
		if ((pnewdm->dmFields & (DM_PAPERLENGTH|DM_PAPERWIDTH)) == (DM_PAPERLENGTH|DM_PAPERWIDTH) &&
			pnewdm->dmPaperLength > 0 && pnewdm->dmPaperWidth > 0)
		{
			pnewdm->dmPaperSize = 0;
		}
	}
	else
	{
		pnewdm->dmPaperSize = 0;
	}

	
	if (pnewdm->dmFields & DM_PAPERLENGTH)
	{
		if ((pnewdm->dmFields & DM_PAPERSIZE) && pnewdm->dmPaperSize > 0)
		{
			pnewdm->dmFields &= ~DM_PAPERLENGTH;
			pnewdm->dmPaperLength = 0;
		}
	}
	else
	{
		pnewdm->dmPaperLength = 0;
	}

	
	if (pnewdm->dmFields & DM_PAPERWIDTH)
	{
		if ((pnewdm->dmFields & DM_PAPERSIZE) && pnewdm->dmPaperSize > 0)
		{
			pnewdm->dmFields &= ~DM_PAPERWIDTH;
			pnewdm->dmPaperWidth = 0;
		}
	}
	else
	{
		pnewdm->dmPaperWidth = 0;
	}

	
	if (pnewdm->dmFields & DM_POSITION)
	{
		// only for display devices
		pnewdm->dmFields &= ~DM_POSITION;
		// do not set 'pnewdm->dmPosition = 0' as this is a union to the above 4 members
	}
	
	if (pnewdm->dmFields & DM_SCALE)
	{
		// this can cause a divide by zero!
		if (pnewdm->dmScale == 0)
		{
			pnewdm->dmFields &= ~DM_SCALE;
		}
	}
	else
	{
		pnewdm->dmScale = 0;
	}

	
	if (pnewdm->dmFields & DM_COPIES)
	{
		if (pnewdm->dmCopies <= 0) pnewdm->dmCopies = 1;
	}
	else
	{
		pnewdm->dmCopies = 0;
	}
	
	
	if (pnewdm->dmFields & DM_DEFAULTSOURCE)
	{
		//pnewdm->dmDefaultSource;
	}
	else
	{
		pnewdm->dmDefaultSource = 0;
	}
	

	if (pnewdm->dmFields & DM_PRINTQUALITY)
	{
		//pnewdm->dmPrintQuality;
	}
	else
	{
		pnewdm->dmPrintQuality = 0;
	}

	
	if (pnewdm->dmFields & DM_COLOR)
	{
		// only monochrome is needed
		pnewdm->dmColor = DMCOLOR_MONOCHROME;
	}
	else
	{
		pnewdm->dmColor = 0;
	}
	

	if (pnewdm->dmFields & DM_DUPLEX)
	{
		if (pnewdm->dmDuplex != DMDUP_SIMPLEX &&
			pnewdm->dmDuplex != DMDUP_HORIZONTAL &&
			pnewdm->dmDuplex != DMDUP_VERTICAL)
		{
			pnewdm->dmFields &= ~DM_DUPLEX;
			pnewdm->dmDuplex = 0;
		}
	}
	else
	{
		pnewdm->dmDuplex = 0;
	}

	
	if (pnewdm->dmFields & DM_YRESOLUTION)
	{
		//pnewdm->dmYResolution;
	}
	else
	{
		pnewdm->dmYResolution = 0;
	}

	
	if (pnewdm->dmFields & DM_TTOPTION)
	{
		if (pnewdm->dmTTOption != DMTT_BITMAP &&
			pnewdm->dmTTOption != DMTT_DOWNLOAD &&
			pnewdm->dmTTOption != DMTT_DOWNLOAD_OUTLINE &&
			pnewdm->dmTTOption != DMTT_SUBDEV)
		{
			pnewdm->dmFields &= ~DM_TTOPTION;
			pnewdm->dmTTOption = 0;
		}
	}
	else
	{
		pnewdm->dmTTOption = 0;
	}


	if (pnewdm->dmFields & DM_COLLATE)
	{
		if (pnewdm->dmCollate != DMCOLLATE_TRUE && pnewdm->dmCollate != DMCOLLATE_FALSE)
		{
			pnewdm->dmFields &= ~DM_COLLATE;
			pnewdm->dmCollate = 0;
		}
	}
	else
	{
		pnewdm->dmCollate = 0;
	}

	
	if (pnewdm->dmFields & DM_FORMNAME)
	{
		//pnewdm->dmFormName; 
	}
	else
	{
		memset(pnewdm->dmFormName, 0, CCHFORMNAME);
	}
	
	return TRUE;
}
