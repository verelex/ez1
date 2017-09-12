// ez1.h : main header file for the ez1 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cez1App:
// See ez1.cpp for the implementation of this class
//

class Cez1App : public CWinApp
{
public:
	Cez1App();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cez1App theApp;