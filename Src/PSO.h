// PSO.h: interface for the PSO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PSO_H__6B52EA1B_AAE2_4E4A_8023_154BD0A422F7__INCLUDED_)
#define AFX_PSO_H__6B52EA1B_AAE2_4E4A_8023_154BD0A422F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CRadial.h"
#include "Population.h"
#include "ProgressBar.h"

class PSO  
{
public:
	PSO();
	virtual ~PSO();
	CRadial RadModel;

	void RunPSO(CRadial &, FILE *, FILE *, double *, double *);
	void RunTracerPSO(CRadial &, FILE *, FILE *, double *, double *);
	void RunTestPSO(CRadial &,  FILE *);
	void CreateDlg(CWnd* pParent);
	int ProcessMessageLoop();

	bool EQ;

	int nP,		 // Number of dimensions
		 popsize, // Population size
		 MaxGen,	 // Maximum Number of generations
		 type,	 // Misfit type, = 0 standard, = 1 Natural Logarithm
		 HC,		 // Random initial population = 0; HC = 1
		 nStages,
		 *stages;

	double TrainPer,	// Percentage of profile for misfiot calculation
			 c1,			// Cognitive parameter 
			 c2;			// Social parameter
	
	Population pop;
	
	ProgressBar ProBar;

private:
	
	int nPoint,
		 Units;	// 0 Field, 1 Metric 
};

#endif // !defined(AFX_PSO_H__6B52EA1B_AAE2_4E4A_8023_154BD0A422F7__INCLUDED_)
