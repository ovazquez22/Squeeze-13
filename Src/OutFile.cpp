// OutFile.cpp: implementation of the OutFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "OutFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OutFile::OutFile()
{
	nPts = 1; 
	nCols = 3;
	Allocate();
}

OutFile::~OutFile()
{
	DeAllocate();
}

void OutFile::Allocate(void)
{
	outArray = new double * [nCols];

	for( int i = 0 ; i < nCols ; i++ )

		outArray[i] = new double [nPts];
}

void OutFile::DeAllocate(void)
{
	for( int i = 0; i<nCols; i++)
	
		delete [] outArray[i];

	delete [] outArray;
}

void OutFile::Equate(const OutFile& rhs)
{	
	int i,j;
	
	nPts = rhs.nPts;
	nCols = rhs.nCols;
	strcpy(NameFile, rhs.NameFile);

	for( i = 0 ; i < nCols ; i++ )
		
		for( j = 0; j < nPts ; j++ )
		
			outArray[i][j] = rhs.outArray[i][j];

}