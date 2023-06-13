// BlockerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "BlockerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// BlockerDlg dialog


BlockerDlg::BlockerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(BlockerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(BlockerDlg)
	nLay = 1;
	check_FD = false;
	//}}AFX_DATA_INIT
}


void BlockerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(BlockerDlg)
	DDX_Control(pDX, IDC_STATIC_WATER_RF, WatRFGrid);
	if (!check_FD)
	DDX_Control(pDX, IDC_STATIC_OIL_RF, OilRFGrid);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(BlockerDlg, CDialog)
	//{{AFX_MSG_MAP(BlockerDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// BlockerDlg message handlers

BOOL BlockerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int r;

	CString sItem;
	
	TableGrid TempLayGrid(nLay+1,4);

	WatRFGrid = OilRFGrid = TempLayGrid;
	
	if (check_FD)
	{
		WatRFGrid.SetCellText(0, 0, "Lay");
		WatRFGrid.SetColWidth(0, 45);
		WatRFGrid.SetCellText(1, 0, "Max RF");
		WatRFGrid.SetColWidth(1, 67);
		WatRFGrid.SetCellText(2, 0, "Min RF");
		WatRFGrid.SetColWidth(2, 45);
		WatRFGrid.SetCellText(3, 0, "nPower");
		WatRFGrid.SetColWidth(3, 45);

		WatRFGrid.SetRowHeight(0, 20);

		WatRFGrid.SetColEdit(0, TableGrid::READ_ONLY);
		WatRFGrid.SetRowEdit(0, TableGrid::READ_ONLY);

		WatRFGrid.SetColFormat(0, TableGrid::HEADER);
		WatRFGrid.SetRowFormat(0, TableGrid::HEADER);

		WatRFGrid.CalculateColRowPos();
	}
	else
	{
		WatRFGrid.SetCellText(0, 0, "Lay");
		WatRFGrid.SetColWidth(0, 45);
		WatRFGrid.SetCellText(1, 0, "Max RFw");
		WatRFGrid.SetColWidth(1, 67);
		WatRFGrid.SetCellText(2, 0, "Min RFw");
		WatRFGrid.SetColWidth(2, 45);
		WatRFGrid.SetCellText(3, 0, "nPower");
		WatRFGrid.SetColWidth(3, 45);

		WatRFGrid.SetRowHeight(0, 20);

		WatRFGrid.SetColEdit(0, TableGrid::READ_ONLY);
		WatRFGrid.SetRowEdit(0, TableGrid::READ_ONLY);

		WatRFGrid.SetColFormat(0, TableGrid::HEADER);
		WatRFGrid.SetRowFormat(0, TableGrid::HEADER);

		WatRFGrid.CalculateColRowPos();

		OilRFGrid.SetCellText(0, 0, "Lay");
		OilRFGrid.SetColWidth(0, 45);
		OilRFGrid.SetCellText(1, 0, "Max RFo");
		OilRFGrid.SetColWidth(1, 67);
		OilRFGrid.SetCellText(2, 0, "Min RFo");
		OilRFGrid.SetColWidth(2, 45);
		OilRFGrid.SetCellText(3, 0, "nPower");
		OilRFGrid.SetColWidth(3, 45);

		OilRFGrid.SetRowHeight(0, 20);

		OilRFGrid.SetColEdit(0, TableGrid::READ_ONLY);
		OilRFGrid.SetRowEdit(0, TableGrid::READ_ONLY);

		OilRFGrid.SetColFormat(0, TableGrid::HEADER);
		OilRFGrid.SetRowFormat(0, TableGrid::HEADER);

		OilRFGrid.CalculateColRowPos();
	}

	for ( r = 0 ; r < nLay ; r++ )
	{
		sItem.Format("%d", r + 1);
		WatRFGrid.colS[0][r+1] = sItem;
		OilRFGrid.colS[0][r+1] = sItem;
	}
	
	UpdateControls();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void BlockerDlg::UpdateControls()
{	
	int r;

	CString sn;

/*	for ( r = 0 ; r < nLay ; r++ )
	{
		sn.Format("%.4g", rfDlg[r].RFwMax);
		WatRFGrid.colS[1][r+1] = sn;

		sn.Format("%.4g", rfDlg[r].RFwMin);
		WatRFGrid.colS[2][r+1] = sn;
		
		sn.Format("%.4g", rfDlg[r].nWat);
		WatRFGrid.colS[3][r+1] = sn;

		sn.Format("%.4g", rfDlg[r].RFoMax);
		OilRFGrid.colS[1][r+1] = sn;

		sn.Format("%.4g", rfDlg[r].RFoMin);
		OilRFGrid.colS[2][r+1] = sn;
		
		sn.Format("%.4g", rfDlg[r].nOil);
		OilRFGrid.colS[3][r+1] = sn;
	}
*/
	for ( r = 0 ; r < nLay ; r++ )
	{
		sn.Format("%.4g", rfDlg.RFwMax);
		WatRFGrid.colS[1][r+1] = sn;

		sn.Format("%.4g", rfDlg.RFwMin);
		WatRFGrid.colS[2][r+1] = sn;
		
		sn.Format("%.4g", rfDlg.nWat);
		WatRFGrid.colS[3][r+1] = sn;

		sn.Format("%.4g", rfDlg.RFoMax);
		OilRFGrid.colS[1][r+1] = sn;

		sn.Format("%.4g", rfDlg.RFoMin);
		OilRFGrid.colS[2][r+1] = sn;
		
		sn.Format("%.4g", rfDlg.nOil);
		OilRFGrid.colS[3][r+1] = sn;
	}
}

void BlockerDlg::UpdateFromEdits()
{	
	int r;

/*	for ( r = 0 ; r < nLay ; r++ )
	{
		rfDlg[r].RFwMax = atof(WatRFGrid.colS[1][r+1]);

		rfDlg[r].RFwMin = atof(WatRFGrid.colS[2][r+1]);
	
		rfDlg[r].nWat = atof(WatRFGrid.colS[3][r+1]);

		rfDlg[r].RFoMax = atof(OilRFGrid.colS[1][r+1]);

		rfDlg[r].RFoMin = atof(OilRFGrid.colS[2][r+1]);
		
		rfDlg[r].nOil = atof(OilRFGrid.colS[3][r+1]);
	}
*/
	for ( r = 0 ; r < nLay ; r++ )
	{
		rfDlg.RFwMax = atof(WatRFGrid.colS[1][r+1]);

		rfDlg.RFwMin = atof(WatRFGrid.colS[2][r+1]);

		rfDlg.nWat = atof(WatRFGrid.colS[3][r+1]);

		rfDlg.RFoMax = atof(OilRFGrid.colS[1][r+1]);

		rfDlg.RFoMin = atof(OilRFGrid.colS[2][r+1]);
		
		rfDlg.nOil = atof(OilRFGrid.colS[3][r+1]);

	}

}

void BlockerDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateFromEdits();

	CDialog::OnOK();
}