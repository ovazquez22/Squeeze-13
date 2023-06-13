// CaNaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "CaNaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CaNaDlg dialog


CaNaDlg::CaNaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CaNaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CaNaDlg)
	m_Radio_IsoType = -1;
	IsoTyp = 1;
	Low_Ads1 = Low_Ads2 = High_Ads1 = High_Ads2 = CacoreConc = NacoreConc = 1;
	Low_Na = 0;
	High_Na = 100;
	//}}AFX_DATA_INIT
}


void CaNaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CaNaDlg)
	DDX_Control(pDX, IDC_EDIT_LOWADS1, m_Low_Ads1);
	DDX_Control(pDX, IDC_EDIT_LOWADS2, m_Low_Ads2);
	DDX_Control(pDX, IDC_EDIT_HIGHADS1, m_High_Ads1);
	DDX_Control(pDX, IDC_EDIT_HIGHADS2, m_High_Ads2);
	DDX_Control(pDX, IDC_EDIT_CA_CONC, m_Ca_coreConc);
	DDX_Control(pDX, IDC_EDIT_NA_CONC, m_Na_coreConc);
	DDX_Control(pDX, IDC_EDIT_LOW_NA, m_Low_Na);
	DDX_Control(pDX, IDC_EDIT_HIGH_NA, m_High_Na);
	DDX_Control(pDX, IDC_RADIO_FREUNDLICH, m_Freundlich);
	DDX_Control(pDX, IDC_RADIO_LANGMUIR, m_Langmuir);
	DDX_Radio(pDX, IDC_RADIO_FREUNDLICH, m_Radio_IsoType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CaNaDlg, CDialog)
	//{{AFX_MSG_MAP(CaNaDlg)
	ON_BN_CLICKED(IDC_RADIO_FREUNDLICH, OnRadioFreundlich)
	ON_BN_CLICKED(IDC_RADIO_LANGMUIR, OnRadioLangmuir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CaNaDlg message handlers

BOOL CaNaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CaNaDlg::UpdateControls()
{
	CString sn;
	
	if ( IsoTyp == 1 ) // Freundlich
	{	
		m_Radio_IsoType = 0;
		m_Freundlich.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_STATIC_LOWADS1)->SetWindowText("k");
		GetDlgItem(IDC_STATIC_LOWADS2)->SetWindowText("n");
		GetDlgItem(IDC_STATIC_HIGHADS1)->SetWindowText("k");
		GetDlgItem(IDC_STATIC_HIGHADS2)->SetWindowText("n");
	}
	
	if ( IsoTyp == 2 ) // Langmuir
	{	
		m_Radio_IsoType = 1;
		m_Langmuir.SetCheck( BST_CHECKED );

		GetDlgItem(IDC_STATIC_LOWADS1)->SetWindowText("a");
		GetDlgItem(IDC_STATIC_LOWADS2)->SetWindowText("b");
		GetDlgItem(IDC_STATIC_HIGHADS1)->SetWindowText("a");
		GetDlgItem(IDC_STATIC_HIGHADS2)->SetWindowText("b");
	}
	
	sn.Format("%.5g", Low_Ads1);
	m_Low_Ads1.SetWindowText(sn);
	
	sn.Format("%.5g", Low_Ads2);
	m_Low_Ads2.SetWindowText(sn);

	sn.Format("%.5g", High_Ads1);
	m_High_Ads1.SetWindowText(sn);

	sn.Format("%.5g", High_Ads2);
	m_High_Ads2.SetWindowText(sn);

	sn.Format("%.5g", CacoreConc);
	m_Ca_coreConc.SetWindowText(sn);
			 
	sn.Format("%.5g", NacoreConc);
	m_Na_coreConc.SetWindowText(sn);
	
	sn.Format("%.5g", Low_Na);
	m_Low_Na.SetWindowText(sn);
			 
	sn.Format("%.5g", High_Na);
	m_High_Na.SetWindowText(sn);
}

void CaNaDlg::UpdateEdits()
{
	CString sn;

	if ( m_Radio_IsoType == 0 ) // Freundlich
	{	
		IsoTyp = 1;
	}
	
	if ( m_Radio_IsoType == 1 ) // Langmuir
	{	
		IsoTyp = 2;
	}
	
	m_Low_Ads1.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Low_Ads1 = atof(sn);
	}

	m_Low_Ads2.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Low_Ads2 = atof(sn);
	}

	m_High_Ads1.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		High_Ads1 = atof(sn);
	}

	m_High_Ads2.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		High_Ads2 = atof(sn);
	}

	m_Ca_coreConc.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		CacoreConc = atof(sn);
	}		 

	m_Na_coreConc.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		NacoreConc = atof(sn);
	}	

	m_Low_Na.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		Low_Na = atof(sn);
	}	

	m_High_Na.GetWindowText(sn);
	if (sn.IsEmpty() == FALSE)
	{
		sn.TrimRight();
		High_Na = atof(sn);
	}
}

void CaNaDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateEdits();
	CDialog::OnOK();
}

void CaNaDlg::OnRadioFreundlich() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_LOWADS1)->SetWindowText("k");
	GetDlgItem(IDC_STATIC_LOWADS2)->SetWindowText("n");
	GetDlgItem(IDC_STATIC_HIGHADS1)->SetWindowText("k");
	GetDlgItem(IDC_STATIC_HIGHADS2)->SetWindowText("n");
}

void CaNaDlg::OnRadioLangmuir() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_LOWADS1)->SetWindowText("a");
	GetDlgItem(IDC_STATIC_LOWADS2)->SetWindowText("b");
	GetDlgItem(IDC_STATIC_HIGHADS1)->SetWindowText("a");
	GetDlgItem(IDC_STATIC_HIGHADS2)->SetWindowText("b");
}
