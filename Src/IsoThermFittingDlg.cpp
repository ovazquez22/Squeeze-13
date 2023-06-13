// IsoThermFittingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "IsoThermFittingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// IsoThermFittingDlg dialog


IsoThermFittingDlg::IsoThermFittingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IsoThermFittingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(IsoThermFittingDlg)
	
	//}}AFX_DATA_INIT
}


void IsoThermFittingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(IsoThermFittingDlg)
	DDX_Control(pDX, IDC_EDIT_NMIN, m_Edit_nMax);
	DDX_Control(pDX, IDC_EDIT_NMIN, m_Edit_nMin);
	DDX_Control(pDX, IDC_EDIT_RMIN, m_Edit_rMin);
	DDX_Control(pDX, IDC_EDIT_RMAX, m_Edit_rMax);
	DDX_Control(pDX, IDC_EDIT_NMAX, m_Edit_nMax);
	DDX_Control(pDX, IDC_EDIT_KMIN, m_Edit_KMin);
	DDX_Control(pDX, IDC_EDIT_KMAX, m_Edit_KMax);
	DDX_Control(pDX, IDC_EDIT_TRAINPER, m_Edit_TrainPer);
	DDX_Control(pDX, IDC_EDIT_GENERATIONS, m_Edit_Generations);
	DDX_Control(pDX, IDC_EDIT_POPULATION, m_Edit_Population);
//	DDX_Control(pDX, IDC_EDIT_K_ORIG, m_Edit_K_Orig);
	DDX_Control(pDX, IDC_EDIT_N_ORIG, m_Edit_N_Orig);
	DDX_Control(pDX, IDC_EDIT_R_ORIG, m_Edit_R_Orig);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(IsoThermFittingDlg, CDialog)
	//{{AFX_MSG_MAP(IsoThermFittingDlg)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &IsoThermFittingDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// IsoThermFittingDlg message handlers

void IsoThermFittingDlg::UpdateControls()
{
	CString sn;
	sn.Format("%.5g", M.Iso.k);
	m_Edit_TrainPer.SetWindowText(sn);

	/*
	if (Opt.OptAlg == 0) // Hill Climber
	{	
		sn.Format("%d", Opt.HCRun);
		m_Edit_Population.SetWindowText(sn);
	}
	else
	{
		sn.Format("%d", Opt.popsize);
		m_Edit_Population.SetWindowText(sn);
	}

	sn.Format("%.5g", Opt.TrainPer);
	m_Edit_TrainPer.SetWindowText(sn);
	
	sn.Format("%d", Opt.nGen);
	m_Edit_Generations.SetWindowText(sn);
		
	sn.Format("%.5g", Opt.xU[0]);
	m_Edit_KMax.SetWindowText(sn);
	
	sn.Format("%.5g", Opt.xL[0]);
	m_Edit_KMin.SetWindowText(sn);

	sn.Format("%.5g", Opt.xU[1]);
	m_Edit_nMax.SetWindowText(sn);
	
	sn.Format("%.5g", Opt.xL[1]);
	m_Edit_nMin.SetWindowText(sn);

	sn.Format("%.5g", Opt.xU[2]);
	m_Edit_rMax.SetWindowText(sn);
	
	sn.Format("%.5g", Opt.xL[2]);
	m_Edit_rMin.SetWindowText(sn);
	
	sn.Format("%d", Opt.HCGen);
	m_Edit_HC_Init.SetWindowText(sn);

	if ( Opt.HC == 0 ) // Random Initial Population
	{	
		if ( Opt.RandHC == 1 )
			m_Check_Init.SetCheck(BST_UNCHECKED);
		else
			m_Check_Init.SetCheck(BST_CHECKED);

		GetDlgItem(IDC_STATIC_INIT_HC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INIT_HC)->EnableWindow(FALSE);
	}

	if ( Opt.HC == 1 ) // Hill Climber Initial Population
	{	
		m_Check_Init.SetCheck(BST_CHECKED);
		GetDlgItem(IDC_STATIC_INIT_HC)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_INIT_HC)->EnableWindow(TRUE);
	}
	
	m_Check_HC.SetCheck( BST_UNCHECKED );
	m_Check_PSO.SetCheck( BST_UNCHECKED );
	m_Check_RCGA.SetCheck( BST_UNCHECKED );
	m_Check_SGA.SetCheck( BST_UNCHECKED );

	if (Opt.OptAlg == 0) // Hill Climber
	{	
		m_Radio_OptAlg = 0;
		m_Check_HC.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_CHECK_INIT)->SetWindowText("Hill Climber Random Seed");
		GetDlgItem(IDC_STATIC_POPULATION)->SetWindowText("Number of Runs");

		GetDlgItem(IDC_STATIC_INIT_HC)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INIT_HC)->EnableWindow(FALSE);
	}

	if (Opt.OptAlg == 1) // PSO
	{	
		m_Radio_OptAlg = 1;
		m_Check_PSO.SetCheck( BST_CHECKED );
		
		GetDlgItem(IDC_CHECK_INIT)->SetWindowText("Hill Climber Initial Population");
		GetDlgItem(IDC_STATIC_POPULATION)->SetWindowText("Population");
	}
	
	if (Opt.OptAlg == 2) // RCGA
	{	
		m_Radio_OptAlg = 2;
		m_Check_RCGA.SetCheck( BST_CHECKED );
		
		GetDlgItem(IDC_CHECK_INIT)->SetWindowText("Hill Climber Initial Population");
		GetDlgItem(IDC_CHECK_INIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_POPULATION)->SetWindowText("Population");
	}
	
	if (Opt.OptAlg == 3) // SGA
	{	
		m_Radio_OptAlg = 3;
		m_Check_SGA.SetCheck( BST_CHECKED );
		
		GetDlgItem(IDC_CHECK_INIT)->SetWindowText("Hill Climber Initial Population");
		GetDlgItem(IDC_CHECK_INIT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_POPULATION)->SetWindowText("Population");
	}
	
	m_Check_Std.SetCheck(BST_UNCHECKED);
	m_Check_Log.SetCheck(BST_UNCHECKED);

	if ( Opt.type == 0 ) // Standard Misfit
	{	
		m_Type = 0;
		m_Check_Std.SetCheck(BST_CHECKED);
	}
	
	if ( Opt.type == 1 ) // Natural Log Misfit
	{	
		m_Type = 1;
		m_Check_Log.SetCheck(BST_CHECKED);
	}
	*/
}

void IsoThermFittingDlg::UpdateEdits()
{
	CString sn;
	
/*	m_Edit_Population.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.popsize = atoi(sn);
		Opt.HCRun = atoi(sn);
	}

	m_Edit_Generations.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.nGen = atoi(sn);
	}

	m_Edit_TrainPer.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.TrainPer = atof(sn);;
	}

	m_Edit_KMax.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xU[0] = atof(sn);;
	}
	
	m_Edit_KMin.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xL[0] = atof(sn);;
	}
	
	m_Edit_nMax.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xU[1] = atof(sn);;
	}
	
	m_Edit_nMin.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xL[1] = atof(sn);;
	}
	
	m_Edit_rMax.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xU[2] = atof(sn);;
	}
	
	m_Edit_rMin.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.xL[2] = atof(sn);;
	}
	
	m_Edit_HC_Init.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Opt.HCGen = atoi(sn);;
	}

	if ( m_Check_Init.GetCheck() == BST_CHECKED )
	{	
		Opt.HC = 1;
		Opt.RandHC = 0;
	}
	else
	{	
		Opt.HC = 0;
		Opt.RandHC = 1;
	}

	Opt.type = m_Type;
	Opt.OptAlg = m_Radio_OptAlg;
	*/
}

BOOL IsoThermFittingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void IsoThermFittingDlg::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
}



void IsoThermFittingDlg::OnOK() 
{
	UpdateEdits();
	
	CDialog::OnOK();
}


void IsoThermFittingDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateEdits();
	CDialog::OnOK();
}


