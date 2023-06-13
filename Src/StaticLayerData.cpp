// StaticLayerData class definitions


#include "StdAfx.h"

#include <math.h>
#include "StaticLayerData.h"

// default constructor
StaticLayerData::StaticLayerData()
{
	InitStaticLayerData();

}


// copy constructor
StaticLayerData::StaticLayerData(const StaticLayerData& rhs)
{

	Equate(rhs);

}

// function common to copy constructor and operator=
void StaticLayerData::Equate(const StaticLayerData& rhs)
{

	// equate each element in the collection
	Phi = rhs.Phi;
	KAbs = rhs.KAbs;
	HLay = rhs.HLay;

}

// assignment overload
StaticLayerData& StaticLayerData::operator=(const StaticLayerData& rhs)
{

	if (this == &rhs) return *this;
	Equate(rhs);

	return *this;
}

// destructor
StaticLayerData::~StaticLayerData()
{

}


// object initialisation
void StaticLayerData::InitStaticLayerData()
{
	Phi = 0.2;
	KAbs = 1;
	HLay = 2;
	return;
}


