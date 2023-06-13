#if !defined( __RESERVOIR_DIMENSIONS_H )
#define __RESERVOIR_DIMENSIONS_H

#include "StdAfx.h"

class ReservoirDimensions
{
	
public:
	ReservoirDimensions();				// constructor
	ReservoirDimensions(int);				// constructor
	~ReservoirDimensions();				// destructor
	ReservoirDimensions(const ReservoirDimensions&);	// copy constructor
	ReservoirDimensions& operator=(const ReservoirDimensions&);	// assignment operator
	void Equate(const ReservoirDimensions&);	// function common to copy const. & op=

	void		Add(const double&);	// add a device to the collection
	void		Remove(int);			// remove a device by index
	void		RemoveAll();			// remove all devices
	void		Sort();					// currently empty unused function
	int			GetnBlocks() { return nBlocks; }
	void	AssignDeltaRsEquallySpaced();	// self-explanatory

	int RadiusOrBlocks;	// block sizes: calculated or input?

	double radiusWell; // Radius of the Well
	double radiusMax; // Maximum Radius
	double MUw; // Initial Water Viscosity
	double MUo; // Oil Viscosity

private:
	void Allocate();
	void Deallocate();
	void InitReservoirDimensions();

	int nBlocks;		// Number of Blocks

};


#endif