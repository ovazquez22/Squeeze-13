// Schedule class definitions


#include "StdAfx.h"

#include "Schedule.h"

// default constructor
Schedule::Schedule()
{
	nCompWater = 1;
	nCompOil = 1;
	nLayers = 1;
	Allocate();
	InitSchedule();

}

// constructor, n sets the number of elements
Schedule::Schedule(int l, int co, int cw)
{
	nLayers = l;
	nCompWater = cw;
	nCompOil = co;
	Allocate();
	InitSchedule();

}

// copy constructor
Schedule::Schedule(const Schedule& rhs)
{

	nCompWater = rhs.nCompWater;
	nCompOil = rhs.nCompOil;
	nLayers = rhs.nLayers;
	Allocate();
	InitSchedule();
	Equate(rhs);

}

// function common to copy constructor and operator=
void Schedule::Equate(const Schedule& rhs)
{

	// equate each element in the collection
	for (int cw = 0; cw < __min(nCompWater,rhs.nCompWater); cw++)
	{
		InitialCw[cw] = rhs.InitialCw[cw];
	}
	
	for (int co = 0; co < __min(nCompWater,rhs.nCompWater); co++)
	{
		InitialCo[co] = rhs.InitialCo[co];
	}
	
	for (int l = 0; l < __min(nLayers,rhs.nLayers); l++) 
	{
		ProdFw[l] = rhs.ProdFw[l];
		PIDF[l] = rhs.PIDF[l];
		LayPres[l] = rhs.LayPres[l];
		nLayOp[l] = rhs.nLayOp[l];
//		NormPIDF[l] = rhs.NormPIDF[l];
	}

	TypeStage = rhs.TypeStage; // (1 INJ, 2 PRO)
	
	watFrac = rhs.watFrac;
	Qwt = rhs.Qwt;
	Qot = rhs.Qot;
	TimeStep = rhs.TimeStep; // Time Step
	Time = rhs.Time;
	TI = rhs.TI;
	nMethod = rhs.nMethod;
	nOpen = rhs.nOpen;
	nChage = rhs.nChage;
	LinDisp = rhs.LinDisp;
	BarPsi = rhs.BarPsi;

	objTime = rhs.objTime;
	objIndexTime = rhs.objIndexTime;
	objSI = rhs.objSI;
	objIndexSI = rhs.objIndexSI;
}

// assignment overload
Schedule& Schedule::operator=(const Schedule& rhs)
{

	if (this == &rhs) return *this;
	Deallocate();
	nCompWater = rhs.nCompWater;
	nCompOil = rhs.nCompOil;
	nLayers = rhs.nLayers;
	Allocate();
	InitSchedule();
	Equate(rhs);

	return *this;
}

// destructor
Schedule::~Schedule()
{

	Deallocate();

}

// resource (memory) allocation
void Schedule::Allocate()
{
	// any memory assigned herein with new must be unassigned
	// in Deallocate() with delete[]
	
	InitialCw = new double[nCompWater];
	InitialCo = new double[nCompOil];
	ProdFw = new double[nLayers];
	nLayOp = new int[nLayers];
	PIDF = new double[nLayers];
	LayPres = new double[nLayers];
//	NormPIDF = new double[nLayers];

	return;
}

// resource deallocation
void Schedule::Deallocate()
{	
//	delete[] NormPIDF;
	delete[] InitialCw;
	delete[] InitialCo;
	delete[] ProdFw;
	delete[] nLayOp;
	delete[] PIDF;
	delete[] LayPres;
	
	return;
}

// object initialisation
void Schedule::InitSchedule()
{

	for (int cw = 0; cw < nCompWater; cw++)
	{
		InitialCw[cw] = 0;
	}
	for (int co = 0; co < nCompOil; co++)
	{
		InitialCo[co] = 0;
	}
	for (int l = 0; l < nLayers; l++)
	{
		ProdFw[l] = 1.0;
		PIDF[l] = 0;
		LayPres[l] = 0;
		nLayOp[l] = 0;
	}

	TypeStage = INJECTION; // (1 INJ, 2 PRO)
	
	watFrac = 1;
	Qwt = 1;
	Qot = 0;
	TimeStep = 0.01; // Time Step
	Time = 1;
	TI = 20;
	LinDisp = 0;

	nChage = 1;
	nOpen = 0;
	nMethod = 0;
	BarPsi = 0;

	objIndexTime = objIndexSI = -1;
	objTime = objSI = 0;

	return;
}

void Schedule::AllocateArrays(int l, int co, int cw)
{	
	Deallocate();
	nLayers = l;
	nCompWater = cw;
	nCompOil = co;
	Allocate();
	InitSchedule();

}
