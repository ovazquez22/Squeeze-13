// LayerProperties class definitions


#include "StdAfx.h"

#include <math.h>
#include "LayerProperties.h"

// default constructor
LayerProperties::LayerProperties()
{
	Allocate();
	InitLayerProperties();

}


// copy constructor
LayerProperties::LayerProperties(const LayerProperties& rhs)
{
	Allocate();
	InitLayerProperties();
	Equate(rhs);
}

// function common to copy constructor and operator=
void LayerProperties::Equate(const LayerProperties& rhs)
{
	// equate each element in the collection
/*	for (int i = 0; i < nWaterComponents; i++) {
		waterAdsIso[i] = rhs.waterAdsIso[i];
	}
	for (i = 0; i < nOilComponents; i++) {
		oilAdsIso[i] = rhs.oilAdsIso[i];
	}
*/	dynamicLayer = rhs.dynamicLayer;
	staticLayer = rhs.staticLayer;

}

// assignment overload
LayerProperties& LayerProperties::operator=(const LayerProperties& rhs)
{

	if (this == &rhs) return *this;
	Deallocate();
	Allocate();
	InitLayerProperties();
	Equate(rhs);

	return *this;
}

// destructor
LayerProperties::~LayerProperties()
{
	Deallocate();
}

// resource (memory) allocation
void LayerProperties::Allocate()
{
	// any memory assigned herein with new must be unassigned
	// in Deallocate() with delete[]

	return;
}

// resource deallocation
void LayerProperties::Deallocate()
{

	return;
}

// object initialisation
void LayerProperties::InitLayerProperties()
{
	// for possible future expansion
	return;
}


void LayerProperties::Sort()
{
	// for possible future expansion
	return;
}

