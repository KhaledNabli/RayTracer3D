
// RTWindow.h : main header file for the RTWindow application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRTWindowApp:
// See RTWindow.cpp for the implementation of this class
//

class CRTWindowApp : public CWinApp
{
public:
	CRTWindowApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRTWindowApp theApp;
