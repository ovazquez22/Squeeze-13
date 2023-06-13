// GraphicsBlock.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "GraphicsBlock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const COLORREF GraphicsBlock::m_crColors[8] = {
	RGB(	0	,	0	,	50	),
	RGB(	0	,	0	,	100	),
	RGB(	0	,	0	,	150	),
	RGB(	0	,	0	,	200	),
	RGB(	0	,	0	,	255	),
	RGB(	85	,	85	,	255	),
	RGB(	170	,	170	,	255	),
	RGB(	255	,	255	,	255	)  // White
};

/////////////////////////////////////////////////////////////////////////////
// GraphicsBlock dialog


GraphicsBlock::GraphicsBlock(CWnd* pParent /*=NULL*/)
	: CDialog(GraphicsBlock::IDD, pParent)
{
	//{{AFX_DATA_INIT(GraphicsBlock)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	xMax = yMax = 1.0;
	xMin = yMin = 0.0;
	nBlocks = 10;
	nLay = 1;
	Allocate();
}


void GraphicsBlock::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GraphicsBlock)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GraphicsBlock, CDialog)
	//{{AFX_MSG_MAP(GraphicsBlock)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COPYTOCLIPBOARD, &GraphicsBlock::OnCopytoclipboard)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GraphicsBlock message handlers

void GraphicsBlock::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	GraphicsBlock *pWnd = (GraphicsBlock*)GetParent();
	
	GetClientRect(clientRect);
	
//	if (bGraphs)
	{
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

		DrawAxis(&mdc, clientRect);
		
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
	// Do not call CDialog::OnPaint() for painting messages
}

void GraphicsBlock::UpdateInterface(CString sEdit)
{
	CClientDC dc(this);

	GetClientRect(m_rcClient);

	if(m_pMemDC.GetSafeHdc())
		return;

	if(!m_pMemDC.CreateCompatibleDC(&dc))
		return;

	CBitmap bmp;
	if(!bmp.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height()))
		return;

	CBitmap* pbmpOld = m_pMemDC.SelectObject(&bmp);
		m_pMemDC.FillRect(m_rcClient, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_WINDOW)));


	if(!m_pMemDC.SelectObject(bmp))
		return;
	
	Stage = sEdit;

	DrawAxis(&m_pMemDC, m_rcClient);

	// copy onto screen
	if (!dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_pMemDC, 0, 0, SRCCOPY)) {
		AfxMessageBox("BitBlt failed");
	}

	// clean up
	if (m_pMemDC.SelectObject(pbmpOld) == NULL) {
		AfxMessageBox("SelectObject(pbmpOld) failed");
		return;
	}

	bmp.DeleteObject();

	m_pMemDC.DeleteDC();
}

void GraphicsBlock::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	UpdateInterface(Stage);
}

void GraphicsBlock::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: Add your message handler code here
	// Load the desired menu
	CMenu mnuPopupSubmit;
	if (mnuPopupSubmit.LoadMenu(IDR_MENU1))
	{
		CMenu *mnuPopupMenu = mnuPopupSubmit.GetSubMenu(0);
		ASSERT(mnuPopupMenu != NULL);
		mnuPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
}

void GraphicsBlock::DrawAxis(CDC *pDC, CRect rect)
{
	COLORREF color = RGB( 255, 255, 255);     // White
	CBrush lSolidBrush(color);
	pDC->FillRect(rect,&lSolidBrush);

	CRect axisRect,
			blockRect;
	
	int cLeft = rect.Width()*20/80,
		 cRight = rect.Width()*77/80,
		 cTop = rect.Height()*3/40,
		 cBottom = rect.Height()*34/40;

	axisRect.SetRect(cLeft, cTop, cRight, cBottom);

	// Blocks 
	blockRect.SetRect(rect.Width()*7/80, cTop, rect.Width()*12/80, cBottom);;
	
	DrawBlocks(pDC,blockRect,rect.Width()*4/80);
	// End Blocks
	
	CFont titleFont;
	
	titleFont.CreatePointFont(5*rect.Height()*3/40, _T("Arial"));
	
	CFont* pOldFont = pDC->SelectObject(&titleFont);
	
	CRect titleRect;
	
	CSize StageSize = pDC->GetTextExtent(Stage);
	titleRect.SetRect( int(rect.left + 3), rect.top+1, int(rect.left + StageSize.cx + 3), (rect.top+1+StageSize.cy));
	pDC->DrawText(Stage, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);

	if (title != "")
	{
		CSize titleSize = pDC->GetTextExtent(title);
		titleRect.SetRect((axisRect.left+axisRect.right)/2-titleSize.cx, rect.top+1, (axisRect.left+axisRect.right)/2+titleSize.cx, (rect.top+1+titleSize.cy));
		pDC->DrawText(title, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	}

	CFont yLabelFont;

	yLabelFont.CreateFont( rect.Width()*3/80, 0, 900, 0, 0, 0, 0, 0, 0, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, _T("Arial"));
	
	
	pDC->SelectObject(&yLabelFont);
	CRect yLabelRect;
	if (yLabel != "")
	{
		CSize yLabelSize = pDC->GetTextExtent(yLabel);
		yLabelRect.SetRect(rect.Width()*13/80, (axisRect.bottom+axisRect.top+yLabelSize.cx)/2, rect.left+yLabelSize.cy, (axisRect.bottom+axisRect.top-yLabelSize.cx)/2);
		pDC->DrawText(yLabel, yLabelRect, DT_NOCLIP);
	}
	
	CFont xLabelFont;

	xLabelFont.CreatePointFont(5*rect.Height()*3/40, _T("Arial"));

	CRect xLabelRect;
	pDC->SelectObject(&xLabelFont);
	if (xLabel != "")
	{
		CSize xLabelSize = pDC->GetTextExtent(xLabel);
		xLabelRect.SetRect((axisRect.left+axisRect.right-xLabelSize.cx)/2, rect.bottom-xLabelSize.cy-4, (axisRect.left+axisRect.right+xLabelSize.cx)/2, (rect.bottom-xLabelSize.cy-2));
		pDC->DrawText(xLabel, xLabelRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	}

	CPoint pAxisTL, 
		    pAxisBR;

	pAxisTL = axisRect.TopLeft( );
	pAxisBR = axisRect.BottomRight( );

	xPMax = pAxisBR.x;
	xPMin = pAxisTL.x;
	yPMax = pAxisBR.y;
	yPMin = pAxisTL.y;

	DrawTics(pDC,axisRect);
	
	CPen penRectangleB( PS_SOLID, 1.9 , RGB(255,0,0));
	pDC->SelectObject(&penRectangleB);
	
	pDC->MoveTo( xPMin , yPMax);
	
	pDC->LineTo( xPMax , yPMax);

	pDC->LineTo( xPMax , yPMin);

	pDC->MoveTo( xPMin , yPMax);
	
	pDC->LineTo( xPMin ,  yPMin );

	pDC->LineTo( xPMax ,  yPMin );

	

}

void GraphicsBlock::DrawTics(CDC *pDC, CRect rect)
{
	int i,
		 l;

	CPoint pAxisBR, 
		    pxMaxTL;

	CSize scaleSize;
	CRect scaleRect;
	CString xAxisScale;

	CPen penGrid( PS_DOT, 0.3 , RGB(200,200,200));
	CPen penAxis( PS_SOLID, 0.9 , RGB(230,0,75));

	int nXTics,
		 xpt,
		 ypt,
		 n1,
		 n2;

	double xTic,
			 DxTic,			
			 TempDxTic,
			 xMaxTic;
	
	// X axis

	DxTic = log10(xMax - xMin);

	DxTic = pow(10.0,int(DxTic));

	nXTics = int((xMax - xMin)/DxTic);

	xMaxTic = DxTic * nXTics;

	n1 = int(2 * (xMax - xMin)/DxTic);

	n2 = int(5 * (xMax - xMin)/DxTic);

	TempDxTic = DxTic;
	
	if ( abs(5-nXTics) > abs(5-n1) )
	{
		nXTics = n1; 

		TempDxTic = DxTic / 2;
	
		xMaxTic = DxTic * nXTics;;
	}

	if( abs(5-nXTics) > abs(5-n2) )
	{
		nXTics = n2;
		
		TempDxTic = DxTic / 5;
	
		xMaxTic = DxTic * nXTics;;
	}
	
	DxTic = TempDxTic;

	xTic = xMin;

	CFont xAxisFont, yAxisFont;
	
	int xFont,
		 yFont;

	if((6 + (70*rect.Width())/300) > 130) xFont = 130;

	else xFont = (6 + (70*rect.Width())/300);

	if((10 + (100*rect.Height())/300) > 140) yFont = 140;

	else yFont = (10 + (100*rect.Height())/300);
	
	xAxisFont.CreatePointFont( xFont, _T("Arial"));

	ypt = yPMax;

	for ( i = 0; i<=nXTics ; i++)
	{	
		xpt = int( ((xPMax-xPMin)/(xMax - xMin)) * (xTic-xMin) + xPMin );
		
		ypt = yPMax;	
		
/*		// Draw X Grid
 
		pDC->SelectObject(&penGrid);
		
		if( i > 0 && i <= nXTics )
		{
			pDC->MoveTo( xpt , yPMin );	
			pDC->LineTo( xpt, yPMax);
		}
*/
		pDC->SelectObject(&penAxis);
		
		pDC->MoveTo( xpt, yPMax );	
		pDC->LineTo( xpt, yPMax + 3);
		
		pDC->SelectObject(&xAxisFont);
		xAxisScale.Format("%.2g",xTic);
/*		xAxisScale.Replace(_T("e+000"), _T("e+"));
		xAxisScale.Replace(_T("e+00"), _T("e+"));
		xAxisScale.Replace(_T("e+0"), _T("e+"));
	*/	scaleSize = pDC->GetTextExtent(xAxisScale);
		
		if( scaleSize.cx%2 == 0 )
		{
			scaleRect.SetRect(xpt-scaleSize.cx/2, ypt+3, xpt+scaleSize.cx/2, ypt+3+scaleSize.cy);
		}	
		else
		{
			scaleRect.SetRect(xpt-int(scaleSize.cx/2)-1, ypt+3, xpt+scaleSize.cx/2, ypt+3+scaleSize.cy);
		}
		
		pAxisBR = scaleRect.BottomRight( );

		CBrush WhiteBrush(RGB(255,255,255));
		pDC->FillRect(scaleRect,&WhiteBrush);
		pDC->DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);

		xTic += DxTic; 
	
	}
	
	// Print the xMax 
	CRect scaleRectxMax;
	xAxisScale.Format("%.2g",xMax);
	xAxisScale.Replace(_T("e+000"), _T("e+"));
	xAxisScale.Replace(_T("e+00"), _T("e+"));
	xAxisScale.Replace(_T("e+0"), _T("e+"));
	scaleSize = pDC->GetTextExtent(xAxisScale);

	if( scaleSize.cx%2 == 0 )
	{
		scaleRectxMax.SetRect(xPMax-scaleSize.cx/2, yPMax+3, xPMax+scaleSize.cx/2, yPMax+3+scaleSize.cy);
	}	
	else
	{
		scaleRectxMax.SetRect(xPMax-int(scaleSize.cx/2)-1, yPMax+3, xPMax+scaleSize.cx/2, yPMax+3+scaleSize.cy);
	}
	
	pxMaxTL = scaleRectxMax.TopLeft( );

	if ( pxMaxTL.x >= (pAxisBR.x+5) )
	{
		CBrush WhiteBrush(RGB(255,255,255));
		pDC->FillRect(scaleRectxMax,&WhiteBrush);
		pDC->SelectObject(&xAxisFont);
		pDC->DrawText(xAxisScale, scaleRectxMax, DT_CENTER|DT_VCENTER);
		
		pDC->MoveTo( xPMax, yPMax );	
		pDC->LineTo( xPMax, ypt + 3);
	}

	// Y Axis

	yAxisFont.CreatePointFont( int( yFont * (1 - 0.05 * GraphXY.nLay) ) , _T("Arial"));
	
	double Height = 0,
		    TotalHeight = 0;

	for ( l = 0; l<=GraphXY.nLay ; l++)
	{			
		ypt = int( ((yPMin-yPMax)/(yMax - yMin))*(yMin-Height) + yPMin );

		pDC->SelectObject(&penGrid);
	
		pDC->SelectObject(&penAxis);
		pDC->MoveTo( xPMin , ypt );	
		pDC->LineTo( xPMin - 5, ypt);

		pDC->SelectObject(&yAxisFont);
		xAxisScale.Format("%.4g",Height);
		xAxisScale.Replace(_T("e+000"), _T("e+"));
		xAxisScale.Replace(_T("e+00"), _T("e+"));
		xAxisScale.Replace(_T("e+0"), _T("e+"));
		scaleSize = pDC->GetTextExtent(xAxisScale);
		
		if( scaleSize.cy%2 == 0 )
		{
			scaleRect.SetRect(xPMin-4-scaleSize.cx, ypt-scaleSize.cy/2, xPMin-4, ypt+scaleSize.cy/2);
		}		
		else
		{
			scaleRect.SetRect(xPMin-4-scaleSize.cx, ypt-int(scaleSize.cy/2)-1, xPMin-4, ypt+scaleSize.cy/2);
		}
			
		pDC->DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);
		
		Height += layHeight[l];
		
		if( l < GraphXY.nLay )
			DrawGraphicsBlocks(pDC, ypt,
				int( ((yPMin-yPMax)/(yMax - yMin))*(yMin-Height) + yPMin ), l);
	}

}

void GraphicsBlock::DrawGraphicsBlocks(CDC *pDC, int yPMaxTemp, int yPMinTemp, int l)
{	
	int i, col;
	
	CRect blockRect;
	CBrush *SolidBlock;


	for ( i = 0; i<Num-1 ; i++)
	{	
		SolidBlock = new CBrush;

		if ( GraphXY.y[i][l] >= 0 && (GraphXY.y[i][l] < 1.0) )
		{	
			col = 7;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 1 && GraphXY.y[i][l] < 10 )
		{	
			col = 6;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 10 && GraphXY.y[i][l] < 1e2)
		{	
			col = 5;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 1e2 && GraphXY.y[i][l] < 1e3)
		{	
			col = 4;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 1e3 && GraphXY.y[i][l] < 1e4)
		{	
			col = 3;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 1e4 && GraphXY.y[i][l] < 1e5) 
		{	
			col = 2;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}

		if ( GraphXY.y[i][l] >= 1e5 && GraphXY.y[i][l] < 1e6) 
		{	
			col = 1;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}
		
		if ( GraphXY.y[i][l] >= 1e6 ) 
		{	
			col = 0;
		
			SolidBlock->CreateSolidBrush(m_crColors[col]);
		}
		
		blockRect.SetRect( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i]-xMin) + xPMin ),
			yPMaxTemp, int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i+1]-xMin) + xPMin ), yPMinTemp);
		
		pDC->FillRect(blockRect,SolidBlock);

		pDC->MoveTo( xPMin, yPMaxTemp );
		pDC->LineTo( xPMax, yPMaxTemp );
		
		delete [] SolidBlock;
	}

}

void GraphicsBlock::SetXYMaxMin(int nB, int nL)
{	
	int i, l;
	
	DeAllocate();
	nBlocks = nB;
	nLay = nL;
	Allocate();

/*	xMax = yMax = -1e20;
	
	xMin = yMin = 1e20;

	for ( i = 0; i<GraphXY.MaxNumTimes ; i++)
	{
		if ( xMax <= GraphXY.x[i] ) xMax = GraphXY.x[i];

		if ( xMin >= GraphXY.x[i] ) xMin = GraphXY.x[i];
		
		for ( l = 0; l<GraphXY.nLay ; l++)
		{
			if ( yMax <= GraphXY.y[i][l] ) yMax = GraphXY.y[i][l];

			if ( yMin >= GraphXY.y[i][l] ) yMin = GraphXY.y[i][l];
		}

	}

	if(yMax <= yMin) yMax = 0.1;
*/	
}

void GraphicsBlock::SetNum(int N)
{
	Num = N;
}
 

void GraphicsBlock::DrawBlocks(CDC *pDC, CRect rect, int xBlock)
{
	int xBMax,
		 xBMin,
		 yBMax,
		 yBMin,
		 stepBlock;
	
	CPoint pAxisTL, 
		    pAxisBR;

	pAxisTL = rect.TopLeft( );
	pAxisBR = rect.BottomRight( );

	xBMax = pAxisBR.x;
	xBMin = pAxisTL.x;
	yBMax = pAxisBR.y;
	yBMin = pAxisTL.y;

	CRect blockRect;

	CPen penRectangleB( PS_SOLID, 1.9 , RGB(0,0,0));
	pDC->SelectObject(&penRectangleB);

	stepBlock = int((yBMax - yBMin)/8.0); 
	
	CBrush *SolidBlock;
	CFont blockFont;
	CString blockScale;
	CRect scaleBlockRect;
	CSize blockSize;

	int bFont,
		 yBlock;

	if((10 + (100*rect.Height())/300) > 130) bFont = 130;

	else bFont = (10 + (100*rect.Height())/300);
	
	blockFont.CreatePointFont( bFont, _T("Arial"));

	for( int i = 0 ; i < 8; i++)
	{	
		SolidBlock = new CBrush;

		blockRect.SetRect(xBMin, yBMin + int(i * stepBlock), xBMax, yBMin + int((i+1) * stepBlock) );

		SolidBlock->CreateSolidBrush(m_crColors[i]);
		
		pDC->FillRect(blockRect,SolidBlock);
		
		pDC->MoveTo( xBMin , yBMin + int(i * stepBlock) );
	
		pDC->LineTo( xBMax , yBMin + int(i * stepBlock) );
		
		yBlock = yBMin + int( ( (2*i+1.0)/2.0 ) * stepBlock);
		
		pDC->SelectObject(&blockFont);
		
		if( i == 7 )
			blockScale.Format("%.2g",0.0);
		else
			blockScale.Format("%.2g",pow(10.0,6-i));
		
		blockScale.Replace(_T("e+000"), _T("e+"));
		blockScale.Replace(_T("e+00"), _T("e+"));
		blockScale.Replace(_T("e+0"), _T("e+"));
		
		blockSize = pDC->GetTextExtent(blockScale);		

		scaleBlockRect.SetRect(xBlock-blockSize.cx-2, yBlock, xBlock+blockSize.cx-2, yBlock+blockSize.cy);	
		
		pAxisBR = blockRect.BottomRight( );

		pDC->DrawText(blockScale, scaleBlockRect, DT_CENTER|DT_VCENTER);

		delete [] SolidBlock;
	}

	pDC->MoveTo( xBMin , yBMax);
	
	pDC->LineTo( xBMax , yBMax);

	pDC->LineTo( xBMax , yBMin-1);

	pDC->MoveTo( xBMin , yBMax);
	
	pDC->LineTo( xBMin ,  yBMin );

	pDC->LineTo( xBMax ,  yBMin );

}

void GraphicsBlock::Allocate()
{
/*	x = new double [nBlocks];

	y = new double * [nBlocks];
	
	for(int i = 0 ; i < nBlocks ; i++ )

		y[i] = new double [nLay];
*/	
	layHeight = new double [nLay];

	for (int l = 0 ; l < nLay; l++ )
		
		layHeight[l] = 1.0;

}

void GraphicsBlock::DeAllocate()
{	
/*	delete [] x;

	for( int i = 0; i<nBlocks-1; i++)
	
		delete [] y[i];

	delete [] y;
*/	
	delete [] layHeight;
}

GraphicsBlock::~GraphicsBlock()
{
	DeAllocate();
}

void GraphicsBlock::OnRButtonDown(UINT nFlags, CPoint point) 
{	
AfxMessageBox("CreateCompatibleBitmap failed in CopyChartToClipboard");
	CopyChartToClipboard();
	CDialog::OnRButtonDown(nFlags, point);
}


void GraphicsBlock::CopyChartToClipboard()
{
	CPaintDC dc(this); // device context for painting
		
	CBrush newBrush(RGB(0, 0, 0));
	CBrush* pOldBrush;
	pOldBrush = dc.SelectObject(&newBrush);

	// create a memory dc, blt it to screen later to avoid flicker
	CDC mdc;
	if (!mdc.CreateCompatibleDC(&dc)) {
		AfxMessageBox("CreateCompatibleDC failed in CopyChartToClipboard");
		return;
	}
	CBitmap mbitmap;
	if (!mbitmap.CreateCompatibleBitmap(&dc, clientRect.Width(), clientRect.Height())) {
		AfxMessageBox("CreateCompatibleBitmap failed in CopyChartToClipboard");
		return;
	}
	
	CBitmap* pbmpOld = mdc.SelectObject(&mbitmap);
	mdc.FillRect(clientRect, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_BTNFACE)));

/*	int cLeft = clientRect.Width()*1/80;
	int cRight = clientRect.Width()*79/80;
	int cTop = clientRect.Height()*1/40;
	int cBottom = clientRect.Height()*39/40;
	chartRect.SetRect(cLeft, cTop, cRight, cBottom);
*///	chart.DrawMultiChartSpectral(&mdc, clientRect, true);

	// here are the actual clipboard functions.
	AfxGetApp()->m_pMainWnd->OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, mbitmap.GetSafeHandle() ) ;
	CloseClipboard () ;
		// next we select the old bitmap back into the memory DC
		// so that our bitmap is not deleted when cMemDC is destroyed.
		// Then we detach the bitmap handle from the cBmp object so that
		// the bitmap is not deleted when cBmp is destroyed.
	// clean up
	if (mdc.SelectObject(pbmpOld) == NULL) {
		AfxMessageBox("SelectObject(pbmpOld) failed");
		return;
	}
	mbitmap.Detach();
	mdc.DeleteDC();
	dc.SelectObject(pOldBrush);
}


void GraphicsBlock::OnOK() 
{
	// Not doing nothing to overrride the dault close when pressinf the key <Enter>
}

void GraphicsBlock::OnCancel() 
{
	// Not doing nothing to overrride the dault close when pressinf the key <Esc>
}


void GraphicsBlock::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
	Invalidate();
	// TODO: Add your message handler code here
}

void GraphicsBlock::OnCopytoclipboard()
{	
	CPaintDC dc(this); // device context for painting
		
	GetClientRect(m_rcClient);
	CBrush newBrush(RGB(0, 0, 0));
	CBrush* pOldBrush;
	pOldBrush = dc.SelectObject(&newBrush);

	// create a memory dc, blt it to screen later to avoid flicker
	
	if (!m_pMemDC.CreateCompatibleDC(&dc)) {
		AfxMessageBox("CreateCompatibleDC failed in CopyChartToClipboard");
		return;
	}
	CBitmap mbitmap;
	if (!mbitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height())) {
		AfxMessageBox("CreateCompatibleBitmap failed in CopyChartToClipboard");
		return;
	}
	
	CBitmap* pbmpOld = m_pMemDC.SelectObject(&mbitmap);
	m_pMemDC.FillRect(m_rcClient, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_BTNFACE)));

/*	int cLeft = clientRect.Width()*1/80;
	int cRight = clientRect.Width()*79/80;
	int cTop = clientRect.Height()*1/40;
	int cBottom = clientRect.Height()*39/40;
	chartRect.SetRect(cLeft, cTop, cRight, cBottom);
*/	DrawAxis(&m_pMemDC, m_rcClient);

	// here are the actual clipboard functions.
	AfxGetApp()->m_pMainWnd->OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, mbitmap.GetSafeHandle() ) ;
	CloseClipboard (); 
	// next we select the old bitmap back into the memory DC
	// so that our bitmap is not deleted when cMemDC is destroyed.
	// Then we detach the bitmap handle from the cBmp object so that
	// the bitmap is not deleted when cBmp is destroyed.
	// clean up
	if (m_pMemDC.SelectObject(pbmpOld) == NULL) {
		AfxMessageBox("SelectObject(pbmpOld) failed");
		return;
	}
	

	mbitmap.Detach();
	m_pMemDC.DeleteDC();
	dc.SelectObject(pOldBrush);
}

void GraphicsBlock::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		CClientDC dc(this);
		
		CString st;
		
		st.Format("X:%d Y:%d", point.x, point.y);

		TextOutA(dc, 10 ,10, st, 10);
	}

	CDialog::OnMouseMove(nFlags, point);
}
