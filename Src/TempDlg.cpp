// TempDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "TempDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TempDlg dialog


TempDlg::TempDlg(CWnd* pParent /*=NULL*/)
	: CDialog(TempDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(TempDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void TempDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(TempDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(TempDlg, CDialog)
	//{{AFX_MSG_MAP(TempDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TempDlg message handlers
