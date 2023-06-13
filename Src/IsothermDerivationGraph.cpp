// IsothermDerivationGraph.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "IsothermDerivationGraph.h"
#include "afxdialogex.h"


// IsothermDerivationGraph dialog

IMPLEMENT_DYNAMIC(IsothermDerivationGraph, CDialogEx)

IsothermDerivationGraph::IsothermDerivationGraph(CWnd* pParent /*=NULL*/)
	: CDialogEx(IsothermDerivationGraph::IDD, pParent)
{
	xIso = new double [1];
	yIso = new double [1];
}

IsothermDerivationGraph::~IsothermDerivationGraph()
{
	delete [] xIso;
	delete [] yIso;
}

void IsothermDerivationGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_YLOG,m_Check_YLogScale);
	DDX_Control(pDX, IDC_CHECK_FIX_LIMITS, m_Check_Fix_Limits);
}


BEGIN_MESSAGE_MAP(IsothermDerivationGraph, CDialogEx)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_EDIT_XMIN, &IsothermDerivationGraph::OnEnChangeEditXmin)
	ON_EN_CHANGE(IDC_EDIT_XMAX, &IsothermDerivationGraph::OnEnChangeEditXmax)
	ON_EN_CHANGE(IDC_EDIT_YMIN_ISO, &IsothermDerivationGraph::OnEnChangeEditYmin)
	ON_EN_CHANGE(IDC_EDIT_YMAX_ISO, &IsothermDerivationGraph::OnEnChangeEditYmax)
	ON_EN_CHANGE(IDC_EDIT_XMIN_SIM, &IsothermDerivationGraph::OnEnChangeEditXminSim)
	ON_EN_CHANGE(IDC_EDIT_XMAX_SIM, &IsothermDerivationGraph::OnEnChangeEditXmaxSim)
	ON_EN_CHANGE(IDC_EDIT_YMIN_SIM, &IsothermDerivationGraph::OnEnChangeEditYminSim)
	ON_EN_CHANGE(IDC_EDIT_YMAX_SIM, &IsothermDerivationGraph::OnEnChangeEditYmaxSim)
	ON_EN_CHANGE(IDC_EDIT_ISO_MOVING, &IsothermDerivationGraph::OnEnChangeEditIsoMove)
	ON_BN_CLICKED(ID_AUTOMATIC, &IsothermDerivationGraph::OnBnClickedAutomatic)
	ON_BN_CLICKED(IDC_CHECK_YLOG, &IsothermDerivationGraph::OnBnClickedCheckYlog)
	ON_BN_CLICKED(ID_BUTTON_VAL_ISO, &IsothermDerivationGraph::OnBnClickedButtonValIso)
	ON_WM_SYSCOMMAND()
	ON_BN_CLICKED(ID_BUTTON_SAVE_ISO, &IsothermDerivationGraph::OnBnClickedButtonSaveIso)
	ON_BN_CLICKED(IDC_CHECK_FIX_LIMITS, &IsothermDerivationGraph::OnBnClickedCheckFixLimits)
	ON_BN_CLICKED(IDCANCEL, &IsothermDerivationGraph::OnBnClickedCancel)
END_MESSAGE_MAP()


// IsothermDerivationGraph message handlers
BOOL IsothermDerivationGraph::OnInitDialog()
{
	CDialog::OnInitDialog();

/*	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
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
	
	// TODO: Add extra initialization here
	
	CRect rect;

	// Isotherm graph
	GetDlgItem(IDC_STATIC_GRAPH_ISO_DER_CHECK)->GetWindowRect(rect);
	ScreenToClient(rect);
	
	m_Graph_Iso.cL = 16.0/80;
	m_Graph_Iso.cR = 75.0/80;
	m_Graph_Iso.cT = 3.0/40;
	m_Graph_Iso.cB = 34.0/40.0;

	m_Graph_Iso.logScale = false;

	m_Graph_Iso.xFont = 80;	
	m_Graph_Iso.yFont = 80; 
	
	m_Graph_Iso.xLabFont = 90;	
	m_Graph_Iso.yLabFont = 15;

	m_Graph_Iso.xLabel = "Concentration (ppm)";
	m_Graph_Iso.yLabel = "Adsorption (mg/L of Rock)";

	delete [] xIso;
	delete [] yIso;

	m_Graph_Iso.m_Lines[0].nPts = Mod.Ciso.size();

	xIso = new double [m_Graph_Iso.m_Lines[0].nPts];
	yIso = new double [m_Graph_Iso.m_Lines[0].nPts];

	m_Graph_Iso.xMin = 1e20;
	m_Graph_Iso.xMax = -1e20;
	m_Graph_Iso.yMin = 1e20;
	m_Graph_Iso.yMax = -1e20;

	m_Graph_Iso.m_Lines[0].nPts = Mod.Ciso.size();

	delete[] m_Graph_Iso.m_Lines[0].X;
	delete[] m_Graph_Iso.m_Lines[0].Y;

	m_Graph_Iso.m_Lines[0].X = new double[m_Graph_Iso.m_Lines[0].nPts];
	m_Graph_Iso.m_Lines[0].Y = new double[m_Graph_Iso.m_Lines[0].nPts];

	for (int i = 0; i < Mod.Ciso.size(); i++)
	{
		m_Graph_Iso.m_Lines[0].X[i] = Mod.Ciso[i];
		m_Graph_Iso.m_Lines[0].Y[i] = Mod.Giso[i];
	}

	for(int i = 0 ; i < m_Graph_Iso.m_Lines[0].nPts ; i++ )
	{	
		xIso[i] = m_Graph_Iso.m_Lines[0].X[i];
		yIso[i] = m_Graph_Iso.m_Lines[0].Y[i];

		if (m_Graph_Iso.xMax < xIso[i]) m_Graph_Iso.xMax = xIso[i];
		if (m_Graph_Iso.xMin > xIso[i]) m_Graph_Iso.xMin = xIso[i];
		if (m_Graph_Iso.yMin > yIso[i]) m_Graph_Iso.yMin = yIso[i];
		if (m_Graph_Iso.yMax < yIso[i]) m_Graph_Iso.yMax = yIso[i];
	}

	m_Graph_Iso.Create(WS_VISIBLE | WS_CHILD, rect, this, IDC_GRAPH);

	// Isotherm Check Graph
	m_Graph_Iso_Sim.title = "Data        SQZ";
	m_Graph_Iso_Sim.bTitle = true;
	m_Graph_Iso_Sim.xLabel = "Pore Volumes";
	m_Graph_Iso_Sim.yLabel = "Return Concentration (ppm)";

	m_Graph_Iso_Sim.logScale = true;
	m_Check_YLogScale.SetCheck(m_Graph_Iso_Sim.logScale);

	GetDlgItem(IDC_STATIC_GRAPH_ISO_DER_SIM_CHECK)->GetWindowRect(rect);
	ScreenToClient(rect);

	delete [] m_Graph_Iso_Sim.m_Lines;

	m_Graph_Iso_Sim.nLines = 1;
	m_Graph_Iso_Sim.m_Lines = new CGraphLine [m_Graph_Iso_Sim.nLines]; 

	m_Graph_Iso_Sim.m_Lines[0].nPts = Mod.nPts;

	m_Graph_Iso_Sim.m_Lines[0].X = new double [m_Graph_Iso_Sim.m_Lines[0].nPts];
	m_Graph_Iso_Sim.m_Lines[0].Y = new double [m_Graph_Iso_Sim.m_Lines[0].nPts];

	m_Graph_Iso_Sim.xMin = 1e20;
	m_Graph_Iso_Sim.xMax= -1e20;
	m_Graph_Iso_Sim.yMin = 1e20;
	m_Graph_Iso_Sim.yMax = -1e20;
	
	for( int i = 0 ; i < Mod.nPts ; i++ )
	{
		m_Graph_Iso_Sim.m_Lines[0].X[i] = Mod.fX[i];
		m_Graph_Iso_Sim.m_Lines[0].Y[i] = Mod.fY[i]; 

		if ( m_Graph_Iso_Sim.xMin > m_Graph_Iso_Sim.m_Lines[0].X[i] ) m_Graph_Iso_Sim.xMin = m_Graph_Iso_Sim.m_Lines[0].X[i];

		if ( m_Graph_Iso_Sim.xMax < m_Graph_Iso_Sim.m_Lines[0].X[i] ) m_Graph_Iso_Sim.xMax = 1.01*m_Graph_Iso_Sim.m_Lines[0].X[i];

		if ( m_Graph_Iso_Sim.yMin > m_Graph_Iso_Sim.m_Lines[0].Y[i] ) m_Graph_Iso_Sim.yMin = m_Graph_Iso_Sim.m_Lines[0].Y[i];

		if ( m_Graph_Iso_Sim.yMax < m_Graph_Iso_Sim.m_Lines[0].Y[i] ) m_Graph_Iso_Sim.yMax = 1.01*m_Graph_Iso_Sim.m_Lines[0].Y[i];
	}

	m_Graph_Iso_Sim.Create(WS_VISIBLE | WS_CHILD, rect, this, IDC_GRAPH);
	
	// Do not call CDialog::OnPaint() for painting messages

	CString sn;
	
	sn.Format("%.6g", m_Graph_Iso.xMin);
	GetDlgItem(IDC_EDIT_XMIN)->SetWindowText(sn);
	
	sn.Format("%.6g", ceil(m_Graph_Iso.xMax));
	GetDlgItem(IDC_EDIT_XMAX)->SetWindowText(sn);
	
	sn.Format("%.6g", m_Graph_Iso.yMin);
	GetDlgItem(IDC_EDIT_YMIN_ISO)->SetWindowText(sn);

	sn.Format("%.6g", ceil(m_Graph_Iso.yMax));
	GetDlgItem(IDC_EDIT_YMAX_ISO)->SetWindowText(sn);

	sn.Format("%.6g", m_Graph_Iso_Sim.xMin);
	GetDlgItem(IDC_EDIT_XMIN_SIM)->SetWindowText(sn);
	
	sn.Format("%.6g", m_Graph_Iso_Sim.xMax);
	GetDlgItem(IDC_EDIT_XMAX_SIM)->SetWindowText(sn);
	
	sn.Format("%.6g", m_Graph_Iso_Sim.yMin);
	GetDlgItem(IDC_EDIT_YMIN_SIM)->SetWindowText(sn);

	sn.Format("%.6g", m_Graph_Iso_Sim.yMax);
	GetDlgItem(IDC_EDIT_YMAX_SIM)->SetWindowText(sn);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void IsothermDerivationGraph::OnEnChangeEditXmin()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_XMIN)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso.xMin = atof(sEdit);
	}
	
	m_Graph_Iso.Update();

}

void IsothermDerivationGraph::OnEnChangeEditXmax()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_XMAX)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso.xMax = atof(sEdit);
	}
	
	m_Graph_Iso.Update();
}

void IsothermDerivationGraph::OnEnChangeEditYmin()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_YMIN_ISO)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso.yMin = atof(sEdit);
	}

	m_Graph_Iso.Update();
}

void IsothermDerivationGraph::OnEnChangeEditYmax()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_YMAX_ISO)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso.yMax = atof(sEdit);
	}

	m_Graph_Iso.Update();
}

void IsothermDerivationGraph::OnEnChangeEditXminSim()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_XMIN_SIM)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso_Sim.xMin = atof(sEdit);
	}
	
	m_Graph_Iso_Sim.Update();
}

void IsothermDerivationGraph::OnEnChangeEditXmaxSim()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_XMAX_SIM)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso_Sim.xMax = atof(sEdit);
	}
	
	m_Graph_Iso_Sim.Update();
}

void IsothermDerivationGraph::OnEnChangeEditYminSim()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_YMIN_SIM)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso_Sim.yMin = atof(sEdit);
	}

	m_Graph_Iso_Sim.Update();
}

void IsothermDerivationGraph::OnEnChangeEditYmaxSim()
{
	CString sEdit;

	GetDlgItem(IDC_EDIT_YMAX_SIM)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph_Iso_Sim.yMax = atof(sEdit);
	}

	m_Graph_Iso_Sim.Update();
}

void IsothermDerivationGraph::OnBnClickedAutomatic()
{
	// TODO: Add your control notification handler code here
	double yAuto;

	yAuto = (yIso[1] - yIso[0])/(xIso[1] - xIso[0]);
	yAuto *= (-xIso[1]);
	yAuto += (yIso[1]);
	yAuto *= -1;

	CString sn;
	
	sn.Format("%.6g", yAuto);
	GetDlgItem(IDC_EDIT_ISO_MOVING)->SetWindowText(sn);

	GetDlgItem(ID_AUTOMATIC)->EnableWindow(false);
	sn.Format("Isotherm Automatically Adjusted by %.6g", yAuto);
	GetDlgItem(IDC_STATIC_AUTO)->SetWindowText(sn);

	m_Graph_Iso.Update();
}

void IsothermDerivationGraph::OnEnChangeEditIsoMove()
{
	CString sEdit;
	double yAuto = 0;

	GetDlgItem(IDC_EDIT_ISO_MOVING)->GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		yAuto = atof(sEdit);
	}

	for(int i = 0 ; i < m_Graph_Iso.m_Lines[0].nPts ; i++)
		
		m_Graph_Iso.m_Lines[0].Y[i] = yIso[i] + yAuto;

	CButton *m_ctlCheck = (CButton*) GetDlgItem(IDC_CHECK_FIX_LIMITS);
	int ChkBox = m_ctlCheck->GetCheck();
	
	if(ChkBox == BST_UNCHECKED)
	{
		sEdit.Format("%.6g", m_Graph_Iso.xMin);
		GetDlgItem(IDC_EDIT_XMIN)->SetWindowText(sEdit);
	
		sEdit.Format("%.6g", ceil(m_Graph_Iso.xMax));
		GetDlgItem(IDC_EDIT_XMAX)->SetWindowText(sEdit);
		
		double num;

		num = ceil(m_Graph_Iso.m_Lines[0].Y[0]);
		if (num < 0) num += 1;
		sEdit.Format("%.5g", num);
		GetDlgItem(IDC_EDIT_YMIN_ISO)->SetWindowText(sEdit);
		
		num = ceil(m_Graph_Iso.m_Lines[0].Y[m_Graph_Iso.m_Lines[0].nPts - 1]);
		sEdit.Format("%.5g", num);
		GetDlgItem(IDC_EDIT_YMAX_ISO)->SetWindowText(sEdit);
	}

	m_Graph_Iso.Update();
}



void IsothermDerivationGraph::OnBnClickedCheckYlog()
{
	// TODO: Add your control notification handler code here
	m_Graph_Iso_Sim.logScale = m_Check_YLogScale.GetCheck();

	m_Graph_Iso_Sim.Update();
}

void IsothermDerivationGraph::OnBnClickedButtonValIso()
{	
	// This is to prevent running this method as when "Enter is pressed"
	// it calls IDOK by defualt, that in this case is this method
	CWnd* pwndCtrl = GetFocus();
    CWnd* pwndCtrlNext = pwndCtrl;
    int ctrl_ID = pwndCtrl->GetDlgCtrlID();
	
	if ( ctrl_ID == ID_BUTTON_VAL_ISO)
	{
		char st[2000];
		Sim_Running_Dlg SRD;
		SRD.Create(IDD_DIALOG_SIM_RUN,this);
		SRD.MoveWindow( 200, 200, 260, 100);

		Mod.Iso.IsoTyp = 3;
		Mod.Iso.TypeAds = 0;

		if (m_Graph_Iso.m_Lines[0].X[0] > 0)
		{	
			Mod.Iso.nIso = m_Graph_Iso.m_Lines[0].nPts+1;
			Mod.Iso.Ciso.resize(Mod.Iso.nIso);
			Mod.Iso.ADiso.resize(Mod.Iso.nIso);
			Mod.Iso.Ciso[0] = 0;
			Mod.Iso.ADiso[0] = 0;
		}
		
		for (int j = 0; j < m_Graph_Iso.m_Lines[0].nPts; j++)
		{
			Mod.Iso.Ciso[j+1] = m_Graph_Iso.m_Lines[0].X[j];
			Mod.Iso.ADiso[j+1] = m_Graph_Iso.m_Lines[0].Y[j];
		}		

		// Add the Isotherm validation
		Mod.LinearCore_SP();

		m_Graph_Iso_Sim.nLines = 2;
		delete [] m_Graph_Iso_Sim.m_Lines;
		m_Graph_Iso_Sim.m_Lines = new CGraphLine[m_Graph_Iso_Sim.nLines];

		for (int i = 0; i < m_Graph_Iso_Sim.nLines; i++)
		{
			delete[] m_Graph_Iso_Sim.m_Lines[i].X;
			delete[] m_Graph_Iso_Sim.m_Lines[i].Y;
		}

		m_Graph_Iso_Sim.m_Lines[0].nPts = Mod.nPts;

		m_Graph_Iso_Sim.m_Lines[0].X = new double[m_Graph_Iso_Sim.m_Lines[0].nPts];
		m_Graph_Iso_Sim.m_Lines[0].Y = new double[m_Graph_Iso_Sim.m_Lines[0].nPts];

		for (int i = 0; i < Mod.nPts; i++)
		{
			m_Graph_Iso_Sim.m_Lines[0].X[i] = Mod.fX[i];
			m_Graph_Iso_Sim.m_Lines[0].Y[i] = Mod.fY[i];
		}

		m_Graph_Iso_Sim.m_Lines[1].nPts = Mod.RT.size();

		m_Graph_Iso_Sim.m_Lines[1].X = new double[Mod.RT.size()];
		m_Graph_Iso_Sim.m_Lines[1].Y = new double[Mod.RT.size()];

		for (int i = 0; i < Mod.RT.size(); i++)
		{
			m_Graph_Iso_Sim.m_Lines[1].X[i] = Mod.RT[i];
			m_Graph_Iso_Sim.m_Lines[1].Y[i] = Mod.RC[i];
		}

		CString sn;
	
		sn.Format("%.6g", m_Graph_Iso_Sim.xMin);
		GetDlgItem(IDC_EDIT_XMIN_SIM)->SetWindowText(sn);
	
		sn.Format("%.6g", m_Graph_Iso_Sim.xMax);
		GetDlgItem(IDC_EDIT_XMAX_SIM)->SetWindowText(sn);
	
		sn.Format("%.6g", m_Graph_Iso_Sim.yMin);
		GetDlgItem(IDC_EDIT_YMIN_SIM)->SetWindowText(sn);

		sn.Format("%.6g", m_Graph_Iso_Sim.yMax);
		GetDlgItem(IDC_EDIT_YMAX_SIM)->SetWindowText(sn);

		m_Graph_Iso_Sim.Update();
		
	}
}

void IsothermDerivationGraph::OnBnClickedButtonSaveIso()
{
	// TODO: Add your control notification handler code here
	int j;

	FILE *filePtr;

	CString pathname,
		    outFileName,
			openFilter = "Adsorption Isotherm (*.txt)|*.txt|All Files (*.*)|*.*||";
	
	outFileName = Mod.ReadFileName;

	outFileName.Replace(".dat","Iso.txt");
	pathname = Mod.PathName + "Isotherms\\" + outFileName;

	CFileDialog saveDialog(FALSE, "txt", pathname, OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST, openFilter, this);

	if (saveDialog.DoModal() == IDOK)
	{	
		pathname = saveDialog.GetPathName();
		
		if ( (filePtr=fopen(pathname, "w")) == NULL)
		{
			printf("Couldn't write to this file.\n");
		}			 

		fprintf( filePtr, "nPts\n"); 
		fprintf( filePtr, "%d\n", m_Graph_Iso.m_Lines[0].nPts);
		
		fprintf( filePtr, "%s\t\t%s\n","Ciso(ppm)","ADiso(mg/L rock)");

		for (j=0; j<m_Graph_Iso.m_Lines[0].nPts; j++)
		{	
			fprintf( filePtr, "%lf\t\t%lf \n", m_Graph_Iso.m_Lines[0].X[j], m_Graph_Iso.m_Lines[0].Y[j]);
		}

		fclose(filePtr);
	}
}

int IsothermDerivationGraph::ProcessMessageLoop() 
{

	MSG msg;
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) ) {

		if (WM_QUIT == msg.message) {
			// Abort all processing
			::PostQuitMessage(0);
			break;
		}

		if (!AfxGetApp()->PreTranslateMessage(&msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			}
		} 

	// let MFC do its idle processing
	AfxGetApp()->OnIdle (0);
	AfxGetApp()->OnIdle (1);
	// Perform some background processing here 
	// using another call to OnIdle
		
	return 0;
}


void IsothermDerivationGraph::OnBnClickedCheckFixLimits()
{
	// TODO: Add your control notification handler code here
	
}


void IsothermDerivationGraph::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}
