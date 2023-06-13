// ReturnableEdit.cpp : implementation file
//

#include "stdafx.h"
#include "ReturnableEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ReturnableEdit

ReturnableEdit::ReturnableEdit()
{
}

ReturnableEdit::~ReturnableEdit()
{
}


BEGIN_MESSAGE_MAP(ReturnableEdit, CEdit)
	//{{AFX_MSG_MAP(ReturnableEdit)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ReturnableEdit message handlers

void ReturnableEdit::OnKillFocus(CWnd* pNewWnd) 
{
	
	// TODO: Add your message handler code here
	CWnd* pWnd = 0;
	pWnd = GetParent();
	if (pWnd != NULL)
	{
		pWnd->PostMessage(UDM_EDIT_LOST_FOCUS);
	}
}

