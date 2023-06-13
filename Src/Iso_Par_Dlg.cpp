// Iso_Par_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Iso_Par_Dlg.h"
#include "afxdialogex.h"


// Iso_Par_Dlg dialog

IMPLEMENT_DYNAMIC(Iso_Par_Dlg, CDialogEx)

Iso_Par_Dlg::Iso_Par_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Iso_Par_Dlg::IDD, pParent)
{
	
}

Iso_Par_Dlg::~Iso_Par_Dlg()
{
}

void Iso_Par_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_POP_SIZE, m_edit_Pop_size);
	DDX_Control(pDX, IDC_EDIT_NUM_GEN, m_edit_Num_Gen);
	DDX_Control(pDX, IDC_EDIT_K_ORIG, m_Edit_K_Orig);
	DDX_Control(pDX, IDC_EDIT_K_UP, m_Edit_K_Up);
	DDX_Control(pDX, IDC_EDIT_K_LOW, m_Edit_K_Low);
	DDX_Control(pDX, IDC_EDIT_N_ORIG, m_Edit_N_Orig);
	DDX_Control(pDX, IDC_EDIT_N_UP, m_Edit_N_Up);
	DDX_Control(pDX, IDC_EDIT_N_LOW, m_Edit_N_Low);
	DDX_Control(pDX, IDC_EDIT_R_ORIG, m_Edit_R_Orig);
	DDX_Control(pDX, IDC_EDIT_R_UP, m_Edit_R_Up);
	DDX_Control(pDX, IDC_EDIT_R_LOW, m_Edit_R_Low);
}


BEGIN_MESSAGE_MAP(Iso_Par_Dlg, CDialogEx)
	ON_BN_CLICKED(IDOK2, &Iso_Par_Dlg::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL, &Iso_Par_Dlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// Iso_Par_Dlg message handlers
BOOL Iso_Par_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Iso_Par_Dlg::UpdateControls()
{
	CString sn;

	m_edit_Pop_size.SetWindowTextA("5");
	m_edit_Num_Gen.SetWindowTextA("3");

	sn.Format("%.5g", M.Iso.k);
	m_Edit_K_Orig.SetWindowTextA(sn);
	
	if (M.SqzOpt.uX[0]>0) sn.Format("%.5g", 1.5);
	else sn.Format("%.5g", 3000.0 / M.Iso.k);
	m_Edit_K_Up.SetWindowTextA(sn);
	

	if (M.SqzOpt.lX[0]>0) sn.Format("%.5g", 0.05);
	else sn.Format("%.5g", 10.0/ M.Iso.k);
	m_Edit_K_Low.SetWindowTextA(sn);
	
	sn.Format("%.5g", M.Iso.n);
	m_Edit_N_Orig.SetWindowTextA(sn);

	if (M.SqzOpt.uX[1]>0) sn.Format("%.5g", M.SqzOpt.uX[1]);
	else sn.Format("%.5g", 1.0/M.Iso.n);
	m_Edit_N_Up.SetWindowTextA(sn);
	
	if (M.SqzOpt.lX[1]>0) sn.Format("%.8g", M.SqzOpt.lX[1]);
	else sn.Format("%.5g", 0.01 / M.Iso.n);
	m_Edit_N_Low.SetWindowTextA(sn);


	if (M.SqzOpt.nPar > 2)
	{	
		m_Edit_R_Orig.EnableWindow(true);
		sn.Format("%.5g", M.Iso.r2);
		m_Edit_R_Orig.SetWindowTextA(sn);

		m_Edit_R_Up.EnableWindow(true);
		if (M.SqzOpt.uX[2]>0) sn.Format("%.5g", M.SqzOpt.uX[2]);
		else sn.Format("%.5g", 1.0 / M.Iso.r2);
		m_Edit_R_Up.SetWindowTextA(sn);
		
		m_Edit_R_Low.EnableWindow(true);
		if (M.SqzOpt.lX[2]>0) sn.Format("%.8g", M.SqzOpt.lX[2]);
		else sn.Format("%.5g", 0.01 / M.Iso.r2);
		m_Edit_R_Low.SetWindowTextA(sn);
	}
	else
	{
		m_Edit_R_Orig.EnableWindow(false);
		m_Edit_R_Up.EnableWindow(false);
		m_Edit_R_Low.EnableWindow(false);
	}
}

void Iso_Par_Dlg::UpdateEdits()
{	
	CString sn;
	m_edit_Pop_size.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.nPopSize = atoi(sn);
	}

	m_edit_Num_Gen.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.nGen = atoi(sn);
	}

	m_Edit_K_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.k = atof(sn);
	}

	m_Edit_K_Up.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.uX[0] = atof(sn);
	}

	m_Edit_K_Low.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.lX[0] = atof(sn);
	}

	m_Edit_N_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.n = atof(sn);
	}

	m_Edit_N_Up.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.uX[1] = atof(sn);
	}

	m_Edit_N_Low.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.SqzOpt.lX[1] = atof(sn);
	}

	m_Edit_R_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.r2 = atof(sn);
	}

	if (M.SqzOpt.nPar > 2)
	{
		m_Edit_R_Up.GetWindowText(sn);
		if (sn.IsEmpty() == FALSE)
		{
			sn.TrimRight();
			M.SqzOpt.uX[2] = atof(sn);
		}

		m_Edit_R_Low.GetWindowText(sn);
		if (sn.IsEmpty() == FALSE)
		{
			sn.TrimRight();
			M.SqzOpt.lX[2] = atof(sn);
		}
	}
}

void Iso_Par_Dlg::OnBnClickedOk2()
{	
	UpdateEdits();

	CDialog::OnOK();
	// TODO: Add your control notification handler code here
}


void Iso_Par_Dlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
