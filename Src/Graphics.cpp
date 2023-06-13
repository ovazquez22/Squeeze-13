// Graphics.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Graphics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Graphics dialog


Graphics::Graphics(CWnd* pParent /*=NULL*/)
	: CDialog(Graphics::IDD, pParent)
{
	//{{AFX_DATA_INIT(Graphics)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	xMax = yMax = 1.0;
	xMin = yMin = 0.0;
	title = "";
	bTitle = bMic = FALSE;

	logScale = 1.0;

	Num = nOutFiles = 0;
	
	MicConc = 1e5;

	kTemp = 0;

	Ptitle = "Project Title1 Max 30 charaters";
}

Graphics::~Graphics()
{

}

void Graphics::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Graphics)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Graphics, CDialog)
	//{{AFX_MSG_MAP(Graphics)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//ON_WM_RBUTTONDOWN()
	//ON_WM_CANCELMODE()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COPYTOCLIPBOARD, &Graphics::OnCopytoclipboard)
	ON_COMMAND(ID_SAVEOUTPUT, &Graphics::OnSaveOutput)
	ON_COMMAND(ID_SAVEIMAGE, &Graphics::OnSaveImage)
//	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Graphics message handlers

BOOL Graphics::OnInitDialog()
{
	CDialog::OnInitDialog();

/*	/////////////////////////////////////////////////////////////////////////////////////
	//Get the drawing surface and create a corresponding bitmap with the same dimensions
	GetWindowRect(&m_rcClient);
	ScreenToClient(&m_rcClient);
	CDC* pDC = GetDC();
	if(pDC != NULL)
	{
		BMIH.biSize = sizeof(BITMAPINFOHEADER);
		BMIH.biBitCount = 24;
		BMIH.biPlanes = 1;
		BMIH.biCompression = BI_RGB;
		BMIH.biWidth = m_rcClient.Width();
		BMIH.biHeight = m_rcClient.Height();
		BMIH.biSizeImage = ((((BMIH.biWidth * BMIH.biBitCount) + 31) & ~31) >> 3) * BMIH.biHeight;
		m_hDrawingSurface = CreateDIBSection(pDC->GetSafeHdc(), (CONST BITMAPINFO*)&BMIH, DIB_RGB_COLORS, (void**)&m_pDrawingSurfaceBits, NULL, 0);
		ReleaseDC(pDC);
	}
	/////////////////////////////////////////////////////////////////////////////////////	
	if((m_hDrawingSurface == NULL) || (m_pDrawingSurfaceBits == NULL))
	{
		//We could not create the bitmap -- quit
		AfxMessageBox(IDS_BITMAP_NOT_CREATED_ERROR_MESSAGE, MB_OK | MB_ICONSTOP);
		PostQuitMessage(0);
	}
	/////////////////////////////////////////////////////////////////////////////////////
	*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Graphics::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Graphics *pWnd = (Graphics*)GetParent();

	GetClientRect(m_rcClient);

	if ( bGraphs )
	{
		// create a memory dc, blt it to screen later to avoid flicker
		if (!m_pMemDC.CreateCompatibleDC(&dc)) {
			AfxMessageBox("CreateCompatibleDC failed in OnPaint");
			return;
		}
		CBitmap mbitmap;
		if (!mbitmap.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height())) {
			AfxMessageBox("CreateCompatibleBitmap failed in OnPaint");
			return;
		}
		
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_pMemDC.GetSafeHdc(), m_hDrawingSurface);	

		CBitmap* pbmpOld = m_pMemDC.SelectObject(&mbitmap);
		m_pMemDC.FillRect(m_rcClient, CBrush::FromHandle((HBRUSH)GetSysColorBrush(COLOR_WINDOW)));

		DrawAxis(&m_pMemDC, m_rcClient);
		
		// copy onto screen
		if (!dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), &m_pMemDC, 0, 0, SRCCOPY)) {
	//		AfxMessageBox("BitBlt failed");
		}

		// clean up
		if (m_pMemDC.SelectObject(pbmpOld) == NULL) {
			AfxMessageBox("SelectObject(pbmpOld) failed");
			return;
		}

		mbitmap.DeleteObject();

		m_pMemDC.DeleteDC();

		// Do not call CDialog::OnPaint() for painting messages
	}

	// Do not call CDialog::OnPaint() for painting messages
	UpdateInterface("");
}

void Graphics::UpdateInterface(CString sEdit)
{	
	CClientDC dc(this);

	GetClientRect(&m_rcClient);

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

void Graphics::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	UpdateInterface(Stage);
}

void Graphics::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
	// TODO: Add your message handler code here
	UpdateInterface(Stage);
	Invalidate();
}

void Graphics::DrawAxis(CDC *pDC, CRect rect)
{	
	int i;
	
	CFont titleFont;
	
	titleFont.CreatePointFont(5*rect.Height()*3/40, _T("Arial"));

	COLORREF color = RGB( 255, 255, 255);     // White
	CBrush lSolidBrush(color);
	m_pMemDC.FillRect(rect,&lSolidBrush);

	CRect axisRect,
		  micRect,
		  wprRect;
	
	int cLeft = rect.Width()*8/80,
		 cRight = rect.Width()*71/80,
		 cTop = rect.Height()*3/40,
		 cBottom = rect.Height()*34/40;
	
	if ( bMic )
	{	
		cLeft = rect.Width()*8/80;

		if ( Mic.size() > 1 ) 
			cRight = rect.Width()*62/80;
		else
			cRight = rect.Width()*71/80;

		cTop = rect.Height()*3/40;
		cBottom = rect.Height()*34/40;
	}
	else
	{
		cLeft = rect.Width()*8/80,
		cRight = rect.Width()*77/80,
		cTop = rect.Height()*3/40,
		cBottom = rect.Height()*34/40;
	}

	axisRect.SetRect(cLeft, cTop, cRight, cBottom);

	CFont* pOldFont = m_pMemDC.SelectObject(&titleFont);

	CRect StageRect,
			titleRect,
		   squareRect,
			fileRect;
	
	CSize StageSize = m_pMemDC.GetTextExtent(Stage);
	titleRect.SetRect( int(rect.right-StageSize.cx - 3), rect.top+1, int(rect.right-3), (rect.top+1+StageSize.cy));
	m_pMemDC.DrawText(Stage, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	
	CSize FileNameSize;
	
	double rat = 0.13;
	int x1 = 0, x2 = 0, y1 = 0, y2 = 0;
	
	if( nOutFiles > 0 )
	{
		for( int i = 0 ; i < nOutFiles; i++ )
		{	
			FileNameSize = m_pMemDC.GetTextExtent(GraphXY.pFile[i].NameFile);

			if ( i == 0 )
			{
				x1 = int( rect.right * rat );
				y1 = int( rect.bottom * rat );
				
				x2 = int( x1 + FileNameSize.cx );
				y2 = int( y1 + FileNameSize.cy );
			
				m_pMemDC.SetTextColor(RGB(0,55,0));
			}
			else
			{
				x2 += int(FileNameSize.cx + 5);

				if ( x2 > int((1-rat/3.0) * rect.right)  )
				{
					x1 = int( rect.right * rat );
					x2 = int( x1 + FileNameSize.cx );
					y1 += int( rect.bottom * rat );
				}
				
				y2 += int( FileNameSize.cy );
			}

			if ( i == 1 ) m_pMemDC.SetTextColor(RGB(0,255,0));
			if ( i == 2 ) m_pMemDC.SetTextColor(RGB(233,23,198));
			if ( i == 3 ) m_pMemDC.SetTextColor(RGB(255,153,0));
			if ( i == 4 ) m_pMemDC.SetTextColor(RGB(102,0,204));
			if ( i == 5 ) m_pMemDC.SetTextColor(RGB(255,255,0));
			if ( i == 6 ) m_pMemDC.SetTextColor(RGB(0,128,128));
			if ( i == 7 ) m_pMemDC.SetTextColor(RGB(121,99,7));
			
			fileRect.SetRect( x1, y1, x2,y1 );
			m_pMemDC.DrawText(GraphXY.pFile[i].NameFile, fileRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);

			x1 = x2;
		}
	}
	
	m_pMemDC.SetTextColor(RGB(0,0,0));

	CSize titleSize;

	// First Line of Project title
	titleSize = pDC->GetTextExtent(Ptitle);
	titleRect.SetRect( int((rect.left+rect.right-titleSize.cx)/2), rect.top+1, int((rect.left+rect.right+titleSize.cx)/2), (rect.top+1+titleSize.cy));
	pDC->DrawText(Ptitle, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	
	if (bTitle)
	{

		// Legend 
		titleSize = m_pMemDC.GetTextExtent(title);
		titleRect.SetRect( int((rect.left+rect.right-titleSize.cx)/2), rect.top+5+titleSize.cy, int((rect.left+rect.right+titleSize.cx)/2), (rect.top+5+2*titleSize.cy));
		m_pMemDC.DrawText(title, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);

		if( GraphXY.nPts > 0 )
		{	
			// Blue pen
			CPen pen0( PS_SOLID, 2.5 , RGB(15,117,209));
		
			m_pMemDC.SelectObject(&pen0);
		
			m_pMemDC.MoveTo( int( titleRect.left + titleRect.Width() / 2 ) - titleSize.cx / 10  - 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
		
			m_pMemDC.LineTo( int( titleRect.left + titleRect.Width() / 2 ) + titleSize.cx / 10  + 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );

			CBrush SolidBlock(RGB(150,27,29));
			
			int Sq = 2;
		/*	squareRect.SetRect(int(titleRect.left - titleSize.cx / 5 ) - Sq, int(rect.top + 1 + titleSize.cy / 2) - Sq,
				int( titleRect.left ) - 4 + Sq , int(rect.top + 1 + titleSize.cy / 2) + Sq );
		*/	
			squareRect.SetRect(int(titleRect.left - titleSize.cx / 6 ) - Sq, int(rect.top + 4 + (3/2.0)*titleSize.cy ) - Sq,
				int( titleRect.left - titleSize.cx / 6 ) + Sq , int(rect.top + 4 + (3/2.0)*titleSize.cy ) + Sq );

			m_pMemDC.FillRect(squareRect,&SolidBlock);
		}
		else
		{	
			// Blue pen
			CPen pen0( PS_SOLID, 2.5 , RGB(15,117,209));
		
			m_pMemDC.SelectObject(&pen0);
		
			m_pMemDC.MoveTo( int(titleSize.cx*(1.0/5.0)+titleRect.left) + 2 , int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
		
			m_pMemDC.LineTo( int(titleSize.cx*(2.0/5.0)+titleRect.left) - 2 , int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
			
			// Green pen
			CPen pen1( PS_DASH, 2.5 , RGB(15,217,29));
		
			m_pMemDC.SelectObject(&pen1);
		
			m_pMemDC.MoveTo( int(titleSize.cx*(3.0/5.0)+titleRect.left) + 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
		
			m_pMemDC.LineTo( int(titleSize.cx*(4.0/5.0)+titleRect.left) - 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );

			// Red pen
			CPen pen2( PS_DASH, 2.5 , RGB(255,10,10)); // Red
		
			m_pMemDC.SelectObject(&pen2);
		
			m_pMemDC.MoveTo( int(titleSize.cx*(5.0/5.0)+titleRect.left) + 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
		
			m_pMemDC.LineTo( int(titleSize.cx*(6.0/5.0)+ titleRect.left) - 2, int(rect.top + 4 + (3/2.0)*titleSize.cy ) );
		}
	}

	CFont yLabelFont;

	yLabelFont.CreateFont( rect.Width()*3/80, 0, 900, 0, 0, 0, 0, 0, 0, OUT_DEFAULT_PRECIS,
				CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, _T("Arial"));
	
	
	m_pMemDC.SelectObject(&yLabelFont);
	CRect yLabelRect;
	if (yLabel != "")
	{
		CSize yLabelSize = m_pMemDC.GetTextExtent(yLabel);
		yLabelRect.SetRect(rect.left+2, (axisRect.bottom+axisRect.top+yLabelSize.cx)/2, rect.left+yLabelSize.cy, (axisRect.bottom+axisRect.top-yLabelSize.cx)/2);
		m_pMemDC.DrawText(yLabel, yLabelRect, DT_NOCLIP);
	}
	
	CFont xLabelFont;

	xLabelFont.CreatePointFont(5*rect.Height()*3/40, _T("Arial"));

	CRect xLabelRect;
	m_pMemDC.SelectObject(&xLabelFont);
	if (xLabel != "")
	{
		CSize xLabelSize = m_pMemDC.GetTextExtent(xLabel);
		xLabelRect.SetRect((axisRect.left+axisRect.right-xLabelSize.cx)/2, rect.bottom-xLabelSize.cy-4, (axisRect.left+axisRect.right+xLabelSize.cx)/2, (rect.bottom-xLabelSize.cy-2));
		m_pMemDC.DrawText(xLabel, xLabelRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	}

	CPoint pAxisTL, 
		    pAxisBR;

	pAxisTL = axisRect.TopLeft( );
	pAxisBR = axisRect.BottomRight( );

	xPMax = pAxisBR.x;
	xPMin = pAxisTL.x;
	yPMax = pAxisBR.y;
	yPMin = pAxisTL.y;

	CBrush lNullBrush(RGB(200, 200, 200));
	
	CPen penRectangleB( PS_SOLID, 1.9 , RGB(255,0,0));
	m_pMemDC.SelectObject(&penRectangleB);
	
	m_pMemDC.MoveTo( xPMin , yPMax);
	
	m_pMemDC.LineTo( xPMax , yPMax);

	m_pMemDC.LineTo( xPMax , yPMin);

	m_pMemDC.MoveTo( xPMin , yPMax);
	
	m_pMemDC.LineTo( xPMin ,  yPMin );

	m_pMemDC.LineTo( xPMax ,  yPMin );

	if( logScale == 0 )
	{	
		m_pMemDC.MoveTo( xPMin - 5 , yPMin);
	
		m_pMemDC.LineTo( xPMin ,  yPMin );
	}

	if ( bMic )
	{	
		int yTop, 
	        yBottom;

		double Height = 0,
			   Sum,
			   tempH;

		cLeft = rect.Width()*8/80,
		cRight = rect.Width()*71/80,
		cTop = rect.Height()*3/40,
		cBottom = rect.Height()*34/40;

		axisRect.SetRect(cLeft, cTop, cRight, cBottom);
		
		cLeft = rect.Width()*72/80,
		cRight = rect.Width()*79/80,
		cTop = rect.Height()*3/40,
		cBottom = rect.Height()*34/40;
				
		yTop = cTop;
				
		CBrush micBrushG(RGB(0,255,0)),
			   micBrushR(RGB(255,0,0)),
			   micBrushY(RGB(255,255,0)),
			   wprBrush(RGB(0,0,255));
		
		CPen penMIC( PS_SOLID, 1.0 , RGB(0,0,0));
		
		Sum = 0;

		for( i = 0 ; i < Mic.size() ; i++ )
			Sum += Mic[i].Height;

		Height = Mic[0].Height;

		tempH = (yMax - yMin)*Height/Sum - 1;

		for( i = 1 ; i <= Mic.size() ; i++ )
		{	
			yBottom = int( ((yPMin-yPMax)/(yMax - yMin))*(yMin-tempH) + yPMin );
			
			// MIC Grpah Block
			micRect.SetRect(cLeft, yTop, cRight, yBottom);

			if ( MicConc < Mic[i-1].wConc ) m_pMemDC.FillRect(micRect,&micBrushG);
			
			if ( fabs(MicConc - Mic[i-1].wConc) < 1 ) m_pMemDC.FillRect(micRect,&micBrushY);

			if ( MicConc > Mic[i-1].wConc ) m_pMemDC.FillRect(micRect,&micBrushR);

			m_pMemDC.SelectObject(&penMIC);

			m_pMemDC.MoveTo(cLeft, yTop);
		
			m_pMemDC.LineTo(cRight, yTop);

			m_pMemDC.LineTo(cRight, yBottom);
			
			m_pMemDC.LineTo(cLeft, yBottom);

			m_pMemDC.LineTo(cLeft, yTop);

			// WPRD, flow rates 
			double diff = rect.Width()*71/80 - rect.Width()*63/80;
			
			if ( Mic.size() > 1 )
			{	
				wprRect.SetRect(rect.Width()*63/80, yTop, Mic[i-1].wprd*diff + rect.Width()*63/80, yBottom);

				m_pMemDC.FillRect(wprRect,&wprBrush);

				m_pMemDC.SelectObject(&penMIC);

				m_pMemDC.MoveTo(rect.Width()*63/80, yTop);
		
				m_pMemDC.LineTo(rect.Width()*71/80, yTop);

				m_pMemDC.LineTo(rect.Width()*71/80, yBottom);
			
				m_pMemDC.LineTo(rect.Width()*63/80, yBottom);

				m_pMemDC.LineTo(rect.Width()*63/80, yTop);
			}

			yTop = yBottom;

			if ( i < Mic.size() )
				
				Height += (Mic[i].Height);

			tempH = (yMax - yMin)*Height/Sum - 1;
		}

		CFont micFont;
	
		micFont.CreatePointFont(rect.Height()*14/50, _T("Arial"));
		
		CString sMic;		

		// MIC text 
		sMic.Format("R<MIC\nG>>MIC\nY~MIC");
		CSize micSize = m_pMemDC.GetTextExtent("G>>MIC");
		
		int mean = 2 + cLeft + (cRight-cLeft)/2.0; 
		micRect.SetRect( mean - int(micSize.cx/2.0), 4+cBottom,mean + int(micSize.cx/2.0), cBottom+3*micSize.cy);
		m_pMemDC.SelectObject(&micFont);
		m_pMemDC.DrawText(sMic, micRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);

		// WPRD text 
		if ( Mic.size() > 1 )
		{
			sMic.Format(_T("WPRD\n0      1"));
			micSize = m_pMemDC.GetTextExtent("0        1");
		
			mean = 2 + rect.Width()*63/80 + (rect.Width()*71/80-rect.Width()*63/80)/2.0; 
			micRect.SetRect( mean - int(micSize.cx/2.0), 4+cBottom,mean + int(micSize.cx/2.0), cBottom+3*micSize.cy);
			m_pMemDC.SelectObject(&micFont);
			m_pMemDC.DrawText(sMic, micRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
		}
	}
	else
	{
		cLeft = rect.Width()*8/80,
		cRight = rect.Width()*77/80,
		cTop = rect.Height()*3/40,
		cBottom = rect.Height()*34/40;

		axisRect.SetRect(cLeft, cTop, cRight, cBottom);
	}

	DrawTics(pDC,axisRect);

}

void Graphics::DrawTics(CDC *pDC, CRect rect)
{
	int i,
		 l;

	CPoint pAxisBR, 
		    pxMaxTL;

	CSize scaleSize;
	CRect scaleRect;
	CString xAxisScale;

	CPen penGrid( PS_DOT, 0.3 , RGB(200,200,200));
	CPen penAxis( PS_SOLID, 1.5 , RGB(230,0,75));

	int nXTics,
		 nYTics,
		 xpt,
		 ypt,
		 n1,
		 n2;

	double xTic,
			DxTic,
			yTic,
			DyTic,
			TempDxTic,
			TempDyTic,
			xMaxTic,
			yMaxTic;

	
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

	// Y axis

	DyTic = log10(yMax - yMin);

	DyTic = pow(10.0,int(DyTic));

	nYTics = int((yMax - yMin)/DyTic);
	
	yMaxTic = DyTic * nYTics;
	
	if( nYTics <= 1 )
	{ 
		nYTics = int(yMax - yMin);
		
		TempDyTic = 1;

		yMaxTic = TempDyTic * nYTics;
	}

	n1 = int(2 * (yMax - yMin)/DyTic);

	n2 = int(5 * (yMax - yMin)/DyTic);

	TempDyTic = DyTic;
	
	if ( abs(5-nYTics) > abs(5-n1) && logScale == 1 )
	{
		nYTics = n1; 

		TempDyTic = DyTic / 2;
	
		yMaxTic = DyTic * nYTics;;
	}

	if( abs(5-nYTics) > abs(5-n2) && logScale == 1 )
	{
		nYTics = n2;
		
		TempDyTic = DyTic / 5;
	
		yMaxTic = DyTic * nYTics;;
	}
	
	DyTic = TempDyTic;
	
	if (logScale == 0 ) DyTic = 1;

	xTic = xMin;

	yTic = yMin;
	
	CFont xAxisFont, yAxisFont;
	
	int xFont,
		 yFont;

	if((6 + (70*rect.Width())/300) > 140) xFont = 140;

	else xFont = (6 + (70*rect.Width())/300);

	if((10 + (100*rect.Height())/300) > 130) yFont = 130;

	else yFont = (10 + (100*rect.Height())/300);
	
	xAxisFont.CreatePointFont( xFont, _T("Arial"));

	ypt = yPMax;

	for ( i = 0; i<=nXTics ; i++)
	{	
		xpt = int( ((xPMax-xPMin)/(xMax - xMin)) * (xTic-xMin) + xPMin );
		
		ypt = yPMax;	
		
		m_pMemDC.SelectObject(&penGrid);
		
		if( i > 0 && i <= nXTics )
		{
			m_pMemDC.MoveTo( xpt , yPMin );	
			m_pMemDC.LineTo( xpt, yPMax);
		}

		m_pMemDC.SelectObject(&penAxis);
		
		m_pMemDC.MoveTo( xpt, yPMax );	
		m_pMemDC.LineTo( xpt, yPMax + 3);
		
		m_pMemDC.SelectObject(&xAxisFont);
		xAxisScale.Format("%.4g",xTic);
		xAxisScale.Replace(_T("e+000"), _T("e+"));
		xAxisScale.Replace(_T("e+00"), _T("e+"));
		xAxisScale.Replace(_T("e+0"), _T("e+"));
		scaleSize = m_pMemDC.GetTextExtent(xAxisScale);
		
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
		m_pMemDC.FillRect(scaleRect,&WhiteBrush);
		m_pMemDC.DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);

		xTic += DxTic; 
	
	}
	
	// Print the xMax 
	CRect scaleRectxMax;
	xAxisScale.Format("%.4g",xMax<1 ? xMax : int(xMax));
//	xAxisScale.Format("%.4g",xMax);
	xAxisScale.Replace(_T("e+000"), _T("e"));
	xAxisScale.Replace(_T("e+00"), _T("e"));
	xAxisScale.Replace(_T("e+0"), _T("e"));
	scaleSize = m_pMemDC.GetTextExtent(xAxisScale);

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
		m_pMemDC.FillRect(scaleRectxMax,&WhiteBrush);
		m_pMemDC.SelectObject(&xAxisFont);
		m_pMemDC.DrawText(xAxisScale, scaleRectxMax, DT_CENTER|DT_VCENTER);
		
		m_pMemDC.MoveTo( xPMax, yPMax );	
		m_pMemDC.LineTo( xPMax, ypt + 3);
	}

	int yPMaxTemp,
		 yPMinTemp,
		 yPDelta;

	yPDelta = int((yPMax-yPMin)/GraphXY.nLay);
	
	yAxisFont.CreatePointFont( int( yFont * (1 - 0.05 * GraphXY.nLay) ) , _T("Arial"));

	for ( l = 0; l<GraphXY.nLay ; l++)
	{	
		// Y Axis
		yTic = yMin;
		
		yPMaxTemp = yPMin + (l + 1) * yPDelta;
			
		yPMinTemp = yPMin + l * yPDelta;
	
		if ( l > 0 && l < GraphXY.nLay )
		{	
			CPen pen( PS_SOLID, 1.2 , RGB(230,0,75));
			m_pMemDC.SelectObject(&pen);

			m_pMemDC.MoveTo( xPMin , yPMinTemp );	
		
			m_pMemDC.LineTo( xPMax , yPMinTemp);
		}
	
		for ( i = 0; i<=nYTics ; i++)
		{ 
			ypt = int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-yTic) + yPMaxTemp );

			m_pMemDC.SelectObject(&penGrid);
		
			if( i > 0 && i <= nYTics )
			{	
				if( fabs(yMax - yTic) > yMax * 0.1 )
				{
					m_pMemDC.MoveTo( xPMax ,ypt );	
					m_pMemDC.LineTo( xPMin, ypt);
				}
			}
			
			m_pMemDC.SelectObject(&penAxis);
			
			
			if(logScale == 0 && i == nYTics )
			{
				m_pMemDC.MoveTo( xPMin , yPMax );	
				m_pMemDC.LineTo( xPMin - 5, yPMax);
			}
			else
			{
				m_pMemDC.MoveTo( xPMin , ypt );	
				m_pMemDC.LineTo( xPMin - 5, ypt);
				
				if( i>0 && i < nYTics)
				{
					m_pMemDC.SelectObject(&penGrid);
					m_pMemDC.MoveTo( xPMax ,ypt );	
					m_pMemDC.LineTo( xPMin, ypt);
				}
			}

			m_pMemDC.SelectObject(&yAxisFont);
			
			if( logScale == 0 )
				xAxisScale.Format("%.4g",pow(10, i + yMin) );
			else
				xAxisScale.Format("%.4g",yTic);

			xAxisScale.Replace(_T("e+000"), _T("e+"));
			xAxisScale.Replace(_T("e+00"), _T("e+"));
			xAxisScale.Replace(_T("e+0"), _T("e+"));
			scaleSize = m_pMemDC.GetTextExtent(xAxisScale);
			
			if( scaleSize.cy%2 == 0 )
			{
				scaleRect.SetRect(xPMin-2-scaleSize.cx, ypt-scaleSize.cy/2, xPMin-2, ypt+scaleSize.cy/2);
			}		
			else
			{
				scaleRect.SetRect(xPMin-2-scaleSize.cx, ypt-int(scaleSize.cy/2)-1, xPMin-2, ypt+scaleSize.cy/2);
			}
				
			if ( !(l != 0 && i == nYTics) )
			
				m_pMemDC.DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);

			yTic += DyTic;

		}

	}

	for ( l = 0; l<GraphXY.nLay ; l++)
	{	
	
		yPMaxTemp = yPMin + (l + 1) * yPDelta;
			
		yPMinTemp = yPMin + l * yPDelta;
		
		for( int n = 0 ; n<GraphXY.nDat ; n++ )
			
			DrawGraph(pDC, yPMaxTemp, yPMinTemp, l, n);
	}

}


void Graphics::DrawGraph(CDC *pDC, int yPMaxTemp, int yPMinTemp, int l, int n)
{	
	int i, j;

	CPen pen0( PS_SOLID, 2.5, RGB(15,117,209)); // Blue

	CPen pen1( PS_DASH, 2.5, RGB(15,217,29)); // Green
	
	CPen pen2( PS_DASH, 2.5, RGB(255,10,10)); // Red

	
	CRect squareRect;
	CBrush SolidBlock(RGB(150,27,29)); 
	
	CPen Pen1(PS_SOLID, 1.75, RGB(0,55,0));
	CPen Pen2(PS_SOLID, 1.75, RGB(0,255,0));
	CPen Pen3(PS_SOLID, 1.75, RGB(233,23,198));
	CPen Pen4(PS_SOLID, 1.75, RGB(255,153,0));
	CPen Pen5(PS_SOLID, 1.75, RGB(102,0,204));
	CPen Pen6(PS_SOLID, 1.75, RGB(255,255,0));
	CPen Pen7(PS_SOLID, 1.75, RGB(0,128,128));
	CPen Pen8(PS_SOLID, 1.75, RGB(121,99,7));
	
	CBrush OutSolidBlock1(RGB(0,55,0));
	CBrush OutSolidBlock2(RGB(0,255,0));
	CBrush OutSolidBlock3(RGB(233,23,198));
	CBrush OutSolidBlock4(RGB(255,153,0));
	CBrush OutSolidBlock5(RGB(102,0,204));
	CBrush OutSolidBlock6(RGB(255,255,0));
	CBrush OutSolidBlock7(RGB(0,128,128));
	CBrush OutSolidBlock8(RGB(121,99,7));

	if( n == 0)
		
		m_pMemDC.SelectObject(&pen0);
	
	if ( n == 1)
		
		m_pMemDC.SelectObject(&pen1);

	if ( n == 2)
		
		m_pMemDC.SelectObject(&pen2);

	if( logScale == 0 )
	{	

		for ( i = 0; i<(Num-1) ; i++)
		{
			if( //pow(10,GraphXY.y1[i][l][n]) >= pow(10,yMin) && 
				GraphXY.y1[i][l][n] != 0 )
			{	
			
				if( i <= 0 )
				{
					m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i]-xMin) + xPMin ), 
						int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-yMin) + yPMaxTemp ) );
				
					m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i]-xMin) + xPMin ), 
						int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-GraphXY.y1[i][l][n]) + yPMaxTemp ) );
				}

				m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i]-xMin) + xPMin ), 
					int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-GraphXY.y1[i][l][n]) + yPMaxTemp ) );
														
				m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i+1]-xMin) + xPMin ), 
					int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-GraphXY.y1[i+1][l][n]) + yPMaxTemp ) );

			}
		
		}
		
		m_pMemDC.SelectObject(&pen1);
		
		int Sq = 2;

		for ( i = 0; i < GraphXY.nPts ; i++ )
		{
			if( GraphXY.yP[i] != 0 && GraphXY.yP[i] >= pow(10,yMin) && GraphXY.xP[i] <= xMax )
			{			
				squareRect.SetRect(int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.xP[i]-xMin) + xPMin ) - Sq,
					int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.yP[i])) + yPMaxTemp ) - Sq,
						int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.xP[i]-xMin) + xPMin ) + Sq,
							int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.yP[i])) + yPMaxTemp ) + Sq );
				
				m_pMemDC.FillRect(squareRect,&SolidBlock);

/*				m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.xP[i]-xMin) + xPMin ), 
					int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.yP[i])) + yPMaxTemp ) );
														
				m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.xP[i+1]-xMin) + xPMin ), 
					int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.yP[i+1])) + yPMaxTemp ) );
*/
			}
		
		}

		// Draw output file
		if( GraphXY.nLay == 1 )
		{	
			for( j = 0 ; j < nOutFiles; j++ )
			{
				for ( i = 0; i < GraphXY.pFile[j].RT.size() ; i++ )
				{
					if( GraphXY.pFile[j].RC[i] != 0 && GraphXY.pFile[j].RC[i] >= pow(10,yMin) && GraphXY.pFile[j].RT[i] <= xMax )
					{	
						Sq = 1;

						squareRect.SetRect(int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i]-xMin) + xPMin ) - Sq,
							int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.pFile[j].RC[i])) + yPMaxTemp ) - Sq,
								int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i]-xMin) + xPMin ) + Sq,
									int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.pFile[j].RC[i])) + yPMaxTemp ) + Sq );

						if (j==0) m_pMemDC.FillRect(squareRect,&OutSolidBlock1);

						if (j==1) m_pMemDC.FillRect(squareRect,&OutSolidBlock2);

						if (j==2) m_pMemDC.FillRect(squareRect,&OutSolidBlock3);

						if (j==3) m_pMemDC.FillRect(squareRect,&OutSolidBlock4);

						if (j==4) m_pMemDC.FillRect(squareRect,&OutSolidBlock5);

						if (j==5) m_pMemDC.FillRect(squareRect,&OutSolidBlock6);

						if (j==6) m_pMemDC.FillRect(squareRect,&OutSolidBlock7);

						if (j==7) m_pMemDC.FillRect(squareRect,&OutSolidBlock8);
						
					}
				}
							
				if (j==0) m_pMemDC.SelectObject(&Pen1);

				if (j==1) m_pMemDC.SelectObject(&Pen2);

				if (j==2) m_pMemDC.SelectObject(&Pen3);

				if (j==3) m_pMemDC.SelectObject(&Pen4);

				if (j==4) m_pMemDC.SelectObject(&Pen5);

				if (j==5) m_pMemDC.SelectObject(&Pen6);

				if (j==6) m_pMemDC.SelectObject(&Pen7);

				if (j==7) m_pMemDC.SelectObject(&Pen8);

				for ( i = 0; (i < GraphXY.pFile[j].RT.size()-1 && GraphXY.pFile[j].RT[i+1] <= xMax) ; i++ )
				{
					if( GraphXY.pFile[j].RT[i+1] <= xMax )
					{	  
						if( GraphXY.pFile[j].RC[i] <= pow(10,yMin) )
						
							m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i]-xMin) + xPMin ), 
								int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-yMin) + yPMaxTemp ) );
						
						else
												
							m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i]-xMin) + xPMin ), 
								int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.pFile[j].RC[i])) + yPMaxTemp ) );
						
						if (GraphXY.pFile[j].RC[i+1] >= pow(10,yMin))
							
							m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i+1]-xMin) + xPMin ), 
								int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-log10(GraphXY.pFile[j].RC[i+1])) + yPMaxTemp ) );
						else
							
							m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.pFile[j].RT[i+1]-xMin) + xPMin ), 
								int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-yMin) + yPMaxTemp ) );
					}	
				}
			}
		}
	}	
	else
	{

		m_pMemDC.MoveTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[0]-xMin) + xPMin ), 
			int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-GraphXY.y1[0][l][n]) + yPMaxTemp ) );
		
		for ( i = 1; i<Num ; i++)
		{			
			m_pMemDC.LineTo( int( ((xPMax-xPMin)/(xMax - xMin)) * (GraphXY.x[i]-xMin) + xPMin ), 
				int( ((yPMaxTemp-yPMinTemp)/(yMax - yMin))*(yMin-GraphXY.y1[i][l][n]) + yPMaxTemp ) );
		}
	
	}

}

void Graphics::SetXYMaxMin()
{	
	int i, l;

	xMax = yMax = -1e20;
	
	xMin = yMin = 1e20;


	for ( i = 0; i<GraphXY.MaxNumTimes ; i++)
	{
		if ( xMax <= GraphXY.x[i] ) xMax = GraphXY.x[i];

		if ( xMin >= GraphXY.x[i] ) xMin = GraphXY.x[i];
		
	/*	for ( l = 0; l<GraphXY.nLay ; l++)
		{
			if ( yMax <= GraphXY.y[i][l] ) yMax = GraphXY.y[i][l];

			if ( yMin >= GraphXY.y[i][l] ) yMin = GraphXY.y[i][l];
		}*/

	}

	if(yMax <= yMin) yMax = 10;
	
}

void Graphics::SetNum(int N)
{
	Num = N;
}
 

void Graphics::OnRButtonDown(UINT nFlags, CPoint point) 
{
/*	// TODO: Add your message handler code here and/or call default
	
	CPaintDC dc(this); // device context for painting
		
	CRect clientRect;
	GetClientRect(clientRect);
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

	CDialog::OnRButtonDown(nFlags, point);*/
}

void Graphics::OnCancelMode() 
{
	CDialog::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void Graphics::OnOK() 
{
	// Not doing nothing to overrride the dault close when pressinf the key <Enter>
}

void Graphics::OnCancel() 
{
	// Not doing nothing to overrride the dault close when pressinf the key <Esc>
}

void Graphics::OnContextMenu(CWnd* pWnd, CPoint point)
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

void Graphics::OnCopytoclipboard()
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

void Graphics::OnSaveImage()
{	
	CDC* pDC = GetDC();
	
	if(pDC != NULL)
	{
		//Create a memory dc and select the drawing surface into it
		if(m_pMemDC.CreateCompatibleDC(pDC) == TRUE)
		{
			HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_pMemDC.GetSafeHdc(), m_hDrawingSurface);	
			/////////////////////////////////////////////////////////////////////////////////////
			//Once out drawing surface has been selected into the memory dc, we can draw anything 
			//and have it all nicely collected in our bitmap for future use
			//Fill up the background
		
			DrawAxis(&m_pMemDC, m_rcClient);

			int cLeft = m_rcClient.Width()*8/80,
				cRight = m_rcClient.Width()*71/80,
				cTop = m_rcClient.Height()*3/40,
				cBottom = m_rcClient.Height()*34/40;

			//Now blit our memory drawing to the static control's rectangle
			BOOL bResult = BitBlt(m_pMemDC.GetSafeHdc(), 
								  cLeft,
								  cTop,
								  cRight,
								  cBottom,
								  m_pMemDC.GetSafeHdc(),
								  0,
								  0,
								  SRCCOPY);

			//And deselect the drawing surface
			SelectObject(m_pMemDC.GetSafeHdc(), hOldBitmap);
			/////////////////////////////////////////////////////////////////////////////////////
		}
		ReleaseDC(pDC);
	}
	
	CString szFilter;
	szFilter.LoadString(IDS_WINDOWS_BITMAP_FILES);
	//Display the "Save As" dialog for the user to specify a path name
	CFileDialog dlg(FALSE, DEFAULT_BITMAP_FILE_EXTENSION, DEFAULT_BITMAP_FILE_NAME, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, NULL);
	if(dlg.DoModal() == IDOK)
	{
		CString szPathName = dlg.GetPathName();
		//Create a new file for writing
		FILE *pFile = fopen(szPathName, "wb");
		if(pFile == NULL)
		{
			AfxMessageBox(IDS_FILE_CREATE_ERROR_MESSAGE);
			return;
		}
		BITMAPFILEHEADER bmfh;	
		int nBitsOffset = sizeof(BITMAPFILEHEADER) + BMIH.biSize; 
		LONG lImageSize = BMIH.biSizeImage;
		LONG lFileSize = nBitsOffset + lImageSize;
		bmfh.bfType = 'B'+('M'<<8);			
		bmfh.bfOffBits = nBitsOffset;		
		bmfh.bfSize = lFileSize;				
		bmfh.bfReserved1 = bmfh.bfReserved2 = 0;
		//Write the bitmap file header
		UINT nWrittenFileHeaderSize = fwrite(&bmfh, 1, sizeof(BITMAPFILEHEADER), pFile);
		//And then the bitmap info header
		UINT nWrittenInfoHeaderSize = fwrite(&BMIH, 1, sizeof(BITMAPINFOHEADER), pFile);
		//Finally, write the image data itself -- the data represents our drawing
		UINT nWrittenDIBDataSize = fwrite(m_pDrawingSurfaceBits, 1, lImageSize, pFile);
		
		fclose(pFile);		
	}
}


void Graphics::OnKillFocus(CWnd* pNewWnd)
{
	CDialog::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here
//	UpdateInterface(Stage);
}

void Graphics::OnSaveOutput()
{	
	CString sItem = PathNameReadFile;

	this->GetWindowText(sItem);
	int nGraph = 0;

	if (sItem == "SQZ Total Return Concentration")
	{
		nGraph = 0;
		sItem = PathNameReadFile;
		sItem.Replace(".dat", "_RC_WC_WP.txt");
		sItem.Replace(".DAT", "_RC_WC_WP.txt");
	}

	if (sItem == "SQZ Layer Return Concentration")
	{
		nGraph = 1;
		sItem = PathNameReadFile;
		sItem.Replace(".dat", "_RC_L.txt");
		sItem.Replace(".DAT", "_RC_L.txt");
	}

	if (sItem == "SQZ Layer Concentration Profile")
	{
		nGraph = 2;
		sItem = PathNameReadFile;
		sItem.Replace(".dat", "_C_A_T_L.txt");
		sItem.Replace(".DAT", "_C_A_T_L.txt");
	}

	size_t err;
	FILE *fP;

	// This sItem will store all the Pareto Front 
	if ((err = fopen_s(&fP, sItem, "w")) != 0)
		printf("Error opening sItem %s for writing\r\n", "conc.txt");
	else
	{	
		if (nGraph == 0) // Total return concentration 
		{
			fprintf_s(fP, "%11s %11s ", "Time\Vol", "RetConc");

			for (int i = 0; i < Mic.size(); i++)
			{
				CString st;
				st.Format("WC_L%d", i);
				fprintf_s(fP, "%11s ", st);
			}

			for (int i = 0; i < Mic.size(); i++)
			{
				CString st;
				st.Format("WPRD_L%d", i);
				fprintf_s(fP, "%11s ", st);
			}

			fprintf_s(fP, "\n");

			for (int i = 0; i < GraphXY.MaxNumTimes; i++)
			{
				fprintf_s(fP, "%11.4f %11.4f ", GraphXY.x[i], pow(10, GraphXY.y1[i][0][0]));

				for (int l = 0; l < Mic.size(); l++)

					fprintf_s(fP, "%11.4f ", WC_L_Out[i][l]);

				for (int l = 0; l < Mic.size(); l++)

					fprintf_s(fP, "%11.4f ", WPRD[i][l]);

				fprintf_s(fP, "\n");
			}
		}

		if (nGraph == 1) // Layer return concentration 
		{
			fprintf_s(fP, "%11s %11s ", "Time\Vol", "RetConc");

			for (int i = 0; i < Mic.size(); i++)
			{
				CString st;
				st.Format("RC_L%d", i);
				fprintf_s(fP, "%11s ", st);
			}

			fprintf_s(fP, "\n");

			for (int i = 0; i < GraphXY.MaxNumTimes; i++)
			{
				fprintf_s(fP, "%11.4f %11.4f ", GraphXY.x[i], pow(10, GraphXY.y1[i][0][0]));

				for (int l = 0; l < Mic.size(); l++)

					fprintf_s(fP, "%11.4f ", RC_L_Out[i][l]);

				fprintf_s(fP, "\n");
			}
		}

		if (nGraph == 2) // Layer Concentration Profile
		{	
			for (int i = 0; i < C_L_Out.size(); i++)
			{
				if (i == 0)
				{
					fprintf_s(fP, "%11s \n", "Radius");

					for (int j = 0; j < Num; j++)

						fprintf_s(fP, "%11.4f ", GraphXY.x[j]);

					fprintf_s(fP, "\n");
				}

				fprintf_s(fP, "Time/Vol %11.4f Conc Adso/ppt Temp\n", RT[i]);

				for (int l = 0; l < GraphXY.nLay; l++)
				{
					fprintf_s(fP, "Layer %d\n", l + 1);

					for (int j = 0; j < Num; j++)

						fprintf_s(fP, "%11.4f ", C_L_Out[i][j][l]);

					fprintf_s(fP, "\n");

					for (int j = 0; j < Num; j++)

						fprintf_s(fP, "%11.4f ", AD_L_Out[i][j][l]);
					
					fprintf_s(fP, "\n");

					if (kTemp)
					{
						for (int j = 0; j < Num; j++)

							fprintf_s(fP, "%11.4f ", FTemp_L_Out[i][j][l]);
					}
				}

			}
				
		}
	}

	fclose(fP);
}

