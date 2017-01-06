; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CUIMgrDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "uimgr.h"

ClassCount=2
Class1=CUIMgrApp
Class2=CUIMgrDlg

ResourceCount=4
Resource2=IDR_MAINFRAME
Resource3=IDD_UIMGR_DIALOG
Resource4=IDR_MAINMENU

[CLS:CUIMgrApp]
Type=0
HeaderFile=uimgr.h
ImplementationFile=uimgr.cpp
Filter=N

[CLS:CUIMgrDlg]
Type=0
HeaderFile=uimgrDlg.h
ImplementationFile=uimgrDlg.cpp
Filter=T
LastObject=CUIMgrDlg
BaseClass=CDialog
VirtualFilter=dWC



[DLG:IDD_UIMGR_DIALOG]
Type=1
Class=CUIMgrDlg
ControlCount=7
Control1=IDC_HOTKEY1,msctls_hotkey32,1350631424
Control2=IDC_CLEARALL,button,1342242816
Control3=IDC_SHOWALL,button,1342242816
Control4=IDC_HIDEALL,button,1342242816
Control5=IDOK,button,1342242817
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352

[MNU:IDR_MAINMENU]
Type=1
Class=?
Command1=ID_CMD_SHOW
Command2=IDC_SHOWALLMNU
Command3=IDC_HIDEALLMNU
Command4=ID_APP_EXIT
CommandCount=4

