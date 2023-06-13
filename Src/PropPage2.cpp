// PropPage2.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropPage2 property page

IMPLEMENT_DYNCREATE(PropPage2, CPropertyPage)

PropPage2::PropPage2() : CPropertyPage(PropPage2::IDD)
{
	//{{AFX_DATA_INIT(PropPage2)
	//}}AFX_DATA_INIT
	nLay = 1;
	nComp = 1;
	nLayMem = nLay;
	Allocate();
}

PropPage2::PropPage2(int l, int c) : CPropertyPage(PropPage2::IDD)
{
	//{{AFX_DATA_INIT(PropPage2)
	//}}AFX_DATA_INIT
	nLay = l;
	nLayMem = nLay;
	nComp = c;

	Allocate();
}

PropPage2::~PropPage2()
{
	DeAllocate();
}

void PropPage2::DoDataExchange(CDataExchange* pDX)
{	

	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropPage2)
	DDX_Control(pDX, IDC_STATIC_MASS_TRANS, MassTransGrid);
	DDX_Control(pDX, IDC_STATIC_WATER_PROP, WaterGrid);
	DDX_Control(pDX, IDC_STATIC_OIL_PROP, OilGrid);
	DDX_Control(pDX, IDC_ALL_LAYERS, m_All_Layers);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropPage2, CPropertyPage)
	//{{AFX_MSG_MAP(PropPage2)
	ON_BN_CLICKED(IDC_ALL_LAYERS, OnAllLayers)
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_BOX_LOST_FOCUS, OnBoxLostFocus)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE,OnListCtrlUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropPage2 message handlers

BOOL PropPage2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	int nTotal = nLay * nComp * 2,
		 numC = 0,
		 r;

	bool fieldUnits = TRUE;
	CString sItem;

	if (nDisabled) // Two Phase 
	{	
		testgrid TempGrid( nTotal , 8, nLay, nComp);
		WaterGrid = TempGrid;
		WaterGrid.kTemp = kTemp;

		WaterGrid.SetColEdit(2, testgrid::COMBO);
		
		WaterGrid.comboList[0].RemoveAll();
		
		WaterGrid.comboList[0].Add("Tracer");
		WaterGrid.comboList[0].Add("Inhib.");
		WaterGrid.comboList[0].Add("Visco.");
		WaterGrid.comboList[0].Add("Blocker");
		WaterGrid.comboList[0].Add("Surfact");
		WaterGrid.comboList[0].Add("Bridging");
		WaterGrid.comboList[0].Add("Wax");
		WaterGrid.comboList[0].Add("SI_FD");
/*		WaterGrid.comboList[0].Add("Ca");
		WaterGrid.comboList[0].Add("Na");*/

		WaterGrid.SetColEdit(3, testgrid::COMBO);
		
		WaterGrid.comboList[1].RemoveAll();
		WaterGrid.comboList[1].Add("Freundlich");
		WaterGrid.comboList[1].Add("Langmuir");
		WaterGrid.comboList[1].Add("IsoTable");
		
		WaterGrid.SetColEdit(6, testgrid::COMBO);

		WaterGrid.comboList[2].RemoveAll();
		WaterGrid.comboList[2].Add("Equilibrium");
//		WaterGrid.comboList[2].Add("Kinetic1");
		WaterGrid.comboList[2].Add("Kinetic");
		WaterGrid.comboList[2].Add("No Adso");

		WaterGrid.CalculateColRowPos();		 

		for ( r = 0; r < nTotal; r++)
		{
			if( r%2 == 0 )
				
				SetHeaderRowWaterGrid(r);

			if( r%2 == 1 )
			{	
				WaterGrid.SetColRowComboList(2, r, 0);
				WaterGrid.SetColRowComboList(3, r, 1);
				WaterGrid.SetColRowComboList(6, r, 2);

				sItem.Format("%d", int(r/(2 * nComp)) + 1);
				WaterGrid.colS[0][r] = sItem;
							
				numC++;	
				sItem.Format("%d", numC );
				WaterGrid.colS[1][r] = sItem;
				if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

		/*		if ( r > (nComp * 2) )
						
					WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;
			*/	
			}
		
		}
	}
	else // Single Phase 
	{	
		testgrid TempGrid( nTotal , 7, nLay, nComp);
		WaterGrid = TempGrid;
		WaterGrid.kTemp = kTemp;
		
		if (kTemp)
		{	
			WaterGrid.SetColEdit(1, testgrid::COMBO);
		
			WaterGrid.comboList[0].RemoveAll();
			WaterGrid.comboList[0].Add("Adsorption");
			WaterGrid.comboList[0].Add("Precipitation");
		}
		else
		{
			WaterGrid.SetColEdit(1, testgrid::READ_ONLY);
		}

		WaterGrid.SetColEdit(2, testgrid::COMBO);
			
		WaterGrid.comboList[1].RemoveAll();
		WaterGrid.comboList[1].Add("Freundlich");
		WaterGrid.comboList[1].Add("Langmuir");
		WaterGrid.comboList[1].Add("IsoTable");

		WaterGrid.SetColEdit(5, testgrid::COMBO);

		WaterGrid.comboList[2].RemoveAll();
		WaterGrid.comboList[2].Add("Equilibrium");
	//	WaterGrid.comboList[2].Add("Kinetic1");
		WaterGrid.comboList[2].Add("Kinetic");

		WaterGrid.CalculateColRowPos();		 

		for ( r = 0; r < nTotal; r++)
		{
			if( r%2 == 0 )
				
				SetHeaderRowWaterGridSingle(r);

			if( r%2 == 1 )
			{	
				if( kTemp )
				
					WaterGrid.SetColRowComboList(1, r, 0);

				else
				{
					WaterGrid.colS[1][r] = "Adsorption";
					WaterGrid.cellEdit[1][r] = testgrid::READ_ONLY;
				
				}
				WaterGrid.SetColRowComboList(2, r, 1);
				WaterGrid.SetColRowComboList(5, r, 2);

				sItem.Format("%d", int(r/(2 * nComp)) + 1);
				WaterGrid.colS[0][r] = sItem;
			}
		
		}
	}
	
/*	testgrid TempGrid( nTotal , 8, nLay, nComp);
	OilGrid = TempGrid;
	
	OilGrid.SetColEdit(2, testgrid::COMBO);
	
	OilGrid.comboList[0].RemoveAll();
	OilGrid.comboList[0].Add("Freundlich");
	OilGrid.comboList[0].Add("Langmuir");
	OilGrid.comboList[0].Add("Isotherm");

	OilGrid.SetColEdit(5, testgrid::COMBO);
	OilGrid.comboList[1].RemoveAll();
	OilGrid.comboList[1].Add("Equilibrium");
	OilGrid.comboList[2].Add("Kinetic1");
	OilGrid.comboList[2].Add("Kinetic2");

	OilGrid.CalculateColRowPos();		 

	for ( r = 0; r < nTotal; r++)
	{
		if( r%2 == 0 )
			
			SetHeaderRowOilGrid(r);

		if( r%2 == 1 )
		{	
			OilGrid.SetColRowComboList(2, r, 0);
			OilGrid.SetColRowComboList(5, r, 1);

			sItem.Format("%d", int(r/(2 * nComp)) + 1);
			OilGrid.colS[0][r] = sItem;
						
			numC++;	
			sItem.Format("%d", numC );
			OilGrid.colS[1][r] = sItem;
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;
		}
	
	}
*/	
	testgrid TempGrid( nTotal , 8, nLay, nComp);
	OilGrid = TempGrid;
	OilGrid.kTemp = kTemp;

	OilGrid.SetColEdit(2, testgrid::COMBO);
	
	OilGrid.comboList[0].RemoveAll();
	
	OilGrid.comboList[0].Add("Tracer");
	OilGrid.comboList[0].Add("Inhib.");
	OilGrid.comboList[0].Add("Visco.");
//	OilGrid.comboList[0].Add("Blocker");

	OilGrid.SetColEdit(3, testgrid::COMBO);
	
	OilGrid.comboList[1].RemoveAll();
	OilGrid.comboList[1].Add("Freundlich");
	OilGrid.comboList[1].Add("Langmuir");
	OilGrid.comboList[1].Add("IsoTable");

	OilGrid.SetColEdit(6, testgrid::COMBO);

	OilGrid.comboList[2].RemoveAll();
	OilGrid.comboList[2].Add("Equilibrium");
//	OilGrid.comboList[2].Add("Kinetic1");
	OilGrid.comboList[2].Add("Kinetic");
	OilGrid.comboList[2].Add("No Adso");

	OilGrid.CalculateColRowPos();		 

	for ( r = 0; r < nTotal; r++)
	{
		if( r%2 == 0 )
			
			SetHeaderRowOilGrid(r);

		if( r%2 == 1 )
		{	
			OilGrid.SetColRowComboList(2, r, 0);
			OilGrid.SetColRowComboList(3, r, 1);
			OilGrid.SetColRowComboList(6, r, 2);

			sItem.Format("%d", int(r/(2 * nComp)) + 1);
			OilGrid.colS[0][r] = sItem;
						
			numC++;	
			sItem.Format("%d", numC );
			OilGrid.colS[1][r] = sItem;
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;
		}
	
	}

	WaterGrid.IsoTable.SetPathName(PathName);
	WaterGrid.IsoTable.NameFile = "IsoTable.txt";
	
	OilGrid.IsoTable.SetPathName(PathName);
	OilGrid.IsoTable.NameFile = "IsoTable.txt";

	testgrid TempMTGrid( nComp + 1 , 3);
	
	MassTransGrid = TempMTGrid;
	
	MassTransGrid.SetColEdit(1, testgrid::COMBO);
	
	MassTransGrid.comboList[0].RemoveAll();
	MassTransGrid.comboList[0].Add("None");
	MassTransGrid.comboList[0].Add("Equ");
	
	SetHeaderRowMassTransGrid(0);
	
	for ( r = 1; r <= nComp; r++)
	{
		sItem.Format("%d", r);
		MassTransGrid.colS[0][r] = sItem;
	}
	
	UpdateControls();
	
	if (!IsoSpec)
	{
		m_All_Layers.SetCheck( BST_CHECKED); 
		OnAllLayers();
	}

	else m_All_Layers.SetCheck( BST_UNCHECKED);
	
	if (!nDisabled)
	{	
		if( iModel == 5 )
			
			WaterGrid.Disabled();
		
		OilGrid.Disabled();
		MassTransGrid.Disabled();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PropPage2::SetHeaderRowWaterGrid(int r)
{
	WaterGrid.SetCellText(0, r, "Layer");
	WaterGrid.SetColWidth(0, 35);
	WaterGrid.SetCellText(1, r, "Comp");
	WaterGrid.SetColWidth(1, 38);
	WaterGrid.SetCellText(2, r, "Role");
	WaterGrid.SetColWidth(2, 48);
	WaterGrid.SetCellText(3, r, "Isotherm");
	WaterGrid.SetColWidth(3, 66);
	WaterGrid.SetCellText(4, r, "a");
	WaterGrid.SetColWidth(4, 70);
	WaterGrid.SetCellText(5, r, "b");
	WaterGrid.SetColWidth(5, 70);
	WaterGrid.SetCellText(6, r, "Adso.Type");
	WaterGrid.SetColWidth(6, 70);
	WaterGrid.SetCellText(7, r, "Rate (1/day)");									
	WaterGrid.SetColWidth(7, 74);

	WaterGrid.SetRowHeight(r, 20);

	WaterGrid.SetColEdit(0, testgrid::READ_ONLY);
	WaterGrid.SetColEdit(1, testgrid::READ_ONLY);
	WaterGrid.SetRowEdit(r, testgrid::READ_ONLY);

	WaterGrid.SetColFormat(0, testgrid::HEADER);
	WaterGrid.SetColFormat(1, testgrid::HEADER);
	WaterGrid.SetRowFormat(r, testgrid::HEADER);

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_WATER_PROP);
	pWnd->GetWindowRect(&rect);

	int c,
		nCols = 8,
		total = 0,
		width,
		tempColw[8] = { 35, 38, 48, 66, 70, 70, 70, 74 };

	width = rect.Width() - 14;

	for (c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		WaterGrid.SetColWidth(c, int(tempColw[c] * width / total));
}


void PropPage2::SetHeaderRowOilGrid(int r)
{
	OilGrid.SetCellText(0, r, "Layer");
	OilGrid.SetColWidth(0, 35);
	OilGrid.SetCellText(1, r, "Comp");
	OilGrid.SetColWidth(1, 38);
	OilGrid.SetCellText(2, r, "Role");
	OilGrid.SetColWidth(2, 48);
	OilGrid.SetCellText(3, r, "Isotherm");
	OilGrid.SetColWidth(3, 66);
	OilGrid.SetCellText(4, r, "a");
	OilGrid.SetColWidth(4, 70);
	OilGrid.SetCellText(5, r, "b");
	OilGrid.SetColWidth(5, 70);
	OilGrid.SetCellText(6, r, "Adso.Type");
	OilGrid.SetColWidth(6, 70);
	OilGrid.SetCellText(7, r, "Rate (1/day)");									
	OilGrid.SetColWidth(7, 74);

	OilGrid.SetRowHeight(r, 20);

	OilGrid.SetColEdit(0, testgrid::READ_ONLY);
	OilGrid.SetColEdit(1, testgrid::READ_ONLY);
	OilGrid.SetRowEdit(r, testgrid::READ_ONLY);

	OilGrid.SetColFormat(0, testgrid::HEADER);
	OilGrid.SetColFormat(1, testgrid::HEADER);
	OilGrid.SetRowFormat(r, testgrid::HEADER);


/*	OilGrid.SetCellText(0, r, "Layer");
	OilGrid.SetColWidth(0, 40);
	OilGrid.SetCellText(1, r, "Comp.Num");
	OilGrid.SetColWidth(1, 73);
	OilGrid.SetCellText(2, r, "Isotherm");
	OilGrid.SetColWidth(2, 66);
	OilGrid.SetCellText(3, r, "a");
	OilGrid.SetColWidth(3, 74);
	OilGrid.SetCellText(4, r, "b");
	OilGrid.SetColWidth(4, 74);
	OilGrid.SetCellText(5, r, "Adso.Type");
	OilGrid.SetColWidth(5, 70);
	OilGrid.SetCellText(6, r, "Rate (1/day)");
	OilGrid.SetColWidth(6, 74);

	OilGrid.SetRowHeight(r, 20);

	OilGrid.SetColEdit(0, testgrid::READ_ONLY);
	OilGrid.SetColEdit(1, testgrid::READ_ONLY);
	OilGrid.SetRowEdit(r, testgrid::READ_ONLY);

	OilGrid.SetColFormat(0, testgrid::HEADER);
	OilGrid.SetColFormat(1, testgrid::HEADER);
	OilGrid.SetRowFormat(r, testgrid::HEADER);
*/	
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_OIL_PROP);
	pWnd->GetWindowRect(&rect);

	int c,
		nCols = 8,
		total = 0,
		width,
		tempColw[8] = { 35, 38, 48, 66, 70, 70, 70, 74 };

	width = rect.Width() - 14;

	for (c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		OilGrid.SetColWidth(c, int(tempColw[c] * width / total));
}

void PropPage2::SetHeaderRowWaterGridSingle(int r)
{
	WaterGrid.SetCellText(0, r, "Layer");
	WaterGrid.SetColWidth(0, 40);
	WaterGrid.SetCellText(1, r, "Prep/Adso");
	WaterGrid.SetColWidth(1, 73);
	WaterGrid.SetCellText(2, r, "Isotherm");
	WaterGrid.SetColWidth(2, 66);
	WaterGrid.SetCellText(3, r, "a");
	WaterGrid.SetColWidth(3, 74);
	WaterGrid.SetCellText(4, r, "b");
	WaterGrid.SetColWidth(4, 74);
	WaterGrid.SetCellText(5, r, "Adso.Type");
	WaterGrid.SetColWidth(5, 70);
	WaterGrid.SetCellText(6, r, "Rate (1/day)");
	WaterGrid.SetColWidth(6, 74);

	WaterGrid.SetRowHeight(r, 40);

	WaterGrid.SetColEdit(0, testgrid::READ_ONLY);
	WaterGrid.SetRowEdit(r, testgrid::READ_ONLY);

	WaterGrid.SetColFormat(0, testgrid::HEADER);
	WaterGrid.SetRowFormat(r, testgrid::HEADER);

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_WATER_PROP);
	pWnd->GetWindowRect(&rect);

	int c,
		nCols = 7,
		total = 0,
		width,
		tempColw[7] = { 40, 73, 66, 74, 74, 70, 74 };

	width = rect.Width() - 14;

	for ( c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		WaterGrid.SetColWidth(c, int(tempColw[c] * width / total));
}

void PropPage2::SetHeaderRowMassTransGrid(int r)
{
	MassTransGrid.SetCellText(0, r, "Comp");
	MassTransGrid.SetColWidth(0, 45);
	MassTransGrid.SetCellText(1, r, "Mass Trans");
	MassTransGrid.SetColWidth(1, 78);
	MassTransGrid.SetCellText(2, r, "Ratio");
	MassTransGrid.SetColWidth(2, 71);
	
	MassTransGrid.SetColEdit(0, testgrid::READ_ONLY);
	MassTransGrid.SetRowEdit(r, testgrid::READ_ONLY);
	MassTransGrid.SetColFormat(0, testgrid::HEADER);
	MassTransGrid.SetRowFormat(r, testgrid::HEADER);

	int c,
		nCols = 3,
		total = 0,
		width;

	int tempColw[9] = { 35, 45, 55, 55, 55, 55, 55, 55, 60 };

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_MASS_TRANS);
	pWnd->GetWindowRect(&rect);

	width = rect.Width() - 14;

	for (int c = 0; c < nCols; c++)
		total += tempColw[c];

	for (c = 0; c < nCols; c++)
		MassTransGrid.SetColWidth(c, int(tempColw[c] * width / total));

	MassTransGrid.SetRowHeight(0, 35);
}

void PropPage2::UpdateGrids()
{
	int nTotal = nLay * nComp * 2,
		 numC = 0,
		 r;

	bool fieldUnits = TRUE;
	CString sItem;
	
	if (nDisabled) // Two Phase 
	{	
		testgrid TempGrid( nTotal , 8, nLay, nComp);
		WaterGrid = TempGrid;
		
		WaterGrid.kTemp = kTemp;	 

		for ( r = 0; r < nTotal; r++)
		{
			if( r%2 == 0 )
				
				SetHeaderRowWaterGrid(r);

			if( r%2 == 1 )
			{	
				WaterGrid.SetColRowComboList(2, r, 0);
				WaterGrid.SetColRowComboList(3, r, 1);
				WaterGrid.SetColRowComboList(6, r, 2);

				sItem.Format("%d", int(r/(2 * nComp)) + 1);
				WaterGrid.colS[0][r] = sItem;
							
				numC++;	
				sItem.Format("%d", numC );
				WaterGrid.colS[1][r] = sItem;
				if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

				if ( r > (nComp * 2) )
						
					WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;
			}
		}

		WaterGrid.SetColEdit(2, testgrid::COMBO);

		WaterGrid.comboList[0].RemoveAll();

		WaterGrid.comboList[0].Add("Tracer");
		WaterGrid.comboList[0].Add("Inhib.");
		WaterGrid.comboList[0].Add("Visco.");
		WaterGrid.comboList[0].Add("Blocker");
		WaterGrid.comboList[0].Add("Surfact");
		WaterGrid.comboList[0].Add("Bridging");
		WaterGrid.comboList[0].Add("Wax");
		WaterGrid.comboList[0].Add("SI_FD");
		/*		WaterGrid.comboList[0].Add("Ca");
		WaterGrid.comboList[0].Add("Na");
		*/
		WaterGrid.SetColEdit(3, testgrid::COMBO);

		WaterGrid.comboList[1].RemoveAll();
		WaterGrid.comboList[1].Add("Freundlich");
		WaterGrid.comboList[1].Add("Langmuir");
		WaterGrid.comboList[1].Add("IsoTable");

		WaterGrid.SetColEdit(6, testgrid::COMBO);

		WaterGrid.comboList[2].RemoveAll();
		WaterGrid.comboList[2].Add("Equilibrium");
		//	WaterGrid.comboList[2].Add("Kinetic1");
		WaterGrid.comboList[2].Add("Kinetic");
		WaterGrid.comboList[2].Add("No Adso");

		WaterGrid.CalculateColRowPos();
	
	}
	else // Single Phase 
	{	
		testgrid TempGrid( nTotal , 7, nLay, nComp);
		WaterGrid = TempGrid;
		
		WaterGrid.kTemp = kTemp;
		
			 
		for ( r = 0; r < nTotal; r++)
		{
			if( r%2 == 0 )
				
				SetHeaderRowWaterGridSingle(r);

			if( r%2 == 1 )
			{	
				WaterGrid.SetColRowComboList(2, r, 0);
				WaterGrid.SetColRowComboList(2, r, 1);
				WaterGrid.SetColRowComboList(5, r, 2);

				sItem.Format("%d", int(r/(2 * nComp)) + 1);
				WaterGrid.colS[0][r] = sItem;
			}
		
		}

		if (kTemp)
		{
			WaterGrid.SetColEdit(1, testgrid::COMBO);

			WaterGrid.comboList[0].RemoveAll();
			WaterGrid.comboList[0].Add("Adsorption");
			WaterGrid.comboList[0].Add("Precipitation");
		}
		else
		{
			WaterGrid.SetColEdit(1, testgrid::READ_ONLY);
		}

		WaterGrid.SetColEdit(2, testgrid::COMBO);

		WaterGrid.comboList[1].RemoveAll();
		WaterGrid.comboList[1].Add("Freundlich");
		WaterGrid.comboList[1].Add("Langmuir");
		WaterGrid.comboList[1].Add("IsoTable");

		WaterGrid.SetColEdit(5, testgrid::COMBO);

		WaterGrid.comboList[2].RemoveAll();
		WaterGrid.comboList[2].Add("Equilibrium");
		WaterGrid.comboList[2].Add("Kinetic");

		WaterGrid.CalculateColRowPos();
	}
	
	testgrid TempGrid( nTotal , 8, nLay, nComp);
	OilGrid = TempGrid;
	
	OilGrid.kTemp = kTemp;

/*	OilGrid.SetColEdit(2, testgrid::COMBO);
	
	OilGrid.comboList[0].RemoveAll();

	OilGrid.comboList[0].Add("Tracer");
	OilGrid.comboList[0].Add("Inhib.");
	OilGrid.comboList[0].Add("Visco.");
//	OilGrid.comboList[0].Add("Blocker");
*/
	OilGrid.SetColEdit(2, testgrid::READ_ONLY);

	OilGrid.SetColEdit(3, testgrid::COMBO);
	
	OilGrid.comboList[1].RemoveAll();
	OilGrid.comboList[1].Add("Freundlich");
	OilGrid.comboList[1].Add("Langmuir");
	OilGrid.comboList[1].Add("IsoTable");

	OilGrid.SetColEdit(6, testgrid::COMBO);

	OilGrid.comboList[2].RemoveAll();
	OilGrid.comboList[2].Add("Equilibrium");
//	OilGrid.comboList[2].Add("Kinetic1");
	OilGrid.comboList[2].Add("Kinetic");
	OilGrid.comboList[2].Add("No Adso");

	OilGrid.CalculateColRowPos();		 

	for ( r = 0; r < nTotal; r++)
	{
		if( r%2 == 0 )
			
			SetHeaderRowOilGrid(r);

		if( r%2 == 1 )
		{	
			OilGrid.SetColRowComboList(2, r, 0);
			OilGrid.SetColRowComboList(3, r, 1);
			OilGrid.SetColRowComboList(6, r, 2);

			sItem.Format("%d", int(r/(2 * nComp)) + 1);
			OilGrid.colS[0][r] = sItem;
						
			numC++;	
			sItem.Format("%d", numC );
			OilGrid.colS[1][r] = sItem;
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

			if ( r > (nComp * 2) )
					
				OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
		}
	}
	
	UpdateControls();
}


void PropPage2::Allocate()
{	
	waterComp.resize(nComp);
	oilComp.resize(nComp);

}

void PropPage2::DeAllocate()
{	
	

}

PropPage2& PropPage2::operator=(const PropPage2& rhs)
{
	if (this == &rhs) return *this;
	DeAllocate();
	nLay = rhs.nLay;
	nLayMem = rhs.nLayMem;
	TempnLay = rhs.TempnLay;
	nComp = rhs.nComp;
	Allocate();
	Equate(rhs);
	return *this;

}

void PropPage2::Equate(const PropPage2& rhs)
{
//	m_All_Layers = rhs.m_All_Layers;

//	m_Mass_Ratio = rhs.m_Mass_Ratio;

//	m_Mass_Transfer = rhs.m_Mass_Transfer;

	TempnLay = rhs.TempnLay;

	WaterGrid = rhs.WaterGrid;

	OilGrid = rhs.OilGrid;

	nLayMem = rhs.nLayMem;

	wIso = rhs.wIso;
	oIso = rhs.oIso;
	
/*
	nLay = rhs.nLay;
	nComp = rhs.nComp;
	IsoSpec = rhs.IsoSpec;
	iModel = rhs.iModel;
	nDisabled =  rhs.nDisabled;
*/
}

/*
void PropPage2::UpdateControls()
{	
	CString sItem;
	
	int nTotal= nLay * nComp * 2,
		 numC = 0,
		 r,
		 c;

	for ( r = 0; r < nTotal; r++)
	{
		if ( r%2 == 1 )
		{

			if(nDisabled) // Two Phase 
			{	
				// WATER GRID
				if ( waterComp[numC].Role == 0 )
				{	
					WaterGrid.colS[2][r] = "Tracer";
				}
				
				if ( waterComp[numC].Role == 1 )
				{
					WaterGrid.colS[2][r] = "Inhib.";
				}

				if ( waterComp[numC].Role == 2 )
				{
					WaterGrid.colS[2][r] = "Visco.";
				}

				if ( waterComp[numC].Role == 3 )
				{
					WaterGrid.colS[2][r] = "Blocker";
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
				{
					sItem = "Freundlich";
					
					WaterGrid.colS[3][r] = sItem;

					WaterGrid.colS[4][r-1] = "k";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );
					
					WaterGrid.colS[4][r] = sItem;

					WaterGrid.colS[5][r-1] = "n";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].n );

					WaterGrid.colS[5][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
				{
					sItem = "Langmuir";
					
					WaterGrid.colS[3][r] = sItem;

					WaterGrid.colS[4][r-1] = "a";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].a );

					WaterGrid.colS[4][r] = sItem;
					
					WaterGrid.colS[5][r-1] = "b";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].b );

					WaterGrid.colS[5][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
				{
					sItem = "Isotherm";
					
					WaterGrid.colS[3][r] = sItem;

					WaterGrid.Merge(4, 5, r, r);

					WaterGrid.Merge(4, 5, r-1, r-1);
					
					WaterGrid.cellEdit[4][r] = testgrid::ISO;

					WaterGrid.cellFormat[4][r-1] = testgrid::HEADER;
					
					WaterGrid.cellFormat[5][r-1] = testgrid::NORMAL;

				//	WaterGrid.colS[4][r-1] = "";

					WaterGrid.colS[4][r-1] = "Iso Table";
					
					WaterGrid.SetCellText(4, r, wIso[int(r/(2 * nComp))][numC].IsoTabFile);
					
				//	WaterGrid.colS[4][r] = "";

				}

				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
				{
					sItem = "Equilibrium";
					
					WaterGrid.colS[6][r] = sItem;

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
					
					WaterGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[7][r] = testgrid::GRAY;
					WaterGrid.colS[7][r] = "";
					
				}
				
				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
				{
					sItem = "Kinetic";
					
					WaterGrid.colS[6][r] = sItem;

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
					WaterGrid.colS[7][r] = sItem;

				}

			}
			else // Single Phase
			{
				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
				{	
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "Freundlich";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.colS[3][r-1] = "k";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );
					
					WaterGrid.colS[3][r] = sItem;

					WaterGrid.colS[4][r-1] = "n";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].n );

					WaterGrid.colS[4][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
				{
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "Langmuir";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.colS[3][r-1] = "a";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].a );

					WaterGrid.colS[3][r] = sItem;
					
					WaterGrid.colS[4][r-1] = "b";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].b );

					WaterGrid.colS[4][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
				{	
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "Isotherm";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.Merge(3, 4, r, r);

					WaterGrid.Merge(3, 4, r-1, r-1);
					
					WaterGrid.cellEdit[3][r] = testgrid::ISO;

					WaterGrid.cellFormat[3][r-1] = testgrid::HEADER;
					
					WaterGrid.cellFormat[4][r-1] = testgrid::NORMAL;

				//	WaterGrid.colS[4][r-1] = "";

					WaterGrid.colS[3][r-1] = "Iso Table";
					
					WaterGrid.SetCellText(3, r, wIso[int(r/(2 * nComp))][numC].IsoTabFile);
					
				//	WaterGrid.colS[4][r] = "";

				}

				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
				{
					sItem = "Equilibrium";
					
					WaterGrid.colS[5][r] = sItem;
		
					WaterGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[6][r] = testgrid::GRAY;					
					WaterGrid.colS[6][r] = "";
				}
				
				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
				{
					sItem = "Kinetic";
					
					WaterGrid.colS[5][r] = sItem;

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
					WaterGrid.colS[6][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 4 )
				{
					WaterGrid.colS[1][r] = "Precipitation";

					WaterGrid.colS[3][r-1] = "Cps1";
					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Cps1);
					WaterGrid.colS[3][r] = sItem;
					
					if ( kTemp )
					{	
						WaterGrid.colS[4][r-1] = "Cps2";
						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Cps2 );
						WaterGrid.colS[4][r] = sItem;
					
						WaterGrid.colS[2][r-1] = "Cr.Temp";
						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Tpc);
						WaterGrid.colS[2][r] = sItem;
						WaterGrid.cellEdit[2][r] = testgrid::EDIT;
					}
					else
					{
						WaterGrid.colS[4][r-1] = "Cps2";
						WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[4][r] = testgrid::GRAY;	
						WaterGrid.colS[4][r] = "";
					
						WaterGrid.colS[2][r-1] = "Cr.Temp";
						WaterGrid.cellEdit[2][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[2][r] = testgrid::GRAY;	
						WaterGrid.colS[2][r] = "";
					}
					
					WaterGrid.colS[5][r-1]= "Prep.Type";
					WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[5][r] = testgrid::GRAY;	

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r4 );
					WaterGrid.colS[6][r] = sItem;
				}

			}
			
			// OIL GRID
	
			if ( oilCompAdsIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
			{
				sItem = "Freundlich";
				
				OilGrid.colS[3][r] = sItem;

				OilGrid.colS[4][r-1] = "k";

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].k );
				
				OilGrid.colS[4][r] = sItem;

				OilGrid.colS[5][r-1] = "n";

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].n );

				OilGrid.colS[5][r] = sItem;
			}

			if ( oilCompAdsIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
			{
				sItem = "Langmuir";
				
				OilGrid.colS[3][r] = sItem;

				OilGrid.colS[4][r-1] = "a";

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].a );

				OilGrid.colS[4][r] = sItem;
				
				OilGrid.colS[5][r-1] = "b";

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].b );

				OilGrid.colS[5][r] = sItem;
			}

			if ( oilCompAdsIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
			{
				sItem = "Isotherm";
				
				OilGrid.colS[3][r] = sItem;

				OilGrid.Merge(4, 5, r, r);

				OilGrid.Merge(4, 5, r-1, r-1);
				
				OilGrid.cellEdit[4][r] = testgrid::ISO;

				OilGrid.cellFormat[4][r-1] = testgrid::HEADER;
				
				OilGrid.cellFormat[5][r-1] = testgrid::NORMAL;

			//	OilGrid.colS[4][r-1] = "";

				OilGrid.colS[4][r-1] = "Iso Table";
				
				OilGrid.SetCellText(4, r, oilCompAdsIso[int(r/(2 * nComp))][numC].IsoTabFile);
				
			//	OilGrid.colS[4][r] = "";

			}

			if ( oilCompAdsIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
			{
				sItem = "Equilibrium";
				
				OilGrid.colS[6][r] = sItem;

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].r2 );
				
				OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
				OilGrid.cellFormat[7][r] = testgrid::GRAY;
				OilGrid.colS[7][r] = "";
				
			}
			
			if ( oilCompAdsIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
			{
				sItem = "Kinetic";
				
				OilGrid.colS[6][r] = sItem;

				sItem.Format("%.4g", oilCompAdsIso[int(r/(2 * nComp))][numC].r2 );
				OilGrid.colS[7][r] = sItem;

			}


			numC++;	
			
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

		}
	
	}

	for ( r = 0; r < nTotal; r++)
	{	

		if( r%2 == 1 )
		{	
			if (nDisabled) // Two Phase 
			{
				// WATER GRID

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
				{
					WaterGrid.colS[5][r-1] = "";

					WaterGrid.colS[5][r] = "";

				}

				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
				{
					WaterGrid.colS[7][r] = "";
				}
				
				if ( waterComp[numC].Role == 0 )
				{
					WaterGrid.colS[2][r] = "Tracer";
				}

				if ( waterComp[numC].Role == 1 )
				{
					WaterGrid.colS[2][r] = "Inhib.";
				}

				if ( waterComp[numC].Role == 2 )
				{
					WaterGrid.colS[2][r] = "Visco.";
				}

				if ( waterComp[numC].Role == 3 )
				{
					WaterGrid.colS[2][r] = "Blocker";
				}

			}
			else // Single Phase
			{
			
			}
			
		}
	
	}
	
	// Update Mass Transfer Grid 	
	
	for ( c = 0; c < nComp; c++)
	{

		if ( waterComp[c].MTrans == 0 )
		{
			sItem = "None";
			MassTransGrid.colS[1][c+1] = sItem;
			MassTransGrid.cellEdit[2][c+1] = testgrid::READ_ONLY;
			MassTransGrid.cellFormat[2][c+1] = testgrid::GRAY;
			MassTransGrid.colS[2][c+1] = "";
		}

		if ( waterComp[c].MTrans == 1 )
		{
			sItem = "Equ";
			MassTransGrid.colS[1][c+1] = sItem;
			sItem.Format("%.4g", waterComp[c].K );
			MassTransGrid.colS[2][c+1] = sItem;
		}
	}
	
}
*/

void PropPage2::UpdateControls()
{	
	CString sItem;
	
	int nTotal= nLay * nComp * 2,
		 numC = 0,
		 r,
		 c,
		 i;
/*	if ( !IsoSpec ) nLay = 1;
	
	nTotal = nLay * nComp * 2;
*/	
	int roles[10] = {0};

	for ( i = 0; i < nComp; i++)
	
		roles[i] = waterComp[i].Role;
	
	testgrid::set_compRole(roles);

	for ( r = 0; r < nTotal; r++)
	{
		if ( r%2 == 1 )
		{

			if(nDisabled) // Two Phase 
			{	
				// WATER GRID
				if ( waterComp[numC].Role == 0 )
				{	
					WaterGrid.colS[2][r] = "Tracer";
					OilGrid.colS[2][r] = "Tracer";
					
					WaterGrid.colS[3][r] = " ";
					WaterGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[3][r] = testgrid::GRAY;
					
					WaterGrid.colS[4][r] = " ";
					WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[4][r] = testgrid::GRAY;
					
					WaterGrid.colS[5][r] = " ";
					WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[5][r] = testgrid::GRAY;
					
					WaterGrid.colS[6][r] = " ";
					WaterGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[6][r] = testgrid::GRAY;
					
					WaterGrid.colS[7][r] = " ";
					WaterGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[7][r] = testgrid::GRAY;
					
					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.colS[3][r] = " ";
					OilGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[3][r] = testgrid::GRAY;

					OilGrid.colS[4][r] = " ";
					OilGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[4][r] = testgrid::GRAY;

					OilGrid.colS[5][r] = " ";
					OilGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[5][r] = testgrid::GRAY;
				
					OilGrid.colS[6][r] = " ";
					OilGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[6][r] = testgrid::GRAY;
				
					OilGrid.colS[7][r] = " ";
					OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[7][r] = testgrid::GRAY;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}
				
				if ( waterComp[numC].Role == 1 )
				{
					WaterGrid.colS[2][r] = "Inhib.";
					OilGrid.colS[2][r] = "Inhib.";
					
					WaterGrid.cellEdit[3][r]= testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;
				
					WaterGrid.cellEdit[4][r]= testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;
				
					WaterGrid.cellEdit[5][r]= testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[6][r]= testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[7][r]= testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL; 

					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r]= testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[4][r]= testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[5][r]= testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[6][r]= testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[7][r]= testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				if ( waterComp[numC].Role == 2 )
				{
					WaterGrid.colS[2][r] = "Visco.";
					OilGrid.colS[2][r] = "Visco.";

					WaterGrid.cellEdit[3][r]= testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[4][r]= testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[5][r]= testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[6][r]= testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[7][r]= testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL;
					
					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r]= testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;
				
					OilGrid.cellEdit[4][r]= testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;
			
					OilGrid.cellEdit[5][r]= testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;
			
					OilGrid.cellEdit[6][r]= testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;
	
					OilGrid.cellEdit[7][r]= testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				if ( waterComp[numC].Role == 3 )
				{
					WaterGrid.colS[2][r] = "Blocker";
					OilGrid.colS[2][r] = "Blocker";
			
					WaterGrid.cellEdit[3][r]= testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[4][r]= testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[5][r]= testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[6][r]= testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[7][r]= testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r]= testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;
				
					OilGrid.cellEdit[4][r]= testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[5][r]= testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[6][r]= testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[7][r]= testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}
				
				if ( waterComp[numC].Role == 4 )
				{
					WaterGrid.colS[2][r] = "Surfact";
					OilGrid.colS[2][r] = "Surfact";
			
					WaterGrid.cellEdit[3][r]= testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[4][r]= testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[5][r]= testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[6][r]= testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[7][r]= testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r]= testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;
				
					OilGrid.cellEdit[4][r]= testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[5][r]= testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[6][r]= testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[7][r]= testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}
				
				if ( waterComp[numC].Role == 7 )
				{
					WaterGrid.colS[2][r] = "Bridging";
					OilGrid.colS[2][r] = "Bridging";
			
					WaterGrid.cellEdit[3][r]= testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[4][r]= testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[5][r]= testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[6][r]= testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					WaterGrid.cellEdit[7][r]= testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r]= testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;
				
					OilGrid.cellEdit[4][r]= testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[5][r]= testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[6][r]= testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;
					
					OilGrid.cellEdit[7][r]= testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				if ( waterComp[numC].Role == 8 )
				{	
					WaterGrid.colS[2][r] = "Wax";
					OilGrid.colS[2][r] = "Wax";
					
					WaterGrid.colS[3][r] = " ";
					WaterGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[3][r] = testgrid::GRAY;
					
					WaterGrid.colS[4][r] = " ";
					WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[4][r] = testgrid::GRAY;
					
					WaterGrid.colS[5][r] = " ";
					WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[5][r] = testgrid::GRAY;
					
					WaterGrid.colS[6][r] = " ";
					WaterGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[6][r] = testgrid::GRAY;
					
					WaterGrid.colS[7][r] = " ";
					WaterGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[7][r] = testgrid::GRAY;
					
					if ( r > (nComp * 2) )
						
						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.colS[3][r] = " ";
					OilGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[3][r] = testgrid::GRAY;

					OilGrid.colS[4][r] = " ";
					OilGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[4][r] = testgrid::GRAY;

					OilGrid.colS[5][r] = " ";
					OilGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[5][r] = testgrid::GRAY;
				
					OilGrid.colS[6][r] = " ";
					OilGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[6][r] = testgrid::GRAY;
				
					OilGrid.colS[7][r] = " ";
					OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[7][r] = testgrid::GRAY;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				if (waterComp[numC].Role == 9)
				{
					WaterGrid.colS[2][r] = "SI_FD";
					OilGrid.colS[2][r] = "SI_FD";

					WaterGrid.cellEdit[3][r] = testgrid::COMBO;
					WaterGrid.cellFormat[3][r] = testgrid::NORMAL;

					WaterGrid.cellEdit[4][r] = testgrid::EDIT;
					WaterGrid.cellFormat[4][r] = testgrid::NORMAL;

					WaterGrid.cellEdit[5][r] = testgrid::EDIT;
					WaterGrid.cellFormat[5][r] = testgrid::NORMAL;

					WaterGrid.cellEdit[6][r] = testgrid::COMBO;
					WaterGrid.cellFormat[6][r] = testgrid::NORMAL;

					WaterGrid.cellEdit[7][r] = testgrid::EDIT;
					WaterGrid.cellFormat[7][r] = testgrid::NORMAL;

					if (r > (nComp * 2))

						WaterGrid.cellEdit[2][r] = testgrid::READ_ONLY;

					OilGrid.cellEdit[3][r] = testgrid::COMBO;
					OilGrid.cellFormat[3][r] = testgrid::NORMAL;

					OilGrid.cellEdit[4][r] = testgrid::EDIT;
					OilGrid.cellFormat[4][r] = testgrid::NORMAL;

					OilGrid.cellEdit[5][r] = testgrid::EDIT;
					OilGrid.cellFormat[5][r] = testgrid::NORMAL;

					OilGrid.cellEdit[6][r] = testgrid::COMBO;
					OilGrid.cellFormat[6][r] = testgrid::NORMAL;

					OilGrid.cellEdit[7][r] = testgrid::EDIT;
					OilGrid.cellFormat[7][r] = testgrid::NORMAL;

					if (r > (nComp * 2))

						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				if ( waterComp[numC].Role > 0 && waterComp[numC].Role < 10 )
				{
					if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
					{
						sItem = "Freundlich";
						
						WaterGrid.colS[3][r] = sItem;

						WaterGrid.colS[4][r-1] = "k";

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );
						
						WaterGrid.colS[4][r] = sItem;

						WaterGrid.colS[5][r-1] = "n";

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].n );

						WaterGrid.colS[5][r] = sItem;
					}

					if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
					{
						sItem = "Langmuir";
						
						WaterGrid.colS[3][r] = sItem;

						WaterGrid.colS[4][r-1] = "a";

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );

						WaterGrid.colS[4][r] = sItem;
						
						WaterGrid.colS[5][r-1] = "b";

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].n );

						WaterGrid.colS[5][r] = sItem;
					}

					if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
					{
						sItem = "IsoTable";
						
						WaterGrid.colS[3][r] = sItem;

						WaterGrid.Merge(4, 5, r, r);

						WaterGrid.Merge(4, 5, r-1, r-1);
						
						WaterGrid.cellEdit[4][r] = testgrid::ISO;

						WaterGrid.cellFormat[4][r-1] = testgrid::HEADER;
						
						WaterGrid.cellFormat[5][r-1] = testgrid::NORMAL;

					//	WaterGrid.colS[4][r-1] = "";

						WaterGrid.colS[4][r-1] = "Iso Table";
						
						WaterGrid.SetCellText(4, r, wIso[int(r/(2 * nComp))][numC].IsoTabFile);
						
					//	WaterGrid.colS[4][r] = "";

					}

					if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
					{
						sItem = "Equilibrium";
						
						WaterGrid.colS[6][r] = sItem;

						WaterGrid.cellEdit[7][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[7][r] = testgrid::GRAY;
						WaterGrid.colS[7][r] = "";
						
					}
					
					if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
					{
						sItem = "Kinetic";
						
						WaterGrid.colS[6][r] = sItem;

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
						WaterGrid.colS[7][r] = sItem;

					}
					
					if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 2 )
					{
						sItem = "Kinetic2";
						
						WaterGrid.colS[6][r] = sItem;

						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
						WaterGrid.colS[7][r] = sItem;

					}
					
					if ( wIso[int(r/(2 * nComp))][numC].TypeAds == -1 )
					{	
						sItem = "No Adso";						
						WaterGrid.colS[6][r] = sItem;

						WaterGrid.colS[3][r] = " ";
						WaterGrid.cellEdit[3][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[3][r] = testgrid::GRAY;
						
						WaterGrid.colS[4][r] = " ";
						WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[4][r] = testgrid::GRAY;
						
						WaterGrid.colS[5][r] = " ";
						WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[5][r] = testgrid::GRAY;
						
						WaterGrid.colS[7][r] = " ";
						WaterGrid.cellEdit[7][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[7][r] = testgrid::GRAY;

					}
					// OIL GRID
		
					if ( oIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
					{
						sItem = "Freundlich";
						
						OilGrid.colS[3][r] = sItem;

						OilGrid.colS[4][r-1] = "k";

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].k );
						
						OilGrid.colS[4][r] = sItem;

						OilGrid.colS[5][r-1] = "n";

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].n );

						OilGrid.colS[5][r] = sItem;
					}

					if ( oIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
					{
						sItem = "Langmuir";
						
						OilGrid.colS[3][r] = sItem;

						OilGrid.colS[4][r-1] = "a";

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].k );

						OilGrid.colS[4][r] = sItem;
						
						OilGrid.colS[5][r-1] = "b";

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].n );

						OilGrid.colS[5][r] = sItem;
					}

					if ( oIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
					{
						sItem = "IsoTable";
						
						OilGrid.colS[3][r] = sItem;

						OilGrid.Merge(4, 5, r, r);

						OilGrid.Merge(4, 5, r-1, r-1);
						
						OilGrid.cellEdit[4][r] = testgrid::ISO;

						OilGrid.cellFormat[4][r-1] = testgrid::HEADER;
						
						OilGrid.cellFormat[5][r-1] = testgrid::NORMAL;

					//	OilGrid.colS[4][r-1] = "";

						OilGrid.colS[4][r-1] = "Iso Table";
						
						OilGrid.SetCellText(4, r, oIso[int(r/(2 * nComp))][numC].IsoTabFile);
						
					//	OilGrid.colS[4][r] = "";

					}

					if ( oIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
					{
						sItem = "Equilibrium";
						
						OilGrid.colS[6][r] = sItem;

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].r2 );
						
						OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
						OilGrid.cellFormat[7][r] = testgrid::GRAY;
						OilGrid.colS[7][r] = " ";
						
					}
					
					if ( oIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
					{
						sItem = "Kinetic";
						
						OilGrid.colS[6][r] = sItem;

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].r2 );
						OilGrid.colS[7][r] = sItem;

					}

					if ( oIso[int(r/(2 * nComp))][numC].TypeAds == 2 )
					{
						sItem = "Kinetic2";
						
						OilGrid.colS[6][r] = sItem;

						sItem.Format("%.4g", oIso[int(r/(2 * nComp))][numC].r2 );
						OilGrid.colS[7][r] = sItem;

					}

					if ( oIso[int(r/(2 * nComp))][numC].TypeAds == -1 )
					{	
						sItem = "No Adso";						
						OilGrid.colS[6][r] = sItem;

						OilGrid.colS[3][r] = " ";
						OilGrid.cellEdit[3][r]= testgrid::READ_ONLY;
						OilGrid.cellFormat[3][r] = testgrid::GRAY;
						
						OilGrid.colS[4][r] = " ";
						OilGrid.cellEdit[4][r]= testgrid::READ_ONLY;
						OilGrid.cellFormat[4][r] = testgrid::GRAY;
						
						OilGrid.colS[5][r] = " ";
						OilGrid.cellEdit[5][r]= testgrid::READ_ONLY;
						OilGrid.cellFormat[5][r] = testgrid::GRAY;
						
						OilGrid.colS[7][r] = " ";
						OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
						OilGrid.cellFormat[7][r] = testgrid::GRAY;

					}
				}

				OilGrid.cellEdit[2][r]= testgrid::READ_ONLY;
				OilGrid.cellFormat[2][r] = testgrid::GRAY;
				
				if ( waterComp[numC].Role == 5 ) // Ca
				{
					WaterGrid.colS[2][r] = "Ca";
					OilGrid.colS[2][r] = "Ca";
					
					WaterGrid.colS[3][r] = " ";
					WaterGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[3][r] = testgrid::GRAY;

					WaterGrid.colS[4][r] = " ";
					WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[4][r] = testgrid::GRAY;

					WaterGrid.colS[5][r] = " ";
					WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[5][r] = testgrid::GRAY;

					OilGrid.colS[3][r] = " ";
					OilGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[3][r] = testgrid::GRAY;

					OilGrid.colS[4][r] = " ";
					OilGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[4][r] = testgrid::GRAY;

					OilGrid.colS[5][r] = " ";
					OilGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[5][r] = testgrid::GRAY;
				
					OilGrid.colS[6][r] = " ";
					OilGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[6][r] = testgrid::GRAY;
				
					OilGrid.colS[7][r] = " ";
					OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[7][r] = testgrid::GRAY;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}
				
				if ( waterComp[numC].Role == 6 ) // Na
				{
					WaterGrid.colS[2][r] = "Na";
					OilGrid.colS[2][r] = "Na";

					OilGrid.colS[3][r] = " ";
					OilGrid.cellEdit[3][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[3][r] = testgrid::GRAY;

					OilGrid.colS[4][r] = " ";
					OilGrid.cellEdit[4][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[4][r] = testgrid::GRAY;

					OilGrid.colS[5][r] = " ";
					OilGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[5][r] = testgrid::GRAY;
				
					OilGrid.colS[6][r] = " ";
					OilGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[6][r] = testgrid::GRAY;
				
					OilGrid.colS[7][r] = " ";
					OilGrid.cellEdit[7][r]= testgrid::READ_ONLY;
					OilGrid.cellFormat[7][r] = testgrid::GRAY;

					if ( r > (nComp * 2) )
						
						OilGrid.cellEdit[2][r] = testgrid::READ_ONLY;
				}

				// MASS TRANSFER GRID 	
				for ( c = 0; c < nComp; c++)
				{

					if ( waterComp[c].MTrans == 0 )
					{
						sItem = "None";
						MassTransGrid.colS[1][c+1] = sItem;
						MassTransGrid.cellEdit[2][c+1] = testgrid::READ_ONLY;
						MassTransGrid.cellFormat[2][c+1] = testgrid::GRAY;
						MassTransGrid.colS[2][c+1] = "";
					}

					if ( waterComp[c].MTrans == 1 )
					{
						sItem = "Equ";
						MassTransGrid.colS[1][c+1] = sItem;
						sItem.Format("%.4g", waterComp[c].K );
						MassTransGrid.colS[2][c+1] = sItem;
					}
				}
			
			}
			else // Single Phase
			{
				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 1 )
				{	
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "Freundlich";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.colS[3][r-1] = "k";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );
					
					WaterGrid.colS[3][r] = sItem;

					WaterGrid.colS[4][r-1] = "n";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].n );

					WaterGrid.colS[4][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 2 )
				{
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "Langmuir";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.colS[3][r-1] = "a";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );

					WaterGrid.colS[3][r] = sItem;
					
					WaterGrid.colS[4][r-1] = "b";

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].k );

					WaterGrid.colS[4][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 3 )
				{	
					WaterGrid.colS[1][r] = "Adsorption";

					sItem = "IsoTable";
					
					WaterGrid.colS[2][r] = sItem;

					WaterGrid.Merge(3, 4, r, r);

					WaterGrid.Merge(3, 4, r-1, r-1);
					
					WaterGrid.cellEdit[3][r] = testgrid::ISO;

					WaterGrid.cellFormat[3][r-1] = testgrid::HEADER;
					
					WaterGrid.cellFormat[4][r-1] = testgrid::NORMAL;

				//	WaterGrid.colS[4][r-1] = "";

					WaterGrid.colS[3][r-1] = "Iso Table";
					
					WaterGrid.SetCellText(3, r, wIso[int(r/(2 * nComp))][numC].IsoTabFile);
					
				//	WaterGrid.colS[4][r] = "";

				}

				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 0 )
				{
					sItem = "Equilibrium";
					
					WaterGrid.colS[5][r] = sItem;
		
					WaterGrid.cellEdit[6][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[6][r] = testgrid::GRAY;					
					WaterGrid.colS[6][r] = "";
				}
				
				if ( wIso[int(r/(2 * nComp))][numC].TypeAds == 1 )
				{
					sItem = "Kinetic";
					
					WaterGrid.colS[5][r] = sItem;

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r2 );
					WaterGrid.colS[6][r] = sItem;
				}

				if ( wIso[int(r/(2 * nComp))][numC].IsoTyp == 4 )
				{
					WaterGrid.colS[1][r] = "Precipitation";

					WaterGrid.colS[3][r-1] = "Cps1";
					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Cps1);
					WaterGrid.colS[3][r] = sItem;
					
					if ( kTemp )
					{	
						WaterGrid.colS[4][r-1] = "Cps2";
						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Cps2 );
						WaterGrid.colS[4][r] = sItem;
					
						WaterGrid.colS[2][r-1] = "Cr.Temp";
						sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].Tpc);
						WaterGrid.colS[2][r] = sItem;
						WaterGrid.cellEdit[2][r] = testgrid::EDIT;
					}
					else
					{
						WaterGrid.colS[4][r-1] = "Cps2";
						WaterGrid.cellEdit[4][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[4][r] = testgrid::GRAY;	
						WaterGrid.colS[4][r] = "";
					
						WaterGrid.colS[2][r-1] = "Cr.Temp";
						WaterGrid.cellEdit[2][r]= testgrid::READ_ONLY;
						WaterGrid.cellFormat[2][r] = testgrid::GRAY;	
						WaterGrid.colS[2][r] = "";
					}
					
					WaterGrid.colS[5][r-1]= "Prep.Type";
					WaterGrid.cellEdit[5][r]= testgrid::READ_ONLY;
					WaterGrid.cellFormat[5][r] = testgrid::GRAY;	

					sItem.Format("%.4g", wIso[int(r/(2 * nComp))][numC].r4 );
					WaterGrid.colS[6][r] = sItem;
				}

			}
			
			numC++;	
			
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

		}
	
	}
	
}

void PropPage2::UpdateFromEdits()
{	
	CString sItem;
	
	int nTotal = nLay * nComp * 2,
		 numC = 0,
		 r,
		 c,
		 l;
	

	for ( r = 0; r < nTotal; r++)
	{

		if( r%2 == 1 )
		{	
			// WATER GRID
			if (nDisabled) // Two Phase 
			{
				if ( WaterGrid.colS[3][r] == "Freundlich" )
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 1;

					wIso[int(r/(2 * nComp))][numC].k = atof(WaterGrid.colS[4][r]);

					wIso[int(r/(2 * nComp))][numC].n = atof(WaterGrid.colS[5][r]);

				}

				if ( WaterGrid.colS[3][r] == "Langmuir")
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 2;

					wIso[int(r/(2 * nComp))][numC].k = atof(WaterGrid.colS[4][r]);

					wIso[int(r/(2 * nComp))][numC].n = atof(WaterGrid.colS[5][r]);
				}

				if ( WaterGrid.colS[3][r] == "IsoTable" )
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 3;
					
					wIso[int(r/(2 * nComp))][numC].IsoTabFile = WaterGrid.colS[4][r];
					
					FILE *fileIso;

					if ( (fileIso = fopen(PathName+CString("Isotherms\\")+wIso[int(r/(2 * nComp))][numC].IsoTabFile,"r")) == NULL )
					{   
						AfxMessageBox("File Not Opened Water");
					}
					else
					{  
						char buffer[128];
						int NIso;

						/* Set pointer to beginning of file: */
						fseek( fileIso, 0L, SEEK_SET );

						// validate the SQUEEZE data file and check the version
						fgets(buffer,128,fileIso);

						fscanf(fileIso,"%d \n",&NIso);
						
						wIso[int(r/(2 * nComp))][numC].nIso = NIso;

						fgets(buffer,128,fileIso);
				
						for( int i = 0 ; i < NIso ; i++)
							
							fscanf(fileIso,"%lf %lf \n",&wIso[int(r/(2 * nComp))][numC].Ciso[i], 
								&wIso[int(r/(2 * nComp))][numC].ADiso[i] );
										
						fclose( fileIso );

					}

				}

				if ( WaterGrid.colS[2][r] == "Tracer" )
				{
					waterComp[numC].Role = 0;
/*
					wIso[int(r/(2 * nComp))][numC].TypeAds = 2;

					oilCompAdsIso[int(r/(2 * nComp))][numC].TypeAds = 2;
*/				}
				
				if ( WaterGrid.colS[2][r] == "Inhib." )
				{
					waterComp[numC].Role = 1;
				}

				if ( WaterGrid.colS[2][r] == "Visco." )
				{
					waterComp[numC].Role = 2;
				}

				if ( WaterGrid.colS[2][r] == "Blocker" )
				{
					waterComp[numC].Role = 3;
				}
				
				if ( WaterGrid.colS[2][r] == "Surfact" )
				{
					waterComp[numC].Role = 4;
				}

				if ( WaterGrid.colS[2][r] == "Ca" )
				{
					waterComp[numC].Role = 5;
				}
				
				if ( WaterGrid.colS[2][r] == "Na" )
				{
					waterComp[numC].Role = 6;
				}

				if ( WaterGrid.colS[2][r] == "Bridging" )
				{
					waterComp[numC].Role = 7;
				}

				if ( WaterGrid.colS[2][r] == "Wax" )
				{
					waterComp[numC].Role = 8;
				}

				if (WaterGrid.colS[2][r] == "SI_FD")
				{
					waterComp[numC].Role = 9;
				}

				if ( WaterGrid.colS[6][r] == "Equilibrium" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 0;			
				}
				
				if ( WaterGrid.colS[6][r] == "Kinetic" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 1;
				
					wIso[int(r/(2 * nComp))][numC].r2 = atof(WaterGrid.colS[7][r]);
				}
				
				if ( WaterGrid.colS[6][r] == "Kinetic2" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 2;
				
					wIso[int(r/(2 * nComp))][numC].r2 = atof(WaterGrid.colS[7][r]);
				}
				
				if ( WaterGrid.colS[6][r] == "No Adso")
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = -1;
				}

				// OIL GRID
				if ( OilGrid.colS[3][r] == "Freundlich" )
				{
					oIso[int(r/(2 * nComp))][numC].IsoTyp = 1;

					oIso[int(r/(2 * nComp))][numC].k = atof(OilGrid.colS[4][r]);

					oIso[int(r/(2 * nComp))][numC].n = atof(OilGrid.colS[5][r]);

				}

				if ( OilGrid.colS[3][r] == "Langmuir")
				{
					oIso[int(r/(2 * nComp))][numC].IsoTyp = 2;

					oIso[int(r/(2 * nComp))][numC].k = atof(OilGrid.colS[4][r]);

					oIso[int(r/(2 * nComp))][numC].n = atof(OilGrid.colS[5][r]);
				}

				if ( OilGrid.colS[3][r] == "IsoTable" )
				{
					oIso[int(r/(2 * nComp))][numC].IsoTyp = 3;
					
					oIso[int(r/(2 * nComp))][numC].IsoTabFile = OilGrid.colS[4][r];

					FILE *fileIso;

					if ( (fileIso = fopen(PathName+CString("Isotherms\\")+oIso[int(r/(2 * nComp))][numC].IsoTabFile,"r")) == NULL )
					{   
						AfxMessageBox("File Not Opened");
					}
					else
					{  
						char buffer[128];
						int NIso;

						/* Set pointer to beginning of file: */
						fseek( fileIso, 0L, SEEK_SET );

						// validate the SQUEEZE data file and check the version
						fgets(buffer,128,fileIso);

						fscanf(fileIso,"%d \n",&NIso);
						
						oIso[int(r/(2 * nComp))][numC].nIso = NIso;

						fgets(buffer,128,fileIso);
				
						for( int i = 0 ; i < NIso ; i++)
							
							fscanf(fileIso,"%lf %lf \n",&oIso[int(r/(2 * nComp))][numC].Ciso[i], 
								&oIso[int(r/(2 * nComp))][numC].ADiso[i] );
										
						fclose( fileIso );

					}

				}

				if ( OilGrid.colS[6][r] == "Equilibrium" )
				{
					oIso[int(r/(2 * nComp))][numC].TypeAds = 0;			
				}
				
				if ( OilGrid.colS[6][r] == "Kinetic" )
				{
					oIso[int(r/(2 * nComp))][numC].TypeAds = 1;
				
					oIso[int(r/(2 * nComp))][numC].r2 = atof(OilGrid.colS[7][r]);
				}
				
				if ( OilGrid.colS[6][r] == "Kinetic2" )
				{
					oIso[int(r/(2 * nComp))][numC].TypeAds = 2;
				
					oIso[int(r/(2 * nComp))][numC].r2 = atof(OilGrid.colS[7][r]);
				}
				
				if ( OilGrid.colS[6][r] == "No Adso")
				{
					oIso[int(r/(2 * nComp))][numC].TypeAds = -1;
				}
				
			}
			else // Single Phase 
			{
				if ( WaterGrid.colS[1][r] == "Precipitation" )
				{	
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 4;

					wIso[int(r/(2 * nComp))][numC].Tpc = atof(WaterGrid.colS[2][r]);

					wIso[int(r/(2 * nComp))][numC].Cps1 = atof(WaterGrid.colS[3][r]);

					wIso[int(r/(2 * nComp))][numC].Cps2 = atof(WaterGrid.colS[4][r]);
					
					wIso[int(r/(2 * nComp))][numC].r4 = atof(WaterGrid.colS[6][r]);
				}
				
				if ( WaterGrid.colS[2][r] == "Freundlich" )
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 1;

					wIso[int(r/(2 * nComp))][numC].k = atof(WaterGrid.colS[3][r]);

					wIso[int(r/(2 * nComp))][numC].n = atof(WaterGrid.colS[4][r]);

				}

				if ( WaterGrid.colS[2][r] == "Langmuir")
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 2;

					wIso[int(r/(2 * nComp))][numC].k = atof(WaterGrid.colS[3][r]);

					wIso[int(r/(2 * nComp))][numC].n = atof(WaterGrid.colS[4][r]);
				}

				if ( WaterGrid.colS[2][r] == "IsoTable" )
				{
					wIso[int(r/(2 * nComp))][numC].IsoTyp = 3;
					
					wIso[int(r/(2 * nComp))][numC].IsoTabFile = WaterGrid.colS[3][r];
					
					FILE *fileIso;

					if ( (fileIso = fopen(PathName+CString("Isotherms\\")+wIso[int(r/(2 * nComp))][numC].IsoTabFile,"r")) == NULL )
					{   
						AfxMessageBox("File Not Opened Water");
					}
					else
					{  
						char buffer[128];
						int NIso;

						/* Set pointer to beginning of file: */
						fseek( fileIso, 0L, SEEK_SET );

						// validate the SQUEEZE data file and check the version
						fgets(buffer,128,fileIso);

						fscanf(fileIso,"%d \n",&NIso);
						
						wIso[int(r/(2 * nComp))][numC].nIso = NIso;

						fgets(buffer,128,fileIso);
				
						for( int i = 0 ; i < NIso ; i++)
							
							fscanf(fileIso,"%lf %lf \n",&wIso[int(r/(2 * nComp))][numC].Ciso[i], 
								&wIso[int(r/(2 * nComp))][numC].ADiso[i] );
										
						fclose( fileIso );

					}

				}

				if ( WaterGrid.colS[5][r] == "Equilibrium" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 0;			
				}
				
				if ( WaterGrid.colS[5][r] == "Kinetic" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 1;
				
					wIso[int(r/(2 * nComp))][numC].r2 = atof(WaterGrid.colS[6][r]);
				}
				
				if ( WaterGrid.colS[5][r] == "Kinetic2" )
				{
					wIso[int(r/(2 * nComp))][numC].TypeAds = 2;
				
					wIso[int(r/(2 * nComp))][numC].r2 = atof(WaterGrid.colS[6][r]);
				}

			}

			numC++;	
			
			if( r%(2*nComp) == (2*nComp)-1 ) numC = 0;

		}
	
	}
	
	// Update Mass Transfer Grid 	
	
	for ( c = 0; c < nComp; c++)
	{
		if ( MassTransGrid.colS[1][c+1] == "None" )
		{
			waterComp[c].MTrans = 0;
		}

		if ( MassTransGrid.colS[1][c+1] == "Equ" )
		{
			waterComp[c].MTrans = 1;
			waterComp[c].K = atof(MassTransGrid.colS[2][c+1]);
		}
	}
	
	if ( m_All_Layers.GetCheck() )
	{	
		IsoSpec = 0;

		for( l = 0 ; l < TempnLay ; l++ )
		{	
			for ( c = 0; c < nComp; c++)
			{
				wIso[l][c] = wIso[0][c];

				oIso[l][c] = oIso[0][c];
			}
		}
	}

	else IsoSpec = 1;
	
	if (nDisabled) // Two Phase 
	{
		for ( c = 0; c < nComp; c++)
		{	
/*			WaterGrid.VisDlg.waterComp[c] = waterComp[c];
			WaterGrid.VisDlg.oilComp[c] = oilComp[c];

			WaterGrid.VisDlg.waterComp[c].Role = waterComp[c].Role;
			WaterGrid.VisDlg.oilComp[c].Role = oilComp[c].Role;

			WaterGrid.VisDlg.waterComp[c].MTrans = waterComp[c].MTrans;
			WaterGrid.VisDlg.waterComp[c].K = waterComp[c].K;

			WaterGrid.VisDlg.oilComp[c].MTrans = waterComp[c].MTrans;
			WaterGrid.VisDlg.oilComp[c].K = waterComp[c].K;

			waterComp[c] = WaterGrid.VisDlg.waterComp[c];
			oilComp[c] = WaterGrid.VisDlg.oilComp[c];*/
		}
	}
}


void PropPage2::OnAllLayers()
{	
	int check = m_All_Layers.GetCheck();

	UpdateFromEdits();

	if( check )
	{
		nLay = 1;
		
		WaterGrid.vScrollPos = 0;
	}
	else
		
		nLay = TempnLay;

	UpdateGrids();
	
	if (!nDisabled)
	{	
		if( iModel == 5 )
			
			WaterGrid.Disabled();
		
		OilGrid.Disabled();
		MassTransGrid.Disabled();
	}

	Invalidate(FALSE);
}

LRESULT PropPage2::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	UpdateFromEdits();
	UpdateControls();
	Invalidate(FALSE);
//	UpdateFromListCtrl();
	return 0;
}

LRESULT PropPage2::OnEditLostFocus(WPARAM w, LPARAM l)
{	
	UpdateFromEdits();
	UpdateControls();
//	AfxMessageBox("Edit Lost Focus");
	if (!nDisabled)
	{	
		OilGrid.Disabled();
		MassTransGrid.Disabled();
	}
	return 0;
}

LRESULT PropPage2::OnBoxLostFocus(WPARAM w, LPARAM l)
{		
/*	theBox.ShowWindow(FALSE);
	CString sBox;
	theBox.GetWindowText(sBox);
*/	
//	AfxMessageBox("Box Lost Focus");
	
/*	if ((sBox.IsEmpty() == FALSE)&&(boxHasFocus))
	{
		sBox.TrimRight();
		colS[cellC][cellR] = sBox;
		boxHasFocus = false;
		
		if ( sBox == "Isotherm")
		{	
			Merge(cellC+1, cellC+2, cellR, cellR);

			Merge(cellC+1, cellC+2, cellR-1, cellR-1);
			
			cellEdit[cellC+1][cellR] = ISO;

			cellFormat[cellC+2][cellR-1] = NORMAL;

			OpenIsothermTable();
			
		}

		if ( sBox == "Freundlich" || sBox == "Langmuir" )
		{
			UnMerge(cellC+1, cellC+2, cellR, cellR);

			UnMerge(cellC+1, cellC+2, cellR-1, cellR-1);
			
			cellEdit[cellC+1][cellR] = EDIT;
		//	colS[cellC+1][cellR] = "";

			cellEdit[cellC+2][cellR] = EDIT;
		//	colS[cellC+2][cellR] = "";
			
			cellFormat[cellC+2][cellR-1] = HEADER;

			if( sBox == "Freundlich" )
			{
				colS[cellC+1][cellR-1] = "k";
				colS[cellC+2][cellR-1] = "n";
			}
			
			if( sBox == "Langmuir" )
			{
				colS[cellC+1][cellR-1] = "a";
				colS[cellC+2][cellR-1] = "b";
			}

			Invalidate(FALSE);
		}
		
		if ( sBox == "Equilibrium" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			colS[cellC+1][cellR] = "";
			InvalidateRect(NULL);
		}
		
		if ( sBox == "Kinetic" )
		{	
			cellEdit[cellC+1][cellR] = EDIT;
			cellFormat[cellC+1][cellR] = NORMAL;
			colS[cellC+1][cellR] = "";
			InvalidateRect(NULL);
		}

		CWnd* pWnd = 0;
		pWnd = GetParent();
		if (pWnd != NULL)
		{
			pWnd->PostMessage(UDM_LIST_CTRL_UPDATE);
		}
//
	}
	
*/	
	return 0;
}


