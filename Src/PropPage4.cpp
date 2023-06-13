// PropPage4.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage4.h"
#include "Sheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PropPage4 dialog


IMPLEMENT_DYNCREATE(PropPage4, CPropertyPage)

PropPage4::PropPage4() : CPropertyPage(PropPage4::IDD)
{
	//{{AFX_DATA_INIT(PropPage3)
	
	//}}AFX_DATA_INIT
	int i;

	TableGrid TempGrid(4,2);

	OldFileGrid = TempGrid;

	CString sEdit;
	
	OldFileGrid.bScroll = TRUE;

	for( i = 0 ; i < MaxFileNm/2 ; i++ )
	{
		OldFileGrid.SetRowEdit(i, TableGrid::TABLE);
		
		sEdit.Format(_T("Click here %d"), i+1);
		OldFileGrid.colS[0][i] = sEdit;
		sEdit.Format(_T("Click here %d"), i+5);
		OldFileGrid.colS[1][i] = sEdit;	
	}

	for( i = 0 ; i < nOlds ; i++ )
	{	
		sEdit.Format(_T("%s"),FileNm[i]);
		
		if( i >= MaxFileNm/2 )
			
			OldFileGrid.colS[1][i%(MaxFileNm/2)] = sEdit;
		
		else

			OldFileGrid.colS[0][i] = sEdit;
	}

	nDisabled = 1;

	
}

PropPage4::~PropPage4()
{
}


void PropPage4::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PropPage4)
	DDX_Radio(pDX, IDC_RC_TIME_LIN, m_Return_Curve_Type);
	DDX_Check(pDX, IDC_SCREEN_DISPLAY, m_Bool_Screen_Display);
	DDX_Check(pDX, IDC_ISCON, m_Bool_IsCon);
	DDX_Check(pDX, IDC_ISTMP, m_Bool_IsTmp);
	DDX_Control(pDX, IDC_STATIC_OLDFILES, OldFileGrid);
	DDX_Control(pDX, IDC_STATIC_NFREQ, m_Edit_Num_Freq);
	DDX_Control(pDX, IDC_STATIC_TOLITR, m_Edit_TolItr);
	DDX_Control(pDX, IDC_STATIC_CLIFE1, m_Edit_CLife[0]);
	DDX_Control(pDX, IDC_STATIC_CLIFE2, m_Edit_CLife[1]);
	DDX_Control(pDX, IDC_STATIC_CLIFE3, m_Edit_CLife[2]);
	DDX_Control(pDX, IDC_STATIC_CLIFE4, m_Edit_CLife[3]);
	DDX_Control(pDX, IDC_STATIC_CLIFE5, m_Edit_CLife[4]);
	DDX_Control(pDX, IDC_STATIC_NITERS, m_Edit_Num_Iters);
	DDX_Control(pDX, IDC_STATIC_FIELDTABLE, m_Edit_FieldTable);
	DDX_Control(pDX, IDC_STATIC_FILENUM, m_Edit_FileNum);
	DDX_Control(pDX, IDC_BUTTON_GRAPHPARA, m_bGraphPara);
	DDX_Control(pDX, IDC_SHOW_TABLE, m_bShowTable);
	DDX_Control(pDX, IDC_INPUT_TABLE, m_bInputTable);
	DDX_Control(pDX, IDC_REMOVE_FILENUM, m_bRemoveFileNum);
	DDX_Control(pDX, IDC_STATIC_GRPA, m_Static_GrPa);
	DDX_Control(pDX, IDC_STATIC_RETPLOT, m_Static_RetPlot);
	DDX_Control(pDX, IDC_STATIC_RETCONC, m_Static_RetConc);
	DDX_Control(pDX, IDC_STATIC_MAXITER, m_Static_MaxIter);
	DDX_Control(pDX, IDC_STATIC_MAXTIM, m_Static_MaxTim);
	DDX_Control(pDX, IDC_STATIC_TOL, m_Static_Tol);
	DDX_Control(pDX, IDC_SCREEN_DISPLAY, m_Button_ScrDis);
	DDX_Control(pDX, IDC_RC_TIME_LIN, m_Button_TimLin);
	DDX_Control(pDX, IDC_RC_TIME_LOG, m_Button_TimLog);
	DDX_Control(pDX, IDC_RC_WATVOL_LIN, m_Button_WatLin);
	DDX_Control(pDX, IDC_RC_WATVOL_LOG, m_Button_WatLog);
	DDX_Control(pDX, IDC_STATIC_GRAFMIC, m_Edit_GrafMic);
	DDX_Control(pDX, IDC_CW, m_Check_Cw);
	DDX_Control(pDX, IDC_CO, m_Check_Co);
	DDX_Control(pDX, IDC_ADSW, m_Check_AdsW);
	DDX_Control(pDX, IDC_ADSO, m_Check_AdsO);
	DDX_Control(pDX, IDC_VISCW, m_Check_ViscW);
	DDX_Control(pDX, IDC_VISCO, m_Check_ViscO);
	DDX_Control(pDX, IDC_RFW, m_Check_RFW);
	DDX_Control(pDX, IDC_RFO, m_Check_RFO);
	DDX_Control(pDX, IDC_CHECK_OUTPUTFILE, m_Check_OutputFile);
	DDX_Control(pDX, IDC_CHECK_OUTPUTFILE2, m_Check_OutputFileTime);
	DDX_Control(pDX, IDC_SW, m_Check_SW);
	DDX_Control(pDX, IDC_FW, m_Check_FW);
	DDX_Control(pDX, IDC_RC_TIME, m_Button_Time);
	DDX_Control(pDX, IDC_RC_WATPRO, m_Button_WatPro);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PropPage4, CDialog)
	//{{AFX_MSG_MAP(PropPage4)
	ON_BN_CLICKED(IDC_BUTTON_GRAPHPARA, OnButtonGraphPara)
	ON_BN_CLICKED(IDC_SHOW_TABLE, OnButtonShowTable)
	ON_BN_CLICKED(IDC_INPUT_TABLE, OnButtonInputTable)
	ON_BN_CLICKED(IDC_REMOVE_TABLE, OnButtonRemoveTable)
	ON_BN_CLICKED(IDC_REMOVE_FILENUM, OnButtonRemoveFileNum)
	ON_BN_CLICKED(IDC_CHECK_OUTPUTFILE, OnButtonOutputFileDistance)
	ON_BN_CLICKED(IDC_CHECK_OUTPUTFILE2, OnButtonOutputFileTime)
	ON_EN_KILLFOCUS(IDD_PROPPAGE_LARGE4, OnKillfocusOutputFile)
	ON_EN_KILLFOCUS(IDC_STATIC_OLDFILES, OnKillfocusOldFiles)
	ON_BN_CLICKED(IDC_CW, OnButtonCw)
	ON_BN_CLICKED(IDC_CO, OnButtonCo)
	ON_BN_CLICKED(IDC_ADSW, OnButtonAdsW)
	ON_BN_CLICKED(IDC_ADSO, OnButtonAdsO)
	ON_BN_CLICKED(IDC_VISCW, OnButtonViscW)
	ON_BN_CLICKED(IDC_VISCO, OnButtonViscO)
	ON_BN_CLICKED(IDC_RFW, OnButtonRFW)
	ON_BN_CLICKED(IDC_RFO, OnButtonRFO)
	ON_BN_CLICKED(IDC_SW, OnButtonSw)
	ON_BN_CLICKED(IDC_FW, OnButtonSo)
	ON_BN_CLICKED(IDC_RC_TIME_LIN,OnTime_Lin)
	ON_BN_CLICKED(IDC_RC_TIME_LOG, OnTime_Log)
	ON_BN_CLICKED(IDC_RC_WATVOL_LIN, OnWatVol_Lin)
	ON_BN_CLICKED(IDC_RC_WATVOL_LOG, OnWatVol_Log)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE,OnListCtrlUpdate)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_BN_CLICKED(IDC_RC_WATPRO, &PropPage4::OnBnClickedRcWatpro)
	ON_BN_CLICKED(IDC_RC_TIME, &PropPage4::OnBnClickedRcTime)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PropPage4 message handlers

void PropPage4::OnButtonGraphPara() 
{
	// TODO: Add your control notification handler code here
	if( GPDlg.DoModal() == IDOK )
	{
//		GPDlg.UpdateFromEdits();
	}
}

void PropPage4::OnButtonShowTable() 
{
	// TODO: Add your control notification handler code here
	m_Edit_FieldTable.FieldTable.SetPathName(m_Edit_FieldTable.PathName);
	
	m_Edit_FieldTable.FieldTable.ReadFieldFileTable(m_Edit_FieldTable.FTFileName, iModel);

	m_Edit_FieldTable.FieldTable.TabGr.SetRowFormat(0, TableGrid::HEADER);
	m_Edit_FieldTable.FieldTable.TabGr.SetRowEdit(0, TableGrid::READ_ONLY);
	
	if ( Units == 0) // Metric
		
		m_Edit_FieldTable.FieldTable.TabGr.colS[0][0] = "Time\n(day/100m3)";

	if ( Units == 1) // Conventional
		
		m_Edit_FieldTable.FieldTable.TabGr.colS[0][0] = "Time\n(day/1000bbls)";
	
	m_Edit_FieldTable.FieldTable.TabGr.colS[1][0] = "Conc\n(ppm)";
	
	if (iModel == 4 || iModel == 5 )
	{
		m_Edit_FieldTable.FieldTable.TabGr.colS[0][0] = "InjVol\n(PV)";
		m_Edit_FieldTable.FieldTable.TabGr.colS[1][0] = "Conc\n(ppm)";
	}
	
	m_Edit_FieldTable.FieldTable.TabGr.rowHeight[0] = 35;

	m_Edit_FieldTable.FieldTable.TabGr.FieldData = 1;

	if ( m_Edit_FieldTable.FieldTable.DoModal() == IDOK 
		|| m_Edit_FieldTable.FieldTable.DoModal() == IDSAVE )
	{	
		m_Edit_FieldTable.FTFileName = m_Edit_FieldTable.FieldTable.NameFile;
		
		m_Edit_FieldTable.sEdit = m_Edit_FieldTable.FTFileName;

		Invalidate(FALSE);

		UpdateFromEdits();
	}
}

void PropPage4::OnButtonInputTable() 
{
	// TODO: Add your control notification handler code here
	m_Edit_FieldTable.FieldTable.SetPathName(m_Edit_FieldTable.PathName);
	
	CString openFilter = "Field Data (*.txt)|*.txt||";
	CString root;

	if ( iModel == 4 ) // Linear coreflood
		root = "Lab Data\\LabData.txt";
	else
		root = "Field Data\\FieldData.txt";

	CFileDialog openDialog(TRUE, _T("txt"), m_Edit_FieldTable.PathName + root, OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST, openFilter, this);
	
	if (openDialog.DoModal() == IDOK )
	{
		m_Edit_FieldTable.FTFileName = openDialog.GetFileName();
		
		m_Edit_FieldTable.sEdit = m_Edit_FieldTable.FTFileName;

		Invalidate(FALSE);

		UpdateFromEdits();
	}

}
void PropPage4::OnButtonRemoveTable()
{
	m_Edit_FieldTable.sEdit = "No File";
	m_Edit_FieldTable.FTFileName = m_Edit_FieldTable.sEdit;
	Invalidate(NULL);
}

void PropPage4::OnButtonRemoveFileNum()
{	
	int n;
	
	CString sItem;

	n = atoi(m_Edit_FileNum.sEdit);
	
	if( n > 0 && n < 5 )
	{	
		sItem.Format(_T("Click here %d"),n);
		OldFileGrid.colS[0][n-1] = sItem;
	}

	if( n > 4 && n < 9 )
	{	
		sItem.Format(_T("Click here %d"), n);
		OldFileGrid.colS[1][n%5] = sItem;
	}
	
	if( n == 0 ) // Remove all the files
	{
		for( int i = 0 ; i < MaxFileNm/2 ; i++ )
		{			
			sItem.Format(_T("Click here %d"), i+1);
			OldFileGrid.colS[0][i] = sItem;
			sItem.Format(_T("Click here %d"), i + 5);
			OldFileGrid.colS[1][i] = sItem;	
		}
	}

	Invalidate(FALSE);
}


BOOL PropPage4::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i;

	TableGrid TempGrid(4,2);

	OldFileGrid = TempGrid;

	CString sEdit;
	
	OldFileGrid.bScroll = TRUE;

	for( i = 0 ; i < MaxFileNm/2 ; i++ )
	{
		OldFileGrid.SetRowEdit(i, TableGrid::TABLE);
		
		sEdit.Format(_T("Click here %d"), i + 1);
		OldFileGrid.colS[0][i] = sEdit;
		sEdit.Format(_T("Click here %d"), i + 5);
		OldFileGrid.colS[1][i] = sEdit;	
	}

	for( i = 0 ; i < nOlds ; i++ )
	{	
		sEdit.Format(_T("%s"), FileNm[i]);
		
		if( i >= MaxFileNm/2 )
			
			OldFileGrid.colS[1][i%(MaxFileNm/2)] = sEdit;
		
		else

			OldFileGrid.colS[0][i] = sEdit;
	}
	
	if (nDisabled) // Disable for SQUEEZE VI Graphics Options 
	{			
		GetDlgItem(IDC_STATIC_RETPLOT)->ShowWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GRAPHPARA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_GRPA)->EnableWindow(FALSE);
		m_Static_GrPa.EnableWindow(FALSE);

		m_Static_RetPlot.ShowWindow(FALSE);
		m_Button_ScrDis.ShowWindow(FALSE);
		m_Button_TimLin.ShowWindow(FALSE);
		m_Button_TimLog.ShowWindow(FALSE);
		m_Button_WatLin.ShowWindow(FALSE);
		m_Button_WatLog.ShowWindow(FALSE);

		m_Button_TimLin.ShowWindow(FALSE);
		m_Button_TimLog.ShowWindow(FALSE);
		m_Button_WatLin.ShowWindow(FALSE);
		m_Button_WatLog.ShowWindow(FALSE);

		m_Static_RetConc.ShowWindow(FALSE);
		m_Static_MaxIter.ShowWindow(FALSE);
		m_Static_MaxTim.ShowWindow(FALSE);

	//	m_Static_Tol.EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NITERS)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_NFREQ)->ShowWindow(FALSE);
		
		// Output file Distance Dependant
		GetDlgItem(IDC_STATIC_OUTPUTFILE)->ShowWindow(TRUE);
		GetDlgItem(IDC_CW)->ShowWindow(TRUE);
		GetDlgItem(IDC_CO)->ShowWindow(TRUE);
		GetDlgItem(IDC_ADSW)->ShowWindow(TRUE);
		GetDlgItem(IDC_ADSO)->ShowWindow(TRUE);
		GetDlgItem(IDC_VISCW)->ShowWindow(TRUE);
		GetDlgItem(IDC_VISCO)->ShowWindow(TRUE);
		GetDlgItem(IDC_RFW)->ShowWindow(TRUE);
		GetDlgItem(IDC_RFO)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_OUTPUTFILE)->ShowWindow(TRUE);
		GetDlgItem(IDC_CHECK_OUTPUTFILE2)->ShowWindow(TRUE);
		GetDlgItem(IDC_SW)->ShowWindow(TRUE);
		GetDlgItem(IDC_FW)->ShowWindow(TRUE);
		
		m_Check_Cw.SetCheck( bCw ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_Co.SetCheck( bCo ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_AdsW.SetCheck( bAdsW ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_AdsO.SetCheck( bAdsO ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_ViscW.SetCheck( bViscW ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_ViscO.SetCheck( bViscO ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_RFW.SetCheck( bRFW ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_RFO.SetCheck( bRFO ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_OutputFile.SetCheck( bOutputFile ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_SW.SetCheck( bSW ? BST_CHECKED : BST_UNCHECKED); 
		m_Check_FW.SetCheck( bFW ? BST_CHECKED : BST_UNCHECKED); 
		
		m_Check_OutputFileTime.SetCheck( bOutputFileTime ? BST_CHECKED : BST_UNCHECKED);

		if (m_Check_OutputFile.GetCheck())
		{
			GetDlgItem(IDC_CW)->EnableWindow(TRUE);
			GetDlgItem(IDC_CO)->EnableWindow(TRUE);
			GetDlgItem(IDC_ADSW)->EnableWindow(TRUE);
			GetDlgItem(IDC_ADSO)->EnableWindow(TRUE);
			GetDlgItem(IDC_VISCW)->EnableWindow(TRUE);
			GetDlgItem(IDC_VISCO)->EnableWindow(TRUE);
			GetDlgItem(IDC_RFW)->EnableWindow(TRUE);
			GetDlgItem(IDC_RFO)->EnableWindow(TRUE);
			GetDlgItem(IDC_SW)->EnableWindow(TRUE);
			GetDlgItem(IDC_FW)->EnableWindow(TRUE);
		}
		else
		{
			GetDlgItem(IDC_CW)->EnableWindow(FALSE);
			GetDlgItem(IDC_CO)->EnableWindow(FALSE);
			GetDlgItem(IDC_ADSW)->EnableWindow(FALSE);
			GetDlgItem(IDC_ADSO)->EnableWindow(FALSE);
			GetDlgItem(IDC_VISCW)->EnableWindow(FALSE);
			GetDlgItem(IDC_VISCO)->EnableWindow(FALSE);
			GetDlgItem(IDC_RFW)->EnableWindow(FALSE);
			GetDlgItem(IDC_RFO)->EnableWindow(FALSE);
			GetDlgItem(IDC_SW)->EnableWindow(FALSE);
			GetDlgItem(IDC_FW)->EnableWindow(FALSE);
		}

		if (kPlot == 0 ) 
		{
			m_Button_Time.SetCheck(true);
			m_Button_WatPro.SetCheck(false);
		}

		if (kPlot == 1 ) 
		{
			m_Button_Time.SetCheck(false);
			m_Button_WatPro.SetCheck(true);
		}

		m_Button_Time.ShowWindow(true);
		m_Button_WatPro.ShowWindow(true);
	}
	else
	{	
		GetDlgItem(IDC_STATIC_RETCONC)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_RETPLOT)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_GRAPHPARA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRPA)->EnableWindow(TRUE);
		m_Static_GrPa.EnableWindow(TRUE);
		m_Static_RetPlot.EnableWindow(TRUE);
		m_Static_RetPlot.EnableWindow(TRUE);
		m_Button_ScrDis.EnableWindow(TRUE);
		m_Button_TimLin.EnableWindow(TRUE);
		m_Button_TimLog.EnableWindow(TRUE);
		m_Button_WatLin.EnableWindow(TRUE);
		m_Button_WatLog.EnableWindow(TRUE);

		m_Static_RetConc.EnableWindow(TRUE);
		m_Static_MaxIter.EnableWindow(TRUE);
		m_Static_MaxTim.EnableWindow(TRUE);
/*		
		m_Static_Tol.EnableWindow(TRUE);
		m_Edit_Num_Freq.cellEdit = TRUE; 
		m_Edit_TolItr.cellEdit = TRUE;
		m_Edit_Num_Iters.cellEdit = TRUE;
*/	
		// Output file Distance Dependant
		GetDlgItem(IDC_STATIC_OUTPUTFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_CW)->ShowWindow(FALSE);
		GetDlgItem(IDC_CO)->ShowWindow(FALSE);
		GetDlgItem(IDC_ADSW)->ShowWindow(FALSE);
		GetDlgItem(IDC_ADSO)->ShowWindow(FALSE);
		GetDlgItem(IDC_VISCW)->ShowWindow(FALSE);
		GetDlgItem(IDC_VISCO)->ShowWindow(FALSE);
		GetDlgItem(IDC_RFW)->ShowWindow(FALSE);
		GetDlgItem(IDC_RFO)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_OUTPUTFILE)->ShowWindow(FALSE);
		GetDlgItem(IDC_CHECK_OUTPUTFILE2)->ShowWindow(FALSE);
		GetDlgItem(IDC_SW)->ShowWindow(FALSE);
		GetDlgItem(IDC_FW)->ShowWindow(FALSE);

		m_Button_Time.ShowWindow(false);
		m_Button_WatPro.ShowWindow(false);
	}
	
	if ( kTemp )

		GetDlgItem(IDC_ISTMP)->ShowWindow(TRUE);
	
	else
	{
		GetDlgItem(IDC_ISTMP)->ShowWindow(FALSE);
		m_Bool_IsTmp = FALSE;
	}

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_OLDFILES);
	pWnd->GetWindowRect(&rect);

	int height,
		width,
		nCols = 2,
		nRows = 4;

	height = rect.Height();
	width = rect.Width() - 14;

	for (int c = 0; c < nCols; c++)
		OldFileGrid.SetColWidth(c, int(width / nCols));

	for (int r = 0; r < nRows; r++)
		OldFileGrid.SetRowHeight(r, int(height / nRows));

	UpdateControls();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void PropPage4::UpdateFromEdits()
{
	int i, 
		 c = 0;
	
	bool Check = TRUE;
	
	CString s, sItem;

	nFreq = atoi(m_Edit_Num_Freq.sEdit);
	
	nIters = atoi(m_Edit_Num_Iters.sEdit);
	
	TolItr = atof(m_Edit_TolItr.sEdit);
	
	GrafMIC = atof(m_Edit_GrafMic.sEdit);

	Clife.resize(5);

	Clife[0] = atof(m_Edit_CLife[0].sEdit);
	
	Clife[1] = atof(m_Edit_CLife[1].sEdit);
	
	Clife[2] = atof(m_Edit_CLife[2].sEdit);
	
	Clife[3] = atof(m_Edit_CLife[3].sEdit);
	
	Clife[4] = atof(m_Edit_CLife[4].sEdit);
	
	nLife = 0;
	i = 0;

	while( Check && i < MaxClife )
	{
		if( m_Edit_CLife[i].sEdit != "" )
		
			++nLife;

		else Check = FALSE;

		i++;
	}
/*
	FILE *filePtr;

   char buffer[128];
	
	CString sItem;
	
	sItem.Format("%s","Field Data\\");
	
	sItem += m_Edit_FieldTable.FTFileName;

	if ( (filePtr = fopen(m_Edit_FieldTable.PathName + sItem,"r")) == NULL )
   {   
		sItem.Format( "The file %s was not opened.\nMake sure is in the right directory.\nUpdate from Edits", m_Edit_FieldTable.FTFileName );
	//	AfxMessageBox(sItem);
	}
	else
   {  
		printf( "The file %s was opened\n", m_Edit_FieldTable.FTFileName );
 	
		/* Set pointer to beginning of file: */
/*		fseek( filePtr, 0L, SEEK_SET );

		// validate the SQUEEZE data file and check the version
		fgets(buffer,128,filePtr);

		fscanf(filePtr,"%d \n",&m_Edit_FieldTable.FieldTable.NIso);
		
		fgets(buffer,128,filePtr);

		m_Edit_FieldTable.FieldTable.Time = new double [m_Edit_FieldTable.FieldTable.NIso];

		m_Edit_FieldTable.FieldTable.Conc = new double [m_Edit_FieldTable.FieldTable.NIso];
		
		for( int i = 0 ; i < m_Edit_FieldTable.FieldTable.NIso ; i++)
			
			fscanf(filePtr,"%lf %lf \n",&m_Edit_FieldTable.FieldTable.Time[i], 
				&m_Edit_FieldTable.FieldTable.Conc[i] );
		
		fclose( filePtr );

	}
*/
	if( m_Edit_FieldTable.sEdit == "No File") 
	
		m_bShowTable.EnableWindow(FALSE);
	
	else
	
		m_bShowTable.EnableWindow(TRUE);

	m_Edit_FieldTable.FTFileName = m_Edit_FieldTable.sEdit;

	bCw = ( m_Check_Cw.GetCheck() ? 1 : 0 ); 
	bCo = ( m_Check_Co.GetCheck() ? 1 : 0 ); 
	bAdsW = ( m_Check_AdsW.GetCheck() ? 1 : 0 ); 
	bAdsO = ( m_Check_AdsO.GetCheck() ? 1 : 0 ); 
	bViscW = ( m_Check_ViscW.GetCheck() ? 1 : 0 ); 
	bViscO = ( m_Check_ViscO.GetCheck() ? 1 : 0 ); 
	bRFW = ( m_Check_RFW.GetCheck() ? 1 : 0 ); 
	bRFO = ( m_Check_RFO.GetCheck() ? 1 : 0); 
	bOutputFile = ( m_Check_OutputFile.GetCheck() ? 1 : 0 ); 
	bSW = ( m_Check_SW.GetCheck() ? 1 : 0 ); 
	bFW = ( m_Check_FW.GetCheck() ? 1 : 0 );
	
	bOutputFileTime = ( m_Check_OutputFileTime.GetCheck() ? 1 : 0 ); 
	
/*	for( i = 0 ; i < nOlds ; i++ )
	{
		if( i >= MaxFileNm/2 )
			
			FileNm[i] = OldFileGrid.colS[1][i%(MaxFileNm/2)];
		
		else

			FileNm[i] = OldFileGrid.colS[0][i];
	}
*/
	Invalidate(NULL);

}

void PropPage4::IntialiseOldFileGrid()
{

}

LRESULT PropPage4::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	UpdateFromEdits();
//	UpdateFromListCtrl();
	return 0;
}

LRESULT PropPage4::OnEditLostFocus(WPARAM w, LPARAM l)
{	
//	AfxMessageBox("EDIT");
	UpdateFromEdits();
	return 0;
}

void PropPage4::UpdateControls()
{	
	int i;

	CString sn;
	
	m_Edit_FieldTable.cellEdit = TRUE;

	sn.Format("%d", nFreq);
	m_Edit_Num_Freq.sEdit = sn;
	
	sn.Format("%d", nIters);
	m_Edit_Num_Iters.sEdit = sn;
	
	sn.Format("%.6g", TolItr);
	sn.Replace(_T("e+000"), _T("e"));
	sn.Replace(_T("e+00"), _T("e"));
	sn.Replace(_T("e+0"), _T("e"));
	sn.Replace(_T("e-000"), _T("e-"));
	sn.Replace(_T("e-00"), _T("e-"));
	sn.Replace(_T("e-0"), _T("e-"));
	m_Edit_TolItr.sEdit = sn;
	
	sn.Format("%.4g", GrafMIC);
	m_Edit_GrafMic.sEdit = sn;

	for( i = 0 ; i < nLife ; i++ )
	{
		sn.Format("%.6g", Clife[i]);
		m_Edit_CLife[i].sEdit = sn;
	}
	
	if ( m_Edit_FieldTable.FieldTable.NIso == 0 )
	{
		m_Edit_FieldTable.sEdit = "No File";
		m_bShowTable.EnableWindow(FALSE);
	}
	else
	{
		m_Edit_FieldTable.sEdit = m_Edit_FieldTable.FTFileName;
		m_bShowTable.EnableWindow(TRUE);
	}

	m_Edit_FieldTable.bFieldTab = TRUE;

	if (nLay > 1 && !(nDisabled))
	{
		GetDlgItem(IDC_BUTTON_GRAPHPARA)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_GRPA)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_GRAPHPARA)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_GRPA)->EnableWindow(FALSE);
	}
	if( iModel == 5 )
	{ 
		OldFileGrid.Disabled();
		m_Edit_FieldTable.cellEdit = FALSE;
		m_bInputTable.EnableWindow(FALSE);
		m_bShowTable.EnableWindow(FALSE);
		m_bRemoveFileNum.EnableWindow(FALSE);
		m_Edit_FileNum.cellEdit = FALSE;
		GetDlgItem(IDC_RC_TIME_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RC_TIME_LOG)->EnableWindow(FALSE);
		GetDlgItem(IDC_RC_WATVOL_LIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_RC_WATVOL_LOG)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_ENTER0)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FIELDSQUEEZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FILES)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_RETPLOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_RETCONC)->EnableWindow(FALSE);
	}
	
	if( iModel == 4 )
	{ 
		GetDlgItem(IDC_STATIC_RETPLOT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_RETCONC)->EnableWindow(FALSE);
		m_Button_TimLin.EnableWindow(FALSE);
		m_Button_TimLog.EnableWindow(FALSE);
		m_Button_WatLin.EnableWindow(FALSE);
		m_Button_WatLog.EnableWindow(FALSE);
	}

	sn.Format(_T("%d"), 0);
	m_Edit_FileNum.sEdit = sn;	

	for( i = 0 ; i < nOlds ; i++ )
	{	
		if( iModel == 5 || iModel == 1)
			
			sn.Format(_T("%s"), "");

		else

			sn.Format(_T("%s"), FileNm[i]);
		
		if( i >= MaxFileNm/2 )
			
			OldFileGrid.colS[1][i%(MaxFileNm/2)] = sn;
		
		else

			OldFileGrid.colS[0][i] = sn;
	}
}

void PropPage4::OnButtonOutputFileDistance()
{
	if (m_Check_OutputFile.GetCheck())
	{
		GetDlgItem(IDC_CW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CO)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADSW)->EnableWindow(TRUE);
		GetDlgItem(IDC_ADSO)->EnableWindow(TRUE);
		GetDlgItem(IDC_VISCW)->EnableWindow(TRUE);
		GetDlgItem(IDC_VISCO)->EnableWindow(TRUE);
		GetDlgItem(IDC_RFW)->EnableWindow(TRUE);
		GetDlgItem(IDC_RFO)->EnableWindow(TRUE);
		GetDlgItem(IDC_SW)->EnableWindow(TRUE);
		GetDlgItem(IDC_FW)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CO)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADSW)->EnableWindow(FALSE);
		GetDlgItem(IDC_ADSO)->EnableWindow(FALSE);
		GetDlgItem(IDC_VISCW)->EnableWindow(FALSE);
		GetDlgItem(IDC_VISCO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RFW)->EnableWindow(FALSE);
		GetDlgItem(IDC_RFO)->EnableWindow(FALSE);
		GetDlgItem(IDC_SW)->EnableWindow(FALSE);
		GetDlgItem(IDC_FW)->EnableWindow(FALSE);
	}
	
	UpdateFromEdits();
}

void PropPage4::OnButtonOutputFileTime()
{	
	UpdateFromEdits();

	if (m_Check_OutputFileTime.GetCheck())
	
		OTDlg.DoModal();
		
}

void PropPage4::OnKillfocusOutputFile()
{
	UpdateFromEdits();
}

void PropPage4::OnKillfocusOldFiles()
{
	UpdateFromEdits();
}

void PropPage4::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	UpdateFromEdits();
}

BOOL PropPage4::OnKillActive()
{
	UpdateFromEdits();

	return CPropertyPage::OnKillActive();
}

void PropPage4::OnButtonCw()
{
	UpdateFromEdits();
}
void PropPage4::OnButtonCo()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonAdsW()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonAdsO()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonViscW()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonViscO()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonRFW()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonRFO()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonSw()
{
	UpdateFromEdits();
}

void PropPage4::OnButtonSo()
{
	UpdateFromEdits();
}


void PropPage4::OnBnClickedRcWatpro()
{
	m_Button_Time.SetCheck(false);
	m_Button_WatPro.SetCheck(true);
	kPlot = 1; 
}

void PropPage4::OnBnClickedRcTime()
{	
	m_Button_Time.SetCheck(true);
	m_Button_WatPro.SetCheck(false);
	kPlot = 0; 
}

void PropPage4:: OnTime_Lin(){ CSheet::kPlot = 0; };
void PropPage4:: OnTime_Log(){ CSheet::kPlot = 0; };
void PropPage4:: OnWatVol_Lin(){ CSheet::kPlot = 1; };
void PropPage4:: OnWatVol_Log(){ CSheet::kPlot = 1; };