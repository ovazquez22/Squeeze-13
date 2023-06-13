// BasicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "BasicDlg.h"
#include "IsoThermFittingDlg.h"
#include "Iso_Par_Dlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const Model *OneLayerData::mod;
vector<Stg> CSheet::ST;
int CSheet::kPlot;

mutex MTX;    // this should be a global variable

void WorkerFunc(string msg, int i)
{
	const int BUFFSIZE = 1024;
	char buff[BUFFSIZE];

	MTX.lock();
	sprintf_s(buff, "This is function %d\t %s\r\n", i, msg.c_str());
	AfxMessageBox(buff);
	cout << buff;
	MTX.unlock();
}

/////////////////////////////////////////////////////////////////////////////
// BasicDlg dialog


BasicDlg::BasicDlg(CWnd* pParent /*=NULL*/)
: CDialog(BasicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BasicDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	userTimeDelay = 0;

	MNTcount = 0;

	GenBool = true;

	int i;

	int nRCharts = 2;

	ParRun = false;

	Allocate();

	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].Create(IDD_GRAPHICS, this);
		GraphTime[i].MoveWindow(379 + 50 + 5 + i * 379, 295, 342, 218);
	}

	for (i = 0; i<nGraphRad; i++)
	{
		GraphRad[i].Create(IDD_GRAPHICS, this);
		GraphRad[i].MoveWindow(379 + 50 + 5 + i * 379, 218 + 295 + 5, 342, 218);
	}

	GraphCross.Create(IDD_GRAPHICS, this);
	GraphCross.MoveWindow(55 + 379, 15, 342, 218);

	ODlg.Create(IDD_OPT, this);

	FNODlg.Create(IDD_FILEOPEN_DLG, this);

	Par.CreateDlg(this);
	/*	DlgWar->Create(IDD_WARNINGS,this);
	DlgWar->GetClientRect(DWRect);*/

	check = false;
}

BasicDlg::~BasicDlg()
{
	//	delete [] DlgWar;
	DeAllocate();
}

void BasicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BasicDlg)
	DDX_Control(pDX, IDC_CHECK_GRAPHS, m_Button_Graphs);
	DDX_Control(pDX, IDC_WARNING, m_Button_Warning);
	DDX_Control(pDX, IDC_MOD, m_Button_MOD);
	DDX_Control(pDX, IDC_CLOSE, m_Button_Close);
	DDX_Control(pDX, IDC_GRAPH_CROSS2, m_Button_Graph_Cross);
	DDX_Control(pDX, IDC_GRAPH_RAD, m_Button_Graph_Rad);
	DDX_Control(pDX, IDC_GRAPH_TIME, m_Button_Graph_Time);
	DDX_Control(pDX, IDC_EDIT_TITLE2, m_EditTitle2);
	DDX_Control(pDX, IDC_EDIT_TITLE1, m_EditTitle1);
	DDX_Control(pDX, IDC_STATIC_ADDEDTIME, m_Static_AddedTime);
	DDX_Control(pDX, IDC_SLIDER_TIME_DELAY, m_Slider_TimeDelay);
	DDX_Control(pDX, IDC_SLIDER_SP_TS, m_Slider_SP_TS);
	DDX_Control(pDX, IDC_ABORT_RUN, m_bAbortRun);
	DDX_Control(pDX, IDC_EDIT_MODEL1, m_bEditModel);
	DDX_Control(pDX, IDC_BASIC_PARA, m_bBasicParam);
	DDX_Control(pDX, IDC_RUN_MODEL, m_bRunModel);
	DDX_Control(pDX, IDC_READ_FILE, m_bReadFile);
	DDX_Control(pDX, IDC_EDIT_MOD, m_editMod);
	DDX_Control(pDX, IDC_STATIC_SIM_TYPE, m_Static_Sim_Type);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(BasicDlg, CDialog)
	//{{AFX_MSG_MAP(BasicDlg)
	ON_BN_CLICKED(IDC_READ_FILE, OnReadFile)
	ON_BN_CLICKED(IDC_EDIT_MODEL1, OnEditModel)
	ON_BN_CLICKED(IDC_RUN_MODEL, OnRunModel)
	ON_BN_CLICKED(IDC_GRAPH_TIME, OnGraphTime)
	ON_BN_CLICKED(IDC_GRAPH_RAD, OnGraphRad)
	ON_BN_CLICKED(IDC_GRAPH_CROSS2, OnGraphCross)
	ON_BN_CLICKED(IDC_MOD, OnMod)
	ON_BN_CLICKED(IDC_BASIC_PARA, OnBasicPara)
	ON_BN_CLICKED(IDC_ABORT_RUN, OnAbortRun)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_HELP, OnHelp)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_HELP, OnHelp)
	ON_BN_CLICKED(IDC_BUTTON_PHREEQC, &BasicDlg::OnBnClickedButtonPhreeqc)
	ON_BN_CLICKED(IDC_CHECK_GRAPHS, &BasicDlg::OnBnClickedCheckGraphs)
	ON_EN_CHANGE(IDC_SLIDER_SP_TS, &BasicDlg::OnEnChangeSlider_SP_TS)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_BACK, &BasicDlg::OnBnClickedButtonBack)
	ON_BN_CLICKED(IDC_BUTTON_FOWARD, &BasicDlg::OnBnClickedButtonFoward)
	ON_BN_CLICKED(IDC_BUTTON_PAR_OPT, &BasicDlg::OnButtonParOpt)
	ON_BN_CLICKED(IDC_BUTTON_ISO_TEST, &BasicDlg::OnBnClickedButtonIsoTest)
	ON_BN_CLICKED(IDC_BUTTON_MOPSO, &BasicDlg::OnBnClickedButtonMopso)
	ON_BN_CLICKED(IDC_BUTTON1, &BasicDlg::OnBnClickedSensitivityCalculations)
	ON_STN_CLICKED(IDC_STATIC_SIM_TYPE, &BasicDlg::OnStnClickedStaticSimType)
	ON_BN_CLICKED(IDC_BUTTON2, &BasicDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_Sec_Opt, &BasicDlg::OnBnClickedButtonSecOpt)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// BasicDlg message handlers

void BasicDlg::OnReadFile()
{
	// TODO: Add your control notification handler code here
	//	CString pathname;

	CString openFilter = "Input data files (*.dat)|*.dat|";

	CFileDialog openDialog(TRUE, "inp", PathNameReadFile, OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{
		Mod.ReadFileName = openDialog.GetFileName();

		PathNameReadFile = openDialog.GetPathName();
		
		TitleNameReadFile = openDialog.GetFileTitle();

		errno_t err;
		FILE *filePtr;

		char buffer[128];

		err = fopen_s(&filePtr, PathNameReadFile, "r");

		if (err == 0)
			printf("The file %s was opened\n", PathNameReadFile);
		else
			printf("The file %s was not opened\n", PathNameReadFile);

		/* Set pointer to beginning of file: */
		fseek(filePtr, 0L, SEEK_SET);

		// validate the SQUEEZE data file and check the version
		fscanf(filePtr, "%s\n", buffer);

		if (strncmp(buffer, "SQZV6.", 6) == 0 || strncmp(buffer, "sqzv6.", 6) == 0)
		{
			Mod.nSimTyp = 1;
			m_Static_Sim_Type.SetWindowText("Two-Phase Simulation");
		}
			

		if (strncmp(buffer, "SQZV5.0", 7) == 0 || strncmp(buffer, "sqzv5.0", 7) == 0)
		{	
			Mod.nSimTyp = 0;
			m_Static_Sim_Type.SetWindowText("Single Simulation");

			Mod.nComponentsWater = 1;
			Mod.wComp.resize(Mod.nComponentsWater);
			Mod.oComp.resize(Mod.nComponentsWater);
		}
			

		fgets(buffer, 128, filePtr);
		fgets(buffer, 128, filePtr);
		fgets(buffer, 128, filePtr);

		int temp;
		fscanf(filePtr, "%d %d %d \n", &Mod.iModel, &temp, &temp);

		fclose(filePtr);
	}

	if (Mod.nSimTyp == 0) // Single Phase Simulation
	{
		GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(false);
		
		//	Isotherm Derivation 
		if (Mod.iModel == 5 || Mod.iModel == 1)
		{	
			IsoDerGrp.Mod.PathName = openDialog.GetPathName();
			IsoDerGrp.Mod.ReadFileName = openDialog.GetFileName();
			IsoDerGrp.Mod.PathName.Replace(IsoDerGrp.Mod.ReadFileName,"");
			Mod.ReadFileSQZ5Isotherm(PathNameReadFile);
			Mod.FieldTabFileName = TitleNameReadFile + "LabTab" + ".txt";
		}

		// Radial 
		if (Mod.iModel == 3)
		{	
			Mod.FieldTabFileName = TitleNameReadFile + "FieldTab" + ".txt";
			Mod.ReadFileSQZ5Radial(PathNameReadFile);
		}
	}

	if (Mod.nSimTyp == 1) // Two Phase Simulation
	{
		GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
		/*	m_Button_Graphs.SetCheck(TRUE);
		m_Button_Graph_Cross.EnableWindow(TRUE);
		m_Button_Graph_Time.EnableWindow(TRUE);
		m_Button_Graph_Rad.EnableWindow(TRUE);
		m_editMod.EnableWindow(TRUE);
		m_Static_AddedTime.EnableWindow(TRUE);
		m_Slider_TimeDelay.EnableWindow(TRUE);
		m_Button_MOD.EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
		m_Button_Graphs.SetCheck(true);*/
	}

	m_bRunModel.EnableWindow(TRUE);


}

void BasicDlg::OnEditModel()
{
	// TODO: Add your control notification handler code here
	int i = 0,
		l, c, s;

	bool Check = TRUE;

	double SumKH;

	if (Mod.nSimTyp == 0)

		Mod.wComp[0].Role = 1;

	CSheet Sheet("Edit Model");

	SumKH = 0;

	for (l = 0; l < Mod.nLay; l++)

		SumKH += (Mod.SLay[l].HLay * Mod.SLay[l].KAbs);

	for (s = 0; s < Mod.nStg; s++)
	{
		if (Mod.stg[s].nMethod == 0)
		{
			for (l = 0; l < Mod.nLay; l++)
			{
				Mod.stg[s].PIDF[l] = (Mod.SLay[l].HLay * Mod.SLay[l].KAbs) / SumKH;
			}
		}
	}

	PropPage1 TempPage1(Mod.nLay);

	Page1 = TempPage1;

	Page1.iModel = Mod.iModel;
	Page1.nSimTyp = Mod.nSimTyp;

	Page1.MUw = Mod.MUw;
	Page1.MUo = Mod.MUo;

	Page1.WellRad = Mod.radiusWell;
	Page1.MaxRad = Mod.radiusMax;

	Page1.SLay = Mod.SLay;

	Page1.PathName = PathName;

	if (Mod.iModel == 3) // Radial 
	{	
		Page1.m_psp.dwFlags |= PSP_USETITLE;
		Page1.m_psp.pszTitle = "Layer Properties";
	}

	if (Mod.iModel == 4) // Linear
	{
		Page1.m_psp.dwFlags |= PSP_USETITLE;
		Page1.m_psp.pszTitle = "Core Properties";
	}

	if (Mod.iModel == 5) // Isotherm
	{
		Page1.m_psp.dwFlags |= PSP_USETITLE;
		Page1.m_psp.pszTitle = "Core Properties";
	}

	PropPage2 Page2;

	PropPage2 TempPage2(Mod.nLay, Mod.nComponentsWater);

	Page2 = TempPage2;

	Page2.nLay = Mod.nLay;
	Page2.TempnLay = Mod.nLay;
	Page2.nComp = Mod.nComponentsWater;
	Page2.IsoSpec = Mod.IsoSpec;
	Page2.iModel = Mod.iModel;
	Page2.kTemp = Mod.kTemp;

	Page2.PathName = PathName;

	Page2.wIso.resize(Page2.nLay, vector<Iso>(Page2.nComp,Mod.Iso));
	Page2.oIso.resize(Page2.nLay, vector<Iso>(Page2.nComp, Mod.Iso));

	for (l = 0; l < Page2.nLay; l++)
	{
		for (c = 0; c < Page2.nComp; c++)
		{
			Page2.wIso[l][c] = Mod.Iso;

			Page2.oIso[l][c] = Mod.Iso;
		}

	}

//	Page2.WaterGrid.BloDlg.rfDlg = Mod.RFLay[0];

	Page2.WaterGrid.VisDlg.Allocate(Page2.nComp);

	Page2.WaterGrid.kTemp = Mod.kTemp;

	for (c = 0; c < Page2.nComp; c++)
	{
		Page2.waterComp[c] = Mod.wComp[c];
		Page2.oilComp[c] = Mod.oComp[c];

		if (Mod.wComp[c].Role == 2) // Viscosifier
		{
			Page2.WaterGrid.VisDlg.waterComp = Mod.wComp[c];
			Page2.WaterGrid.VisDlg.oilComp = Mod.oComp[c];
		}

		if (Mod.wComp[c].Role == 4) // Surfactant
		{
			Page2.WaterGrid.MSDlg.NewSwi = Mod.SLay[0].NewSwi;
			Page2.WaterGrid.MSDlg.NewSor = Mod.SLay[0].NewSor;

			Page2.WaterGrid.MSDlg.MAdso = Mod.wComp[c].MAdso;

			Page2.WaterGrid.MSDlg.IsoTyp = Mod.wComp[c].IsoTypeAds;
			Page2.WaterGrid.MSDlg.Irr = Mod.wComp[c].IrrAdso;

			Page2.WaterGrid.MSDlg.Ads1 = Mod.wComp[c].High_Ads1;
			Page2.WaterGrid.MSDlg.Ads2 = Mod.wComp[c].High_Ads2;

		//	Page2.WaterGrid.MSDlg.TableName = Mod.Iso.CompIsoTabFile;
			Page2.WaterGrid.MSDlg.PathName = PathName;

		}

		if (Mod.wComp[c].Role == 5) // Ca
		{
			Page2.WaterGrid.CaNaDlg.IsoTyp = Mod.wComp[c].IsoTypAct;

			Page2.WaterGrid.CaNaDlg.Low_Ads1 = Mod.wComp[c].Low_Ads1;
			Page2.WaterGrid.CaNaDlg.Low_Ads2 = Mod.wComp[c].Low_Ads2;

			Page2.WaterGrid.CaNaDlg.High_Ads1 = Mod.wComp[c].High_Ads1;
			Page2.WaterGrid.CaNaDlg.High_Ads2 = Mod.wComp[c].High_Ads2;

			Page2.WaterGrid.CaNaDlg.CacoreConc = Mod.wComp[c].coreConc;

			Page2.WaterGrid.CaNaDlg.Low_Na = Mod.wComp[c].Low_Conc;
			Page2.WaterGrid.CaNaDlg.High_Na = Mod.wComp[c].High_Conc;
		}

		if (Mod.wComp[c].Role == 6) // Na
		{
			Page2.WaterGrid.CaNaDlg.NacoreConc = Mod.wComp[c].coreConc;
		}

		if (Mod.wComp[c].Role == 7) // Bridging Agent
		{
			Page2.WaterGrid.MSDlg.MAdso = Mod.wComp[c].MAdso;

			Page2.WaterGrid.MSDlg.IsoTyp = Mod.wComp[c].IsoTypeAds;
			Page2.WaterGrid.MSDlg.Irr = Mod.wComp[c].IrrAdso;

			Page2.WaterGrid.MSDlg.Ads1 = Mod.wComp[c].High_Ads1;
			Page2.WaterGrid.MSDlg.Ads2 = Mod.wComp[c].High_Ads2;

		//	Page2.WaterGrid.MSDlg.TableName = Mod.Iso.CompIsoTabFile;
			Page2.WaterGrid.MSDlg.PathName = PathName;

			Page2.WaterGrid.MSDlg.ModAdd = Mod.wComp[c].ModAdd;
			Page2.WaterGrid.MSDlg.MaxAdso = Mod.wComp[c].MaxAdso;

		}
	}

	PropPage3 Page3;

	PropPage3 TempPage3(Mod.nStg, Mod.nLay, Mod.nComponentsWater);

	Page3 = TempPage3;

	if (Mod.iModel != 2)

		Sheet.AddPage(&Page1);

	if (Mod.iModel != 5 && Mod.iModel != 2)

		Sheet.AddPage(&Page2);

	if (Mod.iModel != 2)

		Sheet.AddPage(&Page3);

	Page3.StageGrid.StgDlg.nWF = Mod.WellFric;

	Page3.nStg = Mod.nStg;

	for (s = 0; s < Page3.nStg; s++)
	{
		// if Well Friction activated, alll layers are open
		if (Page3.StageGrid.StgDlg.nWF)

			Mod.stg[s].nOpen = 0;

		Page3.Stage[s] = Mod.stg[s];

		Page3.StageGrid.StgDlg.Stage[s] = Mod.stg[s];

	}

	Page3.StageGrid.StgDlg.nStg = Page3.nStg;
	Page3.StageGrid.StgDlg.nComp = Mod.nComponentsWater;
	Page3.NoTstep = Mod.NoTstep;
	Page3.iModel = Mod.iModel;
	Page3.StageGrid.iModel = Mod.iModel;
	Page3.kTemp = Mod.kTemp;
	
	for (i = 0; i < Mod.nComponentsWater; i++)
	{
		Page3.StageGrid.StgDlg.compRole[i] = Mod.wComp[i].Role;
		testgrid::set_compRole(i, Mod.wComp[i].Role);
	}

	PropPage4 Page4;

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M')
	{
		Page1.Units = 0;
		//		Page2.Units = 0;
		Page3.Units = 0;
		Page4.Units = 0;
	}
	else
	{
		Page1.Units = 1;
		//		Page2.Units = 1;
		Page3.Units = 1;
		Page4.Units = 1;
	}

	PropPage7 Page7;

	Page7.nCols = 6;
	Page7.nRows = 1;
	Page7.nStg = Page3.nStg;

	Page7.P7SqzOpt = &Mod.SqzOpt;

	Page7.P7SqzOpt->InjHourRate = Mod.SqzOpt.InjHourRate;
	Page7.P7SqzOpt->nGen = Mod.SqzOpt.nGen;
	Page7.P7SqzOpt->nPar = Mod.SqzOpt.nPar;
	Page7.P7SqzOpt->nPopSize = Mod.SqzOpt.nPopSize;
	Page7.P7SqzOpt->BinsMT = Mod.SqzOpt.BinsMT;
	Page7.P7SqzOpt->IterationsSecant = Mod.SqzOpt.IterationsSecant;
	Page7.P7SqzOpt->nBins = Mod.SqzOpt.nBins;
	Page7.P7SqzOpt->Fixed_Target = Mod.SqzOpt.Fixed_Target;
	Page7.P7SqzOpt->SIBblRate = Mod.SqzOpt.SIBblRate;
	Page7.P7SqzOpt->MIC = Mod.SqzOpt.MIC;
	Page7.P7SqzOpt->Obj_T_V = Mod.SqzOpt.Obj_T_V;
	Page7.P7SqzOpt->TotSIVol = Mod.SqzOpt.TotSIVol;
	Page7.P7SqzOpt->TotInjVol = Mod.SqzOpt.TotInjVol;
	Page7.P7SqzOpt->SensCalc = Mod.SqzOpt.SensCalc;
	Page7.P7SqzOpt->Only_SI = Mod.SqzOpt.Only_SI;
	Page7.P7SqzOpt->Only_Tot_Vol = Mod.SqzOpt.Only_Tot_Vol;
	
	for (int n = 0; n < Page7.P7SqzOpt->nPar; n++)
	{
		Page7.P7SqzOpt->Label[n] = Mod.SqzOpt.Label[n];
		Page7.P7SqzOpt->X[n] = Mod.SqzOpt.X[n];
		Page7.P7SqzOpt->Vol[n] = Mod.SqzOpt.Vol[n];
		Page7.P7SqzOpt->uX[n] = Mod.SqzOpt.uX[n];
		Page7.P7SqzOpt->lX[n] = Mod.SqzOpt.lX[n];

	}

	if (Mod.nSimTyp == 1)
	{
		if (Mod.nStg != SqzOpt.oStg.size())
		{
			SqzOpt.oStg.resize(Mod.nStg);
			SqzOpt.oStg.reserve(Mod.nStg);
		}
	}

	if (Mod.iModel == 3)
	{	
		Page7.P7SqzOpt->oStg.resize(Page7.nStg);

		for (int s = 0; s < Page7.nStg; s++)
		{	
			Page7.P7SqzOpt->oStg[s] = Mod.stg[s];
		}
	}

	if (Mod.iModel == 3)
	{
		CSheet::ST = Page3.Stage;
	}

	CSheet::kPlot = Mod.kPlot;
	for (int s = 0; s < Page7.nStg; s++)
	{
		if (Page3.StageGrid.StgDlg.Stage[s].TypeStg == 1)
			Page7.nRows += 2;

		if (Page3.StageGrid.StgDlg.Stage[s].TypeStg == 2)
			Page7.nRows++;
	}

	if (Mod.kPlot == 0 || Mod.kPlot == 2)
		Page7.kPlot = 0;

	if (Mod.kPlot == 1 || Mod.kPlot == 3)
		Page7.kPlot = 1;

	if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C')
		Page7.Units = 0;

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M')
		Page7.Units = 1;



	testgrid tempgrid(Page7.nRows, Page7.nCols);

	Page7.StageGrid = tempgrid;

	if (Mod.iModel == 3) // Radial Model
	{
		Sheet.AddPage(&Page4);

	}

	if (Mod.nSimTyp == 1) //  1 = Squeeze VI
	{
		Page1.nDisabled = Page2.nDisabled = Page3.nDisabled = Page4.nDisabled = 1;

		Page4.TolItr = Mod.TolSQ5;
	}

	if (Mod.nSimTyp == 0) // 0 = Squeeze V,
	{
		Page1.nDisabled = Page2.nDisabled = Page3.nDisabled = Page4.nDisabled = 0;

		Page4.TolItr = Mod.TolSQ5;

		if (Mod.iModel == 3) // Radial Model

			Sheet.AddPage(&Page7);
	}

	Page4.nLay = Mod.nLay;

	Page4.iModel = Mod.iModel;

	Page4.Clife = Mod.Clife;

	Page4.nFreq = Mod.nFreq;

	Page4.nIters = Mod.nIters;

	(Mod.iAnim ? Page4.m_Bool_Screen_Display = TRUE : Page4.m_Bool_Screen_Display = FALSE);

	Page4.kPlot = Mod.kPlot;

	Page4.kTemp = Mod.kTemp;

	if (Page4.kTemp == 0) Mod.IsTmp = 0;

	(Mod.IsCon ? Page4.m_Bool_IsCon = TRUE : Page4.m_Bool_IsCon = FALSE);

	(Mod.IsTmp ? Page4.m_Bool_IsTmp = TRUE : Page4.m_Bool_IsTmp = FALSE);

	if (Page4.kPlot == 0) Page4.m_Return_Curve_Type = 0;

	if (Page4.kPlot == 1) Page4.m_Return_Curve_Type = 1;

	if (Page4.kPlot == 2) Page4.m_Return_Curve_Type = 3;

	if (Page4.kPlot == 3) Page4.m_Return_Curve_Type = 2;

	Page4.m_Edit_FieldTable.FieldTable.NIso = Mod.nPts;

	if (Mod.nPts < 1 && Mod.iModel == 3)

		Mod.FieldTabFileName = "No File";

	Page4.m_Edit_FieldTable.FTFileName = Mod.FieldTabFileName;

	Page4.m_Edit_FieldTable.PathName = PathName;

	//	Page4.OldFileGrid.PathNameReadFile = PathName + Mod.ReadFileName;

	Page4.OldFileGrid.PathNameReadFile = PathNameReadFile;

	Page4.OldFileGrid.PathNameReadFile.Replace(".dat", ".out");

	Page4.OldFileGrid.PathNameReadFile.Replace(".DAT", ".out");

	c = 0;

	for (i = 0; i < Mod.nOlds; i++)
	{
		strcpy(Page4.FileNm[i], Mod.FileNm[i].GetBuffer());

		if (i >= MaxFileNm / 2)

			Page4.OldFileGrid.colS[1][i % (MaxFileNm / 2)] = Mod.FileNm[i];

		else

			Page4.OldFileGrid.colS[0][i] = Mod.FileNm[i];

	}

	Page4.nOlds = Mod.nOlds;

	Page4.nLife = Mod.nLife;

	Page4.GrafMIC = Mod.GrafMIC;

	Page4.GPDlg.nLay = Mod.nLay;

	Page4.GPDlg.MaxConc = Mod.Cmax;

	Page4.GPDlg.MinConc = Mod.Cmin;

	Page4.GPDlg.kCrange = Mod.kCrange;

	Page4.GPDlg.kGraf1 = Mod.kGraf1;

	if (Page4.GPDlg.kGraf1 == 1) Page4.GPDlg.m_OptGraf1 = 0;

	if (Page4.GPDlg.kGraf1 == 2) Page4.GPDlg.m_OptGraf1 = 1;

	if (Page4.GPDlg.kGraf1 == 3) Page4.GPDlg.m_OptGraf1 = 2;

	Page4.GPDlg.kGraf2 = Mod.kGraf2;

	Page4.GPDlg.kGraf3 = Mod.kGraf3;

	Page4.GPDlg.nDebug = Mod.nDebug;

	i = 0;

/*	for (l = 0; l < Page4.nLay; l++)
	{
		if (Mod.nGraf2[i] == (l + 1))
		{
			Page4.GPDlg.nGraf2[l] = Mod.nGraf2[i];

			i++;
		}

		else Page4.GPDlg.nGraf2[l] = 0;

	}

	i = 0;

	for (l = 0; l < Page4.nLay; l++)
	{
		if (Mod.nGraf3[i] == (l + 1))
		{
			Page4.GPDlg.nGraf3[l] = Mod.nGraf3[i];

			i++;
		}

		else Page4.GPDlg.nGraf3[l] = 0;

	}
*/
/*	Page4.bCw = Mod.bCw;
	Page4.bCo = Mod.bCo;
	Page4.bAdsW = Mod.bAdsW;
	Page4.bAdsO = Mod.bAdsO;
	Page4.bViscW = Mod.bViscW;
	Page4.bViscO = Mod.bViscO;
	Page4.bRFW = Mod.bRFW;
	Page4.bRFO = Mod.bRFO;
	Page4.bOutputFile = Mod.bOutputFile;
	Page4.bSW = Mod.bSW;
	Page4.bFW = Mod.bFW;

	Page4.bOutputFileTime = Mod.bOutputFileTime;

	if (Mod.iModel == 3) Page4.OTDlg.bRad = 1; // Radial

	if (Mod.iModel == 4) Page4.OTDlg.bRad = 0; // Linear

	Page4.OTDlg.bWC = Mod.bWC;
	Page4.OTDlg.bTQw = Mod.bTQw;
	Page4.OTDlg.bTQo = Mod.bTQo;
	Page4.OTDlg.bTCw = Mod.bTCw;
	Page4.OTDlg.bTCo = Mod.bTCo;
	Page4.OTDlg.bSIw = Mod.bSIw;
	Page4.OTDlg.bSIo = Mod.bSIo;
	Page4.OTDlg.bLQw = Mod.bLQw;
	Page4.OTDlg.bLQo = Mod.bLQo;
	Page4.OTDlg.bCPw = Mod.bCPw;
	Page4.OTDlg.bCPo = Mod.bCPo;
	Page4.OTDlg.bPw = Mod.bPw;
	Page4.OTDlg.bPo = Mod.bPo;
	Page4.OTDlg.bMass = Mod.bMass;
	*/
	PropPage5 Page5;

	if (Mod.iModel == 2)
	{
		Sheet.AddPage(&Page5);

//		Page5.AnLinModel = Mod.AnalLinModel;

		if (Page5.AnLinModel.Kiso > 0) Page5.m_Kiso = 0;

		else Page5.m_Kiso = 1;

		if (Page5.AnLinModel.nSpace == 1) Page5.m_nSpace = 0;

		else Page5.m_nSpace = 1;
	}

	if (Mod.iModel == 1)
	{
		FILE *filePtr;

		char buffer[128];

		CString sItem;

		sItem.Format("%s", "Lab Data\\");

		sItem += Basic2PhaseDlg.m_Edit_IsoTable.FTFileName;

		if ((filePtr = fopen(Basic2PhaseDlg.m_Edit_IsoTable.PathName + sItem, "r")) == NULL)
		{
			sItem.Format("The file %s was not opened.\nMake sure is in the right directory.\nUpdate from Edits", Page4.m_Edit_FieldTable.FTFileName);
			AfxMessageBox(sItem);
		}
		else
		{
			printf("The file %s was opened\n", Page4.m_Edit_FieldTable.FTFileName);

			/* Set pointer to beginning of file: */
			fseek(filePtr, 0L, SEEK_SET);

			// validate the SQUEEZE data file and check the version
			fgets(buffer, 128, filePtr);

			fscanf(filePtr, "%d \n", &Mod.nPts);

			fgets(buffer, 128, filePtr);

			Mod.fX.resize(Mod.nPts);
			Mod.fY.resize(Mod.nPts);

			for (int i = 0; i < Mod.nPts; i++)

				fscanf(filePtr, "%lf %lf \n", &Mod.fX[i], &Mod.fY[i]);

			std::fclose(filePtr);
		}

		//	Save Edit File
		CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
		CFileDialog saveDialog(FALSE, _T("DAT"), PathNameReadFile, OFN_HIDEREADONLY |
			OFN_FILEMUSTEXIST, openFilter, this);

		if (saveDialog.DoModal() == IDOK)
		{
			PathNameReadFile = saveDialog.GetPathName();

			Mod.WriteFileSQZ5Radial(PathNameReadFile);
		}
		//	End Save Edit File
	}
	else
	{
		if (Sheet.DoModal() == IDOK)
		{

			Mod.radiusWell = Page1.WellRad;

			Mod.radiusMax = Page1.MaxRad;

			if (Mod.kTemp == 0)
			{
				Mod.MUw = Page1.MUw;
				Mod.MUo = Page1.MUo;
			}


			for (l = 0; l < Mod.nLay; l++)
			{
				Mod.SLay[l] = Page1.SLay[l];

			/*	if (Mod.nSimTyp == 1) //  1 = Squeeze VI, Two Phase
				{
					Mod.layer[l].dynamicLayer = Page1.DynamicLayer[l];

					if (Mod.layer[l].dynamicLayer.Type == 1) // Table Rel. Perm
					{
						FILE* fileRPC;

						CString sItem;

						sItem.Format("%s", "Relative Permeability\\");

						sItem += Mod.layer[l].dynamicLayer.RelPermTabFile;

						if ((fileRPC = fopen(PathName + sItem, "r")) == NULL)
						{
							CString st;
							st.Format("The file %s was not opened.\nMake sure is in the right directory.", sItem);
							AfxMessageBox(st);
						}
						else
						{
							char buffer[256];
							fseek(fileRPC, 0L, SEEK_SET);

							// validate the SQUEEZE data file and check the version
							fgets(buffer, 128, fileRPC);

							fscanf(fileRPC, "%d \n", &Mod.layer[l].dynamicLayer.nRP);

							fgets(buffer, 128, fileRPC);

							for (int i = 0; i < Mod.layer[l].dynamicLayer.nRP; i++)

								fscanf(fileRPC, "%lf %lf %lf\n", &Mod.layer[l].dynamicLayer.RpSw[i],
								&Mod.layer[l].dynamicLayer.RpKrw[i], &Mod.layer[l].dynamicLayer.RpKro[i]);

							std::fclose(fileRPC);
						}
					
					}
				}*/
			}

			Mod.IsoSpec = Page2.IsoSpec;

			for (l = 0; l < Page2.TempnLay; l++)
			{
				for (c = 0; c < Page2.nComp; c++)
				{
					Mod.Iso = Page2.oIso[l][c];

					Mod.Iso = Page2.wIso[l][c];
				}
			}

			Mod.wComp = Page2.waterComp;
			Mod.oComp = Page2.oilComp;

			for (c = 0; c < Page2.nComp; c++)
			{
				if (Mod.wComp[c].Role == 2) // Viscosifier
				{
					Mod.wComp[c].MaxMU = Page2.WaterGrid.VisDlg.waterComp.MaxMU;
					Mod.wComp[c].nPowMU = Page2.WaterGrid.VisDlg.waterComp.nPowMU;
					Mod.wComp[c].Shear = Page2.WaterGrid.VisDlg.waterComp.Shear;
					Mod.wComp[c].Landa = Page2.WaterGrid.VisDlg.waterComp.Landa;
					Mod.wComp[c].n = Page2.WaterGrid.VisDlg.waterComp.n;

					Mod.oComp[c].MaxMU = Page2.WaterGrid.VisDlg.oilComp.MaxMU;
					Mod.oComp[c].nPowMU = Page2.WaterGrid.VisDlg.oilComp.nPowMU;
					Mod.oComp[c].Shear = Page2.WaterGrid.VisDlg.oilComp.Shear;
					Mod.oComp[c].Landa = Page2.WaterGrid.VisDlg.oilComp.Landa;
					Mod.oComp[c].n = Page2.WaterGrid.VisDlg.oilComp.n;
				}

				if (Mod.wComp[c].Role == 4) // Surfactant
				{
					//Mod.layer[0].dynamicLayer.NewSwi = Page2.WaterGrid.MSDlg.NewSwi;
					//Mod.layer[0].dynamicLayer.NewSor = Page2.WaterGrid.MSDlg.NewSor;

					Mod.wComp[c].MAdso = Page2.WaterGrid.MSDlg.MAdso;

					Mod.wComp[c].IsoTypeAds = Page2.WaterGrid.MSDlg.IsoTyp;
					Mod.wComp[c].IrrAdso = Page2.WaterGrid.MSDlg.Irr;

					Mod.wComp[c].High_Ads1 = Page2.WaterGrid.MSDlg.Ads1;
					Mod.wComp[c].High_Ads2 = Page2.WaterGrid.MSDlg.Ads2;

				//	Mod.wIso[0][c].CompIsoTabFile = Page2.WaterGrid.MSDlg.TableName;
				}

				if (Mod.wComp[c].Role == 5) // Ca
				{
					Mod.wComp[c].IsoTypAct = Page2.WaterGrid.CaNaDlg.IsoTyp;

					Mod.wComp[c].Low_Ads1 = Page2.WaterGrid.CaNaDlg.Low_Ads1;
					Mod.wComp[c].Low_Ads2 = Page2.WaterGrid.CaNaDlg.Low_Ads2;

					Mod.wComp[c].High_Ads1 = Page2.WaterGrid.CaNaDlg.High_Ads1;
					Mod.wComp[c].High_Ads2 = Page2.WaterGrid.CaNaDlg.High_Ads2;

					Mod.wComp[c].coreConc = Page2.WaterGrid.CaNaDlg.CacoreConc;

					Mod.wComp[c].Low_Conc = Page2.WaterGrid.CaNaDlg.Low_Na;
					Mod.wComp[c].High_Conc = Page2.WaterGrid.CaNaDlg.High_Na;
				}

				if (Mod.wComp[c].Role == 6) // Na
				{
					Mod.wComp[c].coreConc = Page2.WaterGrid.CaNaDlg.NacoreConc;
				}

				if (Mod.wComp[c].Role == 7) // Bridging Agent
				{
					Mod.wComp[c].MAdso = Page2.WaterGrid.MSDlg.MAdso;

					Mod.wComp[c].IsoTypeAds = Page2.WaterGrid.MSDlg.IsoTyp;
					Mod.wComp[c].IrrAdso = Page2.WaterGrid.MSDlg.Irr;

					Mod.wComp[c].High_Ads1 = Page2.WaterGrid.MSDlg.Ads1;
					Mod.wComp[c].High_Ads2 = Page2.WaterGrid.MSDlg.Ads2;

					Mod.wComp[c].ModAdd = Page2.WaterGrid.MSDlg.ModAdd;
					Mod.wComp[c].MaxAdso = Page2.WaterGrid.MSDlg.MaxAdso;

					//Mod.wIso[0][c].CompIsoTabFile = Page2.WaterGrid.MSDlg.TableName;
				}
			}

		/*	for (l = 0; l < Mod.nLay; l++)
			{
				Mod.RFLay[l] = Page2.WaterGrid.BloDlg.rfDlg;
				Mod.layer[l].dynamicLayer.NewSwi = Page2.WaterGrid.MSDlg.NewSwi;
				Mod.layer[l].dynamicLayer.NewSor = Page2.WaterGrid.MSDlg.NewSor;
			}
*/
			Mod.NoTstep = Page3.NoTstep;

			Mod.nFreq = Page4.nFreq;

			Mod.nIters = Page4.nIters;

			if (Mod.nSimTyp == 1) //  1 = Squeeze VI
			{
				Mod.TolSQ5 = Page4.TolItr;
			}

			if (Mod.nSimTyp == 0) // 0 = Squeeze V,
			{
				Mod.TolSQ5 = Page4.TolItr;
			}

			Mod.nLife = Page4.nLife;

			for (i = 0; i < Mod.nLife; i++)

				Mod.Clife[i] = Page4.Clife[i];

			for (i = 0; i< Mod.nOlds; i++)

				strcpy(Mod.FileNm[i].GetBuffer(), Page4.FileNm[i]);


			/*			Mod.nPts = Page4.m_Edit_FieldTable.FieldTable.NIso;

			for( i = 0 ; i < Page4.m_Edit_FieldTable.FieldTable.NIso ; i++ )
			{
			Mod.fx[i] = Page4.m_Edit_FieldTable.FieldTable.Time[i];

			Mod.fy[i] = Page4.m_Edit_FieldTable.FieldTable.Conc[i];

			}
			*/
			CString sItem;

			FILE *filePtr;

			char buffer[128];

			if (Mod.iModel == 4)

				sItem.Format("%s", "Lab Data\\");

			else

				sItem.Format("%s", "Field Data\\");

			sItem += Page4.m_Edit_FieldTable.FTFileName;

			if ((filePtr = fopen(Page4.m_Edit_FieldTable.PathName + sItem, "r")) == NULL)
			{
				sItem.Format("The file %s was not opened.\nMake sure is in the right directory.\nUpdate from Edits", Page4.m_Edit_FieldTable.FTFileName);
				Mod.nPts = 0;
				//	AfxMessageBox(sItem);
			}
			else
			{
				Mod.FieldTabFileName = Page4.m_Edit_FieldTable.FTFileName;

				printf("The file %s was opened\n", Page4.m_Edit_FieldTable.FTFileName);

				/* Set pointer to beginning of file: */
				fseek(filePtr, 0L, SEEK_SET);

				// validate the SQUEEZE data file and check the version
				fgets(buffer, 128, filePtr);

				fscanf(filePtr, "%d \n", &Mod.nPts);

				fgets(buffer, 128, filePtr);

				Mod.fX.resize(Mod.nPts);
				Mod.fY.resize(Mod.nPts);

				for (int i = 0; i < Mod.nPts; i++)

					fscanf(filePtr, "%lf %lf \n", &Mod.fX[i], &Mod.fY[i]);

				std::fclose(filePtr);

			}

			if (Mod.nSimTyp == 1) //Two Phase

				Mod.kPlot = Page4.kPlot;

			else
			{
				if (Page4.m_Return_Curve_Type == 0) Mod.kPlot = 0;

				if (Page4.m_Return_Curve_Type == 1) Mod.kPlot = 1;

				if (Page4.m_Return_Curve_Type == 3) Mod.kPlot = 2;

				if (Page4.m_Return_Curve_Type == 2) Mod.kPlot = 3;
			}

			(Page4.m_Bool_Screen_Display ? Mod.iAnim = 1 : Mod.iAnim = 0);

			(Page4.m_Bool_IsCon ? Mod.IsCon = 1 : Mod.IsCon = 0);

			(Page4.m_Bool_IsTmp ? Mod.IsTmp = 1 : Mod.IsTmp = 0);

			c = 0;
			i = 0;

			Mod.nOlds = 0;

			Check = TRUE;

			while (Check)
			{
				sItem.Format("Click here %d", i + 1);

				if (i >= MaxFileNm / 2) c = 1;

				if (strcmp(Page4.OldFileGrid.colS[c][i % (MaxFileNm / 2)], sItem) != 0)
				{
					Mod.FileNm.push_back(Page4.OldFileGrid.colS[c][i % (MaxFileNm / 2)]);

					++Mod.nOlds;

				}

				else Check = FALSE;

				i++;

				if (i >= MaxFileNm) Check = FALSE;
			}

			Mod.GrafMIC = Page4.GrafMIC;

			Mod.Cmax = Page4.GPDlg.MaxConc;

			Mod.Cmin = Page4.GPDlg.MinConc;

			Mod.kCrange = Page4.GPDlg.kCrange;

			Mod.kGraf1 = Page4.GPDlg.kGraf1;

			Mod.kGraf2 = Page4.GPDlg.kGraf2;

			Mod.kGraf3 = Page4.GPDlg.kGraf3;

			Mod.nDebug = Page4.GPDlg.nDebug;

			i = 0;

/*			for (l = 0; l < Page4.nLay; l++)
			{
				if (Page4.GPDlg.nGraf2[l])
				{
					Mod.nGraf2[l] = Page4.GPDlg.nGraf2[l];

					i++;

				}
			}

			i = 0;

			for (l = 0; l < Page4.nLay; l++)
			{
				if (Page4.GPDlg.nGraf3[l])
				{
					Mod.nGraf3[i] = Page4.GPDlg.nGraf3[l];

					i++;

				}
			}
			*/
			/*
			Mod.bCw = Page4.bCw;
			Mod.bCo = Page4.bCo;
			Mod.bAdsW = Page4.bAdsW;
			Mod.bAdsO = Page4.bAdsO;
			Mod.bViscW = Page4.bViscW;
			Mod.bViscO = Page4.bViscO;
			Mod.bRFW = Page4.bRFW;
			Mod.bRFO = Page4.bRFO;
			Mod.bOutputFile = Page4.bOutputFile;
			Mod.bSW = Page4.bSW;
			Mod.bFW = Page4.bFW;

			Mod.bOutputFileTime = Page4.bOutputFileTime;
			Mod.bWC = Page4.OTDlg.bWC;
			Mod.bTQw = Page4.OTDlg.bTQw;
			Mod.bTQo = Page4.OTDlg.bTQo;
			Mod.bTCw = Page4.OTDlg.bTCw;
			Mod.bTCo = Page4.OTDlg.bTCo;
			Mod.bSIw = Page4.OTDlg.bSIw;
			Mod.bSIo = Page4.OTDlg.bSIo;
			Mod.bLQw = Page4.OTDlg.bLQw;
			Mod.bLQo = Page4.OTDlg.bLQo;
			Mod.bCPw = Page4.OTDlg.bCPw;
			Mod.bCPo = Page4.OTDlg.bCPo;
			Mod.bPw = Page4.OTDlg.bPw;
			Mod.bPo = Page4.OTDlg.bPo;
			Mod.bMass = Page4.OTDlg.bMass;
			*/

			if (Page5.m_Kiso == 0) Page5.AnLinModel.Kiso = 1;

			if (Page5.m_Kiso == 1) Page5.AnLinModel.Kiso = -1;

			if (Page5.m_nSpace == 0) Page5.AnLinModel.nSpace = 1;

			if (Page5.m_nSpace == 1) Page5.AnLinModel.nSpace = 0;

			//Mod.AnalLinModel = Page5.AnLinModel;


			if (Mod.iModel == 3)
			{	
				Mod.stg = Page3.StageGrid.StgDlg.Stage;

				for (int s = 0; s < Mod.stg.size(); s++)
				{
					Mod.stg[s].objFlowRate = Page7.P7SqzOpt->oStg[s].objFlowRate;
					Mod.stg[s].objIndexFlowRate = Page7.P7SqzOpt->oStg[s].objIndexFlowRate;
					Mod.stg[s].objIndexSI = Page7.P7SqzOpt->oStg[s].objIndexSI;
					Mod.stg[s].objIndexTime = Page7.P7SqzOpt->oStg[s].objIndexTime;
					Mod.stg[s].objSI = Page7.P7SqzOpt->oStg[s].objSI;
					Mod.stg[s].objTime = Page7.P7SqzOpt->oStg[s].objTime;
				}
			}

			Mod.SqzOpt.nPar = Page7.P7SqzOpt->nPar;

			Mod.SqzOpt.X = Page7.P7SqzOpt->X;
			Mod.SqzOpt.Vol = Page7.P7SqzOpt->Vol;
			Mod.SqzOpt.lX = Page7.P7SqzOpt->lX;
			Mod.SqzOpt.uX = Page7.P7SqzOpt->uX;

			Mod.SqzOpt.InjHourRate = Page7.P7SqzOpt->InjHourRate;
			Mod.SqzOpt.SIBblRate = Page7.P7SqzOpt->SIBblRate;
			Mod.SqzOpt.MIC = Page7.P7SqzOpt->MIC;
			Mod.SqzOpt.Obj_T_V = Page7.P7SqzOpt->Obj_T_V;
			Mod.SqzOpt.nGen = Page7.P7SqzOpt->nGen;
			Mod.SqzOpt.nPopSize = Page7.P7SqzOpt->nPopSize;
			Mod.SqzOpt.BinsMT = Page7.P7SqzOpt->BinsMT;
			Mod.SqzOpt.IterationsSecant = Page7.P7SqzOpt->IterationsSecant;
			Mod.SqzOpt.nBins = Page7.P7SqzOpt->nBins;
			Mod.SqzOpt.SensCalc = Page7.P7SqzOpt->SensCalc;
			Mod.SqzOpt.Only_SI = Page7.P7SqzOpt->Only_SI;
			Mod.SqzOpt.Only_Tot_Vol = Page7.P7SqzOpt->Only_Tot_Vol;
			Mod.SqzOpt.Fixed_Target = Page7.P7SqzOpt->Fixed_Target;
			Mod.SqzOpt.TotSIVol = Page7.P7SqzOpt->TotSIVol;
			Mod.SqzOpt.TotInjVol = Page7.P7SqzOpt->TotInjVol;

			//	Save Edit File
			CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
			CFileDialog saveDialog(FALSE, _T("dat"), PathNameReadFile, OFN_HIDEREADONLY |
				OFN_FILEMUSTEXIST, openFilter, this);

			//			DlgWar->bPhi = DlgWar->bKabs = DlgWar->bHLay = DlgWar->bSrw = DlgWar->bSro = DlgWar->bSwi = FALSE;

			if (saveDialog.DoModal() == IDOK)
			{
				int count = 0;

				PathNameReadFile = saveDialog.GetPathName();

			/*	for (l = 0; l<Mod.nLay; l++)
				{
					if( Mod.SLay[l].Phi > 1 || Mod.SLay[l].Phi <= 0 )
					{
					DlgWar->bPhi = TRUE;

					++count;
					}

					if( Mod.SLay[l].KAbs > 10000 || Mod.SLay[l].KAbs <= 0 )
					{
					DlgWar->bKabs = TRUE;

					++count;
					}

					if( Mod.SLay[l].HLay <= 0 )
					{
					DlgWar->bHLay = TRUE;

					++count;
					}

					if( Mod.layer[l].dynamicLayer.Srw > 1 || Mod.layer[l].dynamicLayer.Srw < 0 )
					{
					DlgWar->bSrw = TRUE;

					++count;
					}
					if( Mod.layer[l].dynamicLayer.Sro > 1 || Mod.layer[l].dynamicLayer.Sro < 0 )
					{
					DlgWar->bSro = TRUE;

					++count;
					}
					if( Mod.layer[l].dynamicLayer.Swi > 1 || Mod.layer[l].dynamicLayer.Swi < 0 )
					{
					DlgWar->bSwi = TRUE;

					++count;
					}
					
				}*/

				if (Mod.iModel == 3)
				{
					SqzOpt = *Page7.P7SqzOpt;
				}

				//if (Mod.iModel == 2)

				//	WriteFileAnaliticalLinear(PathNameReadFile);

				if (Mod.iModel == 3)
				{
					/*if (Mod.nSimTyp == 1)

						WriteFileRadial(PathNameReadFile);*/

					if (Mod.nSimTyp == 0)

						Mod.WriteFileSQZ5Radial(PathNameReadFile);
				}

				if (Mod.iModel == 4)
				{
					//if (saveDialog.GetFileExt() == "inp") 
					/*if (Mod.nSimTyp == 1)

						WriteFileLinear(PathNameReadFile);*/

					//if (saveDialog.GetFileExt() == _T("DAT")) 
					if (Mod.nSimTyp == 0)

						Mod.WriteFileSQZ5Radial(PathNameReadFile);
				}

				/*if (Mod.iModel == 5 || Mod.iModel == 1)
				{
					WriteFileSQZ5Isotherm(PathNameReadFile);
				}*/

				if (m_Button_Warning.GetCheck() && count > 0)
				{
					/*					DlgWar->width = 288 ;
					DlgWar->height = 132 + 25*(count-1);

					DlgWar->MoveWindow(100, 100, DlgWar->width, DlgWar->height);
					DlgWar->ShowWindow(!DlgWar->IsWindowVisible());

					DlgWar->GetDlgItem(IDC_ICON_SQVI)->MoveWindow(40,14,25,25);
					DlgWar->GetDlgItem(IDC_ICON_SQVI)->ShowWindow(TRUE);

					DlgWar->GetDlgItem(IDC_STATIC_SQVI)->MoveWindow(DWRect.right/3,DWRect.bottom/6,2*DWRect.right/3,15);
					DlgWar->GetDlgItem(IDC_STATIC_SQVI)->ShowWindow(TRUE);

					DlgWar->GetDlgItem(IDOK)->MoveWindow(DlgWar->width/2-30,DlgWar->height-50,60,20);
					DlgWar->GetDlgItem(IDOK)->ShowWindow(TRUE);

					DlgWar->WriteWarnings();
					*/
				}
			}
			//	End Save Edit File	
		}
	}

	m_bRunModel.EnableWindow(TRUE);
}

void BasicDlg::UpdateGraphCross(CString sEdit)
{
	int b, l;

	for (l = 0; l<Mod.nLay; l++)
	{
		for (b = 0; b<Mod.nBlocks; b++)
		{
			GraphCross.GraphXY.y[b][l] = Mod.C_L_Out[0][b][l];
		}

	}

	GraphCross.Num = Mod.nBlocks;

	GraphCross.yMin = 0;

	GraphCross.xMin = 0;
	GraphCross.xMax = GraphCross.GraphXY.x[Mod.nBlocks - 1];;

	GraphCross.Stage = sEdit;

	GraphCross.Invalidate(NULL);

}


void BasicDlg::OnRunModel()
{
	// TODO: Add your control notification handler code here
	int i, SQ;

	char st[2000],
		Val[100];

	CString sTemp,
			sTemp1,
			outFileName,
			pathname,
			pathnameSQ5;

	WORD wReturn;
	char szMsg[100];

	m_Slider_SP_TS.SetPos(100);
	quitRun = STOP = false;

	//	GraphTime[0].MicConc = Mod.GrafMIC;

	CString openFilter = "Ouput files (*.out)|*.out|All Files (*.*)|*.*||";

	if (Mod.nSimTyp == 0) // Squeeze V

		pathname = PathNameReadFile;

	if (Mod.nSimTyp == 1) // Squeeze V

		pathname = PathNameReadFile;

	pathname.Replace(".dat", ".out");
	pathname.Replace(".DAT", ".out");

	//pathname = PathName+"OutputFiles\\"+Mod.ReadFileName;
	//	pathname.Replace(Mod.ReadFileName,"OutputFiles\\"+Mod.ReadFileName);

	CFileDialog saveDialog(FALSE, _T("out"), pathname, OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	for (i = 0; i<nGraphRad; i++)
	{
		GraphRad[i].bGraphs = m_Button_Graphs.GetCheck();

		if (Mod.title1.GetLength() < 30)
			GraphRad[i].Ptitle = Mod.title1;
		else
		{
			GraphRad[i].Ptitle = Mod.title1;
		//	GraphRad[i].Ptitle[30] = '\0';
		}
	}

	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].bGraphs = m_Button_Graphs.GetCheck();

		GraphTime[i].Ptitle = Mod.title1;
	}

	if (saveDialog.DoModal() == IDOK)
	{
		check = true;

		pathname = saveDialog.GetPathName();
		outFileName = saveDialog.GetFileName();

		// New version that runs the file that you save	
		//	if (SQ ==0 ) // Squeeze V
		if (Mod.nSimTyp == 0) // Squeeze V
		{
			SRD.MoveWindow(200, 200, 260, 100);
			SRD.ShowWindow(true);
			SRD.Update(false, "");
			ProcessMessageLoop();

			//	Isotherm Derivation 
			if (Mod.iModel == 5 || Mod.iModel == 1)
			{	
				IsoDerGrp.Mod.IsothermDerivation(PathNameReadFile);

				SRD.ShowWindow(false);

				HideGraph();

				if (IsoDerGrp.DoModal() == IDOK)
				{
					;
				}
			}
			
			// Radial
			if (Mod.iModel == 3)
			{
				GetDlgItem(IDC_GRAPH_CROSS2)->EnableWindow(true);
				GetDlgItem(IDC_GRAPH_RAD)->EnableWindow(true);

				if (Mod.iModel == 2)

					Mod.nPts = 0;

				Particle p;
				Mod.nOpt = -1;

				
				Mod.RunSQV_SP(p);
				
				// Reading Old files
				GraphTime[0].nOutFiles = Mod.nOlds;
				GraphTime[0].GraphXY.pFile.resize(Mod.nOlds);

				GraphRad[0].kTemp = Mod.kTemp;
				GraphRad[1].kTemp = Mod.kTemp;

				GraphRad[0].RT = Mod.RT;
				GraphRad[1].RT = Mod.RT;

				for (int j = 0; j<Mod.nOlds; j++)
				{
					ReadOutputFile(j);

					GraphTime[0].GraphXY.pFile[j].NameFile = Mod.FileNm[j];
				}
				// End of Reading Old files 

				UpdateRun(st);

				Mod.Print_Output(pathname);

				SRD.ShowWindow(false);
				ShowGraph();

				if (Mod.iModel == 4 || Mod.iModel == 2)
				{
					GraphCross.ShowWindow(false);
					GetDlgItem(IDC_GRAPH_CROSS2)->EnableWindow(false);
					GraphTime[1].ShowWindow(false);
					GraphRad[0].ShowWindow(false);
					GraphRad[1].ShowWindow(false);
					GetDlgItem(IDC_GRAPH_RAD)->EnableWindow(false);
				}
			}
		}

		//	if( SQ == 1 ) // Squeeze VI
		if (Mod.nSimTyp == 1) // Squeeze VI
		{
			if (m_Button_Graphs.GetCheck() == false)

				SRD.ShowWindow(true);

			m_bReadFile.EnableWindow(FALSE);
			m_bBasicParam.EnableWindow(FALSE);
			m_bEditModel.EnableWindow(FALSE);
			m_bRunModel.EnableWindow(FALSE);
			m_Button_Close.EnableWindow(FALSE);
			m_bAbortRun.EnableWindow(TRUE);

			// Radial Model
			if (Mod.iModel == 3)

				Mod.ConvertUnits();

			//	Mod.MaxConcentration();

			GraphRad[0].SetWindowText("SQZ Layer Water Saturation");
			GraphRad[1].SetWindowText("SQZ Layer Concentration Profile");

			// Foward and Backward Graphics
			UpdateRun(st);
			SRD.ShowWindow(false);
			GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
			GetDlgItem(IDC_SLIDER_SP_TS)->EnableWindow(true);
			ShowGraph();
		}
	}
	else

		pathname = Mod.ReadFileName;

	ProcessMessageLoop();

}


void BasicDlg::UpdateGraphRad(CString sEdit)
{
	int i, b, l;

	//double MaxAds;

	for (l = 0; l<Mod.nLay; l++)
	{
		//	MaxAds = Mod.Isotherm(Mod.waterComp[0].MaxC, 0, l, WAT );

		for (b = 0; b<Mod.nBlocks; b++)
		{
			GraphRad[0].GraphXY.y1[b][l][0] = Mod.SW_L_Out[b][l][0];

			GraphRad[1].GraphXY.y1[b][l][0] = Mod.C_L_Out[b][l][0] / Mod.wComp[0].MaxC;

			if (MaxAds > 0)

				GraphRad[1].GraphXY.y1[b][l][1] = Mod.AD_L_Out[b][l][0] / MaxAds;

			else

				GraphRad[1].GraphXY.y1[b][l][1] = Mod.AD_L_Out[b][l][0];

			GraphRad[1].GraphXY.y1[b][l][2] = Mod.FTemp_L_Out[b][l][0] / Mod.Temp.Ftemp;
		}

	}

	for (i = 0; i<nGraphRad; i++)
	{
		GraphRad[i].xMin = 0;

		GraphRad[i].xMax = GraphRad[i].GraphXY.x[Mod.nBlocks - 1];

		GraphRad[i].yMin = 0;
	}

	GraphRad[0].yMax = 1.0;

	GraphRad[1].yMax = 1.0;

	//	GraphRad[1].yMax = 1e3 * Mod.waterComp[0].MaxC;

	for (i = 0; i<nGraphRad; i++)
	{
		GraphRad[i].Stage = sEdit;

		GraphRad[i].Invalidate(NULL);
	}
}



void BasicDlg::UpdateGraphTime(int MNTcount, CString sEdit)
{
	int i, l, j, t, n;
	double ConvQ,
		CumProWat;

	GraphTime[0].PathNameReadFile = PathNameReadFile;
	GraphTime[1].PathNameReadFile = PathNameReadFile;

	if (Mod.UnitSpec == 'M' || Mod.UnitSpec == 'm')
	{
		ConvQ = 0.158987295*1e-2; // 100 m3
	}

	if (Mod.UnitSpec == 'C' || Mod.UnitSpec == 'c')
	{
		ConvQ = 1 * 1e-3; // 1000 bbls
	}

	CumProWat = 0;

//	for (l = 0; l<Mod.nLay; l++)

//		CumProWat += Mod.cumProWater[l];

	GraphTime[0].Num = MNTcount;

	GraphTime[0].GraphXY.AddElement();

	GraphTime[1].Num = MNTcount;

	GraphTime[1].GraphXY.AddElement();

	if (Mod.iModel == 3)
	{
		if (Mod.kPlot == 0) // Time

			GraphTime[0].GraphXY.x[MNTcount] = Mod.RT[Mod.RT.size()-1] / 86400;

		if (Mod.kPlot == 1) // Cumulative Water Produced  

			GraphTime[0].GraphXY.x[MNTcount] = (Mod.RT[Mod.RT.size() - 1] / 0.158987295)  * ConvQ;
	}

	if (Mod.iModel == 4)

		GraphTime[0].GraphXY.x[MNTcount] = Mod.RT[Mod.RT.size() - 1];
	//	GraphTime[0].GraphXY.x[MNTcount] = Mod.PV;

	GraphTime[0].SetXYMaxMin();

	if (Mod.wComp[0].MaxC > 0)
		GraphTime[0].yMax = log10(1e3 * Mod.wComp[0].MaxC);
	else
		GraphTime[0].yMax = log10(10.0);

	for (i = 0; i<nGraphTime; i++)
	{
		if (Mod.iModel == 3)

			GraphTime[i].GraphXY.x[MNTcount] = GraphTime[0].GraphXY.x[MNTcount];

		if (Mod.iModel == 4)
			GraphTime[i].GraphXY.x[MNTcount] = Mod.RT[Mod.RT.size() - 1];
			//GraphTime[i].GraphXY.x[MNTcount] = Mod.PV;

		//		GraphTime[i].GraphXY.MaxNumTimes = MNTcount;

		GraphTime[i].Num = MNTcount;

		GraphTime[i].xMax = GraphTime[0].xMax;

		GraphTime[i].xMin = GraphTime[0].xMin;

		GraphTime[i].yMax = GraphTime[0].yMax;

		GraphTime[i].yMin = log10(yMinGT);

	}

	if (Mod.RT[0] > yMinGT)

		GraphTime[0].GraphXY.y1[MNTcount][0][0] = log10(Mod.RT[0]);

	else

		GraphTime[0].GraphXY.y1[MNTcount][0][0] = log10(yMinGT);

	for (l = 0; l<Mod.nLay; l++)
	{
		if (Mod.RC_L_Out[l][0] > yMinGT)

			GraphTime[1].GraphXY.y1[MNTcount][l][0] = log10(Mod.RC_L_Out[l][0]);

		else

			GraphTime[1].GraphXY.y1[MNTcount][l][0] = log10(yMinGT);


		GraphTime[0].Mic[l].wConc = Mod.WC_L_Out[l][0];
		GraphTime[0].Mic[l].wprd = Mod.WPRD[l][0];
	}
	
	for (i = 0; i<nGraphTime; i++)
	{
		//		GraphTime[i].GraphXY.MaxNumTimes = MNTcount+1;
		GraphTime[i].Stage = sEdit;
		GraphTime[i].Invalidate();
	}

}

void BasicDlg::Allocate()
{
	int l, b;

	nGraphTime = 2;
	nGraphRad = 2;

	GraphTime = new Graphics[nGraphTime];
	GraphRad = new Graphics[nGraphRad];
	/*
	DlgWar = new DlgWarning;
	*/
	nL = 1;
	nB = 1;

}

void BasicDlg::DeAllocate()
{
	int l, b;

	delete[] GraphTime;
	delete[] GraphRad;

	
}

void BasicDlg::ChartInitialisation()
{
	int i, j, l;

	CString sEdit;

	yMinGT = 0.1;

	MNTcount = 0;

	for (l = 0; l<Mod.nLay; l++)

		MaxAds = Mod.Geq(Mod.wComp[0].MaxC);

	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].logScale = 0;

		if (i == 0)
		{
			GraphTime[0].bMic = TRUE;

			GraphTime[0].MicConc = Mod.GrafMIC;

			GraphTime[i].GraphXY.SetMaxNumTimes(MNTcount + 1, 1, 1, Mod.nPts, Mod.nOlds);

			GraphTime[i].nOutFiles = Mod.nOlds;

			for (j = 0; j<Mod.nOlds; j++)
			{
				ReadOutputFile(j);
				CString sedit(Mod.FileNm[j]);
				sedit.Replace(".out", "");

				GraphTime[0].GraphXY.pFile[j].NameFile = sedit;
			}

			GraphTime[i].Mic.resize(Mod.nLay);
		}

		if (i == 1)

			GraphTime[i].GraphXY.SetMaxNumTimes(MNTcount + 1, Mod.nLay, 1, 0, 0);

		if (Mod.iModel == 3)
		{
			if (Mod.kPlot == 0)

				GraphTime[i].xLabel = "Time (days)";

			else
			{
				if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

					GraphTime[i].xLabel = "Vol (100m3)";

				if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

					GraphTime[i].xLabel = "Vol (1000bbls)";
			}
		}

		if (Mod.iModel == 4) GraphTime[i].xLabel = "Pore Volumes";

		//if (Mod.iModel == 2) GraphTime[i].xLabel = "Cumulative Production in bbls";

		GraphTime[i].yLabel = "Concentration (ppm)";
	}


	for (i = 0; i<Mod.nLay; i++)
	{
		GraphTime[0].Mic[i].Height = Mod.SLay[i].HLay;
	}

	// Set yMax and yMin

	GraphTime[0].yMin = yMinGT;

	if (Mod.wComp[0].MaxC > 0)
	{
		GraphTime[0].yMax = log10(1e3 * Mod.wComp[0].MaxC);
		GraphTime[1].yMax = log10(1e3 * Mod.wComp[0].MaxC);
	}
	else
	{
		GraphTime[0].yMax = log10(10.0);
		GraphTime[1].yMax = log10(10.0);
	}

	sEdit.Format("SQZ Total Return Conc. %s", TitleNameReadFile);
	GraphTime[0].SetWindowText(sEdit);

	sEdit.Format("SQZ Layer Return Conc. %s", TitleNameReadFile);
	GraphTime[1].SetWindowText(sEdit);

	GraphRad[0].GraphXY.SetMaxNumTimes(Mod.nBlocks, Mod.nLay, 1, 1, 0);

	GraphRad[0].SetNum(Mod.nBlocks);

	GraphRad[1].GraphXY.SetMaxNumTimes(Mod.nBlocks, Mod.nLay, 3, 0, 0);

	GraphRad[1].SetNum(Mod.nBlocks);

	for (i = 0; i<nGraphRad; i++)
	{
		if (Mod.iModel == 3)
		{
			if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

				GraphRad[i].xLabel = "Radius (m)";

			if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

				GraphRad[i].xLabel = "Radius (ft)";
		}

		if (Mod.iModel == 4)
		{
			GraphRad[i].xLabel = "Distance (ft)";
		}

		for (j = 0; j<Mod.nBlocks; j++)
		{
			if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

				GraphRad[i].GraphXY.x[j] = (Mod.R(j,Mod.dr) + Mod.R(j+1, Mod.dr)) / (2.0);

			if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

				GraphRad[i].GraphXY.x[j] = (Mod.R(j, Mod.dr) + Mod.R(j + 1, Mod.dr)) / (0.3048 * 2.0);
		}
	}

	// Set yMax

	GraphRad[0].yMax = 1.0;

	GraphRad[1].yMax = 1.0;
	//	GraphRad[1].yMax = 1e3 * Mod.waterComp[0].MaxC;

	sEdit.Format("SQZ Layer Wat Sat. %s", TitleNameReadFile);
	GraphRad[0].SetWindowText(sEdit);

	GraphRad[0].yLabel = "Water Saturation";

	sEdit.Format("SQZ Layer Conc. Profile %s", TitleNameReadFile);
	GraphRad[1].SetWindowText(sEdit);

	GraphRad[1].yLabel = "Normalized Conc,Adso,Temp";

	GraphRad[1].title = "Conc        Adso        Temp";

	GraphRad[1].bTitle = TRUE;

	GraphRad[1].Stage.Format("Stage 1: Inj/Preflush");

	sEdit.Format("SQZ Cross Section Inh.Conc. %s", TitleNameReadFile);

	GraphCross.SetWindowText(sEdit);

	GraphCross.title = Mod.title1;
	
	//	GraphCross.title = "Mobile Inhibitor Concentration (ppm)";

	if (Mod.iModel == 3)
	{
		if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

			GraphCross.xLabel = "Radius (m)";

		if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

			GraphCross.xLabel = "Radius (ft)";
	}

	if (Mod.iModel == 4)
	{
		GraphCross.xLabel = "Distance (ft)";
	}

	GraphCross.yLabel = "Height (feet)";

	GraphCross.SetXYMaxMin(Mod.nBlocks + 1, Mod.nLay);

	GraphCross.GraphXY.SetMaxNumTimes(Mod.nBlocks + 1, Mod.nLay, 1, 0, 0);

	double trans;

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

		trans = 1.0;

	if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

		trans = 0.3048;

	for (j = 0; j <= Mod.nBlocks; j++)

		GraphCross.GraphXY.x[j] = Mod.R(j,Mod.dr) / trans;

	GraphCross.yMax = 0;

	for (l = 0; l<Mod.nLay; l++)
	{
		GraphCross.layHeight[l] = Mod.SLay[l].HLay / trans;

		GraphCross.yMax += Mod.SLay[l].HLay;
	}

	GraphCross.yMax /= trans;

	GraphCross.yMin = 0;

}

int BasicDlg::ProcessMessageLoop() {

	MSG msg;
	while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

		if (WM_QUIT == msg.message) {
			// Abort all processing
			::PostQuitMessage(0);
			break;
		}

		//		::IsDialogMessage(m_hWnd, &msg);

		if (!AfxGetApp()->PreTranslateMessage(&msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
	}
	/*
	if (!::IsDialogMessage(m_hWnd, &msg)) {
	::TranslateMessage(&msg);
	::DispatchMessage(&msg);
	}
	}
	*/
	// let MFC do its idle processing
	AfxGetApp()->OnIdle(0);
	AfxGetApp()->OnIdle(1);
	// Perform some background processing here 
	// using another call to OnIdle

	return 0;
}


void BasicDlg::OnGraphCross()
{
	GraphCross.ShowWindow(!GraphCross.IsWindowVisible());
	int n = m_Slider_SP_TS.GetPos();

	if (Mod.nSimTyp == 0) //Single Phase

		UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	ProcessMessageLoop();
}

void BasicDlg::OnGraphTime()
{
	// TODO: Add your control notification handler code here
	int i;

	if (Mod.iModel == 4)

		GraphTime[0].ShowWindow(!GraphTime[0].IsWindowVisible());

	else
	{
		for (i = 0; i<nGraphTime; i++)

			GraphTime[i].ShowWindow(!GraphTime[i].IsWindowVisible());
	}

	int n = m_Slider_SP_TS.GetPos();

	if (Mod.nSimTyp == 0) //Single Phase

		UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	else

	if (quitRun)

		UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	ProcessMessageLoop();
}

void BasicDlg::OnGraphRad()
{
	// TODO: Add your control notification handler code here
	int i;

	for (i = 0; i<nGraphRad; i++)

		GraphRad[i].ShowWindow(!GraphRad[i].IsWindowVisible());

	int n = m_Slider_SP_TS.GetPos();

	if (Mod.nSimTyp == 0) //Single Phase

		UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	ProcessMessageLoop();

}

BOOL BasicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
	pSysMenu->AppendMenu(MF_SEPARATOR);
	pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}
	}
	*/
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	char full[_MAX_PATH];

	/* Get the current working directory: */
	if (_getcwd(full, _MAX_PATH) == NULL)
	{
		perror("_getcwd error");
		AfxMessageBox(full);
	}
	else
	{
		strcat(full, "\\");
		PathName = full;
		PathNameReadFile = PathName + CString("test.dat");
		Mod.PathName = PathName;
	}

	int d;

	int stat = CheckLicense(d);

	if (d < 30 && d >= 0)
	{
		CString st;
		st.Format("Days until License Expiration %d ", d);
		AfxMessageBox(st);
	}

	//stat = 0;

	if (stat != 0)
	{
		CString st;

		if (stat == -1) st = "No authorization for product\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk";

		if (stat == -2) st = "Authorization is for another ISV\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk";

		if (stat == -3) st.Format("Authorization has expired\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk");

		if (stat == -4) st = "Wrong host for authorization\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk";

		if (stat == -5) st = "Bad license file\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk";

		if (stat == -6) st = "Requested version not supported\nPlease contact:\nHeather O'Hara: H.F.OHara@hw.ac.uk\nOscar Vazquez: O.Vazquez@hw.ac.uk";


		AfxMessageBox(st);

		GetDlgItem(IDC_READ_FILE)->EnableWindow(false);
		GetDlgItem(IDC_ABORT_RUN)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PAR_RUN)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_MODEL1)->EnableWindow(false);
		GetDlgItem(IDC_RUN_MODEL)->EnableWindow(false);
		GetDlgItem(IDC_GRAPH_TIME)->EnableWindow(false);
		GetDlgItem(IDC_GRAPH_CROSS2)->EnableWindow(false);
		GetDlgItem(IDC_GRAPH_RAD)->EnableWindow(false);
		GetDlgItem(IDC_MOD)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_MOD)->EnableWindow(false);
		GetDlgItem(IDC_BASIC_PARA)->EnableWindow(false);
		GetDlgItem(IDC_SLIDER_TIME_DELAY)->EnableWindow(false);
		GetDlgItem(IDC_STATIC_ADDEDTIME)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FILES)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_OPT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ISOFITTING)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_BACK)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FOWARD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_PAR_OPT)->EnableWindow(false);
		GetDlgItem(IDC_STATIC)->EnableWindow(false);
	}
	else
	{
		if (Mod.nSimTyp == 1)
		{
			m_Static_Sim_Type.SetWindowText("Two-Phase Simulation");

			GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
			m_Button_Graphs.SetCheck(true);
			ShowGraph();
		}

		if (Mod.nSimTyp == 0)
		{
			m_Static_Sim_Type.SetWindowText("Single-Phase Simulation");
			/*	m_Button_Graph_Cross.EnableWindow(FALSE);
			m_Button_Graph_Time.EnableWindow(FALSE);
			m_Button_Graph_Rad.EnableWindow(FALSE);
			m_editMod.EnableWindow(FALSE);
			m_Static_AddedTime.EnableWindow(FALSE);
			m_Slider_TimeDelay.EnableWindow(FALSE);
			m_Button_MOD.EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);*/
			GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(false);
			ShowGraph();
		}

		CString sTemp;

		sTemp = Mod.title1;
		sTemp.Remove('\n');
		m_EditTitle1.SetWindowText(sTemp);
		sTemp = Mod.title2;
		sTemp.Remove('\n');
		m_EditTitle2.SetWindowText(sTemp);

		m_editMod.SetWindowText("10");


		sTemp.Format("Added time per loop: %5.3f s", (userTimeDelay) / 1000.0);
		m_Static_AddedTime.SetWindowText(sTemp);

		m_Slider_TimeDelay.SetRange(0, 100);
		m_Slider_TimeDelay.SetPageSize(10);
		m_Slider_TimeDelay.SetLineSize(1);
		m_Slider_TimeDelay.SetPos(userTimeDelay);
		m_Slider_TimeDelay.SetTicFreq(10);

		m_Slider_SP_TS.SetRange(0, 100);
		m_Slider_SP_TS.SetPageSize(10);
		m_Slider_SP_TS.SetLineSize(1);
		m_Slider_SP_TS.SetPos(100);
		m_Slider_SP_TS.SetTicFreq(10);

		CRect Temp;

		GetClientRect(Temp);

		MoveWindow(50, 15, Temp.Width() + 5, Temp.Height() + 25);

		FirstChartInit();

		m_bRunModel.EnableWindow(FALSE);

		m_bAbortRun.EnableWindow(FALSE);

		SRD.Create(IDD_DIALOG_SIM_RUN, this);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void BasicDlg::FirstChartInit()
{
	int i;

	GraphTime[0].SetWindowText("SQZ Total Return Concentration");

	GraphTime[1].SetWindowText("SQZ Layer Return Concentration");

	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].logScale = 0;

		GraphTime[i].yLabel = "Concentration (ppm)";

		if (Mod.iModel == 3)
		{
			if (Mod.kPlot == 0)

				GraphTime[i].xLabel = "Time (days)";

			else
			{
				if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

					GraphTime[i].xLabel = "Vol (100m3)";

				if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

					GraphTime[i].xLabel = "Vol (1000bbls)";
			}
		}

		if (Mod.iModel == 4) // Linear, coreflood

			GraphTime[i].xLabel = "Pore Volumes";

		if (Mod.iModel == 2) // Analitical Linear
		{
			if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

				GraphTime[i].xLabel = "Cumulative Production in m3";

			if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

				GraphTime[i].xLabel = "Cumulative Production in bbls";
		}
	}

	GraphRad[0].SetWindowText("SQZ Layer Water Saturation");

	GraphRad[1].SetWindowText("SQZ Layer Concentration Profile");

	for (i = 0; i<nGraphRad; i++)
	{
		if (Mod.iModel == 3)
		{
			if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

				GraphRad[i].xLabel = "Radius (m)";

			if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

				GraphRad[i].xLabel = "Radius (ft)";
		}

		if (Mod.iModel == 4)
		{
			GraphRad[i].xLabel = "Distance (ft)";
		}
	}

	GraphCross.SetWindowText("SQZ Cross Section Inh.Conc.");

	GraphCross.title = Mod.title1;

	//	GraphCross.title = "Mobile Inhibitor Concentration (ppm)";

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric
	{
		GraphCross.xLabel = "Radius (m)";
		GraphCross.yLabel = "Height (m)";
	}
	else
	{
		GraphCross.xLabel = "Radius (ft)";
		GraphCross.yLabel = "Height (feet)";
	}

}

void BasicDlg::OnMod()
{
	// TODO: Add your control notification handler code here
	CString szEdit;

	m_editMod.GetWindowText(szEdit);

	//	AfxMessageBox(szEdit);

	if (szEdit.IsEmpty() == FALSE)
	{
		szEdit.TrimRight();
		int n = atoi(szEdit);
	}
}

void BasicDlg::OnBasicPara()
{
	// TODO: Add your control notification handler code here
	int l;

	if (Mod.nSimTyp == 1)
	{
		Basic2PhaseDlg.m_Radio_Simulation = 1;
		m_Static_Sim_Type.SetWindowText("Two-Phase Simulation");
	}

	if (Mod.nSimTyp == 0)
	{
		Basic2PhaseDlg.m_Radio_Simulation = 0;
		m_Static_Sim_Type.SetWindowText("Single-Phase Simulation");
	}

//	Basic2PhaseDlg.EditTitle1 = Mod.title1;
//	Basic2PhaseDlg.EditTitle2 = Mod.title2;

	Basic2PhaseDlg.nTemp = Mod.kTemp;
	
	Basic2PhaseDlg.Ftemp = Mod.Temp.Ftemp;
	Basic2PhaseDlg.Kcr = Mod.Temp.Kcr;
	Basic2PhaseDlg.Kcs = Mod.Temp.Kcs;
	Basic2PhaseDlg.SHr = Mod.Temp.SHr;
	Basic2PhaseDlg.SHw = Mod.Temp.SHw;
	Basic2PhaseDlg.SHo = Mod.Temp.SHo;
	Basic2PhaseDlg.RHOw = Mod.Temp.RHOw;
	Basic2PhaseDlg.RHOr = Mod.Temp.RHOr;
	Basic2PhaseDlg.RHOo = Mod.Temp.RHOo;

	Basic2PhaseDlg.watTemp = Mod.Temp.watTemp;
	Basic2PhaseDlg.oilTemp = Mod.Temp.oilTemp;
	Basic2PhaseDlg.API = Mod.Temp.API;

	Basic2PhaseDlg.nLay = Mod.nLay;
	Basic2PhaseDlg.nB = Mod.nBlocks;
	Basic2PhaseDlg.nComp = Mod.nComponentsWater;
	Basic2PhaseDlg.nSt = Mod.nStg;

	Basic2PhaseDlg.CShut = Mod.Cshut;
//	Basic2PhaseDlg.TotVol = Mod.TotVol;
	Basic2PhaseDlg.VolInj = Mod.VI;
	Basic2PhaseDlg.InhCon = Mod.CI;
	Basic2PhaseDlg.Rho = Mod.rho;

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M')
	{
		Basic2PhaseDlg.m_Units = 0;
	}
	else
	{
		Basic2PhaseDlg.m_Units = 1;
	}

	if (Mod.iModel == 1) // Isotherm Derivation Model
	{
		Basic2PhaseDlg.m_ModelType = 3;
	}

	if (Mod.iModel == 2) // Linear Analytical Model
	{
		Basic2PhaseDlg.m_ModelType = 4;
	}

	if (Mod.iModel == 3) // Radial Model
	{
		Basic2PhaseDlg.m_ModelType = 0;
	}

	if (Mod.iModel == 4) // Linear Core Flood Model
	{
		Basic2PhaseDlg.m_ModelType = 1;
	}

	if (Mod.iModel == 5) // Isotherm Derivation & Validation Model
	{
		Basic2PhaseDlg.m_ModelType = 2;
	}

	Basic2PhaseDlg.nWF = Mod.WellFric;

	Basic2PhaseDlg.WellDia = Mod.WellDia;

	Basic2PhaseDlg.FluidRho = Mod.FluidRho;

	Basic2PhaseDlg.e = Mod.e;

	Basic2PhaseDlg.LaySpacing = Mod.LaySpacing;

	Basic2PhaseDlg.m_Edit_IsoTable.PathName = PathName;

	Basic2PhaseDlg.m_Edit_IsoTable.FTFileName = Mod.FieldTabFileName;

	if (Basic2PhaseDlg.DoModal() == IDOK)
	{
		if (Basic2PhaseDlg.check)
		{
			if (Basic2PhaseDlg.m_Radio_Simulation == 1)
			{
				Mod.nSimTyp = 1;
				m_Static_Sim_Type.SetWindowText("Two-Phase Simulation");
				m_Button_Graph_Cross.EnableWindow(TRUE);
				m_Button_Graph_Time.EnableWindow(TRUE);
				m_Button_Graph_Rad.EnableWindow(TRUE);
				m_editMod.EnableWindow(TRUE);
				m_Static_AddedTime.EnableWindow(TRUE);
				m_Slider_TimeDelay.EnableWindow(TRUE);
				m_Button_MOD.EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(TRUE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
			}

			if (Basic2PhaseDlg.m_Radio_Simulation == 0)
			{
				Mod.nSimTyp = 0;
				m_Static_Sim_Type.SetWindowText("Single-Phase Simulation");
				Mod.NoTstep = 0;
				/*	m_Button_Graph_Cross.EnableWindow(FALSE);
				m_Button_Graph_Time.EnableWindow(FALSE);
				m_Button_Graph_Rad.EnableWindow(FALSE);
				m_editMod.EnableWindow(FALSE);
				m_Static_AddedTime.EnableWindow(FALSE);
				m_Slider_TimeDelay.EnableWindow(FALSE);
				m_Button_MOD.EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_PLAY)->EnableWindow(FALSE);
				GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);*/
			}

			Mod.UnitSpec = (Basic2PhaseDlg.m_Units == 1) ? 'C' : 'M';

			for (int s = 0; s<Basic2PhaseDlg.nSt; s++)

				Mod.stg[s].BarPsi = Basic2PhaseDlg.m_Units;

			Mod.iModel = (Basic2PhaseDlg.m_ModelType == 1) ? 4 : 3;

			if (Basic2PhaseDlg.m_ModelType == 2)
			{
				Mod.iModel = 5;

				Mod.stg[0].InitialCw[0] = Basic2PhaseDlg.InhCon;
				Mod.stg[0].Time = Basic2PhaseDlg.VolInj / (Mod.stg[0].Qwt + Mod.stg[0].Qot);
			}

			if (Basic2PhaseDlg.m_ModelType == 3)

				Mod.iModel = 1;

			if (Basic2PhaseDlg.m_ModelType == 4)

				Mod.iModel = 2;

			Mod.title1 = Basic2PhaseDlg.EditTitle1;
			Mod.title2 = Basic2PhaseDlg.EditTitle2;

			m_EditTitle1.SetWindowText(Mod.title1);
			m_EditTitle2.SetWindowText(Mod.title2);

			Mod.title1.Format("%s\n", Mod.title1);

			Mod.Cshut = Basic2PhaseDlg.CShut;
			Mod.VI = Basic2PhaseDlg.VolInj;
			Mod.CI = Basic2PhaseDlg.InhCon;
			Mod.rho = Basic2PhaseDlg.Rho;

			Mod.WellFric = Basic2PhaseDlg.nWF;

			Mod.WellDia = Basic2PhaseDlg.WellDia;

			Mod.FluidRho = Basic2PhaseDlg.FluidRho;

			Mod.e = Basic2PhaseDlg.e;

			if (Mod.nLay != Basic2PhaseDlg.nLay)
			{
				if (Mod.nLay > Basic2PhaseDlg.nLay)
				{
					Mod.SLay.erase(Mod.SLay.begin() + (Mod.nLay - Basic2PhaseDlg.nLay), Mod.SLay.begin()+Mod.nLay);
				}
				else
				{	
					for (int n = 0; n < (Basic2PhaseDlg.nLay - Mod.nLay) ; n++ )
						
						Mod.SLay.push_back(Mod.SLay[Mod.nLay - 1]);
				}

				Mod.nLay = Basic2PhaseDlg.nLay;
				
				for (int s = 0; s < Mod.nStg; s++)
				{
					Mod.stg[s].PIDF.resize(Mod.nLay);
					Mod.stg[s].QwLay.resize(Mod.nLay);
					Mod.stg[s].LayPres.resize(Mod.nLay);
					Mod.stg[s].ProdFw.resize(Mod.nLay);
				}
			}
			

			Mod.LaySpacing.resize(Basic2PhaseDlg.nLay);

			for (l = 0; l < Basic2PhaseDlg.nLay - 1; l++)

				Mod.LaySpacing[l] = Basic2PhaseDlg.LaySpacing[l];

			Mod.FieldTabFileName = Basic2PhaseDlg.m_Edit_IsoTable.FTFileName;

			Mod.kTemp = Basic2PhaseDlg.nTemp;
			
			Mod.Temp.Ftemp = Basic2PhaseDlg.Ftemp;
			Mod.Temp.Kcr = Basic2PhaseDlg.Kcr;
			Mod.Temp.Kcs = Basic2PhaseDlg.Kcs;
			Mod.Temp.SHr = Basic2PhaseDlg.SHr;
			Mod.Temp.SHw = Basic2PhaseDlg.SHw;
			//	Mod.Temp.RHOr = Basic2PhaseDlg.SHo;
			Mod.Temp.RHOw = Basic2PhaseDlg.RHOw;
			Mod.Temp.RHOr = Basic2PhaseDlg.RHOr;
			//	Mod.Temp.RHOr = Basic2PhaseDlg.RHOo;

			Mod.Temp.watTemp = Basic2PhaseDlg.watTemp;
			Mod.Temp.oilTemp = Basic2PhaseDlg.oilTemp;
			Mod.Temp.API = Basic2PhaseDlg.API;

			if (Mod.nSimTyp == 0)
				GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(false);
			if (Mod.nSimTyp == 1)
				GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);

			//	Save Edit File
			CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
			CFileDialog saveDialog(FALSE, _T("dat"), PathNameReadFile, OFN_HIDEREADONLY |
				OFN_FILEMUSTEXIST, openFilter, this);

			if (saveDialog.DoModal() == IDOK)
			{
				int count = 0;

				PathNameReadFile = saveDialog.GetPathName();

			/*	if (Mod.iModel == 2)

					WriteFileAnaliticalLinear(PathNameReadFile);*/

				if (Mod.iModel == 3)
				{
					/*if (Mod.nSimTyp == 1)

						WriteFileRadial(PathNameReadFile);*/

					if (Mod.nSimTyp == 0)

						Mod.WriteFileSQZ5Radial(PathNameReadFile);
				}

				if (Mod.iModel == 4)
				{
					//if (saveDialog.GetFileExt() == "inp") 
					/*if (Mod.nSimTyp == 1)

						WriteFileLinear(PathNameReadFile); */

					//if (saveDialog.GetFileExt() == _T("DAT")) 
					if (Mod.nSimTyp == 0)

						Mod.WriteFileSQZ5Radial(PathNameReadFile);
				}

				/*if (Mod.iModel == 5 || Mod.iModel == 1)
				{
					WriteFileSQZ5Isotherm(PathNameReadFile);
				}*/
			}
		}
	}

	m_bRunModel.EnableWindow(TRUE);
}

void BasicDlg::OnAbortRun()
{
	// TODO: Add your control notification handler code here
	quitRun = true;

	STOP = FALSE;

	GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
	GetDlgItem(IDC_SLIDER_SP_TS)->EnableWindow(true);


	// Foward and Backward Graphics
	char st[2000];
	
	UpdateRun(st);
	//	UpdateSingleGraphTime(int((nSP_Pts-1)));
}


void BasicDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	userTimeDelay = (double)m_Slider_TimeDelay.GetPos();

	CString sTemp;
	sTemp.Format("Added time per loop: %5.3f s", (userTimeDelay) / 1000.0);
	m_Static_AddedTime.SetWindowText(sTemp);

	int n;

	n = m_Slider_SP_TS.GetPos();

	//	if ( Mod.nSimTyp == 0) //Single Phase

	UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	/*	if ( Mod.nSimTyp == 1) //Two Phase

	UpdateSingleGraphTime(int((nSP_Pts)*(n/100.0)));*/

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void BasicDlg::RelativePIDf(int st)
{
	double SumPIDF = 0;
	int i;

	if (Mod.stg[st].nMethod == 1)
	{
		/*		if ( Mod.stage[st].nOpen <= 0 )

		temp = Mod.nLay;

		else

		temp = Mod.stage[st].nOpen;
		*/
		SumPIDF = 0;

		for (i = 0; i < Mod.nLay; i++)

			SumPIDF += Mod.stg[st].PIDF[i];
	}
}

void BasicDlg::OnClose()
{
	// TODO: Add your control notification handler code here

	int i;

	quitRun = true;

	m_Button_Graph_Cross.SetCheck(0);

	GraphCross.ShowWindow(FALSE);

	m_Button_Graph_Time.SetCheck(0);

	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].ShowWindow(FALSE);
	}

	m_Button_Graph_Rad.SetCheck(0);

	for (i = 0; i<nGraphRad; i++)

		GraphRad[i].ShowWindow(FALSE);

	SRD.DestroyWindow();

	DestroyWindow();

}

void BasicDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	int i;

	quitRun = true;

	m_Button_Graph_Cross.SetCheck(0);

	GraphCross.ShowWindow(FALSE);

	m_Button_Graph_Time.SetCheck(0);

	for (i = 0; i<nGraphTime; i++)

		GraphTime[i].ShowWindow(FALSE);

	m_Button_Graph_Rad.SetCheck(0);

	for (i = 0; i<nGraphRad; i++)

		GraphRad[i].ShowWindow(FALSE);
	/*
	freed3DArray(GraphTime[0].GraphXY.y1,MNTcount+1,1);
	freed3DArray(GraphTime[1].GraphXY.y1,MNTcount+1,GraphTime[1].GraphXY.nLay);
	*/

}


void BasicDlg::ReadOutputFile(int num)
{	
	errno_t err;
	FILE *filePtr;

	char buffer[128];

	CString sItem;

	sItem = PathNameReadFile;
	sItem.Replace(Mod.ReadFileName, Mod.FileNm[num]);

	err = fopen_s(&filePtr, Mod.FileNm[num], "r");

	if (err == 0)
		printf("The file %s was opened\n", sItem);
	else
		printf("The file %s was not opened\n", sItem);

	fseek(filePtr, 0L, SEEK_SET);

	while (!feof(filePtr))
	{
		fgets(buffer, 128, filePtr);

		if (strncmp(buffer, " Simulating ...", 15) == 0)
		{
			fgets(buffer, 128, filePtr);
			fgets(buffer, 128, filePtr);
			fgets(buffer, 128, filePtr);
			fgets(buffer, 128, filePtr);

			double	t1,
					t2,
					t3,
					t4,
					t5;

			if (Mod.iModel == 4)
			{					
				while (fscanf(filePtr, "%lf %lf %lf %lf\n", &t1, &t2, &t3, &t4))
				{
					GraphTime[0].GraphXY.pFile[num].RT.push_back(t2);
					GraphTime[0].GraphXY.pFile[num].RC.push_back(t3);
				}
			}

			if (Mod.iModel == 3)
			{
				while (fscanf(filePtr, "%lf %lf %lf %lf %lf\n", &t1, &t2, &t3, &t4, &t5))
				{
					GraphTime[0].GraphXY.pFile[num].RT.push_back(t1);
					GraphTime[0].GraphXY.pFile[num].RC.push_back(t2);
				}

			}
		}
	}

	fclose(filePtr);
}

double ** BasicDlg::d2DArray(int nrows, int ncols)
{
	double **a;
	int i;

	a = (double **)malloc(nrows * sizeof(double *));

	if (a == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	for (i = 0; i < nrows; i++)
	{
		a[i] = (double *)malloc(ncols * sizeof(double));
		if (a[i] == NULL)
		{
			printf("out of memory\n");
			return 0;
		}
	}

	return a;
}

void BasicDlg::freed2DArray(double **a, int nRows)
{
	for (int i = 0; i < nRows; i++)
		free(a[i]);
	free(a);
}

double *** BasicDlg::d3DArray(int nrow, int ncol, int ncom)
{
	double ***a;

	int i, j;

	a = (double ***)malloc(nrow * sizeof(double**));

	if (a == NULL)
	{
		printf("out of memory\n");
		return 0;
	}

	for (j = 0; j < nrow; j++)
	{
		a[j] = (double **)malloc(ncol * sizeof(double*));

		for (i = 0; i < ncol; i++)
		{
			a[j][i] = (double *)malloc(ncom * sizeof(double));

			if (a[i] == NULL)
			{
				printf("out of memory\n");
				return 0;
			}
		}
	}

	return a;
}

void BasicDlg::freed3DArray(double ***a, int nrow, int ncol)
{
	for (int j = 0; j < nrow; j++)

	for (int i = 0; i < ncol; i++)

		free(a[j][i]);

	free(a);
}

void BasicDlg::OnButtonStop()
{
	// TODO: Add your control notification handler code here
	STOP = TRUE;
}

void BasicDlg::OnButtonPlay()
{
	// TODO: Add your control notification handler code here
	STOP = FALSE;
}

void BasicDlg::ShowGraph()
{
	int i;

	if (!GraphCross.IsWindowVisible())

		GraphCross.ShowWindow(true);

	for (i = 0; i<nGraphTime; i++)

	if (!GraphTime[i].IsWindowVisible())

		GraphTime[i].ShowWindow(true);

	for (i = 0; i<nGraphRad; i++)

	if (!GraphRad[i].IsWindowVisible())

		GraphRad[i].ShowWindow(true);

	m_Button_Graph_Cross.SetCheck(GraphCross.IsWindowVisible());
	m_Button_Graph_Rad.SetCheck(GraphRad[0].IsWindowVisible());
	m_Button_Graph_Time.SetCheck(GraphTime[0].IsWindowVisible());
}

void BasicDlg::HideGraph()
{
	int i;

	GraphCross.ShowWindow(FALSE);

	for (i = 0; i<nGraphTime; i++)

		GraphTime[i].ShowWindow(FALSE);

	for (i = 0; i<nGraphRad; i++)

		GraphRad[i].ShowWindow(FALSE);

	m_Button_Graph_Cross.SetCheck(GraphCross.IsWindowVisible());
	m_Button_Graph_Rad.SetCheck(GraphRad[0].IsWindowVisible());
	m_Button_Graph_Time.SetCheck(GraphTime[0].IsWindowVisible());

}


void BasicDlg::OnHelp()
{
	// TODO: Add your control notification handler code here

	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);

	CString sTemp;
	sTemp = PathName + "\\Manual\\Squeeze Manual - Ver 8.pdf";

	HINSTANCE RESULT = ShellExecute(m_hWnd, NULL, sTemp, NULL, NULL, 0);
	if ((int)RESULT <= 32) {
		RESULT = ShellExecute(m_hWnd, NULL, sTemp, NULL, NULL, 0);
		if ((int)RESULT <= 32) {
			CString errString = "An error occurred attempting to open the help files for this application:\n\n";
			switch ((int)RESULT) {
			case SE_ERR_FNF:
				errString += "File was not found. Check that 'Manual.pdf' exists in a subfolder 'Squeeze Manual - Ver 8',located same directory as the executable, or open it manually with a pdf viewer.";
				break;
			case SE_ERR_NOASSOC:
				errString += "No pdf viewer appears to have been associated with files having the .pdf extension. Try opening 'Squeeze Manual - Ver 8.pdf' manually in a pdf viewer.";
				break;
			case SE_ERR_SHARE:
				errString += "A file share violation occurred. Apparently, 'Squeeze Manual - Ver 8.pdf' is already being accessed by another application.";
				break;
			default:
				errString += "An unknown error was given. Try opening 'Squeeze Manual - Ver 8.pdf' manually in a pdf viewer.";
				break;
			}
			errString += "\n\nIf the problem persists, or cannot be worked around,\n"
				"contact Oscar Vazquez on O.Vazquez@hw.ac.uk.";
			AfxMessageBox(errString);
		}
	}

}


void BasicDlg::OnButtonAbout()
{
	// TODO: Add your control notification handler code here
	ABDlg.DoModal();

}

int testgrid::compRole[MaxComp] = { 0 };
int testgrid::compAdTy[MaxComp] = { 0 };

void BasicDlg::OnOK()
{
	// TODO: Add extra validation here
	CString szEdit;

	m_editMod.GetWindowText(szEdit);

	if (szEdit.IsEmpty() == FALSE)
	{
		szEdit.TrimRight();
		int n = atoi(szEdit);
	}
}



void BasicDlg::OnCancel()
{
	// TODO: Add extra cleanup here

}


void BasicDlg::OnBnClickedCheckGraphs()
{
	GetDlgItem(IDC_CHECK_GRAPHS)->EnableWindow(true);
	m_Button_Graphs.GetCheck();
	ShowGraph();
}

void BasicDlg::OnBnClickedButtonPhreeqc()
{
	// Create a Process
	STARTUPINFO sj;
	PROCESS_INFORMATION pj;

	ZeroMemory(&sj, sizeof(sj));
	sj.cb = sizeof(sj);
	ZeroMemory(&pj, sizeof(pj));

	char buffer[_MAX_PATH];
	// Get the current working directory: 
	_getcwd(buffer, _MAX_PATH);

	if (SetCurrentDirectory(buffer) == 0)
	{
		AfxMessageBox("ERROR");
	}

	FILE *fBarite;
	double IBa = 150,		// mMol/L
		ISO4 = 2650,	// mMol/L
		IBaSO4 = 10;			// mMol

	if ((fBarite = fopen(".\\Phreeqc\\Barite.phr", "w+")) == NULL)
	{
		AfxMessageBox("File not open to write");
	}
	else // Create Barite file
	{
		fprintf(fBarite, "SOLUTION 1	NSSW\n");
		fprintf(fBarite, "\ttemp 22\n");
		fprintf(fBarite, "\tNa	10890\n");
		fprintf(fBarite, "\tCa	428\n");
		fprintf(fBarite, "\tK	460\n");
		fprintf(fBarite, "\tS	%f # SO4\n", ISO4);
		fprintf(fBarite, "\tCl	19766\n");
		fprintf(fBarite, "\tBa %f\n", IBa);

		fprintf(fBarite, "EQUILIBRIUM_PHASES\n");
		fprintf(fBarite, "\tBarite %f %f\n", 0.0, IBaSO4);

		fprintf(fBarite, "SELECTED_OUTPUT\n");
		fprintf(fBarite, "\t-file .\\Phreeqc\\Barite.cvs\n");
		fprintf(fBarite, "\t-reset false\n");
		fprintf(fBarite, "\t-eq  Barite\n");
		fprintf(fBarite, "# This part defines the output in the output file\n");
		fprintf(fBarite, "USER_PUNCH\n");
		fprintf(fBarite, "\t-heading Ca_mmol/kgw Ba_mmol/kgw SO4\n");
		fprintf(fBarite, "\t-start\n");
		fprintf(fBarite, "\t10 PUNCH TOT\(\"Ca\"\), TOT\(\"Ba\"\), TOT\(\"S\"\)\n");
		fprintf(fBarite, "\t-end\n");
		fprintf(fBarite, "END\n");
	}

	std::fclose(fBarite);

	if (!CreateProcess(NULL, ".\\Phreeqc\\phreeqc .\\Phreeqc\\Barite.phr .\\Phreeqc\\Barite.out .\\Phreeqc\\phreeqc.dat", NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &sj, &pj))
	{
		CString st1;
		st1.Format("Hello CreateProcess failed (%d)\n", GetLastError());
		AfxMessageBox(st1);
	}

	// Wait until child processes exit.
	WaitForSingleObject(pj.hProcess, INFINITE);

	// Close process and thread handles.
	CloseHandle(pj.hProcess);
	CloseHandle(pj.hThread);

	// Read Barite.cvs
	double FBa = 150,		// mMol/L
		FSO4 = 2650,	// mMol/L
		FBaSO4,			// mMol
		P0,
		PF,
		temp;

	if ((fBarite = fopen(".\\Phreeqc\\Barite.cvs", "r")) == NULL)
	{
		AfxMessageBox("File not open to write");
	}
	else // Create Barite file
	{
		fgets(buffer, 1024, fBarite);
		fgets(buffer, 1024, fBarite);
		fscanf(fBarite, "%lf %lf %lf %lf %lf\n", &P0, &PF, &temp, &FBa, &FSO4);
	}

	FBaSO4 = P0 + PF;

	std::fclose(fBarite);
}

void BasicDlg::UpdateRun(char *st)
{
	int i,
		j,
		b,
		l,
		n,
		itmp;

	double	temp,
			yMax = -1e20;

	nSP_Pts = Mod.AD_L_Out.size();

	if (Mod.nPts > 0)
	{
		GraphTime[0].bTitle = true;
		GraphTime[0].title = "Data        SQZ";
	}
	else
	{
		GraphTime[0].bTitle = false;
		GraphTime[0].title = "";
	}
	
	GraphTime[0].PathNameReadFile = PathNameReadFile;
	GraphTime[1].PathNameReadFile = PathNameReadFile;
	GraphRad[0].PathNameReadFile = PathNameReadFile;
	GraphRad[1].PathNameReadFile = PathNameReadFile;

	GraphTime[0].WC_L_Out = Mod.WC_L_Out;	// Stores the Well Concentration
	GraphTime[0].WPRD = Mod.WPRD;			// Stores the Water Production Rate
	
	GraphTime[1].RC_L_Out = Mod.RC_L_Out;	// Stores the Layer Concentration

	GraphRad[1].FTemp_L_Out = Mod.FTemp_L_Out;	// Formation Temperature vs Radius and Layer

	GraphRad[1].C_L_Out = Mod.C_L_Out;			// Concentration Profiles vs Radius and Layer	
	GraphRad[1].AD_L_Out = Mod.AD_L_Out;		// Adsoprtion Profiles vs Radius and Layer	
	GraphRad[1].SW_L_Out = Mod.SW_L_Out;		// Water Saturation vs Radius and Layer	
	GraphRad[1].FTemp_L_Out = Mod.FTemp_L_Out;	// Formation Temperature vs Radius and Layer

	
	GraphTime[0].bMic = true;
	GraphTime[0].MicConc = Mod.GrafMIC;
	GraphTime[0].GraphXY.SetMaxNumTimes(nSP_Pts, 1, 1, Mod.nPts, Mod.nOlds);
	GraphTime[0].Mic.resize(Mod.nLay);
	GraphTime[1].GraphXY.SetMaxNumTimes(nSP_Pts, Mod.nLay, 1, 0, 0);
	GraphTime[1].Mic.resize(Mod.nLay);
	if (Mod.nSimTyp == 0) // Single Phase
		GraphRad[1].GraphXY.SetMaxNumTimes(Mod.nBlocks, Mod.nLay, 3, 0, 0);
	if (Mod.nSimTyp == 1) // Two Phase
		GraphRad[1].GraphXY.SetMaxNumTimes(Mod.nBlocks, Mod.nLay, 3, 0, 0);
	GraphRad[1].SetNum(Mod.nBlocks);

	delete[] GraphCross.layHeight;
	GraphCross.nLay = Mod.nLay;
	GraphCross.layHeight = new double[GraphCross.nLay];
	GraphCross.GraphXY.SetMaxNumTimes(Mod.nBlocks + 1, Mod.nLay, 1, 0, 0);

	GraphRad[0].GraphXY.SetMaxNumTimes(Mod.nBlocks, Mod.nLay, 1, 1, 0);
	GraphRad[0].SetNum(Mod.nBlocks);

	GraphRad[1].title = "Conc        Adso        Temp";
	GraphRad[1].bTitle = TRUE;
	GraphRad[1].yLabel = "Normalized Conc,Adso,Temp";

	GraphTime[0].GraphXY.x[0] = 0;
	GraphTime[1].GraphXY.x[0] = 0;
	
	if (Mod.iModel == 4)
	{
		GraphTime[0].bMic = false;
		GraphTime[0].xLabel = "Pore Volumes";
	}

	if (Mod.iModel == 2) GraphTime[0].bMic = false;
	
	if (Mod.iModel == 3)
	{
		double ConvQ;

		if (Mod.UnitSpec == 'M' || Mod.UnitSpec == 'm')
		{
			ConvQ = 1 * 1e-2; // 100 m3
		}

		if (Mod.UnitSpec == 'C' || Mod.UnitSpec == 'c')
		{
			ConvQ = 0.158987295*1e3; // 1000 bbls
		}

		for (i = 1; i < nSP_Pts; i++)
		{
			if (Mod.nSimTyp == 0) // Single-Phase

				GraphTime[0].GraphXY.x[i] = Mod.RT[i];

			// Two-Phase
			if (Mod.nSimTyp == 1)

			if (Mod.kPlot == 1)

				GraphTime[0].GraphXY.x[i] /= ConvQ;

			temp = Mod.RC[i];

			if (temp > 0.1)
				GraphTime[0].GraphXY.y1[i][0][0] = log10(temp);
			else
				GraphTime[0].GraphXY.y1[i][0][0] = log10(0.1);

			if (temp < 0)
			{
				fail = true;
				nF = i;
			}

			if (temp > yMax) yMax = temp;

			for (l = 0; l < Mod.nLay; l++)
			{	
				temp = Mod.RC_L_Out[i][l];

				if (temp >= 0.1)
					GraphTime[1].GraphXY.y1[i][l][0] = log10(temp);
				else
					GraphTime[1].GraphXY.y1[i][l][0] = log10(0.1);
			}

			for (l = 0; l < Mod.nLay; l++)
			{
				GraphTime[0].Mic[l].Height = Mod.SLay[l].HLay;
			}
			
			GraphTime[1].GraphXY.x[i] = GraphTime[0].GraphXY.x[i];
		}
	}

	// Reading Old files 
	GraphTime[0].nOutFiles = Mod.nOlds;

	for (j = 0; j<Mod.nOlds; j++)
	{
		ReadOutputFile(j);
		CString sedit(Mod.FileNm[j]);
		sedit.Replace(".out", "");

		GraphTime[0].GraphXY.pFile[j].NameFile = sedit;
	}
	/*
	if (Mod.iModel == 4 || Mod.iModel == 2)
	{
		if (Mod.nSimTyp == 0) // Single Phase
		{
			fgets(buffer, 2048, fileConc);
			fgets(buffer, 2048, fileConc);
		}

		nSP_Pts--;

		if (Mod.iModel == 2)
			nSP_Pts++;

		for (i = 0; i < nSP_Pts; i++)
		{
			fscanf(fileConc, "%lf %lf", &GraphTime[0].GraphXY.x[i], &temp);

			if (temp > 0.1)
				GraphTime[0].GraphXY.y1[i][0][0] = log10(temp);
			else
				GraphTime[0].GraphXY.y1[i][0][0] = log10(0.1);

			if (temp > yMax)yMax = temp;

			if (temp < 0)
			{
				fail = true;
				nF = i;
			}
		}
	}
	*/

	for (i = 0; i < GraphTime[0].GraphXY.nPts; i++)
	{
		GraphTime[0].GraphXY.xP[i] = Mod.fX[i];
		GraphTime[0].GraphXY.yP[i] = Mod.fY[i];
	}


	//	if ( Mod.nSimTyp == 1 ) // Two Phase
	//	{
	if (Mod.iModel == 3 || Mod.iModel == 4)
	{
		// Read Distance output
//		fseek(fileDist, 0L, SEEK_SET);

	/*	for (l = 0; l < Mod.nLay; l++)
		{
			for (b = 0; b < Mod.nBlocks; b++)
			{
				Lay[l].Conc[b][0] = 0;
				Lay[l].AdsPPt[b][0] = 0;
				Lay[l].Temp[b][0] = 0;
				Lay[l].FTemp[b][0] = 0;
			}
		}

		for (n = 1; n < nSP_Pts; n++)
		{
			fscanf(fileDist, "%s %lf %s %d %s %d\n", buffer, &temp, buffer, &StgNum[n], buffer, &StgType[n]);

			for (l = 0; l < Mod.nLay; l++)
			{
				fgets(buffer, 2048, fileDist);
				fgets(buffer, 2048, fileDist);

				if (n == 1 && l == 0)
				{
					for (b = 0; b < Mod.nBlocks; b++)

						fscanf(fileDist, "%lf", &Dist[b]);
				}

				for (b = 0; b < Mod.nBlocks; b++)
				{
					fscanf(fileDist, "%lf", &Lay[l].Conc[b][n]);

					if (MaxConc < Lay[l].Conc[b][n]) MaxConc = Lay[l].Conc[b][n];
				}

				for (b = 0; b < Mod.nBlocks; b++)
				{
					fscanf(fileDist, "%lf", &Lay[l].AdsPPt[b][n]);

					if (MaxAdsPPt < Lay[l].AdsPPt[b][n]) MaxAdsPPt = Lay[l].AdsPPt[b][n];
				}

				for (b = 0; b < Mod.nBlocks; b++)

					fscanf(fileDist, "%lf", &Lay[l].Temp[b][n]);

				if (Mod.nSimTyp == 1) // Two Phase

				for (b = 0; b < Mod.nBlocks; b++)

					fscanf(fileDist, "%lf", &Lay[l].FTemp[b][n]);

				fscanf(fileDist, "\n");

				if (StgType[n] != 3)

					wprd[l][n] = 0;

			}

		}*/
	}
	//	}

	GraphCross.Num = Mod.nBlocks;

	for (b = 0; b<Mod.nBlocks; b++)
	{
		GraphCross.GraphXY.x[b] = Mod.R(b,Mod.dr);
	}

	GraphCross.xMax = Mod.R(Mod.nBlocks-1, Mod.dr);
	GraphCross.yMax = 0;

	MaxConc = 0;
	MaxAdsPPt = 0;

	for (int t = 0; t < nSP_Pts; t++)
	{
		for (l = 0; l < Mod.nLay; l++)
		{
			for (b = 0; b < Mod.nBlocks; b++)
			{
				if (MaxConc < Mod.C_L_Out[t][b][l]) MaxConc = Mod.C_L_Out[t][b][l];

				if (MaxAdsPPt < Mod.AD_L_Out[t][b][l]) MaxAdsPPt = Mod.AD_L_Out[t][b][l];
			}
		}
	}

	for (l = 0; l<Mod.nLay; l++)
	{
		GraphTime[0].Mic[l].wConc = Mod.WC_L_Out[nSP_Pts - 1][l];
		GraphTime[0].Mic[l].wprd = Mod.WPRD[nSP_Pts - 1][l];;

		for (b = 0; b<Mod.nBlocks; b++)
		{
//			GraphRad[0].GraphXY.y1[b][l][0] = Lay[l].Temp[b][nSP_Pts - 1];
			GraphRad[1].GraphXY.y1[b][l][0] = Mod.C_L_Out[nSP_Pts - 1][b][l] / MaxConc;
			GraphRad[1].GraphXY.y1[b][l][1] = Mod.AD_L_Out[nSP_Pts - 1][b][l] / MaxAdsPPt;
/*			GraphRad[1].GraphXY.y1[b][l][2] = Lay[l].FTemp[b][nSP_Pts - 1] / Mod.Temp.Ftemp;

			if (Mod.nSimTyp == 0) // Single Phase
				GraphRad[1].GraphXY.y1[b][l][2] = Lay[l].Temp[b][nSP_Pts - 1] / Mod.Temp.Ftemp;
*/
			GraphCross.GraphXY.y[b][l] = Mod.C_L_Out[nSP_Pts - 1][b][l];
		}

		GraphCross.layHeight[l] = Mod.SLay[l].HLay;

		GraphCross.yMax += Mod.SLay[l].HLay;
	}

	CString sEdit;

	if (Mod.StgType[nSP_Pts - 1] == 1)	sEdit.Format("Stage %d: Inj/Preflush", Mod.StgNum[nSP_Pts - 1]);

	if (Mod.StgType[nSP_Pts - 1] == 2)	sEdit.Format("Stage %d: Overflush", Mod.StgNum[nSP_Pts - 1]);

	if (Mod.StgType[nSP_Pts - 1] == 5)	sEdit.Format("Stage %d: Shut-in", Mod.StgNum[nSP_Pts - 1]);

	if (Mod.StgType[nSP_Pts - 1] == 3)	sEdit.Format("Stage %d: Production", Mod.StgNum[nSP_Pts - 1]);
	
	for (i = 0; i<nGraphTime; i++)
	{
		GraphTime[i].Num = nSP_Pts - 1;
		if (Mod.nSimTyp == 1) // Two-Phase
			GraphTime[i].Num = nSP_Pts;
		if (Mod.nSimTyp == 0) // Single Phase
		{
			GraphTime[i].SetXYMaxMin();
			GraphTime[i].yMax = log10(yMax);
		}

		if (GraphTime[i].xMin > 0) GraphTime[i].xMin = 0;
		GraphTime[i].yMin = -1;

		if (Mod.iModel == 4)
		{
			GraphTime[0].xMax *= 1.001;
			GraphTime[0].xMin = 0;
		}

		if (Mod.iModel == 2)

			GraphTime[0].xMax *= 1.001;

		if (Mod.iModel == 3)
		{
			if (Mod.kPlot == 0)

				GraphTime[i].xLabel = "Time (days)";

			else
			{
				if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric

					GraphTime[i].xLabel = "Vol (100m3)";

				if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional

					GraphTime[i].xLabel = "Vol (1000bbls)";
			}

		}

		if (Mod.iModel == 4)

			GraphTime[i].xLabel = "Pore Volumes";
	}

	if (Mod.nSimTyp == 0) // Single Phase
	{
		GraphCross.UpdateInterface(sEdit);

		for (i = 0; i<nGraphTime; i++)

			GraphTime[i].UpdateInterface(sEdit);

		for (i = 0; i<nGraphRad; i++)

			GraphRad[i].UpdateInterface(sEdit);

		FirstChartInit();
	}

	if (Mod.nSimTyp == 0) // Single Phase
	{
		GraphRad[0].SetWindowText("SQZ Layer Temperature");
		GraphRad[0].yLabel = "Temperature";
	}

	if (Mod.nSimTyp == 1) // Two Phase
	{
		GraphRad[0].SetWindowText("SQZ Layer Water Saturation");
		GraphRad[0].yLabel = "Water Saturation";
	}

	if (Mod.wComp[0].MaxC > 0)
		yMax = Mod.wComp[0].MaxC;
	else
		yMax = 10.0;

	if (Mod.iModel == 3 || Mod.iModel == 4)
	{
		for (i = 0; i<nGraphRad; i++)
		{
			for (b = 0; b< Mod.nBlocks; b++)
			{
				GraphRad[i].GraphXY.x[b] = Mod.R(b,Mod.dr);
			}

			GraphRad[i].Num = Mod.nBlocks;
			GraphRad[i].SetXYMaxMin();
			GraphRad[i].xMax *= 1.001;
			GraphRad[i].yMin = 0;
			GraphRad[i].yMax = 1;
		}
	}

	if (Mod.iModel == 4 || Mod.iModel == 2)
	{
		GraphTime[1].ShowWindow(false);

		for (i = 0; i<nGraphRad; i++)

			GraphRad[i].ShowWindow(false);
	}

	if (Mod.nSimTyp == 0) // Single-Phase
	{
		if (Mod.kTemp == 1) GraphRad[0].yMax = Mod.Temp.Ftemp;

		if (Mod.iModel == 4) GraphTime[0].xLabel = "Pore Volumes";

		//	if (Mod.iModel == 2) GraphTime[0].xLabel = "Cumulative Production in bbls";

		UpdateSingleGraphTime(nSP_Pts - 1);
	}

//	std::fclose(fileDist);
}

void BasicDlg::UpdateSingleGraphTime(int n)
{
	int i, l, b;
	CString sEdit;

	if (!ParRun)
	{
		nL = Mod.nLay;
		nB = Mod.nBlocks;
	}

	if (check)
	{
		if (Mod.StgType[n] == 1)	sEdit.Format("Stage %d: Inj/Preflush", Mod.StgNum[n]);

		if (Mod.StgType[n] == 2)	sEdit.Format("Stage %d: Overflush", Mod.StgNum[n]);

		if (Mod.StgType[n] == 5)	sEdit.Format("Stage %d: Shut-in", Mod.StgNum[n]);

		if (Mod.StgType[n] == 3)	sEdit.Format("Stage %d: Production", Mod.StgNum[n]);
		
		for (l = 0; l<nL; l++)
		{
			GraphTime[0].Mic[l].wConc = Mod.WC_L_Out[n][l];
			GraphTime[0].Mic[l].wprd = Mod.WPRD[n][l];
			
			for (b = 0; b<nB; b++)
			{
			//	GraphRad[0].GraphXY.y1[b][l][0] = Lay[l].Temp[b][n];
				GraphRad[1].GraphXY.y1[b][l][0] = Mod.C_L_Out[n][b][l] / MaxConc;
				GraphRad[1].GraphXY.y1[b][l][1] = Mod.AD_L_Out[n][b][l] / MaxAdsPPt;
			//	GraphRad[1].GraphXY.y1[b][l][2] = Lay[l].Temp[b][n] / Mod.Temp.Ftemp;

				GraphCross.GraphXY.y[b][l] = Mod.C_L_Out[n][b][l];
			}
		}

		GraphCross.UpdateInterface(sEdit);

		for (i = 0; i<nGraphTime; i++)
		{
			GraphTime[i].Num = n - 1;
			if (n == nSP_Pts)
				GraphTime[i].xMax = GraphTime[i].GraphXY.x[n - 1];
			else
				GraphTime[i].xMax = GraphTime[i].GraphXY.x[n];

			GraphTime[i].UpdateInterface(sEdit);
		}

		for (i = 0; i<nGraphRad; i++)
		{
			GraphRad[i].UpdateInterface(sEdit);
		}
	}
}

void BasicDlg::OnEnChangeSlider_SP_TS()
{
	int n;

	n = m_Slider_SP_TS.GetPos();

	UpdateSingleGraphTime(int(nSP_Pts*(n / 100.0)));
}

void BasicDlg::OnBnClickedButtonBack()
{
	int n;

	n = m_Slider_SP_TS.GetPos();

	if (n > 0)

		n--;

	if (n < 0) n = 0;

	m_Slider_SP_TS.SetPos(n);

	UpdateSingleGraphTime(int(nSP_Pts*(n / 100.0)));
}


void BasicDlg::OnBnClickedButtonFoward()
{
	int n;

	n = m_Slider_SP_TS.GetPos();

	if (n < 100)

		n++;

	if (n > 100) n = 100;

	m_Slider_SP_TS.SetPos(n);

	if (Mod.nSimTyp == 0) //Single Phase

		UpdateSingleGraphTime(int((nSP_Pts - 1)*(n / 100.0)));

	if (Mod.nSimTyp == 1) //Two Phase

		UpdateSingleGraphTime(int((nSP_Pts)*(n / 100.0)));

}

void BasicDlg::OnButtonParOpt()
{
	/*	Mod.ReadFileName

	PathNameReadFile
	Mod.ReadPathName
	*/
	string outputFile;

	outputFile = PathNameReadFile;

	char buf[BUFSIZ];
	size_t size;

	int i = outputFile.find_last_of(".");
	if (i < string::npos)
		outputFile = outputFile.substr(0, i);

	outputFile += "PSOResults.txt";

	
	Mod.nSim = 1;
	Par.SetRun(outputFile, 1, Mod);

	Dlg_PSO_Output.nPar = Mod.SqzOpt.nPar; // Parameter space dimension

	vector<string> temp;

	temp = Mod.SqzOpt.Label;

	int j1 = 0;

	for (int j = 0; j < Mod.SqzOpt.nPar; j++)
	{
		string st = "SI";

		if (Mod.SqzOpt.Label[j].find(st) != -1)
		{
			temp[j1] = Mod.SqzOpt.Label[j];
			j1++;
		}
	}

	int j2 = 0;
	for (int j = 0; j < Mod.SqzOpt.nPar; j++)
	{
		string st = "SI";

		if (Mod.SqzOpt.Label[j].find(st) == -1)
		{
			temp[j1 + j2] = Mod.SqzOpt.Label[j];
			j2++;
		}
	}

	Dlg_PSO_Output.InputPathName = PathName + "Exec\\InputOpt.dat";	// Pathname for Design Input file save 
	Dlg_PSO_Output.SavePathName = outputFile.c_str();					// Pathname of Design Input File Save

	Dlg_PSO_Output.Label = temp;
	
	Dlg_PSO_Output.Label.push_back("SQZ_LT");
	Dlg_PSO_Output.Label.push_back("TotCost");
	Dlg_PSO_Output.Label.push_back("TotInjCost");
	Dlg_PSO_Output.Label.push_back("TotInjTime");
	Dlg_PSO_Output.Label.push_back("TotInjVol");
	Dlg_PSO_Output.Label.push_back("TotSI");
	Dlg_PSO_Output.Label.push_back("TotSICost");

	if (SqzOpt.Fixed_Target)
		Dlg_PSO_Output.Label.push_back("CostPerVol");
		
	Dlg_PSO_Output.Fixed_Target = SqzOpt.Fixed_Target;

	Dlg_PSO_Output.opt_data = Par.pso_dat;
	
	if (Dlg_PSO_Output.DoModal() == IDOK)
	{
		;
	}
}

void BasicDlg::Par_PSO_Opt()
{	
	int nRuns;
	ProgressBar ProBar;

	Model mod;
	vector<Particle> P; // Rarticles to run forecast

	// Random Seed
//	srand((unsigned)time(NULL));
			srand(548);

	double k, n, r2,
		ik, in, ir2;

	mod.ReadFileSQZ5Radial(PathNameReadFile.GetBuffer());

	ik = mod.Iso.k;
	in = mod.Iso.n;
	ir2 = mod.Iso.r2;

	mod.ConvertUnits();

	// Design Optimisation = 1, Isotherm derivation = 0
	mod.nOpt = 1;
	nRuns = 1;
	mod.Max = FALSE; // Minimise
	mod.PathName = PathNameReadFile;

	//	ProBar.ShowWindow(true);
//	ProBar.m_ProgressBar.SetPos(0);
//	ProcessMessageLoop();

	CString file = PathNameReadFile;
	file.Replace(".dat", "Res.txt");
	file.Replace(".DAT", "Res.txt");

	errno_t err;
	if ((err = fopen_s(&mod.fRes, file, "w")) != 0)
		printf("Error opening file %s for reading\r\n", "Res.txt");

	file = PathNameReadFile;
	file.Replace(".dat", "All.txt");
	file.Replace(".DAT", "All.txt");

	if ((err = fopen_s(&mod.filePtrAll, file, "w")) != 0)
		printf("Error opening file %s for reading\r\n", "All.txt");

	for (int rn = 0; rn < nRuns; rn++)
	{
		// Every time PSO runs, it needs a new dup to get a good match
		mod.Prep_du();

		if (rn == 0)
		{
			fprintf(mod.fRes, "%5s %3s %3s", "Run", "gen", "par");

			for (int n = 0; n < mod.SqzOpt.nPar; n++)
			{
				fprintf_s(mod.fRes, " %1s%-9d ", "P", n);
			}
			fprintf_s(mod.fRes, "%10s\n", "SQZLT");

			if (mod.nOpt == 1)  // Design Optimisation
			{
				fprintf_s(mod.filePtrAll, "%4s %4s  ", "#Gen", "nPar");

				for (int i = 0; i < 3 * mod.SqzOpt.nPar; i++)
				{
					const size_t buffsize = 10;
					char ind[buffsize];

					string s;

					if (i < mod.SqzOpt.nPar)
					{
						sprintf_s(ind, buffsize, "%d ", i);
						string tind(ind);
						s = "X" + tind;
						fprintf_s(mod.filePtrAll, "%11s", s.c_str());
					}

					if (i >= mod.SqzOpt.nPar && i < 2 * mod.SqzOpt.nPar)
					{
						string st = "SI";
						string temp;
						temp = mod.SqzOpt.Label[i - mod.SqzOpt.nPar];

						if (temp.find(st) == -1)
						{
							st = "Inj";
							int n = temp.find(string(st));
							if (n > -1)
								temp.replace(3, 3, string("Tim"));
							else
								temp.replace(2, 3, string("Tim"));
						}

						fprintf_s(mod.filePtrAll, "%11s", temp.c_str());

					}

					if (i >= 2 * mod.SqzOpt.nPar && i < 3 * mod.SqzOpt.nPar)
					{
						string st = "SI";
						if (mod.SqzOpt.Label[i - 2 * mod.SqzOpt.nPar].find(st) == -1)
						{
							fprintf_s(mod.filePtrAll, "%11s", mod.SqzOpt.Label[i - 2 * mod.SqzOpt.nPar].c_str());
						}
					}
				}

				CString st1 = "TotInjVol(bbl)",
						st2 = "TotSI(bbl)",
						st3 = "WatVolPro(bbl)", 
						st4 = "CostperBbl";

				if (mod.UnitSpec == 'm' || mod.UnitSpec == 'M')
				{
					st1 = "TotInjVol(m3)";
					st2 = "TotSI(m3)",
					st3 = "WatVolPro(m3)",
					st4 = "Costperm3";
				}

				fprintf_s(mod.filePtrAll, "%16s %16s %11s %11s %11s %11s %11s %11s %11s \n", "TotInjTime(hr)", st1, st2, "TotSICost", "TotInjCost", "TotCost", "RealObj", st3, st4);

			}
		}

		if (mod.nLife > 0)
		{
			mod.Clife[0] = mod.SqzOpt.MIC;
		}
		else
		{
			mod.Clife.resize(1);
			mod.Clife[0] = mod.SqzOpt.MIC;
		}

		P.push_back(RunPSO(mod, rn));
		
//		ProBar.m_ProgressBar.SetPos(int(double(rn) * 100 / double(nRuns + 1)));
//		ProcessMessageLoop();
	}

	//	RunForecast(P, ForecastFile);

//	ProBar.m_ProgressBar.SetPos(int(double(nRuns + 1) * 100 / double(nRuns + 1)));
//	ProcessMessageLoop();

//	ProBar.ShowWindow(false);

	std::fclose(mod.fRes);
	
/*	if (SqzOpt.Fixed_Target) // =0 for normal PSO Optimization and =1 for Secant Optimization
		Par.SetRun(outputFile, pathName, 4, M, SqzOpt);
	else
		Par.SetRun(outputFile, pathName, 1, M, SqzOpt);*/

	Dlg_PSO_Output.nPar = mod.SqzOpt.nPar; // Parameter space dimension

	vector<string> temp;

	temp = mod.SqzOpt.Label;

	int j1 = 0;

	for (int j = 0; j < mod.SqzOpt.nPar; j++)
	{
		string st = "SI";

		if (mod.SqzOpt.Label[j].find(st) != -1)
		{
			temp[j1] = mod.SqzOpt.Label[j];
			j1++;
		}
	}

	int j2 = 0;
	for (int j = 0; j < mod.SqzOpt.nPar; j++)
	{
		string st = "SI";

		if (mod.SqzOpt.Label[j].find(st) == -1)
		{
			temp[j1 + j2] = mod.SqzOpt.Label[j];
			j2++;
		}
	}

//	Dlg_PSO_Output.InputPathName = PathName + "Exec\\InputOpt.dat";	// Pathname for Design Input file save 
//	Dlg_PSO_Output.SavePathName = outputFile.c_str();					// Pathname of Design Input File Save

	Dlg_PSO_Output.Label = temp;

	Dlg_PSO_Output.Label.push_back("SQZ_LT");
	Dlg_PSO_Output.Label.push_back("TotCost");
	Dlg_PSO_Output.Label.push_back("TotInjCost");
	Dlg_PSO_Output.Label.push_back("TotInjTime");
	Dlg_PSO_Output.Label.push_back("TotInjVol");
	Dlg_PSO_Output.Label.push_back("TotSI");
	Dlg_PSO_Output.Label.push_back("TotSICost");
	Dlg_PSO_Output.Label.push_back("CostPerVol");

	Dlg_PSO_Output.Fixed_Target = mod.SqzOpt.Fixed_Target;

	Dlg_PSO_Output.opt_data = opt_data;

	fclose(mod.filePtrAll);

	if (Dlg_PSO_Output.DoModal() == IDOK)
	{
		;
	}
}

//****************************************************************************
// Run PSO, returns best overall particle Pi
Particle BasicDlg::RunPSO(Model & mod, int rn)
{
	int nPar = mod.SqzOpt.nPar;
	int nPopSize = mod.SqzOpt.nPopSize;
	int nGen = mod.SqzOpt.nGen;

	vector<double>	uX,
		lX;

	vector<Particle> fP;

	fP.resize(nPopSize * nGen);

	// define the number of processes (threads);
	// this number can be modified by USER (to be implemented)
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;
	//	int nproc = 1;

	if (mod.nOpt == 0) // Isotherm Derivation
	{
		// = 0 Equilibrium
		if (mod.Iso.TypeAds == 0) nPar = 2;

		// = 1 Kinetic
		if (mod.Iso.TypeAds == 1) nPar = 3;

		if (nPar == 3)
		{
			uX.resize(3);
			lX.resize(3);

			// k
			uX[0] = mod.SqzOpt.uX[0];
			lX[0] = mod.SqzOpt.lX[0];

			// n
			uX[1] = mod.SqzOpt.uX[1];
			lX[1] = mod.SqzOpt.lX[1];

			// r2
			uX[2] = mod.SqzOpt.uX[2];
			lX[2] = mod.SqzOpt.lX[2];
		}

		if (nPar == 2)
		{
			uX.resize(2);
			lX.resize(2);

			// k
			uX[0] = mod.SqzOpt.uX[0];
			lX[0] = mod.SqzOpt.lX[0];

			// n
			uX[1] = mod.SqzOpt.uX[1];
			lX[1] = mod.SqzOpt.lX[1];
		}
	}

	if (mod.nOpt == 1) // Design Optimisation
	{
		uX = mod.SqzOpt.uX;
		lX = mod.SqzOpt.lX;
	}

	vector<Particle>	P(nPopSize),
		Pg(nPopSize);		// Stores the personal best location of each particle;

	Particle	Pi,	// Stores best overall particle 
		Pgen;	// Stores the best per generation

	Pi.X.resize(nPar);
	Pgen.X.resize(nPar);

	vector<vector<double>> vel;

	double r1, r2;

	try
	{
		int no_of_cols = nPar;
		int no_of_rows = nPopSize;
		double initial_value = 0.0;
		vel.resize(no_of_rows, vector<double>(no_of_cols, initial_value));


		for (int i = 0; i < nPopSize; i++)
		{
			P[i].X.resize(nPar);

			Pg[i].X.resize(nPar);

			for (int j = 0; j < nPar; j++)
			{
				// Random Initial Population
				P[i].X[j] = (uX[j] - lX[j]) * randval(0, 1) + lX[j];
			}

			if (mod.Max) // Minimise
			{
				P[i].Obj = -1e20;
				Pg[i].Obj = -1e20;
			}
			else // Maximise
			{
				P[i].Obj = 1e20;
				Pg[i].Obj = 1e20;
			}

		}

	}
	catch (Exception& e)
	{
		printf_s("e.what()\n");
	}

	// Randomized Particle Velocities
	for (int i = 0; i < nPopSize; i++)
	{
		for (int j = 0; j < nPar; j++)
		{
			vel[i][j] = 0;

			if (randval(0, 1) > 0.5)

				vel[i][j] = randval(0, 0.5) * P[i].X[j];

			else

				vel[i][j] = -randval(0, 0.5) * P[i].X[j];
		}
	}

	vector<OneRun*> OLD(nPopSize);
	for (int i = 0; i < nPopSize; i++)
	{
		OLD[i] = new OneRun;		// to be deleted manually!
	}

	JobsPool JP(OLD);

	vector<thread*> threads(nproc);

	if (mod.Max) // Maximise Max = TRUE
		Pi.Obj = -1e20;
	else // Minimise Max = FALSE
		Pi.Obj = 1e20;

	int num = 0;
	// Start with the generations
	for (int n = 0; n < nGen; n++)
	{
		try
		{
			// Generate New Population
			for (int i = 0; i < nPopSize; i++)
			{
				for (int j = 0; j < nPar; j++)
				{
					P[i].X[j] = P[i].X[j] + vel[i][j];
				}

				// DAMPING BOUNDARY CONDITION
				for (int j = 0; j < nPar; j++)
				{
					// The coordinate j is reset to the boundary, then the velocity sign is reversed
					// and multiplied by random between [0,1]
					if (P[i].X[j] >= uX[j])

						P[i].X[j] = uX[j] - randval(0, 0.2) * vel[i][j];

					if (P[i].X[j] <= lX[j])

						P[i].X[j] = lX[j] - randval(0, 0.2) * vel[i][j];

				}
			}

			// Make sure original design is included
			if (n == 0)
			{
				if (mod.nOpt == 0) // Isotherm Derivation
				{
					if (nPar == 3)
					{
						P[0].X[0] = mod.Iso.k;
						P[0].X[1] = mod.Iso.n;
						P[0].X[2] = mod.Iso.r2;
					}

					if (nPar == 2)
					{
						P[0].X[0] = mod.Iso.k;
						P[0].X[1] = mod.Iso.n;
					}
				}

				if (mod.nOpt == 1) // Design Optimisation
				{
					for (int j = 0; j < nPar; j++)
					{
						P[0].X[j] = 1;
					}
				}
			}

			// Evaluate New Population
			for (int i = 0; i < nPopSize; i++)
			{
				OLD[i]->P = P[i];
				OLD[i]->mod = mod;
			}

			JP.SetNT();

			for (int j = 0; j < nproc; j++)
				threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

			// the threads are running here!

			for (int j = 0; j < nproc; j++)
			{
				threads[j]->join();		// wait for all threads to finish
				delete threads[j];		// and delete them
			}

			if (JP.E)
				rethrow_exception(JP.E);	// rethrow exception from the worker thread

			for (int i = 0; i < nPopSize; i++)
			{
				P[i].Obj = OLD[i]->P.Obj;
				P[i].watConc = OLD[i]->P.watConc;
				
				if (mod.nLife == 0) mod.nLife = 1;
				
				P[i].Vol_Life.resize(mod.nLife);
				P[i].Tim_Life.resize(mod.nLife);

				for (int j = 0; j < mod.nLife; j++)
				{
					P[i].Vol_Life[j] = OLD[i]->P.Vol_Life[j];
					P[i].Tim_Life[j] = OLD[i]->P.Tim_Life[j];
				}

				if (mod.nOpt == 0) // Isotherm Derivation
				{
					double ik, in, ir2;

					ik = P[i].X[0];
					in = P[i].X[1];

					if (nPar == 3)
					{
						ir2 = P[i].X[2];
					}
					else
						ir2 = 0;

					fprintf(mod.fRes, "%5d %3d %3d %10.3f %10.3f %10.3f %5.3e %10s", rn, n, i, ik, in, ir2, P[i].Obj, "Sim");

					for (int n = 0; n < P[i].watConc.size(); n++)
						fprintf_s(mod.fRes, "%10.3f ", P[i].watConc[n]);
				}

				if (mod.nOpt == 1) // Design Optimisation
				{
					fprintf(mod.fRes, "%5d %3d %3d ", rn, n, i);

					/*	for (int n = 0; n < mod.nPar; n++)
							fprintf_s(mod.fRes, "%10.3f ", P[i].X[n]);*/

					for (int n = 0; n < mod.SqzOpt.nPar; n++)
						fprintf_s(mod.fRes, "%10.3f ", mod.SqzOpt.X[n] * P[i].X[n]);

					if (P[i].Obj > 1e10) // Run Fail 

						fprintf_s(mod.fRes, "%10.3f ", 0.0);

					else

						fprintf_s(mod.fRes, "%10.3f ", P[i].Obj);

					P[i].Obj = fabs(mod.Obj_T_V - P[i].Obj) / mod.Obj_T_V;

					fprintf_s(mod.filePtrAll, "%-4d %-4d ", n, i);

					for (int j = 0; j < mod.SqzOpt.nPar; j++)
					{
						fprintf_s(mod.filePtrAll, "%11.4f", P[i].X[j]);
					}

					for (int j = 0; j < mod.SqzOpt.nPar; j++)
					{
						if (P[i].X[j] * mod.SqzOpt.X[j] < 1e5)
						{
							fprintf_s(mod.filePtrAll, "%11.4f", P[i].X[j] * mod.SqzOpt.X[j]);
						}
						else
						{
							fprintf_s(mod.filePtrAll, "%11.2f", P[i].X[j] * mod.SqzOpt.X[j]);
						}
					}

					for (int j = 0; j < mod.SqzOpt.nPar; j++)
					{
						string st = "SI";

						if (mod.SqzOpt.Label[j].find(st) == -1)
						{
							fprintf_s(mod.filePtrAll, " %11.2f", P[i].X[j] * mod.SqzOpt.Vol[j]);
						}
					}
				}

				if (mod.fMax <= P[i].Obj) mod.fMax = P[i].Obj;

				fP[num] = P[i];
				num++;
				fprintf_s(mod.fRes, "\n");

				fflush(mod.fRes);

				// Calculate total vol, total SI, ... per run
				data_sqz tempdat;
				tempdat.xVal.resize(mod.SqzOpt.nPar);
				tempdat.dat.resize(mod.SqzOpt.nPar + 8);

				double	TotInjTime = 0,
						TotInjVol = 0,
						TotalSI = 0,
						TotalSICost = 0,
						TotalInjCost = 0,
						TotalCost = 0,
						CostPerVol = 0;

				for (int j = 0; j < mod.SqzOpt.nPar; j++)
				{
					tempdat.xVal[j] = P[i].X[j] * mod.SqzOpt.X[j];
				}

				int j1 = 0;

				for (int j = 0; j < mod.SqzOpt.nPar; j++)
				{
					string st = "SI";

					if (mod.SqzOpt.Label[j].find(st) == -1)
					{
						TotInjTime += (P[i].X[j] * mod.SqzOpt.X[j]);
						TotInjVol += (P[i].X[j] * mod.SqzOpt.Vol[j]);
					}
					else
					{
						tempdat.dat[j1] = P[i].X[j] * mod.SqzOpt.X[j];
						j1++;
					}
				}

				int j2 = 0;
				for (int j = 0; j < nPar; j++)
				{
					string st = "SI";

					if (mod.SqzOpt.Label[j].find(st) == -1)
					{
						tempdat.dat[j1 + j2] = P[i].X[j] * mod.SqzOpt.Vol[j];
						j2++;
					}
				}				

				for (int j = 0; j < mod.stg.size(); j++)
				{
					if (mod.stg[j].TypeStg == 1)
					{
						if (mod.stg[j].objSI == 1 && mod.stg[j].objTime == 1)

							TotalSICost += ((P[i].X[mod.stg[j].objIndexSI] * mod.SqzOpt.X[mod.stg[j].objIndexSI]) * (P[i].X[mod.stg[j].objIndexTime] * mod.SqzOpt.X[mod.stg[j].objIndexTime])*mod.stg[j].Qwt*60);

						if (mod.stg[j].objSI == 1 && mod.stg[j].objTime == 0)

							TotalSICost += ((P[i].X[mod.stg[j].objIndexSI] * mod.SqzOpt.X[mod.stg[j].objIndexSI]) * (mod.stg[j].Qwt * mod.stg[j].Time * 60));

						if (mod.stg[j].objSI == 0 && mod.stg[j].objTime == 1)

							TotalSICost += (mod.stg[j].InitialCw[0] * (P[i].X[mod.stg[j].objIndexTime] * mod.SqzOpt.X[mod.stg[j].objIndexTime]) * mod.stg[j].Qwt * 60);

						if (mod.stg[j].objSI == 0 && mod.stg[j].objTime == 0)

							TotalSICost += (mod.stg[j].InitialCw[0] * (mod.stg[j].Qwt * mod.stg[j].Time * 60));
					}

					if (mod.stg[j].TypeStg != 3)
					{
						if (mod.stg[j].objTime == 1)
						{
							TotInjTime += (P[i].X[mod.stg[j].objIndexTime] * mod.SqzOpt.X[mod.stg[j].objIndexTime]);

							if (mod.stg[j].TypeStg != 5)
							{
								TotInjVol += (P[i].X[mod.stg[j].objIndexTime] * mod.SqzOpt.X[mod.stg[j].objIndexTime] * mod.stg[j].Qwt * 60);
							}
						}
						else
						{
							TotInjTime += (mod.stg[j].Time);

							if (mod.stg[j].TypeStg != 5)
							{
								TotInjVol += (mod.stg[j].Qwt * mod.stg[j].Time * 60.0);
							}
						}
					}
				}

				TotalSI = 1e-6 * TotalSICost;
				TotalSICost = 1e-6 * (TotalSICost * mod.SqzOpt.SIBblRate);
				TotalInjCost = TotInjTime * mod.SqzOpt.InjHourRate;
				TotalCost = TotalSICost + TotalInjCost;

				P[i].Obj = JP.GetObj(i);	// SQZ_LT

				if (P[i].Obj == -1)
					tempdat.dat[mod.SqzOpt.nPar] = 0;
				else
					tempdat.dat[mod.SqzOpt.nPar] = JP.GetObj(i);	// SQZ_LT

				tempdat.dat[mod.SqzOpt.nPar + 1] = TotalCost;	// TotCost
				tempdat.dat[mod.SqzOpt.nPar + 2] = TotalInjCost;	// TotInjCost
				tempdat.dat[mod.SqzOpt.nPar + 3] = TotInjTime;	// TotInjTime
				tempdat.dat[mod.SqzOpt.nPar + 4] = TotInjVol;	// TotInjVol
				tempdat.dat[mod.SqzOpt.nPar + 5] = TotalSI;	// TotSI
				tempdat.dat[mod.SqzOpt.nPar + 6] = TotalSICost;// TotSICost
				if (tempdat.dat[mod.SqzOpt.nPar] > 0)
				{
					if (P[i].Vol_Life[0] >= 0 )
					
						CostPerVol = TotalCost / P[i].Vol_Life[0];	// Cost per barrel

					else

						CostPerVol = 0;
				}
				else
					CostPerVol = 0;

				tempdat.dat[mod.SqzOpt.nPar + 7] = CostPerVol;

				fprintf_s(mod.filePtrAll, " %16.2f %16.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f \n", TotInjTime, TotInjVol, TotalSI, TotalSICost, TotalInjCost, TotalCost, P[i].Obj, P[i].Vol_Life[0], CostPerVol);
 				
				if (P[i].Obj != -1 )
					
					opt_data.push_back(tempdat);

				fflush(mod.filePtrAll);
			}

			if (mod.Max) // Maximise Max = TRUE
				Pgen.Obj = -1e20;
			else // Minimise Max = FALSE
				Pgen.Obj = 1e20;

			for (int i = 0; i < nPopSize; i++)
			{
				if (mod.Max) // Maximise Max = TRUE
				{
					// Set Pgen, best location per generation
					if (Pgen.Obj < P[i].Obj)
					{
						Pgen.watConc = P[i].watConc;
						Pgen.Obj = P[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pgen.X[j] = P[i].X[j];
					}

					// Set Pg, best location of a particle so far
					if (Pg[i].Obj < P[i].Obj)
					{
						Pg[i].watConc = P[i].watConc;
						Pg[i].Obj = P[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pg[i].X[j] = P[i].X[j];
					}

					// Update Pi, best overall particle
					if (Pi.Obj < Pg[i].Obj)
					{
						Pi.watConc = Pg[i].watConc;
						Pi.Obj = Pg[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pi.X[j] = Pg[i].X[j];
					}
				}
				else // Minimise Max = FALSE
				{
					// Set Pgen, best location per generation
					if (Pgen.Obj > P[i].Obj)
					{
						Pgen.watConc = P[i].watConc;
						Pgen.Obj = P[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pgen.X[j] = P[i].X[j];
					}

					// Set Pg, best location of a particle so far
					if (Pg[i].Obj > P[i].Obj)
					{
						Pg[i].watConc = P[i].watConc;
						Pg[i].Obj = P[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pg[i].X[j] = P[i].X[j];
					}

					// Update Pi, best overall particle
					if (Pi.Obj > Pg[i].Obj)
					{
						Pi.watConc = Pg[i].watConc;
						Pi.Obj = Pg[i].Obj;
						for (int j = 0; j < nPar; j++)
							Pi.X[j] = Pg[i].X[j];
					}
				}
				// Calculate Particle velocities	
				double	c1 = 1,
					c2 = 1;

				for (int i = 0; i < nPopSize; i++)
				{
					for (int j = 0; j < nPar; j++)
					{
						r1 = randval(0, 1);
						r2 = randval(0, 1);
						vel[i][j] += c1 * r1 * (Pi.X[j] - P[i].X[j]) + c2 * r2 * (Pg[i].X[j] - P[i].X[j]);

						double vMax = uX[j] - lX[j];

						if (fabs(vel[i][j]) >= vMax)
						{
							if (vel[i][j] > 0) vel[i][j] = vMax;

							if (vel[i][j] <= 0) vel[i][j] = -vMax;
						}
					}
				}
			}

		}
		catch (Exception& e)
		{
			printf_s("e.what()\n");
		}

	}

	return Pi;
}

//----------------------------------------------------------------------------------------
Particle BasicDlg::RunSecantOpt()
{
	int BinsMT = 8;

	FILE *filePtrAll;

	bool check = true,
		check_sol = true;

	vector<Particle> P;
	
	errno_t err;

	CString //PathNameReadFile = "F:\\Oscar\\Old E\\Old F\\Squeeze VI Versions\\2023\\SQUEEZE 13 01.2023 Bridging Adiditive_PSO ID Parallel - Copy\\test.dat",
		file;

	vector<vector<double>>	OFRep,
							LifeRep;

	const double errval = 0.01; // 1% Percent error

	file = PathNameReadFile;

	file.Replace(".dat", "All.txt");
	file.Replace(".DAT", "All.txt");

	if ((err = fopen_s(&filePtrAll, file, "w")) != 0)
		printf("Error opening file %s for reading\r\n", "All.txt");

	Mod.ReadFileSQZ5Radial(PathNameReadFile);

	CString st;
	for (int i = 0; i < BinsMT; i++)
	{	
		fprintf_s(filePtrAll, "%11s %11s %11s ", "X0","X2","Obj");
	}

	fprintf_s(filePtrAll, "\n");
	
	int no_of_cols = BinsMT;
	int no_of_rows = Mod.SqzOpt.IterationsSecant;

	double initial_value = 0.0;

	LifeRep.resize(no_of_rows, vector<double>(no_of_cols, initial_value));
	OFRep.resize(no_of_rows, vector<double>(no_of_cols, initial_value));

	P.resize(BinsMT);

	for (int i = 0; i < BinsMT; i++)
	{
		P[i].X.resize(Mod.SqzOpt.nPar);
		P[i].Obj = 1e20;

		// Starting point the lower value of the limits
		for (int j = 0; j < Mod.SqzOpt.nPar; j++)
		{	
			if (j == 0) P[i].X[j] = 0.25*(i + 1);	// Main Treatment

			if (j == 1) P[i].X[j] = 1;				// SI concentration

			if (j == 2) P[i].X[j] = 0.1;			// 10% of the original Overflush Volume
		}
	}

	// Start with the generations,
	int n = 0;

	Parallel_Runs(P, Mod, BinsMT);

	for (int i = 0; i < BinsMT; i++)
	{
		LifeRep[n][i] = P[i].Obj;
		OFRep[n][i] = P[i].X[2];

		fprintf_s(filePtrAll, "%11.4f %11.4f", P[i].X[0], P[i].X[2]);
		fprintf_s(filePtrAll, " %11.4f ", LifeRep[n][i]);
	}

	fprintf_s(filePtrAll, "\n");
	fflush(filePtrAll);

	n = 1;
	for (int i = 0; i < BinsMT; i++)
	{
		OFRep[n][i] = OFRep[n - 1][i] + 0.1;
		P[i].X[2] = OFRep[n][i];
	}
		
	Parallel_Runs(P, Mod, BinsMT);

	for (int i = 0; i < BinsMT; i++)
	{
		LifeRep[n][i] = P[i].Obj;
		OFRep[n][i] = P[i].X[2];

		fprintf_s(filePtrAll, "%11.4f %11.4f", P[i].X[0], P[i].X[2]);
		fprintf_s(filePtrAll, " %11.4f ", LifeRep[n][i]);
	}
	fprintf_s(filePtrAll, "\n");
	fflush(filePtrAll);

	n = 2;

	while (n < Mod.SqzOpt.IterationsSecant && check)
	{	
		for (int i = 0; i < BinsMT; i++)
		{
			if (LifeRep[n - 1][i] == 0.0)
			{
				P[i].X[2] = 0.1*n;
			}
			else
			{
				if (fabs(LifeRep[n - 1][i] - LifeRep[n - 2][i]) > 1e-4)
				{
					OFRep[n][i] = OFRep[n - 1][i] + (Mod.SqzOpt.Obj_T_V - LifeRep[n - 1][i]) / (LifeRep[n - 1][i] - LifeRep[n - 2][i]) * (OFRep[n - 1][i] - OFRep[n - 2][i]);
					P[i].X[2] = OFRep[n][i];
				}
			}
		}

		Parallel_Runs(P, Mod, BinsMT);

		for (int i = 0; i < BinsMT; i++)
		{
			LifeRep[n][i] = P[i].Obj;
			OFRep[n][i] = P[i].X[2];

			fprintf_s(filePtrAll, "%11.4f %11.4f", P[i].X[0], P[i].X[2]);
			fprintf_s(filePtrAll, " %11.4f ", LifeRep[n][i]);
		}
		fprintf_s(filePtrAll, "\n");

/*			if (fabs(Mod.SqzOpt.Obj_T_V - LifeRep[n][i]) / Mod.SqzOpt.Obj_T_V < errval)

				check = false;
*/
		n++;
	}

	n = (Mod.SqzOpt.IterationsSecant - 1);
	fprintf_s(filePtrAll, "\n\nBest results\n");
	fprintf_s(filePtrAll, "%11s %11s %11s", "X[0]", "X[1]", "X[2]");
	fprintf_s(filePtrAll, " %16s %16s %11s %11s %11s %11s %11s %11s %11s\n", "TotInjTime", "TotInjVol", "TotalSI", "TotalSICost", "TotalInjCost", "TotalCost", "Obj", "T_Obj-Obj", "CostPerVol");

	for (int i = 0; i < BinsMT; i++)
	{
		double	TotInjTime = 0,
				TotInjVol = 0,
				TotalSI = 0,
				TotalSICost = 0,
				TotalInjCost = 0,
				TotalCost = 0,
				TotalTreatedVolume = 0,
				CostPerVol = 0;

		data_sqz tempdat;

		tempdat.xVal.resize(Mod.SqzOpt.nPar);
		tempdat.dat.resize(Mod.SqzOpt.nPar + 8);

		for (int j = 0; j < Mod.stg.size(); j++)
		{
			if (Mod.stg[j].TypeStg == 1)
			{
				if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 1)

					TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

				if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 0)

					TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));

				if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 1)

					TotalSICost += (Mod.stg[j].InitialCw[0] * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

				if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 0)

					TotalSICost += (Mod.stg[j].InitialCw[0] * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));
			}

			if (Mod.stg[j].TypeStg != 3)
			{
				if (Mod.stg[j].objTime == 1)
				{
					TotInjTime += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.X[Mod.stg[j].objIndexTime]);

					if (Mod.stg[j].TypeStg != 5)
					{
						TotInjVol += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]);
					}
				}
				else
				{
					TotInjTime += (Mod.stg[j].Time);

					if (Mod.stg[j].TypeStg != 5)
					{
						TotInjVol += (Mod.stg[j].Qwt * Mod.stg[j].Time * 60.0);
					}
				}
			}
		}

		TotalSI = 1e-6 * TotalSICost;
		TotalSICost = 1e-6 * (TotalSICost * Mod.SqzOpt.SIBblRate);
		TotalInjCost = TotInjTime * Mod.SqzOpt.InjHourRate;
		TotalCost = TotalSICost + TotalInjCost;

		double temp_Vlife = 0.0;

		if (Mod.kPlot == 1 || Mod.kPlot == 3)
		{
			if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') TotalTreatedVolume = LifeRep[n][i] * 100;
			else TotalTreatedVolume = LifeRep[n][i] * 1000;
		}
		else
		{
			temp_Vlife = LifeRep[n][i];

			for (int s = 0; s < Mod.stg.size(); s++)
			{
				if (Mod.stg[s].TypeStg != 3)
					temp_Vlife -= Mod.stg[s].Time / 24;
				else
				{
					if (temp_Vlife < Mod.stg[s].Time)
					{
						TotalTreatedVolume += Mod.stg[s].Qwt * temp_Vlife;
						break;
					}
					else
					{
						TotalTreatedVolume += Mod.stg[s].Qwt * Mod.stg[s].Time;
						temp_Vlife -= Mod.stg[s].Time;
					}
				}
			}
		}

		CostPerVol = TotalCost / TotalTreatedVolume;

		
		double Err = fabs(Mod.SqzOpt.Obj_T_V - LifeRep[n][i]) / Mod.SqzOpt.Obj_T_V;
		
		if ( Err < errval)
		{	
			fprintf_s(filePtrAll, "%11.2f %11.2f %11.2f", P[i].X[0], P[i].X[1], P[i].X[2]);
			fprintf_s(filePtrAll, " %16.2f %16.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f\n", TotInjTime, TotInjVol, TotalSI, TotalSICost, TotalInjCost, TotalCost, P[i].Obj, Mod.SqzOpt.Obj_T_V - LifeRep[n][i], CostPerVol);

			tempdat.dat[0] = P[i].X[0] * Mod.SqzOpt.Vol[0];
			tempdat.dat[1] = P[i].X[1] * Mod.SqzOpt.X[1];
			tempdat.dat[2] = P[i].X[2] * Mod.SqzOpt.Vol[2];

			tempdat.dat[Mod.SqzOpt.nPar] = LifeRep[n][i];	// SQZ_LT
			tempdat.dat[Mod.SqzOpt.nPar + 1] = TotalCost;	// TotCost
			tempdat.dat[Mod.SqzOpt.nPar + 2] = TotalInjCost;	// TotInjCost
			tempdat.dat[Mod.SqzOpt.nPar + 3] = TotInjTime;	// TotInjTime
			tempdat.dat[Mod.SqzOpt.nPar + 4] = TotInjVol;	// TotInjVol
			tempdat.dat[Mod.SqzOpt.nPar + 5] = TotalSI;	// TotSI
			tempdat.dat[Mod.SqzOpt.nPar + 6] = TotalSICost;// TotSICost
			tempdat.dat[Mod.SqzOpt.nPar + 7] = CostPerVol;// CostPerVol

			opt_data.push_back(tempdat);
		}
	}

	fclose(filePtrAll);

	return P[BinsMT-1];
}


void BasicDlg::Parallel_Runs(vector<Particle> &P, Model &M, int num )
{	
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;

	vector<thread*> threads(nproc);

	vector<OneRun*> OLD(num);

	for (int i = 0; i < num; i++)
	{
		OLD[i] = new OneRun;		// to be deleted manually!
		OLD[i]->P = P[i];
		OLD[i]->mod = M;
	}

	JobsPool JP(OLD);

	JP.SetNT();

	for (int j = 0; j < nproc; j++)
		threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

	// the threads are running here!

	for (int j = 0; j < nproc; j++)
	{
		threads[j]->join();		// wait for all threads to finish
		delete threads[j];		// and delete them
	}

	if (JP.E)
	{
		rethrow_exception(JP.E);	// rethrow exception from the worker thread
	}

	for (int j = 0; j < num; j++)

		P[j].Obj = JP.GetObj(j);

/*	for (int j = 0; j < num; j++)

		delete OLD[j];*/
}

// Returns a random number
double BasicDlg::randval(double low, double high)
{
	double val;

	val = double(rand() % 1000 / 1000.0) * (high - low) + low;

	return(val);
}

void BasicDlg::OnBnClickedButtonIsoTest()
{
	string	outputFile,
		pathName;

	Model M;

	CString openFilter = "Input data files (*.dat)|*.dat|";

	CFileDialog openDialog(TRUE, "inp", PathNameReadFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{
		// Read Input file
		outputFile = openDialog.GetPathName();
		Mod.ReadFileSQZ5Radial(outputFile.c_str());

		Iso_Par_Dlg IsoDlg;

		IsoDlg.M.SqzOpt.nPar = M.SqzOpt.nPar;
		IsoDlg.M.SqzOpt.nGen = M.SqzOpt.nGen;
		IsoDlg.M.SqzOpt.nPopSize = M.SqzOpt.nPopSize;
		IsoDlg.M.Iso.k = M.Iso.k;
		IsoDlg.M.Iso.n = M.Iso.n;
		IsoDlg.M.Iso.r2 = M.Iso.r2;
		IsoDlg.M.SqzOpt.uX = M.SqzOpt.uX;
		IsoDlg.M.SqzOpt.lX = M.SqzOpt.lX;

		if (IsoDlg.DoModal() == IDOK)
		{
			M.SqzOpt.nGen = IsoDlg.M.SqzOpt.nGen;
			M.SqzOpt.nPopSize = IsoDlg.M.SqzOpt.nPopSize;
			M.Iso.k = IsoDlg.M.Iso.k;
			M.Iso.n = IsoDlg.M.Iso.n;
			M.Iso.r2 = IsoDlg.M.Iso.r2;

			M.SqzOpt.uX = IsoDlg.M.SqzOpt.uX;
			M.SqzOpt.lX = IsoDlg.M.SqzOpt.lX;

			// Create Isotherm Matching File in the Optimisation format, $P...
			pathName = PathName + "Exec\\InputOpt.dat";
			M.WriteIsothermOptimisationModel(pathName.c_str());

			int i = outputFile.find_last_of(".");
			if (i < string::npos)
				outputFile = outputFile.substr(0, i);

			outputFile += "PSOResults.txt";

			pathName = PathName + "Exec";

			Par.SetRun(outputFile, 2, M);

			// Name of the file to be open
			IsoGraphs.openFile.Format("%s", outputFile.c_str());

			if (Mod.kPlot == 0)
			{
				IsoGraphs.m_Graph.xLabel = "Time (days)";
			}
			else
			{
				if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') // Metric
				{
					IsoGraphs.m_Graph.xLabel = "Vol (100m3)";
				}

				if (Mod.UnitSpec == 'c' || Mod.UnitSpec == 'C') // Conventional
				{
					IsoGraphs.m_Graph.xLabel = "Vol (1000bbls)";
				}
			}

			if (IsoGraphs.DoModal() == IDOK)
			{
				;
			}

		}
	}
}

void BasicDlg::OnBnClickedButtonMopso()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here

	string	outputFile,
		pathName;

	CString openFilter = "Input data files (*.dat)|*.dat|";

	CFileDialog openDialog(TRUE, "inp", PathNameReadFile, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{
		// Read Input file
		outputFile = openDialog.GetPathName();

		char buf[BUFSIZ];
		size_t size;

		FILE* source = fopen(outputFile.c_str(), "rb");
		FILE* dest = fopen(PathName + "\\Parallel\\InputOpt.dat", "wb");

		// clean and more secure
		// feof(FILE* stream) returns non-zero if the end of file indicator for stream is set

		while (size = fread(buf, 1, BUFSIZ, source)) {
			fwrite(buf, 1, size, dest);
		}

		std::fclose(source);
		std::fclose(dest);

		int j = outputFile.find_last_of(".");
		if (j < string::npos)
			outputFile = outputFile.substr(0, j);

		pathName = PathName + "Parallel";

		Par.SetRunMOPSO(outputFile, pathName);
	}
}

void BasicDlg::OnBnClickedSensitivityCalculations()
{
	Model M;

	M.ReadFileSQZ5Radial(PathNameReadFile);
	M.ConvertUnits();

	Par.SetRun(PathNameReadFile.GetBuffer(), 3, M);
}

int BasicDlg::CheckLicense(int &ExpDays)
{
	int stat;
	char buffer[_MAX_PATH],
		buffer1[_MAX_PATH],
		buffer2[_MAX_PATH];

	bool checkVersion = false;

	string ver = "12.0";

	// Get the current working directory: 
	_getcwd(buffer, _MAX_PATH);

	strcpy(buffer1, buffer);
	strcat(buffer1, "\\License");

	if (SetCurrentDirectory(buffer1) == 0)
	{
		AfxMessageBox("ERROR SetCurrentDirectory");
	}

	string cmd = ".\\rlm.exe ";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	/*	if (!CreateProcess(NULL, // No module name (use command line)
	LPSTR(cmd.c_str()), // Command line
	NULL, // Process handle not inheritable
	NULL, // Thread handle not inheritable
	FALSE, // Set handle inheritance to FALSE
	NULL, // No window
	NULL, // Use parent's environment block
	NULL, // Use parent's starting directory
	&si, // Pointer to STARTUPINFO structure
	&pi) // Pointer to PROCESS_INFORMATION structure
	)
	{
	AfxMessageBox("CreateProcess failed");
	}

	WaitForSingleObject(pi.hProcess, INFINITE);*/

	cmd = ".\\RLMClient.exe 0 1000 ";
	cmd += ver;

	STARTUPINFO si2;
	PROCESS_INFORMATION pi2;
	ZeroMemory(&si2, sizeof(si2));
	si2.cb = sizeof(si2);
	ZeroMemory(&pi2, sizeof(pi2));

	if (!CreateProcess(NULL, // No module name (use command line)
		LPSTR(cmd.c_str()), // Command line
		NULL, // Process handle not inheritable
		NULL, // Thread handle not inheritable
		FALSE, // Set handle inheritance to FALSE
		CREATE_NO_WINDOW, // No window
		NULL, // Use parent's environment block
		NULL, // Use parent's starting directory 
		&si2, // Pointer to STARTUPINFO structure
		&pi2) // Pointer to PROCESS_INFORMATION structure
		)
	{
		AfxMessageBox("CreateProcess failed");
	}
	WaitForSingleObject(pi2.hProcess, INFINITE);
	CloseHandle(pi2.hProcess);
	CloseHandle(pi2.hThread);

	FILE * pFile;
	char st[2000];
	vector<string> vec;

	strcpy(buffer2, buffer1);
	strcat(buffer2, "\\diag.out");
	strcat(buffer1, "\\temp.txt");

	if ((fopen_s(&pFile, buffer1, "r")) == 0)
	{
		fscanf(pFile, "%s %s %d\n", st, st, &stat);
		fscanf(pFile, "%s %s %s %s%d\n", st, st, st, st, &ExpDays);
		std::fclose(pFile);
	}
	else
	{
		AfxMessageBox("temp.txt not opened");
	}
	
	if (FileExists(buffer1))
		remove(buffer1);

/*	// Open diag.out to check Squeeze version license
	if ((fopen_s(&pFile, buffer2, "r")) == 0)
	{	
		char temp[1024];
		const char* lpszSearchString = "squeeze";

		unsigned long currentPosition = Mod.FileSearch(pFile, lpszSearchString, 0L),
					  result;

		while (currentPosition != -1)
		{
			result = fseek(pFile, currentPosition, SEEK_SET);
			if (result)
				perror("Fseek failed");
			else
			{	
				vec.clear();
				fgets(temp, 1000, pFile);
				char *p = strtok(temp, " ");
				while (p != NULL)
				{
					vec.push_back(p);
					p = strtok(NULL, " ");
				}
			}

			if (vec[1] == "v12.0")
			{
				currentPosition = -1;
				checkVersion = true;
			}
			else
				currentPosition = Mod.FileSearch(pFile, lpszSearchString, currentPosition + strlen(lpszSearchString));
		}

	}
	else
		AfxMessageBox("diag.out not opened");

	
	if (SetCurrentDirectory(buffer) == 0)
	{
		AfxMessageBox("ERROR SetCurrentDirectory");
	}

	if (!checkVersion) // Wrong Version
		stat = -1;

	*/

	if (stat ==-6) // Wrong Version
	{
		strcpy(buffer1, "");
		strcat(buffer1, "Wrong version license !!!\nYou need a license for Squeeze v");
		strcat(buffer1, ver.c_str());
		AfxMessageBox(buffer1);
	}

	return stat;
}

void BasicDlg::OnStnClickedStaticSimType()
{
	// TODO: Add your control notification handler code here
}


void BasicDlg::OnBnClickedButton2()
{

}

void BasicDlg::CreateDlg(CWnd* pParent)
{
	ProBar.Create(IDD_PROGRESS_BAR, pParent);
}

void BasicDlg::OnBnClickedButtonSecOpt()
{
	string outputFile;

	RunSecantOpt();

	Dlg_PSO_Output.nPar = Mod.SqzOpt.nPar; // Parameter space dimension

	vector<string> temp;

	temp = Mod.SqzOpt.Label;

	for (int j = 0; j < Mod.SqzOpt.nPar; j++)
	{
		temp[j] = Mod.SqzOpt.Label[j];
	}

	Dlg_PSO_Output.InputPathName = PathName + "Exec\\InputOpt.dat";	// Pathname for Design Input file save 
	Dlg_PSO_Output.SavePathName = outputFile.c_str();					// Pathname of Design Input File Save

	Dlg_PSO_Output.Label = temp;

	Dlg_PSO_Output.Label.push_back("SQZ_LT");
	Dlg_PSO_Output.Label.push_back("TotCost");
	Dlg_PSO_Output.Label.push_back("TotInjCost");
	Dlg_PSO_Output.Label.push_back("TotInjTime");
	Dlg_PSO_Output.Label.push_back("TotInjVol");
	Dlg_PSO_Output.Label.push_back("TotSI");
	Dlg_PSO_Output.Label.push_back("TotSICost");

	if (SqzOpt.Fixed_Target)
		Dlg_PSO_Output.Label.push_back("CostPerVol");

	Dlg_PSO_Output.Fixed_Target = SqzOpt.Fixed_Target;

	Dlg_PSO_Output.opt_data = opt_data;

	if (Dlg_PSO_Output.DoModal() == IDOK)
	{
		;
	}

}




