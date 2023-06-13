// HC.h: interface for the HC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HC_H__18A6FBB8_973E_43ED_AC4E_3961FB2FA0CE__INCLUDED_)
#define AFX_HC_H__18A6FBB8_973E_43ED_AC4E_3961FB2FA0CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRadial.h"
#include "Chromosome.h"
#include "ProgressBar.h"

class HC  
{
public:
	HC();
	virtual ~HC();
	CRadial RadModel;
	
	void CreateDlg(CWnd* pParent);
	void RunHC(CRadial &, const char *pszPathName, int, int, double, double, double, FILE *);
	void InitPopHC(CRadial &RadMod, const char *pszPathName, int, FILE *, Chromosome &);
	void evaluate(CRadial &, double, double, double);
	bool Misfit( int, double, double &, double &);
	double Gauss(double, double);
	int ProcessMessageLoop();

	int nP,		 // Number of dimensions
		 MaxGen,	 // Maximum Number of generations
		 RandHC,  // Hill Climber Random Seed (Yes=0, No=1)
		 type,	 // Misfit type, = 0 standard, = 1 Natural Logarithm
		 Runs;	 // Number of runs
		 
	double TrainPer, // Percentage of profile for misfiot calculation
			 *TimeVol,
			 *watConc;

	double kMax,
			 kMin,
			 nMax,
			 nMin,
			 rMax,
			 rMin;
	
	ProgressBar ProBar;

private:
	
	int nPoint,
		 Units;	// 0 Field, 1 Metric 		 
};

#endif // !defined(AFX_HC_H__18A6FBB8_973E_43ED_AC4E_3961FB2FA0CE__INCLUDED_)
