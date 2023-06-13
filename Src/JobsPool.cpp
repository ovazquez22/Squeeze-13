#include "stdafx.h"

#include <iostream>
#include <fstream>
#include <cstdio>
#include <exception>

//------------------------------------------------------------------------------------------
bool FileExists(string fn)
{
	ifstream F(fn.c_str());
	bool res = false;
	if (!F.fail())
		res = true;

	F.close();
	return res;
}
//------------------------------------------------------------------------------------------
void JobsPool::SingleJob(int l)
{	
	run[l]->P.Obj = 0;

	try
	{	
		run[l]->P.Obj = run[l]->mod.RunSQV_SP(run[l]->P);

	//	run[l]->P.watConc = run[l]->mod.IsoC;
	}
	catch (...)
	{
		E = current_exception();	// "save" current exception and leave the thread
	}
}
//------------------------------------------------------------------------------------------
JobsPool::JobsPool(vector<OneRun*> d)
{
	NT = 0;		// set "task counter" to 0

	run = d;
	numRuns = run.size();
}
//------------------------------------------------------------------------------------------
// each thread will take the task within the critical section
int JobsPool::TakeTask()
{
	int res;
	MTX.lock();			// start of critical section
		if (NT >= numRuns || E)  // is exception E is not nullptr, new task is not taken
			res = -1;
		else
		{
			res = NT;
			NT++;
		}
	MTX.unlock();		// end of critical section

	return res;
}
//------------------------------------------------------------------------------------------
void JobsPool::Run()
{
	int t = TakeTask();
	while (t != -1)
	{
		SingleJob(t);	// run the job for layer "t". As it will write data only to data[t], and each thread
						// will have different values of "t", no locking of objects is needed here,
						// and NO RACING CONDITION should emerge
		t = TakeTask();
	}
}
//------------------------------------------------------------------------------------------
void JobsPool::RunIsoDer()
{
	int t = TakeTask();
	while (t != -1)
	{
		SingleIsoJob(t);	// run the job for layer "t". As it will write data only to data[t], and each thread
		// will have different values of "t", no locking of objects is needed here,
		// and NO RACING CONDITION should emerge
		t = TakeTask();
	}
}
//------------------------------------------------------------------------------------------
void JobsPool::SingleIsoJob(int l)
{
	try
	{	
		run[l]->P.Obj = run[l]->mod.RunSQV_SP(run[l]->P);

		run[l]->P.watConc = run[l]->mod.IsoC;
	}
	catch (...)
	{
		E = current_exception();	// "save" current exception and leave the thread
	}
}
//------------------------------------------------------------------------------------------
double JobsPool::GetObj(int i)
{
	return run[i]->P.Obj;
};
//------------------------------------------------------------------------------------------
vector<double> JobsPool::GetTimeVol(int i)
{	
	vector<double> temp;
	for (int j = 0; j < run[i]->conc.size(); j++)
		temp.push_back(run[i]->conc[j].time);
	
	return temp;
};
//------------------------------------------------------------------------------------------
vector<double> JobsPool::GetConc(int i)
{
	vector<double> temp;
	for (int j = 0; j < run[i]->conc.size(); j++)
		temp.push_back(run[i]->conc[j].TotSI);

	return temp;
};
//------------------------------------------------------------------------------------------
vector<TotConcOut> JobsPool::WriteOutput(const char *fname)
{
//	return OneLayerData::WriteTotalConc(fname, data);
	vector<TotConcOut> RES(1);
	return RES;
}
//------------------------------------------------------------------------------------------