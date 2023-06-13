// Optimization.cpp: implementation of the Optimization class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Optimization.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Optimization::Optimization()
{	
	nDim = 3;	 // Number of dimensions of the Parameter Space 
	popsize = 50;// Population size
	lchrom = 30; // Chromosome length
	nGen = 20;	 // Number of generations
	type = 1;	 // Misfit type, = 0 standard, = 1 Natural Logarithm
	OptAlg = 1;	 // HC = 0, PSO = 1, RCGA = 2, SGA = 3
	RandHC = 1;	 // Hill Climber Random Seed (Yes=0, No=1)
	HC = 0;		 // Random initial population = 0; HC = 1
	TrainPer = 1;// Percentage of profile for misfiot calculation
	pcross = 0.6;//x-over probability	 
	pmut = 0.005;			// Mutation probability
	HCGen = 50;	 // Number of Generations for Hill Climbing Initial Population
	HCRun = 1;   // Number of Realitations for Hill Climbing Algorithm

	nStages = 1;

	xL = new double[3];	// Lower Limit of Parameter Space
	xU = new double[3];// Upper Limit of Parameter Space
	
	stages = new int [nStages];

	xU[0] = 3000.0;
	xU[1] = 1.0;
	xU[2] = 1.0;
	
	xL[0] = 1.0;
	xL[1] = 0.001;
	xL[2] = 0.001;
}

Optimization::~Optimization()
{
	delete [] xU;
	delete [] xL;
	delete [] stages;
}

Optimization & Optimization::operator=( const Optimization &opt )
{
	int i,j,n;
	
	if (this == &opt)
		return *this;
	
	nDim = opt.nDim;
	popsize = opt.popsize; 
	lchrom = opt.lchrom;
	nGen = opt.nGen;
	type = opt.type;
	OptAlg = opt.OptAlg;
	RandHC = opt.RandHC;
	HC = opt.HC;
	TrainPer = opt.TrainPer;
	pcross = opt.pcross;
	pmut = opt.pmut;
	HCGen = opt.HCGen;
	HCRun = opt.HCRun;

	for ( i = 0 ; i < 3 ; i++)
	{
		xU[i] = opt.xU[i]; 
		xL[i] = opt.xL[i]; 
	}
}