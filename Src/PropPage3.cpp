// PropPage3.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage3.h"
#include "Sheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropPage3 property page

IMPLEMENT_DYNCREATE(PropPage3, CPropertyPage)

PropPage3::PropPage3() : CPropertyPage(PropPage3::IDD)
{
	//{{AFX_DATA_INIT(PropPage3)
	//}}AFX_DATA_INIT
	nStg = 1;
	nLay = 1;
	nComp = 1;
	iModel = 3;
	Header = 1;
	NoTstep = 1;
	m_TimeStep = 0;
	Allocate();
}

PropPage3::PropPage3(int s, int l, int c) : CPropertyPage(PropPage3::IDD)
{
	//{{AFX_DATA_INIT(PropPage1)	
	//}}AFX_DATA_INIT
	nStg = s;
	nLay = l;
	nComp = c;
	iModel = 3;
	Header = 1;
	NoTstep = 1;
	Allocate();
}

PropPage3::~PropPage3()
{
	DeAllocate();
}

void PropPage3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropPage3)
	DDX_Control(pDX, IDC_CHECK_HEADERS, m_Headers);
	DDX_Control(pDX, IDC_NEW_STAGE_POS, m_Edit_NewStage_Pos);
	DDX_Control(pDX, IDC_CHECK_TIMESTEP, m_AutoTimeStep);
	DDX_Control(pDX, IDC_STATIC_STAGEGRID, StageGrid);
	DDX_Radio(pDX, IDC_RADIO_LOW, m_TimeStep);;
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropPage3, CPropertyPage)
	ON_EN_CHANGE(IDC_STATIC_STAGEGRID, OnEnChangeStageGrid)
	//{{AFX_MSG_MAP(PropPage3)
	ON_BN_CLICKED(IDC_UPDATE_ROWS, OnUpdateRows)
	ON_BN_CLICKED(IDC_CHECK_TIMESTEP, OnAutomaticTimeStep)
	ON_BN_CLICKED(IDC_ADD_STAGE, OnAddStage)
	ON_BN_CLICKED(IDC_REMOVE_STAGE, OnRemoveStage)
	ON_BN_CLICKED(IDC_CHECK_HEADERS, OnCheckHeaders)
	ON_BN_CLICKED(IDC_RADIO_LOW, OnLowTimeStep)
	ON_BN_CLICKED(IDC_RADIO_HIGH, OnHighTimeStep)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE,OnListCtrlUpdate)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, &PropPage3::OnBnClickedButtonUpdate)
	ON_STN_CLICKED(IDC_STATIC_STAGEGRID, &PropPage3::OnStnClickedStaticStagegrid)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropPage3 message handlers

void PropPage3::OnUpdateRows() 
{
	// TODO: Add your control notification handler code here
}


void PropPage3::OnAutomaticTimeStep() 
{
	// TODO: Add your control notification handler code here
	CString sItem;
	int numSt = 0;
	
	double dat;

	if (m_TimeStep == 1 ) dat = 5;

	else dat = 1;
	
	for ( int r = 0 ; r < 2*nStg ; r++ )
	{	
		if( r%2 == 1 )
		{
			if ( m_AutoTimeStep.GetCheck() )
			{
				StageGrid.cellEdit[8][r] = testgrid::READ_ONLY;
				StageGrid.cellFormat[8][r] = testgrid::GRAY;
				sItem = " ";
				NoTstep = 1;
			}
			else
			{	
				StageGrid.cellEdit[8][r] = testgrid::EDIT;
				StageGrid.cellFormat[8][r] = testgrid::NORMAL;
				if (Stage[numSt].TimeStep > 1)
				{
					sItem.Format( Stage[numSt].TimeStep <= (1e6-1) ? "%.6g":"%.2e", dat * Stage[numSt].TimeStep );
					sItem.Replace(_T("e+000"), _T("e"));
					sItem.Replace(_T("e+00"), _T("e"));
					sItem.Replace(_T("e+0"), _T("e"));
				}
				else
				{
					sItem.Format( Stage[numSt].TimeStep>1e-3 ? "%.6g":"%.2e", dat * Stage[numSt].TimeStep );
					sItem.Replace(_T("e-000"), _T("e-"));
					sItem.Replace(_T("e-00"), _T("e-"));
					sItem.Replace(_T("e-0"), _T("e-"));
				}

				NoTstep = 0;
			}
			
			if( Stage[numSt].TimeStep < 1e-20 )
				
				StageGrid.colS[8][r] = "0.01";
			
			else
			
				StageGrid.colS[8][r] = sItem;
			
			numSt++;
		}

	}

	Invalidate(FALSE);

}

void PropPage3::OnLowTimeStep()
{	
	m_AutoTimeStep.SetCheck(FALSE);
	m_TimeStep = 0;
	OnAutomaticTimeStep();
}

void PropPage3::OnHighTimeStep()
{	
	m_AutoTimeStep.SetCheck(FALSE);
	m_TimeStep = 1;
	OnAutomaticTimeStep();
}
BOOL PropPage3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
		
	int num;
	
	m_Edit_NewStage_Pos.SetWindowText("1");

	m_AutoTimeStep.SetCheck( NoTstep ? BST_CHECKED : BST_UNCHECKED); 
	
	if( Header )
	{			
		m_Headers.SetCheck( BST_CHECKED);
	}	
	else
	{	
		m_Headers.SetCheck( BST_UNCHECKED);
	}
	
	num = 2 * nStg;

	testgrid tempgrid( num, 9 );
	
	StageGrid = tempgrid;
	
	StageGrid.kTemp = kTemp;
	
//	StageGrid.nDisabled = nDisabled;

	InitialiseGrid();
	
	UpdateControls();
	
/*	if (!nDisabled)
	{
		m_AutoTimeStep.EnableWindow(FALSE);

		StageGrid.SetColFormat(2, testgrid::GRAY);
		StageGrid.SetColEdit(2, testgrid::READ_ONLY);
		
		for ( r = 0; r < 2 * nStg ; r++ )
	
			if( r%2 == 0 )
			
				StageGrid.SetRowFormat(r, testgrid::HEADER);

		StageGrid.StgDlg.nDisabled = nDisabled;
	}
*/	
	if (!nDisabled)
	{	
		m_AutoTimeStep.SetCheck( BST_UNCHECKED );
		m_AutoTimeStep.EnableWindow(FALSE);
	}

	StageGrid.StgDlg.nDisabled = nDisabled;
	
	// Headers or not headers not ready yet. There was some problems
	m_Headers.SetCheck( BST_CHECKED );
//	m_Headers.EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PropPage3::Equate(const PropPage3& rhs)
{
	nLay = rhs.nLay;
	
	nComp = rhs.nComp;

	StageGrid = rhs.StageGrid;

	for (int s = 0; s < __min(nStg, rhs.nStg); s++)
	{
		Stage[s] = rhs.Stage[s];
	}
	
/*	for (s = 0; s < __min(nComp, rhs.nComp); s++)
	{
		compRole[s] = rhs.compRole[s];
	}
*/
}

PropPage3& PropPage3::operator=(const PropPage3& rhs)
{
	if (this == &rhs) return *this;
	DeAllocate();
	nStg = rhs.nStg;
	nLay = rhs.nLay;
	nComp = rhs.nComp;
	Allocate();
	Equate(rhs);
	return *this;
}

void PropPage3::Allocate()
{	
	int num, i;

	Stage.resize(nStg);
	
	StageGrid.StgDlg.Allocate(nStg, nLay, nComp);
	
	num = 2 * nStg;
	
	testgrid tempgrid( num, 9 );
	
	StageGrid = tempgrid;
	StageGrid.kTemp = kTemp;

	m_TimeStep = 0;
}

void PropPage3::DeAllocate()
{

}

void PropPage3::UpdateControls()
{	
	CString sItem1, 
			  sItem2 = "Click here";
	
	int r,
		 numSt = 0;
	
	StageGrid.StgDlg.Allocate(nStg, nLay, nComp);
	
	StageGrid.StgDlg.nLay = nLay;

	StageGrid.StgDlg.nComp = nComp;

	for ( r = 0 ; r < nStg ; r++ )
	{	
		StageGrid.StgDlg.Stage[r] = Stage[r];
		
	}
	
	for (r = 0 ; r < nStg ; r++)
	{ 
		StageGrid.StgDlg.Stage[r].nChage = 1;
	}

	for ( r = 0 ; r < 2*nStg ; r++ )
	{	
		if( r%2 == 1 )
		{
			if ( Stage[numSt].TypeStg == 1 )
			{	
				if (nDisabled) // TwoPhase
				{	
					sItem1.Format((Stage[numSt].watFrac <= (1e6-1)) ? "%.6g" : "%.2e" , Stage[numSt].watFrac );
					sItem1.Replace(_T("e+000"), _T("e"));
					sItem1.Replace(_T("e+00"), _T("e"));
					sItem1.Replace(_T("e+0"), _T("e"));
					StageGrid.colS[2][r] = sItem1;
				}
				else // Single Phase
				{	
					if( kTemp) // Temperature
					{
						sItem1.Format((Stage[numSt].TI <= (1e6-1)) ? "%.6g" : "%.2e" , Stage[numSt].TI );
						sItem1.Replace(_T("e+000"), _T("e"));
						sItem1.Replace(_T("e+00"), _T("e"));
						sItem1.Replace(_T("e+0"), _T("e"));
						StageGrid.colS[2][r] = sItem1;
					}
					else // No Temperature
					{
						StageGrid.colS[2][r] = " ";
						StageGrid.cellEdit[2][r] = testgrid::READ_ONLY;
						StageGrid.cellFormat[2][r] = testgrid::GRAY;
					}
				}		
				
				sItem1 = "Inj/Preflush";
				
				sItem2 = "Click here";
			}
			
			if ( Stage[numSt].TypeStg == 2 )
			{	
				if (nDisabled) // TwoPhase
				{	
					sItem1.Format((Stage[numSt].watFrac <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].watFrac );
					sItem1.Replace(_T("e+000"), _T("e"));
					sItem1.Replace(_T("e+00"), _T("e"));
					sItem1.Replace(_T("e+0"), _T("e"));
					StageGrid.colS[2][r] = sItem1;
				}
				else // Single Phase
				{	
					if( kTemp) // Temperature
					{
						sItem1.Format((Stage[numSt].TI <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].TI );
						sItem1.Replace(_T("e+000"), _T("e"));
						sItem1.Replace(_T("e+00"), _T("e"));
						sItem1.Replace(_T("e+0"), _T("e"));
						StageGrid.colS[2][r] = sItem1;
					}
					else // No Temperature
					{
						StageGrid.colS[2][r] = " ";
						StageGrid.cellEdit[2][r] = testgrid::READ_ONLY;
						StageGrid.cellFormat[2][r] = testgrid::GRAY;
					}
				}
				
				sItem1 = "Overflush";
				
				sItem2 = "Click here";
			}
			
			if ( Stage[numSt].TypeStg == 3 )
			{	
				sItem1 = "Production";
				
				sItem2 = "Click here";

				StageGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				StageGrid.cellFormat[2][r] = testgrid::GRAY;
				StageGrid.colS[2][r] = " ";
			}

			StageGrid.colS[1][r] = sItem1;
			
			if (Stage[numSt].TypeStg == 1)
			{	
				StageGrid.colS[3][r].Format("[SI] %7.1f", Stage[numSt].InitialCw[0]);
			}
			else
			
				StageGrid.colS[3][r] = sItem2;

			StageGrid.cellEdit[3][r] = testgrid::BICOND;
			
			StageGrid.colS[4][r] = "F/V";
			
			sItem1.Format((Stage[numSt].Qwt <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].Qwt );
			sItem1.Replace(_T("e+000"), _T("e"));
			sItem1.Replace(_T("e+00"), _T("e"));
			sItem1.Replace(_T("e+0"), _T("e"));

			StageGrid.colS[5][r] = sItem1;
			
			sItem1.Format((Stage[numSt].Time <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].Time );
			sItem1.Replace(_T("e+000"), _T("e"));
			sItem1.Replace(_T("e+00"), _T("e"));
			sItem1.Replace(_T("e+0"), _T("e"));

			StageGrid.colS[6][r] = sItem1;
			
			if ( Stage[numSt].TypeStg == 3 )
			{
				sItem1.Format((Stage[numSt].Qwt <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].Time * Stage[numSt].Qwt  );
				sItem1.Replace(_T("e+000"), _T("e"));
				sItem1.Replace(_T("e+00"), _T("e"));
				sItem1.Replace(_T("e+0"), _T("e"));
			}
			else
			{
				if (iModel == 3)
				{	
					sItem1.Format(( (Stage[numSt].Time * Stage[numSt].Qwt * 60) <= (1e6-1)) ? "%.4g" : "%.2e" , Stage[numSt].Time * Stage[numSt].Qwt * 60  );
					sItem1.Replace(_T("e+000"), _T("e"));
					sItem1.Replace(_T("e+00"), _T("e"));
					sItem1.Replace(_T("e+0"), _T("e"));
				}
				else
				{	
					sItem1.Format( (Stage[numSt].Time * Stage[numSt].Qwt) <= (1e6-1) ? "%.4g" : "%.2e", Stage[numSt].Time * Stage[numSt].Qwt);
					sItem1.Replace(_T("e+000"), _T("e"));
					sItem1.Replace(_T("e+00"), _T("e"));
					sItem1.Replace(_T("e+0"), _T("e"));
				}
			}

			StageGrid.colS[7][r] = sItem1;
			StageGrid.cellEdit[6][r] = testgrid::FTV;
			StageGrid.cellFormat[6][r] = testgrid::GRAY;
			
			if ( m_AutoTimeStep.GetCheck() )
			{
				StageGrid.cellEdit[8][r] = testgrid::READ_ONLY;
				StageGrid.cellFormat[8][r] = testgrid::GRAY;
				sItem1 = " ";
				NoTstep = 1;
			}
			else
			{
				StageGrid.cellEdit[8][r] = testgrid::EDIT;
				StageGrid.cellFormat[8][r] = testgrid::NORMAL;
				
				if (Stage[numSt].TimeStep > 1)
				{
					sItem1.Format( Stage[numSt].TimeStep <= (1e6-1) ? "%.4g" : "%.2e", Stage[numSt].TimeStep );
					sItem1.Replace(_T("e+000"), _T("e"));
					sItem1.Replace(_T("e+00"), _T("e"));
					sItem1.Replace(_T("e+0"), _T("e"));
				}
				else
				{
					sItem1.Format( Stage[numSt].TimeStep>1e-3 ? "%.4g":"%.2e", Stage[numSt].TimeStep );
					sItem1.Replace(_T("e-000"), _T("e-"));
					sItem1.Replace(_T("e-00"), _T("e-"));
					sItem1.Replace(_T("e-0"), _T("e-"));
				}

				NoTstep = 0;
			}
			
			StageGrid.colS[8][r] = sItem1;
			
			
			if ( Stage[numSt].TypeStg == 5 )
			{	

				for( int c = 2 ; c < 8 ; c++ )
					
					if( c!=6 )
					{
						StageGrid.cellEdit[c][r] = testgrid::READ_ONLY;
						StageGrid.cellFormat[c][r] = testgrid::GRAY;
					//	StageGrid.colS[c][r] = " ";
					}

					else

						StageGrid.colS[c][r-1] = "Time\nhours";
				
			}

			if ( Stage[numSt].TypeStg == 5 )
			{
				StageGrid.colS[1][r] = "Shut-in";

				StageGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				StageGrid.cellFormat[2][r] = testgrid::GRAY;
				StageGrid.colS[2][r] = " ";
				
				StageGrid.cellEdit[3][r] = testgrid::BICOND;
				StageGrid.cellFormat[3][r] = testgrid::NORMAL;
				StageGrid.colS[3][r] = "Pres.Lay";

				StageGrid.colS[4][r] = " ";
				StageGrid.colS[5][r] = " ";
				StageGrid.colS[7][r] = " ";

				StageGrid.cellEdit[6][r] = testgrid::EDIT;
				StageGrid.cellFormat[6][r] = testgrid::NORMAL;
			}

			numSt++;
		}

	}
	
	UpdateTimeVol();
	
	GetDlgItem(IDC_STATIC_VOL_INJ)->ShowWindow(TRUE);
	GetDlgItem(IDC_STATIC_VOL_PROD)->ShowWindow(TRUE);
	GetDlgItem(IDC_BUTTON_UPDATE)->ShowWindow(TRUE);

	Invalidate(FALSE);

	if (iModel == 5) // Isotherm Derivation 
	{	
		int cellC, cellR;
		double Temp;

		cellC = 4;
		cellR = 1;
		
		StageGrid.SetCellText(cellC,cellR,"F/V");
			
		StageGrid.cellEdit[cellC+1][cellR] = testgrid::EDIT;
		StageGrid.cellFormat[cellC+1][cellR] = testgrid::NORMAL;
		StageGrid.cellEdit[cellC+3][cellR] = testgrid::EDIT;
		StageGrid.cellFormat[cellC+3][cellR] = testgrid::NORMAL;

		StageGrid.cellEdit[cellC+2][cellR] = testgrid::FTV;
		StageGrid.cellFormat[cellC+2][cellR] = testgrid::GRAY;

		if ( StageGrid.colS[cellC-3][cellR] == "Production")
			
			Temp = atof(StageGrid.colS[cellC+3][cellR]) / atof(StageGrid.colS[cellC+1][cellR]);

		else
		{
			Temp = atof(StageGrid.colS[cellC+3][cellR]) / atof(StageGrid.colS[cellC+1][cellR]);
			
			if (iModel == 3) Temp /= 60;
		}

		sItem1.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
		sItem1.Replace(_T("e+000"), _T("e"));
		sItem1.Replace(_T("e+00"), _T("e"));
		sItem1.Replace(_T("e+0"), _T("e"));

		StageGrid.colS[cellC+2][cellR] = sItem1;
		
		GetDlgItem(IDC_STATIC_VOL_INJ)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_VOL_PROD)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_UPDATE)->ShowWindow(FALSE);

		Invalidate(FALSE);
	}
}

void PropPage3::UpdateFromEdits()
{	
	CString sItem;
	int r,
		 numSt = 0;
	
	for ( r = 0 ; r < nStg ; r++ )
		
		Stage[r] = StageGrid.StgDlg.Stage[r];

	for ( r = 0 ; r < 2*nStg ; r++ )
	{	
		if( r%2 == 1 )
		{	
			if( StageGrid.colS[5][r] != "" )
			{
				if ( StageGrid.colS[1][r] == "Inj/Preflush" )
				{
					Stage[numSt].TypeStg = 1;
					
					if ( nDisabled )
						
						Stage[numSt].watFrac = atof(StageGrid.colS[2][r]);
					
					else

						Stage[numSt].TI = atof(StageGrid.colS[2][r]);

					CString st = StageGrid.colS[3][r];
					st.Replace("[SI]"," ");
					Stage[numSt].InitialCw[0] = atof(st);

				}

				if ( StageGrid.colS[1][r] == "Overflush" )
				{
					Stage[numSt].TypeStg = 2;
					
					if ( nDisabled )
						
						Stage[numSt].watFrac = atof(StageGrid.colS[2][r]);
					
					else

						Stage[numSt].TI = atof(StageGrid.colS[2][r]);
				}

				if ( StageGrid.colS[1][r] == "Production" )
				
					Stage[numSt].TypeStg = 3;
				
				if ( StageGrid.colS[1][r] == "Shut-in" )
				
					Stage[numSt].TypeStg = 5;
				
				Stage[numSt].Qwt = atof(StageGrid.colS[5][r]);

				Stage[numSt].Time = atof(StageGrid.colS[6][r]);
				
				Stage[numSt].TimeStep = atof(StageGrid.colS[8][r]);

				numSt++;

			}
		
		}

	}
	
	for ( r = 0 ; r < nStg ; r++ )
		
		StageGrid.StgDlg.Stage[r] = Stage[r];
	
	Invalidate(NULL);
}

void PropPage3::SetHeaderRowStage(int r, int st)
{	
	StageGrid.SetCellText(0, r, "Stage");
	StageGrid.SetColWidth(0, 43);
	StageGrid.SetCellText(1, r, "Type");
	StageGrid.SetColWidth(1, 67);
	
	if (nDisabled)	StageGrid.SetCellText(2, r, "Qw/Qt");
	
	else	StageGrid.SetCellText(2, r, "TI");
	
	StageGrid.SetColWidth(2, 45);
	StageGrid.SetCellText(3, r, "Inj/Prod Controls");
	StageGrid.SetColWidth(3, 75);
	StageGrid.SetCellText(4, r, "F/T/V");
	StageGrid.SetColWidth(4, 45);
	
	if ( iModel == 3 )
	{
		if( Stage[st].TypeStg == 1 || Stage[st].TypeStg == 2 )
		{
			StageGrid.SetCellText(5, r, CString("Flow\n ")+CString( (Units) ? "bbl/min" : "m3/min") );
			StageGrid.SetColWidth(5, 51);
			StageGrid.SetCellText(6, r, "Time \nhours");
			StageGrid.SetColWidth(6, 51);
			StageGrid.SetCellText(7, r, CString("Vol\n ")+CString((Units) ? "bbl" : "m3") );
			StageGrid.SetColWidth(7, 51);
			StageGrid.SetCellText(8, r, "T. Step \nhours");
			StageGrid.SetColWidth(8, 53);
		}
		
		if( Stage[st].TypeStg == 3 )
		{
			StageGrid.SetCellText(5, r, CString("Flow\n ")+CString((Units) ? "bbl/day" : "m3/day") );
			StageGrid.SetColWidth(5, 51);
			StageGrid.SetCellText(6, r, "Time \nday");
			StageGrid.SetColWidth(6, 51);
			StageGrid.SetCellText(7, r, CString("Vol\n ")+CString((Units) ? "bbl" : "m3") );
			StageGrid.SetColWidth(7, 51);
			StageGrid.SetCellText(8, r, "T. Step \nday");
			StageGrid.SetColWidth(8, 53);
		}
		
		if( Stage[st].TypeStg == 5 )
		{	
			StageGrid.SetCellText(5, r, "Flow" );
			StageGrid.SetCellText(5, r+1, "" );
			StageGrid.cellEdit[5][r+1] = testgrid::READ_ONLY;
			StageGrid.cellFormat[5][r+1] = testgrid::GRAY;
			StageGrid.SetColWidth(5, 51);

			StageGrid.SetCellText(6, r, "Time \nday");
			StageGrid.SetColWidth(6, 51);
			
			StageGrid.SetCellText(7, r, "Vol" );
			StageGrid.SetCellText(7, r+1, "" );
			StageGrid.cellEdit[7][r+1] = testgrid::READ_ONLY;
			StageGrid.cellFormat[7][r+1] = testgrid::GRAY;
			StageGrid.SetColWidth(7, 51);
			
			StageGrid.SetCellText(8, r, "T.Step");
			StageGrid.SetCellText(8, r+1, "" );
			StageGrid.cellEdit[8][r+1] = testgrid::READ_ONLY;
			StageGrid.cellFormat[8][r+1] = testgrid::GRAY;
			StageGrid.SetColWidth(8, 53);
		}
	
	}
	
	if ( iModel == 4 || iModel == 5 ) // Linear Core Flood or Isotherm Model
	{
		StageGrid.SetCellText(5, r, CString("Flow\n ")+CString("ml/hour"));
		StageGrid.SetColWidth(5, 51);
		StageGrid.SetCellText(6, r, "Time \nhour");
		StageGrid.SetColWidth(6, 51);
		StageGrid.SetCellText(7, r, CString("Vol\n ")+CString("ml") );
		StageGrid.SetColWidth(7, 51);
		StageGrid.SetCellText(8, r, "T. Step \nhour");
		StageGrid.SetColWidth(8, 53);
	}
	
	if( m_Headers.GetCheck()  )
	
		StageGrid.SetRowHeight(r, 40);
	
	else
		
		if( r > 0)
			
			StageGrid.SetRowHeight(r, 1);

	StageGrid.SetColEdit(0, testgrid::READ_ONLY);
	StageGrid.SetRowEdit(r, testgrid::READ_ONLY);
	StageGrid.SetColFormat(0, testgrid::HEADER);
	StageGrid.SetRowFormat(r, testgrid::HEADER);


	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_STAGEGRID);
	pWnd->GetWindowRect(&rect);

	int c,
		nCols = 9,
		total = 0,
		width,
		tempColw[9] = { 43, 67, 45, 75, 45, 51, 51, 51, 53 };

	width = rect.Width()-14;

	for (int c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		StageGrid.SetColWidth(c, int(tempColw[c] * width / total));


}

LRESULT PropPage3::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	
	UpdateFromEdits();
//	UpdateFromListCtrl();
	return 0;
}

LRESULT PropPage3::OnEditLostFocus(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	SetModified(TRUE);
	UpdateFromEdits();
	return 0;
}

void PropPage3::OnAddStage() 
{
	// TODO: Add your control notification handler code here
	int r,
		i,
		nPos,
		num;
	
	for ( r = 0 ; r < nStg ; r++ )
		
		Stage[r] = StageGrid.StgDlg.Stage[r];

	CString sEdit;

	nStg++;
	
	StageGrid.Tab = testgrid::PP3;
	StageGrid.kTemp = kTemp;

	m_Edit_NewStage_Pos.GetWindowText(sEdit);
	
	if (sEdit.IsEmpty() == FALSE)
	{
		nPos = atoi(sEdit);

		if (nPos > nStg )
		{
			nPos = nStg;
			sEdit.Format("%d", nPos);
			m_Edit_NewStage_Pos.SetWindowText(sEdit);
		}

	}

	// copy old model
	vector<Stg> tempStage;
	tempStage.resize(nStg);

	for( i = 0 ; i < nStg ; i++ )
	{	
		if ( i > nPos )
		
			tempStage[i] = Stage[i-1];
		
		if ( i < nPos )
		
			tempStage[i] = Stage[i];
			
		if ( i == nPos )

			tempStage[i] = Stage[nPos-1];
	}
	
	Allocate();
	
	StageGrid.Tab = testgrid::PP3;

	for( i = 0 ; i < nStg ; i++ )
	{	
		Stage[i] = tempStage[i];
		
		StageGrid.StgDlg.Stage[i] = Stage[i];
	}
	
	InitialiseGrid();

	UpdateControls();
	
//	UpdateFromEdits();
	
	UpdateST();

	Invalidate(FALSE);
}

void PropPage3::InitialiseGrid()
{	
	CString sItem;

	int r,
		 numSt =0;

	StageGrid.Tab = testgrid::PP3;
	StageGrid.kTemp = kTemp;
	StageGrid.Units = Units;

	StageGrid.SetColEdit(1, testgrid::COMBO);
		
	StageGrid.comboList[0].RemoveAll();
	StageGrid.comboList[0].Add("Inj/Preflush");
	StageGrid.comboList[0].Add("Overflush");
	StageGrid.comboList[0].Add("Production");
	StageGrid.comboList[0].Add("Shut-in");
	
//	StageGrid.SetColEdit(2, testgrid::COMBO);
	
	StageGrid.comboList[1].RemoveAll();
	StageGrid.comboList[1].Add("Water");
	StageGrid.comboList[1].Add("Oil");
	
	StageGrid.SetColEdit(4, testgrid::COMBO);
	
	StageGrid.comboList[2].RemoveAll();
	StageGrid.comboList[2].Add("F/T");
	StageGrid.comboList[2].Add("T/V");
	StageGrid.comboList[2].Add("F/V");

	StageGrid.SetCellText(0, 0, "Stage");
	StageGrid.SetColWidth(0, 43);
	StageGrid.SetCellText(1, 0, "Type");
	StageGrid.SetColWidth(1, 67);

	if (nDisabled)
	{
		StageGrid.SetCellText(2, 0, "Qw/Qt");
		GetDlgItem(IDC_RADIO_LOW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_HIGH)->EnableWindow(TRUE);
	}
	else
	{
		StageGrid.SetCellText(2, 0, "TI");
		GetDlgItem(IDC_RADIO_LOW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_HIGH)->EnableWindow(FALSE);
	}

	StageGrid.SetRowHeight(0, 35);
	StageGrid.SetColEdit(0, testgrid::READ_ONLY);
	StageGrid.SetRowEdit(0, testgrid::READ_ONLY);
	StageGrid.SetColFormat(0, testgrid::HEADER);
	StageGrid.SetRowFormat(0, testgrid::HEADER);
	
	StageGrid.CalculateColRowPos();
	
	for ( r = 0; r < 2 * nStg ; r++ )
	{
		if( r%2 == 0 )
			
			SetHeaderRowStage(r, numSt);

		if( r%2 == 1 )
		{	
			StageGrid.SetColRowComboList(1, r, 0);

			StageGrid.SetColRowComboList(2, r, 1);

			StageGrid.SetColRowComboList(4, r, 2);

			numSt++;
			sItem.Format("%d", numSt);
			StageGrid.colS[0][r] = sItem;	
		}
	
	}
	
	if (!Header)
	{
		StageGrid.SetColWidth(2, 45);
		StageGrid.SetCellText(3, 0, "Inj/Prod Controls");
		StageGrid.SetColWidth(3, 75);
		StageGrid.SetCellText(4, 0, "F/T/V");
		StageGrid.SetColWidth(4, 45);
		StageGrid.SetCellText(5, 0, "Flow");
		StageGrid.SetColWidth(5, 51);
		StageGrid.SetCellText(6, 0, "Time");
		StageGrid.SetColWidth(6, 51);
		StageGrid.SetCellText(7, 0, "Vol");
		StageGrid.SetColWidth(7, 51);
		StageGrid.SetCellText(8, 0, "T. Step");
		StageGrid.SetColWidth(8, 53);
	}

	Invalidate(FALSE);
}

void PropPage3::OnRemoveStage() 
{
	// TODO: Add your control notification handler code here
	int r,
		 i,
		 nPos,
		 num;
	
	for ( r = 0 ; r < nStg ; r++ )
		
		Stage[r] = StageGrid.StgDlg.Stage[r];

	CString sEdit;

	nStg--;
	
	m_Edit_NewStage_Pos.GetWindowText(sEdit);
	
	if (sEdit.IsEmpty() == FALSE)
	{
		nPos = atoi(sEdit);
	}

	// copy old model
	vector<Stg> tempStage;
	tempStage.resize(nStg);
	
	for( i = 0 ; i < nStg ; i++ )
	
		tempStage[i] = Stage[i];

	for( i = 0 ; i < nStg ; i++ )
	{	
		if ( i >= (nPos-1) )
		
			tempStage[i] = Stage[i+1];
			
		if ( i < (nPos-1) )
		
			tempStage[i] = Stage[i];
	}
	
	if (nPos > nStg )
	{
		nPos = nStg;
		sEdit.Format("%d", nPos);
		m_Edit_NewStage_Pos.SetWindowText(sEdit);
	}

	DeAllocate();
	
	Allocate();


	for( i = 0 ; i < nStg ; i++ )
	{	
		Stage[i] = tempStage[i];
	
		StageGrid.StgDlg.Stage[i] = Stage[i];
	}
	
	InitialiseGrid();

	UpdateControls();
	
//	UpdateFromEdits();

	UpdateST();

	Invalidate(FALSE);
	
}

void PropPage3::OnCheckHeaders() 
{
	// TODO: Add your control notification handler code here

	if( m_Headers.GetCheck() )
	{
//		num = 2 * nStg;
		Header = 1;
	}	
	else
	{	
//		num = 1 + nStg;
		Header = 0;
	}	
/*	testgrid tempgrid( num, 9 );
	
	if( m_Headers.GetCheck() )
	
		tempgrid.Header = 1;
		
	else

		tempgrid.Header = 0;

	StageGrid = tempgrid;
*/	
	for ( int r = 0 ; r < nStg ; r++ )
		
		Stage[r] = StageGrid.StgDlg.Stage[r];

	StageGrid.kTemp = kTemp;
	
	
	// Make testgrid not to use header becuase otherwise gives problems
	// Check OnBosLostFocus and look for Header
	/*
	if( m_Headers.GetCheck() )
	
		StageGrid.Header = 1;
		
	else

		StageGrid.Header = 0;
	*/

	StageGrid.Header = 1;	

	InitialiseGrid();
	
	UpdateControls();

	Invalidate(FALSE);
}

void PropPage3::UpdateTimeVol()
{
	int r, 
	numSt = 0;
	char st[50];

	double TotWatProd = 0,
			 TotTimeProd = 0,
			 TotWatInj = 0,
			 TotWatSIInj = 0,
			 TotTimeInj = 0,
			 SIVolInj = 0,
			 watFrac = 1;

	for ( r = 0 ; r < 2*nStg ; r++ )
	{              
		if( r%2 == 1 )
		{              
			if ( Stage[numSt].TypeStg == 1 ) // Injection
			{              
				if (!nDisabled) // Single Phase

					watFrac = 1;

				else watFrac = atof(StageGrid.colS[2][r]);

				TotWatInj += watFrac * atof(StageGrid.colS[7][r]);
				TotWatSIInj += watFrac * atof(StageGrid.colS[7][r]);
				TotTimeInj += atof(StageGrid.colS[6][r]);
				double SIConc;
				strcpy(st, StageGrid.colS[3][r].GetBuffer(5));
				SIConc = atof(st+5);
				SIVolInj += (watFrac *atof(StageGrid.colS[7][r])*SIConc/(1e6));

				// To Caculate the Volume of SI, we used the following formula
				// [SI](ppm) = (VolSI/(VolSI+VolStg)) * 1e6
				// VolSI[SI](ppm) = VolStg*[SI](ppm)*1e6/(1-*[SI](ppm)*1e6)
			}
	      
			if ( Stage[numSt].TypeStg == 2 ) // Overflush
			{              
				 TotWatInj += watFrac * atof(StageGrid.colS[7][r]);
				 TotTimeInj += atof(StageGrid.colS[6][r]);                              
			}

			if ( Stage[numSt].TypeStg == 3 ) // Production
			{              
				TotWatProd += atof(StageGrid.colS[7][r]);
				TotTimeProd += atof(StageGrid.colS[6][r]);
			}

			numSt++;
		}
	}

/*
	char U[5], sItem[250];

	if (Units) strcpy(U,"bbls");

	else strcpy(U,"m3");

	if (!nDisabled) // Single Phase
	{
		sprintf(sItem, "Water Volume Produced %.1f %s in %.1f days", TotWatProd,U,TotTimeProd);
		GetDlgItem(IDC_STATIC_VOL_PROD)->SetWindowText(sItem);

		sprintf(sItem, "Water Volume Injected %.1f %s in %.1f hours, Scale Inhibitor Volume Injected %.1f %s", TotWatInj,U,SIVolInj, U);
		GetDlgItem(IDC_STATIC_VOL_INJ)->SetWindowText(sItem);
	}
	else // Two Phase
	{
		sprintf(sItem, "Fluid Volume Produced %.1f %s in %.1f days", TotWatProd,U,TotTimeProd);
		GetDlgItem(IDC_STATIC_VOL_PROD)->SetWindowText(sItem);
	
		sprintf(sItem,"Water Volume Injected %.1f %s in %.1f hours, Scale Inhibitor Volume Injected %.1f %s", TotWatInj,U,TotTimeInj, SIVolInj, U);
		GetDlgItem(IDC_STATIC_VOL_INJ)->SetWindowText(sItem);
	}*/

	CString sItem1, sItem2;
	
	char U[5];
	
	if (Units) strcpy(U,"bbls");
	
	else	strcpy(U,"m3");
	
	if (!nDisabled) // Single Phase
	{
		sItem1.Format( (TotWatProd) <= (1e6-1) ? "%.6g" : "%.2e", TotWatProd);
		sItem1.Replace(_T("e+000"), _T("e"));
		sItem1.Replace(_T("e+00"), _T("e"));
		sItem1.Replace(_T("e+0"), _T("e"));
		sItem2.Format("Water Volume Produced %s %s in %.2f days", sItem1,U,TotTimeProd);
		GetDlgItem(IDC_STATIC_VOL_PROD)->SetWindowText(sItem2);

		sItem1.Format( (TotWatInj) <= (1e5-1) ? "%.5g" : "%.1f", TotWatInj);
		sItem1.Replace(_T("e+000"), _T("e"));
		sItem1.Replace(_T("e+00"), _T("e"));
		sItem1.Replace(_T("e+0"), _T("e"));
		sItem2.Format("Water Volume Injected %s %s in %.2f hours, Scale Inhibitor Volume Injected %.2f %s", sItem1,U,TotTimeInj, SIVolInj, U);
		GetDlgItem(IDC_STATIC_VOL_INJ)->SetWindowText(sItem2);
	}
	else // Two Phase
	{
		sItem1.Format( (TotWatProd) <= (1e6-1) ? "%.6g" : "%.2e", TotWatProd);
		sItem1.Replace(_T("e+000"), _T("e"));
		sItem1.Replace(_T("e+00"), _T("e"));
		sItem1.Replace(_T("e+0"), _T("e"));
		sItem2.Format("Fluid Volume Produced %s %s in %.2f days", sItem1,U,TotTimeProd);
		GetDlgItem(IDC_STATIC_VOL_PROD)->SetWindowText(sItem2);

		sItem1.Format( (TotWatInj) <= (1e6-1) ? "%.6g" : "%.2e", TotWatInj);
		sItem1.Replace(_T("e+000"), _T("e"));
		sItem1.Replace(_T("e+00"), _T("e"));
		sItem1.Replace(_T("e+0"), _T("e"));
		sItem2.Format("Water Volume Injected %s %s in %.2f hours, Scale Inhibitor Volume Injected %.2f %s", sItem1,U,TotTimeInj, SIVolInj, U);
		GetDlgItem(IDC_STATIC_VOL_INJ)->SetWindowText(sItem2);
	}

}

void PropPage3::OnBnClickedButtonUpdate()
{
	UpdateTimeVol();
}

void PropPage3::OnEnChangeStageGrid()
{
	UpdateTimeVol();
}

void PropPage3::OnStnClickedStaticStagegrid()
{
	// TODO: Add your control notification handler code here
	UpdateTimeVol();

	Invalidate(FALSE);
}

BOOL PropPage3::OnKillActive()
{	
	UpdateFromEdits();

	if (CSheet::ST.size() != nStg)
		CSheet::ST.resize(nStg);

	for (int s = 0; s < nStg; s++)
	{
		CSheet::ST[s] = Stage[s];
		
	}

	return CPropertyPage::OnKillActive();
}

void PropPage3::UpdateST()
{
	CSheet::ST = Stage;
}

BOOL PropPage3::OnSetActive()
{	
	for (int s = 0; s < nStg; s++)
	{
		StageGrid.StgDlg.Stage[s].objTime = CSheet::ST[s].objTime;
		StageGrid.StgDlg.Stage[s].objIndexTime = CSheet::ST[s].objIndexTime;
		StageGrid.StgDlg.Stage[s].objSI = CSheet::ST[s].objSI;
		StageGrid.StgDlg.Stage[s].objIndexSI = CSheet::ST[s].objIndexSI;
		
		Stage[s].objTime = CSheet::ST[s].objTime;
		Stage[s].objIndexTime = CSheet::ST[s].objIndexTime;
		Stage[s].objSI = CSheet::ST[s].objSI;
		Stage[s].objIndexSI = CSheet::ST[s].objIndexSI;
	}

	return CPropertyPage::OnSetActive();
}