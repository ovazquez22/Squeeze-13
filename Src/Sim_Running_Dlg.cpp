// Sim_Running_Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Sim_Running_Dlg.h"
#include "afxdialogex.h"


// Sim_Running_Dlg dialog

IMPLEMENT_DYNAMIC(Sim_Running_Dlg, CDialogEx)

Sim_Running_Dlg::Sim_Running_Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Sim_Running_Dlg::IDD, pParent)
{

}

Sim_Running_Dlg::~Sim_Running_Dlg()
{
}

void Sim_Running_Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Sim_Running_Dlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_TXT, &Sim_Running_Dlg::OnStnClickedStaticTxt)
END_MESSAGE_MAP()


// Sim_Running_Dlg message handlers

void Sim_Running_Dlg::Update(bool check, CString sEdit)
{	
	if ( check ) 
	{
		GetDlgItem(IDC_STATIC_TXT)->SetWindowText(sEdit);
	}
	else
	{
		GetDlgItem(IDC_STATIC_TXT)->SetWindowText("Please, wait few more seconds");
	}
}

void Sim_Running_Dlg::OnStnClickedStaticTxt()
{
	// TODO: Add your control notification handler code here
}
