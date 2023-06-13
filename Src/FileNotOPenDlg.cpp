// FileNotOPenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "FileNotOPenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FileNotOPenDlg dialog


FileNotOpenDlg::FileNotOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FileNotOpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FileNotOPenDlg)
	//}}AFX_DATA_INIT
}


void FileNotOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FileNotOPenDlg)
	DDX_Control(pDX, IDC_STATIC_FILE, m_Static_FileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FileNotOpenDlg, CDialog)
	//{{AFX_MSG_MAP(FileNotOPenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FileNotOPenDlg message handlers
