// CTreatment class definitions


#include "StdAfx.h"

#include "Treatment.h"

// default constructor
Treatment::Treatment()
{
	InitTreatment();
}


// copy constructor
Treatment::Treatment(const Treatment& rhs)
{
	InitTreatment();
	Equate(rhs);
}

// function common to copy constructor and operator=
void Treatment::Equate(const Treatment& rhs)
{
	RFRev = rhs.RFRev;
	maxCw = rhs.maxCw;
	maxCo = rhs.maxCo;
	threshold = rhs.threshold;
}

// assignment overload
Treatment& Treatment::operator=(const Treatment& rhs)
{

	if (this == &rhs) return *this;
	InitTreatment();
	Equate(rhs);

	return *this;
}

// destructor
Treatment::~Treatment()
{

}

// object initialisation
void Treatment::InitTreatment()
{
	RFRev = 0;
	maxCw = 1;
	maxCo = 1;
	threshold = 1;

	return;
}


