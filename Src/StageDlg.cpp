// StageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "StageDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StageDlg dialog


StageDlg::StageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(StageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(StageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	nComp = 1;
	nLay = 1;
	Enable = TRUE;
	nWF = 0;	
	checkPLsame = 0;
}


void StageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StageDlg)
	DDX_Control(pDX, IDC_CHECK_PRESS, m_AllPres);
	DDX_Control(pDX, IDC_CHECK_LAY_OPEN, m_AllLayOpen);
	DDX_Control(pDX, IDC_CHECK_ALL_COMP, m_AllComp);
	DDX_Radio(pDX, IDC_KH,m_Check_KH);
	DDX_Radio(pDX, IDC_RADIO_BAR,m_Check_Bar);
	DDX_Control(pDX, IDC_KH, m_KH);
	DDX_Control(pDX, IDC_PIDF, m_PIDF);
	DDX_Control(pDX, IDC_LAYPRES, m_LayPres);
	DDX_Control(pDX, IDC_RADIO_BAR, m_Bar);
	DDX_Control(pDX, IDC_RADIO_PSI, m_Psi);
	DDX_Control(pDX, IDC_STATIC_LAY_COND, LayGrid);
	DDX_Control(pDX, IDC_STATIC_COMP_COND, CompGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StageDlg, CDialog)
	//{{AFX_MSG_MAP(StageDlg)
	ON_BN_CLICKED(IDC_CHECK_LAY_OPEN, OnCheckLayOpen)
	ON_BN_CLICKED(IDC_CHECK_ALL_COMP, OnCheckAllComp)
	ON_BN_CLICKED(IDC_KH, OnKh)
	ON_BN_CLICKED(IDC_LAYPRES, OnLaypres)
	ON_BN_CLICKED(IDC_PIDF, OnPidf)
	ON_BN_CLICKED(IDC_CHECK_PRESS, OnCheckPress)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE1,OnListCtrlUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StageDlg message handlers

BOOL StageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
//	m_bPIDf.EnableWindow(Enable);
	
	// Single phase calculation
	if(!nDisabled)
	{
		GetDlgItem(IDC_LAYPRES)->EnableWindow(FALSE);	
	}
	else
		GetDlgItem(IDC_LAYPRES)->EnableWindow(TRUE);

	// Well Friction and Pressurised layers no close layers allowed
	if ( nWF || Stage[nSt].nMethod == 2 )
	{
		m_AllLayOpen.EnableWindow(FALSE);
		Stage[nSt].nOpen = 0;
	}
	
	m_AllLayOpen.SetCheck( !(Stage[nSt].nOpen) ? BST_CHECKED : BST_UNCHECKED);
	
	checkOpenLayers = m_AllLayOpen.GetCheck();
	
//	m_bPIDf.SetCheck( (Stage[nSt].nMethod) ? BST_CHECKED : BST_UNCHECKED); 
	
	TableGrid TempCompGrid(nComp+1,3);

	CompGrid = TempCompGrid;
	
	if( Stage[nSt].TypeStg != 1 )
	{	
		CompGrid.SetCellText(1, 0, "");
		CompGrid.SetColEdit(1, TableGrid::READ_ONLY);
		CompGrid.SetColFormat(1, TableGrid::GRAY);
		CompGrid.SetCellText(2, 0, "");
		CompGrid.SetColEdit(2, TableGrid::READ_ONLY);
		CompGrid.SetColFormat(2, TableGrid::GRAY);
	}

	CompGrid.SetCellText(0, 0, "Comp");
	CompGrid.SetColWidth(0, 45);
	CompGrid.SetCellText(1, 0, "Wat Conc");
	CompGrid.SetColWidth(1, 67);
	CompGrid.SetCellText(2, 0, "Oil Conc");
	CompGrid.SetColWidth(2, 45);
	
	CompGrid.SetRowHeight(0, 25);

	CompGrid.SetColEdit(0, TableGrid::READ_ONLY);
	CompGrid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	CompGrid.SetColFormat(0, TableGrid::HEADER);
	CompGrid.SetRowFormat(0, TableGrid::HEADER);
	
	CompGrid.CalculateColRowPos();

	TableGrid TempLayGrid(nLay+1,4);

	LayGrid = TempLayGrid;
	
	LayGrid.SetColEdit(1, TableGrid::COMBO);
	
	LayGrid.comboList[0].RemoveAll();
	LayGrid.comboList[0].Add("Open");
	LayGrid.comboList[0].Add("Close");
	
	LayGrid.SetCellText(0, 0, "Lay");
	LayGrid.SetColWidth(0, 45);
	LayGrid.SetCellText(1, 0, "Open Lay");
	LayGrid.SetColWidth(1, 67);
	
	LayGrid.SetCellText(2, 0, "PIDF");
	LayGrid.SetCellText(3, 0, "ProdFw");

/*	if( !m_bPIDf.GetCheck() )
	{		
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);
	}
*/	
	if( Stage[nSt].nMethod == 0 ) // KH
	{		
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);
	}

	if( Stage[nSt].TypeStg == 1 || Stage[nSt].TypeStg == 2 )
	{
		LayGrid.SetColEdit(3, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(3, TableGrid::GRAY);	
	}

	if( Stage[nSt].TypeStg == 5 )
	{
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);

		LayGrid.SetColEdit(3, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(3, TableGrid::GRAY);
	}

	LayGrid.SetColWidth(2, 45);
	
	LayGrid.SetRowHeight(0, 25);
	
	LayGrid.SetColEdit(0, TableGrid::READ_ONLY);
	LayGrid.SetRowEdit(0, TableGrid::READ_ONLY);
	
	LayGrid.SetColFormat(0, TableGrid::HEADER);
	LayGrid.SetRowFormat(0, TableGrid::HEADER);

	for ( int r = 0 ; r < nLay ; r++ )
	{	
		LayGrid.SetColRowComboList(1, r+1, 0);
	}
	
	LayGrid.CalculateColRowPos();

	LayGrid.TypeStage = Stage[nSt].TypeStg;
	
	LayGrid.checkPIDf = Stage[nSt].nOpen;

	m_AllPres.SetCheck(checkPLsame);	
	
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void StageDlg::Allocate(int s, int l, int c)//, int *R)
{
	nStg = s;
	nLay = l;
	nComp = c;

	Stage.resize(nStg);

	compRole.resize(nComp);

}

void StageDlg::DeAllocate()
{
}

void StageDlg::UpdateControls()
{	
	int c,
		 l,
		 i = 0;

	CString sItem, comp;
	
	LayGrid.SetRowFormat(0, TableGrid::HEADER);
	
	if ( nSt == 0 )
		
		GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(TRUE);

	else
		
		GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(FALSE);

	if ( Stage[nSt].nMethod == 0 ) // KH
	{	
		m_Check_KH = 0;	
		m_KH.SetCheck( BST_CHECKED );
		m_PIDF.SetCheck( BST_UNCHECKED );
		m_LayPres.SetCheck( BST_UNCHECKED );
		LayGrid.SetCellText(2, 0, "KH (Auto)");
		GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PSI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(FALSE);
		LayGrid.SetColEdit(1, TableGrid::COMBO);
	}
	
	if ( Stage[nSt].nMethod == 1 ) // PIDf
	{	
		m_Check_KH = 1;	
		m_KH.SetCheck( BST_UNCHECKED );
		m_PIDF.SetCheck( BST_CHECKED );
		m_LayPres.SetCheck( BST_UNCHECKED );
		LayGrid.SetCellText(2, 0, "PIDF");
		GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_BAR)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_PSI)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(FALSE);
		LayGrid.SetColEdit(1, TableGrid::COMBO);
	}
	
	if ( Stage[nSt].nMethod == 2 ) // Lay Press
	{	
		m_Check_KH = 2;
		
		m_KH.SetCheck( BST_UNCHECKED );
		m_PIDF.SetCheck( BST_UNCHECKED );
		m_LayPres.SetCheck( BST_CHECKED );
		LayGrid.SetCellText(2, 0, "LayPres");
		GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(TRUE);
		
		if(nSt == 0)

			GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(TRUE);

		else
			
			GetDlgItem(IDC_CHECK_PRESS)->EnableWindow(FALSE);

		for( l = 0 ; l < nLay ; l++ )
		{
			LayGrid.SetCellText(1, l+1, "Open");
		}
		
		LayGrid.SetColEdit(1, TableGrid::READ_ONLY);
//		GetDlgItem(IDC_RADIO_BAR)->EnableWindow(TRUE);
//		GetDlgItem(IDC_RADIO_PSI)->EnableWindow(TRUE);
	}

	if ( Stage[nSt].BarPsi == 0 )
	{	
		m_Check_Bar = 0;
		m_Bar.SetCheck( BST_CHECKED );
		m_Psi.SetCheck( BST_UNCHECKED );
	}
	
	if ( Stage[nSt].BarPsi == 1 )
	{	
		m_Check_Bar = 1;
		m_Bar.SetCheck( BST_UNCHECKED );
		m_Psi.SetCheck( BST_CHECKED );
	}

	for( l = 0 ; l < nLay ; l++ )
	{	
		sItem.Format(_T("%d"), l+1 );
		LayGrid.colS[0][l+1] = sItem;
		
		if( !m_AllLayOpen.GetCheck() )
		{
			if( (l+1) == Stage[nSt].nLayOp[i] )
			{	
				LayGrid.colS[1][l+1] = "Open";
				
				i++;
			}
			else
			
				LayGrid.colS[1][l+1] = "Close";
		}
		
		else
			
			LayGrid.colS[1][l+1] = "Open";

		if ( Stage[nSt].nMethod == 0 )
		{	
			sItem = "";
		}
		//if ( m_bPIDf.GetCheck() )
		if ( Stage[nSt].nMethod == 1 ) // PIDf
		{	
			if ( LayGrid.colS[1][l+1] == "Open" )
			{
				LayGrid.cellFormat[2][l+1] = TableGrid::NORMAL;
				LayGrid.cellEdit[2][l+1] = TableGrid::EDIT;
				sItem.Format("%.4g", Stage[nSt].PIDF[l] );
			}
			else
			{
				sItem = "";
				LayGrid.cellFormat[2][l+1] = TableGrid::GRAY;
				LayGrid.cellEdit[2][l+1] = TableGrid::READ_ONLY;
			}
			
		}
		
		if ( Stage[nSt].nMethod == 2 ) // Lay Press
		{	
			if ( LayGrid.colS[1][l+1] == "Open" )
			{
				LayGrid.cellFormat[2][l+1] = TableGrid::NORMAL;
				LayGrid.cellEdit[2][l+1] = TableGrid::EDIT;
				sItem.Format("%.4g", Stage[nSt].LayPres[l] );
			}
			else
			{
				sItem = "";
				LayGrid.cellFormat[2][l+1] = TableGrid::GRAY;
				LayGrid.cellEdit[2][l+1] = TableGrid::READ_ONLY;
			}
			
		}
		
		LayGrid.colS[2][l+1] = sItem;
		
		if( Stage[nSt].TypeStg == 1 || Stage[nSt].TypeStg == 2 || Stage[nSt].TypeStg == 5 )
		{
			LayGrid.colS[3][l+1] = "";
		}
		else
		{
			if ( LayGrid.colS[1][l+1] == "Open" )
			{
				LayGrid.cellFormat[3][l+1] = TableGrid::NORMAL;
				LayGrid.cellEdit[3][l+1] = TableGrid::EDIT;
				sItem.Format("%.4g", Stage[nSt].ProdFw[l] );
			}
			else
			{
				sItem = "";
				LayGrid.cellFormat[3][l+1] = TableGrid::GRAY;
				LayGrid.cellEdit[3][l+1] = TableGrid::READ_ONLY;
			}
		}
		
		if( Stage[nSt].TypeStg == 3 )
		
			LayGrid.colS[3][l+1] = sItem;

		if( Stage[nSt].TypeStg == 5 ) // Shut-in Stage
		{	
			if ( Stage[nSt].nMethod != 2 ) // Lay Press
			{	
				LayGrid.colS[2][l+1] = "";
				LayGrid.cellFormat[2][l+1] = TableGrid::GRAY;
				LayGrid.cellEdit[2][l+1] = TableGrid::READ_ONLY;

				LayGrid.colS[3][l+1] = "";
				LayGrid.cellFormat[3][l+1] = TableGrid::GRAY;
				LayGrid.cellEdit[3][l+1] = TableGrid::READ_ONLY;
			}
		}

	}
	
	for( c = 0 ; c < nComp ; c++ )
	{	
		if ( compRole[c] == 0 ) comp = "Tracer";
		if ( compRole[c] == 1 ) comp = "Inhib.";
		if ( compRole[c] == 2 ) comp = "Visco.";
		if ( compRole[c] == 3 ) comp = "Blocker";
		if ( compRole[c] == 4 ) comp = "Surfact";
		if ( compRole[c] == 7 ) comp = "Bridging";
		if ( compRole[c] == 8 ) comp = "Wax";
		if ( compRole[c] == 9 ) comp = "SI_FD";

		sItem.Format("(%d) %s", c+1, comp);
		CompGrid.colS[0][c+1] = sItem;	
		
		if( Stage[nSt].TypeStg == 1 )
		{
			sItem.Format("%7.1f", Stage[nSt].InitialCw[c] );
			CompGrid.colS[1][c+1] = sItem;

			sItem.Format("%7.1f", Stage[nSt].InitialCo[c] );
			CompGrid.colS[2][c+1] = sItem;
		}
		else
		{
			CompGrid.colS[1][c+1] = " ";
			CompGrid.colS[2][c+1] = " ";
		}

	}
	
	if (!nDisabled)
	{	
		LayGrid.SetColEdit(3, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(3, TableGrid::GRAY);
		LayGrid.SetRowFormat(0, TableGrid::HEADER);

		CompGrid.SetColEdit(2, TableGrid::READ_ONLY);
		CompGrid.SetColFormat(2, TableGrid::GRAY);
		CompGrid.SetRowFormat(0, TableGrid::HEADER);
	}
	
	checkOpenLayers = m_AllLayOpen.GetCheck();
	
	Invalidate(FALSE);
}	

void StageDlg::UpdateFromEdits()
{	
	int i=0,l,c;		
		
//	if( !m_AllLayOpen.GetCheck() )
	for( l = 0 ; l < nLay ; l++ )
	{
		if( LayGrid.colS[1][l+1] == "Close" )
			
			checkOpenLayers = 0;
	}

	if( !checkOpenLayers )
	{
		for( l = 0 ; l < nLay ; l++ )
		{	
			Stage[nSt].nLayOp[i] = nLay+1;

			if( LayGrid.colS[1][l+1] == "Open"  )
			{	
				Stage[nSt].nLayOp[i] = l+1;
				
				i++;
			}
		}
		
		Stage[nSt].nOpen = i;
		
	}

	else
		
		Stage[nSt].nOpen = 0;

	Stage[nSt].nMethod = m_Check_KH;
		
	Stage[nSt].BarPsi = m_Check_Bar;
		
/*	if( Stage[nSt].nMethod == 1 ) // PIDf
	{		
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);
	}

	if( Stage[nSt].nMethod == 2 ) // Lay Pres
	{		
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);
	}
*/		
	if( Stage[nSt].nMethod == 1 ) // PIDf
	{	
		Stage[nSt].nMethod = 1;
		
		for( l = 0 ; l < nLay ; l++ )
			
			Stage[nSt].PIDF[l] = atof(LayGrid.colS[2][l+1]);		
	}
	
	if( Stage[nSt].nMethod == 2 ) // Lay Pres
	{	
		for( l = 0 ; l < nLay ; l++ )
			
			Stage[nSt].LayPres[l] = atof(LayGrid.colS[2][l+1]);
	}


	if ( Stage[nSt].TypeStg == 3 )
		
		for( l = 0 ; l < nLay ; l++ )
			
			Stage[nSt].ProdFw[l] = atof(LayGrid.colS[3][l+1]);
		
		
	for( c = 0 ; c < nComp ; c++ )
	{	
		Stage[nSt].InitialCw[c] = atof(CompGrid.colS[1][c+1]);

		Stage[nSt].InitialCo[c] = atof(CompGrid.colS[2][c+1]);

		if( Stage[nSt].InitialCw[c] > 1e6 )
		{
			AfxMessageBox("Water Concentration out of range, 0<C<1e6");
			
			Stage[nSt].InitialCw[c] = 1000000;
		}
		
		if( Stage[nSt].InitialCo[c] > 1e6 )
		{
			AfxMessageBox("Oil Concentration out of range, 0<C<1e6");
			
			Stage[nSt].InitialCo[c] = 1e6;
		}
	}
}
/*
void StageDlg::OnCheckPIDf() 
{
	// TODO: Add your control notification handler code here
	int check = m_bPIDf.GetCheck();
	
	if( check )
	{	
		LayGrid.SetColEdit(2, TableGrid::EDIT);
		LayGrid.SetColFormat(2, TableGrid::NORMAL);
		LayGrid.SetRowFormat(0, TableGrid::HEADER);
		LayGrid.checkPIDf = 1;

		Stage[nSt].nChage = 1;
		Stage[nSt].nMethod = 1;
	}
	else
	{	
		LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
		LayGrid.SetColFormat(2, TableGrid::GRAY);
		LayGrid.SetRowFormat(0, TableGrid::HEADER);
		LayGrid.checkPIDf = 0;

		Stage[nSt].nMethod = 0;
	}	
	
	UpdateControls();
	
	LayGrid.vScrollPos = 0;
	LayGrid.vScroller.SetScrollPos(0);

	Invalidate(FALSE);
}
*/

void StageDlg::OnCheckLayOpen() 
{
	// TODO: Add your control notification handler code here
	int l,
		 
	checkOpenLayers = m_AllLayOpen.GetCheck();

	if( checkOpenLayers )
	{	
		for( l = 0 ; l < nLay ; l++ )
		{
			LayGrid.SetCellText(1, l+1, "Open");
			LayGrid.SetCellText(2, l+1, "1");
			
			if( Stage[nSt].TypeStg == 3 )
				
				LayGrid.SetCellText(3, l+1, "1");
			
		}
		
		LayGrid.SetColEdit(1, TableGrid::READ_ONLY);

		LayGrid.SetColFormat(1, TableGrid::NORMAL);
		
		if( Stage[nSt].TypeStg == 3 )
		{
			LayGrid.SetColEdit(3, TableGrid::EDIT);
			LayGrid.SetColFormat(3, TableGrid::NORMAL);
		}

		Stage[nSt].nOpen = 0;

	}
	
	else
	{	
		Stage[nSt].nOpen = nLay;

		LayGrid.SetColEdit(1, TableGrid::COMBO);
	}
	
	Stage[nSt].nChage = 1;

	UpdateControls();
	
	LayGrid.vScrollPos = 0;
	LayGrid.vScroller.SetScrollPos(0);

	Invalidate(FALSE);
}

LRESULT StageDlg::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");

	if ( LayGrid.colS[LayGrid.cellC][LayGrid.cellR] == "Close" )
	{	
		m_AllLayOpen.SetCheck(BST_UNCHECKED);
		Invalidate(FALSE);
	}
	
	UpdateFromEdits();
//	UpdateFromListCtrl();
	return 0;
}

void StageDlg::OnCheckAllComp() 
{
	// TODO: Add your control notification handler code here
	

	if( m_AllLayOpen.GetCheck() )
	{
		for( int c = 0 ; c < nComp ;  c++ )
		{	
			Stage[nSt].InitialCw[c] = Stage[nSt].InitialCw[0];
			
			Stage[nSt].InitialCo[c] = Stage[nSt].InitialCo[0];
		}

		UpdateControls();

		Invalidate(FALSE);
	}
}

void StageDlg::OnKh() 
{
	LayGrid.SetColEdit(2, TableGrid::READ_ONLY);
	LayGrid.SetColFormat(2, TableGrid::GRAY);
	LayGrid.SetRowFormat(0, TableGrid::HEADER);
	LayGrid.SetColEdit(1, TableGrid::COMBO);

	Stage[nSt].nMethod = 0;
	
	UpdateControls();
	
	LayGrid.vScrollPos = 0;
	LayGrid.vScroller.SetScrollPos(0);
	
	GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_BAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PSI)->EnableWindow(FALSE);
	
	m_AllLayOpen.EnableWindow(TRUE);

	Invalidate(FALSE);
	
}

void StageDlg::OnLaypres() 
{	
	m_AllLayOpen.SetCheck(TRUE);

	LayGrid.SetCellText(2, 0, "LayPres");

	LayGrid.SetColEdit(2, TableGrid::EDIT);
	LayGrid.SetColFormat(2, TableGrid::NORMAL);
	LayGrid.SetRowFormat(0, TableGrid::HEADER);
	
	for( int l = 0 ; l < nLay ; l++ )
	{
		LayGrid.SetCellText(1, l+1, "Open");
	}
		
	LayGrid.SetColEdit(1, TableGrid::READ_ONLY);

	Stage[nSt].nChage = 1;
	Stage[nSt].nMethod = 2;
	
	UpdateControls();
	
	LayGrid.vScrollPos = 0;
	LayGrid.vScroller.SetScrollPos(0);
	
	GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_BAR)->EnableWindow(TRUE);
//	GetDlgItem(IDC_RADIO_PSI)->EnableWindow(TRUE);
	
	m_AllLayOpen.SetCheck( BST_CHECKED );
	m_AllLayOpen.EnableWindow(FALSE);

	Invalidate(FALSE);
	
}

void StageDlg::OnPidf() 
{	
	LayGrid.SetCellText(2, 0, "PIDf");

	LayGrid.SetColEdit(2, TableGrid::EDIT);
	LayGrid.SetColFormat(2, TableGrid::NORMAL);
	LayGrid.SetRowFormat(0, TableGrid::HEADER);
	LayGrid.SetColEdit(1, TableGrid::COMBO);

	LayGrid.checkPIDf = 1;

	Stage[nSt].nChage = 1;
	Stage[nSt].nMethod = 1;
	
	UpdateControls();
	
	LayGrid.vScrollPos = 0;
	LayGrid.vScroller.SetScrollPos(0);
	
	GetDlgItem(IDC_STATIC_UNITS)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_BAR)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_PSI)->EnableWindow(FALSE);

	m_AllLayOpen.EnableWindow(TRUE);

	Invalidate(FALSE);
	
}

void StageDlg::OnCheckPress() 
{	
	checkPLsame = m_AllPres.GetCheck();	
}
