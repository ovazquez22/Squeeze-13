// CheckListBoxGraphs.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "CheckListBoxGraphs.h"
#include "afxdialogex.h"

// CheckListBoxGraphs dialog

IMPLEMENT_DYNAMIC(CheckListBoxGraphs, CDialogEx)

CheckListBoxGraphs::CheckListBoxGraphs(CWnd* pParent /*=NULL*/)
	: CDialogEx(CheckListBoxGraphs::IDD, pParent)
{

}

CheckListBoxGraphs::~CheckListBoxGraphs()
{
	int i = 0;
}

void CheckListBoxGraphs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ISOTHERMS,m_CheckListIsotherms);
	DDX_Control(pDX, IDC_EDIT_YMIN,m_Edit_yMin);
	DDX_Control(pDX, IDC_EDIT_XMIN,m_Edit_xMin);
	DDX_Control(pDX, IDC_EDIT_YMAX,m_Edit_yMax);
	DDX_Control(pDX, IDC_EDIT_XMAX,m_Edit_xMax);
	DDX_Control(pDX, IDC_CHECK_YLOGSCALE,m_Check_YLogScale);
//	DDX_Control(pDX, IDC_STATIC_GRAP_ISO, GraphIso);
}


BEGIN_MESSAGE_MAP(CheckListBoxGraphs, CDialogEx)
	//{{AFX_MSG_MAP(CCheckListBoxCBNDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
//	ON_WM_QUERYDRAGICON()
	ON_LBN_SELCHANGE(IDC_LIST_ISOTHERMS, OnSelchangeListIsotherms)
	//}}AFX_MSG_MAP
	ON_CLBN_CHKCHANGE(IDC_LIST_ISOTHERMS, OnCheckchangeListIsotherms)

	ON_BN_CLICKED(ID_UPDATE, &CheckListBoxGraphs::OnBnClickedUpdate)
	ON_BN_CLICKED(IDC_CHECK_YLOGSCALE, &CheckListBoxGraphs::OnBnClickedCheckYlogscale)
	ON_BN_CLICKED(IDCANCEL, &CheckListBoxGraphs::OnBnClickedCancel)
END_MESSAGE_MAP()


// CheckListBoxGraphs message handlers
BOOL CheckListBoxGraphs::OnInitDialog()
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

	m_Graph.ReadOutputIsoDer(openFile);


	m_CheckListIsotherms.ResetContent();

	//	m_CheckListIsotherms.SetCheckStyle( BS_AUTO3STATE );
	m_CheckListIsotherms.SetCheckStyle(BS_AUTOCHECKBOX);

	m_CheckListIsotherms.AddString("All");
	m_CheckListIsotherms.SetCheck(0, 1);
	for (int i = 1; i < m_Graph.nLines; i++)
	{
		m_CheckListIsotherms.AddString(m_Graph.m_Lines[i].m_strName);
		m_CheckListIsotherms.SetCheck(i, 1);
	}
	CString sn;

	sn.Format("%.6g", m_Graph.yMin);
	m_Edit_yMin.SetWindowText(sn);

	sn.Format("%.6g", m_Graph.xMin);
	m_Edit_xMin.SetWindowText(sn);

	sn.Format("%.6g", m_Graph.yMax);
	m_Edit_yMax.SetWindowText(sn);

	sn.Format("%.6g", m_Graph.xMax);
	m_Edit_xMax.SetWindowText(sn);

	m_Check_YLogScale.SetCheck(m_Graph.logScale);

	CRect rect;
	GetDlgItem(IDC_STATIC_GRAP_ISO)->GetWindowRect(rect);
	ScreenToClient(rect);

	m_Graph.title = "Data    ";
	m_Graph.bTitle = true;

	m_Graph.Create(WS_CHILD | WS_VISIBLE, rect, this, IDC_GRAPH);

	// Do not call CDialog::OnPaint() for painting messages

	return TRUE;  // return TRUE  unless you set the focus to a control

}


void CheckListBoxGraphs::OnSelchangeListIsotherms() 
{
	// TODO: Add your control notification handler code here
	//TRACE( "CheckListBoxGraphs::OnSelchangeListIsotherms" );	
	
}

void CheckListBoxGraphs::OnCheckchangeListIsotherms() 
{
	// TODO: Add your control notification handler code here
	//TRACE( "CheckListBoxGraphs::OnCheckchangeListIsotherms\n" );
	CString text;
	text.Format("Index: %i\n", m_CheckListIsotherms.GetCurSel());

	for ( int i = 1 ; i < m_CheckListIsotherms.GetCount() ; i++)
	{
		text.Format("Index %i ___ Checked: %i\n", i, m_CheckListIsotherms.GetCheck(i));
		TRACE(text);	
		m_Graph.m_Lines[i].check = m_CheckListIsotherms.GetCheck(i);
	}

	if ( m_CheckListIsotherms.GetCurSel() == 0 )
	{
		if ( m_CheckListIsotherms.GetCheck(0) )
		
			for (int i = 1 ; i < m_CheckListIsotherms.GetCount() ; i++)
			{
				m_CheckListIsotherms.SetCheck(i,1);
				m_Graph.m_Lines[i].check = m_CheckListIsotherms.GetCheck(i);
			}
		else

			for (int i = 1 ; i < m_CheckListIsotherms.GetCount() ; i++)
			{
				m_CheckListIsotherms.SetCheck(i,0);
				m_Graph.m_Lines[i].check = m_CheckListIsotherms.GetCheck(i);
			}
	}

	m_Graph.Update();
}


void CheckListBoxGraphs::OnBnClickedUpdate()
{	
	CString sEdit;

	m_Edit_yMin.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.yMin = atof(sEdit);
	}

	m_Edit_xMin.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.xMin = atof(sEdit);
	}

	m_Edit_yMax.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.yMax = atof(sEdit);
	}

	m_Edit_xMax.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.xMax = atof(sEdit);
	}

	m_Graph.logScale = m_Check_YLogScale.GetCheck(); 

	m_Graph.Update();
}


void CheckListBoxGraphs::OnBnClickedCheckYlogscale()
{
	CString sEdit;

	m_Edit_yMin.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.yMin = atof(sEdit);
	}

	m_Edit_xMin.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.xMin = atof(sEdit);
	}

	m_Edit_yMax.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.yMax = atof(sEdit);
	}

	m_Edit_xMax.GetWindowText(sEdit);
	if (sEdit.IsEmpty() == FALSE)
	{
		sEdit.TrimRight();
		m_Graph.xMax = atof(sEdit);
	}

	m_Graph.logScale = m_Check_YLogScale.GetCheck(); 

	m_Graph.Update();
}



void CheckListBoxGraphs::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
