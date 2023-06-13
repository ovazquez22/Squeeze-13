// GraphParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "GraphParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GraphParamDlg dialog


GraphParamDlg::GraphParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GraphParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(GraphParamDlg)
	m_OptGraf1 = -1;
	//}}AFX_DATA_INIT
}


void GraphParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GraphParamDlg)
	DDX_Control(pDX, IDC_CHECK_EXTRA_OUTPUT, m_check_extra);
	DDX_Control(pDX, IDC_RADIO_EXTRAOUT, m_extra_out);
	DDX_Control(pDX, IDC_RADIO_EXTRAOUT2, m_extra_out2);
	DDX_Control(pDX, IDC_RADIO_INH, m_Button_Rad_Inh);
	DDX_Control(pDX, IDC_RADIO_ADSORP, m_Button_Rad_Ads);
	DDX_Control(pDX, IDC_RADIO_TEMPERATURE, m_Button_Rad_Temp);
	DDX_Control(pDX, IDC_EDIT_MINCONC, m_Edit_MinConc);
	DDX_Control(pDX, IDC_EDIT_MAXCONC, m_Edit_MaxConc);
	DDX_Control(pDX, IDC_CHECK_MINMAXCON, m_Check_MinMaxConc);
	DDX_Radio(pDX, IDC_RADIO_INH, m_OptGraf1);
	DDX_Control(pDX, IDC_CHECK_GRAPH1, m_Check_Graph1);
	DDX_Control(pDX, IDC_CHECK_GRAPH2, m_Check_Graph2);
	DDX_Control(pDX, IDC_CHECK_GRAPH3, m_Check_Graph3);
	DDX_Control(pDX, IDC_STATIC_GRAPH2, Graph2Grid);
	DDX_Control(pDX, IDC_STATIC_GRAPH3, Graph3Grid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GraphParamDlg, CDialog)
	//{{AFX_MSG_MAP(GraphParamDlg)
	ON_BN_CLICKED(IDC_CHECK_GRAPH1, OnCheckGraph1)
	ON_BN_CLICKED(IDC_CHECK_MINMAXCON, OnCheckMinmaxcon)
	ON_BN_CLICKED(IDC_CHECK_GRAPH2, OnCheckGraph2)
	ON_BN_CLICKED(IDC_CHECK_GRAPH3, OnCheckGraph3)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_EXTRA_OUTPUT, &GraphParamDlg::OnCheckExtraOutput)
	ON_BN_CLICKED(IDC_RADIO_EXTRAOUT, &GraphParamDlg::OnRadioExtraout)
	ON_BN_CLICKED(IDC_RADIO_EXTRAOUT2, &GraphParamDlg::OnRadioExtraout2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GraphParamDlg message handlers

BOOL GraphParamDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString sItem;

	int l;
	
	TableGrid TempGrid(nLay+1,2);

	Graph2Grid = TempGrid;

	Graph2Grid.SetColEdit(1, TableGrid::COMBO);
	
	Graph2Grid.comboList[0].RemoveAll();
	Graph2Grid.comboList[0].Add("Disp");
	Graph2Grid.comboList[0].Add("NoDisp");
	
	Graph2Grid.SetCellText(0, 0, "Lay");
	Graph2Grid.SetColWidth(0, 1);
	Graph2Grid.SetCellText(1, 0, "Display Lay");
	Graph2Grid.SetColWidth(1, 1);
	
	Graph2Grid.SetColEdit(0, TableGrid::READ_ONLY);
	Graph2Grid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	Graph2Grid.SetColFormat(0, TableGrid::HEADER);
	Graph2Grid.SetRowFormat(0, TableGrid::HEADER);
	
	for( l = 0 ; l < nLay ; l++ )
	{	
		sItem.Format("%d", l+1 );
		Graph2Grid.colS[0][l+1] = sItem;
		
		Graph2Grid.colS[1][l+1] = "Disp";

		Graph2Grid.SetColRowComboList(1, l+1, 0);
	}
	
	Graph2Grid.CalculateColRowPos();
	
	Graph3Grid = TempGrid;

	Graph3Grid.SetColEdit(1, TableGrid::COMBO);
	
	Graph3Grid.comboList[0].RemoveAll();
	Graph3Grid.comboList[0].Add("Disp");
	Graph3Grid.comboList[0].Add("NoDisp");
	
	Graph3Grid.SetCellText(0, 0, "Lay");
	Graph3Grid.SetColWidth(0, 1);
	Graph3Grid.SetCellText(1, 0, "Display Lay");
	Graph3Grid.SetColWidth(1, 1);
	
	Graph3Grid.SetColEdit(0, TableGrid::READ_ONLY);
	Graph3Grid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	Graph3Grid.SetColFormat(0, TableGrid::HEADER);
	Graph3Grid.SetRowFormat(0, TableGrid::HEADER);
	
	for( l = 0 ; l < nLay ; l++ )
	{	
		sItem.Format("%d", l+1 );
		Graph3Grid.colS[0][l+1] = sItem;
		
		Graph3Grid.colS[1][l+1] = "Disp";
		
		Graph3Grid.SetColRowComboList(1, l+1, 0);
	}
	
	Graph3Grid.CalculateColRowPos();

	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GraphParamDlg::UpdateControls()
{	
	int l;

	CString sn;
	
	sn.Format("%.4g", MinConc);
	m_Edit_MinConc.SetWindowText(sn);
	
	sn.Format("%.4g", MaxConc);
	m_Edit_MaxConc.SetWindowText(sn);
	
	if ( kCrange )
	{	 
		m_Check_MinMaxConc.SetCheck(1);
		m_Edit_MaxConc.EnableWindow(TRUE);
		m_Edit_MinConc.EnableWindow(TRUE);	
	}
	else
	{
		m_Check_MinMaxConc.SetCheck(0);
		m_Edit_MaxConc.EnableWindow(FALSE);
		m_Edit_MinConc.EnableWindow(FALSE);
	}
	
	if ( kGraf1 )
	{	 
		m_Check_Graph1.SetCheck(1);

		m_Button_Rad_Inh.EnableWindow(TRUE);
		m_Button_Rad_Ads.EnableWindow(TRUE);
		m_Button_Rad_Temp.EnableWindow(TRUE);

/*		if( kGraf1 == 1 )
		{	
			m_OptGraf1 = 0;
//			m_Button_Rad_Inh.SetCheck(1);
		}
/*		else
		{
			m_Button_Rad_Ads.SetCheck(0);
			m_Button_Rad_Temp.SetCheck(0);
		}
*/
/*		if( kGraf1 == 2 )
		{ 
			m_OptGraf1 = 1;
//			m_Button_Rad_Ads.SetCheck(1);
		}
/*		else
		{
			m_Button_Rad_Inh.SetCheck(0);
			m_Button_Rad_Temp.SetCheck(0);
		}
*/
/*		if( kGraf1 == 3 )
		{ 
			m_OptGraf1 = 2;
//			m_Button_Rad_Temp.SetCheck(1);
		}
/*		else
		{
			m_Button_Rad_Inh.SetCheck(0);
			m_Button_Rad_Ads.SetCheck(0);
		}
*/
	}
	else
	{
		m_Check_Graph1.SetCheck(0);

		m_Button_Rad_Inh.EnableWindow(FALSE);
		m_Button_Rad_Ads.EnableWindow(FALSE);
		m_Button_Rad_Temp.EnableWindow(FALSE);
	}
	
	if ( kGraf2 )
	{	 
		m_Check_Graph2.SetCheck(1);


		for( l = 0 ; l < nLay ; l++ )
		{	
			if ( nGraf2[l] == l+1 )
			
				Graph2Grid.colS[1][l+1] = "Disp";
			
			else

				Graph2Grid.colS[1][l+1] = "NoDisp";
		}
	}
	else
	{
		m_Check_Graph2.SetCheck(0);
		
		Graph2Grid.Disabled();
	}

	if ( kGraf3 )
	{	 
		m_Check_Graph3.SetCheck(1);

		for( l = 0 ; l < nLay ; l++ )
		{	
			if ( nGraf3[l] == l+1 )
			
				Graph3Grid.colS[1][l+1] = "Disp";

			else

				Graph3Grid.colS[1][l+1] = "NoDisp";
		}

	}
	else
	{
		m_Check_Graph3.SetCheck(0);

		Graph3Grid.Disabled();
	}

	if (nDebug == 2 || nDebug == 3)
		
		m_check_extra.SetCheck(BST_CHECKED);

	else

		m_check_extra.SetCheck(BST_UNCHECKED);
	
	if (nDebug == 2 )
	{	
		m_extra_out.SetCheck(BST_CHECKED);
		m_extra_out2.SetCheck(BST_UNCHECKED);
	}
	
	if (nDebug == 3 )
	{	
		m_extra_out.SetCheck(BST_UNCHECKED);
		m_extra_out2.SetCheck(BST_CHECKED);
	}

	Invalidate(NULL);

}

void GraphParamDlg::OnCheckGraph1() 
{
	// TODO: Add your control notification handler code here
	if( m_Check_Graph1.GetCheck() )
	{
		m_Button_Rad_Inh.EnableWindow(TRUE);
		m_Button_Rad_Ads.EnableWindow(TRUE);
		m_Button_Rad_Temp.EnableWindow(TRUE);

		m_OptGraf1 = 0;
	}
	else
	{
		m_Button_Rad_Inh.EnableWindow(FALSE);
		m_Button_Rad_Ads.EnableWindow(FALSE);
		m_Button_Rad_Temp.EnableWindow(FALSE);
	}
}

void GraphParamDlg::OnCheckMinmaxcon() 
{
	// TODO: Add your control notification handler code here
	if( m_Check_MinMaxConc.GetCheck() )
	{
		m_Edit_MaxConc.EnableWindow(TRUE);
		m_Edit_MinConc.EnableWindow(TRUE);
	}
	else
	{
		m_Edit_MaxConc.EnableWindow(FALSE);
		m_Edit_MinConc.EnableWindow(FALSE);
	}
}

void GraphParamDlg::OnCheckGraph2() 
{
	// TODO: Add your control notification handler code here
	if( m_Check_Graph2.GetCheck() )
	{
		EnableGraph2();
	}
	else
	{
		Graph2Grid.Disabled();
	}
}

void GraphParamDlg::EnableGraph2()
{
	CString sItem;

	int l;
	
	TableGrid TempGrid(nLay+1,2);

	Graph2Grid = TempGrid;

	Graph2Grid.SetColEdit(1, TableGrid::COMBO);
	
	Graph2Grid.comboList[0].RemoveAll();
	Graph2Grid.comboList[0].Add("Disp");
	Graph2Grid.comboList[0].Add("NoDisp");
	
	Graph2Grid.SetCellText(0, 0, "Lay");
	Graph2Grid.SetColWidth(0, 1);
	Graph2Grid.SetCellText(1, 0, "Display Lay");
	Graph2Grid.SetColWidth(1, 1);
	
	Graph2Grid.SetColEdit(0, TableGrid::READ_ONLY);
	Graph2Grid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	Graph2Grid.SetColFormat(0, TableGrid::HEADER);
	Graph2Grid.SetRowFormat(0, TableGrid::HEADER);
	
	for( l = 0 ; l < nLay ; l++ )
	{	
		sItem.Format("%d", l+1 );
		Graph2Grid.colS[0][l+1] = sItem;
		
		Graph2Grid.colS[1][l+1] = "Disp";

		Graph2Grid.SetColRowComboList(1, l+1, 0);
	}
	
	Graph2Grid.CalculateColRowPos();

	Invalidate(NULL);
}

void GraphParamDlg::EnableGraph3()
{	
	CString sItem;

	int l;

	TableGrid TempGrid(nLay+1,2);

	Graph3Grid = TempGrid;

	Graph3Grid.SetColEdit(1, TableGrid::COMBO);
	
	Graph3Grid.comboList[0].RemoveAll();
	Graph3Grid.comboList[0].Add("Disp");
	Graph3Grid.comboList[0].Add("NoDisp");
	
	Graph3Grid.SetCellText(0, 0, "Lay");
	Graph3Grid.SetColWidth(0, 1);
	Graph3Grid.SetCellText(1, 0, "Display Lay");
	Graph3Grid.SetColWidth(1, 1);
	
	Graph3Grid.SetColEdit(0, TableGrid::READ_ONLY);
	Graph3Grid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	Graph3Grid.SetColFormat(0, TableGrid::HEADER);
	Graph3Grid.SetRowFormat(0, TableGrid::HEADER);
	
	for( l = 0 ; l < nLay ; l++ )
	{	
		sItem.Format("%d", l+1 );
		Graph3Grid.colS[0][l+1] = sItem;
		
		Graph3Grid.colS[1][l+1] = "Disp";

		Graph3Grid.SetColRowComboList(1, l+1, 0);
	}
	
	Graph3Grid.CalculateColRowPos();
	
	Invalidate(NULL);
}

void GraphParamDlg::OnCheckGraph3() 
{
	// TODO: Add your control notification handler code here
	if( m_Check_Graph3.GetCheck() )
	{
		EnableGraph3();
	}
	else
	{
		Graph3Grid.Disabled();
	}
}

void GraphParamDlg::UpdateFromEdits()
{	
	int l;
	
	CString sEdit;

	kGraf2 = kGraf3 = 0;
	
	if ( m_Check_MinMaxConc.GetCheck() )
	{	
		kCrange = 1;
		
		m_Edit_MinConc.GetWindowText(sEdit);
		MinConc = atof(sEdit);
		
		m_Edit_MaxConc.GetWindowText(sEdit);
		MaxConc = atof(sEdit);
	}
	else
	{
		kCrange = 0;
	}
	
	if ( m_Check_Graph1.GetCheck() )
	{	 
/*		if( m_Button_Rad_Inh.GetCheck() == 1 ) kGraf1 = 1;

		if( m_Button_Rad_Inh.GetCheck() == 1 ) kGraf1 = 2;

		if( m_Button_Rad_Inh.GetCheck() == 1 ) kGraf1 = 3;
*/		
		if( m_OptGraf1 == 0 ) kGraf1 = 1;

		if( m_OptGraf1 == 1 ) kGraf1 = 2;

		if( m_OptGraf1 == 2) kGraf1 = 3;
	
	}
	else
	{
		kGraf1 = 0;
	}
	
	if ( m_Check_Graph2.GetCheck() )
	{	
		for( l = 0 ; l < nLay ; l++ )
		{	
			if( Graph2Grid.colS[1][l+1] == "Disp" )
			{
				nGraf2[l] = l+1;
				
				++kGraf2;
			}
			
			else nGraf2[l] = 0;
		}
	}
	
	if ( m_Check_Graph3.GetCheck() )
	{
		for( l = 0 ; l < nLay ; l++ )
		{	
			if( Graph3Grid.colS[1][l+1] == "Disp" )
			{
				nGraf3[l] = l+1;
				
				++kGraf3;
			}

			else nGraf3[l] = 0;

		}
	}
	
	if (!m_check_extra.GetCheck())
		
		nDebug = -1;
}

void GraphParamDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateFromEdits();
//	UpdateControls();
	
	CDialog::OnOK();
}

void GraphParamDlg::OnRadioExtraout2() 
{
	m_extra_out.SetCheck(FALSE);
	m_extra_out2.SetCheck(TRUE);
	nDebug = 3;	
}

void GraphParamDlg::OnCheckExtraOutput() 
{
	if( m_check_extra.GetCheck() )
	{
		GetDlgItem(IDC_RADIO_EXTRAOUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_EXTRAOUT2)->EnableWindow(TRUE);
		m_extra_out.SetCheck(TRUE);
		m_extra_out2.SetCheck(FALSE);
		nDebug = 2;
	}
	else
	{
		GetDlgItem(IDC_RADIO_EXTRAOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_EXTRAOUT2)->EnableWindow(FALSE);
		m_extra_out.SetCheck(FALSE);
		m_extra_out2.SetCheck(FALSE);
		nDebug = -1;
	}	
}

void GraphParamDlg::OnRadioExtraout() 
{	
	m_extra_out.SetCheck(TRUE);
	m_extra_out2.SetCheck(FALSE);
	nDebug = 2;
}
