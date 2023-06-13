// OutputTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "OutputTimeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// OutputTimeDlg dialog


OutputTimeDlg::OutputTimeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OutputTimeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(OutputTimeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OutputTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OutputTimeDlg)
	DDX_Control(pDX, IDC_CHECK_WATERCUT, m_check_WatCut);
	DDX_Control(pDX, IDC_CHECK_WATER_TOTAL_FLOW, m_check_Wat_Tot_Flow);
	DDX_Control(pDX, IDC_CHECK_WATER_MASS, m_check_Wat_Mass);
	DDX_Control(pDX, IDC_CHECK_WATER_FLOW_LAYER, m_check_Wat_Flow_Lay);
	DDX_Control(pDX, IDC_CHECK_WATER_CONC, m_check_Wat_Conc);
	DDX_Control(pDX, IDC_CHECK_WATER_CUM, m_check_Wat_Cum);
	DDX_Control(pDX, IDC_CHECK_OIL_TOTAL_FLOW, m_check_Oil_Tot_Flow);
	DDX_Control(pDX, IDC_CHECK_OIL_MASS, m_check_Oil_Mass);
	DDX_Control(pDX, IDC_CHECK_OIL_CONC, m_check_Oil_Conc);
	DDX_Control(pDX, IDC_CHECK_OIL_CUM, m_check_Oil_Cum);
	DDX_Control(pDX, IDC_CHECK_WATER_PEN, m_check_Wat_Pen);
	DDX_Control(pDX, IDC_CHECK_OIL_PEN, m_check_Oil_Pen);
	DDX_Control(pDX, IDC_CHECK_TOT_MASS, m_check_Tot_Mass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OutputTimeDlg, CDialog)
	//{{AFX_MSG_MAP(OutputTimeDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OutputTimeDlg message handlers

BOOL OutputTimeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_check_Wat_Flow_Lay.ShowWindow(bRad);

	m_check_WatCut.SetCheck( bWC ? BST_CHECKED : BST_UNCHECKED);
	m_check_Wat_Tot_Flow.SetCheck( bTQw ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Wat_Mass.SetCheck( bSIw ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Wat_Flow_Lay.SetCheck( bLQw ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Wat_Conc.SetCheck( bTCw ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Wat_Cum.SetCheck( bCPw ? BST_CHECKED : BST_UNCHECKED);
	m_check_Oil_Tot_Flow.SetCheck( bTQo ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Oil_Mass.SetCheck( bSIo ? BST_CHECKED : BST_UNCHECKED); 
	m_check_Oil_Conc.SetCheck( bTCo ? BST_CHECKED : BST_UNCHECKED);
	m_check_Oil_Cum.SetCheck( bCPo ? BST_CHECKED : BST_UNCHECKED);
	m_check_Wat_Pen.SetCheck( bPw ? BST_CHECKED : BST_UNCHECKED);
	m_check_Oil_Pen.SetCheck( bPo ? BST_CHECKED : BST_UNCHECKED);
	m_check_Tot_Mass.SetCheck( bMass ? BST_CHECKED : BST_UNCHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OutputTimeDlg::OnOK() 
{
	// TODO: Add extra validation here
	bWC =	( m_check_WatCut.GetCheck() ? 1 : 0 ); 
	bTQw = ( m_check_Wat_Tot_Flow.GetCheck() ? 1 : 0 ); 
	bSIw = ( m_check_Wat_Mass.GetCheck() ? 1 : 0 ); 
	bLQw = ( m_check_Wat_Flow_Lay.GetCheck() ? 1 : 0 ); 
	bTCw = ( m_check_Wat_Conc.GetCheck() ? 1 : 0 ); 
	bCPw = ( m_check_Wat_Cum.GetCheck() ? 1 : 0 );
	bPw = ( m_check_Wat_Pen.GetCheck() ? 1 : 0 );
	bTQo = ( m_check_Oil_Tot_Flow.GetCheck() ? 1 : 0 ); 
	bSIo = ( m_check_Oil_Mass.GetCheck() ? 1 : 0 ); 
	bTCo = ( m_check_Oil_Conc.GetCheck() ? 1 : 0 );
	bCPo = ( m_check_Oil_Cum.GetCheck() ? 1 : 0 );
	bPo = ( m_check_Oil_Pen.GetCheck() ? 1 : 0 );
	bMass = ( m_check_Tot_Mass.GetCheck() ? 1 : 0 );

	CDialog::OnOK();
}
