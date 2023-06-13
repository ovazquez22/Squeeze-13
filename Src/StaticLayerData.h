#if !defined( __STATIC_LAYER_DATA_H )
#define __STATIC_LAYER_DATA_H

#include "StdAfx.h"

class StaticLayerData
{
	
public:
	StaticLayerData();				// constructor
	~StaticLayerData();				// destructor
	StaticLayerData(const StaticLayerData&);	// copy constructor
	StaticLayerData& operator=(const StaticLayerData&);	// assignment operator



	double Phi; // Porosity
	double KAbs; // Absolute Permeability
	double HLay; // Height of Layer

private:
	void InitStaticLayerData();
	void Equate(const StaticLayerData&);	// function common to copy const. & op=


};


#endif