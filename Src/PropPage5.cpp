// PropPage5.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropPage5 property page

IMPLEMENT_DYNCREATE(PropPage5, CPropertyPage)

PropPage5::PropPage5() : CPropertyPage(PropPage5::IDD)
{
	//{{AFX_DATA_INIT(PropPage5)
	m_Kiso = -1;
	int m_nSpace = -1;
	//}}AFX_DATA_INIT
}

PropPage5::~PropPage5()
{
}

void PropPage5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropPage5)
	DDX_Radio(pDX, IDC_RADIO_NK, m_Kiso);
	DDX_Radio(pDX, IDC_RADIO_LIN, m_nSpace);
	DDX_Control(pDX, IDC_STATIC_PHI, m_Edit_Phi);
	DDX_Control(pDX, IDC_STATIC_CI, m_Edit_CI);
	DDX_Control(pDX, IDC_STATIC_VOLINJE, m_Edit_VolInj);
	DDX_Control(pDX, IDC_STATIC_VOLOVEE, m_Edit_VolOve);
	DDX_Control(pDX, IDC_STATIC_FNE, m_Edit_N);
	DDX_Control(pDX, IDC_STATIC_FKE, m_Edit_K);
	DDX_Control(pDX, IDC_STATIC_CSEE, m_Edit_CSE);
	DDX_Control(pDX, IDC_STATIC_MINVOLE, m_Edit_MinVol);
	DDX_Control(pDX, IDC_STATIC_MAXVOLE, m_Edit_MaxVol);
	DDX_Control(pDX, IDC_STATIC_NUMRET, m_Edit_NumRet);
	DDX_Control(pDX, IDC_STATIC_MAXITER, m_Edit_MaxIter);
	DDX_Control(pDX, IDC_STATIC_TOL, m_Edit_Tol);
	DDX_Control(pDX, IDC_STATIC_CLIFE1, m_Edit_CLife[0]);
	DDX_Control(pDX, IDC_STATIC_CLIFE2, m_Edit_CLife[1]);
	DDX_Control(pDX, IDC_STATIC_CLIFE3, m_Edit_CLife[2]);
	DDX_Control(pDX, IDC_STATIC_CLIFE4, m_Edit_CLife[3]);
	DDX_Control(pDX, IDC_STATIC_CLIFE5, m_Edit_CLife[4]);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropPage5, CPropertyPage)
	//{{AFX_MSG_MAP(PropPage5)
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE, OnListCtrlUpdate)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_BN_CLICKED(IDC_RADIO_NK, OnNK)
	ON_BN_CLICKED(IDC_RADIO_NCSE, OnCSE)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropPage5 message handlers
BOOL PropPage5::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT PropPage5::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
	UpdateFromEdits();
	return 0;
}

LRESULT PropPage5::OnEditLostFocus(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	UpdateFromEdits();
	return 0;
}

void PropPage5::UpdateControls()
{	
	int i;

	CString sn;
	
	sn.Format("%.3g", AnLinModel.Phi);
	m_Edit_Phi.sEdit = sn;
	
	sn.Format("%.6g", AnLinModel.InhCon);
	m_Edit_CI.sEdit = sn;
	
	sn.Format("%.6g", AnLinModel.VolInj);
	m_Edit_VolInj.sEdit = sn;

	sn.Format("%.6g", AnLinModel.VolOve);
	m_Edit_VolOve.sEdit = sn;

	sn.Format("%.6g", AnLinModel.xn);
	m_Edit_N.sEdit = sn;

	sn.Format("%.6g", AnLinModel.xk);
	m_Edit_K.sEdit = sn;

	sn.Format("%.6g", AnLinModel.CSE);
	m_Edit_CSE.sEdit = sn;

	sn.Format("%.6g", AnLinModel.VolMin);
	m_Edit_MinVol.sEdit = sn;

	sn.Format("%.6g", AnLinModel.VolMax);
	m_Edit_MaxVol.sEdit = sn;

	sn.Format("%d", AnLinModel.nPtc);
	m_Edit_NumRet.sEdit = sn;

	sn.Format("%d", AnLinModel.nItMax);
	m_Edit_MaxIter.sEdit = sn;

	sn.Format("%.3g", AnLinModel.Tol);
	m_Edit_Tol.sEdit = sn;

	for( i = 0 ; i < AnLinModel.nLife ; i++ )
	{
		sn.Format("%.6g", AnLinModel.Clife[i]);
		m_Edit_CLife[i].sEdit = sn;
	}

	if ( m_Kiso == 0 )
	
		OnNK();

	if ( m_Kiso == 1 )

		OnCSE();
}

void PropPage5::UpdateFromEdits()
{
	int j;

	bool Check = TRUE;
	
	AnLinModel.Phi = atof(m_Edit_Phi.sEdit);
	AnLinModel.InhCon = atof(m_Edit_CI.sEdit);
	AnLinModel.VolInj = atof(m_Edit_VolInj.sEdit);
	AnLinModel.VolOve = atof(m_Edit_VolOve.sEdit);
	AnLinModel.xn = atof(m_Edit_N.sEdit);
	AnLinModel.xk = atof(m_Edit_K.sEdit);
	AnLinModel.CSE = atof(m_Edit_CSE.sEdit);
	AnLinModel.VolMin = atof(m_Edit_MinVol.sEdit);
	AnLinModel.VolMax = atof(m_Edit_MaxVol.sEdit);
	AnLinModel.nPtc = atof(m_Edit_NumRet.sEdit);
	AnLinModel.nItMax = atof(m_Edit_MaxIter.sEdit);
	AnLinModel.Tol = atof(m_Edit_Tol.sEdit);

	AnLinModel.Clife[0] = atof(m_Edit_CLife[0].sEdit);
	
	AnLinModel.Clife[1] = atof(m_Edit_CLife[1].sEdit);
	
	AnLinModel.Clife[2] = atof(m_Edit_CLife[2].sEdit);
	
	AnLinModel.Clife[3] = atof(m_Edit_CLife[3].sEdit);
	
	AnLinModel.Clife[4] = atof(m_Edit_CLife[4].sEdit);
	
	AnLinModel.nLife = 0;
	
	j = 0;

	while( Check && j < MaxClife )
	{
		if( m_Edit_CLife[j].sEdit != "" )
		
			++AnLinModel.nLife;

		else Check = FALSE;

		j++;
	}
}

void PropPage5::OnNK()
{	
	m_Kiso = 0;

	GetDlgItem(IDC_STATIC_CSE)->EnableWindow(FALSE);
	m_Edit_CSE.cellEdit = FALSE;
	
	GetDlgItem(IDC_STATIC_FKE)->EnableWindow(TRUE);
	m_Edit_K.cellEdit = TRUE;
	
	Invalidate(NULL);
}

void PropPage5::OnCSE()
{	
	m_Kiso = 1;

	GetDlgItem(IDC_STATIC_CSE)->EnableWindow(TRUE);
	m_Edit_CSE.cellEdit = TRUE;
	
	GetDlgItem(IDC_STATIC_FKE)->EnableWindow(FALSE);
	m_Edit_K.cellEdit = FALSE;
	
	Invalidate(NULL);
}