// TestKW1281.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTestKW1281App:
// See TestKW1281.cpp for the implementation of this class
//

class CTestKW1281App : public CWinApp
{
public:
	CTestKW1281App();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTestKW1281App theApp;