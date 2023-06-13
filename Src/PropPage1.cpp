// PropPage1.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropPage1 property page

IMPLEMENT_DYNCREATE(PropPage1, CPropertyPage)

PropPage1::PropPage1() : CPropertyPage(PropPage1::IDD)
{
	//{{AFX_DATA_INIT(PropPage1)	
	//}}AFX_DATA_INIT
	nLay = 1;
	Allocate();
	SetModified(TRUE);
}

PropPage1::PropPage1(int n) : CPropertyPage(PropPage1::IDD)
{
	//{{AFX_DATA_INIT(PropPage1)	
	//}}AFX_DATA_INIT
	nLay = n;
	Allocate();
}

PropPage1::~PropPage1()
{
	DeAllocate();
}

void PropPage1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropPage1)
	DDX_Control(pDX, IDC_STATIC_ROCK_PROP, RockGrid);
	DDX_Control(pDX, IDC_STATIC_FLUID_PROP, FluidGrid);
	DDX_Control(pDX, IDC_STATIC_WELL_RAD, m_Well_Rad);
	DDX_Control(pDX, IDC_STATIC_MAX_RAD, m_Max_Rad);
	DDX_Control(pDX, IDC_STATIC_MU_WAT, m_Mu_Wat);
	DDX_Control(pDX, IDC_STATIC_MU_OIL, m_Mu_Oil);
	DDX_Control(pDX, IDC_CHECK_ALLLAYERS, m_All_Layers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropPage1, CPropertyPage)
	//{{AFX_MSG_MAP(PropPage1)
	ON_WM_CANCELMODE()
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE, OnListCtrlUpdate)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_ALLLAYERS, OnAllLayers)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropPage1 message handlers

BOOL PropPage1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	int i,
		 r,
		 nTotal = nLay * 2;
	
	CString sItem,
			  sItem1 = "",
			  sItem2 = "";

	if( iModel == 3 ) // Radial 
	{	
		sItem1 = "Well Radius" + CString((Units) ? ", ft" : ", m");
		sItem2 = "Maximum Radius of Formation" + CString((Units) ? ", ft" : ", m");
	}
	
	if( iModel == 4 ) // Linear
	{
		sItem1 = "";
		sItem2 = "Core Length, cm";
		m_Well_Rad.ShowWindow(SW_HIDE);
	}	
	
	if( iModel == 5 ) // Isotherm
	{
		sItem1 = "";
		sItem2 = "Core Length, cm";
		m_Well_Rad.ShowWindow(SW_HIDE);
	}	

	GetDlgItem(IDC_STATIC_WELLRAD)->SetWindowText(sItem1);
	GetDlgItem(IDC_STATIC_WELLRAD)->ShowWindow( (iModel==4 || iModel==5) ? SW_HIDE : SW_SHOW);
	GetDlgItem(IDC_STATIC_FORMATION)->SetWindowText(sItem2);
	
	sItem1 = "Water Viscosity, cP";
	GetDlgItem(IDC_STATIC_WAT_VISC)->SetWindowText(sItem1);

	sItem1 = "Oil Viscosity, cP";
	GetDlgItem(IDC_STATIC_OIL_VISC)->SetWindowText(sItem1);

	testgrid tempgrid(nLay+1, 4);
	
	RockGrid = tempgrid;
	
	RockGrid.vScrollPos = 0;

	RockGrid.SetCellText(0, 0, "Layer");
	RockGrid.SetColWidth(0, 50);
	RockGrid.SetCellText(1, 0, "Porosity");
	RockGrid.SetColWidth(1, 123);
	RockGrid.SetCellText(2, 0, CString("Absolute Permeability, mD"));
	RockGrid.SetColWidth(2, 176);
	RockGrid.SetCellText(3, 0, CString("Height, ")+CString((Units) ? "ft" : "m"));
	
	if( iModel == 4 )
	
		RockGrid.SetCellText(3, 0, CString("Dcore, ") + CString("cm") );

	if( iModel == 5 )
	{	
		RockGrid.SetCellText(3, 0, CString("Dcore, ") + CString("cm") );
		RockGrid.SetColEdit(2, testgrid::READ_ONLY);
		RockGrid.SetColFormat(2, testgrid::GRAY);
	}

	RockGrid.SetColWidth(3, 124);
	RockGrid.SetRowHeight(0, 25);
	RockGrid.SetColEdit(0, testgrid::READ_ONLY);
	RockGrid.SetRowEdit(0, testgrid::READ_ONLY);
	RockGrid.SetColFormat(0, testgrid::HEADER);
	RockGrid.SetRowFormat(0, testgrid::HEADER);
	
	RockGrid.CalculateColRowPos();
	
	for ( i = 0; i < nLay; i++)
	{
		sItem1.Format("%d", i+1);
		RockGrid.colS[0][i+1] = sItem1;
	}
	
	testgrid tempgrid2(nTotal, 9);
	
	FluidGrid = tempgrid2;
	
	FluidGrid.vScrollPos = 0;

	FluidGrid.SetColEdit(1, testgrid::COMBO);
	
	FluidGrid.comboList[0].RemoveAll();
	
	FluidGrid.comboList[0].Add("Corey");
	FluidGrid.comboList[0].Add("Table");

	FluidGrid.SetColWidth(0, 35);
	FluidGrid.SetColWidth(1, 45);
	FluidGrid.SetColWidth(2, 55);
	FluidGrid.SetColWidth(3, 55);
	FluidGrid.SetColWidth(4, 55);
	FluidGrid.SetColWidth(5, 55);
	FluidGrid.SetColWidth(6, 55);
	FluidGrid.SetColWidth(7, 55);
	FluidGrid.SetColWidth(8, 60);
	
	i = 1;

	for ( r = 0; r < nTotal; r++)
	{
		if( r%2 == 0 )
			
			SetHeaderRowFluidGrid(r);

		if( r%2 == 1 )
		{	
			FluidGrid.SetColRowComboList(1, r, 0);
			
			sItem.Format("%d", i++);
			FluidGrid.colS[0][r] = sItem;
			
		}
	
	}
	
	
	FluidGrid.RelPermTable.SetPathName(PathName);
	FluidGrid.RelPermTable.NameFile = "RelPerm.txt";
	
	FluidGrid.CalculateColRowPos();

	UpdateControls();
	
	if (!nDisabled)
	{
		m_Mu_Wat.cellEdit = FALSE;
		m_Mu_Oil.cellEdit = FALSE;
		FluidGrid.Disabled();
	}
	
	else
	{
		m_Mu_Wat.cellEdit = TRUE;
		m_Mu_Oil.cellEdit = TRUE;
	}
	
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_ROCK_PROP);
	pWnd->GetWindowRect(&rect);

	int c,
		nCols = 4,
		total = 0,
		width,
		tempColw[4] = { 50, 123, 175, 124 };

	width = rect.Width() - 14;

	for (int c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		RockGrid.SetColWidth(c, int(tempColw[c] * width / total));

	nCols = 9;
	total = 0;
	
	int tempColw2[9] = { 35, 45, 55, 55, 55, 55, 55, 55, 60 };

	width = rect.Width() - 14;

	for (int c = 0; c < nCols; c++)
		total += tempColw2[c];

	for (c = 0; c < nCols; c++)
		FluidGrid.SetColWidth(c, int(tempColw2[c] * width / total));

//	FluidGrid.SetRowHeight(0, 20);

	SetModified(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PropPage1::OnCancelMode() 
{
	CPropertyPage::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void PropPage1::Allocate()
{
}

void PropPage1::DeAllocate()
{
}

void PropPage1::UpdateControls()
{	
	int l, 
		 i = 1;

	CString sn;
	
	sn.Format("%.4g", WellRad);
	m_Well_Rad.sEdit = sn;

	sn.Format("%.4g", MaxRad);
	m_Max_Rad.sEdit = sn;

	sn.Format("%.4g", MUw);
	m_Mu_Wat.sEdit = sn;

	sn.Format("%.4g", MUo);
	m_Mu_Oil.sEdit = sn;

	for( l = 0 ; l < nLay ; l++ )
	{	
		// Update Static Data
		sn.Format("%.4g", SLay[l].Phi);
		RockGrid.colS[1][l+1]= sn;

		sn.Format("%.4g", SLay[l].KAbs);
		RockGrid.colS[2][l+1]= sn;

		if( nLay == 1)
		{
			RockGrid.cellEdit[2][l+1] = testgrid::READ_ONLY;
			RockGrid.cellFormat[2][l+1] = testgrid::GRAY;
		}

		sn.Format("%.4g", SLay[l].HLay);
		RockGrid.colS[3][l+1]= sn;
		
		// Update Dynamic Data
		if (nDisabled)
		{
			sn.Format("%d", SLay[l].Type);
			FluidGrid.colS[1][l + i] = sn;

			if (SLay[l].Type == 0)
			{
				FluidGrid.colS[1][l + i] = "Corey";

				sn.Format("%.4g", SLay[l].A);
				FluidGrid.colS[2][l + i] = sn;
			}

			if (SLay[l].Type == 1)
			{
				FluidGrid.colS[1][l + i] = "Table";

				FluidGrid.colS[2][l + i] = SLay[l].RelPermTabFile;
			}

			sn.Format("%.4g", SLay[l].Alpha);
			FluidGrid.colS[3][l + i] = sn;

			sn.Format("%.4g", SLay[l].B);
			FluidGrid.colS[4][l + i] = sn;

			sn.Format("%.4g", SLay[l].Beta);
			FluidGrid.colS[5][l + i] = sn;

			sn.Format("%.4g", SLay[l].Srw);
			FluidGrid.colS[6][l + i] = sn;

			sn.Format("%.4g", SLay[l].Sro);
			FluidGrid.colS[7][l + i] = sn;

			sn.Format("%.4g", SLay[l].Swi);
			FluidGrid.colS[8][l + i] = sn;
		}

		i++;
	}
	
}

void PropPage1::UpdateFromEdits()
{	
	CString sEdit;
	
	MaxRad = atof(m_Max_Rad.sEdit); 

	WellRad = atof(m_Well_Rad.sEdit);

	MUw = atof(m_Mu_Wat.sEdit);
	
	MUo = atof(m_Mu_Oil.sEdit);

	UpdateFromListCtrl();

}

void PropPage1::UpdateFromListCtrl()
{	
//	UpdateFromEdits();

	for (int i = 0; i < nLay; i++)
	{
		SLay[i].Phi = atof(RockGrid.colS[1][i+1]);
		SLay[i].KAbs = atof(RockGrid.colS[2][i+1]);
		SLay[i].HLay = atof(RockGrid.colS[3][i+1]);
		
		if ( FluidGrid.colS[1][2*i+1] == "Corey")
			
			SLay[i].Type = 0;
		
		if ( FluidGrid.colS[1][2*i+1] == "Table")
			
			SLay[i].Type = 1;

		if (SLay[i].Type == 0)
		{
			SLay[i].A = atof(FluidGrid.colS[2][2*i+1]);
			SLay[i].Alpha = atof(FluidGrid.colS[3][2*i+1]);
			SLay[i].B = atof(FluidGrid.colS[4][2*i+1]);
			SLay[i].Beta = atof(FluidGrid.colS[5][2*i+1]);
			SLay[i].Srw = atof(FluidGrid.colS[6][2*i+1]);
			SLay[i].Sro = atof(FluidGrid.colS[7][2*i+1]);			
		}
		
		if (SLay[i].Type == 1)
		
			SLay[i].RelPermTabFile = FluidGrid.colS[2][2*i+1];
		
		SLay[i].Swi = atof(FluidGrid.colS[8][2*i+1]);
	}
}

PropPage1& PropPage1::operator=(const PropPage1& rhs)
{
	if (this == &rhs) return *this;
	DeAllocate();
	nLay = rhs.nLay;
	Allocate();
	Equate(rhs);
	return *this;

}


void PropPage1::Equate(const PropPage1& rhs)
{
/*	m_Max_Rad = rhs.m_Max_Rad;

	m_Well_Rad = rhs.m_Well_Rad;

	m_Mu_Wat = rhs.m_Mu_Wat;

	m_Mu_Oil = rhs.m_Mu_Oil;
*/
	WellRad = rhs.WellRad;

	MaxRad = rhs.MaxRad;

	MUo = rhs.MUo;

	MUw = rhs.MUw;

	RockGrid = rhs.RockGrid;

	FluidGrid = rhs.FluidGrid;

	SLay.clear();

	SLay = rhs.SLay;	
}

LRESULT PropPage1::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	UpdateFromEdits();
	UpdateFromListCtrl();
	UpdateControls();
	if (!nDisabled)
		FluidGrid.Disabled();
	return 0;
}

LRESULT PropPage1::OnEditLostFocus(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	SetModified(TRUE);
	UpdateFromEdits();
	UpdateFromListCtrl();
	if (!nDisabled)
		FluidGrid.Disabled();
	return 0;
}

void PropPage1::SetHeaderRowFluidGrid(int r)
{	
	FluidGrid.SetCellText(0, r, "Layer");
	FluidGrid.SetCellText(1, r, "Type");
	FluidGrid.SetCellText(2, r, "a");
	FluidGrid.SetCellText(3, r, "Alpha");
	FluidGrid.SetCellText(4, r, "b");
	FluidGrid.SetCellText(5, r, "Beta");
	FluidGrid.SetCellText(6, r, "Irr. Water (Swi)");
	FluidGrid.SetCellText(7, r, "Irr. Oil (Sor)");
	FluidGrid.SetCellText(8, r, "Layer Water Sat");
	
	FluidGrid.SetColEdit(0, testgrid::READ_ONLY);
	FluidGrid.SetColFormat(0, testgrid::HEADER);
	FluidGrid.SetRowEdit(r, testgrid::READ_ONLY);
	FluidGrid.SetRowFormat(r, testgrid::HEADER);
	
	
	if (SLay[r/2].Type == 1)
	{
		FluidGrid.Merge(2, 7, r, r);
		
		FluidGrid.cellFormat[2][r] = testgrid::HEADER;

		FluidGrid.cellFormat[3][r] = testgrid::NORMAL;
		FluidGrid.cellFormat[4][r] = testgrid::NORMAL;
		FluidGrid.cellFormat[5][r] = testgrid::NORMAL;
		FluidGrid.cellFormat[6][r] = testgrid::NORMAL;
		FluidGrid.cellFormat[7][r] = testgrid::NORMAL;

		FluidGrid.colS[3][r] = " ";
		FluidGrid.colS[4][r] = " ";
		FluidGrid.colS[5][r] = " ";
		FluidGrid.colS[6][r] = " ";
		FluidGrid.colS[7][r] = " ";

		FluidGrid.Merge(2, 7, r+1, r+1);

		FluidGrid.cellFormat[3][r+1] = testgrid::NORMAL;
		FluidGrid.cellFormat[4][r+1] = testgrid::NORMAL;
		FluidGrid.cellFormat[5][r+1] = testgrid::NORMAL;
		FluidGrid.cellFormat[6][r+1] = testgrid::NORMAL;
		FluidGrid.cellFormat[7][r+1] = testgrid::NORMAL;
		
		FluidGrid.colS[3][r+1] = " ";
		FluidGrid.colS[4][r+1] = " ";
		FluidGrid.colS[5][r+1] = " ";
		FluidGrid.colS[6][r+1] = " ";
		FluidGrid.colS[7][r+1] = " ";

		FluidGrid.colS[2][r] = "Relative Permeability Table";
		FluidGrid.cellEdit[2][r+1] = testgrid::TABLE;
		FluidGrid.colS[2][r+1] = SLay[r/2].RelPermTabFile;

	}

	FluidGrid.SetRowHeight(r, 35);
	
}

void PropPage1::OnAllLayers()
{
	int l, 
		 i = 1,
		 temp;

	CString sn;
	
	for( l = 0 ; l < nLay ; l++ )
	{	
		if ( m_All_Layers.GetCheck() )

			temp = 0;

		else

			temp = l;
		
		// Update Dynamic Data
		sn.Format("%d", SLay[temp].Type);
		FluidGrid.colS[1][l+i]= sn;
		
		if( SLay[l].Type == 0 )
		{	
			FluidGrid.colS[1][l+i]= "Corey";

			sn.Format("%.4g", SLay[temp].A);
			FluidGrid.colS[2][l+i]= sn;
		}

		if( SLay[temp].Type == 1 )
		{		
			FluidGrid.colS[1][l+i]= "Table";

			FluidGrid.colS[2][l+i]= SLay[temp].RelPermTabFile;
		}
		
		sn.Format("%.4g", SLay[temp].Alpha);
		FluidGrid.colS[3][l+i]= sn;

		sn.Format("%.4g", SLay[temp].B);
		FluidGrid.colS[4][l+i]= sn;

		sn.Format("%.4g", SLay[temp].Beta);
		FluidGrid.colS[5][l+i]= sn;

		sn.Format("%.4g", SLay[temp].Srw);
		FluidGrid.colS[6][l+i]= sn;

		sn.Format("%.4g", SLay[temp].Sro);
		FluidGrid.colS[7][l+i]= sn;
		
		i++;
	}
	
	UpdateFromListCtrl();

	Invalidate(FALSE);
}


