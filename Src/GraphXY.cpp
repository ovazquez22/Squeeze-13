// GraphXY.cpp: implementation of the CGraphXY class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FastDialog 5.h"
#include "GraphXY.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphXY::CGraphXY()
{	
	MaxNumTimes = 10;
	nPts = 0;
	nLay = 1;
	nDat = 2;
	nFileNum = 1;
	Allocate();
}

CGraphXY::CGraphXY(int Num, int l, int n, int nP, int nFile)
{	
	MaxNumTimes = Num;
	nLay = l;
	nDat = n;
	nPts = nP;
	nFileNum = nFile;
	Allocate();
}

CGraphXY::~CGraphXY()
{
	DeAllocate();
}

void CGraphXY::Equate(const CGraphXY& rhs)
{	
	int i,j,c;

	nPts = rhs.nPts;
	MaxNumTimes = rhs.MaxNumTimes;
	nLay = rhs.nLay;
	nDat = rhs.nDat;
	nFileNum = rhs.nFileNum;

	for( i = 0 ; i < MaxNumTimes ; i++ )
	{	
		x[i] = rhs.x[i];
		
		for( j = 0; j < nLay ; j++ )
		{
			y[i][j] = rhs.y[i][j];

			for( c = 0; c < nDat ; c++ )
			{
				y1[i][j][c] = rhs.y1[i][j][c];
			}
		}
	}
	
	for( i = 0 ; i < nPts ; i++ )
	{
		xP[i] = rhs.xP[i];
		yP[i] = rhs.yP[i];
	}
	
	for( i = 0 ; i < nFileNum ; i++ )
	{
		pFile[i].NameFile, rhs.pFile[i].NameFile;
	}
}

void CGraphXY::Allocate()
{	
	int i,j,c;

	try
	{
		xP = new double [nPts];
		yP = new double [nPts];
		
		x = new double [MaxNumTimes];
		y = new double * [MaxNumTimes];

		for( i = 0 ; i < MaxNumTimes ; i++ )

			y[i] = new double [nLay];

		for( i = 0 ; i < MaxNumTimes ; i++ )
			
			for( j = 0; j < nLay ; j++ )
			
					y[i][j] = 0;

		y1 = new double ** [MaxNumTimes];

		for( i = 0 ; i < MaxNumTimes ; i++ )
		{
			y1[i] = new double * [nLay];
			
			for( j = 0 ; j < nLay ; j++ )
				
				y1[i][j] = new double [nDat];
		}

		for( i = 0 ; i < MaxNumTimes ; i++ )
			
			for( j = 0; j < nLay ; j++ )

				for( c = 0; c < nDat ; c++ )
			
					y1[i][j][c] = 0;


//		pFile = new OutFile [nFileNum];	
	}	

	catch(CMemoryException* e)
	{
		e->ReportError();
		e->Delete();
	}
	
}

void CGraphXY::DeAllocate()
{	
	int i, j;

	delete [] x;
	
	for( i = 0 ; i < MaxNumTimes ; i++ )
	{	
		for( j = 0 ; j < nLay ; j++ )
		{
			delete [] y1[i][j];
		}
	
	}
	
	for( i = 0 ; i < MaxNumTimes ; i++ )
		
		delete [] y1[i];
	
	delete [] y1;

	for( i = 0; i<MaxNumTimes; i++)
	
		delete [] y[i];

	delete [] y;

	delete [] xP;
	delete [] yP;

//	delete [] pFile;

	return;
}

void CGraphXY::SetMaxNumTimes(int Num, int l, int n, int nP, int nFile)
{
	DeAllocate();
	MaxNumTimes = Num;
	nLay = l;
	nDat = n;
	nPts = nP;
	nFileNum = nFile;
	Allocate();
}

void CGraphXY::AddElement()
{
	CGraphXY temp(MaxNumTimes, nLay, nDat, nPts, nFileNum);	// create temp collection
	temp.Equate(*this);
	DeAllocate();		// delete resources
	MaxNumTimes++;			// add 1 to the number of DeltaRs
	Allocate();			// create required resources
	MaxNumTimes--;			// fool Equate into copying old info
	Equate(temp);
	MaxNumTimes++;
}

