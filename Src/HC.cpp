// HC.cpp: implementation of the HC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "HC.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HC::HC()
{
	nP = 5;
	TrainPer = 1.0;	// Percentage of profile for misfiot calculation
	Runs = 1;
//	srand( 0.08 );

	kMax = 3000;
	kMin = 1;
	
	nMax = 1.0;
	nMin = 0.0;
	
	rMax = 4.0;
	rMin = 0.001;

	RandHC = 1;
}

HC::~HC()
{

}

void HC::CreateDlg(CWnd* pParent)
{
	ProBar.Create(IDD_PROGRESS_BAR, pParent);
}

void HC::RunHC(CRadial &RadMod, const char *pszPathName, int U, int run, double kSeed, double nSeed, double rSeed, FILE *fBest)
{	
	int i;
	double k, knew, kBest,
			 n, nnew, nBest,
			 r, rnew, rBest,
			 Train = 0, TrainNew = 0, TrainBest = 0,
			 Eval = 0, EvalNew = 0, EvalBest = 0,
			 *watConcBest;
	
	CString sn, sn1, sn2, sn3, stemp, stemp1;
	FILE *fResult, *fProf, *fAll;

	Units = U;
	
	if (run == 0 ) RadModel = RadMod;

	nPoint = RadModel.Model.nPts;

	watConcBest = new double [nPoint];
	
	sn = pszPathName;

	sn3.Format("Run%d",run);
	
	stemp.Format("HCProf%s.txt",sn3);

	sn.Replace(".dat",stemp);
	sn.Replace(".DAT",stemp);

	sn1 = sn;
	stemp1.Format("HCRes%s.txt",sn3);
	
	sn1.Replace(stemp,stemp1);

	if ( (fProf = fopen( sn,"w")) == NULL )
	{
		AfxMessageBox("File not open to write");
	}
	
	if ( (fResult = fopen( sn1,"w")) == NULL )
	{
		AfxMessageBox("File not open to write");
	}
	
	stemp.Format("HCAll%s.txt",sn3);
	sn1.Replace(stemp1, stemp);
	if ( (fAll = fopen( sn1,"w")) == NULL )
	{
		AfxMessageBox("File not open to write");
	}

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
	
	
	bool Fail; // True = The isotherm combination did not converge, False = Valid isotherm combination 
	
	fprintf(fProf,"%12s %12s %12s %12s %12s","", "", "", "", "SolTime/Vol" );
	if (run == 0 ) fprintf(fBest,"%5s %12s %12s %12s %12s %12s %12s","","","", "", "", "", "SolTime/Vol" );
	
	for ( i = 0 ; i < RadModel.Model.nPts; i++ )
	{	
		fprintf(fProf,"%12.3f ", RadModel.Model.fx[i] );
		if (run == 0 ) fprintf(fBest,"%12.3f ", RadModel.Model.fx[i] );
	}
	
	fprintf(fProf,"\n");
	if (run == 0 ) fprintf(fBest,"\n");

	watConc = new double [nPoint];
	TimeVol = new double [nPoint];

	for ( i =0 ; i<RadModel.Model.nPts; i++ )
	{
		watConc[i] = 0;
		TimeVol[i] = 0;
	}
	
	// Seed taken from the input file
	if ( RandHC == 1 )
	{
		k = kSeed;
		n = nSeed;
		r = rSeed;
	}	

	// Random Seed
	if ( RandHC == 0 )
	{
		srand( (unsigned)time( NULL ) );

		k = (kMax - kMin ) * (double)rand()/RAND_MAX + kMin;
		n = (nMax - nMin ) * (double)rand()/RAND_MAX + nMin;
		r = (rMax - rMin ) * (double)rand()/RAND_MAX + rMin;
	}

	evaluate(RadModel,k,n,r);
	
	Fail = Misfit(type, TrainPer, Train, Eval);
	
	for ( i =0 ; i<nPoint; i++ )
		watConcBest[i] = watConc[i];
	
	EvalBest = Eval;
	kBest = k;
	nBest = n;
	rBest = r;
	if( TrainPer < 1.0 )
	{	
		EvalBest = Eval;
		TrainBest = Train;
	}
	else // 100 % Train
	{
		TrainBest = Train;
	}

	if ( Fail ) // The Isotherm combination did not converge
		 
		Train = 1e20;
			
	fprintf(fProf,"%12s %12s %12s %12s %12s","", "", "", "", "SolProf" );
	
	if (run == 0 ) fprintf(fBest,"%5s %12s %12s %12s %12s %12s %12s","","k", "n", "r", "Train", "Eval", "SolProf");

	for ( i = 0 ; i<RadModel.Model.nPts; i++ )
	{
		fprintf(fProf,"%12.3f ", RadModel.Model.fy[i] );
		if (run == 0 ) fprintf(fBest,"%12.3f ", RadModel.Model.fy[i] );
	}

	fprintf(fProf,"\n");
	if (run == 0 ) fprintf(fBest,"\n");

	fprintf(fProf,"%12s %12s %12s %12s %12s","", "", "", "", "SolTime/Vol" );
	
	for ( i =0 ; i<nPoint; i++ )
	{			
		fprintf(fProf,"%12.3f ", TimeVol[i] );
	}
	
	fprintf(fProf,"\n");
	
	int it = 0, is = 1;

	fprintf(fProf,"%12s %12s %12s %12s %12s\n","k", "n", "r", "Train", "Eval" );
	fprintf(fProf,"%12.3f %12.3f %12.6f %12.6f %12.6f", k, n, r, Train, Eval );
	for ( i =0 ; i<nPoint; i++ )
		fprintf(fProf,"%12.3f ", watConc[i] );				
	fprintf(fProf, "\n");

	fprintf(fResult,"%5s %5s %12s %12s %12s %12s %12s\n","Samp","Iter", "k", "n", "r", "Train", "Eval" );
	fprintf(fResult,"%5d %5d %12.3f %12.3f %12.6f %12.3f %12.3f",is,it, k, n, r, Train, Eval );
	fprintf(fResult, "\n");
	
	fprintf(fAll,"%5s %5s %12s %12s %12s %12s %12s\n","Samp","Iter", "k", "n", "r", "Train", "Eval" );
	if ( Train > 1000) 
		fprintf(fAll,"%5d %5d %12.3f %12.3f %12.6f %.3e %12.3f",is,it, k, n, r, Train, Eval );
	else
		fprintf(fAll,"%5d %5d %12.3f %12.3f %12.6f %12.3f %12.3f",is,it, k, n, r, Train, Eval );
	fprintf(fAll, "\n");

	while ( it <= MaxGen )
	{	
		// Hill climber implementation 
		// Try a new case based on the previous run		
		// until Fnew(new misfit) is better than the (old F(misfit)
		TrainNew = 1e200;
		while( TrainNew >= Train && it <= MaxGen )
		{	
			knew = -1;
			while( knew < kMin || knew > kMax)
			{
				knew = k + Gauss(1,100);
			}
			
			nnew  = -1;
			while( nnew < nMin || nnew > nMax)
			{
				nnew = n + Gauss(1,0.5);
			}
			
			
			if ( RadModel.wIso[0][0].TypeAds ==  0 ) // Equilibrium
				
				rnew = 0.0;
			else
			{
				rnew  = -1;
				while( rnew < rMin || rnew > rMax)
				{
					rnew = r + Gauss(0,0.5);
				}
			}

			evaluate(RadModel,knew,nnew,rnew);
			
			Fail = Misfit(type, TrainPer, TrainNew, EvalNew);
		
			if ( Fail ) // The Isotherm combination did not converge
				 
				TrainNew = 1e20;
			
			it++;
			ProBar.m_ProgressBar.SetPos(int(double(ProBar.iProBar)*100/double(Runs*MaxGen)));
			ProcessMessageLoop();
			++ProBar.iProBar;
			
			if ( TrainNew > 1000) 
				fprintf(fAll,"%5d %5d %12.3f %12.3f %12.6f %9.3e %12.3f",is, it, knew, nnew, rnew, TrainNew, EvalNew );
			else
				fprintf(fAll,"%5d %5d %12.3f %12.3f %12.6f %12.3f %12.3f",is, it, knew, nnew, rnew, TrainNew, EvalNew );
			fprintf(fAll, "\n");
		}
		
		if ( Train >= TrainNew )
		{
			is++;
			fprintf(fResult,"%5d %5d %12.3f %12.3f %12.6f %12.3f %12.3f",is, it, knew, nnew, rnew, TrainNew, EvalNew );
			fprintf(fResult, "\n");
			
			// Get the best from Train and Eval
			if( TrainPer < 1.0 )
			{
				if ( EvalNew < EvalBest )
				{
					kBest = knew;
					nBest = nnew;
					rBest = rnew;

					EvalBest = EvalNew;
					TrainBest = TrainNew;
				}
			}
			else // 100 % Train
			{
				kBest = knew;
				nBest = nnew;
				rBest = rnew;
				TrainBest = TrainNew;
			}

			fprintf(fProf,"%12.3f %12.3f %12.6f %12.6f %12.6f", knew, nnew, rnew, TrainNew, EvalNew );
			for ( i =0 ; i<nPoint; i++ )
				fprintf(fProf,"%12.3f ", watConc[i] );				
			fprintf(fProf, "\n");
			
			for ( i =0 ; i<nPoint; i++ )
				watConcBest[i] = watConc[i];
		}	
		
		// Search for a new candiate in the updated seed
		k = knew;
		n = nnew;
		r = rnew;
		Train = TrainNew;
		Eval = EvalNew;
	}
	double temp;
	if ( TrainBest > 1e10 ) temp = -1;
	else temp = TrainBest;

	fprintf(fBest,"Run%2d %12.3f %12.3f %12.6f %12.3f %12.3f %12s", run, kBest, nBest, rBest, temp, EvalBest,"Prof" );
	for ( i =0 ; i<nPoint; i++ )
		fprintf(fBest,"%12.3f ", watConcBest[i] );				
	fprintf(fBest, "\n");
	
	delete [] watConc;
	delete [] watConcBest;
	delete [] TimeVol;
	
	fflush(fProf);
	fclose(fProf);
	fclose(fResult);
	fclose(fAll);

	if ( run == (Runs-1) )
	{		
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
	}		
}

void HC::InitPopHC(CRadial &RadMod, const char *pszPathName, int run, FILE *fBest, Chromosome &Best)
{	
	int i;
	double k, knew, kBest,
			 n, nnew, nBest,
			 r, rnew, rBest,
			 Train = 0, TrainNew = 0, TrainBest = 0,
			 Eval = 0, EvalNew = 0, EvalBest = 0;
	
	if (run == 0 ) RadModel = RadMod;

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
	
//	Best.reSize(30,nP,nPoint);

	bool Fail; // True = The isotherm combination did not converge, False = Valid isotherm combination 
		
	watConc = new double [nPoint];
	TimeVol = new double [nPoint];

	for ( i =0 ; i<RadModel.Model.nPts; i++ )
	{
		watConc[i] = 0;
		TimeVol[i] = 0;
	}
	
	// Random Seed
	k = (kMax - kMin ) * (double)rand()/RAND_MAX + kMin;
	n = (nMax - nMin ) * (double)rand()/RAND_MAX + nMin;
	r = (rMax - rMin ) * (double)rand()/RAND_MAX + rMin;

	evaluate(RadModel,k,n,r);
	
	Fail = Misfit(type, TrainPer, Train, Eval);
	
	if ( Fail ) // The Isotherm combination did not converge
	{	 
		Train = Eval = 1e20;
	
	}

	EvalBest = Eval;
	kBest = k;
	nBest = n;
	rBest = r;

	if( TrainPer < 1.0 )
	{	
		EvalBest = Eval;
		TrainBest = Train;
	}
	else // 100 % Train
	{
		TrainBest = Train;
	}

	int it = 0, is = 1;
	
	if (run == 0 )
	
		fprintf(fBest,"%12s %12s %12s %12s %12s\n","k", "n", "r", "Train", "Eval");

	while ( it <= MaxGen )
	{	
		// Hill climber implementation 
		// Try a new case based on the previous run		
		// until Fnew(new misfit) is better than the (old F(misfit)
		TrainNew = 1e200;
		while( TrainNew >= Train && it <= MaxGen )
		{	
			knew = -1;
			while( knew < kMin || knew > kMax)
			{
				knew = k + Gauss(1,100);
			}
			
			nnew  = -1;
			while( nnew < nMin || nnew > nMax)
			{
				nnew = n + Gauss(1,0.5);
			}
			
			
			if ( RadModel.wIso[0][0].TypeAds ==  0 ) // Equilibrium
				
				rnew = 0.0;
			else
			{
				rnew  = -1;
				while( rnew < rMin || rnew > rMax)
				{
					rnew = r + Gauss(0,0.5);
				}
			}

			evaluate(RadModel,knew,nnew,rnew);
			
			Fail = Misfit(type, TrainPer, TrainNew, EvalNew);
		
			if ( Fail ) // The Isotherm combination did not converge
			{ 
				TrainNew = EvalNew = 1e20;
			}

			it++;
		}
		
		if ( Train >= TrainNew )
		{
			is++;
			
			// Get the best from Train and Eval. This is waht it was done for the MSc thesis that is 
			// slighly wrong as only the Train partshould be used for the selection process
			if( TrainPer < 1.0 )
			{
				if ( EvalNew < EvalBest )
				{
					kBest = knew;
					nBest = nnew;
					rBest = rnew;

					EvalBest = EvalNew;
					TrainBest = TrainNew;
				}
			}
			else // 100 % Train
			{
				kBest = knew;
				nBest = nnew;
				rBest = rnew;
				TrainBest = TrainNew;
			}
			
			// Only using the training part for selection
			kBest = knew;
			nBest = nnew;
			rBest = rnew;

			EvalBest = EvalNew;
			TrainBest = TrainNew;

			for ( i =0 ; i<nPoint; i++ )
			{	
				Best.Prof[i] = watConc[i];
			}
		}
		
		// Search for a new candiate in the updated seed
		k = knew;
		n = nnew;
		r = rnew;
		Train = TrainNew;
		Eval = EvalNew;
	}
	
	if ( TrainBest < 1000 )
		
		fprintf(fBest,"Run%2d %12.3f %12.3f %12.6f %12.3f %12.3f ", run, kBest, nBest, rBest, TrainBest, EvalBest );
	else
		
		fprintf(fBest,"Run%2d %12.3f %12.3f %12.6f %.3e %.3e ", run, kBest, nBest, rBest, TrainBest, EvalBest );

	Best.k = kBest;
	Best.n = nBest;
	Best.r = rBest;
	Best.X[0] = kBest;
	Best.X[1] = nBest;
	Best.X[2] = rBest;
	Best.Train = TrainBest;
	Best.Eval = EvalBest;

	for ( i =0 ; i<nPoint; i++ )
	{
		fprintf(fBest,"%12.3f ", Best.Prof[i] );	
	}

	fprintf(fBest, "\n");
	fflush(fBest);

	delete [] watConc;
	delete [] TimeVol;	
}

void HC::evaluate(CRadial &RadModel, double k, double n, double r)
{	
/*	FILE *fOut;
	fOut = fopen( "testFunc.out","w");
*/	

	int i,
		 st,
		 NumTimes, // Number of times per Stage
		 t,
		 mod = 100,
		 N = 100,
		 l,
		 PrintComp = 0,
		 nTemp,
		 Units;
	
	double QT, // It stores the totaRl Flow rate per stage
		    tempTime,
			 tempVol,
			 OldInhLife = -1,
			 OldTime =  0,
			 NewTime  = 0,
			 CumWater = 0,
			 TotalLayHeight = 0,
			 WellVol = 0;

	if ( RadModel.Model.nSimTyp == 1 ) // Squeeze VI
	{
		tempTime = tempVol = 0;
		
		OldTime = NewTime = 0;
		RadModel.time = 0;
		tempTime = tempVol = 0;
		nTemp = 0;
		
		RadModel.Warnings = 0;

		for (l = 0; l < RadModel.GetnLayers(); l++)
		{

			RadModel.InjWaterAtWell[l] = 0;
			RadModel.InjOilAtWell[l] = 0;
			RadModel.WaterLeaving[l] = 0;
			RadModel.OilLeaving[l] = 0;
			RadModel.ResWater[l] = 0;   
			RadModel.WaterWell[l] = 0;
			RadModel.cumProWater[l] = 0;
			RadModel.cumProOil[l] = 0;
			RadModel.LaySpacing[l] = 0;
			RadModel.TimeWellMic[l] = 0;
			RadModel.E[l] = 0;

			for ( int b = 0; b < RadModel.GetnBlocks(); b++)
			{
				RadModel.waterSat[b][l] = RadModel.layer[l].dynamicLayer.Swi;
				RadModel.newWaterSat[b][l] = 0;
				RadModel.oilSat[b][l] = 0;
				RadModel.waterVisc[b][l] = RadModel.resDim.MUw;
				RadModel.oilVisc[b][l] = RadModel.resDim.MUo;
				RadModel.mob[b][l] = 0;
				RadModel.waterFracFlow[b][l] = 0;
				RadModel.watRF[b][l] = 1.0;
				RadModel.oilRF[b][l] = 1.0;
				RadModel.RF[b][l] = 1.0;
				
				for (int co =0; co < RadModel.nComponentsWater; co++)
				{
					RadModel.oConc[b][l][co] = 0;
					RadModel.newOConc[b][l][co] = 0;
					RadModel.oAds[b][l][co] = 0;
					RadModel.newOAds[b][l][co] = 0;
				}
				
				for (int cw =0; cw < RadModel.nComponentsWater; cw++)
				{		
					RadModel.wConc[b][l][cw] = 0;
					RadModel.newWConc[b][l][cw] = 0;
					RadModel.wAds[b][l][cw] = 0;
					RadModel.newWAds[b][l][cw] = 0;

				}
				
			}
		
		}

		for (int cw =0; cw < RadModel.nComponentsWater; cw++)
		{
			RadModel.WatTotalConc[cw] = 0; 
			RadModel.OilTotalConc[cw] = 0; 
			RadModel.WatMassPro[cw] = 0;
 			RadModel.OilMassPro[cw] = 0; 
		}

		for ( l = 0 ; l<RadModel.GetnLayers() ; l++ )
		{	
			RadModel.wIso[l][0].IsoTyp = 1;
			RadModel.wIso[l][0].k = k;
			RadModel.wIso[l][0].n = n;

			if ( r <= 0 ) 
						
				RadModel.wIso[l][0].TypeAds = 0;

			else
			
				RadModel.wIso[0][0].r2 = r/86400.0;
			
			TotalLayHeight += RadModel.LaySpacing[l] + Mod.SLay[l].HLay;
		}

		WellVol = PI * pow(RadModel.resDim.radiusWell, 2.0) * TotalLayHeight;
		
		if ( RadModel.UnitSpec == 'm' || RadModel.UnitSpec == 'M')	// Metric

			Units = 1; 
		
		if ( RadModel.UnitSpec == 'c' || RadModel.UnitSpec == 'C')	// Conventional 

			Units = 0;

		for( st=0 ; st<RadModel.GetnStages(); st++)
		{	
			QT = RadModel.stage[st].Qwt + RadModel.stage[st].Qot;
		
			if ( RadModel.NoTstep == 1 ) // Automatic timestep
			{
				if( RadModel.wIso[0][0].TypeAds == 1 || RadModel.wIso[0][0].TypeAds == 2 )
					
					if( RadModel.stage[st].TimeStep > (1.0/RadModel.wIso[0][0].r2) ) 

						RadModel.stage[st].TimeStep = 0.9*(1.0/RadModel.wIso[0][0].r2);
			}
			
			RadModel.timeStep = RadModel.stage[st].TimeStep;

			// Times per stage
			NumTimes = int( RadModel.stage[st].Time / RadModel.timeStep );
			
	//			fprintf(fCalcOut,"k %12.3f st %d TS %e NumTimes %d \n", k, st, RadModel.stage[st].Time, NumTimes);

			for( t=0 ; t<=NumTimes; t++)
			{	
				if ( t == NumTimes )			
				{	
				
					tempTime += RadModel.stage[st].Time;
											
					RadModel.timeStep = tempTime - RadModel.time;

					RadModel.time += RadModel.timeStep;

				}
				
				if ( t < NumTimes ) 

					RadModel.time += RadModel.timeStep;

				RadModel.LoopLayers( st, t, mod, PrintComp, WellVol );
				
				RadModel.CalculateWatTotalConc(st);
				
				RadModel.PV += (QT * RadModel.timeStep / RadModel.TotPV);
				
				// Data against Time 
				if (RadModel.Model.kPlot == 0 ) 
				{
					NewTime = RadModel.time/86400;

					if( OldTime <= RadModel.Model.fx[nTemp] && NewTime >= RadModel.Model.fx[nTemp] )
					{	
						watConc[nTemp] = RadModel.WatTotalConc[0];
						TimeVol[nTemp] = OldTime;

	//					fprintf(fOut,"%d %12.3f %12.3f %12.3f %12.3f\n",nTemp, RadModel.Model.fx[nTemp],OldTime,RadModel.Model.fy[nTemp],RadModel.WatTotalConc[0] ); 					
						nTemp++;
						nPoint = nTemp;
					}
				}	
				
				// Data against Cumulative Volume 
				if (RadModel.Model.kPlot == 1 ) 
				{	
					CumWater = 0;

					for(l = 0 ; l < RadModel.GetnLayers() ; l++)
			
						CumWater += RadModel.cumProWater[l];
					
					if (Units == 0 )
						
						NewTime = CumWater/(1e3*0.158987295); // Transform into 1000 bbl from m3
					
					if (Units == 1 ) // Metric
						
						NewTime = CumWater/1e2; // Transform into 100 m3 from m3

					
				//	fprintf(fOut,"%d %12.3f %12.3f %12.3f %12.3f\n",nTemp, RadModel.Model.fx[nTemp],OldTime,RadModel.Model.fy[nTemp],RadModel.WatTotalConc[0] ); 
				
					if( OldTime <= RadModel.Model.fx[nTemp] && NewTime > RadModel.Model.fx[nTemp] )
					{	
						TimeVol[nTemp] = OldTime;
						watConc[nTemp] = RadModel.WatTotalConc[0];
						nTemp++;
						nPoint = nTemp;
					}
					
				}	
				
				OldTime = NewTime;
				
				if( nTemp >= RadModel.Model.nPts )

					break;

				if( OldTime >= RadModel.Model.fx[RadModel.Model.nPts-1] )

					break;

			} // t Loop		
			
			if( nTemp >= RadModel.Model.nPts )

				break;

			if( OldTime >= RadModel.Model.fx[RadModel.Model.nPts-1] )

				break;

		} // st Loop+
	}

	if ( RadModel.Model.nSimTyp == 0 ) // Squeeze V, Single Phase
	{	
		// Create a Process
		STARTUPINFO sj;
		PROCESS_INFORMATION pj;

		ZeroMemory( &sj, sizeof(sj) );
		sj.cb = sizeof(sj);
		ZeroMemory( &pj, sizeof(pj) );
		
		CString sItem, s2;
		sItem = RadModel.Model.ReadPathName;	
		if ( !sItem.Replace(".dat",".out") ) sItem.Replace(".DAT",".out");
		
		char buffer[_MAX_PATH];

	   // Get the current working directory: 
		_getcwd( buffer, _MAX_PATH );

		char st[2000] = ".\\Exec\\HMI_Fortran.exe ";
		
		if ( SetCurrentDirectory(RadModel.Model.PathName) == 0 )
		{
			AfxMessageBox("ERROR"); 
		}
		
		char Val[100];
		sprintf(Val, "%f %f %f", k,n,r);
	//	sprintf(Val, "%f %f %f", 500.0,0.2,0.01);
		strcpy(st,".\\Exec\\HMI_Fortran.exe .\\Exec\\ga");
		strcat(st, RadModel.Model.ReadFileName.GetBuffer(0));
		CString sTemp = RadModel.Model.ReadFileName;
		sTemp.Replace(_T("DAT"), _T("out"));
		sTemp.Replace(_T("DAT"), _T("out"));
		strcat(st," .\\Exec\\ga");
		strcat(st,sTemp.GetBuffer(0));	
		strcat(st," .\\Exec\\temp");
		strcat(st,sTemp.GetBuffer(0));
		strcat(st," ");
		strcat(st, Val);
	
	//	strcpy(st,".\\Exec\\HMI_Fortran.exe .\\Exec\\Exec.dat .\\Exec\\Exec.out .\\Exec\\temp.out 1311.563000 0.855000 0.100000");
	//	strcpy(st,".\\Exec\\HMI_Fortran.exe .\\Exec\\Exec.dat .\\Exec\\Exec.out .\\Exec\\temp.out 1311.563000 0.855000 0.100000");
		if(!CreateProcess(NULL,st,  NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &sj, &pj))
		{	
			CString st1;
			st1.Format("Hello CreateProcess failed (%d)\n", GetLastError());
			AfxMessageBox(st1);
		}

		// Wait until child processes exit.
		WaitForSingleObject( pj.hProcess, INFINITE );
		
		// Close process and thread handles.
		CloseHandle( pj.hProcess );
		CloseHandle( pj.hThread );

		FILE *filePtr;

		if ( (filePtr = fopen(RadModel.Model.PathName + "Exec\\temp"+sTemp,"r")) == NULL )
		{   
			AfxMessageBox("The file .\\Exec\\temp"+sTemp+" was not opened.\nMake sure is in the right directory.");
		}
		else
		{  	
			double oldTime, newTime, oldConc, newConc;
			oldTime = newTime = oldConc = newConc = 0;

			/* Set pointer to beginning of file: */
			fseek( filePtr, 0L, SEEK_SET );
			
			i = 0;
			fscanf(filePtr,"%lf %lf \n",&oldTime, &oldConc);
			
			watConc[i] = oldConc;
			TimeVol[i] = oldTime;
			
			if ( oldTime > RadModel.Model.fx[i] )
			{
				watConc[i] = RadModel.Model.fy[i];
				TimeVol[i] = RadModel.Model.fx[i];
				i++;
			}
			
			while( !feof(filePtr) && i<nPoint )
			{
				if ( newTime > RadModel.Model.fx[i] )
				{
					watConc[i] = oldConc;
					TimeVol[i] = RadModel.Model.fx[i];
					i++;
				}
				else
					
					fscanf(filePtr,"%lf %lf \n",&newTime, &newConc);

				if( oldTime <= RadModel.Model.fx[i] && newTime >= RadModel.Model.fx[i] )
				{
					watConc[i] = oldConc;
					TimeVol[i] = oldTime;
					i++;
				}
				
				oldTime = newTime;
				oldConc = newConc;
				
			}
		}

		fclose(filePtr);

	}
	
}

bool HC::Misfit( int type, double TrainPer, double &Train, double &Eval)
{
	// Check for convergence of new Isotherm 
	bool Fail = FALSE;
	
	Train = 0;
	Eval = 0;
	
	double Temp = 0;

	for ( int i =0 ; i < nPoint ; i++ )
	{	
		if ( type == 0 ) // Standard 
		{	
			if (RadModel.Model.fy[i] > 0)
				
				Temp = pow( (watConc[i]-RadModel.Model.fy[i])/RadModel.Model.fy[i], 2.0);

			else

				Temp = 0;
		}

		if ( type == 1 ) // Natural Logarithm
		{	
			double l1, l2;
			if ( watConc[i] > 0 ) l1 = log10(watConc[i]);
			else	l1 = 0;
			
			if (RadModel.Model.fy[i] > 0) l2 = log10(RadModel.Model.fy[i]);
			else l2 = 0;

			Temp = fabs(l1-l2);
		}

		if ( i < TrainPer * nPoint ) // 70 % of the profile for training
			
			Train += Temp;

		else // 30% for evaluation
			
			Eval += Temp;

		if ( watConc[i] < 0 ) Fail = TRUE;
	}

	return Fail;
}

double HC::Gauss(double m, double s)
{
	
	double G,pi,x1,x2;
	pi =  4*atan(1.0);
	x1 = (double)rand()/RAND_MAX;
	x2 = (double)rand()/RAND_MAX;
	G = sqrt(-2*log(x1))*cos( 2* pi* x2 );
	return (m+G*s);
}

int HC::ProcessMessageLoop() {

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