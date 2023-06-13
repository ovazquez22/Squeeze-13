// AnaliticalLinear.h: interface for the CAnaliticalLinear class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANALITICALLINEAR_H__9D6D39B0_3178_4EE3_89DF_D66C909823D9__INCLUDED_)
#define AFX_ANALITICALLINEAR_H__9D6D39B0_3178_4EE3_89DF_D66C909823D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAnaliticalLinear  
{
public:
	CAnaliticalLinear();
	virtual ~CAnaliticalLinear();
	CAnaliticalLinear& operator=(const CAnaliticalLinear&);	// assignment operator
	void Equate(const CAnaliticalLinear& );

	double Phi,
		    VolInj,
			 VolOve,
			 VolMin,
			 VolMax,
			 InhCon,
			 CSE,
			 Tol,
			 xk,
			 xn,
			 Kiso,
			 Clife[MaxClife];

	int nPtc,
		 nSpace,
		 nItMax,
		 nLife;
};

#endif // !defined(AFX_ANALITICALLINEAR_H__9D6D39B0_3178_4EE3_89DF_D66C909823D9__INCLUDED_)
