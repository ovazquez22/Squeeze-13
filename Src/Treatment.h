#if !defined( __TREATMENT_JMC_H )
#define __TREATMENT_JMC_H

#include "StdAfx.h"

class Treatment
{
	
public:
	Treatment();				// constructor
	~Treatment();				// destructor
	Treatment(const Treatment&);	// copy constructor
	Treatment& operator=(const Treatment&);	// assignment operator
	void Equate(const Treatment&);	// function common to copy const. & op=

	int RFKind; // How to apply RF: Var=0, Threshold=1
	int RFRev; // RF or Adsorption : Reversible=0, Irreversible=1

	double maxCw; 
	double maxCo; 
	double threshold; // Threshold when RFKind = 1


private:
	void InitTreatment();

};


#endif