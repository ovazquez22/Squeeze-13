// OptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "OptDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OptDlg dialog


OptDlg::OptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OptDlg)
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//}}AFX_DATA_INIT
}


void OptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OptDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OptDlg, CDialog)
	//{{AFX_MSG_MAP(OptDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OptDlg message handlers

void OptDlg::OnButtonOk() 
{
	ShellExecute(NULL,"open","notepad.exe",snfile,NULL,SW_SHOWNORMAL);
	DestroyWindow( );
}
