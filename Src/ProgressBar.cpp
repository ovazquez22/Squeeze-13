// ProgressBar.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "ProgressBar.h"


// ProgressBar dialog

IMPLEMENT_DYNAMIC(ProgressBar, CDialog)

ProgressBar::ProgressBar(CWnd* pParent /*=NULL*/)
	: CDialog(ProgressBar::IDD, pParent)
{
	iProBar = 0;
}

ProgressBar::~ProgressBar()
{
	DestroyWindow();
}

void ProgressBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressBar);
}


BEGIN_MESSAGE_MAP(ProgressBar, CDialog)
	ON_BN_CLICKED(IDOK, &ProgressBar::OnBnClickedOk)
END_MESSAGE_MAP()


// ProgressBar message handlers

void ProgressBar::OnBnClickedOk()
{	
	ShellExecute(NULL, "open","notepad.exe",openFile,NULL,SW_SHOWNORMAL);

	OnOK();
}
