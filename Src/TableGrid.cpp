// TableGrid.cpp : implementation file
//

#include "stdafx.h"
#include "TableGrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TableGrid

TableGrid::TableGrid()
{	
	nRows = 20;
		
	nCols = 2;
	
	vScrollPos = hScrollPos = 0;
	
	Allocate();
	
	InitGrid();

	bScroll = FALSE;

	PathNameReadFile = "NULL";
	
}

TableGrid::TableGrid(int N)
{	
	nRows = N;
		
	nCols = 2;
	
	vScrollPos = hScrollPos = 0;
	
	Allocate();
	
	InitGrid();

	bScroll = FALSE;

	PathNameReadFile = "NULL";
		
}

TableGrid::TableGrid(int r, int c)
{	
	nRows = r;
		
	nCols = c;
	
	vScrollPos = hScrollPos = 0;
	
	Allocate();
	
	InitGrid();

	bScroll = FALSE;

	PathNameReadFile = "NULL";
	
}

TableGrid::TableGrid(int N, double *C, double *A)
{	
	nRows = N;
		
	nCols = 2;
	
	vScrollPos = hScrollPos = 0;
	
	Allocate();
	
	InitGridFile( C, A);
	
	bScroll = FALSE;

	PathNameReadFile = "NULL";
}

TableGrid::TableGrid(int N, double *S, double *Kw, double *Ko)
{	
	nRows = N;
		
	nCols = 3;
	
	vScrollPos = hScrollPos = 0;
	
	Allocate();
	
	InitGridFileRelPerm( S, Kw, Ko);
	
	bScroll = FALSE;

	PathNameReadFile = "NULL";
}

TableGrid::~TableGrid()
{
	DeAllocate();
}

void TableGrid::Allocate()
{

	colWidth = new int [nCols];
	rowHeight = new int [nRows];
	colPos = new int [nCols];
	rowPos = new int [nRows];
	cellEdit = new CELL_EDIT * [nCols];
	cellFormat = new CELL_FORMAT *[nCols];
	iList = new int * [nCols];

	for (int c = 0; c < nCols; c++)
	{
		colS[c].SetSize(nRows);
		cellEdit[c] = new CELL_EDIT [nRows];
		cellFormat[c] = new CELL_FORMAT [nRows];
		iList[c] = new int [nRows];
	}
	
	editHasFocus = false;
	
	cellC = cellR = 0;

	FieldData = 0;
}

void TableGrid::DeAllocate()
{	
	for (int c = 0; c < nCols; c++)
	{
		delete[] cellEdit[c];
		delete[] cellFormat[c];
		delete[] iList[c];
	}
	
	delete[] cellEdit;
	delete[] cellFormat;
	delete[] iList;

	delete[] colWidth;
	delete[] rowHeight;
	delete[] colPos;
	delete[] rowPos;
}

BEGIN_MESSAGE_MAP(TableGrid, CStatic)
	//{{AFX_MSG_MAP(TableGrid)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE, OnListCtrlUpdate)
	ON_MESSAGE(UDM_BOX_LOST_FOCUS, OnBoxLostFocus)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID__SAVEDESIGN, &TableGrid::OnSavedesign)
	ON_COMMAND(ID__EXPORTTABLE, &TableGrid::OnExporttable)
	ON_COMMAND(ID__EXPORTPARETOFRONT, &TableGrid::OnExportparetofront)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// TableGrid message handlers

void TableGrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	int sWidth;

	GetClientRect(clientRect);
	// some initialisation - can't find better place for this...

	if (!theEdit)
	{
		theEdit.Create(WS_CHILD|ES_CENTER|ES_MULTILINE|ES_WANTRETURN, editRect, this, IDC_EDIT_FOR_JCGRID);
		ASSERT(theEdit);
		theEdit.ShowWindow(FALSE);

	}
	
	if (!theBox)
	{
		CRect comboRect(0, 0, 30, 200);
		theBox.Create(WS_CHILD|WS_VSCROLL|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST, comboRect, this, IDC_BOX_FOR_JCGRID);
		ASSERT(theBox);
		CFont * pFont = GetFont();
		theBox.SetFont(pFont, FALSE);
		theBox.ShowWindow(FALSE);
	}

	if (!vScroller && !bScroll)
	{
		scrollRect.SetRect(clientRect.right - __min(clientRect.Width(), 14),
			clientRect.top, clientRect.right, clientRect.bottom);
		vScroller.Create(WS_CHILD|SBS_VERT, scrollRect, this, 1001);
		ASSERT(vScroller);
		int maxVScroll = __max(rowPos[nRows-1] - clientRect.bottom, 0);
		vScroller.SetScrollRange(0, maxVScroll, FALSE);
		vScroller.SetScrollPos(0, FALSE);
		vScroller.ShowWindow(TRUE);
	}
	

	if( bScroll )  sWidth = 0;

	else sWidth = scrollRect.Width();

		
	if ( FieldData == 0 )
		
		for (int c = 0; c < nCols; c++)
		
			SetColWidth(c,  int ( (clientRect.Width() - sWidth) / nCols ) );

	if (FieldData == 1)
	{
		SetColWidth(0,  int(0.6*(clientRect.Width() - sWidth)) );
		
		SetColWidth(1,  int(0.4*(clientRect.Width() - sWidth)) );
	}


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

	DrawGrid(&mdc, clientRect);

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

}

void TableGrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	if (theEdit)
	{
		OnEditLostFocus(0,0);
	}
	
	if (theBox)
	{
		OnBoxLostFocus(0,0);
	}

	int maxVScroll = __max(rowPos[nRows-1] - clientRect.bottom, 0);
	vScroller.SetScrollRange(0, maxVScroll, FALSE);

	switch (nSBCode)
	{
	case SB_LINEUP:
	case SB_PAGEUP:
		vScrollPos -= rowHeight[nRows-1];
		vScrollPos = __max(vScrollPos, 0);
		break;
	case SB_LINEDOWN:
	case SB_PAGEDOWN:
		vScrollPos += rowHeight[nRows-1];
		vScrollPos = __min(vScrollPos, maxVScroll);
		break;
	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		vScrollPos = nPos;
		break;
	}

	vScroller.SetScrollPos(vScrollPos);

	Invalidate(FALSE);
	
	CStatic::OnVScroll(nSBCode, nPos, pScrollBar);
}

void TableGrid::InitGrid()
{	
	CString sItem;

	for (int r = 0; r < nRows; r++)
	{
		for (int c = 0; c < nCols; c++)
		{
			colWidth[c] = 60;
			
			sItem.Format("(%d,%d)", r,c);
			colS[c][r] = sItem;
			cellEdit[c][r] = EDIT;
			
			iList[c][r] = 0;
		}

		rowHeight[r] = 20;
	}

	CalculateColRowPos();

}

void TableGrid::InitGridFile(double *C, double *A)
{	
	CString sItem;
	
	SetRowEdit(0,TableGrid::READ_ONLY);
	SetRowFormat(0,TableGrid::HEADER);
	
	colS[0][0] = "Ciso\n(ppm)";
	colS[1][0] = "ADiso\n(mg/L rock)";
	
	rowHeight[0] = 20;

	for (int r = 0; r < nRows-1; r++)
	{
		for (int c = 0; c < nCols; c++)
		{
			colWidth[c] = 60;
			
			if( c%2 == 0)
				sItem.Format("%.5g", *(C+r));
			
			if( c%2 == 1)
				sItem.Format("%.5g", *(A+r));
			
			colS[c][r+1] = sItem;
			
		}

		rowHeight[r+1] = 20;
	}

	CalculateColRowPos();

}

void TableGrid::InitGridFileRelPerm(double *S, double *Kw, double *Ko)
{	
	CString sItem;
	
	SetRowEdit(0,TableGrid::READ_ONLY);
	SetRowFormat(0,TableGrid::HEADER);
	
	colS[0][0] = "Sw";
	colS[1][0] = "Krw";
	colS[2][0] = "Kro";
	
	rowHeight[0] = 20;

	for (int r = 0; r < nRows-1; r++)
	{
		for (int c = 0; c < nCols; c++)
		{
			colWidth[c] = 60;
			
			if( c%3 == 0)
				sItem.Format("%.5g", *(S+r));
			
			if( c%3 == 1)
				sItem.Format("%.5g", *(Kw+r));
			
			if( c%3 == 2)
				sItem.Format("%.5g", *(Ko+r));
			
			colS[c][r+1] = sItem;
			
		}

		rowHeight[r+1] = 20;
	}

	CalculateColRowPos();

}

void TableGrid::DrawGrid(CDC* pDC, CRect rect)
{	
	CBrush Brush(RGB(15, 255, 12));
	pDC->FillRect(rect,&Brush);

	CalculateColRowPos();
	int oldMode = pDC->SetBkMode(TRANSPARENT);
	CRect innerRect = rect;
	innerRect.DeflateRect(1, 1);
	CRgn chartRgn;
	chartRgn.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	pDC->SelectClipRgn(&chartRgn);
	pDC->FillRect(rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	CBrush * pOldBrush;
	CBrush newBrush(RGB(0, 255, 0));
	CBrush normalBrush(RGB(255, 255, 255));
	CBrush blueBrush(GetSysColor(COLOR_HIGHLIGHT));
	CBrush hiliteBrush(GetSysColor(COLOR_INFOBK));
	CBrush grayBrush(GetSysColor(COLOR_BTNFACE));
	pOldBrush = pDC->SelectObject(&newBrush);
	CPen* pOldPen;
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen grayPen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
	pOldPen = pDC->SelectObject(&grayPen);

	CFont horizFont, titleFont, symbolFont;
	CFont verticalFont;
	horizFont.CreatePointFont(90, _T("Arial"));
	symbolFont.CreatePointFont(90, "Symbol");
	titleFont.CreatePointFont(90, _T("Arial"));
	verticalFont.CreateFont(2 + (9*rect.Height())/300, 0, 900, 0, 0, 0, 0, 0, 0, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, _T("Arial"));
	CFont* pOldFont = pDC->SelectObject(&titleFont);

	pDC->SelectObject(&blackPen);
	
	int c1, 
		 r;

	for ( c1 = 0; c1 < nCols; c1++)
	{
		for ( r = 0; r < nRows; r++)
		{

			SetEditRect(c1, r);
			innerRect = editRect;
			innerRect.DeflateRect(1, 1);
			
			switch (cellFormat[c1][r])
			{	
				case NORMAL:
					pDC->FillRect(editRect, &normalBrush);
					break;
				case BLUE:
					pDC->FillRect(editRect, &blueBrush);
					break;
				case HILITE:
					pDC->FillRect(editRect, &hiliteBrush);
					break;
				case GRAY:
					pDC->FillRect(editRect, &grayBrush);
					break;
				case HEADER:
					pDC->FillRect(editRect, &grayBrush);
					pDC->Draw3dRect(innerRect, GetSysColor(COLOR_BTNHILIGHT), GetSysColor(COLOR_BTNSHADOW));
					break;
			}

			//  draw text
			CString sCell = colS[c1][r%colS[c1].GetSize()];
			if (sCell.Find("symbol*", 0) != -1)
			{
				sCell.Replace("symbol*", "");
				pDC->SelectObject(&symbolFont);
			}
			pDC->DrawText(sCell, editRect-(LPCRECT(CRect(1, 1, 1, 1))),
				DT_VCENTER|DT_CENTER|DT_WORDBREAK);
			
			pDC->DrawText(colS[c1][r], editRect-(LPCRECT(CRect(1, 1, 1, 1))),
				DT_VCENTER|DT_CENTER|DT_WORDBREAK);
			pDC->SelectObject(&horizFont);

			// draw lines
			pDC->SelectObject(&grayPen);
			pDC->MoveTo(editRect.left-1, editRect.bottom-1);
			pDC->LineTo(editRect.right-1, editRect.bottom-1);
			pDC->LineTo(editRect.right-1, editRect.top-1);
			pDC->SelectObject(&blackPen);
		
		}

	}

	chartRgn.DeleteObject();
	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->SelectClipRgn(NULL);
	pDC->SetBkMode(oldMode);
	
}

void TableGrid::CalculateColRowPos()
{
	int pos = 0;
	for (int c1 = 0; c1 < nCols; c1++)
	{
		pos += colWidth[c1];
		colPos[c1] = pos;
	}
	
	pos = 0;
	for (int r = 0; r < nRows; r++)
	{
		pos += rowHeight[r];
		rowPos[r] = pos;
	}
}

void TableGrid::SetEditRect(int c, int r)
{	
	double c1, c2, c3, c4;

	if (c == 0) c1 = clientRect.left - hScrollPos;

	else c1 = clientRect.left + colPos[c - 1] - hScrollPos;

	if (r == 0) c2 = clientRect.top - vScrollPos;

	else c2 = clientRect.top + rowPos[r - 1] - vScrollPos;

	c3 = clientRect.left + colPos[c] - hScrollPos;

	c4 = clientRect.top + rowPos[r] - vScrollPos;

	editRect.SetRect(c1, c2, c3, c4);
		/*
	editRect.SetRect(
		clientRect.left + ((c == 0) ? 0 : colPos[c-1]),
		clientRect.top + ((r == 0) ? 0 - vScrollPos : rowPos[r-1] - vScrollPos),
		clientRect.left + colPos[c],
		clientRect.top + rowPos[r] - vScrollPos);*/
}

void TableGrid::SetColWidth(int c, int width)
{
	colWidth[c%nCols] = __max(width, 0);
}

void TableGrid::SetRowHeight(int r, int height)
{
	rowHeight[r%nRows] = __max(height, 0);
}

LRESULT TableGrid::OnEditLostFocus(WPARAM w, LPARAM l)
{
//	AfxMessageBox("Edit Lost Focus");
	theEdit.ShowWindow(FALSE);
	CString sEdit;
	theEdit.GetWindowText(sEdit);
	if ((sEdit.IsEmpty() == FALSE)&&(editHasFocus))
	{
		sEdit.TrimRight();
		colS[cellC][cellR] = sEdit;
		editHasFocus = false;
		
		if ( colS[cellC][cellR] == "Open" )
		
			colS[cellC+1][cellR] = "1";

		if ( colS[cellC][cellR] == "Close" )
		
			colS[cellC+1][cellR] = "";
			
		if( colS[1][0] == "Wat Conc" )
		{
			if( atof(colS[cellC][cellR]) > 1e6 )
			{
				AfxMessageBox("Wat,Oil Concentration out of range, 0<C<1e6.");
				
				colS[cellC][cellR] = "1e6";

				Invalidate(FALSE);
			}
		}

		

		CWnd* pWnd = 0;
		pWnd = GetParent();
		if (pWnd != NULL)
		{
			pWnd->PostMessage(UDM_LIST_CTRL_UPDATE);
		}
		
	}

  return 0;
}

LRESULT TableGrid::OnBoxLostFocus(WPARAM w, LPARAM l)
{		
	theBox.ShowWindow(FALSE);
	CString sBox;
	theBox.GetWindowText(sBox);
	
//	AfxMessageBox("Box Lost Focus");
	
	theEdit.ShowWindow(FALSE);
	CString sEdit;
	theEdit.GetWindowText(sEdit);
	if ((sEdit.IsEmpty() == FALSE) && (editHasFocus))
	{
		sEdit.TrimRight();
		colS[cellC][cellR] = sEdit;
		editHasFocus = false;
	}

	if ((sBox.IsEmpty() == FALSE)&&(boxHasFocus))
	{
		if ( sBox == "Open" )
		{	
			if(checkPIDf)
			{	
				cellEdit[cellC+1][cellR] = EDIT;
				cellFormat[cellC+1][cellR] = NORMAL;
				colS[cellC+1][cellR] = "1";
			}

			if( TypeStage == 3 )
			{
				cellEdit[cellC+2][cellR] = EDIT;
				cellFormat[cellC+2][cellR] = NORMAL;
				colS[cellC+2][cellR] = "1";
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Close" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			colS[cellC+1][cellR] = "";
			
			if( TypeStage == 3 )
			{
				cellEdit[cellC+2][cellR] = READ_ONLY;
				cellFormat[cellC+2][cellR] = GRAY;
				colS[cellC+2][cellR] = "";
			}

			Invalidate(FALSE);
		}
		
		sBox.TrimRight();
		colS[cellC][cellR] = sBox;
		boxHasFocus = false;
		
		CWnd* pWnd = 0;
		pWnd = GetParent();
		if (pWnd != NULL)
		{
			pWnd->PostMessage(UDM_LIST_CTRL_UPDATE);
		}

	}
		
	return 0;
}

void TableGrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//	AfxMessageBox(" sds");
	// TODO: Add your message handler code here and/or call default
	if (!initialised) { return; }
	cellR = 0;
	cellC = 0;
	bool gotCell = false;
	
	// hit test - which cell clicked?
	for (int i = 0; i < nCols; i++)
	{
		for (int r = 0; r < nRows; r++)
		{
			SetEditRect(i, r);
			
			if (editRect.PtInRect(point) == TRUE)
			{
				gotCell = true;
				cellC = i;
				cellR = r;
				break;
			}

		}
		
		if (gotCell) { break; }
	}

	if (gotCell) 
	{
		if (cellEdit[cellC][cellR] == EDIT)
		{
			if (theEdit)
			{
				theEdit.SetWindowText(colS[cellC][cellR]);
				theEdit.MoveWindow(&editRect);
				theEdit.ShowWindow(TRUE);
				theEdit.SetFocus();
				theEdit.SetSel(0, -1);
				editHasFocus = true;
			}
		}
		else if (cellEdit[cellC][cellR] == COMBO)
		{	

			if (theBox)
			{
				theBox.ResetContent();
	//			theBox.SetWindowText(GetItemText(item, subItem));
				for (int i = 0; i < comboList[iList[cellC][cellR]%MAX_COLS].GetSize(); i++)
				{
					theBox.AddString(comboList[iList[cellC][cellR]%MAX_COLS].GetAt(i));
					if (colS[cellC][cellR] == comboList[iList[cellC][cellR]%MAX_COLS].GetAt(i))
					{
						theBox.SetCurSel(i);
					}
				}
				theBox.MoveWindow(&editRect);
				theBox.ShowWindow(TRUE);
				theBox.ShowDropDown();
				theBox.SetFocus();
				boxHasFocus = true;
			}
		}
		else if (cellEdit[cellC][cellR] == TABLE)
		{
			CString openFilter = "Output Files (*.out)|*.out||";

			CFileDialog openDialog(TRUE, _T("out"), PathNameReadFile, OFN_HIDEREADONLY|
				OFN_FILEMUSTEXIST, openFilter, this);

			if (openDialog.DoModal() == IDOK)
			{				
				colS[cellC][cellR] = openDialog.GetFileName();;
			}

			Invalidate(FALSE);
		}
		
	}	
	
//	CStatic::OnLButtonDown(nFlags, point);
}

void TableGrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	// To check with cell has been clicked
	/*
	CString st;
	int nR = point.y/rowHeight[0];
	st.Format("Row %d",nR);
	AfxMessageBox(st);
	*/

	SetFocus();
	CStatic::OnLButtonUp(nFlags, point);
}

TableGrid& TableGrid::operator=(const TableGrid& rhs)
{
	if (this == &rhs) return *this;
	DeAllocate();
	nCols = rhs.nCols;
	nRows = rhs.nRows;
	Allocate();
	InitGrid();
	Equate(rhs);
	return *this;

}


void TableGrid::Equate(const TableGrid& rhs)
{
	for (int c = 0; c < __min(nCols, rhs.nCols); c++)
	{
		colWidth[c] = rhs.colWidth[c];
		colPos[c] = rhs.colPos[c];

		for (int r = 0; r < __min(nRows, rhs.nRows); r++)
		{
			colS[c][r] = rhs.colS[c][r];
		}
	}
	for (int r = 0; r < __min(nRows, rhs.nRows); r++)
	{
		rowHeight[r] = rhs.rowHeight[r];
		rowPos[r] = rhs.rowPos[r];
	}
	
	FieldData = rhs.FieldData;
}

LRESULT TableGrid::OnListCtrlUpdate(WPARAM w, LPARAM l)
{	
	AfxMessageBox("Updated");
	//UpdateFromControls();
	return 0;
}

void TableGrid::SetColRowComboList(int c, int r, int list)
{
	if ((list < 0)||(list >= MAX_COLS))
	{
		AfxMessageBox("Error: out of range in JCGrid::SetColComboList()");
		return;
	}
	
	iList[c][r] = list;
	
}


void TableGrid::SetColEdit(int c, CELL_EDIT edit)
{
	for (int r = 0; r < nRows; r++)
	{
		cellEdit[c%nCols][r] = edit;
	}
}

void TableGrid::SetRowEdit(int r, CELL_EDIT edit)
{
	for (int c = 0; c < nCols; c++)
	{
		cellEdit[c][r%nRows] = edit;
	}
}

void TableGrid::SetColFormat(int c, CELL_FORMAT fmt)
{
	for (int r = 0; r < nRows; r++)
	{
		cellFormat[c%nCols][r] = fmt;
	}
}

void TableGrid::SetRowFormat(int r, CELL_FORMAT fmt)
{
	for (int c = 0; c < nCols; c++)
	{
		cellFormat[c][r%nRows] = fmt;
	}
}

void TableGrid::SetCellText(int c, int r, CString txt)
{
	colS[c%nCols][r%nRows] = txt;
}

void TableGrid::Disabled()
{	
//	nDisabled = 0;

	int c,
		 r;

	for( c = 0 ; c < nCols ; c++ )
	{	
		for( r = 0 ; r < nRows ; r++ )
		{
			SetCellText( c, r, " ");
			
			cellEdit[c][r] = TableGrid::READ_ONLY;

			cellFormat[c][r] = TableGrid::GRAY;
		}
	}
	
	Invalidate(NULL);
}

void TableGrid::Enabled()
{	
	Invalidate(NULL);
}

void TableGrid::UpdateGrid()
{
	Invalidate(NULL);
}

void TableGrid::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	CMenu psoMenu;
	CPoint m_ptMsg;
	m_ptMsg = point;
	ScreenToClient(&m_ptMsg);

	nDesign = (m_ptMsg.y + vScrollPos) / rowHeight[0];
	
	psoMenu.LoadMenuA(IDR_MENU2);
	CMenu *pPopup = psoMenu.GetSubMenu(0);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);


}


void TableGrid::OnSavedesign()
{
	GetParent()->SendMessage(UWM_SAVEDESIGN, (WPARAM)nDesign);
}


void TableGrid::OnExporttable()
{
	// TODO: Add your command handler code here
	GetParent()->SendMessage(UWM_EXPORTTABLE, (WPARAM)nDesign);
}


void TableGrid::OnExportparetofront()
{	
	GetParent()->SendMessage(UWM_EXPORTPARETO, (WPARAM)nDesign);
	// TODO: Add your command handler code here
}
