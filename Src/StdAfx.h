// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9DD3D604_B2E9_433D_BD78_936A71D0A232__INCLUDED_)
#define AFX_STDAFX_H__9DD3D604_B2E9_433D_BD78_936A71D0A232__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <signal.h>
//#include <process.h>
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <fstream>
#include <string>
#include <strsafe.h>
#include <atlimage.h>
#include "Resource.h"
#include <iostream>
#include <thread>
#include <random>
#include <vector>
#include <string>
#include <cstdio>
#include <mutex>
#include <ctime>

#define PI 3.14159265358979 // Pi

#define JMAX 300
#define MaxNIso 500
#define MaxClife 5
#define MaxComp 10
#define MaxLay 50
#define MaxnPts 300
#define MaxFileNm 8
#define MaxnRP 300
#define MaxChrom 10
#define MaxGene 10

#define IDC_SCROLLBAR1 1000
#define UDM_LIST_CTRL_UPDATE			102
#define UDM_LIST_CTRL_UPDATE1			102

#define IDC_GRAPH 5469

enum STAGE_TYPE
{
	INJECTION = 1,
	PRODUCTION = 2
};

enum COMPONENT_TYPE
{
	WAT = 1,
	OIL = 2
};


#define NR_END 1
#define FREE_ARG char*
#define TINY 1.0e-20  // A small number.
#define FREERETURN {free_matrix(fjac,1,n,1,n);free_vector(fvec,1,n);\
free_vector(p,1,n);free_ivector(indx,1,n);return;}

#include <iostream>
#include <math.h>
#include <string>
#include <direct.h>
#include <stdio.h>
#include <stddef.h>
#include <vector>

#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <afxcontrolbars.h>

#include <exception>

#include "Windows.h"
#include "JobsPool.h"
#include "Model.h"
#include <afxdlgs.h>



using namespace std;

#define new DEBUG_NEW

#define UWM_CUSTOM (WM_APP+1)
#define UWM_SAVEDESIGN (WM_APP+2)
#define UWM_EXPORTTABLE (WM_APP+3)
#define UWM_EXPORTPARETO (WM_APP+4)
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9DD3D604_B2E9_433D_BD78_936A71D0A232__INCLUDED_)
