// EditSingleGrid.cpp : implementation file
//

#include "stdafx.h"
//#include "Squeeze VI.h"
#include "EditSingleGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditSingleGrid

EditSingleGrid::EditSingleGrid()
{
	cellEdit = TRUE;

	FieldTable.bTab = FALSE;
	
	bFieldTab = FALSE;

	FTFileName = "FieldTab.txt";
}

EditSingleGrid::~EditSingleGrid()
{
}


BEGIN_MESSAGE_MAP(EditSingleGrid, CStatic)
	//{{AFX_MSG_MAP(EditSingleGrid)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditSingleGrid message handlers

void EditSingleGrid::OnPaint() 
{

	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages

	CBrush grayBrush(GetSysColor(COLOR_BTNFACE));
	GetClientRect(clientRect);

	// some initialisation - can't find better place for this...
	
	

	if (!theEdit)
	{
		theEdit.Create(WS_CHILD|ES_CENTER|ES_MULTILINE|ES_WANTRETURN, editRect, this, IDC_EDIT_FOR_JCGRID);
		ASSERT(theEdit);
		theEdit.ShowWindow(FALSE);

	}
	// Do not call CStatic::OnPaint() for painting messages

	// create a memory dc, blt it to screen later to avoid flicker
	CDC mdc;
	if (!mdc.CreateCompatibleDC(&dc)) {
		AfxMessageBox("CreateCompatibleDC failed in OnPaint");
		return;
	}
	CBitmap mbitmap;
	if (!mbitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height())) {
		AfxMessageBox("CreateCompatibleBitmap failed in OnPaint");
		return;
	}
	
	CBitmap* pbmpOld = mdc.SelectObject(&mbitmap);
	mdc.FillRect(clientRect, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_WINDOW)));
	
	
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CFont titleFont;
	titleFont.CreatePointFont(90, _T("Arial"));
	CFont* pOldFont = mdc.SelectObject(&titleFont);

	mdc.SelectObject(&blackPen);
	
	if( cellEdit )
	{
		mdc.DrawText(sEdit, clientRect-(LPCRECT(CRect(1, 1, 1, 1))),
			DT_VCENTER|DT_CENTER|DT_WORDBREAK);
	}
	else
	{
		mdc.FillRect(clientRect, &grayBrush);
	}
	
	// copy onto screen
	if (!dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &mdc, 0, 0, SRCCOPY)) {
//		AfxMessageBox("BitBlt failed");
	}

	// clean up
	if (mdc.SelectObject(pbmpOld) == NULL) {
		AfxMessageBox("SelectObject(pbmpOld) failed");
		return;
	}
	mbitmap.DeleteObject();
	mdc.DeleteDC();

//	dc.SelectObject(pOldBrush);
}

LRESULT EditSingleGrid::OnEditLostFocus(WPARAM w, LPARAM l)
{
//	AfxMessageBox("Edit Lost Focus");
	theEdit.ShowWindow(FALSE);
	theEdit.GetWindowText(sEdit);
	if ((sEdit.IsEmpty() == FALSE)&&(editHasFocus))
	{
		sEdit.TrimRight();
		editHasFocus = false;

		CWnd* pWnd = 0;
		pWnd = GetParent();
		if (pWnd != NULL)
		{
			pWnd->PostMessage(UDM_LIST_CTRL_UPDATE);
		}

	}

  return 0;
}

void EditSingleGrid::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( cellEdit)
	{
		if (bFieldTab)
		{	
			CString openFilter = "Field Data (*.txt)|*.txt||";
	
			CFileDialog openDialog(TRUE, "txt", PathName + CString("Field Data\\FieldData.txt"), OFN_HIDEREADONLY|
				OFN_FILEMUSTEXIST, openFilter, this);
			
			if (openDialog.DoModal() == IDOK )
			{
				sEdit = openDialog.GetFileName();

				FTFileName = sEdit;
			}
		
		}
		
		theEdit.SetWindowText(sEdit);
		theEdit.MoveWindow(&clientRect);
		theEdit.ShowWindow(TRUE);
		theEdit.SetFocus();
		theEdit.SetSel(0, -1);
		editHasFocus = true;

		Invalidate(FALSE);

	}
	else
	{
	
	}

	Invalidate(FALSE);

	CStatic::OnLButtonDown(nFlags, point);
}

void EditSingleGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	CStatic::OnLButtonUp(nFlags, point);
}