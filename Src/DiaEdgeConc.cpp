// DiaEdgeConc.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "DiaEdgeConc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DiaEdgeConc dialog


DiaEdgeConc::DiaEdgeConc(CWnd* pParent /*=NULL*/)
	: CDialog(DiaEdgeConc::IDD, pParent)
{
	//{{AFX_DATA_INIT(DiaEdgeConc)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void DiaEdgeConc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DiaEdgeConc)
	DDX_Control(pDX, IDC_STATIC_EDGE_CONC, m_Static_EdgeConc);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DiaEdgeConc, CDialog)
	//{{AFX_MSG_MAP(DiaEdgeConc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DiaEdgeConc message handlers

BOOL DiaEdgeConc::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sEdit,sEdit1;
	
	if (wo == 0) sEdit1 = "Water";
	
	if ( wo == 1 ) sEdit1 = "Oil";

	sEdit.Format("In Layer %d Component %d in the %s phase reaches the edge of the grid model",lay+1,comp+1, sEdit1);	
	
	m_Static_EdgeConc.SetWindowText(sEdit);	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
