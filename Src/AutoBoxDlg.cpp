// AutoBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "AutoBoxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AutoBoxDlg dialog


AutoBoxDlg::AutoBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(AutoBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(AutoBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void AutoBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AutoBoxDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AutoBoxDlg, CDialog)
	//{{AFX_MSG_MAP(AutoBoxDlg)
	ON_WM_KILLFOCUS()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_SAVEIMAGE, &AutoBoxDlg::OnSaveimage)
	ON_BN_CLICKED(IDOK, &AutoBoxDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_nPopSize, &AutoBoxDlg::OnEnChangeEditnpopsize)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AutoBoxDlg message handlers

void AutoBoxDlg::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	
}

void AutoBoxDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}





void AutoBoxDlg::OnSaveimage()
{
	// TODO: Add your command handler code here
}


void AutoBoxDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void AutoBoxDlg::OnEnChangeEditnpopsize()
{
	// TODO: Add your command handler code here
}