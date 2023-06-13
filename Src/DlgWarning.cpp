// DlgWarning.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "DlgWarning.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DlgWarning dialog


DlgWarning::DlgWarning(CWnd* pParent /*=NULL*/)
	: CDialog(DlgWarning::IDD, pParent)
{
	//{{AFX_DATA_INIT(DlgWarning)
	bPhi = bKabs = bSrw = bSro = bSwi = bHLay = FALSE;
	//}}AFX_DATA_INIT
}


void DlgWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DlgWarning)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DlgWarning, CDialog)
	//{{AFX_MSG_MAP(DlgWarning)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DlgWarning message handlers

BOOL DlgWarning::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int count = 0,
		 h = 20,
		 sp = 5;

	if( bPhi )
	{
		GetDlgItem(IDC_STATIC_PHI)->SetWindowText("Porosity out of range (0<Phi<1)");
		GetDlgItem(IDC_STATIC_PHI)->MoveWindow(width/3,height/3+5+count*h,width*2/3,h);
		GetDlgItem(IDC_STATIC_PHI)->ShowWindow(TRUE);
		count++;
	}
	else
	
		GetDlgItem(IDC_STATIC_PHI)->ShowWindow(FALSE);

	if( bKabs )
	{
		GetDlgItem(IDC_STATIC_KABS)->SetWindowText("Porosity out of range (0<Phi<1)");
		GetDlgItem(IDC_STATIC_KABS)->MoveWindow(width/3,height/3+5+count*h,width*2/3,h);
		GetDlgItem(IDC_STATIC_KABS)->ShowWindow(TRUE);
		count++;
	}			
	else
	
		GetDlgItem(IDC_STATIC_KABS)->ShowWindow(bKabs);

	if( bSrw || bSro || bSwi )
	{
		GetDlgItem(IDC_STATIC_SAT)->SetWindowText("Porosity out of range (0<Phi<1)");
		GetDlgItem(IDC_STATIC_SAT)->MoveWindow(width/3,height/3+5+count*h,width*2/3,h);
		GetDlgItem(IDC_STATIC_SAT)->ShowWindow(TRUE);
		count++;
	}		
	else
	
		GetDlgItem(IDC_STATIC_SAT)->ShowWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DlgWarning::WriteWarnings()
{
	int count = 0,
		 h = 20,
		 sp = 5,
		 xPos = 288/20,
		 yPos = 45,
		 wTxt = 288-xPos;

	if( bPhi )
	{
		GetDlgItem(IDC_STATIC_PHI)->SetWindowText("- Porosity out of range (0<Phi<1)");
		GetDlgItem(IDC_STATIC_PHI)->MoveWindow(xPos,yPos+5+count*h,wTxt,h);
		GetDlgItem(IDC_STATIC_PHI)->ShowWindow(TRUE);
		count++;
	}
	else
	
		GetDlgItem(IDC_STATIC_PHI)->ShowWindow(FALSE);

	if( bKabs )
	{
		GetDlgItem(IDC_STATIC_KABS)->SetWindowText("- Permeability out of range (0<K<10,000)");
		GetDlgItem(IDC_STATIC_KABS)->MoveWindow(xPos,yPos+5+count*h,wTxt,h);
		GetDlgItem(IDC_STATIC_KABS)->ShowWindow(TRUE);
		count++;
	}			
	else
	
		GetDlgItem(IDC_STATIC_KABS)->ShowWindow(FALSE);
	
	if( bHLay )
	{
		GetDlgItem(IDC_STATIC_HEIGHT)->SetWindowText("- Layer Height out of range (H>0)");
		GetDlgItem(IDC_STATIC_HEIGHT)->MoveWindow(xPos,yPos+5+count*h,wTxt,h);
		GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(TRUE);
		count++;
	}			
	else
	
		GetDlgItem(IDC_STATIC_HEIGHT)->ShowWindow(FALSE);

	if( bSrw || bSro || bSwi )
	{
		GetDlgItem(IDC_STATIC_SAT)->SetWindowText("- Water or/and Oil Saturation out of range (0=<S=<1)");
		GetDlgItem(IDC_STATIC_SAT)->MoveWindow(xPos,yPos+5+count*h,wTxt,h);
		GetDlgItem(IDC_STATIC_SAT)->ShowWindow(TRUE);
		count++;
	}		
	else
	
		GetDlgItem(IDC_STATIC_SAT)->ShowWindow(FALSE);
}
