// IsoMatchParDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "IsoMatchParDlg.h"
#include "afxdialogex.h"


// IsoMatchParDlg dialog

IMPLEMENT_DYNAMIC(IsoMatchParDlg, CDialogEx)

IsoMatchParDlg::IsoMatchParDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IsoMatchParDlg::IDD, pParent)
{
	UpdateControls();
}

IsoMatchParDlg::~IsoMatchParDlg()
{
}

void IsoMatchParDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	/*	DDX_Control(pDX, IDC_EDIT_K_ORIG, m_Edit_K_Orig);
		DDX_Control(pDX, IDC_EDIT_N_ORIG, m_Edit_N_Orig);
		DDX_Control(pDX, IDC_EDIT_R_ORIG, m_Edit_R_Orig);
		DDX_Control(pDX, IDC_EDIT_K_UP, m_Edit_K_Up);
		DDX_Control(pDX, IDC_EDIT_N_UP, m_Edit_N_Up);
		DDX_Control(pDX, IDC_EDIT_R_UP, m_Edit_R_Up);
		DDX_Control(pDX, IDC_EDIT_K_LOW, m_Edit_K_Low);
		DDX_Control(pDX, IDC_EDIT_N_LOW, m_Edit_N_Low);
		DDX_Control(pDX, IDC_EDIT_R_LOW, m_Edit_R_Low);*/
	DDX_Control(pDX, IDC_EDIT_K_ORIG, m_Edit_K_Orig);
	DDX_Control(pDX, IDC_EDIT_K_UP, m_Edit_K_Up);
	DDX_Control(pDX, IDC_EDIT_POPULATION_SIZE, m_Edit_PopSize);
}


BEGIN_MESSAGE_MAP(IsoMatchParDlg, CDialogEx)
END_MESSAGE_MAP()


// IsoMatchParDlg message handlers
void IsoMatchParDlg::UpdateControls()
{	
	CString sn;

	m_Edit_PopSize.SetWindowTextA("cfdcfdcde");
/*	sn.Format("%.5g", 3.0);
	m_Edit_K_Up.SetWindowText(sn);
	
	sn.Format("%.5g", M.Iso.n);
	m_Edit_N_Orig.SetWindowText(sn);

	sn.Format("%.5g", M.Iso.r2);
	m_Edit_R_Orig.SetWindowText(sn);

	sn.Format("%.5g", M.Iso.k*M.uX[0]);
	m_Edit_K_Up.SetWindowText(sn);

	sn.Format("%.5g", M.Iso.n*M.uX[1]);
	m_Edit_N_Up.SetWindowText(sn);

	if (M.nPar == 2)
	{
		sn.Format("%.5g", M.uX[2]);
		m_Edit_R_Up.SetWindowText(sn);

		sn.Format("%.5g", M.lX[2]);
		m_Edit_R_Low.SetWindowText(sn);
	}

	sn.Format("%.5g", M.Iso.k*M.lX[0]);
	m_Edit_K_Low.SetWindowText(sn);

	sn.Format("%.5g", M.Iso.n*M.lX[1]);
	m_Edit_N_Low.SetWindowText(sn);

	*/
		
}

void IsoMatchParDlg::UpdateEdits()
{	
	CString sn;

/*	m_Edit_K_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.k = atof(sn);
	}

	m_Edit_N_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.n = atof(sn);
	}

	m_Edit_R_Orig.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.Iso.r2 = atof(sn);
	}

	m_Edit_K_Up.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.uX[0] = atof(sn);
	}

	m_Edit_K_Low.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.lX[0] = atof(sn);
	}

	m_Edit_N_Up.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.uX[1] = atof(sn);
	}

	m_Edit_N_Low.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		M.lX[1] = atof(sn);
	}

	if (M.nPar == 2)
	{
		m_Edit_R_Up.GetWindowText(sn);
		if (sn.IsEmpty() == FALSE)
		{
			sn.TrimRight();
			M.uX[2] = atof(sn);
		}

		m_Edit_R_Low.GetWindowText(sn);
		if (sn.IsEmpty() == FALSE)
		{
			sn.TrimRight();
			M.lX[2] = atof(sn);
		}
	}*/
}


