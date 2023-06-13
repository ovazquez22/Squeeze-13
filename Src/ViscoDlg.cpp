// ViscoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "ViscoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViscoDlg dialog


ViscoDlg::ViscoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ViscoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ViscoDlg)
	//}}AFX_DATA_INIT
}


void ViscoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ViscoDlg)
	DDX_Control(pDX, IDC_EDIT_WAT_NV, m_Edit_Wat_nv);
	DDX_Control(pDX, IDC_EDIT_WAT_MAXMU, m_Edit_Wat_MaxMu);
	DDX_Control(pDX, IDC_EDIT_WAT_LANDA, m_Edit_Wat_Landa);
	DDX_Control(pDX, IDC_EDIT_WAT_N, m_Edit_Wat_n);
	DDX_Control(pDX, IDC_CHECK_WAT_SHEAR, m_Check_Wat_Shear);
	DDX_Control(pDX, IDC_EDIT_OIL_NV, m_Edit_Oil_nv);
	DDX_Control(pDX, IDC_EDIT_OIL_MAXMU, m_Edit_Oil_MaxMu);
	DDX_Control(pDX, IDC_EDIT_OIL_LANDA, m_Edit_Oil_Landa);
	DDX_Control(pDX, IDC_EDIT_OIL_N, m_Edit_Oil_n);
	DDX_Control(pDX, IDC_CHECK_OIL_SHEAR, m_Check_Oil_Shear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ViscoDlg, CDialog)
	//{{AFX_MSG_MAP(ViscoDlg)
	ON_BN_CLICKED(IDC_CHECK_WAT_SHEAR, OnWaterShearThinning)
	ON_BN_CLICKED(IDC_CHECK_OIL_SHEAR, OnOilShearThinning)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViscoDlg message handlers

void ViscoDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateFromEdits();

	CDialog::OnOK();
}

void ViscoDlg::UpdateControls()
{	
	CString sn;
	
	// WATER 
	sn.Format("%.2g", waterComp.MaxMU);
	m_Edit_Wat_MaxMu.SetWindowText(sn);

	sn.Format("%.2g", waterComp.nPowMU);
	m_Edit_Wat_nv.SetWindowText(sn);

	sn.Format("%.4g", waterComp.Landa);
	m_Edit_Wat_Landa.SetWindowText(sn);

	sn.Format("%.4g", waterComp.n);
	m_Edit_Wat_n.SetWindowText(sn);

	if ( waterComp.Shear == 1 )
	{	
		m_Check_Wat_Shear.SetCheck(BST_CHECKED);
		GetDlgItem(IDC_STATIC_WAT_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_WAT_N)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_WAT_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WAT_N)->EnableWindow(TRUE);
	}
	else
	{	
		m_Check_Wat_Shear.SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_STATIC_WAT_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WAT_N)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_WAT_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WAT_N)->EnableWindow(FALSE);
	}

	// OIL 
	sn.Format("%.2g", oilComp.MaxMU);
	m_Edit_Oil_MaxMu.SetWindowText(sn);

	sn.Format("%.2g", oilComp.nPowMU);
	m_Edit_Oil_nv.SetWindowText(sn);

	sn.Format("%.4g", oilComp.Landa);
	m_Edit_Oil_Landa.SetWindowText(sn);

	sn.Format("%.4g", oilComp.n);
	m_Edit_Oil_n.SetWindowText(sn);

	if ( oilComp.Shear == 1 )
	{	
		m_Check_Oil_Shear.SetCheck(BST_CHECKED);
		GetDlgItem(IDC_STATIC_OIL_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_OIL_N)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_OIL_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OIL_N)->EnableWindow(TRUE);
	}
	else
	{	
		m_Check_Oil_Shear.SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_STATIC_OIL_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_OIL_N)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_OIL_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OIL_N)->EnableWindow(FALSE);
	}
}

void ViscoDlg::UpdateFromEdits()
{	
	CString sEdit;
	
	// WATER
	m_Edit_Wat_MaxMu.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		waterComp.MaxMU = atof(sEdit.GetBuffer());
	}

	m_Edit_Wat_nv.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		waterComp.nPowMU = atof(sEdit);
	}

	m_Edit_Wat_Landa.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		waterComp.Landa = atof(sEdit);
	}

	m_Edit_Wat_n.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		waterComp.n = atof(sEdit);
	}

	if ( m_Check_Wat_Shear.GetCheck() )
	
		waterComp.Shear = 1;

	else

		waterComp.Shear = 0;

	
	// OIL
	m_Edit_Oil_MaxMu.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		oilComp.MaxMU = atof(sEdit);
	}

	m_Edit_Oil_nv.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		oilComp.nPowMU = atof(sEdit);
	}

	m_Edit_Oil_Landa.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		oilComp.Landa = atof(sEdit);
	}

	m_Edit_Oil_n.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		oilComp.n = atof(sEdit);
	}

	if ( m_Check_Oil_Shear.GetCheck() )
	
		oilComp.Shear = 1;

	else

		oilComp.Shear = 0;

}

BOOL ViscoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	UpdateControls();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void ViscoDlg::OnWaterShearThinning()
{	
	if ( m_Check_Wat_Shear.GetCheck() )
	{	
		waterComp.Shear = 1;
		GetDlgItem(IDC_STATIC_WAT_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_WAT_N)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_WAT_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_WAT_N)->EnableWindow(TRUE);
	}
	else
	{	
		waterComp.Shear = 0;
		GetDlgItem(IDC_STATIC_WAT_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_WAT_N)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_WAT_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_WAT_N)->EnableWindow(FALSE);
	}
}

void ViscoDlg::OnOilShearThinning()
{	
	if ( m_Check_Oil_Shear.GetCheck() )
	{	
		oilComp.Shear = 1;
		GetDlgItem(IDC_STATIC_OIL_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_OIL_N)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_OIL_LANDA)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OIL_N)->EnableWindow(TRUE);
	}
	else
	{	
		oilComp.Shear = 0;
		GetDlgItem(IDC_STATIC_OIL_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_OIL_N)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_OIL_LANDA)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OIL_N)->EnableWindow(FALSE);
	}
}


void ViscoDlg::Allocate( int n )
{
//	nC = n;
/*	nComp = 0;
	waterComp = (ComponentProperties *) malloc ( n * sizeof(ComponentProperties));
	oilComp = (ComponentProperties *) malloc ( n * sizeof(ComponentProperties));*/
}

void ViscoDlg::DeAllocate()
{	
/*	free(waterComp);
	free(oilComp);*/
}