#include "stdafx.h"
#include <vector>
#include <mutex>
#include <exception>

using namespace std;
//------------------------------------------------------------------------------------------
// this structure contains the output data from one layer
// only create instances of OneLayerData on HEAP (i.e. using "new"), but NOT ON STACK!
struct OneLayerData;
struct OneRun;
struct TotConcOut;
//------------------------------------------------------------------------------------------
class JobsPool
{
protected:
	mutex MTX;				// mutex object for critical section
	int NT;					// variable for tracking (counting) the tasks; task = "layer"
	int numRuns;			// total number of runs
	void SingleJob(int l);			// the user function. Runs a single job for layer "l", 0 <= l < numLayers
									// this function should ONLY write to data[l], but can read any element of "data"
	vector<OneRun*> run;
	
	void SingleIsoJob(int l);	// Single run for isotherm derivation

public:
	string finput;		// the "prefix" part of the file names
	string fconc;
	string fdist;	
	string suff;		// the "suffix" part of the file names

	exception_ptr E;	// for handling exceptions thrown by threads

	JobsPool(vector<OneRun*> d);		// array should have length = number of layers
	int TakeTask();		// this function will take sequentially tasks from 0 to numLayers-1
						// it returns -1 if tasks are finished
	void Run();			// run the whole thing. This function should be called in parallel by many threads
	void RunIsoDer();	// run the whole thing. This function should be called in parallel by many threads
	void SetNT(){ NT = 0; };
	double GetObj(int i);
	vector<double> GetTimeVol(int i);
	vector<double> GetConc(int i);
	vector<TotConcOut> WriteOutput(const char *fname);	// writes the final output file based on "data",
														// see OneLayerData::WriteTotalConc
	   
};
//------------------------------------------------------------------------------------------
bool FileExists(string fn);
//------------------------------------------------------------------------------------------