#pragma once
#include "ProgressBar.h"
#include "Opt_Output.h"

class PSOParallel
{
public:
	Model Mod;
	SqzOptimisation SqzOpt;

	string	outputFile,
			pathName;

	CString pathReadFile;

	ProgressBar ProBar;

	vector<Particle> P;

	vector<data_sqz> pso_dat;

	PSOParallel();
	~PSOParallel();
	void SetRun(string, int nSim, Model &M);				// nSim = 1 Optimisation, nSim = 2 Isotherm Matching, nSim = 3 Sensitivity Calculation, nSim = 4 Secant Optimization
	void SetRunMOPSO(string, string);
	void RunSim(const char *NameFile);
	void RunParallelPSO(const char *input, int nSim);	// nSim = 1 Optimisation, nSim = 2 Isotherm Matching, nSim = 3 Sensitivity Calculation, nSim = 4 Secant Optimization
	void RunParallelSensitivityCalc(const char *input);
	void RunParallelSecantOpt_Vahid(const char *input);
	void RunParallelMOPSO(const char *input);
	double randval(double low, double high);
	void ParetoFront(int nP, int nData);
	void CreateDlg(CWnd* pParent);
	bool IsDominant(const Particle &P1, const Particle &P2, int);
	void EquateParticle(Particle &P1, const Particle &P2, int, int);
	void PrintResults(FILE *, const vector<Particle> &P, string st, int nG, int nR, Model &Mod);
	void ParetoFronts();
	void EquateSqzOpt(SqzOptimisation &P1, const SqzOptimisation &P2);
	void GnuPlotFile(int, int, int, int, int, int);
	
};

