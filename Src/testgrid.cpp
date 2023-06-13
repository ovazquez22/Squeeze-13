// testgrid.cpp : implementation file
//

#include "stdafx.h"
#include "testgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// testgrid

testgrid::testgrid()
{
	nRows = 2;
	nCols = 2;
	
	nL = nC = 0;

	vScrollPos = 0;

	Header = 1;

	kTemp = 0; 
	
	Allocate();
	
	InitGrid();

	testgrid subgrid(2,10);

}

testgrid::testgrid(int r, int c)
{
	nRows = r;
	nCols = c;
	
	nL = nC = 0;

	Header = 1;
	
	kTemp = 0;

	Allocate();

	InitGrid();
}

testgrid::testgrid(int r, int c, int Lay, int Com)
{
	nRows = r;
	nCols = c;
	
	nL = Lay;

	nC = Com;
	
	Header = 1;
	
	kTemp = 0;

	Allocate();

	InitGrid();
}

testgrid::~testgrid()
{
	DeAllocate();
}

void testgrid::Allocate()
{

	colWidth = new int [nCols];
	rowHeight = new int [nRows];
	colPos = new int [nCols];
	rowPos = new int [nRows];
	cellEdit = new CELL_EDIT * [nCols];
	cellFormat = new CELL_FORMAT *[nCols];
	iList = new int * [nCols];
	hmerge = new int * [nCols];
	vmerge = new int * [nCols];

	for (int c = 0; c < nCols; c++)
	{
		colS[c].SetSize(nRows);
		cellEdit[c] = new CELL_EDIT [nRows];
		cellFormat[c] = new CELL_FORMAT [nRows];
		iList[c] = new int [nRows];
		hmerge[c] = new int [nRows];
		vmerge[c] = new int [nRows];
	}

	editHasFocus = false;
	
	Tab = PP1;
	
	cellC = cellR = 0; 
}

void testgrid::DeAllocate()
{
	for (int c = 0; c < nCols; c++)
	{
		delete[] cellEdit[c];
		delete[] cellFormat[c];
		delete[] iList[c];
		delete[] hmerge[c];
		delete[] vmerge[c];
	}

	delete[] cellEdit;
	delete[] cellFormat;
	delete[] colWidth;
	delete[] rowHeight;
	delete[] colPos;
	delete[] rowPos;
	delete[] iList;
	delete[] hmerge;
	delete[] vmerge;
}

testgrid& testgrid::operator=(const testgrid& rhs)
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


void testgrid::Equate(const testgrid& rhs)
{
	for (int c = 0; c < __min(nCols, rhs.nCols); c++)
	{
		colWidth[c] = rhs.colWidth[c];
		colPos[c] = rhs.colPos[c];
		for (int r = 0; r < __min(nRows, rhs.nRows); r++)
		{
//			cellD[c][r] = rhs.cellD[c][r];
			colS[c][r] = rhs.colS[c][r];
			cellEdit[c][r] = rhs.cellEdit[c][r];
		//	cellFormat[c][r] = rhs.cellFormat[c][r];
			iList[c][r] = rhs.iList[c][r];
			comboList[c].Copy(rhs.comboList[c]);
		//	hmerge[c][r] = rhs.hmerge[c][r];
		//	vmerge[c][r] = rhs.vmerge[c][r];
		}
	}
	for (int r = 0; r < __min(nRows, rhs.nRows); r++)
	{
		rowHeight[r] = rhs.rowHeight[r];
		rowPos[r] = rhs.rowPos[r];
	}
	
	nRows = rhs.nRows;
	
	nCols = rhs.nCols;

	Tab = rhs.Tab;

	nL = rhs.nL;

	nC = rhs.nC;

	Header = rhs.Header;
	
	Units = rhs.Units;
}

BEGIN_MESSAGE_MAP(testgrid, CStatic)
	//{{AFX_MSG_MAP(testgrid)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
//	ON_EN_SETFOCUS(IDC_EDIT_FOR_JCGRID, OnSetFocusEdit)
	ON_MESSAGE(UDM_EDIT_LOST_FOCUS, OnEditLostFocus)
	ON_MESSAGE(UDM_BOX_LOST_FOCUS, OnBoxLostFocus)
	ON_MESSAGE(UDM_LIST_CTRL_UPDATE, OnListCtrlUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// testgrid message handlers

void testgrid::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CStatic::OnPaint() for painting messages
	
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

	if (!vScroller)
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
	
	// create a memory dc, blt it to screen later to avoid flicker
	CDC mdc;
	if (!mdc.CreateCompatibleDC(&dc)) {
		AfxMessageBox(_T("CreateCompatibleDC failed in OnPaint"));
		return;
	}
	CBitmap mbitmap;
	if (!mbitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height())) {
		AfxMessageBox(_T("CreateCompatibleBitmap failed in OnPaint"));
		return;
	}
	
	CBitmap* pbmpOld = mdc.SelectObject(&mbitmap);
	mdc.FillRect(clientRect, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_WINDOW)));

/*	int cLeft = clientRect.Width()*1/80;
	int cRight = clientRect.Width()*79/80;
	int cTop = clientRect.Height()*1/40;
	int cBottom = clientRect.Height()*39/40;
*/
	DrawGrid(&mdc, clientRect);

	// copy onto screen
	if (!dc.BitBlt(0, 0, clientRect.Width(), clientRect.Height(), &mdc, 0, 0, SRCCOPY)) {
//		AfxMessageBox("BitBlt failed");
	}

	// clean up
	if (mdc.SelectObject(pbmpOld) == NULL) {
		AfxMessageBox(_T("SelectObject(pbmpOld) failed"));
		return;
	}
	mbitmap.DeleteObject();
	mdc.DeleteDC();

//	dc.SelectObject(pOldBrush);
}

void testgrid::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
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

void testgrid::Disabled()
{	
//	nDisabled = 0;

	int c,
		 r;

	for( c = 0 ; c < nCols ; c++ )
	{	
		for( r = 0 ; r < nRows ; r++ )
		{
			SetCellText( c, r, " ");
			
			cellEdit[c][r] = testgrid::READ_ONLY;

			cellFormat[c][r] = testgrid::GRAY;
		}
	}
	
}

void testgrid::DrawGrid(CDC* pDC, CRect rect)
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
	
	// If disabled then paint in Gray
	CBrush Gray(GetSysColor(COLOR_BTNFACE));
	if( !nDisabled )
		pDC->FillRect(rect, &Gray);
	else
		pDC->FillRect(rect, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));

	CBrush * pOldBrush;
	CBrush newBrush(RGB(0, 255, 0));
	CBrush hiliteBrush(GetSysColor(COLOR_INFOBK));
	CBrush grayBrush(GetSysColor(COLOR_BTNFACE));
	pOldBrush = pDC->SelectObject(&newBrush);
	CPen* pOldPen;
	CPen blackPen(PS_SOLID, 1, RGB(0, 0, 0));
	CPen grayPen(PS_SOLID, 1, GetSysColor(COLOR_BTNFACE));
	pOldPen = pDC->SelectObject(&grayPen);
//	pDC->Draw3dRect(rect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT));
//	pDC->Draw3dRect(innerRect, GetSysColor(COLOR_BTNSHADOW), GetSysColor(COLOR_BTNHILIGHT));

	CFont horizFont, titleFont, symbolFont;
	CFont verticalFont;
	horizFont.CreatePointFont(90, _T("Arial"));
	symbolFont.CreatePointFont(90, "Symbol");
	titleFont.CreatePointFont(90, _T("Arial"));
	verticalFont.CreateFont(2 + (9*rect.Height())/300, 0, 900, 0, 0, 0, 0, 0, 0, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, _T(_T("Arial")));
	CFont* pOldFont = pDC->SelectObject(&titleFont);

	pDC->SelectObject(&blackPen);
	
	int c, 
		 r;

	for ( c = 0; c < nCols; c++)
	{
		for ( r = 0; r < nRows; r++)
		{

			SetEditRect(c, r);
			innerRect = editRect;
			innerRect.DeflateRect(1, 1);
			//	cell formats
			switch (cellFormat[c][r])
			{	
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
			CString sCell = colS[c][r%colS[c].GetSize()];
			if (sCell.Find("symbol*", 0) != -1)
			{
				sCell.Replace("symbol*", "");
				pDC->SelectObject(&symbolFont);
			}
			pDC->DrawText(sCell, editRect-(LPCRECT(CRect(1, 1, 1, 1))),
				DT_VCENTER|DT_CENTER|DT_WORDBREAK);
			
/*			colS[c][r].Replace(_T("e+000"), _T("e"));
			colS[c][r].Replace(_T("e+00"), _T("e"));
			colS[c][r].Replace(_T("e+0"), _T("e"));
*/
			pDC->DrawText(colS[c][r], editRect-(LPCRECT(CRect(1, 1, 1, 1))),
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

void testgrid::CalculateColRowPos()
{
	int pos = 0;
	for (int c = 0; c < nCols; c++)
	{
		pos += colWidth[c];
		colPos[c] = pos;
	}
	pos = 0;
	for (int r = 0; r < nRows; r++)
	{
		pos += rowHeight[r];
		rowPos[r] = pos;
	}
}

void testgrid::SetEditRect(int c, int r)
{
	editRect.SetRect(
		clientRect.left + ((c == 0) ? 0 : colPos[c-1]),
		clientRect.top + ((r == 0) ? 0 - vScrollPos : rowPos[r-1] - vScrollPos),
		clientRect.left + colPos[c+__max(hmerge[c][r], 0)],
//		clientRect.top + rowPos[r+__max(vmerge[c][r], 0)] - vScrollPos
		clientRect.top + rowPos[r+vmerge[c][r]] - vScrollPos);
}

void testgrid::SetColWidth(int c, int width)
{	
	if(nCols != 0 )
		colWidth[c%nCols] = __max(width, 0);
}

void testgrid::SetRowHeight(int r, int height)
{	
	if(nRows != 0 )
		rowHeight[r%nRows] = __max(height, 0);
}

void testgrid::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	
	// TODO: Add your message handler code here and/or call default
	int i,r;

	if (!initialised) { return; }
	cellR = 0;
	cellC = 0;
	bool gotCell = false;
	
	// hit test - which cell clicked?
	for ( i = 0; i < nCols; i++)
	{
		for ( r = 0; r < nRows; r++)
		{
			if ((hmerge[i][r] > -1)&&(vmerge[i][r] > -1))
			{
				SetEditRect(i, r);
//				editRect.SetRect(clientRect.left + ((c == 0) ? 0 : colPos[c-1]),
//					clientRect.top + ((r == 0) ? 0 - vScrollPos : rowPos[r-1] - vScrollPos),
//					clientRect.left + colPos[c+hmerge[c][r]],
//					clientRect.top + rowPos[r+vmerge[c][r]] - vScrollPos);
				if (editRect.PtInRect(point) == TRUE)
				{
					gotCell = true;
					cellC = i;
					cellR = r;
					break;
				}
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
				int size = comboList[iList[cellC][cellR] % MAX_COLS].GetSize(),
					h;
				
				h = editRect.Height();
				editRect.InflateRect(0,0,0,h*size);

				theBox.MoveWindow(&editRect);
				theBox.ShowWindow(TRUE);
				theBox.ShowDropDown();
				theBox.SetFocus();
				boxHasFocus = true;
			}
		}

		else if ( cellEdit[cellC][cellR] == ISO )
		{	
			//IsoTable.ReadFileTableIso(IsoTable.PathName);
			IsoTable.ReadFileTableIso(colS[cellC][cellR]);

			IsoTable.TabGr.SetRowFormat(0, TableGrid::HEADER);
			IsoTable.TabGr.SetRowEdit(0, TableGrid::READ_ONLY);

			IsoTable.TabGr.rowHeight[0] = 35;
			
			if (IsoTable.bFileOpen)
			{	
				if ( IsoTable.DoModal() == IDOK )
				{	
					colS[cellC][cellR] = IsoTable.NameFile;
					Invalidate(FALSE);
				
				}
			
			}
			
	/*		theEdit.SetWindowText(IsoTable.NameFile);
			theEdit.MoveWindow(&editRect);
			theEdit.ShowWindow(TRUE);
			theEdit.SetFocus();
			theEdit.SetSel(0, -1);
			editHasFocus = true;*/
			Invalidate(FALSE);

		}
		
		else if ( cellEdit[cellC][cellR] == TABLE )
		{	
		
			RelPermTable.ReadFileTableRelPerm(colS[cellC][cellR]);

			RelPermTable.TabGr.SetRowFormat(0, TableGrid::HEADER);
			RelPermTable.TabGr.SetRowEdit(0, TableGrid::READ_ONLY);

			RelPermTable.TabGr.rowHeight[0] = 20;
			
			if (RelPermTable.bFileOpen)
			{	
				if ( RelPermTable.DoModal() == IDOK )
				{	
					colS[cellC][cellR] = RelPermTable.NameFile;
					Invalidate(FALSE);
				}
			}
			
		/*	theEdit.SetWindowText(IsoTable.NameFile);
			theEdit.MoveWindow(&editRect);
			theEdit.ShowWindow(TRUE);
			theEdit.SetFocus();
			theEdit.SetSel(0, -1);
			editHasFocus = true;*/
			Invalidate(FALSE);

		}

		else if ( cellEdit[cellC][cellR] == BICOND )
		{	
			for( i = 0 ; i<StgDlg.nComp ; i++ )
				
				StgDlg.compRole[i] = testgrid::compRole[i];
			
			StgDlg.nSt = (cellR - 1) / 2;
			
			StgDlg.Enable= TRUE;

			if( colS[cellC-2][cellR] == "Inj/Preflush")
				
				StgDlg.Stage[StgDlg.nSt].TypeStg = 1;
			
			if( colS[cellC-2][cellR] == "Overflush")
				
				StgDlg.Stage[StgDlg.nSt].TypeStg = 2;
						
			if( colS[cellC-2][cellR] == "Production")
			{	
				StgDlg.Stage[StgDlg.nSt].TypeStg = 3;
			}

			if( colS[cellC-2][cellR] == "Shut-in")
				
				StgDlg.Stage[StgDlg.nSt].TypeStg = 5;

			if ( StgDlg.checkPLsame == 1 )
			{	
				for ( int l = 0 ; l < StgDlg.nLay ; l++ )
				
					StgDlg.Stage[StgDlg.nSt].LayPres[l] = StgDlg.Stage[0].LayPres[l];
			}
		
			if ( StgDlg.DoModal() == IDOK )
			{	
				StgDlg.UpdateFromEdits();

				if ( StgDlg.Stage[StgDlg.nSt].TypeStg == 1 )
				
					colS[3][cellR].Format("[SI] %7.1f", StgDlg.Stage[StgDlg.nSt].InitialCw[0] );
				
//				if ( StgDlg.checkPLsame == 1 )
				Invalidate(FALSE);
			}
	
		}
	}
}

LRESULT testgrid::OnEditLostFocus(WPARAM w, LPARAM l)
{
//	AfxMessageBox("Edit Lost Focus");
	double Temp;

	theEdit.ShowWindow(FALSE);
	
	CString sEdit;
	
	theEdit.GetWindowText(sEdit);
	
	if ((sEdit.IsEmpty() == FALSE)&&(editHasFocus))
	{	
		
		sEdit.TrimRight();
		colS[cellC][cellR] = sEdit;
		editHasFocus = false;


		if( Tab == PP3 )
		{
			if( (cellC + 2) <= 8 )
			{
				if( cellEdit[cellC+1][cellR] == FTV || cellEdit[cellC+2][cellR] == FTV )
				{	
					if( colS[cellC-1][cellR] == "F/T" || colS[cellC-2][cellR] == "F/T")
					{	
						if( colS[1][cellR] == "Production" )
							
							Temp = atof(colS[5][cellR]) * atof(colS[6][cellR]);
						
						else
						{	
							Temp = atof(colS[5][cellR]) * atof(colS[6][cellR]);
							
							if (iModel == 3) Temp *= 60;
						}
						sEdit.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
						sEdit.Replace(_T("e+000"), _T("e"));
						sEdit.Replace(_T("e+00"), _T("e"));
						sEdit.Replace(_T("e+0"), _T("e"));
						colS[7][cellR] = sEdit;
					}
				}
			
			}

			if( (cellC + 1) <= 8 )
			{
				if( cellEdit[cellC+1][cellR] == FTV || cellEdit[cellC-1][cellR] == FTV )
				{	
					if( colS[cellC-1][cellR] == "F/V" || colS[cellC-3][cellR] == "F/V")
					{	
						if( colS[1][cellR] == "Production"  )

							Temp = atof(colS[7][cellR]) / atof(colS[5][cellR]);
						
						else
						{
							Temp = atof(colS[7][cellR]) / atof(colS[5][cellR]);
							
							if (iModel == 3) Temp /= 60; 
						}

						sEdit.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
						sEdit.Replace(_T("e+000"), _T("e"));
						sEdit.Replace(_T("e+00"), _T("e"));
						sEdit.Replace(_T("e+0"), _T("e"));
						colS[6][cellR] = sEdit;
					}
				}

			}
			
			if( cellEdit[cellC-1][cellR] == FTV || cellEdit[cellC-2][cellR] == FTV )
			{	
				if( colS[cellC-2][cellR] == "T/V" || colS[cellC-3][cellR] == "T/V")
				{
					if( colS[1][cellR] == "Production" )
					{
						Temp = atof(colS[7][cellR]) / atof(colS[6][cellR]);
					}
					else
					{
						Temp = atof(colS[7][cellR]) / atof(colS[6][cellR]);
						
						if (iModel == 3) Temp /= 60;
					}
					
					sEdit.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
					sEdit.Replace(_T("e+000"), _T("e"));
					sEdit.Replace(_T("e+00"), _T("e"));
					sEdit.Replace(_T("e+0"), _T("e"));
					colS[5][cellR] = sEdit;
				}
			}
		
		}

		CWnd* pWnd = 0;
		pWnd = GetParent();
		if (pWnd != NULL)
		{
			pWnd->PostMessage(UDM_LIST_CTRL_UPDATE);
		}

		Invalidate(FALSE);

	}

  return 0;
}

LRESULT testgrid::OnBoxLostFocus(WPARAM w, LPARAM l)
{		
	theBox.ShowWindow(FALSE);
	
	double Temp;

	CString sBox,
			  *sVal;
	
	sVal = new CString [nCols];

	for (int i = 0 ; i<nCols; i++)
		
		sVal[i] = colS[i][cellR];

	theBox.GetWindowText(sBox);
	
	int nTotal, 
		 r;

	nTotal = 2 * nL * nC;

//	AfxMessageBox("Box Lost Focus");
	
	if ((sBox.IsEmpty() == FALSE)&&(boxHasFocus))
	{
		sBox.TrimRight();
		colS[cellC][cellR] = sBox;
		boxHasFocus = false;
		
		if ( sBox == "Precipitation")
		{	
			if( colS[cellC+1][cellR] = "IsoTable" )
			{
				UnMerge(cellC+2, cellC+3, cellR, cellR);

				UnMerge(cellC+2, cellC+3, cellR-1, cellR-1);
				
				cellEdit[cellC+2][cellR] = EDIT;
				colS[cellC+2][cellR] = sVal[cellC+2];
				cellFormat[cellC+2][cellR-1] = HEADER;

				cellEdit[cellC+3][cellR] = EDIT;
				colS[cellC+3][cellR] = sVal[cellC+3];		
				cellFormat[cellC+3][cellR-1] = HEADER;
			}
			
			colS[cellC+1][cellR-1] = "Cr.Temp";
			cellEdit[cellC+1][cellR] = EDIT;
			colS[cellC+2][cellR-1] = "Cps1";
			colS[cellC+3][cellR-1] = "Cps2";

			if ( kTemp )
			{	
				cellFormat[cellC+1][cellR] = NORMAL;
			
				colS[cellC+1][cellR] = "1";				
				colS[cellC+2][cellR] = "1";
				colS[cellC+3][cellR] = "1";
			}
			else
			{
				cellEdit[cellC+1][cellR] = READ_ONLY;
				cellFormat[cellC+1][cellR] = GRAY;	
				colS[cellC+1][cellR] = "";

				cellEdit[cellC+3][cellR] = READ_ONLY;
				cellFormat[cellC+3][cellR] = GRAY;
				colS[cellC+3][cellR] = "";
			}
			
			colS[cellC+4][cellR-1] = "Prep.Type";
			colS[cellC+4][cellR] = "Kinetic";
			cellFormat[cellC+4][cellR] = GRAY;
			cellEdit[cellC+4][cellR] = READ_ONLY;
			
			cellFormat[cellC+5][cellR] = NORMAL;
			cellEdit[cellC+5][cellR] = EDIT;
			colS[cellC+5][cellR] = sVal[cellC+5];

			Invalidate(FALSE);
		}
		
		if ( sBox == "Adsorption")
		{	
			cellEdit[cellC+1][cellR] = COMBO;
			cellFormat[cellC+1][cellR] = NORMAL;	
				
			cellEdit[cellC+3][cellR] = EDIT;
			cellFormat[cellC+3][cellR] = NORMAL;

			colS[cellC+1][cellR-1] = "Isotherm";

			colS[cellC+1][cellR] = "Langmuir";

			colS[cellC+2][cellR-1] = "a";

			colS[cellC+2][cellR] = "1";

			colS[cellC+3][cellR-1] = "b";

			colS[cellC+3][cellR] = "1";

			colS[cellC+4][cellR-1] = "Adso.Type";
			cellFormat[cellC+4][cellR] = NORMAL;
			cellEdit[cellC+4][cellR] = COMBO;

			Invalidate(FALSE);
		}

		if ( sBox == "Table")
		{	
			cellEdit[cellC+1][cellR] = TABLE;
			cellEdit[cellC][cellR] = TABLE;

			Merge(cellC+1, cellC+6, cellR-1, cellR-1);
			
			cellFormat[cellC+2][cellR-1] = NORMAL;
			cellFormat[cellC+3][cellR-1] = NORMAL;
			cellFormat[cellC+4][cellR-1] = NORMAL;
			cellFormat[cellC+5][cellR-1] = NORMAL;
			cellFormat[cellC+6][cellR-1] = NORMAL;

			colS[cellC+2][cellR-1] = "";
			colS[cellC+3][cellR-1] = "";
			colS[cellC+4][cellR-1] = "";
			colS[cellC+5][cellR-1] = "";
			colS[cellC+6][cellR-1] = "";

			Merge(cellC+1, cellC+6, cellR, cellR);
		
			cellFormat[cellC+2][cellR] = NORMAL;
			cellFormat[cellC+3][cellR] = NORMAL;
			cellFormat[cellC+4][cellR] = NORMAL;
			cellFormat[cellC+5][cellR] = NORMAL;
			cellFormat[cellC+6][cellR] = NORMAL;
			
			colS[cellC+2][cellR] = "";
			colS[cellC+3][cellR] = "";
			colS[cellC+4][cellR] = "";
			colS[cellC+5][cellR] = "";
			colS[cellC+6][cellR] = "";

			colS[cellC+1][cellR-1] = "Relative Permeability Table";

			colS[cellC+2][cellR] = "";

		//	colS[cellC+1][cellR] = "";
			
			/*
			for ( r = 1; r < nRows; r++)
			{
				colS[1][r] = colS[1][cellR];
				
				cellEdit[cellC+1][r] = cellEdit[cellC+1][cellR] = TABLE;

				if( r>1 )
				{	
					cellEdit[1][r] = READ_ONLY;
					
					Merge(cellC+1, cellC+6, r, r);
		
					cellFormat[cellC+2][r] = NORMAL;
					cellFormat[cellC+3][r] = NORMAL;
					cellFormat[cellC+4][r] = NORMAL;
					cellFormat[cellC+5][r] = NORMAL;
					cellFormat[cellC+6][r] = NORMAL;
					
					colS[cellC+2][r] = "";
					colS[cellC+3][r] = "";
					colS[cellC+4][r] = "";
					colS[cellC+5][r] = "";
					colS[cellC+6][r] = "";
				
				}

			}
*/
			Invalidate(FALSE);
			
			OpenRelPermTable();

		}

		if ( sBox == "Corey" )
		{
			UnMerge(cellC+1, cellC+6, cellR-1, cellR-1);

			cellFormat[cellC+2][cellR-1] = HEADER;
			cellFormat[cellC+3][cellR-1] = HEADER;
			cellFormat[cellC+4][cellR-1] = HEADER;
			cellFormat[cellC+5][cellR-1] = HEADER;
			cellFormat[cellC+6][cellR-1] = HEADER;
			
			colS[cellC+1][cellR-1] = "a";
			colS[cellC+2][cellR-1] = "Alpha";
			colS[cellC+3][cellR-1] = "b";
			colS[cellC+4][cellR-1] = "Beta";
			colS[cellC+5][cellR-1] = "Irr. Water (Swi)";
			colS[cellC+6][cellR-1] = "Irr. Oil (Sor)";
			
			UnMerge(cellC+1, cellC+6, cellR, cellR);

			cellEdit[cellC+1][cellR] = EDIT;
			cellEdit[cellC+2][cellR] = EDIT;
			cellEdit[cellC+1][cellR] = EDIT;
			cellEdit[cellC+2][cellR] = EDIT;
			cellEdit[cellC+1][cellR] = EDIT;
			cellEdit[cellC+2][cellR] = EDIT;
			
			colS[cellC+1][cellR] = "1";
			colS[cellC+2][cellR] = "1";
			colS[cellC+3][cellR] = "1";
			colS[cellC+4][cellR] = "1";
			colS[cellC+5][cellR] = "0.0";
			colS[cellC+6][cellR] = "0.0";
	/*		
			for ( r = 1; r < nRows; r++)
			{
				colS[1][r] = colS[1][cellR];
				
				if( r>1 )
				{	
					cellEdit[1][r] = READ_ONLY;
					
					UnMerge(cellC+1, cellC+6, r, r);

					cellEdit[cellC+1][r] = EDIT;
					cellEdit[cellC+2][r] = EDIT;
					cellEdit[cellC+1][r] = EDIT;
					cellEdit[cellC+2][r] = EDIT;
					cellEdit[cellC+1][r] = EDIT;
					cellEdit[cellC+2][r] = EDIT;

					colS[cellC+1][r] = "1";
					colS[cellC+2][r] = "1";
					colS[cellC+3][r] = "1";
					colS[cellC+4][r] = "1";
					colS[cellC+5][r] = "0.2";
					colS[cellC+6][r] = "0.2";
				}
			}
*/
		   Invalidate(FALSE);
		
		}

		if ( sBox == "Tracer")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,0);

			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
						
					/*	for (int i=3 ; i<8 ; i++)
						{	
							colS[i][r] = " ";
							cellFormat[i][r] = GRAY;
							cellEdit[i][r] = READ_ONLY;
						}*/
					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Inhib.")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,1);

			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
					}
				}
			}

			Invalidate(FALSE);
	
		}

		if ( sBox == "Visco.")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,2);

			VisDlg.kTemp = kTemp;

			VisDlg.nComp = atoi(colS[1][cellR])-1;

			if ( VisDlg.DoModal() == IDOK )
			{
		
			}
			
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Blocker")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,3);

			BloDlg.check_FD = false;

			if ( BloDlg.DoModal() == IDOK )
			{
		//		AfxMessageBox("Not finished yet");
			}
			
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}
		
		if (sBox == "SI_FD")
		{
			testgrid::set_compRole(atoi(colS[1][cellR]) - 1, 3);

			BloDlg.check_FD = true;

			if (BloDlg.DoModal() == IDOK)
			{
				//		AfxMessageBox("Not finished yet");
			}

			if (nRows == nTotal)
			{
				for (r = 0; r < nTotal; r++)
				{
					if (r % (2 * nC) == cellR)
					{
						colS[2][r] = colS[2][cellR];

					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Surfact")
		{	
			MSDlg.MS_BA = true;

			testgrid::set_compRole(atoi(colS[1][cellR])-1,4);

			if ( MSDlg.DoModal() == IDOK )
			{
		//		AfxMessageBox("Not finished yet");
			}
			
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}
		
		if ( sBox == "Bridging")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,7);
		
			MSDlg.MS_BA = false; 

			if ( MSDlg.DoModal() == IDOK )
			{
		//		AfxMessageBox("Not finished yet");
			}
			
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Wax")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,8);

			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
						
					/*	for (int i=3 ; i<8 ; i++)
						{	
							colS[i][r] = " ";
							cellFormat[i][r] = GRAY;
							cellEdit[i][r] = READ_ONLY;
						}*/
					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "Ca")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,5);
			
			Invalidate(FALSE);
		}

		if ( sBox == "Ca")
		{	
			testgrid::compRole[atoi(colS[1][cellR])-1] = 5;
			
			if( colS[cellC+1][cellR] == "IsoTable")
			{
				UnMerge(cellC+2, cellC+3, cellR-1, cellR-1);
				UnMerge(cellC+2, cellC+3, cellR, cellR);
				
				cellEdit[cellC+1][cellR] = COMBO;
				colS[cellC+1][cellR] = "";

				cellEdit[cellC+2][cellR] = EDIT;
				colS[cellC+2][cellR] = "";

				cellEdit[cellC+3][cellR] = EDIT;
				colS[cellC+3][cellR] = "";
				
				cellFormat[cellC+1][cellR-1] = HEADER;
				colS[cellC+1][cellR-1] = "Isotherm";

				cellFormat[cellC+2][cellR-1] = HEADER;
				colS[cellC+2][cellR-1] = "k";

				cellFormat[cellC+3][cellR-1] = HEADER;
				colS[cellC+3][cellR-1] = "n";

			}
			
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellEdit[cellC+2][cellR] = READ_ONLY;
			cellEdit[cellC+3][cellR] = READ_ONLY;
			
			cellFormat[cellC+1][cellR] = GRAY;
			cellFormat[cellC+2][cellR] = GRAY;
			cellFormat[cellC+3][cellR] = GRAY;
			
//			if ( CaNaDlg.DoModal() == IDOK )
			{
		//		AfxMessageBox("Not finished yet");
			}

/*			Merge(cellC+1, cellC+3, cellR, cellR);
			Merge(cellC+1, cellC+3, cellR-1, cellR-1);
			
			cellFormat[cellC+1][cellR-1] = HEADER;
//			cellEdit[cellC+1][cellR] = CANAISO;
			colS[cellC+1][cellR-1] = "[Ca] Adsorption based on [Na]";
			colS[cellC+1][cellR] = "Click Here";
*//*
			colS[cellC+2][cellR] = "";
			colS[cellC+2][cellR-1] = "";
			
			colS[cellC+3][cellR] = "";
			colS[cellC+3][cellR-1] = "";
*/
/*			else
			{
				cellEdit[cellC+1][cellR] = CANAISO;
				
				cellFormat[cellC+2][cellR-1] = NORMAL;
				
				cellFormat[cellC+3][cellR-1] = NORMAL;
				
				colS[cellC+1][cellR-1] = "[Ca] Adsorption based on [Na]";
				colS[cellC+1][cellR] = "Click Here";

				colS[cellC+2][cellR] = "";
				colS[cellC+2][cellR-1] = "";
				
				colS[cellC+3][cellR] = "";
				colS[cellC+3][cellR-1] = "";
			}
*/
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}
		
		if ( sBox == "Na")
		{	
			testgrid::set_compRole(atoi(colS[1][cellR])-1,6);
			
			if( nRows == nTotal )
			{
				for ( r = 0; r < nTotal; r++)
				{
					if( r%(2*nC) == cellR )
					{
						colS[2][r] = colS[2][cellR];
				
					}
				}
			}

			Invalidate(FALSE);
		}

		if ( sBox == "IsoTable")
		{	
			Merge(cellC+1, cellC+2, cellR, cellR);

			Merge(cellC+1, cellC+2, cellR-1, cellR-1);
			
			cellEdit[cellC+1][cellR] = ISO;

			cellFormat[cellC+2][cellR-1] = NORMAL;
			
			colS[cellC+2][cellR-1] = "";

			colS[cellC+1][cellR-1] = "Iso Table";

			colS[cellC+2][cellR] = "";

		//	colS[cellC+1][cellR] = "";
			
			Invalidate(FALSE);

			OpenIsothermTable();
			
		}

		if ( sBox == "Freundlich" || sBox == "Langmuir" )
		{
			UnMerge(cellC+1, cellC+2, cellR, cellR);

			UnMerge(cellC+1, cellC+2, cellR-1, cellR-1);
			
			cellEdit[cellC+1][cellR] = EDIT;
			colS[cellC+1][cellR] = sVal[cellC+1];

			cellEdit[cellC+2][cellR] = EDIT;
			colS[cellC+2][cellR] = sVal[cellC+2];
			
			cellFormat[cellC+2][cellR-1] = HEADER;

			if( sBox == "Freundlich" )
			{
				colS[cellC+1][cellR-1] = "k";
				colS[cellC+2][cellR-1] = "n";
			}
			
			if( sBox == "Langmuir" )
			{
				colS[cellC+1][cellR-1] = "a";
				colS[cellC+2][cellR-1] = "b";
			}
			
			testgrid::compAdTy[atoi(colS[1][cellR])-1] = 0;

			Invalidate(FALSE);
		}

		if ( sBox == "No Adso" )
		{	
			testgrid::compAdTy[atoi(colS[1][cellR])-1] = 1;

			Invalidate(FALSE);
		}
		
		if ( sBox == "Equilibrium" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			colS[cellC+1][cellR] = "";
			Invalidate(FALSE);
		}
		
		if ( sBox == "Kinetic" || sBox == "Kinetic2")
		{	
			cellEdit[cellC+1][cellR] = EDIT;
			cellFormat[cellC+1][cellR] = NORMAL;
			colS[cellC+1][cellR] = sVal[cellC+1];
			Invalidate(FALSE);
		}
		
		if ( sBox == "None" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			colS[cellC+1][cellR] = "";
			Invalidate(FALSE);
		}
		
		if ( sBox == "Equ" )
		{	
			cellEdit[cellC+1][cellR] = EDIT;
			cellFormat[cellC+1][cellR] = NORMAL;
			colS[cellC+1][cellR] = "0.5";
			Invalidate(FALSE);
		}
		
		if ( sBox == "Inj/Preflush" )
		{	
			if(!StgDlg.nDisabled)
			{
				if(kTemp) // Temperature
				{
					cellEdit[cellC+1][cellR] = EDIT;
					cellFormat[cellC+1][cellR] = NORMAL;
					colS[cellC+1][cellR] = "20";
				}
				else // Temperature
				{
					cellEdit[cellC+1][cellR] = READ_ONLY;
					cellFormat[cellC+1][cellR] = GRAY;
					colS[cellC+1][cellR] = "";
				}
			}
			else // Two Phase
			{
				cellEdit[cellC+1][cellR] = EDIT;
				cellFormat[cellC+1][cellR] = NORMAL;
				colS[cellC+1][cellR] = sVal[cellC+1];
			}

			cellEdit[cellC+2][cellR] = BICOND;
			cellFormat[cellC+2][cellR] = NORMAL;
			colS[cellC+2][cellR] = "Click here";

			cellEdit[cellC+3][cellR] = COMBO;
			cellFormat[cellC+3][cellR] = NORMAL;
			colS[cellC+3][cellR] = "F/V";
			
			cellEdit[cellC+4][cellR] = EDIT;
			cellFormat[cellC+4][cellR] = NORMAL;
			colS[cellC+4][cellR] = sVal[cellC+4];

			cellEdit[cellC+6][cellR] = EDIT;
			cellFormat[cellC+6][cellR] = NORMAL;
			
			cellEdit[cellC+5][cellR] = FTV;
			cellFormat[cellC+5][cellR] = GRAY;
			
			if (Header)
			{
				SetCellText(5,cellR-1 , CString("Flow\n ")+CString( (Units) ? "bbl/min" : "m3/min") );
				SetCellText(6, cellR-1, "Time \nhours");
				SetCellText(7, cellR-1, CString("Vol\n ")+CString((Units) ? "bbl" : "m3") );
				SetCellText(8, cellR-1, "T. Step \nhours");
				
				if ( iModel == 4 || iModel == 5 )
				{	
					SetCellText(5, cellR-1, CString("Flow\n ")+"ml/hour" );
					SetCellText(6, cellR-1, "Time hour");
					SetCellText(7, cellR-1, CString("Vol\n ") + "ml" );
					SetCellText(8, cellR-1, "T. Step \nhour");
				}
			}

			Temp = atof(colS[5][cellR]) * atof(colS[6][cellR]);
			
			if (iModel == 3) Temp *= 60;
			
			sBox.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			SetCellText(7, cellR, sBox);
			
			int n = 0,
				 temp;
			
			temp = cellR;

			while(temp>1)
			{
				temp -= 2;
				n++;
			}
			
			colS[cellC+2][cellR].Format("[SI] %7.1f", StgDlg.Stage[n].InitialCw[0] );

			Invalidate(FALSE);
		}
		
		if ( sBox == "Overflush" )
		{	
			if(!StgDlg.nDisabled)
			{
				if(kTemp) // Temperature
				{
					cellEdit[cellC+1][cellR] = EDIT;
					cellFormat[cellC+1][cellR] = NORMAL;
					colS[cellC+1][cellR] = "20";
				}
				else // Temperature
				{
					cellEdit[cellC+1][cellR] = READ_ONLY;
					cellFormat[cellC+1][cellR] = GRAY;
					colS[cellC+1][cellR] = "";
				}
			}
			else // Two Phase
			{
				cellEdit[cellC+1][cellR] = EDIT;
				cellFormat[cellC+1][cellR] = NORMAL;
				colS[cellC+1][cellR] = sVal[cellC+1];
			}

			cellEdit[cellC+2][cellR] = BICOND;
			cellFormat[cellC+2][cellR] = NORMAL;
			colS[cellC+2][cellR] = "Click here";

			cellEdit[cellC+3][cellR] = COMBO;
			cellFormat[cellC+3][cellR] = NORMAL;
			colS[cellC+3][cellR] = "F/V";
			
			cellEdit[cellC+4][cellR] = EDIT;
			cellFormat[cellC+4][cellR] = NORMAL;
			colS[cellC+4][cellR] = sVal[cellC+4];


			cellEdit[cellC+6][cellR] = EDIT;
			cellFormat[cellC+6][cellR] = NORMAL;
			
			cellEdit[cellC+5][cellR] = FTV;
			cellFormat[cellC+5][cellR] = GRAY;
			
			if (Header)
			{
				SetCellText(5,cellR-1 , CString("Flow\n ")+CString( (Units) ? "bbl/min" : "m3/min") );
				SetCellText(6, cellR-1, "Time \nhours");
				SetCellText(7, cellR-1, CString("Vol\n ")+CString((Units) ? "bbl" : "m3") );
				SetCellText(8, cellR-1, "T. Step \nhours");
				
				if ( iModel == 4 || iModel == 5 )
				{	
					SetCellText(5, cellR-1, CString("Flow\n ")+"ml/hour" );
					SetCellText(6, cellR-1, "Time hour");
					SetCellText(7, cellR-1, CString("Vol\n ") + "ml" );
					SetCellText(8, cellR-1, "T. Step \nhour");
				}
			}

			Temp = atof(colS[5][cellR]) * atof(colS[6][cellR]);
			
			if (iModel == 3) Temp *= 60;
			
			sBox.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			SetCellText(7, cellR, sBox);

			Invalidate(FALSE);
		}
		
		if ( sBox == "Shut-in" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			SetCellText(cellC+1, cellR, " ");

			cellEdit[cellC+2][cellR] = BICOND;
			cellFormat[cellC+2][cellR] = NORMAL;
			SetCellText(cellC+2, cellR, "Pres.Lay");

			cellEdit[cellC+3][cellR] = READ_ONLY;
			cellFormat[cellC+3][cellR] = GRAY;
			SetCellText(cellC+3, cellR, " ");

			SetCellText(5,cellR-1 , "Flow" );
			cellEdit[cellC+5][cellR] = EDIT;
			cellFormat[cellC+5][cellR] = NORMAL;
			SetCellText(5, cellR, " ");

			cellEdit[5][cellR] = READ_ONLY;
			cellFormat[5][cellR] = GRAY;

			if (Header)
			{
				SetCellText(6, cellR-1, "Time hours");
			
				SetCellText(7, cellR-1, "Vol" );
				cellEdit[7][cellR] = READ_ONLY;
				cellFormat[7][cellR] = GRAY;
				SetCellText(cellC+6, cellR, " ");

				SetCellText(8, cellR-1, "T.Step");
			}
	/*		cellEdit[8][cellR] = READ_ONLY;
			cellFormat[8][cellR] = GRAY;
*/
			Invalidate(FALSE);
		}

		if ( sBox == "Production" )
		{	
			cellEdit[cellC+1][cellR] = READ_ONLY;
			cellFormat[cellC+1][cellR] = GRAY;
			colS[cellC+1][cellR] = "";

			cellEdit[cellC+2][cellR] = BICOND;
			cellFormat[cellC+2][cellR] = NORMAL;
			colS[cellC+2][cellR] = "Click here";

			cellEdit[cellC+3][cellR] = COMBO;
			cellFormat[cellC+3][cellR] = NORMAL;
			colS[cellC+3][cellR] = "F/V";
			
			cellEdit[cellC+4][cellR] = EDIT;
			cellFormat[cellC+4][cellR] = NORMAL;
			colS[cellC+4][cellR] = sVal[cellC+4];

			cellEdit[cellC+6][cellR] = EDIT;
			cellFormat[cellC+6][cellR] = NORMAL;
			
			cellEdit[cellC+5][cellR] = FTV;
			cellFormat[cellC+5][cellR] = GRAY;
			
			if (Header)
			{
				SetCellText(5, cellR-1, CString("Flow\n ")+CString((Units) ? "bbl/day" : "m3/day") );
				SetCellText(6, cellR-1, "Time day");
				SetCellText(7, cellR-1, CString("Vol\n ")+CString((Units) ? "bbl" : "m3") );
				SetCellText(8, cellR-1, "T. Step \nday");
				
				if ( iModel == 4 || iModel == 5 )
				{	
					SetCellText(5, cellR-1, CString("Flow\n ")+"ml/hour" );
					SetCellText(6, cellR-1, "Time hour");
					SetCellText(7, cellR-1, CString("Vol\n ") + "ml" );
					SetCellText(8, cellR-1, "T. Step \nhour");
				}
			}

			Temp = atof(colS[5][cellR]) * atof(colS[6][cellR]);
		
			sBox.Format(Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			SetCellText(7, cellR, sBox);

			Invalidate(FALSE);
		}
		
		if ( sBox == "F/T" )
		{	
			cellEdit[cellC+1][cellR] = EDIT;
			cellFormat[cellC+1][cellR] = NORMAL;
			cellEdit[cellC+2][cellR] = EDIT;
			cellFormat[cellC+2][cellR] = NORMAL;

			cellEdit[cellC+3][cellR] = FTV;
			cellFormat[cellC+3][cellR] = GRAY;
			
			if ( colS[cellC-3][cellR] == "Production")

				Temp = atof(colS[cellC+1][cellR]) * atof(colS[cellC+2][cellR]);

			else
			{	
				Temp = atof(colS[cellC+1][cellR]) * atof(colS[cellC+2][cellR]);
				
				if (iModel == 3) Temp *= 60;
			}

			sBox.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			colS[cellC+3][cellR] = sBox;
			Invalidate(FALSE);
		}
		
		if ( sBox == "T/V" )
		{	
			cellEdit[cellC+2][cellR] = EDIT;
			cellFormat[cellC+2][cellR] = NORMAL;
			cellEdit[cellC+3][cellR] = EDIT;
			cellFormat[cellC+3][cellR] = NORMAL;

			cellEdit[cellC+1][cellR] = FTV;
			cellFormat[cellC+1][cellR] = GRAY;

			if ( colS[cellC-3][cellR] == "Production")

				Temp = atof(colS[cellC+3][cellR]) / atof(colS[cellC+2][cellR]);

			else
			{
			//	Temp = atof(colS[cellC+3][cellR]) / ( atof(colS[cellC+2][cellR]) );
				double T1, T2;

				T1 = atof(colS[cellC+3][cellR]);
				
				T2 = atof(colS[cellC+2][cellR]);
				
				Temp = T1 / T2;

				if (iModel == 3) Temp /= 60;
			}

			sBox.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			colS[cellC+1][cellR] = sBox;
			Invalidate(FALSE);
		}

		if ( sBox == "F/V" )
		{	
			cellEdit[cellC+1][cellR] = EDIT;
			cellFormat[cellC+1][cellR] = NORMAL;
			cellEdit[cellC+3][cellR] = EDIT;
			cellFormat[cellC+3][cellR] = NORMAL;

			cellEdit[cellC+2][cellR] = FTV;
			cellFormat[cellC+2][cellR] = GRAY;

			if ( colS[cellC-3][cellR] == "Production")
				
				Temp = atof(colS[cellC+3][cellR]) / atof(colS[cellC+1][cellR]);

			else
			{
				Temp = atof(colS[cellC+3][cellR]) / atof(colS[cellC+1][cellR]);
				
				if (iModel == 3) Temp /= 60;
			}

			sBox.Format( Temp <= (1e6-1) ? "%.6g":"%.2e",Temp);
			sBox.Replace(_T("e+000"), _T("e"));
			sBox.Replace(_T("e+00"), _T("e"));
			sBox.Replace(_T("e+0"), _T("e"));

			colS[cellC+2][cellR] = sBox;
			Invalidate(FALSE);
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


void testgrid::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	CStatic::OnLButtonUp(nFlags, point);
}

void testgrid::InitGrid()
{	
	CString sItem;
	Units = 0;
	nDisabled = 1;

	for (int r = 0; r < nRows; r++)
	{
		for (int c = 0; c < nCols; c++)
		{
			colWidth[c] = 60;
			hmerge[c][r] = 0;
			vmerge[c][r] = 0;
			
		
			sItem.Format("(%d,%d)", r,c);
			colS[c][r] = sItem;
			
/*			if (r%2 == 0 )
			{
				cellEdit[c][r] = READ_ONLY;
				cellFormat[c][r] = GRAY;
			}
			
			if (r%2 == 1 )
*/			{
				cellEdit[c][r] = EDIT;
				//cellFormat[c][r] = EDIT;
			}

			iList[c][r] = 0;
		}

		rowHeight[r] = 22;
	}
	
	CalculateColRowPos();

/*	cellEdit[0][1] = COMBO;
	comboList[0].Add("Freundlich");
	comboList[0].Add("Langmuir");
	comboList[0].Add("Table");*/
}

LRESULT testgrid::OnListCtrlUpdate(WPARAM w, LPARAM l)
{
//	UpdateFromListCtrl();
	AfxMessageBox("You've got it");
	return 0;
}

void testgrid::OpenIsothermTable()
{	

	CString openFilter = "Adsorption Isotherm Table (*.txt)|*.txt||";
	
	CFileDialog openDialog(TRUE, "txt", IsoTable.PathName+CString("Isotherms\\TableAdso.txt"), OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST, openFilter, this);
	
	colS[cellC+1][cellR-1] = "Iso Table";

	if (openDialog.DoModal() == IDOK)
	{
		IsoTable.NameFile = openDialog.GetFileName();
		
		colS[cellC+1][cellR] = IsoTable.NameFile;

		colS[cellC+2][cellR] = "";

		colS[cellC+2][cellR-1] = "";

		Invalidate(FALSE);

	}

	else colS[cellC+1][cellR] = IsoTable.NameFile;


}

void testgrid::OpenRelPermTable()
{	
	RelPermTable.bRelPerm = TRUE;
	
	colS[cellC+1][cellR] = RelPermTable.NameFile;

	CString openFilter = "Relative Permeability Table (*.txt)|*.txt||";
	
	CFileDialog openDialog(TRUE, "txt", RelPermTable.PathName+CString("Relative Permeability\\RelPerm.txt"), OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{	
		RelPermTable.NameFile = openDialog.GetFileName();

		colS[cellC+1][cellR] = openDialog.GetFileName();

		colS[cellC+2][cellR] = "";

		colS[cellC+2][cellR-1] = "";

		Invalidate(FALSE);

	}

	else colS[cellC+1][cellR] = RelPermTable.NameFile;


}
void testgrid::Copy(const testgrid &rhs)
{
	for (int c = 0; c < __min(nCols, rhs.nCols); c++)
	{
		colWidth[c] = rhs.colWidth[c];
		colPos[c] = rhs.colPos[c];
		for (int r = 0; r < __min(nRows, rhs.nRows); r++)
		{
//			cellD[c][r] = rhs.cellD[c][r];
			colS[c][r] = rhs.colS[c][r];
			cellEdit[c][r] = rhs.cellEdit[c][r];
		//	cellFormat[c][r] = rhs.cellFormat[c][r];
			iList[c][r] = rhs.iList[c][r];
			comboList[c].Copy(rhs.comboList[c]);
		//	hmerge[c][r] = rhs.hmerge[c][r];
		//	vmerge[c][r] = rhs.vmerge[c][r];
		}
	}
	for (int r = 0; r < __min(nRows, rhs.nRows); r++)
	{
		rowHeight[r] = rhs.rowHeight[r];
		rowPos[r] = rhs.rowPos[r];
	}
}

void testgrid::SetCellText(int c, int r, CString txt)
{
	if(nCols !=0 && nRows != 0 )
		
		colS[c%nCols][r%nRows] = txt;
}

void testgrid::SetColEdit(int c, CELL_EDIT edit)
{
	for (int r = 0; r < nRows; r++)
	{
		cellEdit[c%nCols][r] = edit;
	}
}

void testgrid::SetRowEdit(int r, CELL_EDIT edit)
{
	for (int c = 0; c < nCols; c++)
	{
		if( nRows != 0 )
			
			cellEdit[c][r%nRows] = edit;
	}
}

void testgrid::SetColFormat(int c, CELL_FORMAT fmt)
{
	for (int r = 0; r < nRows; r++)
	{	
		if( nCols != 0 )
			
			cellFormat[c%nCols][r] = fmt;
	}
}

void testgrid::SetRowFormat(int r, CELL_FORMAT fmt)
{
	for (int c = 0; c < nCols; c++)
	{	
		if(nRows != 0 )
		 
			cellFormat[c][r%nRows] = fmt;
	}
}

void testgrid::Merge(int c1, int c2, int r1, int r2)
{
	r2 = __max(r2, r1);
	c2 = __max(c2, c1);
	if ((c1 < 0)||(c2 >= nCols)||(r1 < 0)||(r2 >= nRows))
	{
		AfxMessageBox("Error: Merge() - out of range argument");
		return;
	}
	// all merged cells
	for (int c = c1; c <= c2; c++)
	{
		for (int r = r1; r <= r2; r++)
		{
			hmerge[c][r] = -1;
			vmerge[c][r] = -1;
		}
	}
	// top left cell
	hmerge[c1][r1] = c2 - c1;
	vmerge[c1][r1] = r2 - r1;
}

void testgrid::UnMerge(int c1, int c2, int r1, int r2)
{
	r2 = __max(r2, r1);
	c2 = __max(c2, c1);
	if ((c1 < 0)||(c2 >= nCols)||(r1 < 0)||(r2 >= nRows))
	{
		AfxMessageBox("Error: Merge() - out of range argument");
		return;
	}
	// all merged cells
	for (int c = c1; c <= c2; c++)
	{
		for (int r = r1; r <= r2; r++)
		{
			hmerge[c][r] = 0;
			vmerge[c][r] = 0;
		}
	}
	
}

void testgrid::SetColRowComboList(int c, int r, int list)
{
	if ((list < 0)||(list >= MAX_COLS))
	{
		AfxMessageBox("Error: out of range in JCGrid::SetColComboList()");
		return;
	}
	
	iList[c][r] = list;
	
}
