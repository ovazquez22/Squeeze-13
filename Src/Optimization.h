// Optimization.h: interface for the Optimization class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPTIMIZATION_H__F13954EC_4844_4731_B405_22ED766C20D7__INCLUDED_)
#define AFX_OPTIMIZATION_H__F13954EC_4844_4731_B405_22ED766C20D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Optimization  
{
public:
	Optimization();
	virtual ~Optimization();
	Optimization & operator=( const Optimization &);

public:
	int nDim,	 // Number of dimensions of the Parameter Space 
		popsize, // Population size
		lchrom,  // Chromosome length
		nGen,	 // Number of generations
		Fixed_Target,
		type,	 // Misfit type, = 0 standard, = 1 Natural Logarithm
		OptAlg,	 // HC = 0, PSO = 1, RCGA = 2, SGA = 3
		RandHC,	 // Hill Climber Random Seed (Yes=0, No=1)
		HC,		 // Random initial population = 0; HC = 1
		HCRun,	 // Number of Runs for Hill Climbing Algorithm 
		HCGen,	 // Number of Generations for Hill Climbing Initial Population
		nStages,
		BinsMT,
		IterationsSecant;

	double TrainPer,	// Percentage of profile for misfiot calculation
			 pcross,		//x-over probability	 
			 pmut,		// Mutation probability
			 *xL,			// Lower Limit of Parameter Space
			 *xU;			// Upper Limit of Parameter Space
	
	int *stages;
};

#endif // !defined(AFX_OPTIMIZATION_H__F13954EC_4844_4731_B405_22ED766C20D7__INCLUDED_)
