// Basic2Phase.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Basic2Phase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Basic2Phase dialog


Basic2Phase::Basic2Phase(CWnd* pParent /*=NULL*/)
	: CDialog(Basic2Phase::IDD, pParent)
{
	//{{AFX_DATA_INIT(Basic2Phase)
	m_Units = -1;
	m_ModelType = 0;
	m_Radio_Simulation = -1;
	CShut = VolInj = TotVol = InhCon = 10.0;
	Rho = 2.65;
	m_ModelType = 0;
	nB = 10;
	m_Radio_Simulation = 0;
	//}}AFX_DATA_INIT
}

Basic2Phase::~Basic2Phase()
{

}

void Basic2Phase::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Basic2Phase)
	DDX_Control(pDX, IDC_EDIT_FLUIDRHO, m_Edit_FluidRho);
	DDX_Control(pDX, IDC_STATIC_FLUIDRHO, m_Static_FluidRho);
	DDX_Control(pDX, IDC_EDIT_WELLDIA, m_Edit_WellDia);
	DDX_Control(pDX, IDC_EDIT_TUBEROUGH, m_Edit_TubeRough);
	DDX_Control(pDX, IDC_EDIT_FORMTEMP, m_Edit_FormTemp);
	DDX_Control(pDX, IDC_EDIT_AQHECAP, m_Edit_AqHeCap);
	DDX_Control(pDX, IDC_EDIT_NAHECAP, m_Edit_NAHeCap);
	DDX_Control(pDX, IDC_EDIT_FORMTHCOND, m_Edit_FormThCond);
	DDX_Control(pDX, IDC_EDIT_THCONDRES, m_Edit_ThCondRes);
	DDX_Control(pDX, IDC_EDIT_ROGRDEN, m_Edit_RoGrDen);
	DDX_Control(pDX, IDC_EDIT_AQPHDEN, m_Edit_AqPhDen);
	DDX_Control(pDX, IDC_EDIT_NAPHDEN, m_Edit_NAPhDen);
	DDX_Control(pDX, IDC_EDIT_ROHECAP, m_Edit_RoHeCap);
	DDX_Control(pDX, IDC_STATIC_WELLDIA, m_Static_WellDia);
	DDX_Control(pDX, IDC_STATIC_TUBEROUGH, m_Static_TubeRough);
	DDX_Control(pDX, IDC_VOL_INJ, m_VolInj);
	DDX_Control(pDX, IDC_TOT_VOL, m_TotVol);
	DDX_Control(pDX, IDC_RHO, m_Rho);
	DDX_Control(pDX, IDC_INH_CON, m_InhCon);
	DDX_Control(pDX, IDC_CSHUT, m_CShut);
	DDX_Control(pDX, IDC_STATIC_VI, m_Static_VolInj);
	DDX_Control(pDX, IDC_STATIC_TDV, m_Static_TotVol);
	DDX_Control(pDX, IDC_STATIC_IHC, m_Static_InhCon);
	DDX_Control(pDX, IDC_STATIC_RHO, m_Static_Rho);
	DDX_Control(pDX, IDC_STATIC_CSHUT, m_Static_CShut);
	DDX_Control(pDX, IDC_EDIT_TITLE1, m_EditTitle1);
	DDX_Control(pDX, IDC_EDIT_TITLE2, m_EditTitle2);
	DDX_Control(pDX, IDC_METRIC, m_Metric);
	DDX_Control(pDX, IDC_CONVENTIONAL, m_Conventional);
	DDX_Control(pDX, IDC_SINGLE_PHASE, m_SinglePhase);
	DDX_Control(pDX, IDC_TWO_PHASE, m_TwoPhase);
	DDX_Control(pDX, IDC_NUM_COMP, m_Num_Comp);
	DDX_Control(pDX, IDC_NUM_STAGES, m_Num_Stages);
	DDX_Control(pDX, IDC_NUM_LAYERS, m_Num_Layers);
	DDX_Control(pDX, IDC_NUM_BLOCKS, m_Num_Blocks);
	DDX_Radio(pDX, IDC_METRIC, m_Units);
	DDX_Radio(pDX, IDC_RADIAL, m_ModelType);
	DDX_Radio(pDX, IDC_SINGLE_PHASE, m_Radio_Simulation);
	DDX_Control(pDX, IDC_CHECK_WELLFRICTION, m_Check_WellFriction);
	DDX_Control(pDX, IDC_CHECK_TEMPERATURE, m_Check_Temperature);
	DDX_Control(pDX, IDC_CHECK_WAT_VISC, m_Check_WatTemp);
	DDX_Control(pDX, IDC_CHECK_OIL_VISC, m_Check_OilTemp);
	DDX_Control(pDX, IDC_EDIT_OILAPI, m_API_Oil);
	DDX_Control(pDX, IDC_STATIC_LAYER_SPACING, LaySpacingGrid);
	DDX_Control(pDX, IDC_STATIC_FORMTEMP, m_Static_FormTemp);
	DDX_Control(pDX, IDC_STATIC_FORMTHCOND, m_Static_FormThCond);
	DDX_Control(pDX, IDC_STATIC_THCONDRES, m_Static_ThCondRes);
	DDX_Control(pDX, IDC_STATIC_AQHECAP, m_Static_AqHeCap);
	DDX_Control(pDX, IDC_STATIC_ROHECAP, m_Static_RoHeCap);
	DDX_Control(pDX, IDC_STATIC_AQPHDEN, m_Static_AqPhDen);
	DDX_Control(pDX, IDC_STATIC_ROGRDEN, m_Static_RoGrDen);
	DDX_Control(pDX, IDC_STATIC_ISOTABLE, m_Edit_IsoTable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Basic2Phase, CDialog)
	//{{AFX_MSG_MAP(Basic2Phase)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_RADIAL, OnRadial)
	ON_BN_CLICKED(IDC_LINEAR, OnLinear)
	ON_BN_CLICKED(IDC_ISOTHERM, OnIsotherm)
	ON_BN_CLICKED(IDC_ISOTHERM_DER, OnIsothermDerivation)
	ON_BN_CLICKED(IDC_ANAL_LINEAR, OnAnalitical)
	ON_BN_CLICKED(IDC_METRIC, OnMetric)
	ON_BN_CLICKED(IDC_CONVENTIONAL, OnConventional)
	ON_BN_CLICKED(IDC_CHECK_WELLFRICTION, OnCheckWellfriction)
	ON_BN_CLICKED(IDC_CHECK_TEMPERATURE, OnCheckTemperature)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_EN_KILLFOCUS(IDC_NUM_LAYERS, OnKillfocusNumLayers)
	ON_BN_CLICKED(IDC_INPUT_EFFLUENT, OnInputEffluentData)
	ON_BN_CLICKED(IDC_SHOW_DATA, OnShowData)
	ON_BN_CLICKED(IDC_SINGLE_PHASE, OnSinglePhase)
	ON_BN_CLICKED(IDC_TWO_PHASE, OnTwoPhase)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &Basic2Phase::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_WAT_VISC, &Basic2Phase::OnBnClickedCheckWatVisc)
	ON_BN_CLICKED(IDC_CHECK_OIL_VISC, &Basic2Phase::OnBnClickedCheckOilVisc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Basic2Phase message handlers


void Basic2Phase::OnInputEffluentData() 
{	
	m_Edit_IsoTable.FieldTable.SetPathName(m_Edit_IsoTable.PathName);
	
	CString openFilter = "Lab Data (*.txt)|*.txt||";
	
	CFileDialog openDialog(TRUE, "txt", m_Edit_IsoTable.PathName + CString("Lab Data\\LabData.txt"), OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST, openFilter, this);
	
	if (openDialog.DoModal() == IDOK )
	{
		m_Edit_IsoTable.FTFileName = openDialog.GetFileName();
		
		m_Edit_IsoTable.sEdit = m_Edit_IsoTable.FTFileName;

		Invalidate(FALSE);

		UpdateFromEdits();
	}
}

void Basic2Phase::OnShowData() 
{	
	// TODO: Add your control notification handler code here
	m_Edit_IsoTable.FieldTable.SetPathName(m_Edit_IsoTable.PathName);
	
	m_Edit_IsoTable.FieldTable.ReadLabDataTable(m_Edit_IsoTable.FTFileName);
	
	m_Edit_IsoTable.FieldTable.TabGr.SetRowFormat(0, TableGrid::HEADER);
	m_Edit_IsoTable.FieldTable.TabGr.SetRowEdit(0, TableGrid::READ_ONLY);
	
	m_Edit_IsoTable.FieldTable.TabGr.colS[0][0] = "InjVol\n(pv)";
	m_Edit_IsoTable.FieldTable.TabGr.colS[1][0] = "Conc\n(ppm)";
	
	m_Edit_IsoTable.FieldTable.TabGr.rowHeight[0] = 35;
	
	m_Edit_IsoTable.FieldTable.bIsoDer = TRUE;

	if ( m_Edit_IsoTable.FieldTable.DoModal() == IDOK 
		|| m_Edit_IsoTable.FieldTable.DoModal() == IDSAVE )
	{	
		m_Edit_IsoTable.FTFileName = m_Edit_IsoTable.FieldTable.NameFile;
		
		m_Edit_IsoTable.sEdit = m_Edit_IsoTable.FTFileName;

		Invalidate(FALSE);

		UpdateFromEdits();
	}
}

void Basic2Phase::OnCancel() 
{
	// TODO: Add your control notification handler code here
	check = false;
	CDialog::OnOK();
}

BOOL Basic2Phase::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if ( m_Radio_Simulation == 1 )
	{	
	//	GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);
		m_Check_WellFriction.EnableWindow(TRUE);

		// Model types
		GetDlgItem(IDC_ISOTHERM)->EnableWindow(FALSE);
		GetDlgItem(IDC_ISOTHERM_DER)->EnableWindow(FALSE);
		GetDlgItem(IDC_ANAL_LINEAR)->EnableWindow(FALSE);
	}		

	if ( m_Radio_Simulation == 0 )
	{	
		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(TRUE);
		m_Check_WellFriction.EnableWindow(FALSE);

		// Model types
		GetDlgItem(IDC_ISOTHERM)->EnableWindow(TRUE);
		GetDlgItem(IDC_ISOTHERM_DER)->EnableWindow(TRUE);
		GetDlgItem(IDC_ANAL_LINEAR)->EnableWindow(TRUE);
	}

	if (m_ModelType == 0)
	{	
		// Isotherm
		m_Static_CShut.EnableWindow(FALSE);
		m_Static_VolInj.EnableWindow(FALSE);
		m_Static_TotVol.EnableWindow(FALSE);
		m_Static_InhCon.EnableWindow(FALSE);
		m_Static_Rho.EnableWindow(FALSE);
		
		m_CShut.EnableWindow(FALSE);
		m_VolInj.EnableWindow(FALSE);
		m_TotVol.EnableWindow(FALSE);
		m_InhCon.EnableWindow(FALSE);
		m_Rho.EnableWindow(FALSE);

		GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
		m_Edit_IsoTable.cellEdit = FALSE;
	}
	
	if (m_ModelType == 1)
	{	
		m_Num_Layers.EnableWindow(FALSE);
		m_Conventional.EnableWindow(FALSE);
		m_Metric.EnableWindow(FALSE);

		// Isotherm
		m_Static_CShut.EnableWindow(FALSE);
		m_Static_VolInj.EnableWindow(FALSE);
		m_Static_TotVol.EnableWindow(FALSE);
		m_Static_InhCon.EnableWindow(FALSE);
		m_Static_Rho.EnableWindow(FALSE);
		
		m_CShut.EnableWindow(FALSE);
		m_VolInj.EnableWindow(FALSE);
		m_TotVol.EnableWindow(FALSE);
		m_InhCon.EnableWindow(FALSE);
		m_Rho.EnableWindow(FALSE);

		GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
		m_Edit_IsoTable.cellEdit = FALSE;

	}

	if (m_ModelType == 2)
	{
		m_Num_Layers.EnableWindow(FALSE);
		m_Num_Comp.EnableWindow(FALSE);
		m_Conventional.EnableWindow(FALSE);
		m_Metric.EnableWindow(FALSE);
		
		m_TwoPhase.EnableWindow(FALSE);
		m_Radio_Simulation = 0;
		
		GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHOW_DATA)->EnableWindow(TRUE);
		m_Edit_IsoTable.cellEdit = TRUE;
	}
	
	if (m_ModelType == 3)
	{
		m_Num_Layers.EnableWindow(FALSE);
		m_Num_Comp.EnableWindow(FALSE);
		m_Conventional.EnableWindow(FALSE);
		m_Metric.EnableWindow(FALSE);
		
		m_Radio_Simulation = 0;		
		m_TwoPhase.EnableWindow(FALSE);

		GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHOW_DATA)->EnableWindow(TRUE);
		m_Edit_IsoTable.cellEdit = TRUE;
	}
	
	if (m_ModelType == 4)
	{
		m_Num_Layers.EnableWindow(FALSE);
		m_Num_Comp.EnableWindow(FALSE);
		m_Num_Stages.EnableWindow(FALSE);
		m_Num_Blocks.EnableWindow(FALSE);

		m_Conventional.EnableWindow(TRUE);
		m_Metric.EnableWindow(TRUE);

		// Isotherm
		m_Static_CShut.EnableWindow(FALSE);
		m_Static_VolInj.EnableWindow(FALSE);
		m_Static_TotVol.EnableWindow(FALSE);
		m_Static_InhCon.EnableWindow(FALSE);
		m_Static_Rho.EnableWindow(FALSE);
		
		m_CShut.EnableWindow(FALSE);
		m_VolInj.EnableWindow(FALSE);
		m_TotVol.EnableWindow(FALSE);
		m_InhCon.EnableWindow(FALSE);
		m_Rho.EnableWindow(FALSE);
		
		GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
		m_Edit_IsoTable.cellEdit = FALSE;

		// Well Friction
		m_Check_WellFriction.EnableWindow(FALSE);
		m_Check_WellFriction.SetCheck(FALSE);
		
		// Temperature Calculations
		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);

		// Simulation Type
		m_TwoPhase.EnableWindow(FALSE);
		m_TwoPhase.SetCheck(FALSE);
		m_Radio_Simulation = 0;
		m_SinglePhase.SetCheck(TRUE);
	}
	
	m_Check_Temperature.SetCheck(nTemp);

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Basic2Phase::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void Basic2Phase::UpdateControls()
{
	CString sn;
	
//	EditTitle1.Remove('\n');
//	EditTitle2.Remove('\n');
	m_EditTitle1.SetWindowText(EditTitle1);
	m_EditTitle2.SetWindowText(EditTitle2);

	sn.Format("%d", nB);
	m_Num_Blocks.SetWindowText(sn);
	
	nLay = (m_ModelType ? 1 : nLay);
	sn.Format("%d", nLay);
	m_Num_Layers.SetWindowText(sn);
	
	
	if ( m_Radio_Simulation == 0 )
	{
		nComp = 1;
		m_Num_Comp.EnableWindow(FALSE);
	}
	else
	{
		nComp = nComp;
		m_Num_Comp.EnableWindow(TRUE);
	}

	sn.Format("%d", nComp);
	m_Num_Comp.SetWindowText(sn);
	
	sn.Format("%d", nSt);
	m_Num_Stages.SetWindowText(sn);
	
	sn.Format("%.5g", CShut);
	m_CShut.SetWindowText(sn);
	
	sn.Format("%.5g", VolInj);
	m_VolInj.SetWindowText(sn);
	
	sn.Format("%.5g", TotVol);
	m_TotVol.SetWindowText(sn);
	
	sn.Format("%.5g", Rho);
	m_Rho.SetWindowText(sn);
	
	sn.Format("%.5g", InhCon);
	m_InhCon.SetWindowText(sn);
	
	m_Edit_IsoTable.sEdit = m_Edit_IsoTable.FTFileName;

	sn.Format("%.6g", WellDia);
	m_Edit_WellDia.SetWindowText(sn);

	sn.Format("%.6g", e);
	m_Edit_TubeRough.SetWindowText(sn);
	
	sn.Format("%.6g", FluidRho);
	m_Edit_FluidRho.SetWindowText(sn);

	sn = "Tubing Inner Diameter" + CString((m_Units) ? ", ft" : ", m");
	GetDlgItem(IDC_STATIC_WELLDIA)->SetWindowText(sn);
	
	sn = "Tubing Absolute Roughness" + CString((m_Units) ? ", ft" : ", m");
	GetDlgItem(IDC_STATIC_TUBEROUGH)->SetWindowText(sn);
	
	sn = "Fluid Density" + CString((m_Units) ? ", lb/ft3" : ", kg/m3");
	GetDlgItem(IDC_STATIC_FLUIDRHO)->SetWindowText(sn);

	sn = "Formation Temperature Before Squeeze Treatment, degC";
   GetDlgItem(IDC_STATIC_FORMTEMP)->SetWindowText(sn);

   sn = "Thermal Conductivity of Reservoir" + CString((m_Units) ? ", Btu/ft-day-degC" : ", Btu/m-day-degC");
   GetDlgItem(IDC_STATIC_THCONDRES)->SetWindowText(sn);
	
   sn = "Formation Underburden/Overburden Thermal Conductivity" + CString((m_Units) ? ", Btu/ft-day-degC" : ", Btu/m-day-degC");
   GetDlgItem(IDC_STATIC_FORMTHCOND)->SetWindowText(sn);
   
	sn = "Rock Heat Capacity" + CString((m_Units) ? ", Btu/lb-degC" : ", Btu/Kg-degC");
	GetDlgItem(IDC_STATIC_ROHECAP)->SetWindowText(sn);
   
	sn = "Aqueous Heat Capacity" + CString((m_Units) ? ", Btu/lb-degC" : ", Btu/Kg-degC");
	GetDlgItem(IDC_STATIC_AQHECAP)->SetWindowText(sn);
   
	sn = "Aqueous Phase Density" + CString((m_Units) ? ", lb/ft3" : ", lb/m3");
	GetDlgItem(IDC_STATIC_AQPHDEN)->SetWindowText(sn);
	
	sn = "Rock Grain Density" + CString((m_Units) ? ", lb/ft3" : ", lb/m3");
	GetDlgItem(IDC_STATIC_ROGRDEN)->SetWindowText(sn);

	sn.Format("%.6g", Ftemp);
	m_Edit_FormTemp.SetWindowText(sn);
	
	sn.Format("%.6g", SHw);
	m_Edit_AqHeCap.SetWindowText(sn);
	
	sn.Format("%.6g", SHo);
	m_Edit_NAHeCap.SetWindowText(sn);

	sn.Format("%.6g", Kcs);
	m_Edit_FormThCond.SetWindowText(sn);
	
	sn.Format("%.6g", Kcr);
	m_Edit_ThCondRes.SetWindowText(sn);

	sn.Format("%.6g", RHOr);
	m_Edit_RoGrDen.SetWindowText(sn);

	sn.Format("%.6g", RHOw);
	m_Edit_AqPhDen.SetWindowText(sn);

	sn.Format("%.6g", RHOo);
	m_Edit_NAPhDen.SetWindowText(sn);

	sn.Format("%.6g", SHr);
	m_Edit_RoHeCap.SetWindowText(sn);

	sn.Format("%.6g", API);
	m_API_Oil.SetWindowText(sn);

	if(m_Units) unit = TRUE;

	else unit = FALSE;

	if ( nWF )
	{
		m_Check_WellFriction.SetCheck( BST_CHECKED );
		m_Check_WellFriction.EnableWindow(TRUE);

		m_Static_WellDia.EnableWindow(TRUE);
		m_Edit_WellDia.EnableWindow(TRUE);

		m_Static_TubeRough.EnableWindow(TRUE);
		m_Edit_TubeRough.EnableWindow(TRUE);

		m_Static_FluidRho.EnableWindow(TRUE);
		m_Edit_FluidRho.EnableWindow(TRUE);

		LaySpacingGridDraw();
	}
	else
	{
		m_Check_WellFriction.SetCheck( BST_UNCHECKED); 

		m_Static_WellDia.EnableWindow(FALSE);
		m_Edit_WellDia.EnableWindow(FALSE);

		m_Static_TubeRough.EnableWindow(FALSE);
		m_Edit_TubeRough.EnableWindow(FALSE);

		m_Static_FluidRho.EnableWindow(FALSE);
		m_Edit_FluidRho.EnableWindow(FALSE);

		LaySpacingGrid.Disabled();
	}
	
	if ( m_Radio_Simulation == 1 && nLay > 1 )
	
		m_Check_WellFriction.EnableWindow(TRUE);

	nTemp = m_Check_Temperature.GetCheck();

	if ( nTemp )
	{	
		m_Check_Temperature.SetCheck( BST_CHECKED );
		
		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(TRUE);  
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(TRUE);

		if (m_Radio_Simulation)
		{
			GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(TRUE);

			GetDlgItem(IDC_CHECK_WAT_VISC)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_OIL_VISC)->EnableWindow(TRUE);

			if (watTemp == 0)
				m_Check_WatTemp.SetCheck(FALSE);
			else
				m_Check_WatTemp.SetCheck(TRUE);

			if (oilTemp == 0)
			{
				m_Check_OilTemp.SetCheck(FALSE);
				GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);
			}
			else
			{
				m_Check_OilTemp.SetCheck(TRUE);
				GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(TRUE);
			}

			m_Edit_NAPhDen.EnableWindow(TRUE);
			m_Edit_NAHeCap.EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(FALSE);
			GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(FALSE);

			GetDlgItem(IDC_CHECK_WAT_VISC)->EnableWindow(FALSE);
			m_Check_WatTemp.SetCheck(FALSE);
			GetDlgItem(IDC_CHECK_OIL_VISC)->EnableWindow(FALSE);
			m_Check_OilTemp.SetCheck(FALSE);
			GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);
		}

		m_Edit_FormTemp.EnableWindow(TRUE);
		m_Edit_AqHeCap.EnableWindow(TRUE);
		m_Edit_FormThCond.EnableWindow(TRUE);
		m_Edit_ThCondRes.EnableWindow(TRUE);
		m_Edit_RoGrDen.EnableWindow(TRUE);
		m_Edit_AqPhDen.EnableWindow(TRUE);
		m_Edit_RoHeCap.EnableWindow(TRUE);
		
	}
	else
	{	
	//	m_Check_Temperature.SetCheck( BST_UNCHECKED );

		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_ROGRDEN)->EnableWindow(FALSE);  
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(FALSE); 

		GetDlgItem(IDC_CHECK_WAT_VISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_OIL_VISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);

		m_Edit_FormTemp.EnableWindow(FALSE);
		m_Edit_AqHeCap.EnableWindow(FALSE);
		m_Edit_FormThCond.EnableWindow(FALSE);
		m_Edit_ThCondRes.EnableWindow(FALSE);
		m_Edit_RoGrDen.EnableWindow(FALSE);
		m_Edit_AqPhDen.EnableWindow(FALSE);
		m_Edit_RoHeCap.EnableWindow(FALSE);
		m_Edit_NAPhDen.EnableWindow(FALSE);
		m_Edit_NAHeCap.EnableWindow(FALSE);
	}
}

void Basic2Phase::UpdateFromEdits()
{
	CString sEdit;
	int n;

	double Val;

	m_EditTitle1.GetWindowText(EditTitle1);
	m_EditTitle2.GetWindowText(EditTitle2);

	m_Num_Blocks.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 20000)) { nB = n; }
	}

	m_Num_Layers.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 50))
		{ nLay = n; }
	}

	m_Num_Comp.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 50))
		{ nComp = n; }
	}
	
	m_Num_Stages.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 500))
		{ nSt = n; }
	}

	if( m_ModelType || m_ModelType == 2)
		nLay = 1;

	m_VolInj.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Val = atof(sEdit);
		VolInj = Val;
	}

	m_TotVol.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Val = atof(sEdit);
		TotVol = Val;
	}

	m_Rho.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Val = atof(sEdit);
		Rho = Val;
	}

	m_InhCon.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Val = atof(sEdit);
		InhCon = Val;
	}

	m_CShut.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Val = atof(sEdit);
		CShut = Val;
	}

	m_Edit_WellDia.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		WellDia = atof(sEdit);
	}

	m_Edit_TubeRough.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		e = atof(sEdit);
	}
	
	m_Edit_FluidRho.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		FluidRho = atof(sEdit);
	}
	
	m_Edit_FormTemp.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Ftemp = atof(sEdit);
	}

	m_Edit_AqHeCap.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		SHw = atof(sEdit);
	}

	m_Edit_NAHeCap.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		SHo = atof(sEdit);
	}

	m_Edit_FormThCond.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Kcs = atof(sEdit);
	}

	m_Edit_ThCondRes.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		Kcr = atof(sEdit);
	}
	m_Edit_RoGrDen.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		RHOr = atof(sEdit);
	}

	m_Edit_AqPhDen.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		RHOw = atof(sEdit);
	}
	
	m_Edit_NAPhDen.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		RHOo = atof(sEdit);
	}

	m_Edit_RoHeCap.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		SHr = atof(sEdit);
	}

	m_API_Oil.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		API = atof(sEdit);
	}
	
	if ( m_Check_WellFriction.GetCheck() )
	{	
		nWF = 1;
	
		for ( int r = 1 ; r < nLay ; r++ )
		{
			LaySpacing[r-1] = atof(LaySpacingGrid.colS[1][r]);
		}
	}
	else

		nWF = 0;

	if ( m_Check_Temperature.GetCheck() )
		
		nTemp = 1;

	else

		nTemp = 0;
}

void Basic2Phase::OnRadial() 
{
	// TODO: Add your control notification handler code here
	m_ModelType = 0;

	m_Num_Layers.EnableWindow(TRUE);
	m_Num_Comp.EnableWindow(TRUE);
	m_Num_Stages.EnableWindow(TRUE);
	m_Num_Blocks.EnableWindow(TRUE);

	m_Conventional.EnableWindow(TRUE);
	m_Metric.EnableWindow(TRUE);

	// Isotherm
	m_Static_CShut.EnableWindow(FALSE);
	m_Static_VolInj.EnableWindow(FALSE);
	m_Static_TotVol.EnableWindow(FALSE);
	m_Static_InhCon.EnableWindow(FALSE);
	m_Static_Rho.EnableWindow(FALSE);
	
	m_CShut.EnableWindow(FALSE);
	m_VolInj.EnableWindow(FALSE);
	m_TotVol.EnableWindow(FALSE);
	m_InhCon.EnableWindow(FALSE);
	m_Rho.EnableWindow(FALSE);
	
	GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
	m_Edit_IsoTable.cellEdit = FALSE;

	// Temperature Calculations
	if ( m_Radio_Simulation == 0 )

		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(TRUE);
	
	/*if ( m_Radio_Simulation == 1 )
		
		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);*/
	
	// Well Friction
	if (m_Radio_Simulation)
		
		if (nLay > 1)
	
			m_Check_WellFriction.EnableWindow(TRUE);
	
	// Simulation Type
	m_TwoPhase.EnableWindow(TRUE);

	Invalidate(NULL);

	UpdateControls();
}

void Basic2Phase::OnLinear() 
{
	// TODO: Add your control notification handler code here
	m_ModelType = 1;

	m_Num_Layers.EnableWindow(FALSE);
	m_Num_Comp.EnableWindow(TRUE);
	m_Num_Stages.EnableWindow(TRUE);
	m_Num_Blocks.EnableWindow(TRUE);

	m_Conventional.EnableWindow(FALSE);
	m_Metric.EnableWindow(FALSE);

	// Isotherm
	m_Static_CShut.EnableWindow(FALSE);
	m_Static_VolInj.EnableWindow(FALSE);
	m_Static_TotVol.EnableWindow(FALSE);
	m_Static_InhCon.EnableWindow(FALSE);
	m_Static_Rho.EnableWindow(FALSE);
	
	m_CShut.EnableWindow(FALSE);
	m_VolInj.EnableWindow(FALSE);
	m_TotVol.EnableWindow(FALSE);
	m_InhCon.EnableWindow(FALSE);
	m_Rho.EnableWindow(FALSE);
	
	GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
	m_Edit_IsoTable.cellEdit = FALSE;

	// Well Friction
	m_Check_WellFriction.EnableWindow(FALSE);
	m_Check_WellFriction.SetCheck(FALSE);

	// Temperature Calculations
	if ( m_Radio_Simulation == 0 )

		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(TRUE);
	
	if ( m_Radio_Simulation == 1 )

		GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);
	
	
	m_TwoPhase.EnableWindow(TRUE);

	Invalidate(NULL);
	
	UpdateControls();
}

void Basic2Phase::OnIsotherm()
{
	m_ModelType = 2;

	m_Num_Layers.EnableWindow(FALSE);
	m_Num_Comp.EnableWindow(FALSE);
	m_Num_Stages.EnableWindow(TRUE);
	m_Num_Blocks.EnableWindow(TRUE);

	m_Conventional.EnableWindow(FALSE);
	m_Metric.EnableWindow(FALSE);

	// Isotherm
	m_Static_CShut.EnableWindow(TRUE);
	m_Static_VolInj.EnableWindow(TRUE);
	m_Static_TotVol.EnableWindow(TRUE);
	m_Static_InhCon.EnableWindow(TRUE);
	m_Static_Rho.EnableWindow(TRUE);
	
	m_CShut.EnableWindow(TRUE);
	m_VolInj.EnableWindow(TRUE);
	m_TotVol.EnableWindow(TRUE);
	m_InhCon.EnableWindow(TRUE);
	m_Rho.EnableWindow(TRUE);
	
	GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SHOW_DATA)->EnableWindow(TRUE);
	m_Edit_IsoTable.cellEdit = TRUE;

	// Well Friction
	m_Check_WellFriction.EnableWindow(FALSE);
	m_Check_WellFriction.SetCheck(FALSE);
	
	// Temperature Calculations
	GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);

	// Simulation Type
	m_TwoPhase.EnableWindow(FALSE);
	m_TwoPhase.SetCheck(FALSE);
	m_Radio_Simulation = 0;
	m_SinglePhase.SetCheck(TRUE);
	
	Invalidate(NULL);

	UpdateControls();
	
}


void Basic2Phase::OnIsothermDerivation()
{
	m_ModelType = 3;

	m_Num_Layers.EnableWindow(FALSE);
	m_Num_Comp.EnableWindow(FALSE);
	m_Num_Stages.EnableWindow(FALSE);
	m_Num_Blocks.EnableWindow(FALSE);

	m_Conventional.EnableWindow(FALSE);
	m_Metric.EnableWindow(FALSE);

	// Isotherm
	m_Static_CShut.EnableWindow(TRUE);
	m_Static_VolInj.EnableWindow(TRUE);
	m_Static_TotVol.EnableWindow(TRUE);
	m_Static_InhCon.EnableWindow(TRUE);
	m_Static_Rho.EnableWindow(TRUE);
	
	m_CShut.EnableWindow(TRUE);
	m_VolInj.EnableWindow(TRUE);
	m_TotVol.EnableWindow(TRUE);
	m_InhCon.EnableWindow(TRUE);
	m_Rho.EnableWindow(TRUE);
	
	GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(TRUE);
	GetDlgItem(IDC_SHOW_DATA)->EnableWindow(TRUE);
	m_Edit_IsoTable.cellEdit = TRUE;

	// Well Friction
	m_Check_WellFriction.EnableWindow(FALSE);
	m_Check_WellFriction.SetCheck(FALSE);
	
	// Temperature Calculations
	GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);

	// Simulation Type
	m_TwoPhase.EnableWindow(FALSE);
	m_TwoPhase.SetCheck(FALSE);
	m_Radio_Simulation = 0;
	m_SinglePhase.SetCheck(TRUE);
		
	Invalidate(NULL);

	UpdateControls();
	
}


void Basic2Phase::OnAnalitical()
{
	m_ModelType = 4;

	m_Num_Layers.EnableWindow(FALSE);
	m_Num_Comp.EnableWindow(FALSE);
	m_Num_Stages.EnableWindow(FALSE);
	m_Num_Blocks.EnableWindow(FALSE);

	m_Conventional.EnableWindow(TRUE);
	m_Metric.EnableWindow(TRUE);

	// Isotherm
	m_Static_CShut.EnableWindow(FALSE);
	m_Static_VolInj.EnableWindow(FALSE);
	m_Static_TotVol.EnableWindow(FALSE);
	m_Static_InhCon.EnableWindow(FALSE);
	m_Static_Rho.EnableWindow(FALSE);
	
	m_CShut.EnableWindow(FALSE);
	m_VolInj.EnableWindow(FALSE);
	m_TotVol.EnableWindow(FALSE);
	m_InhCon.EnableWindow(FALSE);
	m_Rho.EnableWindow(FALSE);
	
	GetDlgItem(IDC_INPUT_EFFLUENT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOW_DATA)->EnableWindow(FALSE);
	m_Edit_IsoTable.cellEdit = FALSE;

	// Well Friction
	m_Check_WellFriction.EnableWindow(FALSE);
	m_Check_WellFriction.SetCheck(FALSE);
	
	// Temperature Calculations
	GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(FALSE);

	// Simulation Type
	m_TwoPhase.EnableWindow(FALSE);
	m_TwoPhase.SetCheck(FALSE);
	m_Radio_Simulation = 0;
	m_SinglePhase.SetCheck(TRUE);
		
	Invalidate(NULL);

	UpdateControls();
	
}


void Basic2Phase::OnMetric() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_WELLDIA)->SetWindowText("Tubing Inner Diameter, m");
	
	GetDlgItem(IDC_STATIC_TUBEROUGH)->SetWindowText("Tubing Absolute Roughness, m");
	
	GetDlgItem(IDC_STATIC_FLUIDRHO)->SetWindowText("Fluid Density, kg/m3");
	
	GetDlgItem(IDC_STATIC_FORMTEMP)->SetWindowText("Formation Temperature Before Squeeze Treatment, degC");

   GetDlgItem(IDC_STATIC_THCONDRES)->SetWindowText("Thermal Conductivity of Reservoir, Btu/m-day-degC");
	
   GetDlgItem(IDC_STATIC_FORMTHCOND)->SetWindowText("Formation Underburden/Overburden Thermal Conductivity, Btu/m-day-degC");
   
	GetDlgItem(IDC_STATIC_ROHECAP)->SetWindowText("Rock Heat Capacity, Btu/Kg-degC");
   
	GetDlgItem(IDC_STATIC_AQHECAP)->SetWindowText("Aqueous Heat Capacity, Btu/Kg-degC");
   
	GetDlgItem(IDC_STATIC_ROGRDEN)->SetWindowText("Rock Grain Density, lb/m3");
   
	GetDlgItem(IDC_STATIC_AQPHDEN)->SetWindowText("Aqueous Phase Density, lb/m3");

	m_Units = 0;

	unit = FALSE;
	
	if ( !nWF && m_Check_WellFriction.GetCheck() ) LaySpacingGridDraw();
}

void Basic2Phase::OnConventional() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_STATIC_WELLDIA)->SetWindowText("Tubing Inner Diameter, ft");
	
	GetDlgItem(IDC_STATIC_TUBEROUGH)->SetWindowText("Tubing Absolute Roughness, ft");
	
	GetDlgItem(IDC_STATIC_FLUIDRHO)->SetWindowText("Fluid Density, lb/ft3");
	
   GetDlgItem(IDC_STATIC_FORMTEMP)->SetWindowText("Formation Temperature Before Squeeze Treatment, degC");

   GetDlgItem(IDC_STATIC_THCONDRES)->SetWindowText("Thermal Conductivity of Reservoir, Btu/ft-day-degC");
	
   GetDlgItem(IDC_STATIC_FORMTHCOND)->SetWindowText("Formation Underburden/Overburden Thermal Conductivity, Btu/ft-day-degC");
   
	GetDlgItem(IDC_STATIC_ROHECAP)->SetWindowText("Rock Heat Capacity, Btu/lb-degC");
   
	GetDlgItem(IDC_STATIC_AQHECAP)->SetWindowText("Aqueous Heat Capacity, Btu/lb-degC");
   
	GetDlgItem(IDC_STATIC_ROGRDEN)->SetWindowText("Rock Grain Density, lb/ft3");
   
	GetDlgItem(IDC_STATIC_AQPHDEN)->SetWindowText("Aqueous Phase Density, lb/ft3");
	
	m_Units = 1;

	unit = TRUE;

	if ( !nWF && m_Check_WellFriction.GetCheck() ) LaySpacingGridDraw();
}

void Basic2Phase::OnCheckWellfriction() 
{
	// TODO: Add your control notification handler code here
	CString sEdit;
	int n;
	
	m_Num_Layers.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 50))
		{ nLay = n; }
	}
	
	if ( m_Check_WellFriction.GetCheck()  )
	{	
		if ( nLay > 1 )
		{
			m_Static_WellDia.EnableWindow(TRUE);
			m_Edit_WellDia.EnableWindow(TRUE);

			m_Static_TubeRough.EnableWindow(TRUE);
			m_Edit_TubeRough.EnableWindow(TRUE);

			m_Static_FluidRho.EnableWindow(TRUE);
			m_Edit_FluidRho.EnableWindow(TRUE);
		
			LaySpacingGridDraw();
		}
		else
		{	
			m_Static_WellDia.EnableWindow(FALSE);
			m_Edit_WellDia.EnableWindow(FALSE);

			m_Static_TubeRough.EnableWindow(FALSE);
			m_Edit_TubeRough.EnableWindow(FALSE);

			m_Static_FluidRho.EnableWindow(FALSE);
			m_Edit_FluidRho.EnableWindow(FALSE);

			LaySpacingGrid.Disabled();

			Invalidate(NULL);
		}
	}
	else
	{	
		m_Static_WellDia.EnableWindow(FALSE);
		m_Edit_WellDia.EnableWindow(FALSE);

		m_Static_TubeRough.EnableWindow(FALSE);
		m_Edit_TubeRough.EnableWindow(FALSE);

		m_Static_FluidRho.EnableWindow(FALSE);
		m_Edit_FluidRho.EnableWindow(FALSE);

		LaySpacingGrid.Disabled();

		Invalidate(NULL);
	}
}

void Basic2Phase::OnCheckTemperature() 
{
	if ( m_Check_Temperature.GetCheck() )
	{
		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROGRDEN)->EnableWindow(TRUE);  
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(TRUE);

		m_Edit_FormTemp.EnableWindow(TRUE);
		m_Edit_AqHeCap.EnableWindow(TRUE);
		m_Edit_FormThCond.EnableWindow(TRUE);
		m_Edit_ThCondRes.EnableWindow(TRUE);
		m_Edit_RoGrDen.EnableWindow(TRUE);
		m_Edit_AqPhDen.EnableWindow(TRUE);
		m_Edit_RoHeCap.EnableWindow(TRUE);

		if (m_Radio_Simulation == 1)
		{

			GetDlgItem(IDC_CHECK_WAT_VISC)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_OIL_VISC)->EnableWindow(TRUE);

			if (m_Check_WatTemp.GetCheck())
				m_Check_WatTemp.SetCheck(TRUE);
			else
				m_Check_WatTemp.SetCheck(FALSE);

			if (m_Check_OilTemp.GetCheck())
			{
				GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(TRUE);
			}
			else
			{
				GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);
			}

			GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_NAHECAP)->EnableWindow(TRUE);
			GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_NAPHDEN)->EnableWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(FALSE); 
		GetDlgItem(IDC_STATIC_ROGRDEN)->EnableWindow(FALSE);  
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(FALSE);

		m_Edit_FormTemp.EnableWindow(FALSE);
		m_Edit_AqHeCap.EnableWindow(FALSE);
		m_Edit_FormThCond.EnableWindow(FALSE);
		m_Edit_ThCondRes.EnableWindow(FALSE);
		m_Edit_RoGrDen.EnableWindow(FALSE);
		m_Edit_AqPhDen.EnableWindow(FALSE);
		m_Edit_RoHeCap.EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_WAT_VISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_OIL_VISC)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);
	}

	Invalidate(NULL);
}

void Basic2Phase::OnKillfocusNumLayers() 
{
	// TODO: Add your control notification handler code here
	CString sEdit;
	int n;
	
	m_Num_Layers.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		n = atoi(sEdit);
		if ((n > 0)&&(n < 50))
		{ nLay = n; }
	}
	
	if ( m_Check_WellFriction.GetCheck() )
	{
		LaySpacingGridDraw();
	}

	if( nLay < 2)
	{
		m_Check_WellFriction.SetCheck( BST_UNCHECKED); 
		m_Check_WellFriction.EnableWindow(FALSE); 
		
		m_Static_WellDia.EnableWindow(FALSE);
		m_Edit_WellDia.EnableWindow(FALSE);

		m_Static_TubeRough.EnableWindow(FALSE);
		m_Edit_TubeRough.EnableWindow(FALSE);

		m_Static_FluidRho.EnableWindow(FALSE);
		m_Edit_FluidRho.EnableWindow(FALSE);

		LaySpacingGrid.Disabled();
	}
	else
		 
		if (m_Radio_Simulation == 1)
		
			 m_Check_WellFriction.EnableWindow(TRUE);
}

void Basic2Phase::LaySpacingGridDraw()
{	
	CString sItem;

	testgrid TempLayGrid(nLay,2);

	LaySpacingGrid = TempLayGrid;

	LaySpacingGrid.SetCellText(0, 0, "Lay");
	LaySpacingGrid.SetColWidth(0, 40);
	LaySpacingGrid.SetCellText(1, 0, "Spacing, " + CString((unit) ? "ft" : "m") );
	LaySpacingGrid.SetColWidth(1, 87);
	
	LaySpacingGrid.SetRowHeight(0, 20);

	LaySpacingGrid.SetColEdit(0, testgrid::READ_ONLY);
	LaySpacingGrid.SetRowEdit(0, testgrid::READ_ONLY);
	
	LaySpacingGrid.SetColFormat(0, testgrid::HEADER);
	LaySpacingGrid.SetRowFormat(0, testgrid::HEADER);
	
	LaySpacingGrid.CalculateColRowPos();

	for ( int r = 1 ; r < nLay ; r++ )
	{
		sItem.Format("(%d,%d)", r, r + 1);
		LaySpacingGrid.colS[0][r] = sItem;
		sItem.Format("%.4g", LaySpacing[r-1]);
		LaySpacingGrid.colS[1][r] = sItem;
	}
	
	Invalidate(NULL);
}


void Basic2Phase::OnSinglePhase() 
{	
	m_Radio_Simulation = 0;

	GetDlgItem(IDC_CHECK_TEMPERATURE)->EnableWindow(TRUE);
	
	// Model types
	GetDlgItem(IDC_ISOTHERM)->EnableWindow(TRUE);
	GetDlgItem(IDC_ISOTHERM_DER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ANAL_LINEAR)->EnableWindow(TRUE);

	// Well Friction
	m_Check_WellFriction.EnableWindow(FALSE);
	m_Check_WellFriction.SetCheck(FALSE);

	m_Static_WellDia.EnableWindow(FALSE);
	m_Edit_WellDia.EnableWindow(FALSE);

	m_Static_TubeRough.EnableWindow(FALSE);
	m_Edit_TubeRough.EnableWindow(FALSE);

	m_Static_FluidRho.EnableWindow(FALSE);
	m_Edit_FluidRho.EnableWindow(FALSE);

	LaySpacingGrid.Disabled();
	
	UpdateControls();

	Invalidate(NULL);
}

void Basic2Phase::OnTwoPhase() 
{	
	m_Radio_Simulation = 1;
	
	// Model types
	GetDlgItem(IDC_ISOTHERM)->EnableWindow(FALSE);
	GetDlgItem(IDC_ISOTHERM_DER)->EnableWindow(FALSE);
	GetDlgItem(IDC_ANAL_LINEAR)->EnableWindow(FALSE);

//	m_Check_Temperature.SetCheck(FALSE);
	
	if (nLay > 1)
	
		m_Check_WellFriction.EnableWindow(TRUE);

	if (m_Check_Temperature.GetCheck())
	{
		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(TRUE); 
		GetDlgItem(IDC_STATIC_ROGRDEN)->EnableWindow(TRUE);  
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NAHECAP)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_NAPHDEN)->EnableWindow(TRUE);
		m_Edit_FormTemp.EnableWindow(TRUE);
		m_Edit_AqHeCap.EnableWindow(TRUE);
		m_Edit_FormThCond.EnableWindow(TRUE);
		m_Edit_ThCondRes.EnableWindow(TRUE);
		m_Edit_RoGrDen.EnableWindow(TRUE);
		m_Edit_AqPhDen.EnableWindow(TRUE);
		m_Edit_RoHeCap.EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_STATIC_FORMTEMP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_THCONDRES)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FORMTHCOND)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ROHECAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AQHECAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ROGRDEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_AQPHDEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NAHECAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NAHECAP)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NAPHDEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_NAPHDEN)->EnableWindow(FALSE);
		m_Edit_FormTemp.EnableWindow(FALSE);
		m_Edit_AqHeCap.EnableWindow(FALSE);
		m_Edit_FormThCond.EnableWindow(FALSE);
		m_Edit_ThCondRes.EnableWindow(FALSE);
		m_Edit_RoGrDen.EnableWindow(FALSE);
		m_Edit_AqPhDen.EnableWindow(FALSE);
		m_Edit_RoHeCap.EnableWindow(FALSE);
	}

	

	UpdateControls();
}	


void Basic2Phase::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	check = true;

	UpdateFromEdits();
	UpdateControls();

	CDialog::OnOK();
}


void Basic2Phase::OnBnClickedCheckWatVisc()
{
	// TODO: Add your control notification handler code here
	if (m_Check_WatTemp.GetCheck())
		watTemp = 1;
	else
		watTemp = 0;
}


void Basic2Phase::OnBnClickedCheckOilVisc()
{
	if (m_Check_OilTemp.GetCheck())
	{
		oilTemp = 1;
		GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(TRUE);
	}
	else
	{	
		oilTemp = 0;
		GetDlgItem(IDC_STATIC_OILAPI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OILAPI)->EnableWindow(FALSE);
	}
}
