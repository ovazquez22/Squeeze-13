// PSO.cpp: implementation of the PSO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PSO.h"
#include "Population.h"
#include "Chromosome.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

PSO::PSO()
{	
	EQ = false;

	nP = 10;
	popsize = 10;
	nPoint = 10;
	MaxGen= 500;
	TrainPer = 1.0;	// Percentage of profile for misfiot calculation
	nStages  = 1;

//	srand( 0.08 );
	c1 = c2 = 1;
	stages = new int [nStages];
}

PSO::~PSO()
{
	delete [] stages;
}
void PSO::CreateDlg(CWnd* pParent)
{
	ProBar.Create(IDD_PROGRESS_BAR, pParent);
}

void PSO::RunTestPSO(CRadial &RadModel, FILE *fRes)
{	
	int i,j;
	double sum = 0;
	double **vel;
	double **Pi;
	double r1, r2;
	Chromosome Pg;
	
	nPoint = 10;
	
	Population pop, tp;
	
	nPoint = 1;
	
	pop.reSize(popsize,1,nP,nPoint);
	pop.nGen = MaxGen;
	
	Pg.reSize(1,nP,nPoint);

	tp.reSize(popsize,1,nP,nPoint);
	tp.nGen = MaxGen;

	vel = new  double *[popsize];
	Pi = new  double *[popsize];

	for ( i = 0 ; i < popsize ; i++ )
	{
		vel[i] = new double [nP];
		Pi[i] = new double [nP];
	}

	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{	
			pop.chromosome[i].lX[j] = -100;
			pop.chromosome[i].uX[j] = 100;
			pop.chromosome[i].Iso = false;
		}
	}	

	// Initial Population
	// false to real code
	pop.initial_gen(false, RadModel);
	
	// Initialise Pi
	for ( i = 0 ; i < popsize ; i++ )
		
		for ( j = 0 ; j < nP ; j++ )
		
			pop.chromosome[i].Pi[j] = pop.chromosome[i].X[j];

	// Evaluate Initial Population
	pop.SetFitnessSphere();
	
	// Find Pg in initial Population
	pop.FindPg();
	 
	// Randomized Particle Velocities
	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{	
			Pi[i][j] = pop.chromosome[i].X[j];

			vel[i][j] = 0;

			if ( pop.randval(0,1) > 0.5)
				
				 vel[i][j] = pop.randval(0,0.5) * pop.chromosome[i].X[j];

			else

				vel[i][j] = -pop.randval(0,0.5) * pop.chromosome[i].X[j];
		}
	}

	fprintf(fRes,"Initialised\n	");
	pop.printPopulation(false, fRes);
	
	for ( int g = 1 ; g <= pop.nGen ; g++ )
	{	
		// Generate New Populetion
		for ( i = 0 ; i < popsize ; i++ )
		{	
			for ( j = 0 ; j < nP ; j++ )
			{	
				tp.chromosome[i].X[j] = pop.chromosome[i].X[j] + vel[i][j];
			}
		}

		// Evaluate New Population
		tp.SetFitnessSphere();
		
		tp.FindPg();
		// Update Pg
		if ( tp.Pg.Train < pop.Pg.Train )
			pop.Pg = tp.Pg;

		// Upadte Pi
		for ( i = 0 ; i < popsize ; i++ )
		{
			if ( tp.chromosome[i].Train < pop.chromosome[i].Train )
			{	
				for ( j = 0 ; j < nP ; j++ )
				{			
					Pi[i][j] = tp.chromosome[i].X[j];
				}
			}
		}
		
		// Calculate Particle velocities		
		for ( i = 0 ; i < popsize ; i++ )
		{
			for ( j = 0 ; j < nP ; j++ )
			{	
				r1 = tp.randval(0,1);
				r2 = tp.randval(0,1);
				vel[i][j] += c1*r1*( Pi[i][j] - tp.chromosome[i].X[j] ) + c2*r2*( pop.Pg.X[j] - tp.chromosome[i].X[j] );
			}
		}
		
		pop = tp;

		fprintf(fRes,"Generation %d,\n", g);
		pop.printPopulation(false, fRes);
	}

	for (i = 0; i < popsize; i++)
	{
		delete[] vel[i];
		delete[] Pi[i];
	}

	delete[] vel;
	delete[] Pi;
}


void PSO::RunPSO(CRadial &RadMod, FILE *fGA, FILE *fGARes, double *U, double *L)
{	
	int i,j, lchrom =1, l;
	double sum = 0, MaxVel = 0;
	double **vel;
	double **Pi;
	double r1, r2;
	
	srand( 0.01 );
	srand( (unsigned)time( NULL ) );
	RadModel = RadMod;	
	
	nPoint = RadModel.Model.nPts;
	
	if ( RadModel.UnitSpec == 'm' || RadModel.UnitSpec == 'M')	// Metric

		Units = 1; 
	
	if ( RadModel.UnitSpec == 'c' || RadModel.UnitSpec == 'C')	// Conventional 

		Units = 0;
	
	// Kinetic
	nP = 3;

	for ( int l1 = 0 ; l1<RadModel.GetnLayers() ; l1++ )
	{	
		// = 0 Equilibrium
		if (RadModel.wIso[l1][0].TypeAds == 0 ) nP = 2;
	}

	Population tp;
	
	for ( i = 0 ; i < nPoint ; i++ )
	{	
		pop.fx[i] = RadModel.Model.fx[i];
		pop.fy[i] = RadModel.Model.fy[i];
	}

	pop.nGen = MaxGen;
	
	tp.reSize(popsize,lchrom,nP,nPoint);
	tp.nGen = MaxGen;
	
	vel = new  double *[popsize];
	Pi = new  double *[popsize];

	for ( i = 0 ; i < popsize ; i++ )
	{
		vel[i] = new double [nP];
		Pi[i] = new double [nP];
	}

	for ( i = 0 ; i < popsize ; i++ )
	{
		pop.chromosome[i].uX[0] = U[0];
		pop.chromosome[i].lX[0] = L[0];
		
		pop.chromosome[i].uX[1] = U[1];
		pop.chromosome[i].lX[1] = L[1];
				
		if (pop.nP == 3 )
		{
			pop.chromosome[i].uX[2] = U[2];
			pop.chromosome[i].lX[2] = L[2];			
		}

		pop.chromosome[i].Iso = true;

		pop.chromosome[i].TrainPer = TrainPer;
		pop.chromosome[i].type = type;
	}	

	
	// Random Initial Population
	if (HC == 0 )
	{
		for ( i = 0 ; i < popsize ; i++ )
		{	
			for ( j = 0 ; j < nP ; j++ )
			{
				pop.chromosome[i].X[j] = (pop.chromosome[i].uX[j] - pop.chromosome[i].lX[j]) * pop.randval(0,1) + pop.chromosome[i].lX[j];
			}
		}
	}

	if ( RadModel.wIso[0][0].IsoTyp == 1 ) // Freundlich Iostherm 
	{
		pop.chromosome[0].X[0] = RadModel.wIso[0][0].k;
		pop.chromosome[0].X[1] = RadModel.wIso[0][0].n;
	}
	
	if ( RadModel.wIso[0][0].IsoTyp == 2 ) // Langmuir Iostherm 
	{
		pop.chromosome[0].X[0] = RadModel.wIso[0][0].k;
		pop.chromosome[0].X[1] = RadModel.wIso[0][0].n;
	}

	if (nP == 3)
		pop.chromosome[0].X[2] = RadModel.wIso[0][0].r2;

	// Initialise Pi
	for ( i = 0 ; i < popsize ; i++ )
		
		for ( j = 0 ; j < nP ; j++ )
		
			pop.chromosome[i].Pi[j] = pop.chromosome[i].X[j];
	
	// Evaluate Initial Population
	if (HC == 0 )
		
		pop.EvaluatePopulation(RadModel);

	// Find Pg in initial Population
	pop.FindPg();
	 
	// Randomized Particle Velocities
	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{	
			Pi[i][j] = pop.chromosome[i].X[j];

			vel[i][j] = 0;

			if ( pop.randval(0,1) > 0.5)
				
				 vel[i][j] = pop.randval(0,0.5) * pop.chromosome[i].X[j];

			else

				vel[i][j] = -pop.randval(0,0.5) * pop.chromosome[i].X[j];
		}
	}

	printf("Initialised\n");
//	pop.printPopulation(false, fRes);
	
	fprintf(fGA,"%12s %12s %12s %12s %12s ", " "," "," "," ","SolTime/Vol");
	for ( i = 0 ; i < nPoint ; i++ )
		fprintf(fGA," %12.3f",RadModel.Model.fx[i]);
	fprintf(fGA,"\n");
	fprintf(fGA,"%12s %12s %12s %12s %12s ", " "," "," "," ","Target_Prof");
	for ( i = 0 ; i < nPoint ; i++ )
		fprintf(fGA," %12.3f",RadModel.Model.fy[i]);
	fprintf(fGA,"\n");
	
	pop.printResults(fGA, fGARes, 0);
	
	ProBar.iProBar = 1;
	ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar)*100/double(pop.nGen)));

	for ( int g = 1 ; g <= pop.nGen ; g++ )
	{	
		tp = pop;

		// Generate New Populetion
		for ( i = 0 ; i < popsize ; i++ )
		{	
			for ( j = 0 ; j < nP ; j++ )
			{	
				tp.chromosome[i].X[j] = pop.chromosome[i].X[j] + vel[i][j];
			}

			// DAMPING BOUNDARY CONDITION
			for ( j = 0 ; j < nP ; j++ )
			{	
				// The coordinate j is reset to the boundary, then the velocity sign is reversed
				// and multiplied by random between [0,1]
				if ( tp.chromosome[i].X[j] > tp.chromosome[i].uX[j] ) 
				
					tp.chromosome[i].X[j] = tp.chromosome[i].uX[j] - tp.randval(0,1) * vel[i][j];

				if ( tp.chromosome[i].X[j] < tp.chromosome[i].lX[j] ) 
				
					tp.chromosome[i].X[j] = tp.chromosome[i].lX[j] - tp.randval(0,1) * vel[i][j];
				
			}
		}
				
		// Evaluate New Population
		tp.EvaluatePopulation(RadModel);;
		
		tp.FindPg();
		// Update Pg
		if ( tp.Pg.Train < pop.Pg.Train )
			pop.Pg = tp.Pg;

		// Upadte Pi
		for ( i = 0 ; i < popsize ; i++ )
		{
			if ( tp.chromosome[i].Train < pop.chromosome[i].Train )
			{	
				for ( j = 0 ; j < nP ; j++ )
				{			
					Pi[i][j] = tp.chromosome[i].X[j];
				}
			}
		}
		
		// Calculate Particle velocities		
		for ( i = 0 ; i < popsize ; i++ )
		{
			for ( j = 0 ; j < nP ; j++ )
			{	
				r1 = tp.randval(0,1);
				r2 = tp.randval(0,1);
				vel[i][j] += c1*r1*( Pi[i][j] - tp.chromosome[i].X[j] ) + c2*r2*( pop.Pg.X[j] - tp.chromosome[i].X[j] );
				
				double vMax = tp.chromosome[i].uX[j] - tp.chromosome[i].lX[j];
				
				if ( fabs( vel[i][j] ) >= vMax )
				{
					if ( vel[i][j] > 0 ) vel[i][j] = vMax;
				
					if ( vel[i][j] <= 0 ) vel[i][j] = -vMax;
				}
			}
		}
		
		pop = tp;
	//	fprintf(fRes,"Generation %d,\n", g);
	//	pop.printPopulation(false, fRes);
		pop.printResults(fGA, fGARes, g);

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar)*100/double(pop.nGen)));
		
		ProcessMessageLoop();

		++ProBar.iProBar;
	}
	

	CString sTemp = RadModel.Model.ReadFileName;
	char st[1024];
	
	// Remove GA input file
	strcpy(st,".\\Exec\\ga");
	strcat(st,RadModel.Model.ReadFileName.GetBuffer(0));
	remove(st);

	// Remove GA output file
	strcpy(st,".\\Exec\\ga");
	sTemp.Replace(_T("DAT"),_T("out"));
	sTemp.Replace(_T("DAT"),_T("out"));
	strcat(st,sTemp.GetBuffer(0));
	remove(st);

	for (i = 0; i < popsize; i++)
	{
		delete[] vel[i];
		delete[] Pi[i];
	}

	delete[] vel;
	delete[] Pi;
	
}

void PSO::RunTracerPSO(CRadial &RadMod, FILE *fGA, FILE *fGARes, double *U, double *L)
{	
	int i,j, lchrom =1, s,l, nStg;
	double sum = 0, MaxVel = 0;
	double **vel;
	double **Pi;
	double r1, r2;
	double sumX[3];
	double *PIDf;

	srand( 0.08 );
	RadModel = RadMod;	
	
	nPoint = RadModel.Model.nPts;
	
	nStg = 0;
	for ( s = 0 ; s < RadModel.nStages ; s++ )
		nStg += stages[s];
	
	nP = Mod.nLay*nStg;

	PIDf = new double [nP];

	if ( RadModel.UnitSpec == 'm' || RadModel.UnitSpec == 'M')	// Metric

		Units = 1; 
	
	if ( RadModel.UnitSpec == 'c' || RadModel.UnitSpec == 'C')	// Conventional 

		Units = 0;
	
	pop.reSize(popsize,lchrom,nP, RadModel.Model.nPts);

	Population tp;
	
	for ( i = 0 ; i < nPoint ; i++ )
	{	
		pop.fx[i] = RadModel.Model.fx[i];
		pop.fy[i] = RadModel.Model.fy[i];
	}
	
	// Get the PIDf from the data file
	int temp = 0;
			
	for (int s = 0 ; s < RadModel.nStages ; s++ )
	{
		if ( stages[s] == 1 ) // Stage included
		{		
			for (l = 0 ; l < Mod.nLay ; l++ )
			{	
				PIDf[Mod.nLay*temp + l] = RadModel.stage[s].PIDF[l];
				
			}

			temp++;
		}
	}

	pop.nGen = MaxGen;
	
	tp.reSize(popsize,lchrom,nP,nPoint);
	tp.nGen = MaxGen;
	
	vel = new  double *[popsize];
	Pi = new  double *[popsize];

	for ( i = 0 ; i < popsize ; i++ )
	{
		vel[i] = new double [nP];
		Pi[i] = new double [nP];
	}

	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{
			pop.chromosome[i].uX[j] = 10.0;
			pop.chromosome[i].lX[j] = 0.1;
		}
		
		pop.chromosome[i].Iso = true;

		pop.chromosome[i].TrainPer = TrainPer;
		pop.chromosome[i].type = type;
	}	

	
	// Random Initial Population
	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{	
			int temp = 0;
			
			for (int s = 0 ; s < RadModel.nStages ; s++ )
			{
				if ( stages[s] == 1 )
				{		
					for (l = 0 ; l < Mod.nLay ; l++ )
					{	
						if ( i == 0 )
						{
							pop.chromosome[i].X[Mod.nLay*temp + l] = RadModel.stage[s].PIDF[l];
						}
						else
						{
							pop.chromosome[i].X[Mod.nLay*temp + l] = RadModel.stage[s].PIDF[l]* pop.randval(0,1);
						}
					}

					temp++;
				}
			}
		}

		// Normalised X for 14 layers and three stages
		for (j = 0 ; j < nStg ; j++)
		{	
			sumX[j] = 0;
			for (l = 0 ; l < Mod.nLay ; l++ )
			{
				sumX[j] += pop.chromosome[i].X[Mod.nLay*j + l];
			}
		}

		for (j = 0 ; j < nStg ; j++)		
			for (l = 0 ; l < Mod.nLay ; l++ )
				pop.chromosome[i].X[Mod.nLay*j + l] /= sumX[j];
	}
	
	// Initialise Pi
	for ( i = 0 ; i < popsize ; i++ )
		
		for ( j = 0 ; j < nP ; j++ )
		
			pop.chromosome[i].Pi[j] = pop.chromosome[i].X[j];
	
	pop.EvaluatePopulation(RadModel, stages);

	// Find Pg in initial Population
	pop.FindPg();
	 
	// Randomized Particle Velocities
	for ( i = 0 ; i < popsize ; i++ )
	{	
		for ( j = 0 ; j < nP ; j++ )
		{	
			Pi[i][j] = pop.chromosome[i].X[j];

			vel[i][j] = 0;

			if ( pop.randval(0,1) > 0.5)
				
				 vel[i][j] = pop.randval(0,0.5) * pop.chromosome[i].X[j];

			else

				vel[i][j] = -pop.randval(0,0.5) * pop.chromosome[i].X[j];
		}
	}

	printf("Initialised\n");
//	pop.printPopulation(false, fRes);
	int temp2 = Mod.nLay*nStg;
	for ( i = 0 ; i < temp2 ; i++)
		fprintf(fGA,"%12s ", " ");
	fprintf(fGA,"%12s %12s "," ","SolTime/Vol");
	for ( i = 0 ; i < nPoint ; i++ )
		fprintf(fGA," %12.3f",RadModel.Model.fx[i]);
	fprintf(fGA,"\n");
	for ( i = 0 ; i < temp2 ; i++)
		fprintf(fGA,"%12s ", " ");
	fprintf(fGA,"%12s %12s "," ","Target_Prof");
	for ( i = 0 ; i < nPoint ; i++ )
		fprintf(fGA," %12.3f",RadModel.Model.fy[i]);
	fprintf(fGA,"\n");
	
	pop.printResults(fGA, fGARes, 0, RadModel.nStages, Mod.nLay, stages);
	
	ProBar.iProBar = 1;
	ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar)*100/double(pop.nGen)));

	for ( int g = 1 ; g <= pop.nGen ; g++ )
	{	
		tp = pop;

		// Generate New Populetion
		for ( i = 0 ; i < popsize ; i++ )
		{	
			for ( j = 0 ; j < nP ; j++ )
			{	
				tp.chromosome[i].X[j] = pop.chromosome[i].X[j] + vel[i][j];
			}

			// DAMPING BOUNDARY CONDITION
			for ( j = 0 ; j < nP ; j++ )
			{	
				// The coordinate j is reset to the boundary, then the velocity sign is reversed
				// and multiplied by random between [0,1]
				if ( tp.chromosome[i].X[j] > tp.chromosome[i].uX[j] ) 
				
					tp.chromosome[i].X[j] = tp.chromosome[i].uX[j] - tp.randval(0,1) * vel[i][j];

				if ( tp.chromosome[i].X[j] < tp.chromosome[i].lX[j] ) 
				
					tp.chromosome[i].X[j] = tp.chromosome[i].lX[j] - tp.randval(0,1) * vel[i][j];
				
			}
			
			for (j = 0 ; j < nStg ; j++)
			{	
				sumX[j] = 0;
				for (l = 0 ; l < Mod.nLay ; l++ )
				{	
					tp.chromosome[i].X[Mod.nLay*j + l] *= PIDf[l];
					sumX[j] += tp.chromosome[i].X[Mod.nLay*j + l];
				}
			}

			for (j = 0 ; j < nStg ; j++)		
				for (l = 0 ; l < Mod.nLay ; l++ )
					tp.chromosome[i].X[Mod.nLay*j + l] /= sumX[j];
		}
		
		// Evaluate New Population
 		tp.EvaluatePopulation(RadModel, stages);
		
		tp.FindPg();
		// Update Pg
		if ( tp.Pg.Train < pop.Pg.Train )
			pop.Pg = tp.Pg;

		// Upadte Pi
		for ( i = 0 ; i < popsize ; i++ )
		{
			if ( tp.chromosome[i].Train < pop.chromosome[i].Train )
			{	
				for ( j = 0 ; j < nP ; j++ )
				{			
					Pi[i][j] = tp.chromosome[i].X[j];
				}
			}
		}
		
		// Calculate Particle velocities		
		for ( i = 0 ; i < popsize ; i++ )
		{
			for ( j = 0 ; j < nP ; j++ )
			{	
				r1 = tp.randval(0,1);
				r2 = tp.randval(0,1);
				vel[i][j] += c1*r1*( Pi[i][j] - tp.chromosome[i].X[j] ) + c2*r2*( pop.Pg.X[j] - tp.chromosome[i].X[j] );
				
				double vMax = tp.chromosome[i].uX[j] - tp.chromosome[i].lX[j];
				
				if ( fabs( vel[i][j] ) >= vMax )
				{
					if ( vel[i][j] > 0 ) vel[i][j] = vMax;
				
					if ( vel[i][j] <= 0 ) vel[i][j] = -vMax;
				}
			}
		}
		
		pop = tp;
	//	fprintf(fRes,"Generation %d,\n", g);
	//	pop.printPopulation(false, fRes);
		pop.printResults(fGA, fGARes, g, RadModel.nStages, Mod.nLay, stages);

		ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar)*100/double(pop.nGen)));
		
		ProcessMessageLoop();

		++ProBar.iProBar;
	}
	

	CString sTemp = RadModel.Model.ReadFileName;
	char st[1024];
	
	// Remove GA input file
	strcpy(st,".\\Exec\\ga");
	strcat(st,RadModel.Model.ReadFileName.GetBuffer(0));
//	remove(st);

	// Remove GA output file
	strcpy(st,".\\Exec\\ga");
	sTemp.Replace(_T("DAT"),_T("out"));
	sTemp.Replace(_T("DAT"),_T("out"));
	strcat(st,sTemp.GetBuffer(0));
//	remove(st);

	// Remove GA temp output file
	strcpy(st,".\\Exec\\temp");
	sTemp.Replace(_T("DAT"),_T("out"));
	sTemp.Replace(_T("DAT"),_T("out"));
	strcat(st,sTemp.GetBuffer(0));
//	remove(st);
	
	for (i = 0; i < popsize; i++)
	{
		delete[] vel[i];
		delete[] Pi[i];
	}

	delete[] vel;
	delete[] Pi;
}

int PSO::ProcessMessageLoop() {

	MSG msg;
	while ( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE) ) {

		if (WM_QUIT == msg.message) {
			// Abort all processing
			::PostQuitMessage(0);
			break;
			}
			
//		::IsDialogMessage(m_hWnd, &msg);

		if (!AfxGetApp()->PreTranslateMessage(&msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			}
		} 
/*
		if (!::IsDialogMessage(m_hWnd, &msg)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			}
		} 
*/
	// let MFC do its idle processing
	AfxGetApp()->OnIdle (0);
	AfxGetApp()->OnIdle (1);
	// Perform some background processing here 
	// using another call to OnIdle
		
	return 0;
}
  