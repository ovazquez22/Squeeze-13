// RFLayer.cpp: implementation of the RFLayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "RFLayer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// default constructor
RFLayer::RFLayer()
{
	InitRFLayer();

}


// copy constructor
RFLayer::RFLayer(const RFLayer& rhs)
{

	InitRFLayer();
	Equate(rhs);

}

// function common to copy constructor and operator=
void RFLayer::Equate(const RFLayer& rhs)
{

	// equate each element in the collection
	 
	 RFwMin = rhs.RFwMin;
	 RFwMax = rhs.RFwMax;
	 nWat = rhs.nWat;
	 
	 RFoMin = rhs.RFoMin;
	 RFoMax = rhs.RFoMax;
	 nOil = rhs.nOil;

}

// assignment overload
RFLayer& RFLayer::operator=(const RFLayer& rhs)
{

	if (this == &rhs) return *this;
	InitRFLayer();
	Equate(rhs);

	return *this;
}

// destructor
RFLayer::~RFLayer()
{

}

// object initialisation
void RFLayer::InitRFLayer()
{ 
	 RFwMin = 1;
	 RFwMax = 1;
	 nWat = 1;
	 
	 RFoMin = 1;
	 RFoMax = 1;
	 nOil = 1;

	return;
}
