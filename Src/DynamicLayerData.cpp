// DynamicLayerData class definitions


#include "StdAfx.h"
#include <math.h>
#include "DynamicLayerData.h"

// default constructor
DynamicLayerData::DynamicLayerData()
{
	InitDynamicLayerData();

}

// copy constructor
DynamicLayerData::DynamicLayerData(const DynamicLayerData& rhs)
{

	Equate(rhs);

}

// function common to copy constructor and operator=
void DynamicLayerData::Equate(const DynamicLayerData& rhs)
{
	// equate each element in the collection
	A = rhs.A;
	Alpha = rhs.Alpha;
	B = rhs.B;
	Beta = rhs.Beta;
	Swi = rhs.Swi;
	Srw = rhs.Srw;
	Sro = rhs.Sro;
	NewSwi = rhs.NewSwi;
	NewSor = rhs.NewSor;
	
	Type = rhs.Type;
	
	RelPermTabFile = rhs.RelPermTabFile;

	nRP = rhs.nRP;

	for ( int i = 0 ; i < nRP ; i++ )
	{
		RpSw[i] = rhs.RpSw[i];
		RpKrw[i] = rhs.RpKrw[i];
		RpKro[i] = rhs.RpKro[i];
	}
}

// assignment overload
DynamicLayerData& DynamicLayerData::operator=(const DynamicLayerData& rhs)
{
	if (this == &rhs) return *this;
	Equate(rhs);
	return *this;
}

// destructor
DynamicLayerData::~DynamicLayerData()
{
	delete [] RpSw;
	delete [] RpKrw;
	delete [] RpKro;
}


// object initialisation
void DynamicLayerData::InitDynamicLayerData()
{	
	Type = 0;

	A = 1;
	Alpha = 1;
	B = 1;
	Beta = 1.0;
	Swi = 1.0;
	Srw = 0.0;
	Sro = 0.0;
	NewSwi = 0.0;
	NewSor = 0.0;
	
	RelPermTabFile = "RelPerm.txt";

	RpSw = new double [MaxnRP];
	RpKrw = new double [MaxnRP];
	RpKro = new double [MaxnRP];

	return;
}


int DynamicLayerData::BinarySearch( double newSw )
{
	int low = 0, 
		 high = nRP,
		 middle;
	
	while( (high - low) > 1  )
	{	
		middle = ( low + high )  / 2;
		
		if ( newSw == RpSw[middle] )
			
			return middle;

		else if ( newSw < RpSw[middle] )	

			high = middle;

		else

			low = middle;
		
	}
	
	return low;
}
