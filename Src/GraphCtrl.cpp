// GraphCtrl.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "GraphCtrl.h"
/* find maximum of a and b */
#define MAX(a,b)	(((a)>(b))?(a):(b))
#define expt(a, n) pow(a, (double)(n))

/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl

CGraphCtrl::CGraphCtrl()
{
	m_pMemDC = NULL;	
	m_crBackGround = RGB(255,255, 255);
	m_crGrid = RGB(230,0,75);		

	xMax = 1;
	xMin = 0;
	yMax = 1000;
	yMin = 0.1;

	cL = 8.0/80;
	cR = 78.0/80;
	cT = 3.0/40;
	cB = 34.0/40;

	xFont = 120;
	yFont = 120;

	xLabFont = 130; // x Label Font
	yLabFont = 20; // y Label Font

	xLabel = "Time (days)";
	yLabel = "Concentration (ppm)";

	logScale = true;
	bTitle = false;

	nLines = 1;
	m_Lines = new CGraphLine[nLines];
}

CGraphCtrl::~CGraphCtrl()
{
	if (m_pMemDC)
		delete m_pMemDC;

	delete[] m_Lines;
}

BOOL CGraphCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT uID)
{
	BOOL bRet = CWnd::CreateEx(
		WS_EX_STATICEDGE, 
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW),
		NULL,
		dwStyle,
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(),
		(HMENU)uID);

	if(!bRet)
	{
		return FALSE;
	}		

	m_pMemDC = new CDC;
	if(!m_pMemDC)
	{
		return FALSE;
	}		
		
	GetClientRect(m_rcClient);
	 
	// Set the graphing rectangle
	CRect axisRect;
	
	int cLeft = m_rcClient.Width()*cL,
		cRight = m_rcClient.Width()*cR,
		cTop = m_rcClient.Height()*cT,
		cBottom = m_rcClient.Height()*cB;
	
	CPoint pAxisTL, 
		   pAxisBR;

	axisRect.SetRect(cLeft, cTop, cRight, cBottom);
	
	pAxisTL = axisRect.TopLeft( );
	pAxisBR = axisRect.BottomRight( );

	xPMax = pAxisBR.x;
	xPMin = pAxisTL.x;
	yPMax = pAxisBR.y;
	yPMin = pAxisTL.y;
	
	if(!InvalidateCtrl())
		return FALSE;	 

	return TRUE;
}
	
BOOL CGraphCtrl::SetGridColor(COLORREF crGrid)
{
	BOOL bRet = FALSE;	

	m_crGrid = crGrid;

	CClientDC dc(this);
	
	if(m_pMemDC->DeleteDC())
	{
		if(InvalidateCtrl())
		{
			bRet = TRUE;
		}			
	}		

	return bRet;
}

BOOL CGraphCtrl::SetBkColor(COLORREF crBk)
{
	BOOL bRet = FALSE;	

	m_crBackGround = crBk;

	CClientDC dc(this);
	
	if(m_pMemDC->DeleteDC())
	{
		if(InvalidateCtrl())
		{
			bRet = TRUE;
		}			
	}		

	return bRet;
}

BOOL CGraphCtrl::Update()
{
	BOOL bRet = FALSE;	

	CClientDC dc(this);

	if(m_pMemDC->DeleteDC())
	{
		if(InvalidateCtrl())
		{
			bRet = TRUE;
		}			
	}		

	return bRet;
}

void CGraphCtrl::DrawBands()
{	
	if(!m_pMemDC->GetSafeHdc())
		return;

	CRect bkRect(m_rcClient.right - 3, m_rcClient.top, m_rcClient.right, m_rcClient.bottom);

	CBrush bkBrush;
	bkBrush.CreateSolidBrush(m_crBackGround);
	m_pMemDC->FillRect(bkRect, &bkBrush);

	m_pMemDC->BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), m_pMemDC, 3, 0, SRCCOPY);

	int H = 255,
		S = 172,
		L = 150;

	if ( nLines > 1 )
		H = 255/(nLines-1);

	for (int l = 0 ; l < nLines ; l++ )
	{	
		m_Lines[l].logScale = logScale;

		if ( l > 0 ) 
		{
			m_Lines[l].Square = false;
			RGB_Col p = hslToRgb(H + H*(l-1), S, L);
			m_Lines[l].SetPen(2.5,RGB(int(p.r), int(p.g), int(p.b)));

			if ( nLines == 2 )
			
				m_Lines[l].SetPen(2.5,RGB(15,117,209));
		}

		if (!logScale && nLines == 3)
		{	
			m_Lines[0].color = RGB(91, 155, 213);
			m_Lines[1].color = RGB(237, 125, 49);
			m_Lines[2].color = RGB(255, 0, 0);
		}

		if (!logScale && nLines == 1)
		
			m_Lines[0].color = RGB(91, 155, 213);

		if ( m_Lines[l].check )
		{	
			m_Lines[l].SetMaxMin(xMax, xMin, yMax, yMin, xPMax, xPMin, yPMax, yPMin); 
			m_Lines[l].DrawBand(m_pMemDC,m_rcClient);
		}
	}	
}

BOOL CGraphCtrl::InvalidateCtrl()
{
	CClientDC dc(this);

	if(m_pMemDC->GetSafeHdc())
		return FALSE;

	if(!m_pMemDC->CreateCompatibleDC(&dc))
		return FALSE;

	CBitmap bmp;
	if(!bmp.CreateCompatibleBitmap(&dc, m_rcClient.Width(), m_rcClient.Height()))
		return FALSE;

	if(!m_pMemDC->SelectObject(bmp))
		return FALSE;

	//Set the background color of the control
	CBrush bkBrush;
	if(!bkBrush.CreateSolidBrush(m_crBackGround))
		return FALSE;
	
	m_pMemDC->FillRect(m_rcClient, &bkBrush);

	//Select a specified pen to the device context to draw background lines
	CPen bkLinesPen;
	if(!bkLinesPen.CreatePen(PS_SOLID, 1, m_crGrid))
		return FALSE;

	if(!m_pMemDC->SelectObject(bkLinesPen))
		return FALSE;
	
	DrawAxis(m_pMemDC,m_rcClient);
	
	DrawBands();

	InvalidateRect(m_rcClient);

	// here are the actual clipboard functions.
	AfxGetApp()->m_pMainWnd->OpenClipboard() ;
	EmptyClipboard() ;
	SetClipboardData (CF_BITMAP, bmp.GetSafeHandle() ) ;
	CloseClipboard (); 
	
	bmp.Detach();
	//m_pMemDC->DeleteDC();

	

	return TRUE;
}

void CGraphCtrl::ReadOutputIsoCheck(char *NameFile, int index)
{	
	int nPts = 0;

	FILE *filePtr;
	
	char buffer[2048];
	
	if ( (filePtr = fopen( NameFile,"r")) == NULL )
		printf( "The file %s was not opened\n", NameFile );
	else
		printf( "The file %s was opened\n", NameFile );
	
	/* Set pointer to beginning of file: */
	fseek( filePtr, 0L, SEEK_SET );
	nPts = 0;

	fgets(buffer,2048,filePtr);
	fscanf( filePtr, "%d \n", &nPts);

	nLines = index + 1;

	m_Lines = new CGraphLine[nLines];

	m_Lines[index].nPts = nPts;

	delete[] m_Lines[index].X;
	delete[] m_Lines[index].Y;

	m_Lines[index].X = new double[nPts];
	m_Lines[index].Y = new double[nPts];
	
	fgets(buffer,2048,filePtr);

	xMin = 1e10;
	xMax = 0;
	yMin = 1e10;
	yMax = 0;

	for ( int i = 0 ; i < nPts; i++ )
	{	
		fscanf( filePtr, "%lf %lf\n", &m_Lines[index].X[i], &m_Lines[index].Y[i] );

		if ( xMin > m_Lines[index].X[i] ) xMin = m_Lines[index].X[i];

		if ( xMax < m_Lines[index].X[i] ) xMax = m_Lines[index].X[i];

		if ( yMin > m_Lines[index].Y[i] ) yMin = m_Lines[index].Y[i];

		if ( yMax < m_Lines[index].Y[i] ) yMax = m_Lines[index].Y[i];
	}

	
	if (abs(xMax - xMin) < 1)
	{
		xMin = (xMin*0.9);
		xMax = (xMax*1.1);
	}
	else
	{
		xMin = 0;
		xMax = ceil(xMax*1.01);
	}

	if (abs(yMax - yMin) < 1)
	{
		yMin = (yMin*0.9);
		yMax = (yMax*1.1);
	}
	else
	{	
		if ( yMin > 0)
			yMin = 0;
		yMax = ceil(yMax*1.01);
	}

	fclose(filePtr);
}

void CGraphCtrl::ReadOutputIsoDer(const char *NameFile)
{	
	int nGen,
		nPts = 0;

	FILE *filePtr;
	
	char buffer[12048],
		 *p;
	
	if ( (filePtr = fopen( NameFile,"r")) == NULL )
		printf( "The file %s was not opened\n", NameFile );
	else
		printf( "The file %s was opened\n", NameFile );
	
	/* Set pointer to beginning of file: */
	fseek( filePtr, 0L, SEEK_SET );
	nGen = 0;

	while(!feof(filePtr))
	{
		if ( fgets(buffer,12048,filePtr) != NULL )
		{	
			if ( nGen ==0 )
			{	
				bool check = false;

				p=strtok (buffer," ");
				while (p!=NULL)
				{	
					p=strtok(NULL," ");
					
					if ( !check ) if (strcmp(p, "Time/Vol") == 0) check = true;

					if (check)
					{
						if (p)
						{
							double val = atof(p);
							if (val > 0) nPts++;
						}
					}
				}

				if (nPts <= 0) AfxMessageBox("No Field Data");
 
			}
			
			nGen++;

		}

	}
	
	
	nGen -= 2;

	m_Lines = new CGraphLine[nGen + 1];

	for (int i = 0; i < nGen + 1; i++)
	{
		m_Lines[i].nPts = nPts;

		delete[] m_Lines[i].X;
		delete[] m_Lines[i].Y;

		m_Lines[i].X = new double[nPts];
		m_Lines[i].Y = new double[nPts];
	}

	/* Set pointer to beginning of file: */
	fseek( filePtr, 0L, SEEK_SET );

	int l = 0,
		j;

	vector<string> vec;
	int ind = 10,
		nX = 0;

	while( !feof(filePtr) )
	{
		if ( fgets(buffer,12048,filePtr) != NULL )
		{	
			p=strtok (buffer," ");
			while (p!=NULL)
			{
				vec.push_back(p);
				p=strtok(NULL," ");
			}

			if ( l == 0 )
			{	
				xMax = atof((char*)vec[vec.size()-2].c_str()); 
				for ( int i = 0 ; i < nGen+1; i ++ )
				{
					for ( j = 0 ; j < vec.size() ; j++ )
					{
						if (strcmp((char*)vec[j].c_str(), "Time/Vol") == 0 && i == 0) ind = j;
							
						if (j > ind)
						{	
							double tmp = atof((char*)vec[j].c_str());
							m_Lines[i].X[j - ind - 1] = tmp;
						}
						
					}
				}
			}
			else
			{	
				if (l == 1)
				{	
					char *pch;
					
					ind = 10;

					for (j = 0; j < vec.size(); j++)
					{
						if (strcmp((char*)vec[j].c_str(), "Prof") == 0) ind = j;

						if (j > ind)
						{
							m_Lines[l - 1].Y[j - ind-1] = atof((char*)vec[j].c_str());
							if (yMax < m_Lines[l - 1].Y[j - ind - 1]) yMax = m_Lines[l - 1].Y[j - ind - 1];
						}

						if (strcmp((char*)vec[j].c_str(), "OptObj") == 0) nX = j;
					}
				
				}

				if (l > 1)
				{	
					for (j = 0; j < vec.size(); j++)
					{
						if (j > ind)
						{
							m_Lines[l - 1].Y[j - ind - 1] = atof((char*)vec[j].c_str());
							if (yMax < m_Lines[l - 1].Y[j - ind - 1]) yMax = m_Lines[l - 1].Y[j - ind - 1];
						}

					}

					if ( nX > 2 )
						m_Lines[l-1].m_strName.Format("Gen%d k=%-.f, n=%-.2f, r=%-.3f",l-2,atof((char*)vec[0].c_str()),atof((char*)vec[1].c_str()),atof((char*)vec[2].c_str()));
					else
						m_Lines[l-1].m_strName.Format("Gen%d k=%-.f, n=%-.2f",l-2,atof((char*)vec[0].c_str()),atof((char*)vec[1].c_str()));
				}
			}
			
			vec.erase(vec.begin(),vec.end());
			l++;
		}
	}

	nLines = nGen+1;

	fclose(filePtr);
	
}


void CGraphCtrl::DrawAxis(CDC *pDC, CRect rect)
{	
	int i;

	CFont titleFont;
	
	titleFont.CreatePointFont(5*rect.Height()*3/40, _T("Arial"));

	COLORREF color = RGB( 255, 255, 255);     // White
	CBrush lSolidBrush(color);
	pDC->FillRect(rect,&lSolidBrush);

	// Draw Title
	if ( bTitle ) DrawTitle(pDC, rect);

	// Draw Tics and Grid
	CPen penGrid( PS_DOT, 0.3 , RGB(200,200,200));
	CPen penAxis( PS_SOLID, 1.5 , RGB(230,0,75));

	int nXTics,
		 nYTics,
		 xpt,
		 ypt,
		 n1,
		 n2;

	double xMinTic,
		   DxTic,
		   yMinTic,
		   DyTic,
		   TempDxTic,
		   TempDyTic,
		   xMaxTic,
		   yMaxTic;

	// X axis
/*	DxTic = log10(xMax - xMin);
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
		xMaxTic = DxTic * nXTics;
	}

	if( abs(5-nXTics) > abs(5-n2) )
	{
		nXTics = n2;
		TempDxTic = DxTic / 5;	
		xMaxTic = DxTic * nXTics;
	}
	

	DxTic = TempDxTic;
	xMinTic = xMin;
	*/
	
	NiceTics(xMin, xMax, DxTic, nXTics);

	

	// Y axis
	if ( logScale )
	{
		yMax = log10(yMax);
		if (yMin > 0)
			yMin = log10(yMin);
		else
			yMin = -1;
		

/*		DyTic = log10(ceil(yMax) - floor(yMin));
		DyTic = pow(10.0,int(DyTic));
		nYTics = int((yMax - yMin)/DyTic);
		yMaxTic = DyTic * nYTics;

		TempDyTic = DyTic;
	
		DyTic = 1;

		yMinTic = yMin;*/

		// This is to make number of tics in the log scale in powers ot 10 from ceil and floor of ymax and ymin respectively
		yMax = ceil(yMax);
		nYTics = int(ceil(yMax) - floor(yMin)) + 1;
		DyTic = 1;
	}
	else
	{
/*		DyTic = log10(yMax - yMin);
		DyTic = pow(10.0,int(DyTic));
		nYTics = int((yMax - yMin)/DyTic);
		yMaxTic = DyTic * nYTics;

		n1 = int(2 * (yMax - yMin)/DyTic);
		n2 = int(5 * (yMax - yMin)/DyTic);

		TempDyTic = DyTic;
	
		if ( abs(5-nYTics) > abs(5-n1) )
		{
			nYTics = n1; 
			TempDyTic = DyTic / 2;
			yMaxTic = DyTic * nYTics;
		}

		if( abs(5-nYTics) > abs(5-n2) )
		{
			nYTics = n2;
			TempDyTic = DyTic / 5;	
			yMaxTic = DyTic * nYTics;
		}
	
		DyTic = TempDyTic;
		yMinTic = yMin;*/

		NiceTics(yMin, yMax, DyTic, nYTics);
	}

	// Axis Legends 
	CFont xAxisFont, yAxisFont;
	CSize scaleSize;
	CRect scaleRect;
	CString xAxisScale;

	xAxisFont.CreatePointFont( xFont, _T("Arial"));

	// X-Axis tics
	double xTic = 0;

	for ( i = 0; i<nXTics ; i++)
	{
		xTic = xMin + float(i) * DxTic;

		xpt = int( ((xPMax-xPMin)/(xMax - xMin)) * (xTic-xMin) + xPMin );
		ypt = yPMax;	
		
		pDC->SelectObject(&penGrid);
		
		if( i > 0 && i <= nXTics )
		{
			pDC->MoveTo( xpt, yPMin );	
			pDC->LineTo( xpt, yPMax);
		}

		pDC->SelectObject(&penAxis);
		
		pDC->MoveTo( xpt, yPMax );	
		pDC->LineTo( xpt, yPMax + 3);
		
		pDC->SelectObject(&xAxisFont);
		xAxisScale.Format("%.4g",xTic);
		xAxisScale.Replace(_T("e+000"), _T("e+"));
		xAxisScale.Replace(_T("e+00"), _T("e+"));
		xAxisScale.Replace(_T("e+0"), _T("e+"));
		scaleSize = pDC->GetTextExtent(xAxisScale);
		
		if (i == 0 ) xpt += 2;

		if( scaleSize.cx%2 == 0 )
		{
			scaleRect.SetRect(xpt-scaleSize.cx/2, ypt+3, xpt+scaleSize.cx/2, ypt+3+scaleSize.cy);
		}	
		else
		{
			scaleRect.SetRect(xpt-int(scaleSize.cx/2)-1, ypt+3, xpt+scaleSize.cx/2, ypt+3+scaleSize.cy);
		}

		CBrush WhiteBrush(RGB(255,255,255));
		pDC->FillRect(scaleRect,&WhiteBrush);
		pDC->DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);

		
	}

	// Print the xMax 
	CRect scaleRectxMax;
	xAxisScale.Format("%.4g",xMax<1 ? xMax : int(xMax));
	xAxisScale.Replace(_T("e+000"), _T("e"));
	xAxisScale.Replace(_T("e+00"), _T("e"));
	xAxisScale.Replace(_T("e+0"), _T("e"));
	scaleSize = pDC->GetTextExtent(xAxisScale);

	if( scaleSize.cx%2 == 0 )
	{
		scaleRectxMax.SetRect(xPMax-scaleSize.cx/2, yPMax+3, xPMax+scaleSize.cx/2, yPMax+3+scaleSize.cy);
	}	
	else
	{
		scaleRectxMax.SetRect(xPMax-int(scaleSize.cx/2)-1, yPMax+3, xPMax+scaleSize.cx/2, yPMax+3+scaleSize.cy);
	}
	
	// X Axis Label
	CFont xLabelFont;
	xLabelFont.CreatePointFont(xLabFont, _T("Arial"));

	CRect xLabelRect;
	pDC->SelectObject(&xLabelFont);
	
	if (xLabel != "")
	{
		CSize xLabelSize = pDC->GetTextExtent(xLabel);
		xLabelRect.SetRect((xPMin+xPMax-xLabelSize.cx)/2, rect.bottom-xLabelSize.cy-4, (xPMin+xPMax+xLabelSize.cx)/2, (rect.bottom-xLabelSize.cy-2));
		pDC->DrawText(xLabel, xLabelRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);
	}

	int yPDelta;

	yPDelta = int(yPMax-yPMin);
	
	yAxisFont.CreatePointFont( yFont, _T("Arial"));

	// Y Axis Tics
	yMinTic = yMin;
	
	double yTic =0;

	for ( i = 0; i<nYTics ; i++)
	{	
		yTic = yMin + float(i) * DyTic;

		ypt = int( ((yPMax-yPMin)/(yMax - yMin))*(yMin-yTic) + yPMax );

		pDC->SelectObject(&penGrid);
		
		if( i > 0 )
		{	
			pDC->MoveTo( xPMax ,ypt );	
			pDC->LineTo( xPMin, ypt);
		}
			
		pDC->SelectObject(&penAxis);
			
			
		pDC->MoveTo( xPMin , ypt );	
		pDC->LineTo( xPMin - 3, ypt);
				
		pDC->SelectObject(&yAxisFont);
		
		if ( logScale )
		{
			xAxisScale.Format("%.4g",pow(10, i + yMin) );
			xAxisScale.Replace(_T("e+000"), _T("e+"));
			xAxisScale.Replace(_T("e+00"), _T("e+"));
			xAxisScale.Replace(_T("e+0"), _T("e+"));
		}
		else
		{
			xAxisScale.Format("%.4g",yTic);
			xAxisScale.Replace(_T("e+000"), _T("e+"));
			xAxisScale.Replace(_T("e+00"), _T("e+"));
			xAxisScale.Replace(_T("e+0"), _T("e+"));
		}

		scaleSize = pDC->GetTextExtent(xAxisScale);
			
		if( scaleSize.cy%2 == 0 )
		{
			scaleRect.SetRect(xPMin-2-scaleSize.cx, ypt-scaleSize.cy/2, xPMin-2, ypt+scaleSize.cy/2);
		}		
		else
		{
			scaleRect.SetRect(xPMin-2-scaleSize.cx, ypt-int(scaleSize.cy/2)-1, xPMin-2, ypt+scaleSize.cy/2);
		}
				
		pDC->DrawText(xAxisScale, scaleRect, DT_CENTER|DT_VCENTER);

	}

	// X Axis Label
	CFont yLabelFont;
	yLabelFont.CreateFont( yLabFont, 0, 900, 0, 0, 0, 0, 0, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, _T("Arial"));

	CRect yLabelRect;
	pDC->SelectObject(&yLabelFont);
	
	if (yLabel != "")
	{
		CSize yLabelSize = pDC->GetTextExtent(yLabel);
		yLabelRect.SetRect(rect.left+8, (yPMax+yPMin+yLabelSize.cx)/2, rect.left+yLabelSize.cy, (yPMax+yPMin-yLabelSize.cx)/2);
		pDC->DrawText(yLabel, yLabelRect, DT_NOCLIP);
	}

	// Draw Main Axis

	CPen penRectangleB( PS_SOLID, 1.9 , RGB(255,0,0));
	pDC->SelectObject(&penRectangleB);
	
	pDC->MoveTo( xPMin , yPMax);
	pDC->LineTo( xPMax , yPMax);
	pDC->LineTo( xPMax , yPMin);
	pDC->MoveTo( xPMin , yPMax);
	pDC->LineTo( xPMin ,  yPMin );
	pDC->LineTo( xPMax ,  yPMin );

	if (logScale)
	{
		yMax = pow(10.0,yMax);
		yMin = pow(10.0,yMin);
	}

}
RGB_Col CGraphCtrl::hslToRgb( float hue, float saturation, float lightness ) 
{

    RGB_Col rgb1, rgbResult;

	hue = (hue*360.0)/255.0;
	saturation /=255.0;
	lightness /= 255.0;

    float chroma = ( 1.0 - (float) abs( 2.0 * lightness - 1.0 ) ) * saturation;
    float h1 = hue / 60.0;
    float x = chroma * ( 1.0 - (float) abs( fmodf(h1, 2.0) - 1.0 ));

    if ( ( 0 <= h1 ) && ( h1 < 1 ) ) {

        rgb1.r = chroma;
        rgb1.g = x;
        rgb1.b = 0.0;

    } else if ( ( 1 <= h1 ) && ( h1 < 2 ) ) {

        rgb1.r = x;
        rgb1.g = chroma;
        rgb1.b = 0.0;

    } else if ( ( 2 <= h1 ) && ( h1 < 3 ) ) {

        rgb1.r = 0.0;
        rgb1.g = chroma;
        rgb1.b = x;

    } else if ( ( 3 <= h1 ) && ( h1 < 4 ) ) {

        rgb1.r = 0.0;
        rgb1.g = x;
        rgb1.b = chroma;

    } else if ( ( 4 <= h1 ) && ( h1 < 5 ) ) {

        rgb1.r = x;
        rgb1.g = 0.0;
        rgb1.b = chroma;

    } else if ( ( 5 <= h1 ) && ( h1 < 6 ) ) {

        rgb1.r = chroma;
        rgb1.g = 0;
        rgb1.b = x;

    } else {

        rgb1.r = 0.0;
        rgb1.g = 0.0;
        rgb1.b = 0.0;

    }

    float m = lightness - 0.5 * chroma;

    rgbResult.r = int(255*(rgb1.r + m));
    rgbResult.g = int(255*(rgb1.g + m));
    rgbResult.b = int(255*(rgb1.b + m));

    return rgbResult;

}

BEGIN_MESSAGE_MAP(CGraphCtrl, CWnd)
	//{{AFX_MSG_MAP(CGraphCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_COPYTOCLIPBOARD, &CGraphCtrl::OnCopytoclipboard)
	ON_WM_MOUSEMOVE(IDC_STATIC_PARETO_GRAPH, OnMouseMove)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl message handlers

void CGraphCtrl::OnPaint() 
{
	CPaintDC dc(this);

	if(m_pMemDC->GetSafeHdc())
	{
		dc.BitBlt(0, 0, m_rcClient.Width(), m_rcClient.Height(), m_pMemDC, 0, 0, SRCCOPY);
	}

}

void CGraphCtrl::DrawTitle(CDC *pDC, CRect rect) 
{
	CFont titleFont;
	
	if (!logScale && nLines == 3 )

		titleFont.CreatePointFont(4*rect.Height() * 3 / 40, _T("Arial"));

	else

		titleFont.CreatePointFont(5 * rect.Height() * 3 / 40, _T("Arial"));

	CFont* pOldFont = pDC->SelectObject(&titleFont);

	CRect titleRect,
		  squareRect;
	
	CSize titleSize = pDC->GetTextExtent(title);
		
	titleRect.SetRect( int((rect.left+rect.right-titleSize.cx)/2), rect.top+1, int((rect.left+rect.right+titleSize.cx)/2), (rect.top+1+titleSize.cy));
	pDC->DrawText(title, titleRect, DT_CENTER|DT_VCENTER|DT_NOCLIP);

	if (!logScale && nLines == 3)
	{
		m_Lines[0].color = RGB(91, 155, 213);
		m_Lines[1].color = RGB(237, 125, 49);
		m_Lines[2].color = RGB(255, 0, 0);

		for (int l = 0; l < nLines; l++)
		{	
			int Sq = 3;
			
			CSize titleSize = pDC->GetTextExtent("    ");
			CSize titleSize1 = pDC->GetTextExtent("All  ");
			CSize titleSize2 = pDC->GetTextExtent("All    Pareto  ");

			if (l == 0)
			{
				CBrush SolidBlock(m_Lines[l].color);
				squareRect.SetRect(int(titleRect.left) - 2 * Sq - titleSize.cx/2, int(rect.top + 1 + titleSize.cy / 2) - Sq,
					int(titleRect.left) - titleSize.cx / 2, int(rect.top + 1 + titleSize.cy / 2) + Sq);
				pDC->FillRect(squareRect, &SolidBlock);
			}			

			if (l == 1)
			{	
				CBrush SolidBlock(m_Lines[l].color);
				squareRect.SetRect(int(titleRect.left) - Sq + titleSize1.cx, int(rect.top + 1 + titleSize.cy / 2) - Sq,
					int(titleRect.left) + titleSize1.cx + Sq , int(rect.top + 1 + titleSize.cy / 2) + Sq);
				pDC->FillRect(squareRect, &SolidBlock);
			}

			if (l == 2)
			{
				CBrush SolidBlock(m_Lines[l].color);
				squareRect.SetRect(int(titleRect.left) - Sq + titleSize2.cx, int(rect.top + 1 + titleSize.cy / 2) - Sq,
					int(titleRect.left) + titleSize2.cx + Sq, int(rect.top + 1 + titleSize.cy / 2) + Sq);
				pDC->FillRect(squareRect, &SolidBlock);
			}
		}
	}

	if ( nLines == 2)
	{
		if (title.GetLength() > 8)
		{
			// Blue pen
			CPen pen0(PS_SOLID, 2.5, RGB(15, 117, 209));

			pDC->SelectObject(&pen0);

			pDC->MoveTo(int(titleRect.left + titleRect.Width() / 2) - titleSize.cx / 10 - 2, int(rect.top + 1 + titleSize.cy / 2));

			pDC->LineTo(int(titleRect.left + titleRect.Width() / 2) + titleSize.cx / 10 + 2, int(rect.top + 1 + titleSize.cy / 2));
		}

		CBrush SolidBlock(RGB(150, 27, 29));

		int Sq = 3;
		squareRect.SetRect(int(titleRect.left - titleSize.cx / 6) - Sq, int(rect.top + 1 + titleSize.cy / 2) - Sq,
			int(titleRect.left - titleSize.cx / 6) + Sq, int(rect.top + 1 + titleSize.cy / 2) + Sq);

		pDC->FillRect(squareRect, &SolidBlock);
	}
}

void CGraphCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
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


void CGraphCtrl::OnCopytoclipboard()
{	
	
}

void CGraphCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//	if ((nFlags & MK_RBUTTON) == MK_RBUTTON)
	
	CWnd::OnMouseMove(nFlags, point);
}

void CGraphCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);

	CString st;

	double x,y;

	x = ((xMax - xMin) / (xPMax - xPMin)) * (point.x - xPMin) + xMin;
	y = ((yMax - yMin) / (yPMax - yPMin)) * (yPMin - point.y) + yMax;
	
	st.Format("X:%10.1f Y:%10.1f", x, y);

	bool check = false;
	int ind = -1;

	double xSq = ((xMax - xMin) / (xPMax - xPMin)) * (2*m_Lines[0].Sq) + xMin;
	double ySq = ((yMax - yMin) / (yPMax - yPMin)) * (2 * m_Lines[0].Sq) + yMin;

	for (int l = 0; l < nLines && !check; l++)
	{	
		for (int n = 0; n < m_Lines[l].nPts && !check; n++)
		{
			if (x >= (m_Lines[l].X[n] - xSq) && x <= (m_Lines[l].X[n] + xSq))
			{
				if (y >= (m_Lines[l].Y[n] - ySq) && y <= (m_Lines[l].Y[n] + ySq))
				{	
					ind = n;
					check = true;
				}
			}
		}
	}
		
/*	if (!check) st = "NO";

	else st = "YES";
	
	TextOutA(dc, 10, 20, st, st.GetLength());*/
	
	// Send Message to parent Window
	GetParent()->SendMessage(UWM_CUSTOM, (WPARAM)ind);

	CWnd::OnLButtonDown(nFlags, point);
}

// Code sourced from http://www.realtimerendering.com/resources/GraphicsGems/gems/Label.c
void CGraphCtrl::NiceTics(double& min, double& max, double& DTic, int& nTics)
{
	char str[6], temp[20];
	int nfrac;
	double d;				/* tick mark spacing */
	double graphmin, graphmax;		/* graph range min and max */
	double range, x;

	nTics = 5;
	/* we expect min!=max */
	range = niceNum(max - min, 0);
	if (range == 0)
	{
		range = niceNum(m_Lines[0].yMax - m_Lines[0].yMin, 0);
	}
	d = niceNum(range / (nTics - 1), 1);
	graphmin = floor(min / d)*d;
	graphmax = ceil(max / d)*d;

	nfrac = (int)MAX(-floor(log10(d)), 0);	/* # of fractional digits to show */
	sprintf_s(str, sizeof(str), "%%.%df", nfrac);	/* simplest axis labels */

	nTics = 0;
	printf("min %lf max%lf\n", min, max);
	printf("graphmin=%g graphmax=%g increment=%g\n", graphmin, graphmax, d);
	for (x = graphmin; x < graphmax + .5*d; x += d) {
		sprintf_s(temp, sizeof(temp), str, x);
		printf("(%s)\n", temp);
		nTics++;
	}

	DTic = d;
	min = graphmin;
	max = graphmax;
}

double CGraphCtrl::niceNum(double x, int round)
{
	int expv;				/* exponent of x */
	double f;				/* fractional part of x */
	double nf;				/* nice, rounded fraction */

	expv = (int)floor(log10(x));
	f = x / pow(10., expv);		/* between 1 and 10 */
	if (round)
		if (f < 1.5) nf = 1.;
		else if (f < 3.) nf = 2.;
		else if (f < 7.) nf = 5.;
		else nf = 10.;
	else
		if (f <= 1.) nf = 1.;
		else if (f <= 2.) nf = 2.;
		else if (f <= 5.) nf = 5.;
		else nf = 10.;
	return nf * expt(10., expv);
}