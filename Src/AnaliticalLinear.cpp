// AnaliticalLinear.cpp: implementation of the CAnaliticalLinear class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "AnaliticalLinear.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnaliticalLinear::CAnaliticalLinear()
{
	Phi = 0.25;
   VolInj = 500;
	VolOve = 500;
	VolMin = 1000;
	VolMax = 8000;
	InhCon = 150000;
	CSE = 30000; 
	Tol = 1.00e-006;
	Clife[MaxClife];

	Kiso = -1;
	xk = 500;
	xn = 0.4;
	nPtc = 50;
	nSpace = 1;
	nItMax = 50;

	nLife = 4;
	Clife[0] = 50.0000;
	Clife[1] = 10.0000;
	Clife[2] = 5.0000;
	Clife[3] = 1.0000;
}

CAnaliticalLinear::~CAnaliticalLinear()
{

}

void CAnaliticalLinear::Equate(const CAnaliticalLinear& rhs)
{
	Phi = rhs.Phi;
	VolInj = rhs.VolInj; 
	VolOve = rhs.VolOve; 
	VolMin = rhs.VolMin; 
	VolMax = rhs.VolMax; 
   InhCon = rhs.InhCon; 
	CSE = rhs.CSE; 
	Tol = rhs.Tol; 

	Kiso = rhs.Kiso; 
	xk = rhs.xk; 
	xn = rhs.xn; 
	nPtc = rhs.nPtc; 
	nSpace = rhs.nSpace; 
	nItMax = rhs.nItMax; 
	nLife = rhs.nLife;

	for( int i = 0 ; i <nLife ; i++ )
		
		Clife[i] = rhs.Clife[i];
}

CAnaliticalLinear& CAnaliticalLinear::operator=(const CAnaliticalLinear& rhs)
{
	if (this == &rhs) return *this;
	
	Equate(rhs);

	return *this;
}