// MutSolvDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "MutSolvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MutSolvDlg dialog


MutSolvDlg::MutSolvDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MutSolvDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(MutSolvDlg)
	NewSwi = NewSor = 0.0;
	MAdso = 0; // Adsorption Alteration (=0 No, =1 Yes) 
	IsoTyp = 1; // = 1 - Freundlich,
					// = 2 - Langmuir,
			
	Ads1 = 100;
	Ads2 = 0.1;
	MaxAdso = 50000;
	ModAdd = 0;
	MS_BA = false;
			 
	//}}AFX_DATA_INIT
}


void MutSolvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MutSolvDlg)
	DDX_Control(pDX, IDC_CHECK_MODADD, m_Check_ModAdd);
	DDX_Control(pDX, IDC_EDIT_MAX_ADS, m_Edit_MaxAdso);
	DDX_Control(pDX, IDC_EDIT_NEW_SRO, m_Edit_New_Sro);
	DDX_Control(pDX, IDC_EDIT_NEW_SWI, m_Edit_New_Swi);
	DDX_Control(pDX, IDC_EDIT_ADS1, m_Ads1);
	DDX_Control(pDX, IDC_EDIT_ADS2, m_Ads2);
	DDX_Control(pDX,IDC_EDIT_TABLE,m_Table);
	DDX_Control(pDX, IDC_CHECK_MADSO, m_Check_MAdso);
	DDX_Control(pDX, IDC_CHECK_IRR, m_Check_Irr);
	DDX_Control(pDX, IDC_RADIO_FREU, m_Freundlich);
	DDX_Control(pDX, IDC_RADIO_LANG, m_Langmuir);
	DDX_Control(pDX, IDC_RADIO_TABLE, m_Tab);
	DDX_Radio(pDX, IDC_RADIO_FREU, m_Radio_IsoType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MutSolvDlg, CDialog)
	//{{AFX_MSG_MAP(MutSolvDlg)
	ON_BN_CLICKED(IDC_CHECK_MADSO, OnCheckMAdso)
	ON_BN_CLICKED(IDC_RADIO_FREU, OnRadioFreundlich)
	ON_BN_CLICKED(IDC_RADIO_LANG, OnRadioLangmuir)
	ON_BN_CLICKED(IDC_RADIO_TABLE, OnRadioTable)
	ON_BN_CLICKED(IDC_CHECK_IRR, OnCheckIrr)
	ON_BN_CLICKED(IDC_CHECK_MODADD, OnCheckModadd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MutSolvDlg message handlers

BOOL MutSolvDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void MutSolvDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateEdits();
	CDialog::OnOK();
}

void MutSolvDlg::UpdateControls()
{
	CString sn;
	
//	GetDlgItem(IDD_MUTSOLV_DIALOG)->SetWindowText("New SI Isotherm with Additive");

	sn.Format("%.5g", NewSwi);
	m_Edit_New_Swi.SetWindowText(sn);
	
	sn.Format("%.5g", NewSor);
	m_Edit_New_Sro.SetWindowText(sn);
	
	sn.Format("%.5g", Ads1);
	m_Ads1.SetWindowText(sn);
	
	sn.Format("%.5g", Ads2);
	m_Ads2.SetWindowText(sn);
	
	sn.Format("%s", TableName);
	m_Table.SetWindowText(sn);
	
	sn.Format("%.5g", MaxAdso);
	m_Edit_MaxAdso.SetWindowText(sn);

	if ( Irr == 1 )
	{	
		m_Check_Irr.SetCheck(BST_CHECKED);
	}

	if ( Irr == 0 )
	{	
		m_Check_Irr.SetCheck(BST_UNCHECKED);
	}
	
	GetDlgItem(IDC_STATIC_IRREDUCIBLE)->EnableWindow(MS_BA);
	GetDlgItem(IDC_STATIC_NEWSWI)->EnableWindow(MS_BA);
	GetDlgItem(IDC_EDIT_NEW_SWI)->EnableWindow(MS_BA);
	GetDlgItem(IDC_STATIC_NEWSRO)->EnableWindow(MS_BA);
	GetDlgItem(IDC_EDIT_NEW_SRO)->EnableWindow(MS_BA);
	GetDlgItem(IDC_CHECK_IRR)->ShowWindow(MS_BA);
	
	GetDlgItem(IDC_CHECK_MODADD)->ShowWindow(!MS_BA);

	if ( ModAdd == 1 )
	{	
		m_Check_ModAdd.SetCheck(BST_CHECKED);
		GetDlgItem(IDC_STATIC_MAX_ADS)->ShowWindow(!MS_BA);
		GetDlgItem(IDC_EDIT_MAX_ADS)->ShowWindow(!MS_BA);
	}
	else
	{
		m_Check_ModAdd.SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_STATIC_MAX_ADS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_ADS)->ShowWindow(FALSE);
	}
	
	if ( MAdso == 1 )
	{	
		m_Check_MAdso.SetCheck(BST_CHECKED);
		GetDlgItem(IDC_EDIT_ADS1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ADS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MUT_ADS1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MUT_ADS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREU)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LANG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_IRR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MODADD)->EnableWindow(TRUE);
	}	
	else
	{	
		m_Check_MAdso.SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_EDIT_ADS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MUT_ADS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MUT_ADS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREU)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IRR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MODADD)->EnableWindow(FALSE);
	}

	if (IsoTyp == 1) // Freundlich
	{	
		m_Radio_IsoType = 0;
		m_Freundlich.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_STATIC_MUT_ADS1)->SetWindowText("k");
		GetDlgItem(IDC_STATIC_MUT_ADS2)->SetWindowText("n");
	}

	if (IsoTyp == 2) // Langmuir
	{	
		m_Radio_IsoType = 1;
		m_Langmuir.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_STATIC_MUT_ADS1)->SetWindowText("a");
		GetDlgItem(IDC_STATIC_MUT_ADS2)->SetWindowText("b");
	}
	
	if (IsoTyp == 3) // Table
	{	
		m_Radio_IsoType = 2;
		m_Tab.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_STATIC_MUT_ADS1)->SetWindowText("a");
		GetDlgItem(IDC_STATIC_MUT_ADS2)->SetWindowText("b");
	}
}

void MutSolvDlg::UpdateEdits()
{
	CString sn;

	m_Edit_New_Swi.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		NewSwi = atof(sn);
	}

	m_Edit_New_Sro.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		NewSor = atof(sn);
	}

	m_Ads1.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Ads1 = atof(sn);;
	}

	m_Ads2.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Ads2 = atof(sn);;
	}
	
	m_Edit_MaxAdso.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		MaxAdso = atof(sn);;
	}

	m_Table.GetWindowText(sn);
	TableName = sn;
	
	if ( m_Check_ModAdd.GetCheck() == BST_CHECKED )
	{	
		ModAdd = 1;
	}
	else
	{	
		ModAdd = 0;
	}

	if ( m_Check_MAdso.GetCheck() == BST_CHECKED )
	{	
		MAdso = 1;
	}
	else
	{	
		MAdso = 0;
	}
	
	if ( m_Check_Irr.GetCheck() == BST_CHECKED )
	{	
		Irr = 1;
	}
	else
	{	
		Irr = 0;
	}

	if ( m_Radio_IsoType == 0 ) // Freundlich
	{	
		IsoTyp = 1;
	}
	
	if ( m_Radio_IsoType == 1 ) // Langmuir
	{	
		IsoTyp = 2;
	}

	if ( m_Radio_IsoType == 2 ) // Table
	{	
		IsoTyp = 3;
	}
}

void MutSolvDlg::OnCheckMAdso() 
{
	if ( m_Check_MAdso.GetCheck() )
	{
		GetDlgItem(IDC_EDIT_ADS1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ADS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MUT_ADS1)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MUT_ADS2)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_FREU)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_LANG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_IRR)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_TABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_MODADD)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MAX_ADS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAX_ADS)->EnableWindow(TRUE);
	}
	else
	{	
		GetDlgItem(IDC_EDIT_ADS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ADS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MUT_ADS1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MUT_ADS2)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_FREU)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_LANG)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_IRR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TABLE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_MODADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MAX_ADS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_ADS)->EnableWindow(FALSE);
	}
}

void MutSolvDlg::OnRadioFreundlich() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_MUT_ADS1)->SetWindowText("k");
	GetDlgItem(IDC_STATIC_MUT_ADS2)->SetWindowText("n");
	m_Radio_IsoType = 0;
}

void MutSolvDlg::OnRadioLangmuir() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_MUT_ADS1)->SetWindowText("a");
	GetDlgItem(IDC_STATIC_MUT_ADS2)->SetWindowText("b");
	m_Radio_IsoType = 1;
}

void MutSolvDlg::OnRadioTable() 
{
	// TODO: Add your control notification handler code here
	CString openFilter = "Adsorption Isotherm Table (*.txt)|*.txt||";
	
	CFileDialog openDialog(TRUE, _T("txt"), PathName + CString("Isotherms\\TableAdso.txt"), OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{
		m_Table.SetWindowText(openDialog.GetFileName());
	}
	
	m_Radio_IsoType = 2;
	
}

void MutSolvDlg::OnCheckIrr() 
{
	if ( m_Check_Irr.GetCheck() )
	{
		Irr = 1;
	}	
	else
	{
		Irr = 0;
	}
}

void MutSolvDlg::OnCheckModadd() 
{
	if ( m_Check_ModAdd.GetCheck() )
	{
		ModAdd = 1;
		GetDlgItem(IDC_STATIC_MAX_ADS)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MAX_ADS)->ShowWindow(TRUE);
	}	
	else
	{
		ModAdd = 0;
		GetDlgItem(IDC_STATIC_MAX_ADS)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MAX_ADS)->ShowWindow(FALSE);
	}
	
}





