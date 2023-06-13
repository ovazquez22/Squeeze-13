// ReservoirDimensions class definitions


#include "StdAfx.h"

#include <math.h>
#include "ReservoirDimensions.h"

// default constructor
ReservoirDimensions::ReservoirDimensions()
{
	nBlocks = 10;
	Allocate();
	InitReservoirDimensions();

}

// constructor, n sets the number of elements
ReservoirDimensions::ReservoirDimensions(int n)
{
	nBlocks = n;
	Allocate();
	InitReservoirDimensions();

}

// copy constructor
ReservoirDimensions::ReservoirDimensions(const ReservoirDimensions& rhs)
{

	nBlocks = rhs.nBlocks;
	Allocate();
	InitReservoirDimensions();
	Equate(rhs);

}

// function common to copy constructor and operator=
void ReservoirDimensions::Equate(const ReservoirDimensions& rhs)
{

	RadiusOrBlocks = rhs.RadiusOrBlocks;

	radiusWell = rhs.radiusWell;
	radiusMax = rhs.radiusMax;

/*	for (int i = 0; i < rhs.nBlocks; i++)
	{
		DeltaRs[i] = rhs.DeltaRs[i];
	}
*/
	MUw = rhs.MUw;
	MUo = rhs.MUo;

}

// assignment overload
ReservoirDimensions& ReservoirDimensions::operator=(const ReservoirDimensions& rhs)
{

	if (this == &rhs) return *this;
	Deallocate();
	nBlocks = rhs.nBlocks;
	Allocate();
	InitReservoirDimensions();
	Equate(rhs);

	return *this;
}

// destructor
ReservoirDimensions::~ReservoirDimensions()
{

	Deallocate();

}

// resource (memory) allocation
void ReservoirDimensions::Allocate()
{
	// any memory assigned herein with new must be unassigned
	// in Deallocate() with delete[]
	
//	DeltaRs = new double[nBlocks];
	


	return;
}

// resource deallocation
void ReservoirDimensions::Deallocate()
{
	return;
}

// object initialisation
void ReservoirDimensions::InitReservoirDimensions()
{
/*	for (int i = 0; i < nBlocks; i++)
	{
		DeltaRs[i] = 0;
	}
*/
	RadiusOrBlocks = 0;
	
	radiusWell = 1;
	radiusMax = 100;
	AssignDeltaRsEquallySpaced();
	MUw = 1;
	MUo = 1;

	return;
}


void ReservoirDimensions::Sort()
{
	// for possible future expansion
	return;
}

void ReservoirDimensions::Add(const double& newElement)
{
	// add a element to the collection...
	ReservoirDimensions temp(nBlocks);	// create temp collection
	temp = *this;
	Deallocate();		// delete resources
	nBlocks++;			// add 1 to the number of DeltaRs
	Allocate();			// create required resources
	nBlocks--;			// fool Equate into copying old info
	Equate(temp);
	nBlocks++;			// reset correct number
//	DeltaRs[nBlocks-1] = newElement;  // add the element to the end

}

void ReservoirDimensions::Remove(int index)
{
	
	ReservoirDimensions temp(nBlocks);	// create temp collection
	temp = *this;
	Deallocate();		// delete resources
	nBlocks--;			// remove 1 from the number of DeltaRs
	Allocate();			// create required resources
	// equate each element in the collection bar the removed one
/*	int j;
	for (int i = 0; i < nBlocks; i++)
	{
		j = (i < index) ? i : i+1;
		DeltaRs[i] = temp.DeltaRs[j];
	}
*/
	return;
}

void ReservoirDimensions::RemoveAll()
{
	// remove all DeltaRs from collection
	ReservoirDimensions temp;
	*this = temp;
	return;
}

void ReservoirDimensions::AssignDeltaRsEquallySpaced()
{
	if (nBlocks > 1)
	{
		for (int b = 0; b < nBlocks; b++)
		{
//			DeltaRs[b] = radiusWell + double(b)*(radiusMax - radiusWell)/double(nBlocks-1);
		}
	}
}


