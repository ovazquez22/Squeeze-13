// OV Grid DLL.h : main header file for the OV GRID DLL application
//

#if !defined(AFX_OVGRIDDLL_H__003284F9_91CB_47C5_B635_32118425FF6D__INCLUDED_)
#define AFX_OVGRIDDLL_H__003284F9_91CB_47C5_B635_32118425FF6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COVGridDLLApp:
// See OV Grid DLL.cpp for the implementation of this class
//

class COVGridDLLApp : public CWinApp
{
public:
	COVGridDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COVGridDLLApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COVGridDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVGRIDDLL_H__003284F9_91CB_47C5_B635_32118425FF6D__INCLUDED_)
