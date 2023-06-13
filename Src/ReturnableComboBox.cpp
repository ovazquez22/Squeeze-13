// ReturnableComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "ReturnableComboBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReturnableComboBox

ReturnableComboBox::ReturnableComboBox()
{
}

ReturnableComboBox::~ReturnableComboBox()
{
}


BEGIN_MESSAGE_MAP(ReturnableComboBox, CComboBox)
	//{{AFX_MSG_MAP(ReturnableComboBox)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReturnableComboBox message handlers

void ReturnableComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	CWnd* pWnd = 0;
	pWnd = GetParent();
	if (pWnd != NULL)
	{
		pWnd->PostMessage(UDM_BOX_LOST_FOCUS);
	}
	
}

void ReturnableComboBox::OnSelchange() 
{
	CWnd* pWnd = 0;
	pWnd = GetParent();
	if (pWnd != NULL)
	{
		pWnd->PostMessage(UDM_BOX_LOST_FOCUS);
	}
}
