/////////////////////////////////////////////////////////////////////////////
// CGraphLine
#include "stdafx.h"
#include "CGraphLine.h"

CGraphLine::CGraphLine()
{	
	nPts = 1;
	check = true;
	X = new double[nPts];
	Y = new double[nPts];
	logScale = true;
	Square = true;
	Point = false;
	Circle = false;
}

CGraphLine::CGraphLine(LPCTSTR lpszName, COLORREF crColor, 
																		 int nWidth, UINT uLower, UINT uUpper)
:m_strName(lpszName)
{			
	SetRange(uLower, uUpper);
	m_pen.CreatePen(PS_SOLID, nWidth, crColor);
	logScale = true;
	Square = true;
	Point = false;
	Circle = false;
}

CGraphLine::~CGraphLine()
{
	delete[] X;
	delete[] Y;
}

void CGraphLine::SetRange(UINT uLower, UINT uUpper)
{
	ASSERT(uLower && uUpper && uLower < uUpper);
	m_uUpper = uUpper - uLower + 1;
	m_uLower = 1;
	m_uOffset = uLower - 1;
}

void CGraphLine::SetPen(int nWidth, COLORREF crColor)
{
	m_pen.DeleteObject();
	m_pen.CreatePen(PS_SOLID, nWidth, crColor);
}

void CGraphLine::DrawBand(CDC* pDC, RECT& rcBand)
{	
	int i;

	if(!pDC->GetSafeHdc())
		return;	

	// Field Data Graph
	Sq = 3;
	CBrush SolidBlock(RGB(150,27,29)); 
	CRect squareRect,
		circleRect;
	
	// Pen for Field Data
	CPen m_penFD;
	m_penFD.CreatePen(PS_SOLID, 0.5, RGB(150,27,29));
	
	double tyMin,
		   tyMax,
		   *tY;

	tY = new double [nPts];

	if (logScale)
	{	
		for ( i = 0; i < nPts ; i++ )
		{
			if ( Y[i] > yMin ) 
				tY[i] = log10(Y[i]);
			else
				tY[i] = log10(yMin);
		}
		if ( yMin > 0  )
			tyMin = log10(yMin);
		else
			tyMin = -1;
	
		tyMax = log10(yMax);
	}
	else
	{	
		tyMin = yMin;
		tyMax = yMax;
		for ( i = 0; i < nPts ; i++ )
			tY[i] = Y[i];
	}
	
	if (Circle)
	{
		CBrush SolidBrush;
		SolidBrush.CreateSolidBrush(color);
		for (i = 0; i < nPts; i++)
		{
			int rd = 4;

			if (tY[i] >= tyMin && tY[i] <= tyMax && X[i] >= xMin && X[i] <= xMax)
			{
				circleRect.SetRect(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) - rd,
					int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) - rd,
					int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) + rd,
					int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) + rd);

				pDC->FillRect(circleRect, &SolidBrush);
				
			}
		}
	}

	if (Point) // Points
	{	
		CBrush SolidBlock(color);

		for (i = 0; i < nPts; i++)
		{
			if (tY[i] >= tyMin && tY[i] <= tyMax && X[i] >= xMin && X[i] <= xMax)
			{
				squareRect.SetRect(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) - Sq,
					int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) - Sq,
					int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) + Sq,
					int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) + Sq);

				pDC->FillRect(squareRect, &SolidBlock);
			}
		}
	}
	else
	{
		if (Square) // Field Data
		{
			for (i = 0; i < nPts; i++)
			{
				if (tY[i] >= tyMin && tY[i] <= tyMax && X[i] >= xMin && X[i] <= xMax)
				{
					squareRect.SetRect(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) - Sq,
						int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) - Sq,
						int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin) + Sq,
						int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax) + Sq);

					pDC->FillRect(squareRect, &SolidBlock);

					if (i < nPts - 1)
					{
						pDC->SelectObject(&m_penFD);

						if (tY[i] >= tyMin && tY[i] <= tyMax && tY[i + 1] <= tyMax && X[i] >= xMin && X[i] <= xMax && X[i + 1] <= xMax)
						{
							pDC->MoveTo(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin),
								int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax));

							pDC->LineTo(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i + 1] - xMin) + xPMin),
								int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i + 1]) + yPMax));
						}
					}
				}
			}
		}
		else // Simulated Lines
		{
			for (i = 0; i < nPts - 1; i++)
			{
				pDC->SelectObject(&m_pen);

				if (tY[i] >= tyMin && tY[i] <= tyMax && tY[i + 1] <= tyMax && X[i] >= xMin && X[i] <= xMax && X[i + 1] <= xMax)
				{
					pDC->MoveTo(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i] - xMin) + xPMin),
						int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i]) + yPMax));

					pDC->LineTo(int(((xPMax - xPMin) / (xMax - xMin)) * (X[i + 1] - xMin) + xPMin),
						int(((yPMax - yPMin) / (tyMax - tyMin))*(tyMin - tY[i + 1]) + yPMax));
				}
			}
		}
	}

	delete [] tY;
}

void CGraphLine::SetMaxMin(double txMax, double txMin, double tyMax, double tyMin, int txPMax, int txPMin, int tyPMax, int tyPMin)
{
	xMax = txMax;
	xMin = txMin;
	yMax = tyMax;
	yMin = tyMin;

	xPMax = txPMax;
	xPMin = txPMin;
	yPMax = tyPMax;
	yPMin = tyPMin;	
}