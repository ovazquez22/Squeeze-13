#include "stdafx.h"
#include "PSOParallel.h"


PSOParallel::PSOParallel()
{
}


PSOParallel::~PSOParallel()
{
}

void PSOParallel::CreateDlg(CWnd* pParent)
{
	ProBar.Create(IDD_PROGRESS_BAR, pParent);

	ProBar.SetWindowText("Automatic Optimisation and Pareto Front");
}
//----------------------------------------------------------------------------------------
void PSOParallel::SetRun(string out, int nSim, Model& M)
{
	try
	{
		string input = "inputOpt.dat";

		outputFile = out;
		Mod = M;

		if (nSim == 3) // Sensitivity Study

			RunParallelSensitivityCalc(input.c_str());

		if (nSim == 1) // Global Optimisation

			RunParallelPSO(input.c_str(), nSim);

	}
	catch (Exception& e)
	{
		cout << e.what();
	}

}


//----------------------------------------------------------------------------------------
void PSOParallel::RunSim(const char* NameFile)
{
	char st[2000];

	strcpy_s(st, "HMI_Fortran_Release.exe ");
	strcat_s(st, NameFile);

	strcat_s(st, " test.out");
	strcat_s(st, " conc.out");
	strcat_s(st, " dist.out");

	system(st);
}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void PSOParallel::RunParallelPSO(const char* NameFile, int nSim)
{
	exception_ptr E;	// for handling exceptions thrown by threads

	int i, j;

	FILE* filePtrRes,
		* filePtrAll;

	double BasicCost = 0;

	bool checkIso = true;

	errno_t err;

	string	input = NameFile,
		input_short;

	i = input.find_last_of(".");
	if (i < string::npos)
		input_short = input.substr(0, i);
	else
		input_short = input;

	// This file will store all the results from PSO 
	if (nSim == 2) // Isotherm Matching
	{
		if ((err = fopen_s(&filePtrRes, outputFile.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", outputFile);
		else
			printf("File %s was opened for writing\r\n", outputFile);

		size_t start_pos = outputFile.find("Results.txt");
		outputFile.replace(start_pos, outputFile.length(), "All.txt");

		// This file will store all the results from PSO 
		if ((err = fopen_s(&filePtrAll, outputFile.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", outputFile);
		else
			printf("File %s was opened for writing\r\n", outputFile);
	}

	if (nSim == 1) // Design Optimisation
	{
		size_t start_pos = outputFile.find("Results.txt");
		outputFile.replace(start_pos, outputFile.length(), "All.txt");

		// This file will store all the results from PSO 
		if ((err = fopen_s(&filePtrAll, outputFile.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", outputFile);
		else
			printf("File %s was opened for writing\r\n", outputFile);
	}

	// Random Seed
	srand((unsigned)time(NULL));
	//	srand(548);

	// define the number of processes (threads);
	// this number can be modified by USER (to be implemented)
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;

	cout << "Number of processors: " << nproc << "\r\n";	// TEST; DELETE if necessary

	if (nproc < 1)
		throw Exception("Number of threads should be greater than zero");

	Mod.suff = "_run_";

	if (nSim == 1) // Design Optimisation
	{
		fprintf_s(filePtrAll, "%4s %4s  ", "#Gen", "nPar");

		for (i = 0; i < 3 * Mod.SqzOpt.nPar; i++)
		{
			const size_t buffsize = 10;
			char ind[buffsize];

			string s;

			if (i < Mod.SqzOpt.nPar)
			{
				sprintf_s(ind, buffsize, "%d ", i);
				string tind(ind);
				s = "X" + tind;
				fprintf_s(filePtrAll, "%11s", s.c_str());
			}

			if (i >= Mod.SqzOpt.nPar && i < 2 * Mod.SqzOpt.nPar)
			{
				string st = "SI";
				string temp;
				temp = Mod.SqzOpt.Label[i - Mod.SqzOpt.nPar];

				if (temp.find(st) == -1)
				{
					st = "Inj";
					int n = temp.find(string(st));
					if (n > -1)
						temp.replace(3, 3, string("Tim"));
					else
						temp.replace(2, 3, string("Tim"));
				}

				fprintf_s(filePtrAll, "%11s", temp.c_str());
			}

			if (i >= 2 * Mod.SqzOpt.nPar && i < 3 * Mod.SqzOpt.nPar)
			{
				string st = "SI";
				if (Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].find(st) == -1)
				{
					fprintf_s(filePtrAll, "%11s", Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].c_str());
				}
			}
		}

		CString st1 = "TotInjVol(bbl)",
				st2 = "TotSI(bbl)";

		if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M')
		{
			st1 = "TotInjVol(m3)";
			st2 = "TotSI(m3)";
		}

		fprintf_s(filePtrAll, "%16s %16s %11s %11s %11s %11s %11s %11s %11s\n", "TotInjTime(hr)", st1, st2, "TotSICost", "TotInjCost", "TotCost", "%OptObj", "RealObj", "Tar-Obj");
	}

	if (nSim == 2) // Isotherm Matching
	{
		checkIso = false;

		//		fprintf_s(filePtrRes, "%4s%4s", "\t", "\t");
		for (i = 0; i < Mod.SqzOpt.nPar; i++)
			fprintf_s(filePtrRes, "%11s", "\t");
		fprintf_s(filePtrRes, "%11s   %11s", "\t", "Time/Vol");
		for (i = 0; i < Mod.nPts; i++)
			fprintf_s(filePtrRes, " %11.4f ", Mod.fX[i]);
		fprintf_s(filePtrRes, " \n");

		//		fprintf_s(filePtrRes, "%4s %4s ", "#Gen", "nPar");
		/*	for (i = 0; i < Mod.SqzOpt.nPar; i++)
		{
		const size_t buffsize = 10;
		char ind[buffsize];
		sprintf_s(ind, buffsize, "%d ", i);
		string tind(ind);
		string s;
		s = "X" + tind;
		fprintf_s(filePtrRes, "%11s", s.c_str());
		}*/
		if (Mod.Iso.IsoTyp == 1)
		{
			fprintf_s(filePtrRes, "%11s", "k");
			fprintf_s(filePtrRes, "%11s", "n");
		}

		if (Mod.Iso.IsoTyp == 0)
		{
			fprintf_s(filePtrRes, "%11s", "a");
			fprintf_s(filePtrRes, "%11s", "b");
		}

		if (Mod.SqzOpt.nPar > 2)
			fprintf_s(filePtrRes, "%11s", "r2");

		fprintf_s(filePtrRes, "%11s ", "OptObj");
		fprintf_s(filePtrRes, "%11s", "Prof");
		for (i = 0; i < Mod.nPts; i++)
			fprintf_s(filePtrRes, " %11.4f ", Mod.fY[i]);
		fprintf_s(filePtrRes, "\n");

		fprintf_s(filePtrAll, "%4s%4s", "\t", "\t");
		for (i = 0; i < Mod.SqzOpt.nPar; i++)
			fprintf_s(filePtrAll, "%11s", "");
		fprintf_s(filePtrAll, "%11s   %11s", "", "Time/Vol");
		for (i = 0; i < Mod.nPts; i++)
			fprintf_s(filePtrAll, " %11.4f ", Mod.fX[i]);
		fprintf_s(filePtrAll, "\n");

		fprintf_s(filePtrAll, "%4s %4s ", "#Gen", "nPar");
		/*		for (i = 0; i < Mod.SqzOpt.nPar; i++)
		{
		const size_t buffsize = 10;
		char ind[buffsize];
		sprintf_s(ind, buffsize, "%d ", i);
		string tind(ind);
		string s;
		s = "X" + tind;
		fprintf_s(filePtrAll, "%11s", s.c_str());
		}
		*/
		if (Mod.Iso.IsoTyp == 1)
		{
			fprintf_s(filePtrAll, "%11s", "k");
			fprintf_s(filePtrAll, "%11s", "n");
		}

		if (Mod.Iso.IsoTyp == 0)
		{
			fprintf_s(filePtrAll, "%11s", "a");
			fprintf_s(filePtrAll, "%11s", "b");
		}

		if (Mod.SqzOpt.nPar > 2)
			fprintf_s(filePtrAll, "%11s", "r2");

		fprintf_s(filePtrAll, "%11s ", "OptObj");
		fprintf_s(filePtrAll, "%11s", "Prof");
		for (i = 0; i < Mod.nPts; i++)
			fprintf_s(filePtrAll, " %11.4f ", Mod.fY[i]);
		fprintf_s(filePtrAll, "\n");
	}

	vector<Particle>	Pg(Mod.SqzOpt.nPopSize);		// Stores the personal best location of each particle;

	Particle	Pi,		// Stores best overall particle 
		Pgen;	// Stores the best per generation

	Pi.X.resize(Mod.SqzOpt.nPar);
	Pgen.X.resize(Mod.SqzOpt.nPar);

	P.resize(Mod.SqzOpt.nPopSize);

	vector<vector<double>> vel;

	double r1, r2;

	try
	{
		int no_of_cols = Mod.SqzOpt.nPar;
		int no_of_rows = Mod.SqzOpt.nPopSize;
		double initial_value = 0.0;
		vel.resize(no_of_rows, vector<double>(no_of_cols, initial_value));


		for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
		{
			P[i].X.resize(Mod.SqzOpt.nPar);

			Pg[i].X.resize(Mod.SqzOpt.nPar);

			for (j = 0; j < Mod.SqzOpt.nPar; j++)
			{
				// Random Initial Population
				P[i].X[j] = (Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * randval(0, 1) + Mod.SqzOpt.lX[j];
			}

			P[i].Obj = 1e20;
			Pg[i].Obj = 1e20;
		}

	}
	catch (Exception& e)
	{
		cout << e.what() << endl;
	}

	Pi.Obj = -100;

	// Randomized Particle Velocities
	for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
	{
		for (j = 0; j < Mod.SqzOpt.nPar; j++)
		{
			vel[i][j] = 0;

			if (randval(0, 1) > 0.5)

				vel[i][j] = randval(0, 0.5) * P[i].X[j];

			else

				vel[i][j] = -randval(0, 0.5) * P[i].X[j];
		}
	}

	vector<OneRun*> OLD(Mod.SqzOpt.nPopSize);
	for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
	{
		OLD[i] = new OneRun;		// to be deleted manually!
		OLD[i]->mod = Mod;
	}

	JobsPool JP(OLD);
	JP.finput = input_short;
	JP.fconc = "conc";		// these file names should be provided by USER (to be implemented)
	JP.fdist = "dist";
	JP.suff = "_run_";		// suffix

	vector<thread*> threads(nproc);

	ProBar.ShowWindow(true);

	ProBar.iProBar = 0;
	ProBar.m_ProgressBar.SetPos(0);

	Pi.Obj = 1e20;

	pso_dat.clear();

	// Count the number of entries, if smaller than the first one in the first generation 
	// It is because the run was not valid, it stopped at some stage
	int nCheck = 0;

	// Start with the generations
	for (int n = 0; n < Mod.SqzOpt.nGen; n++)
	{
		try
		{
			// Generate New Population
			for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
			{
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					P[i].X[j] = P[i].X[j] + vel[i][j];
				}

				// DAMPING BOUNDARY CONDITION
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					// The coordinate j is reset to the boundary, then the velocity sign is reversed
					// and multiplied by random between [0,1]
					if (P[i].X[j] >= Mod.SqzOpt.uX[j])

						P[i].X[j] = Mod.SqzOpt.uX[j] - randval(0, 0.2) * vel[i][j];

					if (P[i].X[j] <= Mod.SqzOpt.lX[j])

						P[i].X[j] = Mod.SqzOpt.lX[j] - randval(0, 0.2) * vel[i][j];

				}

				// Damping boundary conditions for SI Vol 
				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.nPar == 3 && Mod.SqzOpt.Only_SI == 1 && Mod.SqzOpt.Only_Tot_Vol == 0 & checkIso)
				{
					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
						SI = P[i].X[1] * Mod.SqzOpt.X[1] / 1e6;

					// Fix concentration to honor max SI Vol Inj 
					if ((MT * SI) >= Mod.TotSIVol)
					{
						SI = Mod.TotSIVol * 1e6 / MT;
						P[i].X[1] = SI / Mod.SqzOpt.X[1];
					}

				}

				// Damping boundary conditions for Tot Inj Vol 
				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.nPar == 3 && Mod.SqzOpt.Only_SI == 0 && Mod.SqzOpt.Only_Tot_Vol == 1)
				{
					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
						OF = P[i].X[2] * Mod.SqzOpt.Vol[2];

					// If MT and OF Volume in bigger than TotInjVol, adjust OF to honor
					if ((MT + OF) >= Mod.TotInjVol)
					{
						OF = Mod.TotInjVol - MT;
						if (OF <= 0)
						{
							OF = MT * 0.01;
							MT = MT - OF;
						}

						P[i].X[0] = MT / Mod.SqzOpt.Vol[0];
						P[i].X[2] = OF / Mod.SqzOpt.Vol[2];
					}
				}

				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.nPar == 3 && Mod.SqzOpt.Only_SI == 1 && Mod.SqzOpt.Only_Tot_Vol == 1) // Operational Sensitivity Calcualtions
				{
					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
							SI,
							OF = P[i].X[2] * Mod.SqzOpt.Vol[2];

					SI = P[i].X[1] * Mod.SqzOpt.X[1] / 1e6; // Weight percentage

					MT = Mod.TotSIVol / SI;
					P[i].X[0] = MT / Mod.SqzOpt.Vol[0];

					OF = Mod.TotInjVol - MT;
					P[i].X[2] = OF / Mod.SqzOpt.Vol[2];
				}

			}

			// Make sure original design is included
			if (n == 0)
			{
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					P[0].X[j] = 1;
				}
			}

			for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
			{
				OLD[i]->P = P[i];
			}

			JP.SetNT();

			for (j = 0; j < nproc; j++)
				threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

			// the threads are running here!

			for (j = 0; j < nproc; j++)
			{
				threads[j]->join();		// wait for all threads to finish
				delete threads[j];		// and delete them
			}

			if (JP.E)
				rethrow_exception(JP.E);	// rethrow exception from the worker thread

			try
			{

				// Assign new Objectives to Particles
				if (nSim == 1) // Squeeze Design Optimisation
				{
					for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
						P[i].Obj = fabs(Mod.SqzOpt.Obj_T_V - JP.GetObj(i)) / Mod.SqzOpt.Obj_T_V;
				}

				if (nSim == 2) // Isotherm Matching
				{
					vector<double>	tempTV,
									tempConc;

					// Count the number of entries, if smaller than the first one in the first generation 
					// It is becuase the run was not valid, it stopped at some stage
					if (n == 0) nCheck = JP.GetTimeVol(0).size();

					for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
					{
						int i1, i2;
						tempTV = JP.GetTimeVol(i);
						tempConc = JP.GetConc(i);

						if (tempTV[1] < 0)
						{
							P[i].Obj = 1e4;
							P[i].TimeVol.clear();
							P[i].watConc.clear();
						}
						else
						{
							if (nCheck == tempTV.size())
							{
								i1 = 0;
								i2 = 0;
								P[i].TimeVol.clear();
								P[i].watConc.clear();

								while (i2 < (tempTV.size() - 1) && i1 < Mod.nPts)
								{
									if (tempTV[i2] <= Mod.fX[i1] && tempTV[i2 + 1] >= Mod.fX[i1])
									{
										P[i].TimeVol.push_back(tempTV[i2]);
										P[i].watConc.push_back(tempConc[i2]);
										i1++;
									}
									else
										i2++;
								}

								// Natural Logarithm
								P[i].Obj = 0;
								for (j = 0; j < P[i].TimeVol.size(); j++)
								{
									double l1, l2;
									if (P[i].watConc[j] > 0) l1 = log10(P[i].watConc[j]);
									else	l1 = 0;

									if (Mod.fY[j] > 0) l2 = log10(Mod.fY[j]);
									else l2 = 0;

									P[i].Obj += fabs(l1 - l2);
								}
							}
							else
							{
								P[i].Obj = 1e4;
								P[i].TimeVol.clear();
								P[i].watConc.clear();
							}
						}
					}
				}

				Pgen.Obj = 1e20;

				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					// Check if Particle i is top 1%
					bool check = false;

					if (P[i].Obj < 0.01) check = true;

					// Set Pgen, best location per generation
					if (Pgen.Obj > P[i].Obj)
					{
						Pgen.watConc = P[i].watConc;
						Pgen.Obj = P[i].Obj;
						for (j = 0; j < Mod.SqzOpt.nPar; j++)
							Pgen.X[j] = P[i].X[j];
					}

					// Set Pg, best location of a particle so far
					if (Pg[i].Obj > P[i].Obj)
					{
						Pg[i].watConc = P[i].watConc;
						Pg[i].Obj = P[i].Obj;
						for (j = 0; j < Mod.SqzOpt.nPar; j++)
							Pg[i].X[j] = P[i].X[j];
					}

					if (nSim == 1) // Design Optimisation
					{
						data_sqz tempdat;
						tempdat.xVal.resize(Mod.SqzOpt.nPar);
						tempdat.dat.resize(Mod.SqzOpt.nPar + 7);

						fprintf_s(filePtrAll, "%-4d %-4d ", n, i);

						double	TotInjTime = 0,
							TotInjVol = 0;

						for (j = 0; j < Mod.SqzOpt.nPar; j++)
						{
							fprintf_s(filePtrAll, "%11.4f", P[i].X[j]);
						}

						for (j = 0; j < Mod.SqzOpt.nPar; j++)
						{
							if (P[i].X[j] * Mod.SqzOpt.X[j] < 1e5)
							{
								fprintf_s(filePtrAll, "%11.4f", P[i].X[j] * Mod.SqzOpt.X[j]);
							}
							else
							{
								fprintf_s(filePtrAll, "%11.2f", P[i].X[j] * Mod.SqzOpt.X[j]);
							}

							tempdat.xVal[j] = P[i].X[j] * Mod.SqzOpt.X[j];
						}

						int j1 = 0;

						for (j = 0; j < Mod.SqzOpt.nPar; j++)
						{
							string st = "SI";

							if (Mod.SqzOpt.Label[j].find(st) == -1)
							{
								TotInjTime += (P[i].X[j] * Mod.SqzOpt.X[j]);
								TotInjVol += (P[i].X[j] * Mod.SqzOpt.Vol[j]);
							//	fprintf_s(filePtrAll, " %11.2f", P[i].X[j] * Mod.SqzOpt.Vol[j]);
							}
							else
							{
								tempdat.dat[j1] = P[i].X[j] * Mod.SqzOpt.X[j];
								j1++;
							}
						}

						int j2 = 0;
						for (j = 0; j < Mod.SqzOpt.nPar; j++)
						{
							string st = "SI";

							if (Mod.SqzOpt.Label[j].find(st) == -1)
							{
								tempdat.dat[j1 + j2] = P[i].X[j] * Mod.SqzOpt.Vol[j];
								j2++;
							}
						}

						double	TotalSI = 0,
								TotalSICost = 0,
								TotalInjCost = 0,
								TotalCost = 0;

						TotInjTime = 0;
						TotInjVol = 0;

						for (j = 0; j < Mod.stg.size(); j++)
						{
							if (Mod.stg[j].TypeStg == 1)
							{
								if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 1)

									TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

								if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 0)

									TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));

								if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 1)

									TotalSICost += (Mod.stg[j].InitialCw[0] * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

								if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 0)

									TotalSICost += (Mod.stg[j].InitialCw[0] * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));
							}

							if (Mod.stg[j].TypeStg != 3)
							{
								if (Mod.stg[j].objTime == 1)
								{
									TotInjTime += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.X[Mod.stg[j].objIndexTime]);

									if (Mod.stg[j].TypeStg != 5)
									{
										TotInjVol += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]);
									}
								}
								else
								{
									TotInjTime += (Mod.stg[j].Time);

									if (Mod.stg[j].TypeStg != 5)
									{
										TotInjVol += (Mod.stg[j].Qwt * Mod.stg[j].Time * 60.0);
									}
								}
							}
						}

						TotalSI = 1e-6 * TotalSICost;
						TotalSICost = 1e-6 * (TotalSICost * SqzOpt.SIBblRate);
						TotalInjCost = TotInjTime * SqzOpt.InjHourRate;
						TotalCost = TotalSICost + TotalInjCost;

						fprintf_s(filePtrAll, " %16.2f %16.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f\n", TotInjTime, TotInjVol, TotalSI, TotalSICost, TotalInjCost, TotalCost, P[i].Obj, JP.GetObj(i), Mod.SqzOpt.Obj_T_V - JP.GetObj(i));
						
						tempdat.dat[Mod.SqzOpt.nPar] = JP.GetObj(i);	// SQZ_LT
						tempdat.dat[Mod.SqzOpt.nPar + 1] = TotalCost;	// TotCost
						tempdat.dat[Mod.SqzOpt.nPar + 2] = TotalInjCost;	// TotInjCost
						tempdat.dat[Mod.SqzOpt.nPar + 3] = TotInjTime;	// TotInjTime
						tempdat.dat[Mod.SqzOpt.nPar + 4] = TotInjVol;	// TotInjVol
						tempdat.dat[Mod.SqzOpt.nPar + 5] = TotalSI;	// TotSI
						tempdat.dat[Mod.SqzOpt.nPar + 6] = TotalSICost;// TotSICost

						pso_dat.push_back(tempdat);
					}

					if (nSim == 2) // Isotherm Matching
					{
						fprintf_s(filePtrAll, "%-4d %-4d ", n, i);

						fprintf_s(filePtrAll, "%11.4f", P[i].X[0] * Mod.Iso.k);
						fprintf_s(filePtrAll, "%11.4f", P[i].X[1] * Mod.Iso.n);
						if (Mod.SqzOpt.nPar > 2)
							fprintf_s(filePtrAll, "%11.4f", P[i].X[2] * Mod.Iso.r2);

						fprintf_s(filePtrAll, "%11.4f %11s", P[i].Obj, "Prof");
						for (j = 0; j < P[i].watConc.size(); j++)
							fprintf_s(filePtrAll, " %11.4f ", P[i].watConc[j]);
						fprintf_s(filePtrAll, "\n");
					}

					// Update Pi, best overall particle
					if (Pi.Obj > Pg[i].Obj)
					{
						Pi.watConc = Pg[i].watConc;
						Pi.Obj = Pg[i].Obj;
						for (j = 0; j < Mod.SqzOpt.nPar; j++)
							Pi.X[j] = Pg[i].X[j];
					}
				}

				// Print Best candidate per generation, Isotherm Fitting
				if (nSim == 2) // Isotherm Matching
				{
					int l,
						nPts;

					nPts = Mod.nPts;

					if (nPts > Pgen.watConc.size()) nPts = Pgen.watConc.size();

					/*	if (n == 0)
					{
					if (Mod.SqzOpt.nPar > 2)
					fprintf(filePtrRes, "%12s %12s %12s %12s %12s %12s %12s %12s %12s", "", "", "", "", "", "", "", "", "SolTime/Vol");
					else
					fprintf(filePtrRes, "%12s %12s %12s %12s %12s %12s %12s %12s","", "", "", "", "", "", "", "SolTime/Vol");

					for (int l = 0; l < nPts; l++)
					fprintf(filePtrRes, " %12.3f", Mod.fX[l]);

					fprintf(filePtrRes, "\n");

					if (Mod.SqzOpt.nPar == 3)
					fprintf(filePtrRes, "%12s %12s %12s %12s %12s %12s %12s %12s %12s", "Generation", "MinTrain", "AveTrain", "MinEval", "AveEval", "k", "n", "r", "ProfTar");

					if (Mod.SqzOpt.nPar == 2)
					fprintf(filePtrRes, "%12s %12s %12s %12s %12s %12s %12s %12s", "Generation", "MinTrain", "AveTrain", "MinEval", "AveEval", "k", "n", "ProfTar");

					for (l = 0; l < nPts; l++)
					fprintf(filePtrRes, " %12.3f", Mod.fY[l]);

					fprintf(filePtrRes, "\n");
					}

					fprintf(filePtrRes, "%12d %12.4f %12.4f %12.4f %12.4f ", n, Pgen.Obj, 0.0, 0.0, 0.0);

					if (Mod.SqzOpt.nPar > 2)
					fprintf(filePtrRes, "%12.4f %12.4f %12.4f %12s", Pgen.X[0] * Mod.Iso.k, Pgen.X[1] * Mod.Iso.n, Pgen.X[2] * Mod.Iso.r2, "Prof");
					else
					fprintf(filePtrRes, "%12.4f %12.4f %12s", Pgen.X[0] * Mod.Iso.k, Pgen.X[1] * Mod.Iso.n, "Prof");

					for (l = 0; l < nPts; l++)
					fprintf(filePtrRes, " %12.3f", Pgen.watConc[l]);

					fprintf(filePtrRes, "\n");*/

					//	fprintf_s(filePtrRes, "%-4d %-4d ", n, i);

					fprintf_s(filePtrRes, "%11.4f", Pgen.X[0] * Mod.Iso.k);
					fprintf_s(filePtrRes, "%11.4f", Pgen.X[1] * Mod.Iso.n);
					if (Mod.SqzOpt.nPar > 2)
						fprintf_s(filePtrRes, "%11.4f", Pgen.X[2] * Mod.Iso.r2);

					fprintf_s(filePtrRes, "%11.4f %11s", Pgen.Obj, "Prof");
					for (j = 0; j < Pgen.watConc.size(); j++)
						fprintf_s(filePtrRes, " %11.4f ", Pgen.watConc[j]);
					fprintf_s(filePtrRes, "\n");

					// Print all the cases
					/*	for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
					{
					fprintf_s(filePtrRes, "%-4d %-4d ", n, i);

					fprintf_s(filePtrRes, "%11.4f", P[i].X[0] * Mod.Iso.k);
					fprintf_s(filePtrRes, "%11.4f", P[i].X[1] * Mod.Iso.n);
					if (Mod.SqzOpt.nPar>2)
					fprintf_s(filePtrRes, "%11.4f", P[i].X[2] * Mod.Iso.r2);

					fprintf_s(filePtrRes, "%11.4f %11s", P[i].Obj, "Prof");
					for (j = 0; j < P[i].watConc.size(); j++)
					fprintf_s(filePtrRes, " %11.4f ", P[i].watConc[j]);
					fprintf_s(filePtrRes, "\n");
					}*/

				}

				// Calculate Particle velocities	
				double c1 = 1,
					c2 = 1;

				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						r1 = randval(0, 1);
						r2 = randval(0, 1);
						vel[i][j] += c1 * r1 * (Pi.X[j] - P[i].X[j]) + c2 * r2 * (Pg[i].X[j] - P[i].X[j]);

						double vMax = Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j];

						if (fabs(vel[i][j]) >= vMax)
						{
							if (vel[i][j] > 0) vel[i][j] = vMax;

							if (vel[i][j] <= 0) vel[i][j] = -vMax;
						}
					}
				}
			}
			catch (Exception& e)
			{
				cout << e.what() << endl;
			}
		}
		catch (Exception& e)
		{
			cout << e.what() << endl;
		}

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar) * 100 / double(Mod.SqzOpt.nGen - 1)));

		++ProBar.iProBar;

		if (nSim == 1) // Design Optimisation
		{
			fflush(filePtrAll);
		}

		if (nSim == 2) // Isotherm Matching
		{
			fflush(filePtrRes);
		}
	}

	ProBar.ShowWindow(false);



	if (nSim == 2) // Isotherm Matching
	{
		// Print in Results the best of all runs
		fprintf_s(filePtrRes, "%11.4f", Pi.X[0] * Mod.Iso.k);
		fprintf_s(filePtrRes, "%11.4f", Pi.X[1] * Mod.Iso.n);
		if (Mod.SqzOpt.nPar > 2)
			fprintf_s(filePtrRes, "%11.4f", Pi.X[2] * Mod.Iso.r2);

		fprintf_s(filePtrRes, "%11.4f %11s", Pi.Obj, "Prof");
		for (j = 0; j < Pi.watConc.size(); j++)
			fprintf_s(filePtrRes, " %11.4f ", Pi.watConc[j]);
		fprintf_s(filePtrRes, "\n");

		fclose(filePtrRes);
		fclose(filePtrAll);
	}


	if (nSim == 1) // Design Optimisation
	{
		fclose(filePtrAll);
	}

}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void PSOParallel::RunParallelSensitivityCalc(const char* NameFile)
{
	exception_ptr E;	// for handling exceptions thrown by threads

	int i, j;

	FILE* filePtrRes;

	double BasicCost = 0;

	errno_t err;

	string	input = NameFile,
		input_short;

	P.resize(Mod.SqzOpt.nBins);

	// Random Seed
	srand((unsigned)time(NULL));
	//	srand(548);

	// This file will store all the results from Sensitivity Calculations
	size_t start_pos = outputFile.find(".dat");
	if (start_pos > 5000) start_pos = outputFile.find(".DAT");

	outputFile.replace(start_pos, outputFile.length(), "SensCalc.txt");

	if ((err = fopen_s(&filePtrRes, outputFile.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", outputFile);
	else
		printf("File %s was opened for writing\r\n", outputFile);

	// define the number of processes (threads);
	// this number can be modified by USER (to be implemented)
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;

	cout << "Number of processors: " << nproc << "\r\n";	// TEST; DELETE if necessary

	if (nproc < 1)
		throw Exception("Number of threads should be greater than zero");

	Mod.suff = "_run_";
	
	Mod.SqzOpt.nPopSize = Mod.SqzOpt.nBins;

	for (i = 0; i < 3 * Mod.SqzOpt.nPar; i++)
	{
		const size_t buffsize = 10;
		char ind[buffsize];

		string s;

		if (i < Mod.SqzOpt.nPar)
		{
			sprintf_s(ind, buffsize, "%d ", i);
			string tind(ind);
			s = "X" + tind;
			fprintf_s(filePtrRes, "%11s", s.c_str());
		}

		if (i >= Mod.SqzOpt.nPar && i < 2 * Mod.SqzOpt.nPar)
		{
			string st = "SI";
			string temp;
			temp = Mod.SqzOpt.Label[i - Mod.SqzOpt.nPar];

			if (temp.find(st) == -1)
			{
				st = "Inj";
				int n = temp.find(string(st));
				if (n > -1)
					temp.replace(3, 3, string("Tim"));
				else
					temp.replace(2, 3, string("Tim"));
			}

			fprintf_s(filePtrRes, "%11s", temp.c_str());
		}

		if (i >= 2 * Mod.SqzOpt.nPar && i < 3 * Mod.SqzOpt.nPar)
		{
			string st = "SI";
			if (Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].find(st) == -1)

				fprintf_s(filePtrRes, "%11s", Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].c_str());
		}
	}

	fprintf_s(filePtrRes, " %11s\n", "RealObj");

	vector<OneRun*> OLD(Mod.SqzOpt.nBins);
	for (i = 0; i < Mod.SqzOpt.nBins; i++)
	{
		OLD[i] = new OneRun;		// to be deleted manually!
		OLD[i]->mod = Mod;
		P[i].X.resize(Mod.SqzOpt.nPar);
	}

	JobsPool JP(OLD);
	JP.finput = input_short;
	JP.fconc = "conc";		// these file names should be provided by USER (to be implemented)
	JP.fdist = "dist";
	JP.suff = "_run_";		// suffix

	vector<thread*> threads(nproc);

	ProBar.ShowWindow(true);

	ProBar.iProBar = 0;
	ProBar.m_ProgressBar.SetPos(0);

	// Start the sensitivity calculations
	int iterations = 1;

	// i.e. the number of variables chosen
	if (Mod.SqzOpt.SensCalc == 0) // Conventional Sensitivity Calcualtions
		iterations = Mod.SqzOpt.nPar;

	if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.nPar == 3) // Operational Sensitivity Calcualtions
		iterations = Mod.SqzOpt.nPar;

	for (int n = 0; n < iterations; n++)
	{
		try
		{
			// Generate New Population
			for (i = 0; i < Mod.SqzOpt.nBins; i++)
			{
				if (Mod.SqzOpt.SensCalc == 0) // Conventional Sensitivity Calcualtions
				{
					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						if (n == j)
							P[i].X[j] = (Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * (i / double(Mod.SqzOpt.nBins - 1)) + Mod.SqzOpt.lX[j];
						else
							P[i].X[j] = 1.0;
					}
				}

				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.nPar == 3) // Operational Sensitivity Calcualtions, All included
				{
					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
						SI,
						OF = P[i].X[2] * Mod.SqzOpt.Vol[2];

					P[i].X[1] = (Mod.SqzOpt.uX[1] - Mod.SqzOpt.lX[1]) * (i / double(Mod.SqzOpt.nBins - 1)) + Mod.SqzOpt.lX[1];

					SI = P[i].X[1] * Mod.SqzOpt.X[1] / 1e6; // Weight percentage

					MT = Mod.TotSIVol / SI;
					P[i].X[0] = MT / Mod.SqzOpt.Vol[0];

					OF = Mod.TotInjVol - MT;
					P[i].X[2] = OF / Mod.SqzOpt.Vol[2];
				}

				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.Only_SI == 1 && Mod.SqzOpt.Only_Tot_Vol == 0) // Operational Sensitivity Calcualtions, constrained to SI Vol
				{
					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
						SI = P[i].X[1] * Mod.SqzOpt.X[1] / 1e6;

					// Fix concentration to honor max SI Vol Inj 
					if ((MT * SI) >= Mod.TotSIVol)
					{
						SI = Mod.TotSIVol * 1e6 / MT;
						P[i].X[1] = SI / Mod.SqzOpt.X[1];
					}

				}

				if (Mod.SqzOpt.SensCalc == 1 && Mod.SqzOpt.Only_SI == 0 && Mod.SqzOpt.Only_Tot_Vol == 1) // Operational Sensitivity Calcualtions, constrained to Tot_Vol 
				{
					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						if (n == j)
							P[i].X[j] = (Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * (i / double(Mod.SqzOpt.nBins - 1)) + Mod.SqzOpt.lX[j];
						else
							P[i].X[j] = 1.0;
					}

					double	MT = P[i].X[0] * Mod.SqzOpt.Vol[0],
						OF;

					if (MT >= Mod.TotInjVol)

						MT = 0.95 * Mod.TotInjVol;

					OF = Mod.TotInjVol - MT;

					P[i].X[0] = MT / Mod.SqzOpt.Vol[0];
					P[i].X[1] = OF / Mod.SqzOpt.Vol[1];


					/*					// If MT and OF Volume in bigger than TotInjVol, adjust OF to honor
										if ((MT + OF) >= Mod.TotInjVol)
										{
											OF = Mod.TotInjVol - MT;
											if (OF <= 0)
											{
												OF = MT*0.01;
												MT = MT - OF;
											}

											P[i].X[0] = MT / Mod.SqzOpt.Vol[0];
											P[i].X[1] = OF / Mod.SqzOpt.Vol[1];
										}*/
				}
			}
			
			for (i = 0; i < Mod.SqzOpt.nBins; i++)
			{
				OLD[i]->P = P[i];
			}

			JP.SetNT();

			for (j = 0; j < nproc; j++)
				threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

			// the threads are running here!

			for (j = 0; j < nproc; j++)
			{
				threads[j]->join();		// wait for all threads to finish
				delete threads[j];		// and delete them
			}

			if (JP.E)
				rethrow_exception(JP.E);	// rethrow exception from the worker thread

			try
			{
				for (i = 0; i < Mod.SqzOpt.nBins; i++)
				{
					P[i].Obj = JP.GetObj(i);

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
						fprintf_s(filePtrRes, "%11.4f", P[i].X[j]);

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						if (P[i].X[j] * Mod.SqzOpt.X[j] < 1e5)
							fprintf_s(filePtrRes, "%11.4f", P[i].X[j] * Mod.SqzOpt.X[j]);
						else
							fprintf_s(filePtrRes, "%11.2f", P[i].X[j] * Mod.SqzOpt.X[j]);

					}

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						string st = "SI";

						if (Mod.SqzOpt.Label[j].find(st) == -1)
						{
							fprintf_s(filePtrRes, " %11.2f", P[i].X[j] * Mod.SqzOpt.Vol[j]);
						}
					}


					fprintf_s(filePtrRes, " %11.2f\n", P[i].Obj);
				}

			}
			catch (Exception& e)
			{
				cout << e.what() << endl;
			}
		}
		catch (Exception& e)
		{
			cout << e.what() << endl;
		}

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar) * 100 / double(Mod.SqzOpt.nGen - 1)));

		++ProBar.iProBar;
	}

	ProBar.ShowWindow(false);

	fclose(filePtrRes);

}
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
void PSOParallel::RunParallelSecantOpt_Vahid(const char* NameFile)
{
	exception_ptr E;	// for handling exceptions thrown by threads

	int i, j;

//	FILE * filePtrAll;

	double BasicCost = 0;

	errno_t err;

	string	input = NameFile,
		input_short;

	i = input.find_last_of(".");
	if (i < string::npos)
		input_short = input.substr(0, i);
	else
		input_short = input;

	// This file will store all the results from PSO 

	size_t start_pos = outputFile.find("Results.txt");
	outputFile.replace(start_pos, outputFile.length(), "All.txt");

	// This file will store all the results from Secant
/*	if ((err = fopen_s(&filePtrAll, outputFile.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", outputFile);
	else
		printf("File %s was opened for writing\r\n", outputFile);*/

	// Random Seed
	//srand((unsigned)time(NULL));
		srand(548);

	// define the number of processes (threads);
	// this number can be modified by USER (to be implemented)
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;

	cout << "Number of processors: " << nproc << "\r\n";	// TEST; DELETE if necessary

	if (nproc < 1)
		throw Exception("Number of threads should be greater than zero");

	Mod.suff = "_run_";

//	fprintf_s(filePtrAll, "%4s %4s  ", "#Gen", "nPar");

	for (i = 0; i < 3 * Mod.SqzOpt.nPar; i++)
	{
		const size_t buffsize = 10;
		char ind[buffsize];

		string s;

/*		if (i < Mod.SqzOpt.nPar)
		{
			sprintf_s(ind, buffsize, "%d ", i);
			string tind(ind);
			s = "X" + tind;
			fprintf_s(filePtrAll, "%11s", s.c_str());
			fprintf_s(filePtrTop, "%11s", s.c_str());

		}*/

		if (i >= Mod.SqzOpt.nPar && i < 2 * Mod.SqzOpt.nPar)
		{
			string st = "SI";
			string temp;
			temp = Mod.SqzOpt.Label[i - Mod.SqzOpt.nPar];

			if (temp.find(st) == -1)
			{
				st = "Inj";
				int n = temp.find(string(st));
				if (n > -1)
					temp.replace(3, 3, string("Tim"));
				else
					temp.replace(2, 3, string("Tim"));
			}

			//fprintf_s(filePtrAll, "%11s", temp.c_str());

		}

		/*if (i >= 2 * Mod.SqzOpt.nPar && i < 3 * Mod.SqzOpt.nPar)
		{
			string st = "SI";
			if (Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].find(st) == -1)
			{
				fprintf_s(filePtrAll, "%11s", Mod.SqzOpt.Label[i - 2 * Mod.SqzOpt.nPar].c_str());
			}
		}*/
	}

	CString st1 = "TotInjVol(bbl)",
			st2 = "TotSI(bbl)",
			st3 = "Cost per bbl";

	if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M')
	{
		st1 = "TotInjVol(m3)";
		st2 = "TotSI(m3)";
		st3 = "Cost per m3";
	}

	//fprintf_s(filePtrAll, "%16s %16s %11s %11s %11s %11s %11s %11s %11s %16s\n", "TotInjTime(hr)", st1, st2, "TotSICost", "TotInjCost", "TotCost", "%OptObj", "RealObj", "Tar-Obj", st3);

	P.resize(Mod.SqzOpt.BinsMT);

	vector<vector<double>> OFRep;
	vector<vector<double>> LifeRep;

	vector<int> ind;
	vector<Particle> temp_P;

	try
	{
		int no_of_cols = Mod.SqzOpt.IterationsSecant;
		int no_of_rows = Mod.SqzOpt.BinsMT;

		double initial_value = 0.0;

		OFRep.resize(no_of_rows, vector<double>(no_of_cols, initial_value));
		LifeRep.resize(no_of_rows, vector<double>(no_of_cols, initial_value));


		for (i = 0; i < Mod.SqzOpt.BinsMT; i++)
		{
			P[i].X.resize(Mod.SqzOpt.nPar);

			for (j = 0; j < Mod.SqzOpt.nPar; j++)
			{
				if (Mod.SqzOpt.BinsMT > 1)
				{ 
					if (j == 0)
						P[i].X[j] = (Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * (i / double(Mod.SqzOpt.BinsMT - 1)) + Mod.SqzOpt.lX[j];
					else if (j == 1)
						P[i].X[j] = 1;
					else
						P[i].X[j] = -(Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * (i / double(Mod.SqzOpt.BinsMT - 1)) + Mod.SqzOpt.uX[j];
				}
				else
					P[i].X[j] = 1;

			}

			P[i].Obj = 1e20;
		}


	}
	catch (Exception& e)
	{
		cout << e.what() << endl;
	}

	vector<thread*> threads(nproc);

	ProBar.ShowWindow(true);

	ProBar.iProBar = 0;
	ProBar.m_ProgressBar.SetPos(0);

	pso_dat.clear();

	const double errval = 0.5 / 100;

	// Start with the generations
	for (int n = 0; n < Mod.SqzOpt.IterationsSecant; n++)
	{
		try
		{
			int temp_BinsMT = 0;
			//temp_P.resize(Mod.SqzOpt.BinsMT);
			temp_P = P;
			vector<int> ind;
			// Generate New Population
			for (i = 0; i < Mod.SqzOpt.BinsMT; i++)
			{
				if (n == 0)
				{
					temp_BinsMT = Mod.SqzOpt.BinsMT;
					ind.push_back(i);
				}
				else if (n == 1 && P[i].Obj > errval)
				{
					temp_BinsMT++;
					ind.push_back(i);
					temp_P.resize(temp_BinsMT);
					temp_P[temp_BinsMT - 1] = P[i];

					if (LifeRep[i][0] > 0)
					{
						if (OFRep[i][n - 1] != 0)
							temp_P[temp_BinsMT - 1].X[2] = (Mod.SqzOpt.Obj_T_V / LifeRep[i][n - 1] * OFRep[i][n - 1]) / Mod.SqzOpt.X[2];
						else
							temp_P[temp_BinsMT - 1].X[2] = 0.01;
					}

					else
						temp_P[temp_BinsMT - 1].X[2] = (0.2 * OFRep[i][n - 1]) / Mod.SqzOpt.X[2];
				}
				else if (n > 1 && LifeRep[i][n - 1] != LifeRep[i][n - 2] && P[i].Obj > errval)
				{
					temp_BinsMT++;
					ind.push_back(i);
					temp_P.resize(temp_BinsMT);
					temp_P[temp_BinsMT - 1] = P[i];

					temp_P[temp_BinsMT - 1].X[2] = (OFRep[i][n - 1] + (Mod.SqzOpt.Obj_T_V - LifeRep[i][n - 1]) / (LifeRep[i][n - 1] - LifeRep[i][n - 2]) * (OFRep[i][n - 1] - OFRep[i][n - 2])) / Mod.SqzOpt.X[2];

					if (temp_P[temp_BinsMT - 1].X[2] < 0)
						temp_P[temp_BinsMT - 1].X[2] = 0;
				}
			}

			vector<OneRun*> OLD(temp_BinsMT);
			for (i = 0; i < temp_BinsMT; i++)
			{
				OLD[i] = new OneRun;		// to be deleted manually!
				OLD[i]->P = temp_P[i];
				OLD[i]->mod = Mod;
			}

			JobsPool JP(OLD);

			JP.finput = input_short;
			JP.fconc = "conc";		// these file names should be provided by USER (to be implemented)
			JP.fdist = "dist";
			JP.suff = "_run_";		// suffix

			JP.SetNT();

			for (j = 0; j < nproc; j++)
				threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

			// the threads are running here!

			for (j = 0; j < nproc; j++)
			{
				threads[j]->join();		// wait for all threads to finish
				delete threads[j];		// and delete them
			}

			if (JP.E)
			{
				rethrow_exception(JP.E);	// rethrow exception from the worker thread
			}

			try
			{

				// Assign new Objectives to Particles
				for (i = 0; i < temp_BinsMT; i++)
				{
					temp_P[i].Obj = fabs(Mod.SqzOpt.Obj_T_V - JP.GetObj(i)) / Mod.SqzOpt.Obj_T_V;
				}

				int c = 0;

				for (i = 0; i < Mod.SqzOpt.BinsMT; i++)
				{
					if (c < ind.size() && i == ind[c])
					{
						P[i] = temp_P[c];
						LifeRep[i][n] = JP.GetObj(c);
						OFRep[i][n] = temp_P[c].X[2] * Mod.SqzOpt.X[2];
						c++;
					}
					else
					{
						LifeRep[i][n] = LifeRep[i][n - 1];
						OFRep[i][n] = OFRep[i][n - 1];
					}
				}


				for (i = 0; i < Mod.SqzOpt.BinsMT; i++)
				{
					// Check if Particle i is top 1%
					bool check = false;

					if (n == (Mod.SqzOpt.IterationsSecant - 1) && P[i].Obj < 2 * errval) check = true;


					data_sqz tempdat;
					tempdat.xVal.resize(Mod.SqzOpt.nPar);
					tempdat.dat.resize(Mod.SqzOpt.nPar + 8);

//					fprintf_s(filePtrAll, "%-4d %-4d ", n, i);

					double	TotInjTime = 0,
						TotInjVol = 0;

					/*for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						fprintf_s(filePtrAll, "%11.4f", P[i].X[j]);
					}*/

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						/*if (P[i].X[j] * Mod.SqzOpt.X[j] < 1e5)
						{
							fprintf_s(filePtrAll, "%11.4f", P[i].X[j] * Mod.SqzOpt.X[j]);
						}
						else
						{
							fprintf_s(filePtrAll, "%11.2f", P[i].X[j] * Mod.SqzOpt.X[j]);
						}*/

						tempdat.xVal[j] = P[i].X[j] * Mod.SqzOpt.X[j];
					}

					int j1 = 0;

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						string st = "SI";

						if (Mod.SqzOpt.Label[j].find(st) == -1)
						{
							TotInjTime += (P[i].X[j] * Mod.SqzOpt.X[j]);
							TotInjVol += (P[i].X[j] * Mod.SqzOpt.Vol[j]);
							//fprintf_s(filePtrAll, " %11.2f", P[i].X[j] * Mod.SqzOpt.Vol[j]);
						}
						else
						{
							tempdat.dat[j1] = P[i].X[j] * Mod.SqzOpt.X[j];
							j1++;
						}
					}

					int j2 = 0;
					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						string st = "SI";

						if (Mod.SqzOpt.Label[j].find(st) == -1)
						{
							tempdat.dat[j1 + j2] = P[i].X[j] * Mod.SqzOpt.Vol[j];
							j2++;
						}
					}

					double	TotalSI = 0,
						TotalSICost = 0,
						TotalInjCost = 0,
						TotalCost = 0,
						TotalTreatedVolume = 0,
						CostPerVol = 0;

					TotInjTime = 0;
					TotInjVol = 0;

					for (j = 0; j < Mod.stg.size(); j++)
					{
						if (Mod.stg[j].TypeStg == 1)
						{
							if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 1)

								TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

							if (Mod.stg[j].objSI == 1 && Mod.stg[j].objTime == 0)

								TotalSICost += ((P[i].X[Mod.stg[j].objIndexSI] * Mod.SqzOpt.X[Mod.stg[j].objIndexSI]) * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));

							if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 1)

								TotalSICost += (Mod.stg[j].InitialCw[0] * (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]));

							if (Mod.stg[j].objSI == 0 && Mod.stg[j].objTime == 0)

								TotalSICost += (Mod.stg[j].InitialCw[0] * (Mod.stg[j].Qwt * Mod.stg[j].Time * 60));
						}

						if (Mod.stg[j].TypeStg != 3)
						{
							if (Mod.stg[j].objTime == 1)
							{
								TotInjTime += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.X[Mod.stg[j].objIndexTime]);

								if (Mod.stg[j].TypeStg != 5)
								{
									TotInjVol += (P[i].X[Mod.stg[j].objIndexTime] * Mod.SqzOpt.Vol[Mod.stg[j].objIndexTime]);
								}
							}
							else
							{
								TotInjTime += (Mod.stg[j].Time);

								if (Mod.stg[j].TypeStg != 5)
								{
									TotInjVol += (Mod.stg[j].Qwt * Mod.stg[j].Time * 60.0);
								}
							}
						}
					}

					TotalSI = 1e-6 * TotalSICost;
					TotalSICost = 1e-6 * (TotalSICost * Mod.SqzOpt.SIBblRate);
					TotalInjCost = TotInjTime * Mod.SqzOpt.InjHourRate;
					TotalCost = TotalSICost + TotalInjCost;

					double temp_Vlife = 0.0;

					if (Mod.kPlot == 1 || Mod.kPlot == 3)
					{
						if (Mod.UnitSpec == 'm' || Mod.UnitSpec == 'M') TotalTreatedVolume = LifeRep[i][n] * 100;
						else TotalTreatedVolume = LifeRep[i][n] * 1000;
					}
					else
					{
						temp_Vlife = LifeRep[i][n];

						for (int s = 0; s < Mod.stg.size(); s++)
						{
							if (Mod.stg[s].TypeStg != 3)
								temp_Vlife -= Mod.stg[s].Time / 24;
							else
							{
								if (temp_Vlife < Mod.stg[s].Time)
								{
									TotalTreatedVolume += Mod.stg[s].Qwt * temp_Vlife;
									break;
								}
								else
								{
									TotalTreatedVolume += Mod.stg[s].Qwt * Mod.stg[s].Time;
									temp_Vlife -= Mod.stg[s].Time;
								}
							}
						}
					}

					CostPerVol = TotalCost / TotalTreatedVolume;

					//fprintf_s(filePtrAll, " %16.2f %16.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.2f %11.4f\n", TotInjTime, TotInjVol, TotalSI, TotalSICost, TotalInjCost, TotalCost, P[i].Obj, LifeRep[i][n], Mod.Obj_T_V - LifeRep[i][n], CostPerVol);
					
					if (n == (Mod.SqzOpt.IterationsSecant - 1) && P[i].Obj < 2 * errval)
					{
						tempdat.dat[Mod.SqzOpt.nPar] = LifeRep[i][n];	// SQZ_LT
						tempdat.dat[Mod.SqzOpt.nPar + 1] = TotalCost;	// TotCost
						tempdat.dat[Mod.SqzOpt.nPar + 2] = TotalInjCost;	// TotInjCost
						tempdat.dat[Mod.SqzOpt.nPar + 3] = TotInjTime;	// TotInjTime
						tempdat.dat[Mod.SqzOpt.nPar + 4] = TotInjVol;	// TotInjVol
						tempdat.dat[Mod.SqzOpt.nPar + 5] = TotalSI;	// TotSI
						tempdat.dat[Mod.SqzOpt.nPar + 6] = TotalSICost;// TotSICost
						tempdat.dat[Mod.SqzOpt.nPar + 7] = CostPerVol;// CostPerVol

						pso_dat.push_back(tempdat);
					}


				}
			}
			catch (Exception& e)
			{
				cout << e.what() << endl;
			}
		}
		catch (Exception& e)
		{
			cout << e.what() << endl;
		}

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar) * 100 / double(Mod.SqzOpt.IterationsSecant - 1)));

		++ProBar.iProBar;
	}

	ProBar.ShowWindow(false);

//	fclose(filePtrAll);
}

// ------------------------------------------------------------------------------------------------

void PSOParallel::ParetoFronts()
{
	FILE* filePtr;

	char buffer[1024],
		* ret,
		substring[5];

	int iTemp,
		count = 0,
		num1 = 0;

	double dTemp;

	vector<SqzOptimisation> Temp,
		ParInjTime;

	/*	Mod.SqzOpt.nPar = 3;
	Mod.SqzOpt.nGen = 1;
	Mod.SqzOpt.nPopSize = 5;
	outputFile = "F:\\Oscar\\Old E\\Old F\\Squeeze VI Versions\\2016\\SQUEEZE 10 07.2016 Bridging Adiditive_PSO ID Parallel - Copy\\xPSOAll.txt";
	*/
	if ((filePtr = fopen(outputFile.c_str(), "r")) == NULL)
		printf("The file %s was not opened\n", outputFile);
	else
		printf("The file %s was opened\n", outputFile);

	Temp.resize(Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize);

	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);

	fgets(buffer, 1024, filePtr);

	char* p = strtok(buffer, " ");
	while (p)
	{
		if (strstr(p, "jSI"))
		{
			count++;
		}

		p = strtok(NULL, " ");
		num1++;
	}

	if (Temp[0].Label.size() != 2 * Mod.SqzOpt.nPar - count)
		Temp[0].Label.resize(2 * Mod.SqzOpt.nPar - count);

	fseek(filePtr, 0L, SEEK_SET);

	fscanf(filePtr, "%s %s", buffer, buffer);

	for (int n = 0; n < Mod.SqzOpt.nPar; n++)
		fscanf(filePtr, "%s ", buffer);

	for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
		fscanf(filePtr, "%s ", Temp[0].Label[n].c_str());

	fscanf(filePtr, "%s %s %s %s %s %s %s %s", buffer, buffer, buffer, buffer, buffer, buffer, buffer, buffer);

	for (int i = 0; i < Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize; i++)
	{
		if (Temp[i].X.size() != 2 * Mod.SqzOpt.nPar - count)
			Temp[i].X.resize(2 * Mod.SqzOpt.nPar - count);

		fscanf(filePtr, "%d %d ", &iTemp, &iTemp);

		for (int n = 0; n < Mod.SqzOpt.nPar; n++)
			fscanf(filePtr, "%lf", &dTemp);

		for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
			fscanf(filePtr, "%lf ", &Temp[i].X[n]);

		if (Temp[i].Obj.size() != 6)
			Temp[i].Obj.resize(6);

		// Objectives to calculate the Pareto Fronts
		/*Obj[0] = TotInjTime
		Obj[1] = TotInjVol
		Obj[2] = TotSICost
		Obj[3] = TotInjCost
		Obj[4] = TotCost
		Obj[5] = %OptObj*/

		for (int n = 0; n < 6; n++)
			fscanf(filePtr, "%lf ", &Temp[i].Obj[n]);

		fscanf(filePtr, "%lf %lf ", &Temp[i].Obj_T_V, &dTemp);
	}

	// Identify the Pareto designs
	double Obj1,
		TObj1,
		Obj2,
		TObj2;

	bool Cond1 = true,
		Cond2 = true;

	int j,
		nObj1 = 1,
		nObj2 = 5;
	// Objectives 
	// TotInjTime = 0
	// TotInjVol = 1
	// TotSICost = 2
	// TotInjCost = 3
	// TotCost = 4
	// %OptObj = 5

	SqzOptimisation swap;

	for (int i = 0; i < Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize; i++)
	{
		// TotInjVol
		Obj1 = Temp[i].Obj[nObj1];

		// %OptObj
		Obj2 = Temp[i].Obj[nObj2];

		j = 0;

		Cond1 = true,
			Cond2 = true;

		while (j < (Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize) && (Cond1 || Cond2))
		{
			Cond1 = true,
				Cond2 = true;

			TObj1 = Temp[j].Obj[nObj1];
			TObj2 = Temp[j].Obj[nObj2];

			if (i != j)
			{
				if (Obj1 > TObj1) Cond1 = false;
				if (Obj2 > TObj2) Cond2 = false;
			}

			j++;
		}

		if (Cond1 || Cond2)
		{
			ParInjTime.push_back(Temp[i]);
		}
	}

	// Bubble sort against Injection Total Time
	for (int i = 0; i < ParInjTime.size(); i++)
	{
		for (int j = 0; j < ParInjTime.size() - i - 1; j++)
		{
			if (ParInjTime[j].Obj[nObj1] > ParInjTime[j + 1].Obj[nObj1]) // For decreasing order use < 
			{
				swap = ParInjTime[j];
				ParInjTime[j] = ParInjTime[j + 1];
				ParInjTime[j + 1] = swap;
			}
		}
	}

	fclose(filePtr);

	size_t start_pos = outputFile.find("All.txt");
	outputFile.replace(start_pos, outputFile.length(), "VolPar.txt");

	if ((filePtr = fopen(outputFile.c_str(), "w")) == NULL)
		printf("The file %s was not opened\n", outputFile);
	else
		printf("The file %s was opened\n", outputFile);

	fprintf(filePtr, "%7s ", "#Design");
	for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
		fprintf(filePtr, "%11s ", Temp[0].Label[n].c_str());
	fprintf(filePtr, "%11s %11s %11s %11s %11s %11s %11s\n", "TotInjTime", "TotInjVol", "TotSICost", "TotSICost", "TotInjCost", "TotCost", "%OptObj", "RealObj");

	for (int i = 0; i < ParInjTime.size(); i++)
	{
		fprintf(filePtr, "%-6d ", i);
		for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
			fprintf(filePtr, "%11.2f ", ParInjTime[i].X[n]);

		for (int n = 0; n < 6; n++)
			fprintf(filePtr, "%11.2f ", ParInjTime[i].Obj[n]);

		fprintf(filePtr, "%11.2f\n", ParInjTime[i].Obj_T_V);

	}

	fclose(filePtr);


	// Pareto Squeeze lifetime and Total Cost
	nObj1 = 4,
		nObj2 = 5;
	// Objectives 
	// TotInjTime = 0
	// TotInjVol = 1
	// TotSICost = 2
	// TotInjCost = 3
	// TotCost = 4
	// %OptObj = 5

	ParInjTime.clear();

	for (int i = 0; i < Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize; i++)
	{
		// TotInjVol
		Obj1 = Temp[i].Obj[nObj1];

		// %OptObj
		Obj2 = Temp[i].Obj[nObj2];

		j = 0;

		Cond1 = true,
			Cond2 = true;

		while (j < (Mod.SqzOpt.nGen * Mod.SqzOpt.nPopSize) && (Cond1 || Cond2))
		{
			Cond1 = true,
				Cond2 = true;

			TObj1 = Temp[j].Obj[nObj1];
			TObj2 = Temp[j].Obj[nObj2];

			if (i != j)
			{
				if (Obj1 > TObj1) Cond1 = false;
				if (Obj2 > TObj2) Cond2 = false;
			}

			j++;
		}

		if (Cond1 || Cond2)
		{
			ParInjTime.push_back(Temp[i]);
		}
	}

	// Bubble sort against Injection Total Time
	for (int i = 0; i < ParInjTime.size(); i++)
	{
		for (int j = 0; j < ParInjTime.size() - i - 1; j++)
		{
			if (ParInjTime[j].Obj[nObj1] > ParInjTime[j + 1].Obj[nObj1]) // For decreasing order use < 
			{
				swap = ParInjTime[j];
				ParInjTime[j] = ParInjTime[j + 1];
				ParInjTime[j + 1] = swap;
			}
		}
	}

	start_pos = outputFile.find("VolPar.txt");
	outputFile.replace(start_pos, outputFile.length(), "CostPar.txt");

	if ((filePtr = fopen(outputFile.c_str(), "w")) == NULL)
		printf("The file %s was not opened\n", outputFile);
	else
		printf("The file %s was opened\n", outputFile);

	fprintf(filePtr, "%7s ", "#Design");
	for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
		fprintf(filePtr, "%11s ", Temp[0].Label[n].c_str());
	fprintf(filePtr, "%11s %11s %11s %11s %11s %11s %11s\n", "TotInjTime", "TotInjVol", "TotSICost", "TotInjCost", "TotCost", "%OptObj", "RealObj");

	for (int i = 0; i < ParInjTime.size(); i++)
	{
		fprintf(filePtr, "%-6d ", i);
		for (int n = 0; n < 2 * Mod.SqzOpt.nPar - count; n++)
			fprintf(filePtr, "%11.2f ", ParInjTime[i].X[n]);

		for (int n = 0; n < 6; n++)
			fprintf(filePtr, "%11.2f ", ParInjTime[i].Obj[n]);

		fprintf(filePtr, "%11.2f\n", ParInjTime[i].Obj_T_V);

	}

	fclose(filePtr);

	int num2 = 2 * Mod.SqzOpt.nPar - count + 1;

	GnuPlotFile(num1 - 6, num1 - 2, num1 - 3, num2 + 2, num2 + 6, num2 + 5);
}
//----------------------------------------------------------------------------------------
void PSOParallel::GnuPlotFile(int nA1, int  nA2, int nA3, int nP1, int nP2, int nP3)
{
	FILE* filePtr;
	string	nFile,
		st;

	char full[_MAX_PATH];

	nFile = outputFile;

	size_t start_pos = nFile.find("PSOCostPar.txt");
	nFile.replace(start_pos, nFile.length(), "Vol.plt");

	if ((filePtr = fopen(nFile.c_str(), "w")) == NULL)
		printf("The file %s was not opened\n", outputFile);
	else
		printf("The file %s was opened\n", outputFile);

	start_pos = nFile.find_last_of("\\");
	size_t last_pos = nFile.find(".plt");
	st = nFile.substr(start_pos + 1, last_pos - start_pos - 1);
	fprintf(filePtr, "set title '%s' noenhanced\n", st.c_str());

	fprintf(filePtr, "set autoscale\n", nFile.c_str());

	nFile = outputFile;

	fprintf(filePtr, "set terminal wxt\nset size 1.0, 1.0\nset palette model RGB defined(0 \"green\", 1 \"blue\", 2 \"red\")\n");
	fprintf(filePtr, "set grid xtics\nset grid ytics\n");

	if (Mod.UnitSpec == 'C')
		st = "bbls";
	else
		st = "m3";

	fprintf(filePtr, "set xlabel \"Total Injected Water %s\" offset 0, 0.5\n", st.c_str());
	fprintf(filePtr, "set ylabel \"(Target - Simulated) / Target\" offset 1.5, 0\n");

	if (Mod.UnitSpec == 'C')
		st = "GBP";
	else
		st = "$";

	fprintf(filePtr, "set cblabel \"Total Cost %s\" offset -0.5, 0\n", st.c_str());

	start_pos = nFile.find("PSOCostPar.txt");
	nFile.replace(start_pos, nFile.length(), "PSOAll.txt");
	//	fflush(filePtr);

	fprintf(filePtr, "plot '%s' using %d:%d:%d title 'Squeeze Designs' with points pt 7 lc palette, \\\n", nFile.c_str(), nA1, nA2, nA3);

	start_pos = nFile.find("PSOAll.txt");
	nFile.replace(start_pos, nFile.length(), "PSOVolPar.txt");

	fprintf(filePtr, "'%s' using %d:%d:%d with  p pt 7 palette notitle, \\\n", nFile.c_str(), nP1, nP2, nP3);

	fprintf(filePtr, "'%s' using %d:%d title 'Pareto Front' with l lc - 1", nFile.c_str(), nP1, nP2);

	fflush(filePtr);

	fclose(filePtr);

	nFile = outputFile;

	start_pos = nFile.find("PSOCostPar.txt");
	nFile.replace(start_pos, nFile.length(), "Cost.plt");

	if ((filePtr = fopen(nFile.c_str(), "w")) == NULL)
		printf("The file %s was not opened\n", outputFile);
	else
		printf("The file %s was opened\n", outputFile);

	start_pos = nFile.find_last_of("\\");
	last_pos = nFile.find(".plt");
	st = nFile.substr(start_pos + 1, last_pos - start_pos - 1);
	fprintf(filePtr, "set title '%s' noenhanced\n", st.c_str());

	fprintf(filePtr, "set autoscale\n", nFile.c_str());

	nFile = outputFile;

	fprintf(filePtr, "set terminal wxt\nset size 1.0, 1.0\nset palette model RGB defined(0 \"green\", 1 \"blue\", 2 \"red\")\n");
	fprintf(filePtr, "set grid xtics\nset grid ytics\n");

	if (Mod.UnitSpec == 'C')
		st = "GBP";
	else
		st = "$";

	fprintf(filePtr, "set xlabel \"Total Cost %s\" offset 0, 0.5\n", st.c_str());
	fprintf(filePtr, "set ylabel \"(Target - Simulated) / Target\" offset 1.5, 0\n");

	if (Mod.UnitSpec == 'C')
		st = "bbls";
	else
		st = "m3";

	fprintf(filePtr, "set cblabel \"Total Injected Water %s\" offset -0.5, 0\n", st.c_str());

	start_pos = nFile.find("PSOCostPar.txt");
	nFile.replace(start_pos, nFile.length(), "PSOAll.txt");
	//	fflush(filePtr);

	fprintf(filePtr, "plot '%s' using %d:%d:%d title 'Squeeze Designs' with points pt 7 lc palette, \\\n", nFile.c_str(), nA3, nA2, nA1);

	start_pos = nFile.find("PSOAll.txt");
	nFile.replace(start_pos, nFile.length(), "PSOCostPar.txt");

	fprintf(filePtr, "'%s' using %d:%d:%d with  p pt 7 palette notitle, \\\n", nFile.c_str(), nP3, nP2, nP1);

	fprintf(filePtr, "'%s' using %d:%d title 'Pareto Front' with l lc - 1", nFile.c_str(), nP3, nP2);

	fflush(filePtr);

	fclose(filePtr);
}


//----------------------------------------------------------------------------------------
double PSOParallel::randval(double low, double high)
{
	double val;

	val = double(rand() % 1000 / 1000.0) * (high - low) + low;

	return(val);
}


void PSOParallel::RunParallelMOPSO(const char* NameFile)
{
	// define the number of processes (threads);
	// this number can be modified by USER (to be implemented)
	SYSTEM_INFO sysinfo;
	GetSystemInfo(&sysinfo);
	int nproc = sysinfo.dwNumberOfProcessors - 1;


	int i, j, k;
	vector<vector<double>> vel;

	//exception_ptr E;	// for handling exceptions thrown by threads

	FILE* filePtrGen,	// file storing swarm generations	
		* filePtrPar,	// file to store pareto front	
		* filePtrArc;	// file to store Archive

	errno_t err;

	string	input = NameFile,
		input_short,
		input_temp;

	i = input.find_last_of(".");
	if (i < string::npos)
		input_short = input.substr(0, i);
	else
		input_short = input;

	// File storing the Generations
	input_temp = outputFile + "Gen.txt";

	if ((err = fopen_s(&filePtrGen, input_temp.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", input_temp);
	else
		printf("File %s was opened for writing\r\n", input_temp);

	// File storing the Pareto Front
	input_temp = outputFile + "Par.txt";

	if ((err = fopen_s(&filePtrPar, input_temp.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", input_temp);
	else
		printf("File %s was opened for writing\r\n", input_temp);

	// File storing the Archive
	input_temp = outputFile + "Arc.txt";

	if ((err = fopen_s(&filePtrArc, input_temp.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", input_temp);
	else
		printf("File %s was opened for writing\r\n", input_temp);

	Mod.suff = "_run_";
	Mod.ReadFileSQZ5Optimisation(input.c_str());		// read input data

	vector<Particle>	Pb(Mod.SqzOpt.nPopSize),		// Stores the personal best location of each particle;
		Pareto(Mod.SqzOpt.nPopSize),	// Stores the dominant solutions 
		Archive(Mod.SqzOpt.nPopSize);	// Archive the dominant particles (leaders)

	P.resize(Mod.SqzOpt.nPopSize);

	Mod.SqzOpt.lObj.resize(Mod.nObjs);
	Mod.SqzOpt.uObj.resize(Mod.nObjs);

	for (i = 0; i < Mod.nObjs; i++)
	{
		Mod.SqzOpt.lObj[i] = 1e20;
		Mod.SqzOpt.uObj[i] = -1e20;
	}

	Grid g;

	g.depth.resize(Mod.nObjs);
	g.delta.resize(Mod.nObjs);

	int dens = 1;
	vector<int> density;

	for (i = 0; i < Mod.nObjs; i++)
	{
		g.depth[i] = 5;
		dens *= g.depth[i];
	}

	density.resize(dens);

	for (i = 0; i < Mod.nObjs; i++)
	{
		density[i] = 0;
		g.delta[i] = 1.0 / g.depth[i];
	}

	double r1, r2;

	int no_of_cols = Mod.SqzOpt.nPar;
	int no_of_rows = Mod.SqzOpt.nPopSize;
	double initial_value = 0.0;
	vel.resize(no_of_rows, vector<double>(no_of_cols, initial_value));

	try
	{
		for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
		{
			P[i].X.resize(Mod.SqzOpt.nPar);
			P[i].Objs.resize(Mod.nObjs);
			P[i].CObjs.resize(Mod.nObjs);

			Pb[i].X.resize(Mod.SqzOpt.nPar);
			Pb[i].Objs.resize(Mod.nObjs);
			Pb[i].CObjs.resize(Mod.nObjs);

			Pareto[i].X.resize(Mod.SqzOpt.nPar);
			Pareto[i].Objs.resize(Mod.nObjs);
			Pareto[i].CObjs.resize(Mod.nObjs);

			Archive[i].X.resize(Mod.SqzOpt.nPar);
			Archive[i].Objs.resize(Mod.nObjs);
			Archive[i].CObjs.resize(Mod.nObjs);

			for (j = 0; j < Mod.nObjs; j++)
				Pb[i].Objs[j] = 100;

			for (j = 0; j < Mod.SqzOpt.nPar; j++)
			{
				// Random Initial Population
				P[i].X[j] = (Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j]) * randval(0, 1) + Mod.SqzOpt.lX[j];
			}
		}
	}
	catch (Exception& e)
	{
		cout << e.what() << endl;
	}

	// Randomized Particle Velocities
	for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
	{
		for (j = 0; j < Mod.SqzOpt.nPar; j++)
		{
			vel[i][j] = 0;

			if (randval(0, 1) > 0.5) vel[i][j] = randval(0, 0.5) * P[i].X[j];

			else vel[i][j] = -randval(0, 0.5) * P[i].X[j];
		}
	}

	vector<OneRun*> OLD(Mod.SqzOpt.nPopSize);
	for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
	{
		OLD[i] = new OneRun;		// to be deleted manually!
	}

	JobsPool JP(OLD);
	JP.finput = input_short;
	JP.fconc = "conc";		// these file names should be provided by USER (to be implemented)
	JP.fdist = "dist";
	JP.suff = "_run_";		// suffix

	vector<thread*> threads(nproc);

	ProBar.ShowWindow(true);

	ProBar.iProBar = 0;
	ProBar.m_ProgressBar.SetPos(0);

	// Pointer to objective fuction
	vector<double> arr;
	arr.push_back(1.0);

	PrintResults(filePtrGen, P, "Generations", -1, 0, Mod);
	PrintResults(filePtrPar, P, "Pareto", -1, 0, Mod);
	PrintResults(filePtrArc, P, "Archive", -1, 0, Mod);

	Mod.nArc = 0;
	Mod.nPareto = 0;

	// Start with the generations
	for (int n = 0; n < Mod.SqzOpt.nGen; n++)
	{
		try
		{
			// Generate New Population
			for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
			{
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					P[i].X[j] = P[i].X[j] + vel[i][j];
				}

				// DAMPING BOUNDARY CONDITION
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					// The coordinate j is reset to the boundary, then the velocity sign is reversed
					// and multiplied by random between [0,1]
					if (P[i].X[j] >= Mod.SqzOpt.uX[j])

						P[i].X[j] = Mod.SqzOpt.uX[j] - randval(0, 0.2) * vel[i][j];

					if (P[i].X[j] <= Mod.SqzOpt.lX[j])

						P[i].X[j] = Mod.SqzOpt.lX[j] - randval(0, 0.2) * vel[i][j];
				}
			}

			// Make sure original design is included
			if (n == 0)
			{
				for (j = 0; j < Mod.SqzOpt.nPar; j++)
				{
					P[0].X[j] = 1;
				}
			}

			// Evaluate New Population
			//	Mod.WriteOptimisationModels(input.c_str(), P, n);		// write input for each separate layer
			try
			{
				Mod.WriteOptimisationFileModels(input.c_str(), P);
			}
			catch (Exception& e)
			{
				cout << e.what() << endl;
			}

			/*for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
			{
				OLD[i]->nLay = Mod.nLay;

				OLD[i]->GrafMIC = Mod.GrafMIC;
			}*/

			JP.SetNT();

			for (j = 0; j < nproc; j++)
				threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

			// the threads are running here!

			for (j = 0; j < nproc; j++)
			{
				threads[j]->join();		// wait for all threads to finish
				delete threads[j];		// and delete them
			}

			if (JP.E)
				rethrow_exception(JP.E);	// rethrow exception from the worker thread

			try
			{
				// Assign new Objectives to Particles
				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					double Obj;

					P[i].nGrid = 0;

					// Assign values to arr vector
					arr.clear();

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
						arr.push_back(P[i].X[j]);

					// Calculate Objectives
					P[i].Objs[0] = fabs(Mod.Obj_T_V - JP.GetObj(i)) / Mod.Obj_T_V;

					P[i].Objs[1] = P[i].X[0] + P[i].X[2];

					if (Mod.nObjs > 2)
					{
						P[i].Objs[2] = P[i].X[0] * P[i].X[1];
					}
				}

				// Calculate Max and Min Objectives
				// Limits of Objective space
				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					for (j = 0; j < Mod.nObjs; j++)
					{
						if (Mod.SqzOpt.lObj[j] > P[i].Objs[j]) Mod.SqzOpt.lObj[j] = P[i].Objs[j];

						if (Mod.SqzOpt.uObj[j] < P[i].Objs[j]) Mod.SqzOpt.uObj[j] = P[i].Objs[j];
					}
				}

				// Linear interpolation to [0,1], it is easier to calculate the grid density
				// Calculate Max and Min Objectives
				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					double Obj = 0;

					P[i].nGrid = 0;

					for (j = 0; j < Mod.nObjs; j++)
					{
						Obj = (P[i].Objs[j] - Mod.SqzOpt.lObj[j]) / (Mod.SqzOpt.uObj[j] - Mod.SqzOpt.lObj[j]);

						// Calculate grid position in the Objectives space
						P[i].CObjs[j] = int(Obj / g.delta[j]) + 1;

						if (P[i].CObjs[j] > g.depth[j]) P[i].CObjs[j] = g.depth[j];

						//		cout << P[i].CObjs[j] << " ";
						double Prod = 1;
						if (j == 0)
							P[i].nGrid += P[i].CObjs[j];
						else
						{
							for (int k1 = 0; k1 < j; k1++)
								Prod *= g.depth[k1];

							P[i].nGrid += (P[i].CObjs[j] - 1) * Prod;
						}

						P[i].density = 0;
					}

					// Identify Personal Best for each Particle
					// Check if new position is dominates Pb
					if (IsDominant(P[i], Pb[i], Mod.nObjs))
					{
						EquateParticle(Pb[i], P[i], Mod.SqzOpt.nPar, Mod.nObjs);
					}
					else
					{
						// Check if Pb dominates new position
						if (!IsDominant(Pb[i], P[i], Mod.nObjs))
							// If neither of them is dominated by the other, then we select one of them	randomly
							if (randval(0, 1) > 0.5)
							{
								EquateParticle(Pb[i], P[i], Mod.SqzOpt.nPar, Mod.nObjs);
							}
					}

				}

				PrintResults(filePtrGen, P, "Generations", n, Mod.SqzOpt.nPopSize, Mod);
				fflush(filePtrGen);

				// Identify Dominant Particles, Pareto Front
				Mod.nPareto = 0;

				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					bool check = true;
					j = 0;
					while (j < Mod.SqzOpt.nPopSize && check)
					{
						if (IsDominant(Pb[j], Pb[i], Mod.nObjs) && j != i) check = false;
						j++;
					}

					if (check)
					{
						EquateParticle(Pareto[Mod.nPareto], Pb[i], Mod.SqzOpt.nPar, Mod.nObjs);

						Mod.nPareto++;
					}
				}

				// Print Pareto Front
				PrintResults(filePtrPar, Pareto, "Pareto", n, Mod.nPareto, Mod);
				fflush(filePtrPar);

				// Include all particle in the Pareto front in the archive in the first iteration
				if (Mod.nArc == 0)
				{
					Mod.nArc = Mod.nPareto;

					for (i = 0; i < Mod.nPareto; i++)
					{
						EquateParticle(Archive[i], Pareto[i], Mod.SqzOpt.nPar, Mod.nObjs);
					}
					// Calculate density of the archive
					for (i = 0; i < dens; i++)

						density[i] = 0;

					for (i = 0; i < Mod.nArc; i++)

						density[Archive[i].nGrid]++;
				}
				else // Update Archive
				{
					// Replace any Particle in the archive that is dominated
					for (i = 0; i < Mod.nPareto; i++)
					{
						j = 0;

						while (j < Mod.nArc)
						{
							// if Pareto[i] dominates a particle Archive[j] in the archive, replace
							if (IsDominant(Pareto[i], Archive[j], Mod.nObjs)) EquateParticle(Archive[j], Pareto[i], Mod.SqzOpt.nPar, Mod.nObjs);
							j++;
						}
					}

					// Identify the Particles in Pareto, iP, that are not dominated by any in the archive and archive them
					int iP;
					for (iP = 0; iP < Mod.nPareto; iP++)
					{
						bool check = true;
						j = 0;

						while (j < Mod.nArc && check)
						{
							if (IsDominant(Archive[j], Pareto[iP], Mod.nObjs)) check = false;
							j++;
						}

						if (check)
						{
							// Include Pareto[iP] in the Archive if it is not full 
							if (Mod.nArc < Mod.SqzOpt.nPopSize)
							{
								Mod.nArc++;
								EquateParticle(Archive[Mod.nArc - 1], Pareto[iP], Mod.SqzOpt.nPar, Mod.nObjs);

							}
							else // The Archive is full, so we need to replace one particle, a particle in the most dense populated cuboid will be replaced 
							{
								// Calculate density of the archive
								for (i = 0; i < dens; i++)

									density[i] = 0;

								for (i = 0; i < Mod.nArc; i++)

									density[Archive[i].nGrid]++;

								// Allocate density value to each particle in the archive
								int maxDensity = 0,
									iReplace;

								for (i = 0; i < Mod.nArc; i++)
								{
									Archive[i].density = density[Archive[i].nGrid];

									if (maxDensity < Archive[i].density)
									{
										maxDensity = Archive[i].density;
										iReplace = i;
									}
								}

								EquateParticle(Archive[iReplace], Pareto[iP], Mod.SqzOpt.nPar, Mod.nObjs);
							}
						}
					}
				}

				// Print Archive
				PrintResults(filePtrArc, Archive, "Archive", n, Mod.nArc, Mod);
				fflush(filePtrArc);

				// Re-calculate density of the archive
				for (i = 0; i < dens; i++)

					density[i] = 0;

				for (i = 0; i < Mod.nArc; i++)

					density[Archive[i].nGrid]++;

				// Calculate Probability of each particle
				double max = 0;
				for (i = 0; i < Mod.nArc; i++)
				{
					Archive[i].probability = 1.0 / density[Archive[i].nGrid];
					max += Archive[i].probability;
				}

				for (i = 0; i < Mod.nArc; i++)
				{
					Archive[i].probability /= max;
					if (i != 0)
						Archive[i].probability += Archive[i - 1].probability;
				}

				// Calculate Particle velocities	
				double W = 1,
					c1 = 1,
					c2 = 1;

				for (i = 0; i < Mod.SqzOpt.nPopSize; i++)
				{
					// Select a leader per particle with the Roulette Wheel Tournament
					double r = randval(0.0, 1.0);

					int k = 0;
					while (Archive[k].probability < r) k++;

					if (k == 0) k = 1; // Just for the case where the first particle in archive is chosen

					for (j = 0; j < Mod.SqzOpt.nPar; j++)
					{
						// From Reyes and Coello, 2005
						W = randval(0.1, 0.5);
						c1 = randval(1.5, 2);
						c2 = randval(1.5, 2);
						r1 = randval(0, 1);
						r2 = randval(0, 1);

						vel[i][j] = W * vel[i][j] + c1 * r1 * (Pb[i].X[j] - P[i].X[j]) + c2 * r2 * (Archive[k - 1].X[j] - P[i].X[j]);

						double vMax = Mod.SqzOpt.uX[j] - Mod.SqzOpt.lX[j];

						if (fabs(vel[i][j]) >= vMax)
						{
							if (vel[i][j] > 0) vel[i][j] = vMax;

							if (vel[i][j] <= 0) vel[i][j] = -vMax;
						}
					}
				}
			}
			catch (Exception& e)
			{
				cout << e.what() << endl;
			}
		}
		catch (Exception& e)
		{
			cout << e.what() << endl;
		}

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar) * 100 / double(Mod.SqzOpt.nGen - 1)));
		ProBar.ShowWindow(true);
		++ProBar.iProBar;

		fflush(filePtrGen);
		fflush(filePtrPar);
		fflush(filePtrArc);
	}

	ProBar.ShowWindow(false);

	fclose(filePtrGen);
	fclose(filePtrPar);
	fclose(filePtrArc);

}

void PSOParallel::SetRunMOPSO(string out, string path)
{
	try
	{
		_chdir(path.c_str());

		string input = "inputOpt.dat";

		outputFile = out;
		RunParallelMOPSO(input.c_str());

	}
	catch (Exception& e)
	{
		cout << e.what();
	}
}

//----------------------------------------------------------------------------------------
bool PSOParallel::IsDominant(const Particle& P1, const Particle& P2, int nObjs)
{
	int i = 0;
	bool check = true;

	while (i < nObjs && check)
	{
		if (P2.Objs[i] < P1.Objs[i]) check = false;

		i++;
	}

	return check;
}
//----------------------------------------------------------------------------------------
void PSOParallel::EquateParticle(Particle& P1, const Particle& P2, int nPar, int nObjs)
{
	int j;

	P1.nGrid = P2.nGrid;
	P1.density = P2.density;

	for (j = 0; j < nObjs; j++)
	{
		P1.Objs[j] = P2.Objs[j];
		P1.CObjs[j] = P2.CObjs[j];
	}

	for (j = 0; j < nPar; j++)
		P1.X[j] = P2.X[j];
}
//----------------------------------------------------------------------------------------
void PSOParallel::EquateSqzOpt(SqzOptimisation& P1, const SqzOptimisation& P2)
{
	int j;

	for (j = 0; j < P1.nPar; j++)
	{
		P1.X[j] = P2.X[j];
		P1.Vol[j] = P2.Vol[j];
	}

	P1.Obj_T_V = P2.Obj_T_V;
}
//----------------------------------------------------------------------------------------

void PSOParallel::ParetoFront(int nP, int nData)
{
	size_t err;
	FILE* fP;
	bool dom = true, cond1, cond2;
	int i = 0,
		j = 0;

	double	Obj1,
		TObj1,
		Obj2,
		TObj2,
		* RealObj;

	vector<Particle*> P(nP);

	for (i = 0; i < nP; i++)
	{
		P[i]->X.resize(nData);
	}

	RealObj = new double[nP];

	string tempLfname;
	tempLfname = outputFile;
	size_t n = tempLfname.find_last_of(".");
	if (n < string::npos)
		tempLfname = tempLfname.substr(0, n - strlen("PSO_Results") + 1);
	else
		tempLfname = tempLfname;

	tempLfname += "ParetoFront.txt";

	// This file will store all the Pareto Front 
	if ((err = fopen_s(&fP, outputFile.c_str(), "r")) != 0)
		printf("Error opening file %s for writing\r\n", outputFile);
	else
	{
		printf("File %s was opened for writing\r\n", outputFile);

		const size_t BUFFSIZE = 4096;
		char buffer[BUFFSIZE];

		fseek(fP, 0L, SEEK_SET);		// set pointer to beginning of file
		fgets(buffer, BUFFSIZE, fP);	// skip one line of header

		for (i = 0; i < nP; i++)
		{
			int temp;

			fscanf_s(fP, "%d %d", &temp, &temp);

			for (j = 0; j < nData; j++)
				fscanf_s(fP, "%lf ", &P[i]->X[j]);

			fscanf_s(fP, "%lf %lf", &P[i]->Obj, &RealObj[i]);
		}

		fclose(fP);
	}

	// This file will store all the Pareto Front
	if ((err = fopen_s(&fP, tempLfname.c_str(), "w")) != 0)
		printf("Error opening file %s for writing\r\n", tempLfname);
	else
	{
		printf("File %s was opened for writing\r\n", tempLfname);

		for (i = 0; i < nData; i++)
		{
			const size_t buffsize = 10;
			char ind[buffsize];
			sprintf_s(ind, buffsize, "%d ", i);
			string tind(ind);
			string s;
			s = "X" + tind;
			fprintf_s(fP, "%11s", s.c_str());
		}

		fprintf_s(fP, "%11s %11s\n", "OptObj", "RealObj");

		// i holds member to check if i dominates j
		for (j = 0; j < nP; j++)
		{
			Obj1 = P[j]->X[0] + P[j]->X[2];
			Obj2 = P[j]->Obj;

			i = 0;
			dom = true;

			while (i < nP && dom)
			{
				if (i != j)
				{
					TObj1 = P[i]->X[0] + P[i]->X[2];
					TObj2 = P[i]->Obj;

					cond1 = true;
					cond2 = true;

					if (Obj1 > TObj1)

						cond1 = false;

					if (Obj2 > TObj2)

						cond2 = false;

					if (cond1 || cond2) dom = true;

					else dom = false;

				}

				i++;
			}

			if (dom)
			{
				fprintf_s(fP, "%11.4f %11.4f", P[j]->X[0] + P[j]->X[2], P[j]->Obj);
				for (int n = 0; n < nData; n++)
					fprintf_s(fP, "%11.4f ", P[j]->X[n]);
				fprintf_s(fP, "%11.4f %11.4f\n", P[j]->Obj, RealObj[j]);
				fflush(fP);
			}
		}

		fclose(fP);
	}

	for (i = 0; i < nP; i++)
		delete P[i];

	delete[] RealObj;
}

void PSOParallel::PrintResults(FILE* file, const vector<Particle>& P, string st, int nG, int nR, Model& Mod)
{
	int i, j;

	// Print Headers
	if (nG == -1)
	{
		fprintf_s(file, "%s\n", st.c_str());
		fprintf_s(file, "%4s %4s ", "#Gen", "nPar");
		for (i = 0; i < Mod.SqzOpt.nPar; i++)
		{
			const size_t buffsize = 10;
			char ind[buffsize];
			sprintf_s(ind, buffsize, "%d ", i);
			string tind(ind);
			string s;
			s = "X" + tind;
			fprintf_s(file, "%11s", s.c_str());
		}

		for (i = 0; i < Mod.nObjs; i++)
		{
			const size_t buffsize = 10;
			char ind[buffsize];
			sprintf_s(ind, buffsize, "%d ", i);
			string tind(ind);
			string s;
			s = "Objs" + tind;
			fprintf_s(file, "%11s", s.c_str());
		}

		fprintf_s(file, "%11s %11s\n", "nGrid", "Density");
	}
	else
	{
		for (i = 0; i < nR; i++)
		{
			fprintf_s(file, "%4d %4d ", nG, i);
			for (j = 0; j < Mod.SqzOpt.nPar; j++)
				fprintf_s(file, "%11.6f", P[i].X[j]);

			for (j = 0; j < Mod.nObjs; j++)
			{
				fprintf_s(file, "%11.6f", P[i].Objs[j]);
			}

			fprintf_s(file, " %11d %11d\n", P[i].nGrid, P[i].density);
		}
	}
}
