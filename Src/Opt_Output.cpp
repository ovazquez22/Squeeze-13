// Opt_Output.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Opt_Output.h"
#include "afxdialogex.h"


// Opt_Output dialog

IMPLEMENT_DYNAMIC(Opt_Output, CDialogEx)

Opt_Output::Opt_Output(CWnd* pParent /*=NULL*/)
	: CDialogEx(Opt_Output::IDD, pParent)
{
	Fixed_Target = 0; // =0 for normal PSO Optimization and =1 for Secant Optimization	
}

Opt_Output::~Opt_Output()
{
}

void Opt_Output::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_XAXIS, m_comboList_Xaxis);
	DDX_Control(pDX, IDC_COMBO_YAXIS, m_comboList_Yaxis);
	DDX_Control(pDX, IDC_STATIC_DESIGNS_HEADER, PSOGridHeader);
	DDX_Control(pDX, IDC_STATIC_DESIGNS, PSOGrid);

	DDX_Control(pDX, IDC_SCROLLBAR_HOR, m_hScrollBar);
	DDX_Control(pDX, IDC_SCROLLBAR_VER, m_vScrollBar);
}


BEGIN_MESSAGE_MAP(Opt_Output, CDialogEx)
	ON_CBN_CLOSEUP(IDC_COMBO_XAXIS, &Opt_Output::OnCbnCloseupComboXaxis)
	ON_CBN_CLOSEUP(IDC_COMBO_YAXIS, &Opt_Output::OnCbnCloseupComboYaxis)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE(IDC_STATIC_PARETO_GRAPH, OnMouseMove)
	ON_MESSAGE(UWM_CUSTOM, OnCustom)
	ON_MESSAGE(UWM_SAVEDESIGN, OnSavedesign)
	ON_MESSAGE(UWM_EXPORTTABLE, OnExporttable)
	ON_MESSAGE(UWM_EXPORTPARETO, OnExportparetofront)
	ON_BN_CLICKED(IDCANCEL, &Opt_Output::OnBnClickedCancel)
END_MESSAGE_MAP()


// Opt_Output message handlers
// CheckListBoxGraphs message handlers
BOOL Opt_Output::OnInitDialog()
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
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	int nL = Label.size();

	InitPSOGrid();

	for (int i = 0; i < nL; i++)
	{
		m_comboList_Xaxis.AddString(Label[i].c_str());
		m_comboList_Yaxis.AddString(Label[i].c_str());
	}

	if (nL>nPar+7)
	{
		m_comboList_Xaxis.SetCurSel(m_comboList_Xaxis.FindString(-1, "TotInjVol"));
		m_comboList_Yaxis.SetCurSel(m_comboList_Xaxis.FindStringExact(-1, "CostPerVol"));
	}
	else
	{
		m_comboList_Xaxis.SetCurSel(m_comboList_Xaxis.FindStringExact(-1, "TotSI"));
		m_comboList_Yaxis.SetCurSel(m_comboList_Xaxis.FindStringExact(-1, "SQZ_LT"));
	}
	   	
	InitialiseGraph();
	
	if (Fixed_Target == 0) // PSO Optimization
		
		Pareto_Front(m_comboList_Xaxis.GetCurSel(), m_comboList_Yaxis.GetCurSel());

	// Do not call CDialog::OnPaint() for painting messages
	CRect rect;
	GetDlgItem(IDC_STATIC_PARETO_GRAPH)->GetWindowRect(rect);
	ScreenToClient(rect);

/*	m_Graph.title = "";
	m_Graph.bTitle = false;*/

	m_Graph.Create(WS_CHILD | WS_VISIBLE, rect, this, IDC_GRAPH);

	int maxHScroll = __max(PSOGrid.colPos[PSOGrid.nCols - 1] - PSOGrid.clientRect.left, 0);
	m_hScrollBar.SetScrollRange(0, maxHScroll, FALSE);

	int maxVScroll = __max(PSOGrid.rowPos[PSOGrid.nRows - 1] - PSOGrid.clientRect.bottom, 0);
	m_vScrollBar.SetScrollRange(0, maxVScroll, FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control

}

void Opt_Output::InitPSOGrid()
{	
	TableGrid tempgrid(1, Label.size() + 1);
	PSOGridHeader = tempgrid;

	TableGrid tempgrid1(opt_data.size(), Label.size() + 1);
	PSOGrid = tempgrid1;

	for (int i = 0; i < PSOGrid.nCols; i++)
	{	
		if (i > 0)
		{
			PSOGridHeader.SetCellText(i, 0, Label[i - 1].c_str());
		}
		else
		{
			PSOGridHeader.SetCellText(i, 0, "Design");
		}

		PSOGrid.SetColWidth(i, 100);
		PSOGridHeader.SetColWidth(i, 100);
	}

	PSOGrid.bScroll = PSOGridHeader.bScroll = TRUE;

	PSOGrid.FieldData = PSOGridHeader.FieldData = 2;

	PSOGridHeader.FieldData = 2;

	PSOGridHeader.SetRowFormat(0, TableGrid::HEADER);
	PSOGridHeader.SetRowEdit(0, TableGrid::READ_ONLY);

	CString sItem;

	for (int r = 0; r < PSOGrid.nRows; r++)
	{	
		PSOGrid.SetRowEdit(0, TableGrid::READ_ONLY);

		for (int c = 0; c < PSOGrid.nCols; c++)
		{	
			if (c > 0) sItem.Format("%10.1f", opt_data[r].dat[c-1]);
				
			else sItem.Format("%d", r+1);

			if (c == 11) sItem.Format("%10.4f", opt_data[r].dat[c - 1]);

			PSOGrid.colS[c][r] = sItem;
		}
	}
}

void Opt_Output::InitialiseGraph()
{	
	m_Graph.logScale = false;

	m_Graph.xFont = 80;
	m_Graph.yFont = 80;

	m_Graph.xLabFont = 120; // x Label Font
	m_Graph.yLabFont = 18; // y Label Font

	m_Graph.bTitle = true;
	
	if (Fixed_Target == 0) // PSO Optimization
	{
		m_Graph.title = "All    Pareto    BaseCase";
		m_Graph.nLines = 3;
	}
	else
	{
		m_Graph.title = "";
		m_Graph.nLines = 1;
	}

	m_Graph.xLabel = "TotSI";
	m_Graph.yLabel = "SqzLT";

	m_Graph.cL = 14.0 / 80;
	m_Graph.cB = 34.0 / 40;

	delete[] m_Graph.m_Lines;

	m_Graph.m_Lines = new CGraphLine[m_Graph.nLines];

	int nPts = opt_data.size();

	for (int i = 0; i < m_Graph.nLines; i++)
	{	
		m_Graph.m_Lines[i].Point = true;

		if (i == 2)
		{
			m_Graph.m_Lines[i].nPts = 1;
			m_Graph.m_Lines[i].Point = false;
		}

		else m_Graph.m_Lines[i].nPts = opt_data.size();

		delete[] m_Graph.m_Lines[i].X;
		delete[] m_Graph.m_Lines[i].Y;

		m_Graph.m_Lines[i].X = new double[nPts];
		m_Graph.m_Lines[i].Y = new double[nPts];
	}

	int indX = m_comboList_Xaxis.GetCurSel();
	int indY = m_comboList_Yaxis.GetCurSel();

	vector<double> x, y;
	x.resize(nPts);
	y.resize(nPts);

	m_Graph.xLabel = Label[indX].c_str();

	for (int i = 0; i < nPts; i++)
		x[i] = opt_data[i].dat[indX];

	m_Graph.yLabel = Label[indY].c_str();
	for (int i = 0; i < nPts; i++)
		y[i] = opt_data[i].dat[indY];

	if (x.size()!=0 && y.size()!=0)
	UpdateGraph(x, y, indX, indY);

	m_Graph.xMin = 1e10;
	m_Graph.xMax = 0;
	m_Graph.yMin = 1e10;
	m_Graph.yMax = 0;

	for (int i = 0; i < nPts; i++)
	{
		if (m_Graph.xMin > m_Graph.m_Lines[0].X[i]) m_Graph.xMin = m_Graph.m_Lines[0].X[i];

		if (m_Graph.xMax < m_Graph.m_Lines[0].X[i]) m_Graph.xMax = m_Graph.m_Lines[0].X[i];

		if (m_Graph.yMin > m_Graph.m_Lines[0].Y[i]) m_Graph.yMin = m_Graph.m_Lines[0].Y[i];

		if (m_Graph.yMax < m_Graph.m_Lines[0].Y[i]) m_Graph.yMax = m_Graph.m_Lines[0].Y[i];
	}

	if (Fixed_Target == 0) // PSO Optimization, and default
	{

		if (abs(m_Graph.xMax - m_Graph.xMin) < 1)
		{
			m_Graph.xMin = (m_Graph.xMin*0.9);
			m_Graph.xMax = (m_Graph.xMax*1.1);
		}
		else
		{
			m_Graph.xMin = 0;
			m_Graph.xMax = ceil(m_Graph.xMax*1.01);
		}

		if (abs(m_Graph.yMax - m_Graph.yMin) < 1)
		{
			m_Graph.yMin = (m_Graph.yMin*0.9);
			m_Graph.yMax = (m_Graph.yMax*1.1);
		}
		else
		{
			m_Graph.yMin = 0;
			m_Graph.yMax = ceil(m_Graph.yMax*1.01);
		}
	}

	if (nPts == 1)
	{	
		m_Graph.xMin = (m_Graph.xMin * 0.9);
		m_Graph.xMax = (m_Graph.xMax * 1.1);
		m_Graph.yMin = (m_Graph.yMin * 0.9);
		m_Graph.yMax = (m_Graph.yMax * 1.1);
	}
}

void Opt_Output::UpdateGraph(vector<double> x, vector<double> y, int indX, int indY)
{
	int nPts = x.size();

	if (Fixed_Target == 0 ) // PSO Optimization
	{
		for (int i = 0; i < 2; i++)
		{
			m_Graph.m_Lines[i].nPts = nPts;

			delete[] m_Graph.m_Lines[i].X;
			delete[] m_Graph.m_Lines[i].Y;

			m_Graph.m_Lines[i].X = new double[nPts];
			m_Graph.m_Lines[i].Y = new double[nPts];

			m_Graph.m_Lines[i].Point = true;
		}

		if (m_Graph.nLines > 2)
		{
			m_Graph.m_Lines[2].Circle = true;
			m_Graph.m_Lines[2].X[0] = x[0];
			m_Graph.m_Lines[2].Y[0] = y[0];
		}

		for (int i = 0; i < nPts; i++)
		{
			m_Graph.m_Lines[0].X[i] = x[i];
			m_Graph.m_Lines[0].Y[i] = y[i];
		}

	}
	else
	{
		m_Graph.m_Lines[0].nPts = nPts;

		delete[] m_Graph.m_Lines[0].X;
		delete[] m_Graph.m_Lines[0].Y;

		m_Graph.m_Lines[0].X = new double[nPts];
		m_Graph.m_Lines[0].Y = new double[nPts];

		m_Graph.m_Lines[0].Point = true;

		for (int i = 0; i < nPts; i++)
		{
			m_Graph.m_Lines[0].X[i] = x[i];
			m_Graph.m_Lines[0].Y[i] = y[i];
		}
	}

	m_Graph.xMin = 1e10;
	m_Graph.xMax = 0;
	m_Graph.yMin = 1e10;
	m_Graph.yMax = 0;

	for (int i = 0; i < nPts; i++)
	{
		if (m_Graph.xMin > m_Graph.m_Lines[0].X[i]) m_Graph.xMin = m_Graph.m_Lines[0].X[i];

		if (m_Graph.xMax < m_Graph.m_Lines[0].X[i]) m_Graph.xMax = m_Graph.m_Lines[0].X[i];

		if (m_Graph.yMin > m_Graph.m_Lines[0].Y[i]) m_Graph.yMin = m_Graph.m_Lines[0].Y[i];

		if (m_Graph.yMax < m_Graph.m_Lines[0].Y[i]) m_Graph.yMax = m_Graph.m_Lines[0].Y[i];
	}

	if (Fixed_Target == 0) // PSO Optimization
	{
	/*	if (abs(m_Graph.xMax - m_Graph.xMin) < 1)
		{
			m_Graph.xMin = (m_Graph.xMin*0.9);
			m_Graph.xMax = (m_Graph.xMax*1.1);
		}
		else
		{
			m_Graph.xMin = 0;
			m_Graph.xMax = ceil(m_Graph.xMax*1.01);
		}

		if (abs(m_Graph.yMax - m_Graph.yMin) < 1)
		{
			m_Graph.yMin = (m_Graph.yMin*0.9);
			m_Graph.yMax = (m_Graph.yMax*1.1);
		}
		else
		{
			m_Graph.yMin = 0;
			m_Graph.yMax = ceil(m_Graph.yMax*1.01);
		}
		*/
		Pareto_Front(indX, indY);
	}
}

void Opt_Output::OnCbnCloseupComboXaxis()
{
	int nPts = opt_data.size();
	int indX = m_comboList_Xaxis.GetCurSel();
	int indY = m_comboList_Yaxis.GetCurSel();

	vector<double> x, y;
	x.resize(nPts);
	y.resize(nPts);

	m_Graph.xLabel = Label[indX].c_str();

	for (int i = 0; i < nPts; i++) 
		x[i] = opt_data[i].dat[indX];
	
	m_Graph.yLabel = Label[indY].c_str();
	for (int i = 0; i < nPts; i++)
		y[i] = opt_data[i].dat[indY];
		
	UpdateGraph(x, y, indX, indY);
	
	m_Graph.Update();
}

void Opt_Output::OnCbnCloseupComboYaxis()
{
	int nPts = opt_data.size();
	int indX = m_comboList_Xaxis.GetCurSel();
	int indY = m_comboList_Yaxis.GetCurSel();

	vector<double> x, y;
	x.resize(nPts);
	y.resize(nPts);

	m_Graph.xLabel = Label[indX].c_str();

	// X-Axis
	for (int i = 0; i < nPts; i++)
	x[i] = opt_data[i].dat[indX];

	// Y axis
	m_Graph.yLabel = Label[indY].c_str();
	for (int i = 0; i < nPts; i++)
	y[i] = opt_data[i].dat[indY];

	UpdateGraph(x, y, indX, indY);

	m_Graph.Update();
}

void Opt_Output::Pareto_Front(int iX, int iY)
{
	int nPts = opt_data.size();
	bool Cond1 = true,
		 Cond2 = true;
	vector<double> x, y;

	parInd.clear();

	for (int i = 0; i < nPts; i++)
	{
		Cond1 = true;
		Cond2 = true;

		int j = 0;

		while (j < nPts && (Cond1 || Cond2))
		{	
			Cond1 = true;
			Cond2 = true;

			if (i != j)
			{	
				if (iX == nPar) // SQ LT maximise
				{								
					if (opt_data[i].dat[iX] < opt_data[j].dat[iX]) Cond1 = false;
				}
				else // Others minimise
				{
					if (opt_data[i].dat[iX] > opt_data[j].dat[iX]) Cond1 = false;
				}

				if (iY == nPar) // SQ LT maximise
				{
					if (opt_data[i].dat[iY] < opt_data[j].dat[iY]) Cond2 = false;
				}
				else // Others minimise
				{	
					if (opt_data[i].dat[iY] > opt_data[j].dat[iY]) Cond2 = false;
				}
			}

			j++;
		}

		if (Cond1 || Cond2)
		{
			x.push_back(opt_data[i].dat[iX]);
			y.push_back(opt_data[i].dat[iY]);
			parInd.push_back(i);
		}
	}

	m_Graph.m_Lines[1].nPts = x.size();

	delete[] m_Graph.m_Lines[1].X;
	delete[] m_Graph.m_Lines[1].Y;

	m_Graph.m_Lines[1].X = new double[x.size()];
	m_Graph.m_Lines[1].Y = new double[x.size()];

	for (int i = 0; i < x.size(); i++)
	{
		m_Graph.m_Lines[1].X[i] = x[i];
		m_Graph.m_Lines[1].Y[i] = y[i];
	}
}



void Opt_Output::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	int maxHScroll = __max(PSOGrid.colPos[PSOGrid.nCols - 1] - PSOGrid.clientRect.right, 0);
	m_hScrollBar.SetScrollRange(0, maxHScroll, FALSE);

	switch (nSBCode)
	{
	case SB_LINEUP:
	case SB_PAGEUP:
		PSOGrid.hScrollPos -= PSOGrid.colWidth[PSOGrid.nCols - 1];
		PSOGrid.hScrollPos = __max(PSOGrid.hScrollPos, 0);
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		PSOGrid.hScrollPos += PSOGrid.colWidth[PSOGrid.nCols - 1];
		PSOGrid.hScrollPos = __min(PSOGrid.hScrollPos, maxHScroll);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		PSOGrid.hScrollPos = nPos;
		break;
	}

	m_hScrollBar.SetScrollPos(PSOGrid.hScrollPos);

	PSOGridHeader.hScrollPos = PSOGrid.hScrollPos;

	PSOGridHeader.UpdateGrid();
	PSOGrid.UpdateGrid();
	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void Opt_Output::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	
	int maxVScroll = __max(PSOGrid.rowPos[PSOGrid.nRows - 1] - PSOGrid.clientRect.bottom, 0);
	m_vScrollBar.SetScrollRange(0, maxVScroll, FALSE);

	switch (nSBCode)
	{
	case SB_LINEUP:
	case SB_PAGEUP:
		PSOGrid.vScrollPos -= PSOGrid.rowHeight[PSOGrid.nRows - 1];
		PSOGrid.vScrollPos = __max(PSOGrid.vScrollPos, 0);
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		PSOGrid.vScrollPos += PSOGrid.rowHeight[PSOGrid.nRows - 1];
		PSOGrid.vScrollPos = __min(PSOGrid.vScrollPos, maxVScroll);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		PSOGrid.vScrollPos = nPos;
		break;
	}

	m_vScrollBar.SetScrollPos(PSOGrid.vScrollPos);

	PSOGrid.UpdateGrid();

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}


void Opt_Output::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
/*	if ((nFlags & MK_RBUTTON) == MK_RBUTTON)
	{
		CClientDC dc(this);

		CString st;

		st.Format("X:%d Y:%d", point.x, point.y);

		TextOutA(dc, 10, 20, st, 10);
	}
	*/
	CDialogEx::OnMouseMove(nFlags, point);
}

LRESULT Opt_Output::OnCustom(WPARAM wparam, LPARAM lparam)
{	
	int ind= (int)wparam;
	
	for (int r = 0; r < PSOGrid.nRows; r++)
	{
		PSOGrid.SetRowFormat(r, TableGrid::NORMAL);
	}

	if (ind != -1)
	{
		PSOGrid.SetRowFormat(ind , TableGrid::BLUE);

		int maxVScroll = __max(PSOGrid.rowPos[PSOGrid.nRows - 1] - PSOGrid.clientRect.bottom, 0);
		PSOGrid.vScrollPos = __min(PSOGrid.rowPos[ind] - PSOGrid.rowHeight[0], maxVScroll);
		m_vScrollBar.SetScrollPos(PSOGrid.vScrollPos);
	}

	PSOGrid.UpdateGrid();

	return 0;
}

LRESULT Opt_Output::OnSavedesign(WPARAM wparam, LPARAM lparam)
{
	int ind = (int)wparam;

	const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
	char st[buffsize];
	sprintf_s(st, buffsize, "Des%d.dat", ind + 1);	// "task number" is written to char *ind
	string str = "PSOResults.txt";
	string saveFileName = SavePathName;
	saveFileName.replace(SavePathName.find(str), str.length(), st);

	//	Save Design File
	CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
	CFileDialog saveDialog(FALSE, _T("DAT"), saveFileName.c_str(), OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (saveDialog.DoModal() == IDOK)
	{
		WriteOptimisationFileModels(opt_data[ind].xVal, saveDialog.GetPathName());
	}

	return 0;
}

LRESULT Opt_Output::OnExporttable(WPARAM wparam, LPARAM lparam)
{
	int ind = (int)wparam;

	//	Save Design File
	CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
	CFileDialog saveDialog(FALSE, _T("DAT"), SavePathName.c_str(), OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (saveDialog.DoModal() == IDOK)
	{
		exception_ptr E;	// for handling exceptions thrown by threads

		try
		{
			FILE *file;

			char buf[BUFSIZ];
			size_t len;
			errno_t err;

			// Write Output Table File
			if ((err = fopen_s(&file, saveDialog.GetPathName(), "w")) != 0)
				printf("Error opening file %s for writing\r\n", saveDialog.GetPathName());
			else

			{
				printf("File %s was opened for writing\r\n", saveDialog.GetPathName());
				for (int i = 0; i < PSOGrid.nCols; i++)
				{
					if (i > 0)

						fprintf(file, "%-12s ", Label[i - 1].c_str());

					else

						fprintf(file, "%-12s ", "Design");
				}
				
				fprintf(file, "\n");

				for (int r = 0; r < PSOGrid.nRows; r++)
				{
					for (int c = 0; c < PSOGrid.nCols; c++)
					{
						if (c > 0)

							fprintf(file, "%-12.2f ", opt_data[r].dat[c - 1]);

						else

							fprintf(file, "%-12d ", r+1);
					}

					fprintf(file, "\n");

				}
				
			}

			
			fclose(file);
		}
		catch (...)
		{
			E = current_exception();	// "save" current exception and leave the thread
		}

	}

	return 0;
}

LRESULT Opt_Output::OnExportparetofront(WPARAM wparam, LPARAM lparam)
{
	int ind = (int)wparam;
	string str = "PSOResults.txt";
	string saveFileName = SavePathName;
	saveFileName.replace(saveFileName.find(str), str.length(), "Pareto.txt");

	//	Save Design File
	CString openFilter = "Input Data files (*.dat)|*.dat|All files (*.*)|*.*|";
	CFileDialog saveDialog(FALSE, _T("DAT"), saveFileName.c_str(), OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (saveDialog.DoModal() == IDOK)
	{
		exception_ptr E;	// for handling exceptions thrown by threads

		try
		{
			FILE *file;

			char buf[BUFSIZ];
			size_t len;
			errno_t err;

			// Write Output Table File
			if ((err = fopen_s(&file, saveDialog.GetPathName(), "w")) != 0)
				printf("Error opening file %s for writing\r\n", saveDialog.GetPathName());
			else

			{
				printf("File %s was opened for writing\r\n", saveDialog.GetPathName());
				for (int i = 0; i < PSOGrid.nCols; i++)
				{
					if (i > 0)

						fprintf(file, "%-12s ", Label[i - 1].c_str());

					else

						fprintf(file, "%-12s ", "Design");
				}

				fprintf(file, "\n");

				for (int r = 0; r < parInd.size(); r++)
				{
					for (int c = 0; c < PSOGrid.nCols; c++)
					{	

						if (c > 0)

							fprintf(file, "%-12.1f ", opt_data[parInd[r]].dat[c - 1]);

						else

							fprintf(file, "%-12d ", parInd[r] + 1);
					}

					fprintf(file, "\n");

				}

			}


			fclose(file);
		}
		catch (...)
		{
			E = current_exception();	// "save" current exception and leave the thread
		}
	}

	return 0;
}

void Opt_Output::WriteOptimisationFileModels(vector<double> dat, CString fileName)
{
	exception_ptr E;	// for handling exceptions thrown by threads

	try
	{
		FILE *source;
		
		char buf[BUFSIZ];
		size_t len;
		errno_t err;
		
		// Make a copy of Input file
		if ((err = fopen_s(&source, InputPathName.c_str(), "r")) != 0)
			printf("Error opening file %s for writing\r\n", InputPathName);
		else
			printf("File %s was opened for writing\r\n", InputPathName);

		// Write Design Input File
		
		FILE *dest;
		const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
		char ind[buffsize];
					
		if ((err = fopen_s(&dest, fileName, "w+")) != 0)
			printf("Error opening file %s for writing\r\n", fileName);
		else
			printf("File %s was opened for writing\r\n", fileName);

		/* Set pointer to beginning of file: */
		fseek(source, 0L, SEEK_SET);

		while (fgets(buf, BUFSIZ, source))
		{
			string line(buf);

			int j = 0;

			while (j < nPar)
			{
				sprintf_s(ind, buffsize, "%d", j);	// "task number" is written to char *ind
				string tind(ind);
				string wordToReplace = "$p" + tind;

				while (true)
				{
					size_t pos = line.find(wordToReplace);
					if (pos != string::npos)
					{
						sprintf_s(ind, buffsize, "%8.4f", dat[j]);
						string wordToReplaceWith(ind);
						line.replace(pos, wordToReplace.length(), wordToReplaceWith);
					}
					else
						break;
				}
				j++;
			}

			fwrite(line.c_str(), 1, line.length(), dest);
		}

		fclose(dest);
		

		fclose(source);
	}
	catch (...)
	{
		E = current_exception();	// "save" current exception and leave the thread
	}
}

void Opt_Output::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}

