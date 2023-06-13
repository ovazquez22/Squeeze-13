#if !defined( __SCHEDULE_JMC_H )
#define __SCHEDULE_JMC_H

#include "StdAfx.h"


class Schedule
{
	
public:
	Schedule();				// constructor
	Schedule(int l, int co, int cw);				// constructor
	~Schedule();				// destructor
	Schedule(const Schedule&);	// copy constructor
	Schedule& operator=(const Schedule&);	// assignment operator
	void Equate(const Schedule&);	// function common to copy const. & op=
	void AllocateArrays(int l, int co, int cw);

	int			GetnCompWater() { return nCompWater; }
	int			GetnCompOil() { return nCompOil; }
	int			GetnLayers() { return nLayers; }

	int TypeStage, // = 1 injection/preflush,
						// = 2 overflush,
						// = 3 back production,	
						// = 5 shut-in.
		 nChage,    // Indicator of whether nOpen, nMethod, nLayOp and PIDf change in the new stage:
						//	= 0: no
						// = 1: yes (this may be true even when nOpen & nMethod unchanged).
		 nOpen,		// Number of open layers at a specific stage (nOpen < nLay; if all open, nOpen = 0).
		 nMethod,	   // Specification of which method is to be used in calculating the layer flowrates:
						// = 0: calculating in terms of EffPerm,
						// = 1: calculating in terms of PIDf.
						// = 2: calculating in terms of EffPerm and KH product.
		 BarPsi,		// Layer pressure units = 0 Bar, = 1 PSI 

		 * nLayOp,   // Indices of open layers.
		  nLayers,		// number of layers

		  objTime,			// = 0: no; = 1 yes, included in optimisation 
		  objIndexTime,		// Index for lower and upper for objectives values
		  objSI,			// = 0: no; = 1 yes, included in optimisation 
		  objIndexSI;		// Index for lower and upper for objectives values


	double * InitialCw,		// per water components
			 * InitialCo,	// per oil components
			 * ProdFw,		// per layer
			 * PIDF,		// per layer
			 * LayPres,		// per layer
//			 * NormPIDF,	// per layer

			 Qwt, 
		    Qot, 
			 watFrac,	   // water Fraction at injection stages
	       TimeStep, // Time Step
	       Time,
			 LinDisp, // Dispersion for the linear coreflood
			 TI; // Injected fluid temperature at the well bottomhole during specific stage (°C), it is
				  // meaningful only if kTemp = 1.

private:
	void Allocate();
	void Deallocate();
	void InitSchedule();

	int nCompWater;		// number of water components
	int nCompOil;		// number of oil components
	

};


#endif