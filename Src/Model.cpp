#include "stdafx.h"
#include <assert.h>

using namespace std;
//------------------------------------------------------------------------------------------
Iso::~Iso()
{
}
//------------------------------------------------------------------------------------------
Stg::~Stg()
{
}
//------------------------------------------------------------------------------------------
Model::~Model()
{
	
}

//------------------------------------------------------------------------------------------
void Model::ReadFileSQZ5Radial(const char* NameFile)
{
	FILE* filePtr;
	errno_t err;

	int i,
		l,
		j,
		nComp,	// Number of Components
		temp, temp1,
		tmp,
		nInjStg;

	double SumKH;

	// before, the buffer size was 128, and was used in the code as number "128", not as constant BUFFSIZE - ILYA
	const size_t BUFFSIZE = 4096;		// the fixed buffer size; BE AWARE!
	char buffer[BUFFSIZE];

	nComp = 1;

	if ((err = fopen_s(&filePtr, NameFile, "r")) != 0)
		printf("Error opening file %s for reading\r\n", NameFile);

	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);

	// validate the SQUEEZE data file and check the version
	fgets(buffer, BUFFSIZE, filePtr);
	// before it was: fscanf_s(filePtr, "%s\n", buffer), but it didn't work well - ILYA

	if (strncmp(buffer, "SQZV5.0", 7) != 0 && strncmp(buffer, "sqzv5.0", 7) != 0)
	{
		//	printf("This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.\n");
		printf("This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.");
		fclose(filePtr);
		nSimTyp = 0;
	}

	/* Reads in SQUEEZE V files */
	/* read in variables */
	fgets(buffer, 128, filePtr);
	title1.Format("%s", buffer);
	fgets(buffer, 128, filePtr);
	title2.Format("%s", buffer);
	
	fgets(buffer, BUFFSIZE, filePtr);
	fscanf_s(filePtr, "%d %d %d \n", &iModel, &temp, &temp1);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

	// Isotherm Derivation Read File
	if (iModel == 1)
	{
		printf("NOT READY\n");
	}

	// Analitical Read File
	if (iModel == 2)
	{
		printf("NOT READY\n");
	}

	// Radial Read File
	if (iModel == 3)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%d %c\n", &nLay, &UnitSpec);         //nLay =1 default   

		// Set nComp to be 1 by default
		nComp = 1;

		SLay.resize(nLay);
		LaySpacing.assign(nLay, 0);

		if (nLay == 1)
		{
			fgets(buffer, BUFFSIZE, filePtr);

			fscanf_s(filePtr, "%lf %lf %lf %lf\n", &radiusWell,
				&radiusMax, &SLay[0].HLay,
				&SLay[0].Phi);

			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%d %d\n", &kChem, &kTemp);

			if (kChem == 1)
			{
				fgets(buffer, BUFFSIZE, filePtr);
				fscanf_s(filePtr, "%d %d %lf\n", &Iso.IsoTyp,
					&Iso.TypeAds, &TolSQ5);

				if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
				{
					fgets(buffer, BUFFSIZE, filePtr);
					fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
				}

				else if (Iso.IsoTyp == 2)
				{
					fgets(buffer, BUFFSIZE, filePtr);
					fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
				}

				else if (Iso.IsoTyp == 3)
				{
					fgets(buffer, BUFFSIZE, filePtr);
					fscanf_s(filePtr, "%d %lf\n", &Iso.nIso, &Iso.r2);

					++Iso.nIso;

					Iso.Ciso.resize(Iso.nIso);
					Iso.ADiso.resize(Iso.nIso);

					fgets(buffer, BUFFSIZE, filePtr);

					Iso.Ciso[0] = 0;
					Iso.ADiso[0] = 0;

					for (j = 1; j < Iso.nIso; j++)
					{
						fscanf_s(filePtr, "%lf %lf\n", &Iso.Ciso[j], &Iso.ADiso[j]);
					}

				}

				if (Iso.IsoTyp == 4)
				{
					fgets(buffer, BUFFSIZE, filePtr);
					fscanf_s(filePtr, "%lf %lf %lf %lf\n", &Iso.Cps1, &Iso.Cps2, &Iso.Tpc, &Iso.r4);

					Iso.TypeAds = 1;
				}

			}		// end of kChem == 1 

		}	// if nLays = 1

		if (nLay > 1)
		{
			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%lf %lf \n", &radiusWell, &radiusMax);

			SumKH = 0;

			fgets(buffer, BUFFSIZE, filePtr);
			for (l = 0; l < nLay; l++)
			{
				fscanf_s(filePtr, "%lf %lf %lf\n", &SLay[l].HLay, &SLay[l].Phi, &SLay[l].KAbs);

				SumKH += (SLay[l].HLay * SLay[l].KAbs);
			}

			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%d %d\n", &kChem, &kTemp);

			if (kChem == 1)
			{

				fgets(buffer, BUFFSIZE, filePtr);
				fscanf_s(filePtr, "%d %lf\n", &IsoSpec, &TolSQ5);

				if (IsoSpec == 0)
				{
					fgets(buffer, BUFFSIZE, filePtr);
					fscanf_s(filePtr, "%d\n", &Iso.IsoTyp);

					fgets(buffer, BUFFSIZE, filePtr);
					for (int ii = 0; ii < nLay; ii++)
						fscanf_s(filePtr, "%d %lf\n", &Iso.TypeAds, &Iso.r2);

					if (Iso.IsoTyp == 4)
						Iso.r4 = Iso.r2;

					fgets(buffer, BUFFSIZE, filePtr); // check also; ILYA

					if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
					{
						fgets(buffer, BUFFSIZE, filePtr);
						fscanf_s(filePtr, "%lf %lf\n", &Iso.k, &Iso.n);
					}

					else if (Iso.IsoTyp == 2)
					{
						fgets(buffer, BUFFSIZE, filePtr);
						fscanf_s(filePtr, "%lf %lf\n", &Iso.k, &Iso.n);
					}

					else if (Iso.IsoTyp == 3)
					{
						fgets(buffer, BUFFSIZE, filePtr);
						fscanf_s(filePtr, "%d\n", &Iso.nIso);

						Iso.Ciso.resize(Iso.nIso);
						Iso.ADiso.resize(Iso.nIso);

						fgets(buffer, BUFFSIZE, filePtr);

						for (j = 0; j < Iso.nIso; j++)
						{
							fscanf_s(filePtr, "%lf %lf\n", &Iso.Ciso[j], &Iso.ADiso[j]);
						}

					}

					else if (Iso.IsoTyp == 4)
					{
						fgets(buffer, BUFFSIZE, filePtr);
						fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.Cps1, &Iso.Cps2, &Iso.Tpc);
					}

				}	// End of IsoSpec == 0

				else  // IsoSpec == 1         

				{

					fgets(buffer, BUFFSIZE, filePtr);

					for (l = 0; l < nLay; l++)
						fscanf_s(filePtr, "%d %d\n", &Iso.IsoTyp, &Iso.TypeAds);


					for (l = 0; l < nLay; l++)
					{
						fgets(buffer, BUFFSIZE, filePtr);

						if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
						{
							fgets(buffer, BUFFSIZE, filePtr);
							fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
						}

						else if (Iso.IsoTyp == 2)
						{
							fgets(buffer, BUFFSIZE, filePtr);
							fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
						}

						else if (Iso.IsoTyp == 3)
						{
							fgets(buffer, BUFFSIZE, filePtr);
							fscanf_s(filePtr, "%d %lf\n", &Iso.nIso, &Iso.r2);

							fgets(buffer, BUFFSIZE, filePtr);

							for (j = 0; j < Iso.nIso; j++)
							{
								fscanf_s(filePtr, "%lf %lf\n", &Iso.Ciso[j], &Iso.ADiso[j]);
							}

						}

						else if (Iso.IsoTyp == 4)
						{
							fgets(buffer, BUFFSIZE, filePtr);
							fscanf_s(filePtr, "%lf %lf %lf %lf\n", &Iso.Cps1, &Iso.Cps2, &Iso.Tpc, &Iso.r4);

							Iso.TypeAds = 1;
						}

					}	// end of for loop Lays	

				}  // end of else IsoSpec ==1   

			}	// end of kChem == 1 

		}	// if nLays > 1	

		if (kTemp == 1)
		{
			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%lf %lf %lf %lf %lf %lf %lf\n", &Temp.Ftemp, &Temp.Kcr, &Temp.Kcs, &Temp.SHr,&Temp.SHw, &Temp.RHOw, &Temp.RHOr);
		}

		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%d %d %d %d %d %d %d\n", &nBlocks, &nStg, &nIters, &nFreq, &NoTstep, &IsCon, &IsTmp);

		// To make compatible with Squeeze V, since although it is is the manual you need to
		// include always the timestep
		if (NoTstep == 1) NoTstep = 0;

		stg.resize(nStg);

		// Reading Stgs 
		fgets(buffer, BUFFSIZE, filePtr);
		nInjStg = 0;
		TotalInhMass = 0;
		TotalProd_T_V = 0;

		for (j = 0; j < nStg; j++)
		{
			stg[j].PIDF.resize(nLay);
			stg[j].QwLay.resize(nLay);
			stg[j].nLayOp.resize(nLay);
			stg[j].LayPres.resize(nLay);
			stg[j].ProdFw.resize(nLay);

			stg[j].InitialCw.resize(nComp);
			stg[j].InitialCo.resize(nComp);

			for (i = 0; i < nLay; i++)
			{
				stg[j].nLayOp[i] = 0;
			}

			if (nLay > 1)
			{
				if (NoTstep == 0)

					fscanf_s(filePtr, "%lf %lf %lf", &stg[j].Qwt,
						&stg[j].Time, &stg[j].TimeStep);

				else

					fscanf_s(filePtr, "%lf %lf ", &stg[j].Qwt,
						&stg[j].Time);

				fscanf_s(filePtr, "%lf", &stg[j].InitialCw[0]);

				fscanf_s(filePtr, "%lf %d %d %d %d\n", &stg[j].TI,
					&stg[j].TypeStg, &stg[j].nChage,
					&stg[j].nOpen, &stg[j].nMethod);

				if (stg[j].TypeStg != 3 && stg[j].TypeStg != 5) // No Production or Shut-in Stage

					nInjStg++;

				if (stg[j].TypeStg != 1)

					stg[j].InitialCw[0] = 0;

				for (i = 0; i < nLay; i++)

					stg[j].PIDF[i] = (SLay[i].HLay * SLay[i].KAbs) / SumKH;

				if (j == 0 || stg[j].nChage == 1)
				{

					if (stg[j].nOpen > 0)
					{
						temp = stg[j].nOpen;

						fgets(buffer, BUFFSIZE, filePtr);
						if (temp <= 10)
						{
							for (i = 0; i < temp - 1; i++)
								fscanf_s(filePtr, "%d ", &stg[j].nLayOp[i]);
							fscanf_s(filePtr, "%d \n", &stg[j].nLayOp[temp - 1]);

						}
						else
						{
							for (i = 0; i < 9; i++)
								fscanf_s(filePtr, "%d ", &stg[j].nLayOp[i]);
							fscanf_s(filePtr, "%d \n", &stg[j].nLayOp[9]);

							for (i = 10; i < temp - 1; i++)
								fscanf_s(filePtr, "%d ", &stg[j].nLayOp[i]);
							fscanf_s(filePtr, "%d \n", &stg[j].nLayOp[temp - 1]);

						}

						if (stg[j].nMethod == 1)
						{
							for (i = 0; i < nLay; i++)

								stg[j].PIDF[i] = 0;

							tmp = 0;

							fgets(buffer, BUFFSIZE, filePtr);
							if (nLay <= 10)
							{
								for (i = 0; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
										++tmp;
									}
								}

								if ((nLay - 1) == (stg[j].nLayOp[tmp] - 1))
								{
									fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[nLay - 1]);
									++tmp;
								}
							}
							else
							{
								tmp = 0;

								for (i = 0; i < 9; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
										++tmp;
									}
								}

								if (9 == (stg[j].nLayOp[tmp] - 1))
								{
									fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[9]);
									tmp++;
								}

								for (i = 10; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
										tmp++;
									}
								}

								if (nLay - 1 == (stg[j].nLayOp[tmp] - 1))
								{
									fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[nLay - 1]);
								}

							}

						}  // end of iMethd ==1

					}
					else // stg[j].nOpen == 0
					{
						for (i = 0; i < nLay; i++)
						{
							stg[j].nLayOp[i] = i + 1;
						}

						temp = nLay;

						if (stg[j].nMethod == 1)
						{
							for (i = 0; i < nLay; i++)

								stg[j].PIDF[i] = 0;

							fgets(buffer, BUFFSIZE, filePtr);
							if (temp <= 10)
							{
								for (i = 0; i < temp - 1; i++)
									fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
								fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[temp - 1]);
							}
							else
							{
								for (i = 0; i < 9; i++)
									fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
								fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[9]);
								for (i = 10; i < temp - 1; i++)
									fscanf_s(filePtr, "%lf ", &stg[j].PIDF[i]);
								fscanf_s(filePtr, "%lf \n", &stg[j].PIDF[temp - 1]);
							}

						}  // end of iMethd ==1

					}	// end of stg[j].nOpen == 0 

				}	// end of j==1 || nChage == 1	

				else if (j > 0)
				{
					stg[j].nMethod = stg[j - 1].nMethod;
					stg[j].nOpen = stg[j - 1].nOpen;
					temp = stg[j].nOpen;

					if (temp == 0) temp = nLay;

					for (i = 0; i < temp; i++)
					{
						stg[j].nLayOp[i] = stg[j - 1].nLayOp[i];

						if (stg[j].nMethod == 1)
						{
							stg[j].PIDF[i] = stg[j - 1].PIDF[i];
						}
					}

				}

			} // if (nLay > 1)

			if (nLay == 1)
			{
				if (NoTstep == 0)

					fscanf_s(filePtr, "%lf %lf %lf ", &stg[j].Qwt, &stg[j].Time, &stg[j].TimeStep);

				else

					fscanf_s(filePtr, "%lf %lf ", &stg[j].Qwt, &stg[j].Time);

				fscanf_s(filePtr, "%lf", &stg[j].InitialCw[0]);

				fscanf_s(filePtr, "%lf %d \n", &stg[j].TI, &stg[j].TypeStg);

				stg[j].nOpen = 0;

				stg[j].PIDF[0] = 1;

				if (stg[j].TypeStg != 3)

					nInjStg++;
			}

			//	RelativePIDf(j);

			TotalInhMass += stg[j].Time * stg[j].Qwt * stg[j].InitialCw[0];

		}   // end of for loop nStg

		if (nLay > 1)
		{
			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%d %d\n", &nLife, &kPlot);

			Clife.resize(nLife);

			TVlife.resize(nLife);

			if (nLife > 0)
			{
				fgets(buffer, BUFFSIZE, filePtr);
				for (j = 0; j < nLife; j++)
				{
					fscanf_s(filePtr, "%lf ", &Clife[j]);
					TVlife[j] = -1;
				}

			}

			//			if (kChem == 1)
			{
				fgets(buffer, BUFFSIZE, filePtr);
				fscanf_s(filePtr, "%lf\n", &GrafMIC);
			}

			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%d %d %d %d\n", &kCrange, &kGraf1, &kGraf2,
				&kGraf3);

			if (kChem == 0) kCrange = 0;

			if (kCrange > 0 && kChem == 1)
			{
				fgets(buffer, BUFFSIZE, filePtr);
				fscanf_s(filePtr, "%lf %lf\n", &Cmin, &Cmax);
			}

			if (kGraf2 > 0)
			{
				nGraf2.resize(kGraf2);

				fgets(buffer, BUFFSIZE, filePtr);
				for (i = 0; i < kGraf2 - 1; i++)
					fscanf_s(filePtr, "%d ", &nGraf2[i]);
				fscanf_s(filePtr, "%d\n ", &nGraf2[kGraf2 - 1]);
			}

			if (kGraf3 > 0)
			{
				nGraf3.resize(kGraf3);

				fgets(buffer, BUFFSIZE, filePtr);
				for (i = 0; i < kGraf3 - 1; i++)
					fscanf_s(filePtr, "%d ", &nGraf3[i]);
				fscanf_s(filePtr, "%d\n ", &nGraf3[kGraf3 - 1]);
			}

		} // if nLays > 1

		// else Clife[0] = 10; -- it was a weird line - ILYA

		if (nLay == 1)
		{
			fgets(buffer, BUFFSIZE, filePtr);
			fscanf_s(filePtr, "%d %d %d\n", &iAnim, &nLife, &kPlot);

			if (nLife > 0)
			{	
				Clife.resize(nLife);
				TVlife.resize(nLife);

				fgets(buffer, BUFFSIZE, filePtr);
				for (j = 0; j < nLife; j++)
				{
					fscanf_s(filePtr, "%lf ", &Clife[j]);
					TVlife[j] = -1;
				}
			}
			else
			{	
				nLife = 1;
				Clife.resize(nLife);
				TVlife.resize(nLife);
			}

			GrafMIC = Clife[0];

		} // if nLays == 1

	} // Model.iModel == 3


	for (j = 0; j < nStg; j++)
	{
		if (kPlot == 0)

			TotalProd_T_V += stg[j].Time;

		if (kPlot == 1)

			TotalProd_T_V += stg[j].Time*stg[j].Qwt;
	}

	for (j = 0; j < nLife; j++)
	{
		TVlife[j] = TotalProd_T_V;
	}

	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);

	fscanf_s(filePtr, "%d\n", &nPts);

	if (nPts > 0)
	{
		int loc_npts;
		double temp1, temp2;

		fgets(buffer, BUFFSIZE, filePtr);

		if (nPts > 300) loc_npts = 300;
		else loc_npts = nPts;

		fX.clear();
		fY.clear();

		for (j = 0; j < loc_npts; j++)
		{
			fscanf_s(filePtr, "%lf %lf \n", &temp1, &temp2);

			fX.push_back(temp1);
			fY.push_back(temp2);
		}

		nPts = fX.size();

		CString st = PathName + "\\Field Data\\" + FieldTabFileName;
		
		errno_t err;
		FILE *fp;

		err = fopen_s(&fp, st, "w");
		if (err == 0)
		{	
			fprintf_s(fp, "nPts\n");
			fprintf_s(fp, "%d\n", nPts);

			fprintf_s(fp, "t(day/1000bbl,day/100m3)	C(ppm)\n");
			for (int i = 0; i < nPts; i++)
			fprintf_s(fp, "%lf %lf \n", fX[i], fY[i]);
			fclose(fp);
		}
	}

	fgets(buffer, BUFFSIZE, filePtr);
	fscanf_s(filePtr, "%d\n", &nOlds);

	if (nOlds > 0)
	{
		FileNm.clear();

		fgets(buffer, BUFFSIZE, filePtr);
		for (j = 0; j < nOlds; j++)
		{
			fgets(buffer, BUFFSIZE, filePtr);
			FileNm.push_back(buffer);
		}
	}

	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);

	fscanf_s(filePtr, "%d %d %d\n", &SqzOpt.nPar, &SqzOpt.nPopSize, &SqzOpt.nGen);

	fgets(buffer, BUFFSIZE, filePtr);
	fscanf_s(filePtr, "%lf %lf\n", &SqzOpt.MIC, &SqzOpt.Obj_T_V);

	// Iostherm Derivation
	if (nOpt == 0)
	{
		if (Iso.TypeAds == 0) // Equilibrium
		{
			SqzOpt.nPar = 2;
		}

		if (Iso.TypeAds == 1) // Kinetic
		{
			SqzOpt.nPar = 3;
		}
	}

	if (SqzOpt.nPar > 0)
	{
		SqzOpt.Vol.resize(SqzOpt.nPar);
		SqzOpt.lX.resize(SqzOpt.nPar);
		SqzOpt.uX.resize(SqzOpt.nPar);
		SqzOpt.X.resize(SqzOpt.nPar);
	}

	fgets(buffer, BUFFSIZE, filePtr);
	for (unsigned long i = 0; i < SqzOpt.nPar; i++)
	{
		int itemp;
		double temp;

		fscanf_s(filePtr, "%d %lf %lf %lf\n", &itemp, &SqzOpt.X[i], &SqzOpt.lX[i], &SqzOpt.uX[i]);
	}

	int index = 0;

	SqzOpt.Label.resize(SqzOpt.nPar);

	fgets(buffer, BUFFSIZE, filePtr);
	for (unsigned long i = 0; i < nInjStg; i++)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		const char* delim = " \t";
		char* next_token;
		char* p;
		p = strtok_s(buffer, delim, &next_token);
		int pos = 0;
		while (p)
		{
			char* pch = 0;
			if (p != NULL) pch = strchr(p, '$');
			if (pch != NULL)
			{
				if (pos == 0)
				{	
					int temp = index + 1;
					char intstr[3];
					itoa(i + 1, intstr, 10);
					SqzOpt.Label[index] = "InjQt" + string(intstr);
					
					stg[i].objFlowRate = 1;
					stg[i].objIndexFlowRate = index;
					SqzOpt.X[index] = stg[i].Qwt;
					index++;
				}

				if (pos == 1)
				{	
					int temp = index + 1;
					char intstr[3];
					itoa(i + 1, intstr, 10);
					if (stg[i].TypeStg == 1)
					{
						SqzOpt.Label[index] = "InjVol" + string(intstr);
					}

					if (stg[i].TypeStg == 2)
					{
						SqzOpt.Label[index] = "OFVol" + string(intstr);
					}

					stg[i].objTime = 1;
					stg[i].objIndexTime = index;
					SqzOpt.X[index] = stg[i].Time;
					SqzOpt.Vol[index] = stg[i].Time * stg[i].Qwt * 60;;
					index++;
				}

				if (pos == 3)
				{	
					int temp = index + 1;
					char intstr[3];
					itoa(i + 1, intstr, 10);
					if (stg[i].TypeStg == 1)
					{
						SqzOpt.Label[index] = "InjSI" + string(intstr);
					}

					stg[i].objSI = 1;
					stg[i].objIndexSI = index;
					SqzOpt.X[index] = stg[i].InitialCw[0];
					index++;
				}

			}
			p = strtok_s(NULL, delim, &next_token);
			pos++;
		}
	}

	fscanf_s(filePtr, "%d %d %lf %lf %d %d %d %d %d\n", &SqzOpt.SensCalc, &SqzOpt.nBins, &SqzOpt.TotSIVol, &SqzOpt.TotInjVol, &SqzOpt.Only_SI, &SqzOpt.Only_Tot_Vol, &SqzOpt.Fixed_Target, &SqzOpt.BinsMT, &SqzOpt.IterationsSecant);
														
	fclose(filePtr);

	// all deletes are done in destructors - ILYA

	return;
}
//------------------------------------------------------------------------------------------
void Model::WriteSingleLayers(const char *NameFile)
{
	FILE *filePtr;
	errno_t err;

	string tempLfname;
	tempLfname = NameFile;
	size_t p = tempLfname.find_last_of(".");
	if (p < string::npos)
		tempLfname = tempLfname.substr(0, p);
	else
		tempLfname = tempLfname;

	single_layer_fn = vector<string>(nLay);

	// Write the File Layer
	for (int l = 0; l < nLay; l++)
	{
		const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
		char ind[buffsize];
		sprintf_s(ind, buffsize, "%d", l);	// "task number" is written to char *ind
		string tind(ind);

		string Lfname = tempLfname + suff + tind + ".dat";
		single_layer_fn[l] = Lfname;

		if ((err = fopen_s(&filePtr, Lfname.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", Lfname.c_str());
		else
			printf("File %s was opened for writing\r\n", Lfname.c_str());

		/* Writes in SQUEEZE V files Single Phase in 1 layer*/
		fprintf_s(filePtr, "SQZV5.0\n"); // distinguishes between SQUEEZE III and SQUEEZE IV data files 
		fprintf_s(filePtr, "%s %d\n", "Lay File", l + 1);
		fprintf_s(filePtr, "%s %d\n", "Lay File", l + 1);
		fprintf_s(filePtr, "%s\n", "iModel      nEcho       nDebug");
		fprintf_s(filePtr, "%d %d %d \n", iModel, 1, 1);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

		// Radial Read File
		if (iModel == 3)
		{
			fprintf_s(filePtr, "nLay	UnitSpec\n");
			fprintf_s(filePtr, "%d %c\n", 1, UnitSpec);         //nLay =1 default   

			fprintf_s(filePtr, "Rwell\t\tRmax\t\tHeight\t\tPhi\n");

			fprintf_s(filePtr, "%lf %lf %lf %lf\n", radiusWell,
				radiusMax, SLay[l].HLay, SLay[l].Phi);

			fprintf_s(filePtr, "kChem      kTemp\n");
			fprintf_s(filePtr, "%d %d\n", kChem, kTemp);

			if (kChem == 1)
			{
				fprintf_s(filePtr, "IsoTyp		Neqil2		TolItr\n");
				fprintf_s(filePtr, "%d %d %g\n", Iso.IsoTyp,
					Iso.TypeAds, TolSQ5);

				if (Iso.IsoTyp == 1 || Iso.IsoTyp == 2)
				{
					fprintf_s(filePtr, "a		b		r2(1/day)\n");
					fprintf_s(filePtr, "%lf  %lf %lf\n", Iso.k, Iso.n, Iso.r2);
				}

				if (Iso.IsoTyp == 3)
				{
					fprintf_s(filePtr, "Niso			r2(1/day)\n");
					fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.nIso, Iso.r2);

					fprintf_s(filePtr, "Ciso		ADiso\n");
					for (int j = 0; j < Iso.nIso; j++)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
					}

				}

				if (Iso.IsoTyp == 4)
				{
					fprintf_s(filePtr, "Cs1		Cs2		Tpc		r4(1/day)\n");
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);

					Iso.TypeAds = 1;
				}

			}		// end of kChem == 1 

			if (kTemp == 1)
			{
				if (UnitSpec == 'm' || UnitSpec == 'M')

					fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/m*day*degC)  Kcs   SHr(Btu/kg*degC)  SHw    RHOw(kg/ft3)     RHOr \n");

				else

					fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/ft*day*degC)  Kcs   SHr(Btu/lb*degC)  SHw    RHOw(lb/ft3)    RHOr \n");

				fprintf_s(filePtr, "%-11.4f \t %-11.4f \t %-11.4f %-11.4f %-11.4f %-11.4f %-11.4f\n",
					Temp.Ftemp, Temp.Kcr, Temp.Kcs, Temp.SHr,
					Temp.SHw, Temp.RHOw, Temp.RHOr);
			}

			fprintf_s(filePtr, "nR		Nstages		NIters		Nfreq			NoTstep		IsCon			IsTmp\n");
			fprintf_s(filePtr, "%d %11d %11d %11d %11d %11d %11d\n", nBlocks, nStg, nIters, nFreq, NoTstep, IsCon, IsTmp);

			if (UnitSpec == 'm' || UnitSpec == 'M')
			{
				fprintf_s(filePtr, "Qw(cumpm/cumpd)	tspan(h/d)  dt(h/d)	 CIw(ppm)  TI(degC)	OperCode\n");
			}
			else
			{
				fprintf_s(filePtr, "Qw(bpm/bpd)  tspan(h/d)  dt(h/d)   CIw(ppm)  TI(degC)	OperCode\n");
			}

			for (int j = 0; j < nStg; j++)
			{
				int nL;
				double QLay;

				if (stg[j].nOpen != 0) nL = stg[j].nOpen;

				else nL = nLay;

				if ( l == 0 )
				{
					double PIDFSum = 0;

					for (int i = 0; i < nL; i++)

						PIDFSum += stg[j].PIDF[stg[j].nLayOp[i] - 1];

					for (int i = 0; i < nL; i++)
					{
						if (PIDFSum > 0)
							stg[j].PIDF[stg[j].nLayOp[i] - 1] = stg[j].PIDF[stg[j].nLayOp[i] - 1] / PIDFSum;
						else
							stg[j].PIDF[stg[j].nLayOp[i] - 1] = stg[j].PIDF[stg[j].nLayOp[i] - 1];
					
					}
				}

				QLay = 0;

				if (stg[j].nOpen != 0)
				{
					for (int i = 0; i < nL && QLay <= 0; i++)
					{
						if (stg[j].nLayOp[i] == (l + 1))
							QLay = stg[j].Qwt*stg[j].PIDF[stg[j].nLayOp[i] - 1];
						else
							QLay = 0;
					}
				}
				else
					QLay = stg[j].Qwt*stg[j].PIDF[l];

				stg[j].QwLay[l] = QLay;

				fprintf_s(filePtr, "%-13.8f %-11.4f %-11.8f", QLay, stg[j].Time, stg[j].TimeStep);

				fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);

				if (QLay > 0)
					fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI, stg[j].TypeStg);
				else
					fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI, 5);

			}

			fprintf_s(filePtr, "iAnim	nLife       kPlot \n");
			fprintf_s(filePtr, "%-11d %-11d %d \n", 1, nLife, kPlot);

			if (nLife > 0)
			{
				fprintf_s(filePtr, "Clife(ppm) \n");
				for (int j = 0; j<nLife - 1; j++)
				{
					fprintf_s(filePtr, "%-11.4f ", Clife[j]);
				}
				fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
			}

			fprintf_s(filePtr, "Field Squeeze Return Data \n");

			fprintf_s(filePtr, "nPts \n");
			fprintf_s(filePtr, "%d \n", 0);

			fprintf_s(filePtr, "nOlds \n");
			fprintf_s(filePtr, "%d \n", 0);

		} // IsoMod == 3

		fclose(filePtr);
	}
}
//------------------------------------------------------------------------------------------
void Model::ReadFileSQZ5Optimisation(const char *NameFile)
{
	FILE *filePtr;
	errno_t err;

	char line[1000];
	unsigned long result;

	if ((err = fopen_s(&filePtr, NameFile, "r")) != 0)
		printf("Error opening file %s for reading\r\n", NameFile);
	else
		printf("File %s was opened for reading\r\n", NameFile);

	// Find number of Layers 
	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);
	const size_t BUFFSIZE = 4096;		// the fixed buffer size; BE AWARE!
	char buffer[BUFFSIZE];

	// validate the SQUEEZE data file and check the version
	fgets(buffer, BUFFSIZE, filePtr);
	// before it was: fscanf_s(filePtr, "%s\n", buffer), but it didn't work well - ILYA

	if (strncmp(buffer, "SQZV5.0", 7) != 0 && strncmp(buffer, "sqzv5.0", 7) != 0)
	{
		//	printf("This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.\n");
		cout << "This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.";
		fclose(filePtr);
	}

	/* Reads in SQUEEZE V files */
	/* read in variables */
	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);
	int temp;
	fscanf_s(filePtr, "%d %d %d \n", &iModel, &temp, &temp);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

	// Isotherm Derivation Read File
	if (iModel == 1)
	{
		printf("NOT READY\n");
	}

	// Analitical Read File
	if (iModel == 2)
	{
		printf("NOT READY\n");
	}

	// Radial Read File
	if (iModel == 3)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%d %c\n", &nLay, &UnitSpec);         //nLay =1 default   
	}

	// Find Parameters Positions
	SqzOpt.nPar = 0;
	unsigned long ntemp;
	ntemp = SqzOpt.nPar;

	DataPos = GetPositionsFile(filePtr, "$p", 0L, ntemp);

	result = fseek(filePtr, FileSearch(filePtr, "Optimisation", DataPos[SqzOpt.nPar - 1]), SEEK_SET);
	if (result)
		perror("Fseek failed");
	else
	{
		int temp;
		fgets(line, 1000, filePtr);
		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d %d %d\n", &SqzOpt.nPar, &SqzOpt.nPopSize, &SqzOpt.nGen);

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%lf %lf\n", &SqzOpt.GrafMIC, &SqzOpt.Obj_T_V);
		
		SqzOpt.X.resize(SqzOpt.nPar);
		SqzOpt.Vol.resize(SqzOpt.nPar);
		SqzOpt.lX.resize(SqzOpt.nPar);
		SqzOpt.uX.resize(SqzOpt.nPar);

		fgets(line, 1000, filePtr);
		for (int i = 0; i < SqzOpt.nPar; i++)
		{
			fscanf_s(filePtr, "%d %lf %lf %lf\n", &temp, &SqzOpt.X[i], &SqzOpt.lX[i], &SqzOpt.uX[i]);
		}

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d %d %lf %lf %d %d %d %d %d\n", &SqzOpt.SensCalc, &SqzOpt.nBins, &SqzOpt.TotSIVol, &SqzOpt.TotInjVol, &SqzOpt.Only_SI, &SqzOpt.Only_Tot_Vol, &SqzOpt.Fixed_Target, &SqzOpt.BinsMT, &SqzOpt.IterationsSecant);

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d\n", &nObjs);
	}

	fclose(filePtr);
}
//------------------------------------------------------------------------------------------
void Model::ReadFileSQZ5Optimisation_Secant(const char *NameFile)
{
	FILE *filePtr;
	errno_t err;

	char line[1000];
	unsigned long result;

	if ((err = fopen_s(&filePtr, NameFile, "r")) != 0)
		printf("Error opening file %s for reading\r\n", NameFile);
	else
		printf("File %s was opened for reading\r\n", NameFile);

	// Find number of Layers 
	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);
	const size_t BUFFSIZE = 4096;		// the fixed buffer size; BE AWARE!
	char buffer[BUFFSIZE];

	// validate the SQUEEZE data file and check the version
	fgets(buffer, BUFFSIZE, filePtr);
	// before it was: fscanf_s(filePtr, "%s\n", buffer), but it didn't work well - ILYA

	if (strncmp(buffer, "SQZV5.0", 7) != 0 && strncmp(buffer, "sqzv5.0", 7) != 0)
	{
		//	printf("This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.\n");
		cout << "This is not a SQUEEZE V data file. Please select a SQUEEZE V file again.";
		fclose(filePtr);
	}

	/* Reads in SQUEEZE V files */
	/* read in variables */
	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);
	fgets(buffer, BUFFSIZE, filePtr);
	int temp;
	fscanf_s(filePtr, "%d %d %d \n", &iModel, &temp, &temp);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

	// Isotherm Derivation Read File
	if (iModel == 1)
	{
		printf("NOT READY\n");
	}

	// Analitical Read File
	if (iModel == 2)
	{
		printf("NOT READY\n");
	}

	// Radial Read File
	if (iModel == 3)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%d %c\n", &nLay, &UnitSpec);         //nLay =1 default  

		unsigned long tmp;
		DataPos = GetPositionsFile(filePtr, "kPlot", 0L, tmp);

		result = fseek(filePtr, FileSearch(filePtr, "kPlot", DataPos[tmp - 1]), SEEK_SET);
		if (result)
			perror("Fseek failed");
		else
		{
			int temp;
			fgets(line, 1000, filePtr);
			if (nLay>1)
				fscanf_s(filePtr, "%d %d\n", &nLife, &kPlot);
			else
				fscanf_s(filePtr, "%d %d %d\n", &iAnim, &nLife, &kPlot);
		}
	}

	// Find Parameters Positions
	SqzOpt.nPar = 0;
	unsigned long ntemp;
	ntemp = SqzOpt.nPar;
	DataPos = GetPositionsFile(filePtr, "$p", 0L, ntemp);

	result = fseek(filePtr, FileSearch(filePtr, "Optimisation", DataPos[SqzOpt.nPar - 1]), SEEK_SET);
	if (result)
		perror("Fseek failed");
	else
	{
		int temp;
		fgets(line, 1000, filePtr);
		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d %d %d\n", &SqzOpt.nPar, SqzOpt.nPopSize, &SqzOpt.nGen);

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%lf %lf\n", &SqzOpt.GrafMIC, &SqzOpt.Obj_T_V);

		SqzOpt.X.resize(SqzOpt.nPar);
		SqzOpt.Vol.resize(SqzOpt.nPar);
		SqzOpt.lX.resize(SqzOpt.nPar);
		SqzOpt.uX.resize(SqzOpt.nPar);

		fgets(line, 1000, filePtr);
		for (int i = 0; i < SqzOpt.nPar; i++)
		{
			fscanf_s(filePtr, "%d %lf %lf %lf\n", &temp, &SqzOpt.X[i], &SqzOpt.lX[i], &SqzOpt.uX[i]);
		}

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d %d %lf %lf %d %d %d %d %d\n", &SqzOpt.SensCalc, &SqzOpt.nBins, &SqzOpt.TotSIVol, &SqzOpt.TotInjVol, &SqzOpt.Only_SI, &SqzOpt.Only_Tot_Vol, &SqzOpt.Fixed_Target, &SqzOpt.BinsMT, &SqzOpt.IterationsSecant);

		fgets(line, 1000, filePtr);
		fscanf_s(filePtr, "%d\n", &nObjs);
	}

	fclose(filePtr);
}
//------------------------------------------------------------------------------------------
void Model::WriteOptimisationFileModels(const char *NameFile, vector<Particle> &p)
{	
	exception_ptr E;	// for handling exceptions thrown by threads

	try
	{
		FILE *source;
		unsigned long result;
		string tempLfname;
		char buf[BUFSIZ];
		size_t len;
		errno_t err;

		tempLfname = NameFile;
		size_t n = tempLfname.find_last_of(".");
		if (n < string::npos)
			tempLfname = tempLfname.substr(0, n);
		else
			tempLfname = tempLfname;

		// Make a copy of Input file
		if ((err = fopen_s(&source, NameFile, "r")) != 0)
			printf("Error opening file %s for writing\r\n", NameFile);
		else
			printf("File %s was opened for writing\r\n", NameFile);

		// Write the Simulation File
		for (int n = 0; n < SqzOpt.nPopSize; n++)
		{
			FILE *dest;
			const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
			char ind[buffsize];
			sprintf_s(ind, buffsize, "%d", n);	// "task number" is written to char *ind
			string tind(ind);

			string Lfname = tempLfname + suff + tind + ".dat";

			if ((err = fopen_s(&dest, Lfname.c_str(), "w+")) != 0)
				printf("Error opening file %s for writing\r\n", Lfname.c_str());
			else
				printf("File %s was opened for writing\r\n", Lfname.c_str());

			/* Set pointer to beginning of file: */
			fseek(source, 0L, SEEK_SET);

			while (fgets(buf, BUFSIZ, source))
			{
				string line(buf);

				int j = 0;

				while (j < SqzOpt.nPar)
				{
					sprintf_s(ind, buffsize, "%d", j);	// "task number" is written to char *ind
					string tind(ind);
					string wordToReplace = "$p" + tind;

					while (true)
					{
						size_t pos = line.find(wordToReplace);
						if (pos != string::npos)
						{
							sprintf_s(ind, buffsize, "%8.7f", p[n].X[j] * SqzOpt.X[j]);
							string wordToReplaceWith(ind);
							line.replace(pos, wordToReplace.length(), wordToReplaceWith);
						}
						else
							break;
					}
					j++;
				}

				fwrite(line.c_str(), 1, line.length(), dest);
			}

			fclose(dest);
		}

		fclose(source);
	}
	catch (...)
	{
		E = current_exception();	// "save" current exception and leave the thread
	}
}
//------------------------------------------------------------------------------------------
void Model::WriteOptimisationFileModels_Secant(const char *NameFile, vector<Particle> &p, int nP)
{
	exception_ptr E;	// for handling exceptions thrown by threads

	try
	{
		FILE *source;
		unsigned long result;
		string tempLfname;
		char buf[BUFSIZ];
		size_t len;
		errno_t err;

		tempLfname = NameFile;
		size_t n = tempLfname.find_last_of(".");
		if (n < string::npos)
			tempLfname = tempLfname.substr(0, n);
		else
			tempLfname = tempLfname;

		// Make a copy of Input file
		if ((err = fopen_s(&source, NameFile, "r")) != 0)
			printf("Error opening file %s for writing\r\n", NameFile);
		else
			printf("File %s was opened for writing\r\n", NameFile);

		// Write the Simulation File
		for (int n = 0; n < nP; n++)
		{
			FILE *dest;
			const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
			char ind[buffsize];
			sprintf_s(ind, buffsize, "%d", n);	// "task number" is written to char *ind
			string tind(ind);

			string Lfname = tempLfname + suff + tind + ".dat";

			if ((err = fopen_s(&dest, Lfname.c_str(), "w+")) != 0)
				printf("Error opening file %s for writing\r\n", Lfname.c_str());
			else
				printf("File %s was opened for writing\r\n", Lfname.c_str());

			/* Set pointer to beginning of file: */
			fseek(source, 0L, SEEK_SET);

			while (fgets(buf, BUFSIZ, source))
			{
				string line(buf);

				int j = 0;

				while (j < SqzOpt.nPar)
				{
					sprintf_s(ind, buffsize, "%d", j);	// "task number" is written to char *ind
					string tind(ind);
					string wordToReplace = "$p" + tind;

					while (true)
					{
						size_t pos = line.find(wordToReplace);
						if (pos != string::npos)
						{
							sprintf_s(ind, buffsize, "%8.7f", p[n].X[j] * SqzOpt.X[j]);
							string wordToReplaceWith(ind);
							line.replace(pos, wordToReplace.length(), wordToReplaceWith);
						}
						else
							break;
					}
					j++;
				}

				fwrite(line.c_str(), 1, line.length(), dest);
			}

			fclose(dest);
		}

		fclose(source);
	}
	catch (...)
	{
		E = current_exception();	// "save" current exception and leave the thread
	}
}
//------------------------------------------------------------------------------------------
void Model::WriteOptimisationModels(const char *NameFile, vector<Particle*> p, int nGen)
{
	FILE *filePtr;
	errno_t err;
	int nPopSize;

	nPopSize = p.size();

	string tempLfname;
	tempLfname = NameFile;
	size_t n = tempLfname.find_last_of(".");
	if (n < string::npos)
		tempLfname = tempLfname.substr(0, n);
	else
		tempLfname = tempLfname;

	optimisation_model_fn = vector<string>(nPopSize);

	// Write the Simulation File
	for (int n = 0; n < nPopSize; n++)
	{
		const size_t buffsize = 100;		// the buffer size is hard-coded here, set to 100; BE AWARE!
		char ind[buffsize];
		sprintf_s(ind, buffsize, "%d", n);	// "task number" is written to char *ind
		string tind(ind);

		string Lfname = tempLfname + suff + tind + ".dat";
		optimisation_model_fn[n] = Lfname;

		if ((err = fopen_s(&filePtr, Lfname.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", Lfname.c_str());
		else
			printf("File %s was opened for writing\r\n", Lfname.c_str());

		/* Writes in SQUEEZE V files Single Phase in 1 layer*/
		fprintf_s(filePtr, "SQZV5.0\n"); // distinguishes between SQUEEZE III and SQUEEZE IV data files 
		fprintf_s(filePtr, "%s %d\n", "Optimisation Model %d Generation %d", n, nGen);
		fprintf_s(filePtr, "%s %d\n", "Optimisation Model %d Generation %d", n, nGen);
		fprintf_s(filePtr, "%s\n", "iModel      nEcho       nDebug");
		fprintf_s(filePtr, "%d %d %d \n", iModel, 1, 1);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

		// Radial Read File
		if (iModel == 3)
		{
			fprintf_s(filePtr, "nLay	UnitSpec\n");
			fprintf_s(filePtr, "%d %c\n", nLay, UnitSpec);         //nLay =1 default   

			if (nLay == 1)
			{
				fprintf(filePtr, "Rwell\t\tRmax\t\tHeight\t\tPhi\n");

				fprintf_s(filePtr, "%lf %lf %lf %lf\n", radiusWell,
					radiusMax, SLay[0].HLay, SLay[0].Phi);

				fprintf_s(filePtr, "kChem      kTemp\n");
				fprintf_s(filePtr, "%d %d\n", kChem, kTemp);

				if (kChem == 1)
				{
					fprintf_s(filePtr, "IsoTyp		Neqil2		TolItr\n");
					fprintf_s(filePtr, "%d %d %g\n", Iso.IsoTyp,
						Iso.TypeAds, TolSQ5);

					if (Iso.IsoTyp == 1 || Iso.IsoTyp == 2)
					{
						fprintf_s(filePtr, "a		b		r2(1/day)\n");
						fprintf_s(filePtr, "%lf  %lf %lf\n", Iso.k, Iso.n, Iso.r2);
					}

					if (Iso.IsoTyp == 3)
					{
						fprintf_s(filePtr, "Niso			r2(1/day)\n");
						fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.nIso, Iso.r2);

						fprintf_s(filePtr, "Ciso		ADiso\n");
						for (int j = 0; j < Iso.nIso; j++)
						{
							fprintf(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
						}

					}

					if (Iso.IsoTyp == 4)
					{
						fprintf_s(filePtr, "Cs1		Cs2		Tpc		r4(1/day)\n");
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);

						Iso.TypeAds = 1;
					}

				}		// end of kChem == 1 

			} // end of nLay == 1

			if (nLay > 1)
			{
				fprintf_s(filePtr, "Rwell(ft/m)		Rmax(ft/m)\n");
				fprintf_s(filePtr, "%lf %lf \n", radiusWell, radiusMax);

				fprintf(filePtr, "Height(ft/m)		Phi		EffPerm(md or d)\n");
				for (int l = 0; l<nLay; l++)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", SLay[l].HLay, SLay[l].Phi, SLay[l].KAbs);
				}

				fprintf_s(filePtr, "kChem			kTemp\n");
				fprintf_s(filePtr, "%-11d %-11d\n", kChem, kTemp);

				if (kChem == 1)
				{
					fprintf_s(filePtr, "IsoSpec			TolItr\n");
					fprintf_s(filePtr, "%-11d %.4e\n", IsoSpec, TolSQ5);

					if (IsoSpec == 0)
					{
						fprintf_s(filePtr, "IsoTyp\n");
						fprintf_s(filePtr, "%-11d\n", Iso.IsoTyp);

						fprintf_s(filePtr, "Neqil2	r2(or r4)(1/day)\n");
						for (int l = 0; l < nLay; l++)
							fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.TypeAds, Iso.r2);

						if (Iso.IsoTyp == 4)
							Iso.r4 = Iso.r2;

						fprintf_s(filePtr, "A single isotherm for all layers\n");

						if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
						{
							fprintf_s(filePtr, "k		n\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.k, Iso.n);
						}

						else if (Iso.IsoTyp == 2)
						{
							fprintf_s(filePtr, "a		b\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.k, Iso.n);
						}

						else if (Iso.IsoTyp == 3)
						{
							fprintf_s(filePtr, "NIso\n");
							fprintf_s(filePtr, "%-11d\n", Iso.nIso);

							Iso.Ciso.resize(Iso.nIso);;
							Iso.ADiso.resize(Iso.nIso);;

							fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

							for (int j = 0; j<Iso.nIso; j++)
							{
								fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
							}

						}

						else if (Iso.IsoTyp == 4)
						{
							fprintf(filePtr, "Cps1		Cps2		Tpc\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc);
						}

					}	// End of IsoSpec == 0

					else  // IsoSpec == 1         
					{

						fprintf_s(filePtr, "IsoTyp		Neqil2\n");

						for (int l = 0; l<nLay; l++)
							fprintf_s(filePtr, "%-11d %-11d\n", Iso.IsoTyp, Iso.TypeAds);

						for (int l = 0; l<nLay; l++)
						{
							fprintf_s(filePtr, "Isotherm data for the %d layer\n", l + 1);

							if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
							{
								fprintf_s(filePtr, "k		n		r2(1/day)\n");
								fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
							}

							else if (Iso.IsoTyp == 2)
							{
								fprintf_s(filePtr, "a		b		r2(1/day)\n");
								fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
							}

							else if (Iso.IsoTyp == 3)
							{
								fprintf_s(filePtr, "NIso		r2(1/day)\n");
								fprintf_s(filePtr, "%d %-11.4f\n", Iso.nIso, Iso.r2);

								fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

								for (int j = 0; j<Iso.nIso; j++)
								{
									fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
								}

							}

							else if (Iso.IsoTyp == 4)
							{
								fprintf_s(filePtr, "Cps1		Cps2		Tpc		r4(1/day)\n");
								fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);
							}

						}	// end of for loop Lays	

					}  // end of else IsoSpec ==1   

				}	// end of kChem == 1 

			}	// if nLays > 1	

			if (kTemp == 1)
			{
				if (UnitSpec == 'm' || UnitSpec == 'M')

					fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/m*day*degC)  Kcs   SHr(Btu/kg*degC)  SHw    RHOw(kg/ft3)     RHOr \n");

				else

					fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/ft*day*degC)  Kcs   SHr(Btu/lb*degC)  SHw    RHOw(lb/ft3)    RHOr \n");

				fprintf_s(filePtr, "%-11.4f \t %-11.4f \t %-11.4f %-11.4f %-11.4f %-11.4f %-11.4f\n",
					Temp.Ftemp, Temp.Kcr, Temp.Kcs, Temp.SHr,
					Temp.SHw, Temp.RHOw, Temp.RHOr);
			}

			fprintf_s(filePtr, "nR		Nstages		NIters		Nfreq			NoTstep		IsCon			IsTmp\n");
			fprintf_s(filePtr, "%d %11d %11d %11d %11d %11d %11d\n", nBlocks, nStg, nIters, nFreq, NoTstep, IsCon, IsTmp);

			if (nLay == 1)
			{
				if (UnitSpec == 'm' || UnitSpec == 'M')
				{
					fprintf_s(filePtr, "Qw(cumpm/cumpd)	tspan(h/d)  dt(h/d)	 CIw(ppm)  TI(degC)	OperCode\n");
				}
				else
				{
					fprintf_s(filePtr, "Qw(bpm/bpd)  tspan(h/d)  dt(h/d)   CIw(ppm)  TI(degC)	OperCode\n");
				}

				double watTime = stg[0].Time + stg[1].Time;

				for (int j = 0; j < nStg; j++)
				{
					// Main Treatment
					if (j == 0)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, (p[n]->X[0] / (p[n]->X[0] + p[n]->X[2]))* watTime, stg[j].TimeStep);

						fprintf_s(filePtr, "\t%-11.4lf ", p[n]->X[1] * stg[j].InitialCw[0]);
					}

					// Overflush
					if (j == 1)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, (p[n]->X[2] / (p[n]->X[0] + p[n]->X[2]))* watTime, stg[j].TimeStep);

						fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
					}

					if (j > 1)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

						fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
					}

					fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI, stg[j].TypeStg);

				}

				fprintf_s(filePtr, "iAnim	nLife       kPlot \n");
				fprintf_s(filePtr, "%-11d %-11d %d \n", 1, nLife, kPlot);

				if (nLife > 0)
				{
					fprintf_s(filePtr, "Clife(ppm) \n");
					for (int j = 0; j < nLife - 1; j++)
					{
						fprintf_s(filePtr, "%-11.4f ", Clife[j]);
					}
					fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
				}

				fprintf_s(filePtr, "Field Squeeze Return Data \n");

				fprintf_s(filePtr, "nPts \n");
				fprintf_s(filePtr, "%d \n", 0);

				fprintf_s(filePtr, "nOlds \n");
				fprintf_s(filePtr, "%d \n", 0);

			} // end if nlay == 1

			if (nLay > 1)
			{
				if (UnitSpec == 'm' || UnitSpec == 'M')
				{
					fprintf_s(filePtr, "Qw(cumpm/cumpd)  tspan(h/d)  dt(h/d)\t\t CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
				}
				else
				{
					fprintf_s(filePtr, "Qw(bpm/bpd) tspan(h/d)  dt(h/d)\t\t  CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
				}

				for (int j = 0; j < nStg; j++)
				{
					// Main Treatment
					if (j == 0)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, p[n]->X[0] * stg[j].Time, stg[j].TimeStep);

						fprintf_s(filePtr, "\t%-11.4lf ", p[n]->X[1] * stg[j].InitialCw[0]);
					}

					// Overflush
					if (j == 1)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, p[n]->X[2] * stg[j].Time, stg[j].TimeStep);

						fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
					}

					// Shut-in and onwards 
					if (j > 1)
					{
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

						fprintf_s(filePtr, "%-11.4f ", stg[j].InitialCw[0]);
					}

					fprintf_s(filePtr, "%-11.4f %-11d %-11d %-11d %-11d\n", stg[j].TI,
						stg[j].TypeStg, stg[j].nChage, stg[j].nOpen, stg[j].nMethod);

					if (j == 0 || stg[j].nChage == 1)
					{
						if (stg[j].nOpen > 0)
						{
							int temp = stg[j].nOpen;

							fprintf_s(filePtr, "nLayOp\n");
							if (temp <= 10)
							{
								for (int i = 0; i < temp - 1; i++)
									fprintf_s(filePtr, "%-11d ", stg[j].nLayOp[i]);
								fprintf_s(filePtr, "%-11d \n", stg[j].nLayOp[temp - 1]);

							}
							else
							{
								for (int i = 0; i < 9; i++)
									fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
								fprintf_s(filePtr, "%d \n", stg[j].nLayOp[9]);

								for (int i = 10; i < temp - 1; i++)
									fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
								fprintf_s(filePtr, "%d \n", stg[j].nLayOp[temp - 1]);

							}

							if (stg[j].nMethod == 1)
							{
								int tmp = 0;

								fprintf_s(filePtr, "PIDf\n");
								if (nLay <= 10)
								{
									for (int i = 0; i < nLay - 1; i++)
									{
										if (i == (stg[j].nLayOp[tmp] - 1))
										{
											fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
											++tmp;
										}
									}

									if ((nLay - 1) == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
										++tmp;
									}
								}
								else
								{
									tmp = 0;

									for (int i = 0; i < 9; i++)
									{
										if (i == (stg[j].nLayOp[tmp] - 1))
										{
											fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
											++tmp;
										}
									}

									if (9 == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
										tmp++;
									}

									for (int i = 10; i < nLay - 1; i++)
									{
										if (i == (stg[j].nLayOp[tmp] - 1))
										{
											fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
											tmp++;
										}
									}

									if (nLay - 1 == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
									}

								}

							}  // end of iMethd ==1

						}
						else // stg[j].nOpen == 0
						{
							int temp = nLay;

							if (stg[j].nMethod == 1)
							{
								fprintf_s(filePtr, "PIDf\n");
								if (temp <= 10)
								{
									for (int i = 0; i < temp - 1; i++)
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
								}
								else
								{
									for (int i = 0; i < 9; i++)
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
									fscanf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
									for (int i = 10; i < temp - 1; i++)
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
								}

							}  // end of iMethd ==1

						}	// end of stg[j].nOpen == 0 

					}	// end of j==1 || nChage == 1	
				}

				fprintf_s(filePtr, "nLife       kPlot \n");
				fprintf_s(filePtr, "%-11d %d \n", nLife, kPlot);

				if (nLife > 0)
				{
					fprintf_s(filePtr, "Clife(ppm) \n");
					for (int j = 0; j<nLife - 1; j++)
					{
						fprintf_s(filePtr, "%-11.4f ", Clife[j]);
					}
					fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
				}

				if (kChem == 1)
				{
					fprintf_s(filePtr, "GrafMic\n");
					fprintf_s(filePtr, "%.4f\n", GrafMIC);
				}

				fprintf_s(filePtr, "kCrange	   kGraf1	   kGraf2	   kGraf3\n");
				fprintf_s(filePtr, "%-11d %-11d %-11d %d\n", kCrange, kGraf1, kGraf2, kGraf3);

				if (kCrange > 0)
				{
					fprintf_s(filePtr, "Cmin	      Cmax\n");
					fprintf_s(filePtr, "%-11.4f %.4f\n", Cmin, Cmax);
				}

				if (kGraf2 > 0)
				{
					fprintf_s(filePtr, "nGraf2\n");
					for (int i = 0; i<kGraf2 - 1; i++)
						fprintf_s(filePtr, "%d \t", nGraf2[i]);
					fprintf_s(filePtr, "%d\n", nGraf2[kGraf2 - 1]);
				}

				if (kGraf3 > 0)
				{
					fprintf_s(filePtr, "nGraf3\n");
					for (int i = 0; i<kGraf3 - 1; i++)
						fprintf_s(filePtr, "%d ", nGraf3[i]);
					fprintf_s(filePtr, "%d\n ", nGraf3[kGraf3 - 1]);
				}

				fprintf_s(filePtr, "Field Squeeze Return Data \n");

				fprintf_s(filePtr, "nPts \n");
				fprintf_s(filePtr, "%d \n", 0);

				fprintf_s(filePtr, "nOlds \n");
				fprintf_s(filePtr, "%d \n", 0);

			} // if (nLay > 1)

		} // IsoMod == 3

		fclose(filePtr);
	}
}
//------------------------------------------------------------------------------------------
void Model::WriteIsothermOptimisationModel(const char *NameFile)
{
	FILE *filePtr;
	errno_t err;
	
	// Write the Optimisation File
	if ((err = fopen_s(&filePtr, NameFile, "w")) != 0)
		printf("Error opening file %s for writing\r\n", NameFile);
	else
		printf("File %s was opened for writing\r\n", NameFile);

	/* Writes in SQUEEZE V files Single Phase in 1 layer*/
	fprintf_s(filePtr, "SQZV5.0\n"); // distinguishes between SQUEEZE III and SQUEEZE IV data files 
	fprintf_s(filePtr, "%s %d\n", "Optimisation Format Input Model");
	fprintf_s(filePtr, "%s %d\n", "Isotherm parematers parametrisied");
	fprintf_s(filePtr, "%s\n", "iModel      nEcho       nDebug");
	fprintf_s(filePtr, "%d %d %d \n", iModel, 1, 1);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

	// Radial Read File
	if (iModel == 3)
	{
		fprintf_s(filePtr, "nLay	UnitSpec\n");
		fprintf_s(filePtr, "%d %c\n", nLay, UnitSpec);         //nLay =1 default   

		if (nLay == 1)
		{
			fprintf(filePtr, "Rwell\t\tRmax\t\tHeight\t\tPhi\n");

			fprintf_s(filePtr, "%lf %lf %lf %lf\n", radiusWell,
				radiusMax, SLay[0].HLay, SLay[0].Phi);

			fprintf_s(filePtr, "kChem      kTemp\n");
			fprintf_s(filePtr, "%d %d\n", kChem, kTemp);

			if (kChem == 1)
			{
				fprintf_s(filePtr, "IsoTyp		Neqil2		TolItr\n");
				fprintf_s(filePtr, "%d %d %g\n", Iso.IsoTyp, Iso.TypeAds, TolSQ5);

				if (Iso.IsoTyp == 1 || Iso.IsoTyp == 2)
				{
					fprintf_s(filePtr, "k		n		r2(1/day)\n");
					
					if (Iso.TypeAds == 0) // Equilibrium
					{
						fprintf_s(filePtr, "%s  %s %lf\n", "$p0", "$p1", 0.1);
						SqzOpt.nPar = 2;
					}
					if (Iso.TypeAds == 1) // Kinetic
					{
						fprintf_s(filePtr, "%s  %s %s\n", "$p0", "$p1", "$p2");
						SqzOpt.nPar = 3;
					}
				}

				if (Iso.IsoTyp == 3)
				{
					fprintf_s(filePtr, "Niso			r2(1/day)\n");
					fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.nIso, Iso.r2);

					fprintf_s(filePtr, "Ciso		ADiso\n");
					for (int j = 0; j < Iso.nIso; j++)
					{
						fprintf(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
					}

				}

				if (Iso.IsoTyp == 4)
				{
					fprintf_s(filePtr, "Cs1		Cs2		Tpc		r4(1/day)\n");
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);

					Iso.TypeAds = 1;
				}

			}		// end of kChem == 1 

		} // end of nLay == 1

		if (nLay > 1)
		{
			fprintf_s(filePtr, "Rwell(ft/m)		Rmax(ft/m)\n");
			fprintf_s(filePtr, "%lf %lf \n", radiusWell, radiusMax);

			fprintf(filePtr, "Height(ft/m)		Phi		EffPerm(md or d)\n");
			for (int l = 0; l<nLay; l++)
			{
				fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", SLay[l].HLay, SLay[l].Phi, SLay[l].KAbs);
			}

			fprintf_s(filePtr, "kChem			kTemp\n");
			fprintf_s(filePtr, "%-11d %-11d\n", kChem, kTemp);

			if (kChem == 1)
			{
				fprintf_s(filePtr, "IsoSpec			TolItr\n");
				fprintf_s(filePtr, "%-11d %.4e\n", IsoSpec, TolSQ5);

				if (IsoSpec == 0)
				{
					fprintf_s(filePtr, "IsoTyp\n");
					fprintf_s(filePtr, "%-11d\n", Iso.IsoTyp);

					fprintf_s(filePtr, "Neqil2	r2(or r4)(1/day)\n");
					for (int l = 0; l < nLay; l++)
					{	
						if (Iso.TypeAds == 0) // Equilibrium
						{
							fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.TypeAds, Iso.r2);
							SqzOpt.nPar = 2;
						}

						if (Iso.TypeAds == 1) // Kinetic
						{
							fprintf_s(filePtr, "%-11d %-11s\n", Iso.TypeAds, "$p2");
							SqzOpt.nPar = 3;
						}
					}	
					if (Iso.IsoTyp == 4)
						Iso.r4 = Iso.r2;

					fprintf_s(filePtr, "A single isotherm for all layers\n");

					if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
					{
						fprintf_s(filePtr, "k		n\n");
						fprintf_s(filePtr, "%s %s\n", "$p0", "$p1");
					}

					else if (Iso.IsoTyp == 2)
					{
						fprintf_s(filePtr, "a		b\n");
						fprintf_s(filePtr, "%s %s\n", "$p0", "$p1");
					}

					else if (Iso.IsoTyp == 3)
					{
						fprintf_s(filePtr, "NIso\n");
						fprintf_s(filePtr, "%-11d\n", Iso.nIso);

						Iso.Ciso.resize(Iso.nIso);;
						Iso.ADiso.resize(Iso.nIso);;

						fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

						for (int j = 0; j<Iso.nIso; j++)
						{
							fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
						}

					}

					else if (Iso.IsoTyp == 4)
					{
						fprintf(filePtr, "Cps1		Cps2		Tpc\n");
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc);
					}

				}	// End of IsoSpec == 0

				else  // IsoSpec == 1         
				{

					fprintf_s(filePtr, "IsoTyp		Neqil2\n");

					for (int l = 0; l<nLay; l++)
						fprintf_s(filePtr, "%-11d %-11d\n", Iso.IsoTyp, Iso.TypeAds);

					for (int l = 0; l<nLay; l++)
					{
						fprintf_s(filePtr, "Isotherm data for the %d layer\n", l + 1);

						if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
						{
							fprintf_s(filePtr, "k		n		r2(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
						}

						else if (Iso.IsoTyp == 2)
						{
							fprintf_s(filePtr, "a		b		r2(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
						}

						else if (Iso.IsoTyp == 3)
						{
							fprintf_s(filePtr, "NIso		r2(1/day)\n");
							fprintf_s(filePtr, "%d %-11.4f\n", Iso.nIso, Iso.r2);

							fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

							for (int j = 0; j<Iso.nIso; j++)
							{
								fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
							}

						}

						else if (Iso.IsoTyp == 4)
						{
							fprintf_s(filePtr, "Cps1		Cps2		Tpc		r4(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);
						}

					}	// end of for loop Lays	

				}  // end of else IsoSpec ==1   

			}	// end of kChem == 1 

		}	// if nLays > 1	

		if (kTemp == 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')

				fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/m*day*degC)  Kcs   SHr(Btu/kg*degC)  SHw    RHOw(kg/ft3)     RHOr \n");

			else

				fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/ft*day*degC)  Kcs   SHr(Btu/lb*degC)  SHw    RHOw(lb/ft3)    RHOr \n");

			fprintf_s(filePtr, "%-11.4f \t %-11.4f \t %-11.4f %-11.4f %-11.4f %-11.4f %-11.4f\n",
				Temp.Ftemp, Temp.Kcr, Temp.Kcs, Temp.SHr,
				Temp.SHw, Temp.RHOw, Temp.RHOr);
		}

		fprintf_s(filePtr, "nR		Nstages		NIters		Nfreq			NoTstep		IsCon			IsTmp\n");
		fprintf_s(filePtr, "%d %11d %11d %11d %11d %11d %11d\n", nBlocks, nStg, nIters, nFreq, NoTstep, IsCon, IsTmp);

		if (nLay == 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')
			{
				fprintf_s(filePtr, "Qw(cumpm/cumpd)	tspan(h/d)  dt(h/d)	 CIw(ppm)  TI(degC)	OperCode\n");
			}
			else
			{
				fprintf_s(filePtr, "Qw(bpm/bpd)  tspan(h/d)  dt(h/d)   CIw(ppm)  TI(degC)	OperCode\n");
			}

			double watTime = stg[0].Time + stg[1].Time;

			for (int j = 0; j < nStg; j++)
			{
				// Main Treatment
				if (j == 0)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				// Overflush
				if (j == 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				if (j > 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI, stg[j].TypeStg);

			}

			fprintf_s(filePtr, "iAnim	nLife       kPlot \n");
			fprintf_s(filePtr, "%-11d %-11d %d \n", 1, nLife, kPlot);

			if (nLife > 0)
			{
				fprintf_s(filePtr, "Clife(ppm) \n");
				for (int j = 0; j < nLife - 1; j++)
				{
					fprintf_s(filePtr, "%-11.4f ", Clife[j]);
				}
				fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
			}

			fprintf_s(filePtr, "Field Squeeze Return Data \n");

			fprintf_s(filePtr, "nPts \n");
			fprintf_s(filePtr, "%d \n", 0);

			fprintf_s(filePtr, "nOlds \n");
			fprintf_s(filePtr, "%d \n", 0);

		} // end if nlay == 1

		if (nLay > 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')
			{
				fprintf_s(filePtr, "Qw(cumpm/cumpd)  tspan(h/d)  dt(h/d)\t\t CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
			}
			else
			{
				fprintf_s(filePtr, "Qw(bpm/bpd) tspan(h/d)  dt(h/d)\t\t  CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
			}

			for (int j = 0; j < nStg; j++)
			{
				// Main Treatment
				if (j == 0)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ",stg[j].InitialCw[0]);
				}

				// Overflush
				if (j == 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt,stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				// Shut-in and onwards 
				if (j > 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "%-11.4f ", stg[j].InitialCw[0]);
				}

				fprintf_s(filePtr, "%-11.4f %-11d %-11d %-11d %-11d\n", stg[j].TI,
					stg[j].TypeStg, stg[j].nChage, stg[j].nOpen, stg[j].nMethod);

				if (j == 0 || stg[j].nChage == 1)
				{
					if (stg[j].nOpen > 0)
					{
						int temp = stg[j].nOpen;

						fprintf_s(filePtr, "nLayOp\n");
						if (temp <= 10)
						{
							for (int i = 0; i < temp - 1; i++)
								fprintf_s(filePtr, "%-11d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%-11d \n", stg[j].nLayOp[temp - 1]);

						}
						else
						{
							for (int i = 0; i < 9; i++)
								fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%d \n", stg[j].nLayOp[9]);

							for (int i = 10; i < temp - 1; i++)
								fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%d \n", stg[j].nLayOp[temp - 1]);

						}

						if (stg[j].nMethod == 1)
						{
							int tmp = 0;

							fprintf_s(filePtr, "PIDf\n");
							if (nLay <= 10)
							{
								for (int i = 0; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										++tmp;
									}
								}

								if ((nLay - 1) == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
									++tmp;
								}
							}
							else
							{
								tmp = 0;

								for (int i = 0; i < 9; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										++tmp;
									}
								}

								if (9 == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
									tmp++;
								}

								for (int i = 10; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										tmp++;
									}
								}

								if (nLay - 1 == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
								}

							}

						}  // end of iMethd ==1

					}
					else // stg[j].nOpen == 0
					{
						int temp = nLay;

						if (stg[j].nMethod == 1)
						{
							fprintf_s(filePtr, "PIDf\n");
							if (temp <= 10)
							{
								for (int i = 0; i < temp - 1; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
							}
							else
							{
								for (int i = 0; i < 9; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
								for (int i = 10; i < temp - 1; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
							}

						}  // end of iMethd ==1

					}	// end of stg[j].nOpen == 0 

				}	// end of j==1 || nChage == 1	
			}

			fprintf_s(filePtr, "nLife       kPlot \n");
			fprintf_s(filePtr, "%-11d %d \n", nLife, kPlot);

			if (nLife > 0)
			{
				fprintf_s(filePtr, "Clife(ppm) \n");
				for (int j = 0; j<nLife - 1; j++)
				{
					fprintf_s(filePtr, "%-11.4f ", Clife[j]);
				}
				fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
			}

			if (kChem == 1)
			{
				fprintf_s(filePtr, "GrafMic\n");
				fprintf_s(filePtr, "%.4f\n", GrafMIC);
			}

			fprintf_s(filePtr, "kCrange	   kGraf1	   kGraf2	   kGraf3\n");
			fprintf_s(filePtr, "%-11d %-11d %-11d %d\n", kCrange, kGraf1, kGraf2, kGraf3);

			if (kCrange > 0)
			{
				fprintf_s(filePtr, "Cmin	      Cmax\n");
				fprintf_s(filePtr, "%-11.4f %.4f\n", Cmin, Cmax);
			}

			if (kGraf2 > 0)
			{
				fprintf_s(filePtr, "nGraf2\n");
				for (int i = 0; i<kGraf2 - 1; i++)
					fprintf_s(filePtr, "%d \t", nGraf2[i]);
				fprintf_s(filePtr, "%d\n", nGraf2[kGraf2 - 1]);
			}

			if (kGraf3 > 0)
			{
				fprintf_s(filePtr, "nGraf3\n");
				for (int i = 0; i<kGraf3 - 1; i++)
					fprintf_s(filePtr, "%d ", nGraf3[i]);
				fprintf_s(filePtr, "%d\n ", nGraf3[kGraf3 - 1]);
			}

			fprintf_s(filePtr, "Field Squeeze Return Data \n");

			fprintf_s(filePtr, "nPts \n");
			fprintf_s(filePtr, "%d \n", nPts);
			fprintf_s(filePtr, "%-15s %-10s\n", "t(day/1000bbl)", "c(ppm)");
			for (int j = 0; j < fX.size(); j++)
			{
				fprintf_s(filePtr, "%-15.4f %-15.4f\n", fX[j], fY[j]);
			}

			fprintf_s(filePtr, "nOlds \n");
			fprintf_s(filePtr, "%d \n", 0);

		} // if (nLay > 1)

	} // IsoMod == 3


	fprintf_s(filePtr, "Optimisation Parameters \n");
	fprintf_s(filePtr, "%-13s %-13s %-13s \n", "nData","PopSize", "Generations");
	fprintf_s(filePtr, "%-13d %-13d %-13d \n", SqzOpt.nPar, SqzOpt.nPopSize, SqzOpt.nGen);

	fprintf_s(filePtr, "%-12s %-11s \n", "MIC", "TIME");
	fprintf_s(filePtr, "%-11.4f %-11.4f \n", 10, 100);

	fprintf_s(filePtr, "%-11s %-11s %-11s %-11s\n", "Parameter", "Original", "Lower", "Upper");
	fprintf_s(filePtr, "%-11d %-11.4f %-11.4f %-11.4f \n", 0, Iso.k, SqzOpt.lX[0], SqzOpt.uX[0]);
	fprintf_s(filePtr, "%-11d %-11.4f %-11.4f %-11.4f \n", 1, Iso.n, SqzOpt.lX[1], SqzOpt.uX[1]);
	if (SqzOpt.nPar == 3)
		fprintf_s(filePtr, "%-11d %-11.4f %-11.4f %-11.4f \n", 2, Iso.r2, SqzOpt.lX[2], SqzOpt.uX[2]);
	fprintf_s(filePtr, "%-30s %-13s %-30s %-30s %-30s %-30s %-13s %-18s %-18s\n", "Sens.Calc(=0 Conv, =1 Ope)", "nBins", "TotSIVol", "TotInjVol", "Only_SI,=0 y,=1 N", "Only_Tot_Vol,=0 y,=1 N", "FixedTarget", "BinsMT", "IterationsSecant");
	fprintf_s(filePtr, "%-30d %-13d %-11.4f %-11.4f %-30d %-30d %-13d %-15d %-15d\n", SqzOpt.SensCalc, SqzOpt.nBins, SqzOpt.TotSIVol, SqzOpt.TotInjVol, SqzOpt.Only_SI, SqzOpt.Only_Tot_Vol, SqzOpt.Fixed_Target, SqzOpt.BinsMT, SqzOpt.IterationsSecant);

	fclose(filePtr);

}
//------------------------------------------------------------------------------------------
void Model::WriteFileSQZ5Radial(const char *NameFile)
{
	FILE *filePtr;
	errno_t err;

	// Write the Optimisation File
	if ((err = fopen_s(&filePtr, NameFile, "w")) != 0)
		printf("Error opening file %s for writing\r\n", NameFile);
	else
		printf("File %s was opened for writing\r\n", NameFile);

	/* Writes in SQUEEZE V files Single Phase in 1 layer*/
	fprintf_s(filePtr, "SQZV5.0\n"); // distinguishes between SQUEEZE III and SQUEEZE IV data files 
	fprintf_s(filePtr, "%s\n", "Optimisation Format Input Model");
	fprintf_s(filePtr, "%s\n", "Isotherm parematers parametrisied");
	fprintf_s(filePtr, "%s\n", "iModel      nEcho       nDebug");
	fprintf_s(filePtr, "%d %d %d \n", iModel, 1, 1);	// before &temp was in place of &temp1; it is not good to write simultaneously to the same address - ILYA

	// Radial Read File
	if (iModel == 3)
	{
		fprintf_s(filePtr, "nLay	UnitSpec\n");
		fprintf_s(filePtr, "%d %c\n", nLay, UnitSpec);         //nLay =1 default   

		if (nLay == 1)
		{
			fprintf(filePtr, "Rwell\t\tRmax\t\tHeight\t\tPhi\n");

			fprintf_s(filePtr, "%lf %lf %lf %lf\n", radiusWell,
				radiusMax, SLay[0].HLay, SLay[0].Phi);

			fprintf_s(filePtr, "kChem      kTemp\n");
			fprintf_s(filePtr, "%d %d\n", kChem, kTemp);

			if (kChem == 1)
			{
				fprintf_s(filePtr, "IsoTyp		Neqil2		TolItr\n");
				fprintf_s(filePtr, "%d %d %g\n", Iso.IsoTyp, Iso.TypeAds, TolSQ5);

				if (Iso.IsoTyp == 1 || Iso.IsoTyp == 2)
				{
					fprintf_s(filePtr, "k		n		r2(1/day)\n");

					if (Iso.TypeAds == 0) // Equilibrium
					{
						fprintf_s(filePtr, "%-11.4f  %-11.4f %-11.4f\n", Iso.k, Iso.n, 0.1);
					}
					if (Iso.TypeAds == 1) // Kinetic
					{
						fprintf_s(filePtr, "%-11.4f  %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
					}
				}

				if (Iso.IsoTyp == 3)
				{
					fprintf_s(filePtr, "Niso			r2(1/day)\n");
					fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.nIso, Iso.r2);

					fprintf_s(filePtr, "Ciso		ADiso\n");
					for (int j = 0; j < Iso.nIso; j++)
					{
						fprintf(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
					}

				}

				if (Iso.IsoTyp == 4)
				{
					fprintf_s(filePtr, "Cs1		Cs2		Tpc		r4(1/day)\n");
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);

					Iso.TypeAds = 1;
				}

			}		// end of kChem == 1 

		} // end of nLay == 1

		if (nLay > 1)
		{
			fprintf_s(filePtr, "Rwell(ft/m)		Rmax(ft/m)\n");
			fprintf_s(filePtr, "%lf %lf \n", radiusWell, radiusMax);

			fprintf(filePtr, "Height(ft/m)		Phi		EffPerm(md or d)\n");
			for (int l = 0; l<nLay; l++)
			{
				fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", SLay[l].HLay, SLay[l].Phi, SLay[l].KAbs);
			}

			fprintf_s(filePtr, "kChem			kTemp\n");
			fprintf_s(filePtr, "%-11d %-11d\n", kChem, kTemp);

			if (kChem == 1)
			{
				fprintf_s(filePtr, "IsoSpec			TolItr\n");
				fprintf_s(filePtr, "%-11d %.4e\n", IsoSpec, TolSQ5);

				if (IsoSpec == 0)
				{
					fprintf_s(filePtr, "IsoTyp\n");
					fprintf_s(filePtr, "%-11d\n", Iso.IsoTyp);

					fprintf_s(filePtr, "Neqil2	r2(or r4)(1/day)\n");
					for (int l = 0; l < nLay; l++)
					{
						if (Iso.TypeAds == 0) // Equilibrium
						{
							fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.TypeAds, Iso.r2);
						}

						if (Iso.TypeAds == 1) // Kinetic
						{
							fprintf_s(filePtr, "%-11d %-11.4f\n", Iso.TypeAds, Iso.r2);
						}
					}
					if (Iso.IsoTyp == 4)
						Iso.r4 = Iso.r2;

					fprintf_s(filePtr, "A single isotherm for all layers\n");

					if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
					{
						fprintf_s(filePtr, "k		n\n");
						fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.k, Iso.n);
					}

					else if (Iso.IsoTyp == 2)
					{
						fprintf_s(filePtr, "a		b\n");
						fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.k, Iso.n);
					}

					else if (Iso.IsoTyp == 3)
					{
						fprintf_s(filePtr, "NIso\n");
						fprintf_s(filePtr, "%-11d\n", Iso.nIso);

						Iso.Ciso.resize(Iso.nIso);
						Iso.ADiso.resize(Iso.nIso);;

						fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

						for (int j = 0; j<Iso.nIso; j++)
						{
							fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
						}

					}

					else if (Iso.IsoTyp == 4)
					{
						fprintf(filePtr, "Cps1		Cps2		Tpc\n");
						fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc);
					}

				}	// End of IsoSpec == 0

				else  // IsoSpec == 1         
				{

					fprintf_s(filePtr, "IsoTyp		Neqil2\n");

					for (int l = 0; l<nLay; l++)
						fprintf_s(filePtr, "%-11d %-11d\n", Iso.IsoTyp, Iso.TypeAds);

					for (int l = 0; l<nLay; l++)
					{
						fprintf_s(filePtr, "Isotherm data for the %d layer\n", l + 1);

						if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
						{
							fprintf_s(filePtr, "k		n		r2(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
						}

						else if (Iso.IsoTyp == 2)
						{
							fprintf_s(filePtr, "a		b		r2(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f\n", Iso.k, Iso.n, Iso.r2);
						}

						else if (Iso.IsoTyp == 3)
						{
							fprintf_s(filePtr, "NIso		r2(1/day)\n");
							fprintf_s(filePtr, "%d %-11.4f\n", Iso.nIso, Iso.r2);

							fprintf_s(filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");

							for (int j = 0; j<Iso.nIso; j++)
							{
								fprintf_s(filePtr, "%-11.4f %-11.4f\n", Iso.Ciso[j], Iso.ADiso[j]);
							}

						}

						else if (Iso.IsoTyp == 4)
						{
							fprintf_s(filePtr, "Cps1		Cps2		Tpc		r4(1/day)\n");
							fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f %-11.4f\n", Iso.Cps1, Iso.Cps2, Iso.Tpc, Iso.r4);
						}

					}	// end of for loop Lays	

				}  // end of else IsoSpec ==1   

			}	// end of kChem == 1 

		}	// if nLays > 1	

		if (kTemp == 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')

				fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/m*day*degC)  Kcs   SHr(Btu/kg*degC)  SHw    RHOw(kg/ft3)     RHOr \n");

			else

				fprintf_s(filePtr, "Ftemp(degC) Kcr(Btu/ft*day*degC)  Kcs   SHr(Btu/lb*degC)  SHw    RHOw(lb/ft3)    RHOr \n");

			fprintf_s(filePtr, "%-11.4f \t %-11.4f \t %-11.4f %-11.4f %-11.4f %-11.4f %-11.4f\n",
				Temp.Ftemp, Temp.Kcr, Temp.Kcs, Temp.SHr,
				Temp.SHw, Temp.RHOw, Temp.RHOr);
		}

		fprintf_s(filePtr, "nR		Nstages		NIters		Nfreq			NoTstep		IsCon			IsTmp\n");
		fprintf_s(filePtr, "%d %11d %11d %11d %11d %11d %11d\n", nBlocks, nStg, nIters, nFreq, NoTstep, IsCon, IsTmp);

		if (nLay == 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')
			{
				fprintf_s(filePtr, "Qw(cumpm/cumpd)	tspan(h/d)  dt(h/d)	 CIw(ppm)  TI(degC)	OperCode\n");
			}
			else
			{
				fprintf_s(filePtr, "Qw(bpm/bpd)  tspan(h/d)  dt(h/d)   CIw(ppm)  TI(degC)	OperCode\n");
			}

			for (int j = 0; j < nStg; j++)
			{
				// Main Treatment
				if (j == 0)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				// Overflush
				if (j == 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				if (j > 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI, stg[j].TypeStg);

			}

			fprintf_s(filePtr, "iAnim	nLife       kPlot \n");
			fprintf_s(filePtr, "%-11d %-11d %d \n", 1, nLife, kPlot);

			if (nLife > 0)
			{
				fprintf_s(filePtr, "Clife(ppm) \n");
				for (int j = 0; j < nLife - 1; j++)
				{
					fprintf_s(filePtr, "%-11.4f ", Clife[j]);
				}
				fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
			}

		} // end if nlay == 1

		if (nLay > 1)
		{
			if (UnitSpec == 'm' || UnitSpec == 'M')
			{
				fprintf_s(filePtr, "Qw(cumpm/cumpd)  tspan(h/d)  dt(h/d)\t\t CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
			}
			else
			{
				fprintf_s(filePtr, "Qw(bpm/bpd) tspan(h/d)  dt(h/d)\t\t  CIw(ppm)  TI(degC)    OperCode    nChage	   nOpen	   nMethod\n");
			}

			for (int j = 0; j < nStg; j++)
			{
				// Main Treatment
				if (j == 0)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				// Overflush
				if (j == 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.8f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw[0]);
				}

				// Shut-in and onwards 
				if (j > 1)
				{
					fprintf_s(filePtr, "%-11.4f %-11.4f %-11.4f", stg[j].Qwt, stg[j].Time, stg[j].TimeStep);

					fprintf_s(filePtr, "%-11.4f ", stg[j].InitialCw[0]);
				}

				fprintf_s(filePtr, "%-11.4f %-11d %-11d %-11d %-11d\n", stg[j].TI,
					stg[j].TypeStg, stg[j].nChage, stg[j].nOpen, stg[j].nMethod);

				if (j == 0 || stg[j].nChage == 1)
				{
					if (stg[j].nOpen > 0)
					{
						int temp = stg[j].nOpen;

						fprintf_s(filePtr, "nLayOp\n");
						if (temp <= 10)
						{
							for (int i = 0; i < temp - 1; i++)
								fprintf_s(filePtr, "%-11d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%-11d \n", stg[j].nLayOp[temp - 1]);

						}
						else
						{
							for (int i = 0; i < 9; i++)
								fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%d \n", stg[j].nLayOp[9]);

							for (int i = 10; i < temp - 1; i++)
								fprintf_s(filePtr, "%d ", stg[j].nLayOp[i]);
							fprintf_s(filePtr, "%d \n", stg[j].nLayOp[temp - 1]);

						}

						if (stg[j].nMethod == 1)
						{
							int tmp = 0;

							fprintf_s(filePtr, "PIDf\n");
							if (nLay <= 10)
							{
								for (int i = 0; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										++tmp;
									}
								}

								if ((nLay - 1) == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
									++tmp;
								}
							}
							else
							{
								tmp = 0;

								for (int i = 0; i < 9; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										++tmp;
									}
								}

								if (9 == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
									tmp++;
								}

								for (int i = 10; i < nLay - 1; i++)
								{
									if (i == (stg[j].nLayOp[tmp] - 1))
									{
										fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
										tmp++;
									}
								}

								if (nLay - 1 == (stg[j].nLayOp[tmp] - 1))
								{
									fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[nLay - 1]);
								}

							}

						}  // end of iMethd ==1

					}
					else // stg[j].nOpen == 0
					{
						int temp = nLay;

						if (stg[j].nMethod == 1)
						{
							fprintf_s(filePtr, "PIDf\n");
							if (temp <= 10)
							{
								for (int i = 0; i < temp - 1; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
							}
							else
							{
								for (int i = 0; i < 9; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fscanf_s(filePtr, "%-11.4f \n", stg[j].PIDF[9]);
								for (int i = 10; i < temp - 1; i++)
									fprintf_s(filePtr, "%-11.4f ", stg[j].PIDF[i]);
								fprintf_s(filePtr, "%-11.4f \n", stg[j].PIDF[temp - 1]);
							}

						}  // end of iMethd ==1

					}	// end of stg[j].nOpen == 0 

				}	// end of j==1 || nChage == 1	
			}

			fprintf_s(filePtr, "nLife       kPlot \n");
			fprintf_s(filePtr, "%-11d %d \n", nLife, kPlot);

			if (nLife > 0)
			{
				fprintf_s(filePtr, "Clife(ppm) \n");
				for (int j = 0; j<nLife - 1; j++)
				{
					fprintf_s(filePtr, "%-11.4f ", Clife[j]);
				}
				fprintf_s(filePtr, "%.4f \n", Clife[nLife - 1]);
			}

			if (kChem == 1)
			{
				fprintf_s(filePtr, "GrafMic\n");
				fprintf_s(filePtr, "%.4f\n", GrafMIC);
			}

			fprintf_s(filePtr, "kCrange	   kGraf1	   kGraf2	   kGraf3\n");
			fprintf_s(filePtr, "%-11d %-11d %-11d %d\n", kCrange, kGraf1, kGraf2, kGraf3);

			if (kCrange > 0)
			{
				fprintf_s(filePtr, "Cmin	      Cmax\n");
				fprintf_s(filePtr, "%-11.4f %.4f\n", Cmin, Cmax);
			}

			if (kGraf2 > 0)
			{
				fprintf_s(filePtr, "nGraf2\n");
				for (int i = 0; i<kGraf2 - 1; i++)
					fprintf_s(filePtr, "%d \t", nGraf2[i]);
				fprintf_s(filePtr, "%d\n", nGraf2[kGraf2 - 1]);
			}

			if (kGraf3 > 0)
			{
				fprintf_s(filePtr, "nGraf3\n");
				for (int i = 0; i<kGraf3 - 1; i++)
					fprintf_s(filePtr, "%d ", nGraf3[i]);
				fprintf_s(filePtr, "%d\n ", nGraf3[kGraf3 - 1]);
			}
			
		} // if (nLay > 1)

	} // IsoMod == 3

	fprintf_s(filePtr, "Field Squeeze Return Data \n");

	fprintf_s(filePtr, "nPts \n");
	fprintf_s(filePtr, "%d \n", nPts);

	if (nPts > 0)
	{
		fprintf_s(filePtr, "%-15s %-10s\n", "t(day/1000bbl)", "c(ppm)");
		for (int j = 0; j < fX.size(); j++)
		{
			fprintf_s(filePtr, "%-15.4f %-15.4f\n", fX[j], fY[j]);
		}
	}

	fprintf_s(filePtr, "nOlds \n");
	fprintf_s(filePtr, "%d \n", nOlds);

	if (nOlds > 0)
	{	
		fprintf_s(filePtr, "Old File Names \n");
		for (int i = 0; i < FileNm.size(); i++)
			fprintf_s(filePtr, "%s \n", FileNm[i]);
	}

	fflush(filePtr);

	fprintf_s(filePtr, "Optimisation Parameters \n");
	fprintf_s(filePtr, "%-13s %-13s %-13s \n", "nData", "PopSize", "Generations");
	fprintf_s(filePtr, "%-13d %-13d %-13d \n", SqzOpt.nPar, SqzOpt.nPopSize, SqzOpt.nGen);

	fprintf_s(filePtr, "%-12s %-11s \n", "MIC", "TIME/VOL");
	fprintf_s(filePtr, "%-11.4f %-11.4f \n", SqzOpt.MIC, SqzOpt.Obj_T_V);

	fprintf_s(filePtr, "%-11s %-11s %-11s %-11s\n", "Parameter", "Original", "Lower", "Upper");
	for (int j = 0; j < SqzOpt.nPar; j++)
		fprintf_s(filePtr, "%-11d %-11.4f %-11.4f %-11.4f \n", j, SqzOpt.X[j], SqzOpt.lX[j], SqzOpt.uX[j]);
	
	int index = 0;
	fprintf_s(filePtr, "Qw(bpm/bpd)  tspan(h/d)  dt(h/d)   CIw(ppm)  TI(degC)	OperCode\n");
	for (int j = 0; j < nStg; j++)
	{	
		if (SqzOpt.nPar == 0)
		{
			stg[j].objTime = 0;
			stg[j].objSI = 0;
		}

		if (stg[j].TypeStg != 3 && stg[j].TypeStg != 5) // No Shut-in Nor production stage
		{
			fprintf_s(filePtr, "%-11.4f ", stg[j].Qwt);

			if (stg[j].objTime == 1)
			{
				fprintf_s(filePtr, "%s%d ", "$P", index);
				index++;
			}
			else
				fprintf_s(filePtr, "%-11.4f ", stg[j].Time);

			fprintf_s(filePtr, "%-11.4f ", stg[j].TimeStep);

			if (stg[j].objSI == 1)
			{
				fprintf_s(filePtr, "%s%d ", "$P", index);
				index++;
			}
			else
				fprintf_s(filePtr, "\t%-11.4lf ", stg[j].InitialCw);

			fprintf_s(filePtr, "%-11.4f %-11d \n", stg[j].TI,
				stg[j].TypeStg);
		}
	}
	
	fprintf_s(filePtr, "%-30s %-13s %-30s %-30s %-30s %-30s %-13s %-18s %-18s\n", "Sens.Calc(=0 Conv, =1 Ope)", "nBins", "TotSIVol", "TotInjVol", "Only_SI,=0 y,=1 N", "Only_Tot_Vol,=0 y,=1 N", "FixedTarget", "BinsMT", "IterationsSecant");
	fprintf_s(filePtr, "%-30d %-13d %-11.4f %-11.4f %-30d %-30d %-13d %-15d %-15d\n", SqzOpt.SensCalc, SqzOpt.nBins, SqzOpt.TotSIVol, SqzOpt.TotInjVol, SqzOpt.Only_SI, SqzOpt.Only_Tot_Vol, SqzOpt.Fixed_Target, SqzOpt.BinsMT, SqzOpt.IterationsSecant);

	fclose(filePtr);

}
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------
/* http://www.geekpedia.com/tutorial38_Searching-for-a-string-in-a-File.html */
unsigned long * Model::GetPositionsFile(FILE* pFile, const char* lpszSearchString, unsigned long ulCurrentPosition, unsigned long &n)
{
	unsigned long * array, currentPosition, result;
	char line[1000];
	n = 0;
	array = new unsigned long[n + 1];

	currentPosition = FileSearch(pFile, lpszSearchString, ulCurrentPosition);

	while (currentPosition != -1)
	{
		result = fseek(pFile, currentPosition, SEEK_SET);
		if (result)
			perror("Fseek failed");
		else
		{
			fgets(line, 1000, pFile);
		}

		array[n] = currentPosition;
		unsigned long *temp, i;
		temp = new unsigned long[n + 1];
		for (i = 0; i < n + 1; i++) temp[i] = array[i];
		delete[] array;
		n++;
		array = new unsigned long[n + 1];
		for (i = 0; i < n; i++) array[i] = temp[i];
		delete[] temp;
		currentPosition = FileSearch(pFile, lpszSearchString, currentPosition + strlen(lpszSearchString));
	}

	return array;
}

//------------------------------------------------------------------------------------------
unsigned long Model::FileSearch(FILE* pFile, const char* lpszSearchString, unsigned long ulCurrentPosition)
{
	//make sure we were passed a valid, if it isn t return -1 
	if ((!pFile) || (!lpszSearchString))
	{
		return -1;
	}

	unsigned long ulFileSize = 0;

	//get the size of the file 
	fseek(pFile, 0, SEEK_END);

	ulFileSize = ftell(pFile);

	fseek(pFile, 0, SEEK_SET);

	//if the file is empty return -1 
	if (!ulFileSize)
	{
		return -1;
	}

	//get the length of the string we re looking for, this is 
	//the size the buffer will need to be 
	unsigned long ulBufferSize = strlen(lpszSearchString);

	if (ulBufferSize>ulFileSize)
	{
		return -1;
	}

	//allocate the memory for the buffer 
	char* lpBuffer = (char*)malloc(ulBufferSize);

	//if malloc() returned a null pointer (which probably means 
	//there is not enough memory) then return -1 
	if (!lpBuffer)
	{
		return -1;
	}

	//this is where the actual searching will happen, what happens 
	//here is we set the file pointer to the current position 
	//is incrimented by one each pass, then we read the size of 
	//the buffer into the buffer and compare it with the string 
	//we re searching for, if the string is found we return the 
	//position at which it is found 
	while (ulCurrentPosition<ulFileSize - ulBufferSize)
	{
		//set the pointer to the current position 
		fseek(pFile, ulCurrentPosition, SEEK_SET);

		//read ulBufferSize bytes from the file 
		fread(lpBuffer, 1, ulBufferSize, pFile);

		//if the data read matches the string we re looking for 
		if (!memcmp(lpBuffer, lpszSearchString, ulBufferSize))
		{
			//free the buffer 
			free(lpBuffer);

			//return the position the string was found at 
			return ulCurrentPosition;
		}

		//incriment the current position by one 
		ulCurrentPosition++;
	}

	//if we made it this far the string was not found in the file 
	//so we free the buffer 
	free(lpBuffer);

	//and return -1 
	return -1;
}
//------------------------------------------------------------------------------------------
void Model::ReadFileSQZ5Linear(const char* NameFile)
{
	FILE* filePtr;

	int l,
		j,
		temp; // Number of Components

	const size_t BUFFSIZE = 4096;
	char buffer[BUFFSIZE];

	if ((filePtr = fopen(NameFile, "r")) == NULL)
		printf("The file %s was not opened\n", NameFile);
	else
		printf("The file %s was opened\n", NameFile);

	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);

	// validate the SQUEEZE data file and check the version
	fscanf(filePtr, "%s\n", buffer);

	if (strncmp(buffer, "SQZV5.0", 7) != 0 && strncmp(buffer, "sqzv5.0", 7) != 0)
	{
		AfxMessageBox("This is not a SQUEEZE VI data file. Please select a SQUEEZE V file again.");
		std::fclose(filePtr);
	}

	/* Reads in SQUEEZE V files */
	/* read in variables */
	fgets(buffer, 128, filePtr);
	title1.Format("%s", buffer);
	fgets(buffer, 128, filePtr);
	title2.Format("%s", buffer);

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%d %d %d \n", &iModel, &temp, &temp);

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%lf %lf %lf\n", &xL, &Dcor, &phi);

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%d %d %lf\n", &Iso.IsoTyp,&Iso.TypeAds,&TolSQ5);

	if (Iso.IsoTyp == 1)     // IsoTyp = pchem->m_IsoTyp+1;
	{
		fgets(buffer, 128, filePtr);
		fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
	}

	else if (Iso.IsoTyp == 2)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%lf %lf %lf\n", &Iso.k, &Iso.n, &Iso.r2);
	}

	else if (Iso.IsoTyp == 3)
	{
		fgets(buffer, BUFFSIZE, filePtr);
		fscanf_s(filePtr, "%d %lf\n", &Iso.nIso, &Iso.r2);

		++Iso.nIso;

		Iso.Ciso.resize(Iso.nIso);
		Iso.ADiso.resize(Iso.nIso);

		fgets(buffer, BUFFSIZE, filePtr);

		Iso.Ciso[0] = 0;
		Iso.ADiso[0] = 0;

		for (j = 1; j < Iso.nIso; j++)
		{
			fscanf_s(filePtr, "%lf %lf\n", &Iso.Ciso[j], &Iso.ADiso[j]);
		}
	}
	
	fgets(buffer, BUFFSIZE, filePtr);
	fscanf_s(filePtr, "%d %d %d %d %d %d %d\n", &nBlocks, &nStg, &nIters, &nFreq, &NoTstep, &IsCon, &IsTmp);

	stg.resize(nStg);

	// Reading Stages 
	fgets(buffer, 128, filePtr);

	for (j = 0; j < nStg; j++)
	{
		stg[j].InitialCw.resize(nComponentsWater);
		stg[j].InitialCo.resize(nComponentsWater);

		fscanf(filePtr, "%lf %lf ", &stg[j].Qwt, &stg[j].Time);

		if (NoTstep == 0)

			fscanf(filePtr, "%lf", &stg[j].TimeStep);

		fscanf(filePtr, "%lf", &stg[j].InitialCw[0]);

		fscanf(filePtr, "%lf %d\n", &stg[j].LinDisp, &stg[j].TypeStg);

	}   // end of for loop nstage

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%d %d\n", &iAnim, &nLife);

	if (nLife > 0)
	{
		Clife.resize(nLife);

		fgets(buffer, 128, filePtr);
		for (j = 0; j < nLife - 1; j++)
		{
			fscanf(filePtr, "%lf ", &Clife[j]);
		}
		fscanf(filePtr, "%lf \n", &Clife[nLife - 1]);
	}

	fgets(buffer, 128, filePtr);
	fgets(buffer, 128, filePtr);
		
	fscanf(filePtr, "%d \n", &nPts);
	
	fX.resize(nPts);
	fY.resize(nPts);

	fgets(buffer, 128, filePtr);
	for (int j = 0 ; j < nPts ; j++ )
	{	
		double tx, ty;
		fscanf(filePtr, "%lf %lf \n", &tx, &ty);
		fX[j] = tx;
		fY[j] = ty;
	}
	

	return;
}
//------------------------------------------------------------------------------------------
void Model::ReadFileSQZ5Isotherm(const char* NameFile)
{
	FILE* filePtr;

	int l,
		j,
		temp; // Number of Components

	char buffer[128];

	if ((filePtr = fopen(NameFile, "r")) == NULL)
		printf("The file %s was not opened\n", NameFile);
	else
		printf("The file %s was opened\n", NameFile);

	/* Set pointer to beginning of file: */
	fseek(filePtr, 0L, SEEK_SET);

	// validate the SQUEEZE data file and check the version
	fscanf(filePtr, "%s\n", buffer);

	if (strncmp(buffer, "SQZV5.0", 7) != 0 && strncmp(buffer, "sqzv5.0", 7) != 0)
	{
		AfxMessageBox("This is not a SQUEEZE VI data file. Please select a SQUEEZE V file again.");
		std::fclose(filePtr);
	}

	/* Reads in SQUEEZE V files */
	/* read in variables */
	fgets(buffer, 128, filePtr);
	title1.Format("%s", buffer);
	fgets(buffer, 128, filePtr);
	title2.Format("%s", buffer);

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%d %d %d \n", &iModel, &temp, &temp);
	
	fgets(buffer, 128, filePtr);

	fscanf(filePtr, "%lf %lf %lf %lf\n", &xL, &Dcor, &phi,&rho);

	fgets(buffer, 128, filePtr);
	fscanf(filePtr, "%lf %lf %lf %lf\n", &VI, &VD,&CI, &Cshut);

	if (iModel == 5)
	{
		fgets(buffer, 128, filePtr);
		fscanf(filePtr, "%lf \n", &TolSQ5);

		fgets(buffer, 128, filePtr);
		fscanf(filePtr, "%d %d %d %d %d %d %d\n", &nBlocks,	&nStg, &nIters, &nFreq, &NoTstep, &IsCon, &temp);

		stg.resize(nStg);
		
		// Reading Stages 
		fgets(buffer, 128, filePtr);

		for (j = 0; j < nStg; j++)
		{
			fscanf(filePtr, "%lf %lf ", &stg[j].Qwt, &stg[j].Time);

			if (NoTstep == 0)

				fscanf(filePtr, "%lf", &stg[j].TimeStep);

			fscanf(filePtr, "%lf", &stg[j].InitialCw[0]);

			fscanf(filePtr, "%lf %d\n", &stg[j].LinDisp, &stg[j].TypeStg);

		}   // end of for loop nstage

		fgets(buffer, 128, filePtr);
		fscanf(filePtr, "%d %d\n", &iAnim, &nLife);

		if (nLife > 0)
		{	
			Clife.resize(nLife);

			fgets(buffer, 128, filePtr);
			for (j = 0; j < nLife - 1; j++)
			{
				fscanf(filePtr, "%lf ", &Clife[j]);
			}
			fscanf(filePtr, "%lf \n", &Clife[nLife - 1]);
		}
	}

	fgets(buffer, 128, filePtr);
	fgets(buffer, 128, filePtr);

	j = 0;
	double	tempx,
			tempy;
	while (fscanf(filePtr, "%lf %lf \n", &tempx, &tempy) != EOF)
	{	
		fX.push_back(tempx);
		fY.push_back(tempy);
		++j;
	}

	nPts = fX.size();

	return;
}
//------------------------------------------------------------------------------------------
void Model::IsothermDerivation(const char* fname)
{	
	/*
	PVtot - cumulative pore volumes of brine throughput the core (including chemical slug and brine postflush).
	PVpf - cumulative pore volumes of brine postflush.
	PVpf2 - cumulative pore volumes of brine postflush,
	PVpf2 = PVpf when PVpf > 1.
	Ceff - effluent sample chemical concentration(ppm) during chemical slug injection and postflush, corresponds to PVtot.
	Cpf - effluent sample chemical concentration(ppm) during postflush, corresponds to PVpf.
	Cpf2 - effluent sample chemical concentration(ppm) during postflush, corresponds to PVpf2.Cpf2 = Cpf when PVpf > 1.
	dGdC - adsorption isotherm derivative at Cpf2(i.e.Ciso).
	Ciso0 - i.e.Cpf2, the mobile concentrations in the isotherm.
	Ciso - as Ciso, but arranged from low to high.
	Giso0 - chemical adsorption level(mg per litre of rock grains) at corresonding Ciso initially determined from chemical 		c             effluent concentration data.
	Giso - as Giso0 but arranged from low to high, correspons to Ciso
	Giso2 - chemical adsorption level(mg per g of rock) at corresponding Ciso1, converted from Giso.
	GG - i.e. (Giso0 - Gmax1), where Gmax1 is the maximum adsorption (mg / L) calculated from the injection front(including 			c             any shut - in before postflush) chemical concentration data.
	*/
	
	ReadFileSQZ5Isotherm(fname);

	int N1, 
		N2, 
		N3,
		N4;

	double	Fac,
			aCore,
			PV,
			PVt,
			PV1,
			PVshut,
			PVend,
			PVpf2,
			C0,
			Gmax1,
			Gmax2;

	vector<double>	CmoIso,
					CadIso,
					PVpf,
					Cpf,
					PVtot,
					Ceff,
					Cpf2,
					dGdC,
					GG,
					Ciso0,
					Giso0;

	Fac = phi / (1.0 - phi);
	aCore = 0.25 * 3.1415926 * Dcor * Dcor;
	PV = aCore * xL * phi;
	PVt = VI / PV;

	PV1 = (VI - VD) / PV;

	C0 = CI;

	CmoIso.push_back(0);
	CadIso.push_back(0);

	// It works out numbers of samples from injection(N2) and from postflush(N3)
	N1 = fX.size();
	PVtot = fX;
	Ceff = fY;

	for (int i = 0; i < N1; i++)
	{	
		if (PV1 < PVtot[i] && PV1 >= PVtot[i - 1])
		
			N2 = i - 1;

		if ( PV1 < PVtot[i] )
		{	
			PVpf.push_back(PVtot[i]-PV1);
			Cpf.push_back(Ceff[i]);
		}

	}
	
	// Calculates maximum adsorption, Gmax, from injection front (including adsorption shut - in)
	double PVeff = 0.;

	for (int l = 0; l < N1; l++)
	{
		if (l == 0) PVeff = 0.5 * PVtot[l] * Ceff[l] / C0;

		else PVeff = PVeff + 0.5 * (PVtot[l] - PVtot[l - 1]) * (Ceff[l - 1] + Ceff[l]) / C0;

		if (l == N2) PVshut = PVeff;

	}

	PVend = PVeff;
	PVeff = PVshut + (PV1 - PVtot[N2]) * Ceff[N2] / C0;

	// Gmax1 in mg per litre of rock grains
	// Gmax2 in mg per g rock

	Gmax1 = Fac * (PV1 - PVeff - Cshut / C0) * C0;
	Gmax2 = Gmax1 / rho / 1000.0;

	// Works out number of samples, N4, from postflush PVpf > 1.0 to
	// end of flood and then proceeds to derive adsorption isotherm.
	int m;
	bool check = true;
	for (int j = 0; j < PVpf.size(); j++)
	{ 
		if (PVpf[j] > 1.0) 
		{ 
			PVpf2 = PVpf[j]-1.0;
			Cpf2.push_back(Cpf[j]);
			dGdC.push_back(Fac * (PVpf2));
		}
	}

	for (int j = 0; j < Cpf2.size(); j++)
	{
		if (j == 0) GG.push_back(0.5 * dGdC[j] * (Cpf2[j] - C0));

		else GG.push_back(GG[j - 1] + 0.5 * (dGdC[j - 1] + dGdC[j]) * (Cpf2[j] - Cpf2[j - 1]));

		Ciso0.push_back(Cpf2[j]);
		Giso0.push_back(GG[j] + Gmax1);
	}

	// Change index from low to high
	for (int j = 1; j <= Ciso0.size(); j++)
	{ 
		Ciso.push_back(Ciso0[Ciso0.size() - j]);
		Giso.push_back(Giso0[Giso0.size() - j]);
	}
}

//------------------------------------------------------------------------------------------
void Model::ConvertUnits()
{
	//  Convert METRIC Units to traditional ones
	dr = (radiusMax - radiusWell) / double(nBlocks);

	if (iModel == 3)
	{
		if (UnitSpec == 'M' || UnitSpec == 'm')
		{
			// m = 3.2808ft   
			radiusWell *= 3.2808;
			dr *= 3.2808;
			radiusMax *= 3.2808;

			for (int l = 0; l < nLay; l++)
				SLay[l].HLay *= 3.2808;

			for (int s = 0; s < nStg; s++)
			{
				stg[s].Qwt *= 6.2933; // 1m3 = 6.2933bbls
			}
		}

		for (int s = 0; s < nStg; s++)
		{
			if (stg[s].TypeStg != 3)
			{
				stg[s].Qwt *= 1440;
				stg[s].Time /= 24.0;
				stg[s].TimeStep /= 24;
			}

		}
	}
}
//------------------------------------------------------------------------------------------
void Model::DeConvertUnits()
{
	//  Revert to original units 
	if (iModel == 3)
	{
		if (UnitSpec == 'M' || UnitSpec == 'm')
		{
			// m = 3.2808ft   
			radiusWell /= 3.2808;
			dr /= 3.2808;
			radiusMax /= 3.2808;

			for (int l = 0; l < nLay; l++)
				SLay[l].HLay /= 3.2808;

			for (int s = 0; s < nStg; s++)
			{
				stg[s].Qwt /= 6.2933; // 1m3 = 6.2933bbls
			}
		}

		for (int s = 0; s < nStg; s++)
		{
			if (stg[s].TypeStg != 3)
			{
				stg[s].Qwt /= 1440;
				stg[s].Time *= 24.0;
				stg[s].TimeStep *= 24;
			}

		}
	}
}
//------------------------------------------------------------------------------------------

double Model::RunSQV_SP(Particle &P)
{
	int num = 0,
		maxNkt = 0;

	double	dz,
			Pi = atan(1.0) * 4.0,
			VwCP = 0,
			T = 0,
			WellC0 = 0,
			LH, 
			Twl,
			MassProd = 0;

	vector<int> Nkt(nLay, 0);
	vector<double> TwlH(nLay, 0);

	vector<vector<double>> C(nBlocks, vector<double>(nLay, 0)),
						   ADPN(nBlocks, vector<double>(nLay, 0)),
						   FTemp(nBlocks, vector<double>(nLay, Temp.Ftemp)),
						   HcIB(nLay, vector<double>(nBlocks, 0));

	size_t err1;
	FILE *fP1;

	// This sItem will store all the Pareto Front 
	err1 = fopen_s(&fP1, PathName+"Temp.txt", "w");
	
	if (nLay == 1)
	{
		Nkt[0] = 2;
		dz = SLay[0].HLay / Nkt[0];
		maxNkt = Nkt[0];
	}
	else
	{
		for (int l = 0; l < nLay; l++)
		{
			if (SLay[l].HLay < 20) Nkt[l] = 2;
			if (SLay[l].HLay >= 20) Nkt[l] = int(SLay[l].HLay / 10.);

			if (maxNkt < Nkt[l]) maxNkt = Nkt[l];
		}
	}

	vector<vector<vector<double>>>	Tik(nLay, vector<vector<double>>(nBlocks, vector<double>(maxNkt, Temp.Ftemp))),
									HcIB2(nLay, vector<vector<double>>(nBlocks, vector<double>(maxNkt, 0)));

	size_t err;
	
	ConvertUnits();

	if ( nLife == 0) nLife = 1;

	P.Tim_Life.resize(nLife);
	P.Vol_Life.resize(nLife);

	for (int i = 0; i < nLife; i++)
	{
		P.Tim_Life[i] = -1;
		P.Vol_Life[i] = -1;
	}

	forecast = true;

	if (nOpt == 0) // Isotherm Derivation
	{
		Iso.k = P.X[0];
		Iso.n = P.X[1];

		if (SqzOpt.nPar == 3)
			Iso.r2 = P.X[2];
		else
			Iso.r2 = 0.1;

		forecast = false;
	}

	if (nOpt == 1) // Design OPtimisation, Stages 
	{
		for (int s = 0; s < nStg; s++)
		{
			// Stage Flowrate
			if (stg[s].objFlowRate == 1)
			{
				stg[s].Qwt = SqzOpt.X[stg[s].objIndexFlowRate] * P.X[stg[s].objIndexFlowRate] * 1440;

				if (UnitSpec == 'M' || UnitSpec == 'm')
				
					stg[s].Qwt *= 6.2933; // 1m3 = 6.2933bbls
					
			}

			// Stage Time
			if (stg[s].objTime == 1)
			{
				stg[s].Time = SqzOpt.X[stg[s].objIndexTime] * P.X[stg[s].objIndexTime] / 24.0;
			}

			// For Injection Stage
			if (stg[s].TypeStg == 1)
			{
				// Stage SI Concentration
				if (stg[s].objSI == 1)
				{
					stg[s].InitialCw[0] = SqzOpt.X[stg[s].objIndexSI] * P.X[stg[s].objIndexSI];
				}
			}
		}
	}

	int	Nt1 = 1,
		Nt2;

	bool	checkFail = true, // It is true run did not converge
			check = true;

	RT.clear();
	RC.clear();
	RC_L_Out.clear();
	WC_L_Out.clear();
	WPRD.clear();
	C_L_Out.clear();	
	AD_L_Out.clear();
	StgType.clear();
	StgNum.clear();

	MassProd = 0;
	
	for (int s = 0; s < nStg && checkFail; s++)
	{
		int l1,
			l2,
			iFlow;

		double QTr = 5.615 * stg[s].Qwt;
		double dT = stg[s].TimeStep;
		double Cedge = stg[s].InitialCw[0];
		double khProd = 0,
			SumPartLay = 0;
		vector<double> PartLay;

		PartLay.resize(nLay);

		IsoTV.resize(fX.size());
		IsoC.resize(fX.size());

		vector<vector<double>> McIB(nBlocks, vector<double>(nLay, 0));

		for (int l = 0; l < nLay; l++)
		{
			PartLay[l] = 0;

			if (stg[s].nOpen > 0)
			{
				for (int il = 0; il < stg[s].nOpen; il++)
				{
					if (l == (stg[s].nLayOp[il] - 1))

						if (stg[s].nMethod == 0) PartLay[l] = SLay[il].HLay * SLay[il].KAbs;

						else PartLay[l] = stg[s].PIDF[il];
				}
			}

			if (stg[s].nOpen == 0) // All Layers open
			{
				if (stg[s].nMethod == 0) PartLay[l] = SLay[l].HLay * SLay[l].KAbs;

				else PartLay[l] = stg[s].PIDF[l];

			}

			SumPartLay += PartLay[l];
		}

		for (int l = 0; l < nLay; l++)

			PartLay[l] /= SumPartLay;

		double Time = stg[s].Time;

		Nt2 = Nt1 + int(Time / dT);

		if ((Time / dT) - int(Time / dT) >= 0.5 && s > 0) Nt2++;

		for (int t = Nt1; t <= Nt2 && checkFail; t++)
		{
			double	Cupstr,
				WellC = 0;

			vector<double>	WC_L,
				WR_L;

			WC_L.resize(nLay);
			WR_L.resize(nLay);

			if (kTemp == 1) // Temperature Calculations
			{
				if (t == Nt1 && stg[s].TypeStg == 5)
				{
					for (int l = 0; l < nLay; l++)
					{
						for (int i = 0; i < nBlocks; i++)
						{
							double	rb1 = i*dr + radiusWell,
								rb2 = (i + 1)*dr + radiusWell,

								Va = Pi* SLay[l].HLay * (rb2 *rb2 - rb1*rb1),
								Vw = Va*SLay[l].Phi,
								Vr = Va*(1. - SLay[l].Phi);

							for (int j = 0; j < Nkt[l]; j++)
							{
								HcIB2[l][i][j] = FTemp[i][l] * (Vw*Temp.RHOw*Temp.SHw + Vr*Temp.RHOr*Temp.SHr) / double(Nkt[l]);
								Tik[l][i][j] = FTemp[i][l];
							}
						}
					}
				}
			}

			T = T + dT;

			VwCP = VwCP + stg[s].Qwt * dT / 1000; // In 1000 bbls

			for (int l = 0; l < nLay && checkFail; l++)
			{
				double Qr = PartLay[l] * QTr;
				Cupstr = Cedge;

				WR_L[l] = PartLay[l];

				if (nLay > 1)

					dz = SLay[l].HLay / double(Nkt[l]);

				for (int i1 = 0; i1 < nBlocks && checkFail; i1++)
				{
					int i,
						it = 0;

					double	Cold,
							Gold,
							Pold,
							GeqOld,
							GuessC,
							GuessG,
							GuessP,
							Cnew,
							Gnew,
							Pnew;

					if (stg[s].TypeStg == 3) i = nBlocks - i1 - 1;

					else i = i1;

					double	Va = R(i, dr) * dr * SLay[l].HLay * Pi * 2.0,
							Vw = Va * SLay[l].Phi,
							Vr = Va * (1.0 - SLay[l].Phi);

					if (kTemp == 0) // No temperature calculations
					{
						Cold = C[i][l];
						Gold = ADPN[i][l];

						GeqOld = Geq(Cold);
						GuessC = Cold;
						GuessG = Gold;

						check = true;
						checkFail = true; // It is false run did not converge

						int idT = 1;
						double new_dT = dT;
						double new_TolSQ5 = TolSQ5;

						for (int is = 0; is < idT; is++)
						{
							while (it < nIters && check && checkFail)
							{
								double P = fP(GuessC, Cold, Cupstr, GuessG, Gold, Qr, Vw, Vr, new_dT);

								double Q = fQ(GuessC, Cold, GuessG, Gold, new_dT);

								double dPdC = fdPdC(Qr, Vw, new_dT);

								double dPdG = fdPdG(Vr);

								double dQdC = fdQdC(GuessC);

								double dQdG = fdQdG(new_dT);

								double Denom = dPdC * dQdG - dPdG * dQdC;
								Cnew = GuessC + (Q * dPdG - P * dQdG) / Denom;
								Gnew = GuessG + (P * dQdC - Q * dPdC) / Denom;

								// Precheck if concentration and adosprtion are low, below 1e-10

								if (abs(Cnew - GuessC) <= 1e-12 && abs(Gnew - GuessG) <= 1e-12)
								{
									check = false;
									Cupstr = (Cold + Cnew) / 2.0;
									C[i][l] = Cnew;
									ADPN[i][l] = Gnew;
								}
								else
								{
									//  At certain(low) concentrations, the process predicts a negative value
									//	of the mobile concentration.This is caused by very steep isotherms.The
									//	solution is to "correct" the new guess for the mobile concentration to
									//	make it 1 / 2 of the previous guess(i.e.allow C to reduce a little, but
									//	not too much).The Adsorped concentration is suitably adjusted.The new
									//	guess is then put through the iteration process - but is NOT counted as a
									//	new iteration.If C is halved all the way down to zero, then there is
									//	obviously some problem - almost certainly that the timestep is too large.

									if (Cnew < 0.0)
									{
										GuessC = GuessC / 2.0;

										GuessG = Gnew + (Cnew - GuessC) * Vw / Vr;

										it++;

										if (it == nIters - 1)
										{
											new_dT /= 2;
											GuessC = Cold;
											GuessG = Gold;

											it = 0;
											idT *= 2;

											new_TolSQ5 = pow(10, -3);
											if (new_dT <= dT / 20.0)
											{
												checkFail = false;
												Check_NR(Cold, Gold, Cupstr, dT, Qr, Vw, Vr);
											}
										}
									}
									else
									{
										//  Tolerance test for convergence of iteration.The "unothodox" form of
										//  condition was used to make sure that the requirement was strict enough
										//  when concentrations are high for good accuracy, but lax enough at low
										//  concentrations to prevent unnecessary CPU - time being used when very little
										//  mass is affected - a compromise.

										if ((abs(Cnew - GuessC) <= ((Cnew + 1.) * new_TolSQ5)) && (abs(Gnew - GuessG) <= ((Gnew + 1.) * new_TolSQ5)))
										{
											check = false;
											Cupstr = (Cold + Cnew) / 2.0;
											C[i][l] = Cnew;
											ADPN[i][l] = Gnew;
										}
										else
										{
											GuessC = Cnew;
											GuessG = Gnew;
											it++;
										}
									}

									// When Cnew > 0 but it does not converge 
									if (it == nIters - 1)
									{
										new_TolSQ5 = pow(10, -3);

										new_dT /= 2;
										GuessC = Cold;
										GuessG = Gold;

										it = 0;
										idT *= 2;

										if (new_dT <= dT / 20.0)
										{
											checkFail = false;
											Check_NR(Cold, Gold, Cupstr, dT, Qr, Vw, Vr);
										}
									}
								}
							}
						}

						WellC += C[0][l] * Qr;

						WC_L[l] = C[0][l];

						if (stg[s].TypeStg == 3)

							MassProd += (Qr * dT * C[0][l]) / 5.615;

					} // Ktemp == 0

					if (Iso.IsoTyp == 4) // Precipitation
					{
						double	CIcp,
							Dconv = 28.3168; // Convert ft**3 to litre

						if (s == 1)
							cout<<"";

						if (kTemp != 1)
						{
							CIcp = Iso.Cps1;
						}
						else
						{
							if (FTemp[i][l] >= Iso.Tpc) CIcp = Iso.Cps1;

							if (FTemp[i][l] < Iso.Tpc) CIcp = Iso.Cps2;
						}

						if (stg[s].TypeStg != 3)
						{
							if (i == 0) McIB[i][l] = McIB[i][l] + Qr * (Cedge - C[i][l]) * dT * Dconv;

							if (i > 0) McIB[i][l] = McIB[i][l] + Qr * (C[i-1][l] - C[i][l]) * dT * Dconv;
						}
						else
						{
							if (i == nBlocks - 1) McIB[i][l] = McIB[i][l] - Qr * C[i][l] * dT * Dconv;

							if (i < nBlocks-1) McIB[i][l] = McIB[i][l] + Qr * (C[i+1][l] - C[i][l]) * dT * Dconv;
						}

						Cold = C[i][l];
						Pold = ADPN[i][l];

						if (McIB[i][l] < 1)
						{
							Cnew = 0;
							Pnew = 0;
						}
						else
						{
							double R44;

							GuessC = Cold;

							if (GuessC < 0) GuessC = 0.;

							GuessP = Pold;

							R44 = Iso.r4;

							if (GuessC == CIcp || (GuessC < CIcp && ADPN[i][l] <= 0.)) R44 = 0.;

							// Inhibitor mass conservation coefficients

							double	a11 = Vw * Dconv,
								a12 = Vr * Dconv,
								a13 = -McIB[i][l];

							//  Inhibitor precipitation / dissolution coefficients

							double	a21 = R44,
								a22 = -1.0 / dT,
								a23 = ADPN[i][l] / dT - R44 * CIcp;

							check = true;

							while (it < nIters && check)
							{
								double	P0 = a11 * GuessC + a12 * GuessP + a13,
									Q0 = a21 * GuessC + a22 * GuessP + a23,

									dP0dC = a11,
									dP0dP = a12,
									dQ0dC = a21,
									dQ0dP = a22,

									Denom = dP0dC * dQ0dP - dP0dP * dQ0dC;

								Cnew = GuessC + (Q0 * dP0dP - P0 * dQ0dP) / Denom;
								Pnew = GuessP + (P0 * dQ0dC - Q0 * dP0dC) / Denom;

								//  Some adjustments are neccessary if the updated mobile concentration or
								//  precipitation turns out to be negative.

								if (Cnew < 0)
								{
									GuessC = GuessC / 2.0;
									GuessP = Pnew + (Cnew - GuessC) * Vw / Vr;
								}


								if (Pnew < 0)
								{
									Cnew = Cnew + Pnew * Vr / Vw;
									Pnew = 0.;
								}

								//  Tolerance test for convergence of iteration.The "unothodox" form of
								//  condition was used to make sure that the requirement was strict enough
								//  when concentrations are high for good accuracy, but lax enough at low
								//  concentrations to prevent unnecessary CPU - time being used when very little
								//  mass is affected - a compromise.

								if ((fabs(Cnew - GuessC) < (Cnew + 1.) * TolSQ5) && (fabs(Pnew - GuessP) < (Pnew + 1.) * TolSQ5))
								{
									check = false;
								}
								it++;
								GuessC = Cnew;
								GuessP = Pnew;
							}

							Cupstr = (Cold + Cnew) / 2.;

							C[i][l] = Cnew;
							ADPN[i][l] = Pnew;
						}
					
					}	// IsoType == 4 Precipitation

					if (kTemp == 1) // Temperature Calculations
					{
						double	rb1 = i*dr + radiusWell,
								rb2 = (i + 1)*dr + radiusWell,

								dHc,
								dHf,
								dHCz,
								dHCr,

								Va = Pi* SLay[l].HLay * (rb2 *rb2 - rb1*rb1),
								Vw = Va*SLay[l].Phi,
								Vr = Va*(1. - SLay[l].Phi);

						if (stg[s].TypeStg <= 2)
						{
							Twl = stg[s].TI;
							TwlH[l] = Twl;
						}

						if (stg[s].TypeStg != 5)
						{

							if (t == 1) HcIB[l][i] = Temp.Ftemp*(Vw*Temp.RHOw*Temp.SHw + Vr*Temp.RHOr*Temp.SHr);

							if (i == 0)
							{
								dHCr = (2.0*Pi*SLay[l].HLay*dT*Temp.Kcr / dr)*(rb2*(FTemp[i + 1][l] - FTemp[i][l]) - rb1*(FTemp[i][l] - TwlH[l]));

								if (stg[s].TypeStg == 5 || Qr <= 0.0001)

									dHCr = (2.0*Pi*SLay[l].HLay*dT*Temp.Kcr / dr)*rb2*(FTemp[i + 1][l] - FTemp[i][l]);
							}
							else
							{
								if (i == nBlocks - 1)
								{
									dHCr = (2.0*Pi*SLay[l].HLay*dT*Temp.Kcr / dr)*(rb2*(Temp.Ftemp - FTemp[i][l]) - rb1*(FTemp[i][l] - FTemp[i - 1][l]));
								}
								else
								{
									dHCr = (2.0*Pi*SLay[l].HLay*dT*Temp.Kcr / dr)*(rb2*(FTemp[i + 1][l] - FTemp[i][l]) - rb1*(FTemp[i][l] - FTemp[i - 1][l]));

								}

							}

							if (nLay == 1)
							{
								dHCz = (4.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / SLay[l].HLay)*(Temp.Ftemp - FTemp[i][l]);

								dHc = dHCr + dHCz;
							}
							else
							{
								//  During injection and production periods, the conduction between
								//  neighbouring layers can be ignored since it would be too small comapred
								//	to the covection heat transfer.

								if (l == 0 || l == nLay - 1)
								{
									dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / SLay[l].HLay)*(Temp.Ftemp - FTemp[i][l]);
									dHc = dHCr + dHCz;
								}
								else
									dHc = dHCr;
							}


							if (stg[s].TypeStg != 3)
							{
								if (i == 0)
									dHf = Qr*dT*Temp.RHOw*Temp.SHw*(TwlH[l] - FTemp[i][l]);
								else
									dHf = Qr*dT*Temp.RHOw*Temp.SHw*(FTemp[i - 1][l] - FTemp[i][l]);
							}
							else
							{
								if (i == nBlocks - 1)
									dHf = Qr*dT*Temp.RHOw*Temp.SHw*(Temp.Ftemp - FTemp[i][l]);
								else
									dHf = Qr*dT*Temp.RHOw*Temp.SHw*(FTemp[i + 1][l] - FTemp[i][l]);
							}

							HcIB[l][i] = HcIB[l][i] + dHc + dHf;
						}
						else // Heat conduction at shut-in stage
						{
							// During the well shut - in, conduction is the major heat transfer mechanism
							// For a multi - layer system, heat transfer calculation depends what
							// configuration the system has.If no thick barriers between any two layers
							// then the boundary condition of middle layers(1<i<nLayer) can be set
							// to be identical to the nabouring two layers.

							HcIB[l][i] = 0;

							if (l == 1)
							{
								cout << "";
							}

							for (int kl = 0; kl < Nkt[l]; kl++)
							{
								if (i == 0)
								{
									double	t1 = Tik[l][i + 1][kl],
										t2 = Tik[l][i][kl];

									dHCr = (2.0*Pi*dz*dT*Temp.Kcr / dr)*rb2 * (Tik[l][i + 1][kl] - Tik[l][i][kl]);
								}
								else
								{
									if (i == nBlocks - 1)
									{
										dHCr = (2.0*Pi*dz*dT*Temp.Kcr / dr)*(rb2*(Temp.Ftemp - Tik[l][i][kl]) - rb1*(Tik[l][i][kl] - Tik[l][i - 1][kl]));
									}
									else
									{
										dHCr = (2.0*Pi*dz*dT*Temp.Kcr / dr)*(rb2 * (Tik[l][i + 1][kl] - Tik[l][i][kl]) - rb1*(Tik[l][i][kl] - Tik[l][i - 1][kl]));
									}
								}

								if (nLay == 1)
								{
									if (kl == 0)
									{
										dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Tik[l][i][kl + 1] - Tik[l][i][kl]) - (Tik[l][i][kl] - Temp.Ftemp));
									}
									else
									{
										if (kl == Nkt[0] - 1)

											dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Temp.Ftemp - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl - 1]));

										else

											dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Tik[l][i][kl + 1] - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl - 1]));

									}
								}
								else
								{
									if (kl == 0)
									{
										if (l == 0)
											dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Temp.Ftemp - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl + 1]));
										else
										{
											double t1, t2, t3;
											t1 = Tik[l - 1][i][Nkt[l - 1] - 1];
											t2 = Tik[l][i][kl];
											t3 = Tik[l][i][kl + 1];

											dHCz = (2.0 * Pi * R(i, dr) * dr * dT * Temp.Kcs / dz) * ((t1 - t2) - (t2 - t3));
										}
									}
									else
									{
										if (kl == Nkt[l] - 1)
										{
											if (l == nLay - 1)
												dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Temp.Ftemp - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl - 1]));
											else
											{
												double t1, t2, t3;
												t1 = Tik[l + 1][i][kl];
												t2 = Tik[l][i][kl];
												t3 = Tik[l][i][kl - 1];
												dHCz = (2.0 * Pi * R(i, dr) * dr * dT * Temp.Kcs / dz) * ((Tik[l + 1][i][kl] - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl - 1]));
											}
										}
										else

											dHCz = (2.0*Pi*R(i, dr)*dr*dT*Temp.Kcs / dz) * ((Tik[l][i][kl + 1] - Tik[l][i][kl]) - (Tik[l][i][kl] - Tik[l][i][kl - 1]));

									}
								}

								dHc = dHCr + dHCz;

								double t;
								HcIB2[l][i][kl] = HcIB2[l][i][kl] + dHc;
								HcIB[l][i] = HcIB[l][i] + HcIB2[l][i][kl];

								t = HcIB[l][i];

							}
						}

					} // kTemp == 1

				} // For nBlocks

				WellC += C[0][l] * Qr;

				WC_L[l] = C[0][l];

				if (stg[s].TypeStg == 3)

					MassProd += (Qr * dT * C[0][l]) / 5.615;

			} // For nLay
			
			for (int l = 0; l < nLay; l++)
			{
				for (int i = 0; i < nBlocks && checkFail; i++)
				{
					double	rb1 = i*dr + radiusWell,
						rb2 = (i + 1)*dr + radiusWell,
						Va = Pi* SLay[l].HLay * (rb2 *rb2 - rb1*rb1),
						Vw = Va*SLay[l].Phi,
						Vr = Va*(1. - SLay[l].Phi);

					FTemp[i][l] = HcIB[l][i] / (Vw*Temp.RHOw*Temp.SHw + Vr*Temp.RHOr*Temp.SHr);

					if (stg[s].TypeStg == 5) // Shut-in
					{
						for (int kl = 0; kl < Nkt[l]; kl++)

							Tik[l][i][kl] = HcIB2[l][i][kl] * double(Nkt[l]) / (Vw*Temp.RHOw*Temp.SHw + Vr*Temp.RHOr*Temp.SHr);
					}
				}
			}

			if (stg[s].TypeStg == 3)
			{
				if ((kTemp == 1) || (kTemp == 2))
				{
					double	AA5 = 0,
							AA4 = 0;

					for (int l = nLay - 1; l > -1; l--)
					{
						AA5 = AA5 + PartLay[l] * FTemp[0][l];
						AA4 = AA4 + PartLay[l];

						if (AA4 > 0)
							TwlH[l] = AA5 / AA4;
						else
							TwlH[l] = 0;
					}

					Twl = TwlH[0];
				}
			}

			if (stg[s].TypeStg == 5)
			{
				if ((kTemp == 1) || (kTemp == 2))
				{
					double	AA5 = 0,
							AA4 = 0;

					for (int l = 0; l < nLay; l++)
					{
						double st = SLay[l].HLay * FTemp[0][l];
						AA4 = AA4 + SLay[l].HLay;
						AA5 = AA5 + SLay[l].HLay * FTemp[0][l];
					}
					Twl = AA5 / AA4;
				}
			}


			if (t % nFreq == 0)
			{	
				RC_L_Out.push_back(WC_L);	// Layer Return Concentration Profile
				
				double	t0 = 0,
						t1 = 0,
						tQ = 0;

				vector<double>	temp,
								temp1;
				
				temp.resize(nLay);
				temp1.resize(nLay);

				for (int l = nLay-1; l >= 0; l--)
				{	
					tQ += PartLay[l] * QTr;
					t0 += C[0][l] * PartLay[l] * QTr;
					temp[l] = t0 / tQ;
					t1 += PartLay[l];
					temp1[l] = t1;
				}

				WC_L_Out.push_back(temp);
				WPRD.push_back(temp1);
				
				C_L_Out.push_back(C);					// Concentration Profiles vs Radius and Layer	
				AD_L_Out.push_back(ADPN);					// Adsoprtion Profiles vs Radius and Layer

				StgType.push_back(stg[s].TypeStg);
				StgNum.push_back(s+1);
			}

			if (QTr > 0)
				WellC /= QTr;
			else
				WellC = 0;

			if (stg[s].TypeStg == 3)
			{	
				if ( nSim == 1 ) Clife[0] = SqzOpt.MIC;

				for (int c = 0; c < nLife; c++)
				{
					if (Clife[c] >= WellC && Clife[c] < WellC0)
					{	
						if (UnitSpec == 'm' || UnitSpec == 'M')

							P.Vol_Life[c] = 1000. * VwCP * 0.1589 / 100. - stg[s].Qwt * dT * 0.1589 * (WellC - Clife[c]) / (WellC - WellC0) / 100.0;

						if (UnitSpec == 'c' || UnitSpec == 'C')

							P.Vol_Life[c] = VwCP - stg[s].Qwt * dT * (WellC - Clife[c]) / (WellC - WellC0) / 1000.0;

						P.Tim_Life[c] = T - (Clife[c] - WellC) / (WellC0 - WellC) * dT;

						//	Volume of Water
						if (kPlot == 1 || kPlot == 3)

							TVlife[c] = P.Vol_Life[c];
						
						//	Days
						if (kPlot == 0 || kPlot == 2)

							TVlife[c] = P.Tim_Life[c];
					}

				}
			}
			
			if (t % nFreq == 0)
			{
				// Time
				if (kPlot == 0 || kPlot == 2)
				{
					RT.push_back(T);
				}

				//	Volume of Water
				if (kPlot == 1 || kPlot == 3)
				{
					double temp = VwCP;

					if (UnitSpec == 'm' || UnitSpec == 'M')

						temp *= 1.589; // To transform to 100m3 from bbls

					RT.push_back(temp);
				}

				RC.push_back(WellC);

				if ( nLay == 1)
					RTemp.push_back(FTemp[0][0]);
				else
					RTemp.push_back(Twl);

				CumProd.push_back(MassProd);

				fprintf_s(fP1, "Time/Vol %11.4f Conc Adso/ppt Temp\n", RT[RT.size()-1]);

				for (int l = 0; l < nLay; l++)
				{
					fprintf_s(fP1, "Layer %d\n", l + 1);

					if (kTemp)
					{
						for (int j = 0; j < nBlocks; j++)

							fprintf_s(fP1, "%11.4f ", C[j][l]);
					}
				}

				fprintf_s(fP1, "\n");
			}
			
			// Iostherm Matching data
			if (num < fX.size())
			{
				// Time
				if (kPlot == 0 || kPlot == 2)
				{
					if (T > fX[num])
					{
						IsoTV[num] = T;
						IsoC[num] = WellC;
						num++;
					}
				}

				//	Volume of Water
				if (kPlot == 1 || kPlot == 3)
				{
					double VwCPTemp = 0;

					if (UnitSpec == 'm' || UnitSpec == 'M')

						VwCPTemp = VwCP * 1.589;	// To transform to 100m3 from bbls

					if (UnitSpec == 'c' || UnitSpec == 'C')

						VwCPTemp = VwCP;				// VwCP is in 1000bbls by default

					if (VwCPTemp > fX[num])
					{
						IsoTV[num] = VwCPTemp;
						IsoC[num] = WellC;
						num++;
					}

				}
			}

			WellC0 = WellC;
		}

		Nt1 = Nt2 + 1;
	}

	
	if (checkFail) // If it is false it fails
	{
		if (!forecast)
			LH = Likelihood();
		else
			LH = TVlife[0];
	}
	else
	{		
		if (Max) LH = 0.0; // Maximise
		else LH = 1e20; // Minimise

		for (int i = 0; i < fX.size(); i++)
		{
			IsoC[i] = 0;
		}

		if (nOpt == 1) LH = 0;
	}

	fclose(fP1);

	DeConvertUnits();

	return LH;
}

//------------------------------------------------------------------------------------------

void Model::Check_NR(double Cold, double Gold, double Cupstr, double dt, double Qr, double Vw, double Vr)
{
	int it = 0,
		nIters = 30;

	bool	check = true,
			checkFail = true; // It is false run did not converge

	double	new_dT = dt,
			Cnew,
			Gnew,
			TolSQ5 = 1.0000000000000000e-08,
			GuessC,
			GuessG;
			
			GuessC = Cold;
			GuessG = Gold;

	while (it < nIters && check && checkFail)
	{
		double P = fP(GuessC, Cold, Cupstr, GuessG, Gold, Qr, Vw, Vr, new_dT);

		double Q = fQ(GuessC, Cold, GuessG, Gold, new_dT);

		double dPdC = fdPdC(Qr, Vw, new_dT);

		double dPdG = fdPdG(Vr);

		double dQdC = fdQdC(GuessC);

		double dQdG = fdQdG(new_dT);

		double Denom = dPdC * dQdG - dPdG * dQdC;
		Cnew = GuessC + (Q * dPdG - P * dQdG) / Denom;
		Gnew = GuessG + (P * dQdC - Q * dPdC) / Denom;

		// Precheck if concentration and adosprtion are low, below 1e-10

		if (abs(Cnew - GuessC) <= 1e-12 && abs(Gnew - GuessG) <= 1e-12)
		{
			check = false;
			Cupstr = (Cold + Cnew) / 2.0;
		}
		else
		{
			//  At certain(low) concentrations, the process predicts a negative value
			//	of the mobile concentration.This is caused by very steep isotherms.The
			//	solution is to "correct" the new guess for the mobile concentration to
			//	make it 1 / 2 of the previous guess(i.e.allow C to reduce a little, but
			//	not too much).The Adsorped concentration is suitably adjusted.The new
			//	guess is then put through the iteration process - but is NOT counted as a
			//	new iteration.If C is halved all the way down to zero, then there is
			//	obviously some problem - almost certainly that the timestep is too large.

			if (Cnew < 0.0)
			{
				GuessC = GuessC / 2.0;

				GuessG = Gnew + (Cnew - GuessC) * Vw / Vr;

				it++;

				if (it == nIters - 1)
				{
					it = 0;
					new_dT = dt / 2;
					GuessC = Cold;
					GuessG = Gold;

					if (new_dT <= dt / 20)
					{
						checkFail = false;
					}
				}
				
			}
			else
			{
				//  Tolerance test for convergence of iteration.The "unothodox" form of
				//  condition was used to make sure that the requirement was strict enough
				//  when concentrations are high for good accuracy, but lax enough at low
				//  concentrations to prevent unnecessary CPU - time being used when very little
				//  mass is affected - a compromise.

				if ((abs(Cnew - GuessC) <= ((Cnew + 1.) * TolSQ5)) && (abs(Gnew - GuessG) <= ((Gnew + 1.) * TolSQ5)))
				{
					check = false;
					Cupstr = (Cold + Cnew) / 2.0;
				}
				else
				{
					GuessC = Cnew;
					GuessG = Gnew;
					it++;
				}
			}
		}
	}

}

void Model::LinearCore_SP()
{
	int nX = nBlocks;

	double	Zeta,
			Zeta1,
			aCore;

	vector<double>	C,
					AD,
					COld,
					ADOld,
					Ceff,
					PVs;
	
	C.resize(nX);
	AD.resize(nX);
	COld.resize(nX);
	ADOld.resize(nX);

	//  Now perform the main part of the time integration in the linear 1D model

	double	T = 0.0,
			wPVs = 0.0,
			Ceff0 = 0.0,
			Ceff1 = 0.0,
			CUMINJ = 0.0,
			CUMPRD = 0.0,
			MassProd = 0.0,
			QRATE,
			DT,
			NTSTEP,
			C0,
			VEL,
			DX,
			DIS = 0,
			CON30,
			CON31,
			a11,
			b11,
			c11,
			PV,
			GuessC,
			GuessG,
			Cold,
			Gold,
			Tlocal,
			GeqOld,
			Cnew,
			Gnew;
	
	int NT1 = 1,
		NT2,
		KKK = 1,
		NWARN = 1,
		Nrep = 1,
		KRFLO = 0,
		IPSK;

	bool checkFail = false;

	Zeta = (1. - phi) / phi;
	Zeta1 = 1.0 / Zeta;
	aCore = 0.25*3.1415926*Dcor*Dcor;
	kChem = 1;
	kTemp = 0;

	PV = xL*aCore*phi;
	DX = xL / (nX-1);

	for (int i = 0; i < nX; i++)
	{
		C[i] = 0;
		AD[i] = 0;
	}

	for (int s = 0; s < nStg; s++)
	{
		QRATE = stg[s].Qwt/3600.0; // Time units into secs
		DT = stg[s].TimeStep * 3600.0; // Time units into secs
		NTSTEP = stg[s].Time * 3600.0 / DT;
		NT2 = int(NT1 + NTSTEP - 1 + 0.0001);
		IPSK = stg[s].TypeStg;

		if (IPSK == 1) C0 = stg[s].InitialCw[0];

		else C0 = 0;

		//  Calculate the superficial velocity
		VEL = QRATE / (aCore*phi);

		//  Calculate the cumulative amount of inhibitor injected
		CUMINJ = CUMINJ + stg[s].Time*QRATE*C0;

		CON30 = (2.0*DIS*DT) / (DX*DX);
		CON31 = VEL*DT / DX;

		a11 = Zeta1*(1.0 + CON31);
		b11 = 1.0;

		for (int t = NT1; t < NT2; t++)
		{
			T = T + DT;
			wPVs = wPVs + QRATE*DT / PV;

			for (int i = 0; i < nX; i++)
			{	
				int it = 0;

				GuessC = C[i];
				GuessG = AD[i];
				COld[i] = C[i];
				ADOld[i] = AD[i];
				Cold = C[i];
				Gold = AD[i];
				Tlocal = 0;
				GeqOld = Geq(Cold);

				if (i == 0) c11 = -ADOld[i] - Zeta1*(COld[i] + CON31*C0);
				
				else
				{
					if (i == nX - 1) c11 = -ADOld[i] - Zeta1 * (COld[i] + CON31 * C[i - 1]);

					else c11 = -ADOld[i] - Zeta1 * (COld[i] + CON31 * C[i - 1] + CON30 / 2.0 * (COld[i + 1] + COld[i - 1] - 2. * COld[i]));
				}

				bool check = true;

				while (it < nIters && check && !checkFail)
				{
					double P = a11*GuessC + b11*GuessG + c11;

					double Q = fQ(GuessC, Cold, GuessG, Gold, DT);

					double dPdC = a11;

					double dPdG = b11;

					double dQdC = fdQdC(GuessC);

					double dQdG = fdQdG(DT);

					double Denom = dPdC * dQdG - dPdG * dQdC;
					
					Cnew = GuessC + (Q * dPdG - P * dQdG) / Denom;
					Gnew = GuessG + (P * dQdC - Q * dPdC) / Denom;

					//  At certain(low) concentrations, the process predicts a negative value
					//	of the mobile concentration.This is caused by very steep isotherms.The
					//	solution is to "correct" the new guess for the mobile concentration to
					//	make it 1 / 2 of the previous guess(i.e.allow C to reduce a little, but
					//	not too much).The Adsorped concentration is suitably adjusted.The new
					//	guess is then put through the iteration process - but is NOT counted as a
					//	new iteration.If C is halved all the way down to zero, then there is
					//	obviously some problem - almost certainly that the timestep is too large.

					if (Cnew < 0.0)
					{
						GuessC = GuessC / 2.0;

						GuessG = Gnew + (Cnew - GuessC)*Zeta1;

						if (GuessC <= 1e-10)
						{
							checkFail = true;
						}
						it--;
					}
					else
					{
						//  Tolerance test for convergence of iteration.The "unothodox" form of
						//  condition was used to make sure that the requirement was strict enough
						//  when concentrations are high for good accuracy, but lax enough at low
						//  concentrations to prevent unnecessary CPU - time being used when very little
						//  mass is affected - a compromise.

						if ((abs(Cnew - GuessC) <= (Cnew + 1.) / 1.0e8) && (abs(Gnew - GuessG) <= (Gnew + 1.) / 1.0e8))
						{
							C[i] = Cnew;
							AD[i] = Gnew;
							check = false;
						}
						else
						{
							GuessC = Cnew;
							GuessG = Gnew;
							it++;
						}
					}
				}

			}
				
			if (t%nFreq == 0 )
			{
				Ceff1 = C[nX - 1];
				Ceff.push_back(Ceff1);
				PVs.push_back(wPVs);
				Time.push_back(T);
				MassProd += stg[s].Time*QRATE*Ceff1;
				CumProd.push_back(MassProd);
			}

			if (nLife > 0 && (IPSK == 2 || IPSK == 3))
			{
				for (int l = 0; l < nLife; l++)
				{
					if (Ceff0 >= Ceff1 && Ceff1 == Clife[l])

						TVlife[l] = wPVs;

					if (Ceff0 > Clife[l] && Ceff1 < Clife[l])

						TVlife[l] = wPVs - QRATE * DT / PV * (Ceff1 - Clife[l]) / (Ceff1 - Ceff0);
				}

				Ceff0 = Ceff1;
			}
			
			CUMPRD = CUMPRD + DT * QRATE * C[nX-1];
		}

		NT1 = NT2 + 1;
	}

	RT = PVs;
	RC = Ceff;
}

//------------------------------------------------------------------------------------------

// Convection equation is P(C, G) = 0. The following define the equation and its first partial derivatives.
double Model::fP(double C, double Cold, double Cupstr, double G, double Gold, double Qr, double Vw, double Vr, double dT)
{
	double fP = (C - Cold) * Vw + Qr * dT * ((C + Cold) / 2. - Cupstr) + Vr * (G - Gold);
	return fP;
}

double Model::fdPdG(double Vr)
{
	double fdPdG = Vr;
	return fdPdG;
}

double Model::fdPdC(double Qr, double Vw, double dT)
{
	double fdPdC = Vw + Qr * dT / 2.0;
	return fdPdC;
}

//  Adsorption equation is Q(C, G) = 0. The following define the equation and its first partial derivatives
double Model::Geq(double Cmo)
{
	double Geq;

	if (Iso.IsoTyp == 1) // Freundlich Isotherm 
	{
		if (Cmo >= 1.0) Geq = Iso.k * pow(Cmo, Iso.n);

		else Geq = Iso.k * Cmo;
	}

	if (Iso.IsoTyp == 2) // Langmuir Isotherm 
	{
		Geq = Iso.k * Iso.n * Cmo / (1.0 + Iso.n * Cmo);
	}

	if (Iso.IsoTyp == 3) // Table - of - numbers Isotherm ***
	{
		if (Cmo > Iso.Ciso[Iso.nIso - 1])

			Geq = Iso.ADiso[Iso.nIso - 1];

		else
		{
			bool check = true;

			for (int i = 0; i < (Iso.nIso - 1) && check; i++)
			{
				if (Cmo >= Iso.Ciso[i] && Cmo <= Iso.Ciso[i + 1])
				{
					Geq = ((Iso.ADiso[i + 1] - Iso.ADiso[i]) / (Iso.Ciso[i + 1] - Iso.Ciso[i])) * (Cmo - Iso.Ciso[i]) + Iso.ADiso[i];
					check = false;
				}
			}
		}
	}

	return Geq;
}

//  Derivative of Adsorption equation is Q(C, G) = 0. The following define the equation and its first partial derivatives
double Model::dGeqdC(double Cmo)
{
	double dGeqdC;

	if (Iso.IsoTyp == 1) // Freundlich Isotherm 
	{
		if (Cmo >= 1.0) dGeqdC = Iso.k * Iso.n * pow(Cmo, Iso.n - 1.0);

		else dGeqdC = Iso.k;
	}

	if (Iso.IsoTyp == 2) // Langmuir Isotherm 
	{
		dGeqdC = Iso.k * Iso.n / pow(1.0 + Iso.n * Cmo, 2.0);
	}

	if (Iso.IsoTyp == 3) // Table - of - numbers Isotherm ***
	{
		dGeqdC = 0.0;

		if (Cmo > Iso.Ciso[Iso.nIso - 1])

			dGeqdC = 0.0;

		else

			for (int i = 0; i < (Iso.nIso - 1); i++)
			{
				if (Cmo >= Iso.Ciso[i] && Cmo <= Iso.Ciso[i + 1])

					dGeqdC = ((Iso.ADiso[i + 1] - Iso.ADiso[i]) / (Iso.Ciso[i + 1] - Iso.Ciso[i]));
			}
	}

	return dGeqdC;
}

double Model::fQ(double C, double Cold, double G, double Gold, double dT)
{
	double fQ;

	if (Iso.TypeAds == 0) // Equilibrium
	{
		fQ = G - Geq(C);
	}

	if (Iso.TypeAds == 1) // Kinetic
	{
		fQ = (G - Gold) / dT - Iso.r2 * (Geq(C) + Geq(Cold) - G - Gold) / 2.0;
	}

	return fQ;
}

double Model::fdQdG(double dT)
{
	double fdQdG;

	if (Iso.TypeAds == 0) // Equilibrium
	{
		fdQdG = 1;
	}

	if (Iso.TypeAds == 1) // Kinetic
	{
		fdQdG = 1.0 / dT + Iso.r2 / 2.0;
	}

	return fdQdG;
}

double Model::fdQdC(double C)
{
	double fdQdC = 0;

	if (Iso.TypeAds == 0) // Equilibrium
	{
		fdQdC = -dGeqdC(C);
	}

	if (Iso.TypeAds == 1) // Kinetic
	{
		fdQdC = -Iso.r2 * dGeqdC(C) / 2.0;
	}

	return fdQdC;
}

double Model::R(int Iblock, double dR) // Returns radius of centre of block No.IBLOCK
{
	double R = radiusWell + (double(Iblock + 1) - 0.5) * dR;
	return R;
}

void Model::PrintReturn(vector<double> RT, vector<double> RC, CString file, double CLife, Particle P)
{
	size_t err;
	FILE *fP;

	file.Replace(".dat", "Conc.txt");
	file.Replace(".DAT", "Conc.txt");

	// This file will store all the Pareto Front 
	if ((err = fopen_s(&fP, file, "w")) != 0)
		printf("Error opening file %s for writing\r\n", "conc.txt");
	else
	{	
		for (int s = 0; s < nStg; s++)
		{	
			CString st="Stage_Inj";
			
			if (stg[s].TypeStg == 1)

				st = "Stage_Inj";

			if (stg[s].TypeStg == 2)

				st = "Stage_OF";

			// Stage Flowrate
			if (stg[s].objFlowRate == 1)
			{
				fprintf_s(fP, "%11s %d Flowrate %11.4f \n", st, s+1, SqzOpt.X[stg[s].objIndexFlowRate] * P.X[stg[s].objIndexFlowRate]);
			}

			// Stage Time
			if (stg[s].objTime == 1)
			{	

				fprintf_s(fP, "%11s %d Time %11.4f \n", st, s + 1, SqzOpt.X[stg[s].objIndexTime] * P.X[stg[s].objIndexTime] );
			}

			// For Injection Stage
			if (stg[s].TypeStg == 1)
			{
				// Stage SI Concentration
				if (stg[s].objSI == 1)
				{
					fprintf_s(fP, "%11s %d SI %11.4f \n", st, s + 1, SqzOpt.X[stg[s].objIndexSI] * P.X[stg[s].objIndexSI]);
				}
			}
		}

		fprintf_s(fP, "%11s %11s \n", "Time\Vol", "RetConc");

		for (int i = 0; i < RT.size(); i++)
		if (i%nFreq == 0)
			fprintf_s(fP, "%11.4f %11.4f\n", RT[i], RC[i]);
	}

	fprintf_s(fP, "Clife %11.4f Tim_Life %11.4f Vol_Life %11.4f\n", CLife, P.Tim_Life[0], P.Vol_Life[0]);

	fclose(fP);
}

vector<vector<double>> Model::cholesky(vector<vector<double>> A, int n)
{
	vector<vector<double>> lower(n, vector<double>(n, 0));

	// Decomposing a matrix into Lower Triangular 
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <= i; j++) {
			int sum = 0;

			if (j == i) // summation for diagnols 
			{
				for (int k = 0; k < j; k++)
					sum += pow(lower[j][k], 2);
				lower[j][j] = sqrt(A[j][j] -
					sum);
			}
			else {

				// Evaluating L(i, j) using L(j, j) 
				for (int k = 0; k < j; k++)
					sum += (lower[i][k] * lower[j][k]);
				lower[i][j] = (A[i][j] - sum) /
					lower[j][j];
			}
		}
	}

	return lower;
}

vector<vector<double>> Model::mult(vector<vector<double>> A, vector<vector<double>> B, int N, int M, int P)
{
	vector<vector<double>> C(N, vector<double>(P, 0));

	for (int n = 0; n < N; n++) {
		for (int p = 0; p < P; p++) {
			double num = 0;
			for (int m = 0; m < M; m++)
			{
				double p1, p2;
				p1 = A[n][m]; p2 = B[m][p];
				num += A[n][m] * B[m][p];
			}
			C[n][p] = num;
		}
	}

	return C;
}

void Model::PrepLikelihood()
{
	int N = fX.size();

	du.resize(N);

	vector<vector<double>>	S(N, vector<double>(N, 0)),
		R(N, vector<double>(N, 0));

	// Cd covariance matrix
	// S diagonal matrix, where std_i = 0.*log(c[i])
	// R shows the degree of correlation of different data points

	double r = (fX[fX.size() - 1] - fX[0]) / 5.0;

	for (int n = 0; n < N; n++)
	{
		for (int m = 0; m < N; m++)
		{
			S[n][m] = 0;
			R[n][m] = 0;

			if (n == m)
			{
				if (fY[n] != 1)
					S[n][m] = 0.5 * log10(fY[n]);
				else
					S[n][m] = 0.5;
				//	S[n][m] = (0.5 - 2.0)*n / N + 2.0;
				R[n][m] = 1.0;
			}
			/*	else
				{
					double temp = pow(fX[n] - fX[m], 2.0) / (r*r);
					temp *= -3.0;
					temp = exp(temp);
					R[n][m] = temp;
				}*/
		}
	}

	Cd = mult(S, R, N, N, N);

	Cd = mult(Cd, S, N, N, N);

	print(Cd);

	iCd = inverse(Cd);

	//	print(iCd);

		//print(mult(iCd, Cd, N, N, N));

}

void Model::Prep_du() // Generate Random error observations
{
	int N = du.size();

	normal_distribution<double> normalDistr(0, 1.0);
	default_random_engine generator((unsigned)time(NULL));

	vector<vector<double>>	t(N, vector<double>(1, 0)),
		kt;

	for (int i = 0; i < N; i++)
		t[i][0] = normalDistr(generator);

	kt = mult(cholesky(Cd, N), t, N, N, 1);

	for (int i = 0; i < N; i++)
		du[i] = fY[i] + kt[i][0];
}

double Model::Likelihood()
{
	int N = IsoTV.size();

	vector<vector<double>>	L1(N, vector<double>(1, 0)),
		L2;

	for (int i = 0; i < N; i++)
	{
		if (du[i] > 0)
			L1[i][0] = log(IsoC[i]) - log(du[i]);
		else
			L1[i][0] = log(IsoC[i]);
	}

	L2 = mult(iCd, L1, N, N, 1);

	/*	for (int j = 0; j< N; j++)
			printf("%10.4f\t", L1[j][0]);
		printf("\n");

		for (int j = 0; j< N; j++)
			printf("%10.4f\t", L2[j][0]);
		printf("\n");*/

	double Sum = 0;

	for (int i = 0; i < N; i++)
	{
		Sum = Sum + L2[i][0] * L1[i][0];
	}

	if (Max) Sum = exp(-Sum);

	return Sum;
}


// Function to calculate and store inverse, returns false if 
// matrix is singular 
vector<vector<double>> Model::inverse(vector<vector<double>> A)
{
	int n = A.size();
	double det = luDecomposition(A, A.size()),
		temp;

	vector<vector<double>> inverse(n, vector<double>(n, 0));
	vector<vector<double>> aug(2 * n, vector<double>(2 * n, 0));

	if (fabs(det) > 0)
	{
		for (int i = 0; i < n; i++)

			for (int j = 0; j < n; j++)

				aug[i][j] = A[i][j];

		// Create the augmented matrix 
		// Add the identity matrix 
		// of n at the end of orignal matrix. 
		for (int i = 0; i < n; i++) {

			for (int j = 0; j < 2 * n; j++) {

				// Add '1' at the diagonal places of 
				// the matrix to create a identity matirx 
				if (j == (i + n))
					aug[i][j] = 1;
			}
		}

		// Interchange the row of matrix, 
		// interchanging of row will start from the last row 
		for (int i = n - 1; i > 0; i--) {

			if (aug[i - 1][0] < aug[i][0])
				for (int j = 0; j < 2 * n; j++) {

					// Swapping of the row, if above 
					// condition satisfied. 
					temp = aug[i][j];
					aug[i][j] = aug[i - 1][j];
					aug[i - 1][j] = temp;
				}
		}

		// Replace a row by sum of itself and a 
		// constant multiple of another row of the matrix 
		for (int i = 0; i < n; i++) {

			for (int j = 0; j < 2 * n; j++) {

				if (j != i) {

					temp = aug[j][i] / aug[i][i];
					for (int k = 0; k < 2 * n; k++) {

						aug[j][k] -= aug[i][k] * temp;
					}
				}
			}
		}

		// Multiply each row by a nonzero integer. 
		// Divide row element by the diagonal element 
		for (int i = 0; i < n; i++)
		{

			temp = aug[i][i];
			for (int j = 0; j < 2 * n; j++)
			{

				aug[i][j] = aug[i][j] / temp;
			}
		}

		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				inverse[i][j] = aug[i][j + n];
	}
	else
		inverse.clear();

	return inverse;
}

double Model::luDecomposition(vector<vector<double>> mat, int n)
{
	vector<vector<double>>	lower(n, vector<double>(n, 0));
	vector<vector<double>>	upper(n, vector<double>(n, 0));

	// Decomposing matrix into Upper and Lower 
	// triangular matrix 
	for (int i = 0; i < n; i++) {

		// Upper Triangular 
		for (int k = i; k < n; k++) {

			// Summation of L(i, j) * U(j, k) 
			int sum = 0;
			for (int j = 0; j < i; j++)
				sum += (lower[i][j] * upper[j][k]);

			// Evaluating U(i, k) 
			upper[i][k] = mat[i][k] - sum;
		}

		// Lower Triangular 
		for (int k = i; k < n; k++) {
			if (i == k)
				lower[i][i] = 1; // Diagonal as 1 
			else {

				// Summation of L(k, j) * U(j, i) 
				int sum = 0;
				for (int j = 0; j < i; j++)
					sum += (lower[k][j] * upper[j][i]);

				// Evaluating L(k, i) 
				lower[k][i] = (mat[k][i] - sum) / upper[i][i];
			}
		}
	}

	/*	// Displaying the result :
		for (int i = 0; i < n; i++) {
			// Lower
			for (int j = 0; j < n; j++)
				cout << lower[i][j] << "\t";
			cout << "\t";

			// Upper
			for (int j = 0; j < n; j++)
				cout << upper[i][j] << "\t";
			cout << endl;
		}*/

	double detU = 1;

	// Determinant
	for (int i = 0; i < n; i++)
	{
		detU *= upper[i][i];
	}

	return detU;
}

// Print vector
void Model::print(vector< vector<double> > A)
{
	int n = A.size();

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%10.4f\t", A[i][j]);
		}
		printf_s("\n");
	}

	printf_s("\n");
}

void Model::Print_Output(CString FileName)
{	
	int sw;

	errno_t err;
	FILE *fOut;

	err = fopen_s(&fOut, FileName, "w");

	if (err == 0)
		printf("The file %s was opened\n", FileName);
	else
		printf("The file %s was not opened\n", FileName);

	fprintf_s(fOut, "Squeeze V - Near well squeeze treatment simulator\n\n");

	(fOut, "%s\n", title1);
	fprintf_s(fOut, "%s\n", title2);

	if (nLay > 1)
		fprintf_s(fOut, "This is the MULTI-LAYER RADIAL model.\n\n");
	else
		fprintf_s(fOut, "This is the SINGLE-LAYER RADIAL model.\n\n");

	fprintf_s(fOut, "Well Geometry:\n\n");

	fprintf_s(fOut, "          %5.2f feet        %d  annuli, each  %5.4f feet of radius\n", radiusWell, nBlocks, (radiusMax - radiusWell) / nBlocks);

	fprintf_s(fOut, "          |<------>|\n");
	fprintf_s(fOut, "                                                   \\    \\    \\    \\\n");
	fprintf_s(fOut, " (        +        )    )    )   . . . . . . . . .  |    |    |    |\n");
	fprintf_s(fOut, "                                                   /    /    /    /\n");
	fprintf_s(fOut, " |<-- Well Bore -->|\n");
	fprintf_s(fOut, "          |<-------------------  %5.2f feet ---------------------->|\n\n", radiusMax);

//	fprintf(fOut, "NO RESERVOIR TEMPERATURE SIMULATION\n\n");

	fprintf_s(fOut, "Stage   Duration       Well Mode    Well  Rate");
	for (sw = 0; sw<nComponentsWater; sw++)
		fprintf_s(fOut, "    Well Conc.     ");
	fprintf_s(fOut, "Well Temp. \n");
	fprintf_s(fOut, " No.   (Hours/days)                  (Bpm/Bpd)");
	for (sw = 0; sw<nComponentsWater; sw++)
		fprintf_s(fOut, "  WatComp%2d (ppm)  ", sw + 1);
	fprintf_s(fOut, "   (deg C)\n");
	fprintf_s(fOut, "-----  ------------  -------------  ----------");
	for (sw = 0; sw<nComponentsWater; sw++)
		fprintf_s(fOut, "  ---------------");
	fprintf_s(fOut, "  ---------- \n");

	for (int j = 0; j< nStg; j++)
	{
		/*if (stg[j].TypeStg == 1 || stg[j].TypeStg == 2 || stg[j].TypeStg == 5)
		{
			// To convert form m3/min to bbl/min
			if (UnitSpec != 'M' && UnitSpec != 'm')

				ConvQ = 1.0 / 0.158987295;

			else ConvQ = 1.0;

			// To convert form m3/sec to m3/min
			ConvQ *= 60;

			// To convert from sec to hours 
			ConvTime = 1.0 / 3600;

		}

		if (stg[j].TypeStg == 3)
		{
			// To convert form m3/day to bbl/day 
			if (UnitSpec != 'M' && UnitSpec != 'm')

				ConvQ = 1.0 / 0.158987295;

			else ConvQ = 1.0;

			// To convert form m3/sec to m3/day
			ConvQ *= 86400;

			// To convert from sec to days
			ConvTime = 1.0 / 86400;

		}*/

		char T[128];

		if (stg[j].TypeStg == 1) strcpy(T, "Inh.Injection");

		if (stg[j].TypeStg == 2) strcpy(T, "Water Inject");

		if (stg[j].TypeStg == 3) strcpy(T, "Production");

		if (stg[j].TypeStg == 5) strcpy(T, "Shut-in");

		fprintf_s(fOut, "%3d    %12.3f  %-13s  %10.3f", j + 1, stg[j].Time, T, stg[j].Qwt);
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "	%15.3f	", stg[j].InitialCw[sw]);
		fprintf_s(fOut, "%12.3f\n", Temp.Ftemp);
	}

	fprintf_s(fOut, "\n Simulating ...\n\n");

	if (IsCon == 0 && iModel == 3) // No in situ concentrations versus distance 
	{	
		if (kPlot == 0 || kPlot == 2)
			fprintf_s(fOut, "	Elapsed Time");

		if (kPlot == 1 || kPlot == 3)
			fprintf_s(fOut, "	Cum.Water Vol.");
		
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "	Well Concentration  Cumulative Production     Recovered     Well Bottomhole Temp.");
		fprintf_s(fOut, "\n");

		if (kPlot == 0 || kPlot == 2)
			fprintf_s(fOut, "       (days)");

		if (kPlot == 1 || kPlot == 3)
		{	
			if (UnitSpec != 'C' && UnitSpec != 'c')

				fprintf_s(fOut, "(1000bbls)");

			if (UnitSpec != 'M' && UnitSpec != 'm')
				
				fprintf_s(fOut, "    (100m3)");
		}
			

		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "          Comp %d (ppm)      Comp %d (ppm.bbls)     Comp %d (percent)          (deg C)      ", sw + 1, sw + 1, sw + 1);
		fprintf_s(fOut, "\n");
		fprintf_s(fOut, "    ------------");
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "    ------------------  ---------------------  ----------------  ----------------");
		fprintf_s(fOut, "\n");
	}

	if (IsCon == 0 && iModel == 4) // No in situ concentrations versus distance 
	{
		fprintf_s(fOut, "	Elapsed Time    Pore Volume");
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "	Well Concentration  Cumulative Production     ");
		fprintf_s(fOut, "\n");

		fprintf_s(fOut, "(Hours)");
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "          Comp %d (ppm)      Comp %d (ppm.bbls)     Comp %d (percent)", sw + 1, sw + 1, sw + 1);
		fprintf_s(fOut, "\n");
		fprintf_s(fOut, "    ------------");
		for (sw = 0; sw<nComponentsWater; sw++)
			fprintf_s(fOut, "    ------------------  ---------------------  ----------------");
		fprintf_s(fOut, "\n");
	}

	if (IsCon == 0)
	{
		if (iModel == 3)
		{	
			for (int i = 0; i<RT.size(); i++)
			{
				fprintf(fOut, "   %12.3f", RT[i]);

				fprintf(fOut, "    %18.5f", RC[i]);

				fprintf(fOut, "    %18.5f", (TotalInhMass <= 0 ? 0.0 : 100 * CumProd[i] / TotalInhMass));

				fprintf(fOut, "    %18.5f", RTemp[i]);

				fprintf(fOut, "\n");
			}
			
		}

		if (iModel == 4)
		{	
			for (int i = 0; i < RT.size(); i++)
			{
				fprintf(fOut, "   %12.3f", Time[i] / 86400);

				fprintf(fOut, "   %12.3f", RT[i]);

				for (sw = 0; sw < nComponentsWater; sw++)
					fprintf(fOut, "    %18.5f", RC[i]);

				for (sw = 0; sw < nComponentsWater; sw++)
					fprintf(fOut, "    %18.5f", CumProd[i]);

				fprintf(fOut, "\n");
			}
		}

	}

	/*if (IsCon == 1) // In situ concentrations versus distance 
	{
		if (Model.iModel == 3)
		{
			for (l = 0; l<nLayers; l++)
			{
				fprintf(fOut, "\nInhibitor Concentrations for Layer  %d\n\n", l + 1);

				fprintf(fOut, "At time t =   %12.4f days, the in-situ inhibitor concentration profile looks like:\n\n", time / 86400);

				for (sw = 0; sw<nComponentsWater; sw++)
					fprintf(fOut, "Comp %2d Injected: %12.4f Total mass: %12.4f\n", sw + 1,
					(watMassInj[l][sw] + oilMassInj[l][sw]), (oilMass[l][sw] + oilMassLea[l][sw] + oilMassWell[l][sw])
					+ (watMass[l][sw] + watMassLea[l][sw] + watMassWell[l][sw]) + (oilMassAds[l][sw] + watMassAds[l][sw]));

				fprintf(fOut, "         Distance from well      ");

				for (sw = 0; sw<nComponentsWater; sw++)
					fprintf(fOut, "Inhibitor Concentration WatComp %2d          Inhibitor Concentration OilComp %2d", sw + 1, sw + 1);
				fprintf(fOut, "\n");

				for (sw = 0; sw<nComponentsWater; sw++)
					fprintf(fOut, "              (feet)          Mobile Phase (ppm)   Adsorped Phase (ppm)   Mobile Phase (ppm)   Adsorped Phase (ppm)");
				fprintf(fOut, "\n");

				fprintf(fOut, "         ------------------ ");
				for (sw = 0; sw<nComponentsWater; sw++)
					fprintf(fOut, "  ------------------   --------------------   ------------------   --------------------");
				fprintf(fOut, "\n");

				for (b = 0; b<nBlocks; b++)
				{
					fprintf(fOut, "              %12.4f", (radius[b] + radius[b + 1])*ConvRad / 2.0);

					for (sw = 0; sw<nComponentsWater; sw++)
						fprintf(fOut, "    %15.4f        %15.4f      %15.4f        %15.4f\n",
						wConc[b][l][sw] * 1e3, wAds[b][l][sw], oConc[b][l][sw] * 1e3, oAds[b][l][sw]);
				}
			}

		} // Model.iModel == 3

		if (Model.iModel == 4)
		{
			// Set l = 0 by default since there is only one layer
			l = 0;

			fprintf(fOut, "\nAt time t =   %12.4f hours, the in-situ inhibitor concentration profile looks like:\n\n", time / 3600);

			for (sw = 0; sw<nComponentsWater; sw++)
				fprintf(fOut, "Comp %2d Injected: %12.4f Total mass: %12.4f\n", sw + 1,
				(watMassInj[l][sw] + oilMassInj[l][sw]), (oilMass[l][sw] + oilMassLea[l][sw] + oilMassWell[l][sw])
				+ (watMass[l][sw] + watMassLea[l][sw] + watMassWell[l][sw]) + (oilMassAds[l][sw] + watMassAds[l][sw]));

			fprintf(fOut, "         Distance along core      ");

			for (sw = 0; sw<nComponentsWater; sw++)
				fprintf(fOut, "Inhibitor Concentration WatComp %2d          Inhibitor Concentration OilComp %2d", sw + 1, sw + 1);
			fprintf(fOut, "\n");

			for (sw = 0; sw<nComponentsWater; sw++)
				fprintf(fOut, "              (Normalized)          (Normalized)   Adsorped Phase (ppm)   (Normalized)   Adsorped Phase (ppm)");
			fprintf(fOut, "\n");

			fprintf(fOut, "         ------------------ ");
			for (sw = 0; sw<nComponentsWater; sw++)
				fprintf(fOut, "  ------------------   --------------------   ------------------   --------------------");
			fprintf(fOut, "\n");

			for (b = 0; b<nBlocks; b++)
			{
				fprintf(fOut, "              %12.4f", (radius[b]) / resDim.radiusMax);

				for (sw = 0; sw<nComponentsWater; sw++)
					fprintf(fOut, "    %15.4f        %15.4f      %15.4f        %15.4f\n",
					wConc[b][l][sw] / waterComp[sw].MaxC, wAds[b][l][sw], oConc[b][l][sw] / oilComp[sw].MaxC, oAds[b][l][sw]);
			}


		} // Model.iModel == 4

	}*/
	
	fprintf_s(fOut, "\n\n... simulation complete.\n\n");

	for (int i = 0; i < nLife; i++)
	{
		if (Clife[i] > 0)
		{
			if (TVlife[i] > 0)
			{
				fprintf(fOut, "\nInhibitor lifetime (from start of treatment) above\n");

				fprintf(fOut, " %12.3f ppm was  %12.3f days", Clife[i], TVlife[i]);
			}

			else
			{
				fprintf(fOut, "Never crossed %12.3f ppm\n", Clife[i]);
			}

		}

	}

	fprintf(fOut, "\n");

	fclose(fOut);
}

//------------------------------------------------------------------------------------------
void OneLayerData::ReadOutput(const char *fname, const char *fname2, int l)
{
	FILE *filePtr;
	errno_t err;
	OneConcOut C;		// a structure to store a single line (actually, part of it) from the file
	const size_t BUFFSIZE = 4096;	
	char buffer[BUFFSIZE];
	double tmp1, tmp2, tmp3;

	if ((err = fopen_s(&filePtr, fname, "r")) != 0)
		printf("Error opening file %s for reading\r\n", fname);
	else
		printf("File %s was opened for reading\r\n", fname);

	fseek(filePtr, 0L, SEEK_SET);		// set pointer to beginning of file
	fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header

	conc = vector<OneConcOut>();	// assign an empty vector to erase any previous data in "conc"
	
	int e = 1,
		num = 0;
	
	bool check = true;

	Obj = 1e20;

 	while (e != EOF && e > 0)
	{
		e = fscanf_s(filePtr, "%d %d %lf %lf", &C.stage, &C.TypeStage, &C.time, &C.TotSI);

		for (int i = 0; i < nLay * 4; i++)
		{
			if (i == 3) e = fscanf_s(filePtr, "%lf ", &C.Temp);

			else e = fscanf_s(filePtr, "%lf ", &tmp1);
		}

		if (e != EOF && e > 0)
		{
			conc.push_back(C);		// add new entry to the vector

			// Only at Production Stages
			if (C.TypeStage == 3)
			{
				if (GrafMIC > C.TotSI)
				{	
					// This condition to check the profile is decreasing
					if (C.TotSI < conc.at(num - 1).TotSI) 
					{
						Obj = conc.at(num).time;
						e = 0;
					}
						
				}
				else // To check that the return profile in the production is anove MIC
				{
					check = false;
				}

				if (check) Obj = 1;
			}

			num++;
		}
	}
	
	fclose(filePtr);

	if (nSim == 0) // Parallel Single Simulation
	{
		if ((err = fopen_s(&filePtr, fname2, "r")) != 0)
			printf("Error opening file %s for reading\r\n", fname);
		else
			printf("File %s was opened for reading\r\n", fname);

		fseek(filePtr, 0L, SEEK_SET);		// set pointer to beginning of file
		fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
		fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
		fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
		
		
		// Radius
		for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
		{
			fscanf_s(filePtr, "%lf", &tmp1);
			conc[l].Rad.push_back(tmp1);
		}

		// Concentration
		for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
		{
			fscanf_s(filePtr, "%lf", &tmp1);
			conc[l].LayConc.push_back(tmp1);
		}

		// Adsoprtion/Precipiation
		for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
		{
			fscanf_s(filePtr, "%lf", &tmp1);
			conc[l].LayAP.push_back(tmp1);
		}

		// Temperature
		for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
		{
			fscanf_s(filePtr, "%lf", &tmp1);
			conc[l].LayTemp.push_back(tmp1);
		}

		e = 1;
		while (e != EOF && e > 0)
		{
			fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
			fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
			fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header
			fgets(buffer, BUFFSIZE, filePtr);	// skip one line of header

			if (e != EOF && e > 0)
			{
				// Concentration
				for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
				{
					e = fscanf_s(filePtr, "%lf", &tmp1);
					conc[l].LayConc.push_back(tmp1);
				}

				// Adsoprtion/Precipiation
				for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
				{
					e = fscanf_s(filePtr, "%lf", &tmp1);
					conc[l].LayAP.push_back(tmp1);
				}

				// Temperature
				for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
				{
					e = fscanf_s(filePtr, "%lf", &tmp1);
					conc[l].LayTemp.push_back(tmp1);
				}
			}
		}
		
		fclose(filePtr);
	}

}

//------------------------------------------------------------------------------------------
//void OneLayerData::ReadMod(const char *fname)
//{
//	mod.ReadFileSQZ5Radial(fname);
//}
//------------------------------------------------------------------------------------------
vector<TotConcOut> OneLayerData::WriteTotalConc(const char *fname, const vector<OneLayerData*> &OL)
{
	// NOTE!
	// "assert" might be replaced by throwing exceptions

	// ---------------------------------
	// stage I, create the output vector

	size_t NLAY = OL.size();
	assert(NLAY != 0);
	
	size_t tSteps = OL[0]->conc.size();
	size_t nBlocks = OneLayerData::mod->nBlocks;
	size_t NSTG = OneLayerData::mod->nStg;
	for (size_t w = 0; w < NLAY; w++)
	{
		assert(OL[w]->conc.size() == tSteps);	// checking that all layers have the same number of time steps
		//assert(OL[w]->mod.nStg == NSTG);
	}

	vector<TotConcOut> RES(tSteps);		// the resulting vector
	vector<double> AUX(NLAY);			// auxiliary vector for TotConcOut entries
	
	for (size_t t = 0; t < tSteps; t++)		// time step
	{
		RES[t].stageNum = OL[0]->conc[t].stage;
		
		// I need to add all the cumulative water produced for all layers
		if (OneLayerData::mod->kPlot == 1)
		{	
			RES[t].time = 0;

			for (size_t w = 0; w < NLAY; w++)	// layer
			{
				RES[t].time += OL[w]->conc[t].time;
			}
		}
		else RES[t].time = OL[0]->conc[t].time;

		double c = 0, tp = 0, d = 0;				// for making averaging for TotReturnSI
		vector<double> vC(NLAY), vD(NLAY);	// for making averaging for WellSI(l)
		for (size_t w = 0; w < NLAY; w++)
			vC[w] = vD[w] = 0;

		for (size_t w = 0; w < NLAY; w++)	// layer
		{
			int S = OL[w]->conc[t].stage - 1;	// stage, zero-based
			assert(S >= 0 && S < (int)NSTG);
		
			c += OL[w]->conc[t].TotSI * OneLayerData::mod->stg[S].QwLay[w];	// making the weighted sum
			tp += OL[w]->conc[t].Temp * OneLayerData::mod->stg[S].QwLay[w];	// making the weighted sum
			d += OneLayerData::mod->stg[S].QwLay[w];

			for (size_t L = 0; L < NLAY; L++)
				if (L <= w)
				{
					vC[L] += OL[w]->conc[t].TotSI * OneLayerData::mod->stg[S].QwLay[w];	// making the weighted sum
					vD[L] += OneLayerData::mod->stg[S].QwLay[w];
				}
		}
		assert(d != 0 || c == 0);
		RES[t].TotSI = 0;
		RES[t].Twl = 0;
		if (d != 0)
		{
			RES[t].TotSI = c / d;
			RES[t].Twl = tp / d;
		}
		for (size_t w = 0; w < NLAY; w++)
		{
			assert(vD[w] != 0 || vC[w] == 0);
			if (vD[w] != 0)	
				vC[w] /= vD[w];	// vC stores WellSI(l)	
			else
				vC[w] = 0;	
		}

		for (size_t w = 0; w < NLAY; w++)
			AUX[w] = OL[w]->conc[t].TotSI;		// LayerSI
		RES[t].LaySI = AUX;
		
		for (size_t w = 0; w < NLAY; w++)
			AUX[w] = OL[w]->conc[t].Temp;		// LayerTemp
		RES[t].LayTemp = AUX;

		RES[t].WellSI = vC;						// WellSI

		double Tot = 0;
		for (size_t w = 0; w < NLAY; w++)
		{
			int S = OL[w]->conc[t].stage - 1;	// stage, zero-based
			AUX[w] = OneLayerData::mod->stg[S].QwLay[w];		// Wprd
			Tot += AUX[w];
		}

		RES[t].Wprd = AUX;
		for (int w = NLAY-1; w >= 0 ; w--)
		{	
			if (Tot > 0)
				RES[t].Wprd[w] = AUX[w] / Tot;
			else
				RES[t].Wprd[w] = AUX[w];

			if (w < NLAY - 1) RES[t].Wprd[w] += RES[t].Wprd[w + 1];
		}
			
	}

	// ----------------------------
	// stage II, output to the file

	if (fname != 0)
	{
		FILE *filePtr;
		errno_t err;
		if ((err = fopen_s(&filePtr, fname, "w")) != 0)
			printf("Error opening file %s for writing\r\n", fname);
		else
			printf("File %s was opened for writing\r\n", fname);

		fprintf_s(filePtr, "%11s\t%11s","Time/Vol","TotReturnSI");		// header
		
		char buffer[20];
		int n;

/*		for (size_t w = 0; w < NLAY; w++)
		{	
			n = sprintf_s(buffer, "LayerSI%.2d", w+1);
			fprintf_s(filePtr, "\t%11s", buffer,n);
		}
		for (size_t w = 0; w < NLAY; w++)
		{
			n = sprintf_s(buffer, "WellSI%.2d", w + 1);
			fprintf_s(filePtr, "\t%11s", buffer, n);
		}
		for (size_t w = 0; w < NLAY; w++)
		{
			n = sprintf_s(buffer, "Wprd%.2d", w + 1);
			fprintf_s(filePtr, "\t%11s", buffer, n);
		}
		
		for (size_t w = 0; w < NLAY; w++)
		{
			n = sprintf_s(buffer, "Temp%.2d", w + 1);
			fprintf_s(filePtr, "\t%11s", buffer, n);
		}
		
		fprintf_s(filePtr, "\n");
*/		
/*		OneLayerData::mod->TVlife.size(OneLayerData::mod->nLife);
		for (size_t w = 0; w < OneLayerData::mod->nLife; w++)
			OneLayerData::mod->TVlife[w] = 0;

		for (size_t t = 0; t < tSteps; t++)		// time step
		{	
			fprintf_s(filePtr, "%11.4f\t%11.4f", RES[t].time, RES[t].TotSI);	// TotReturnSI
			for (size_t w = 0; w < NLAY; w++)
				fprintf_s(filePtr, "\t%11.4f", RES[t].LaySI[w]);		// LayerSI(w)
			for (size_t w = 0; w < NLAY; w++)
				fprintf_s(filePtr, "\t%11.4f", RES[t].WellSI[w]);		// WellSI(w)
			for (size_t w = 0; w < NLAY; w++)
				fprintf_s(filePtr, "\t%11.4f", RES[t].Wprd[w]);			// Wprd(w)
			for (size_t w = 0; w < NLAY; w++)
				fprintf_s(filePtr, "\t%11.4f", RES[t].LayTemp[w] / OneLayerData::mod->Temp.Ftemp);		// LayTemp(w)
			fprintf_s(filePtr, "\n");

			// Only in Production Stages 

			OneLayerData::mod->TVlife.size(OneLayerData::mod->nLife);

			if (OneLayerData::mod->stg[RES[t].stageNum-1].TypeStg == 3)
			{
				for (size_t w = 0; w < OneLayerData::mod->nLife; w++)
				{
					if (OneLayerData::mod->Clife[w] > RES[t].TotSI && OneLayerData::mod->TVlife[w] == 0)

						OneLayerData::mod->TVlife[w] = RES[t].time;
				}
			}
		}

		fclose(filePtr);*/
/*
		string temp,tempLfname(fname);
		size_t p = tempLfname.find_last_of("\\");
		tempLfname = tempLfname.substr(0, p);
		temp = tempLfname + "\\dist.out";

		if ((err = fopen_s(&filePtr, temp.c_str(), "w")) != 0)
			printf("Error opening file %s for writing\r\n", temp);
		else
			printf("File %s was opened for writing\r\n", temp);

		for (size_t t = 0; t < tSteps; t++)		// time step
		{
			fprintf_s(filePtr, "Time %11.4f StageNum  %d Type %d\n", RES[t].time, RES[t].stageNum, OneLayerData::mod->stg[RES[t].stageNum - 1].TypeStg);
			
			for (size_t l = 0; l < NLAY; l++)		// Layers
			{
				fprintf_s(filePtr, "Layer %11d\n", l+1);
				fprintf_s(filePtr, "Distance Conc Adso / ppt Temp\n");

				if (t == 0 && l == 0)
				{	
					// Radius
					for (size_t b = 0; b < nBlocks; b++)
						fprintf_s(filePtr, "%15.3f ", OL[l]->conc[t].Rad[b]);
					fprintf_s(filePtr, "\n");
				}
				
				// Concentraion
				for (size_t b = t * nBlocks ; b < ((t+1)*nBlocks); b++)
					fprintf_s(filePtr, "%15.3f ", OL[l]->conc[0].LayConc[b]);
				fprintf_s(filePtr, "\n");

				// Adsorption/Precipitation
				for (size_t b = t * nBlocks; b < ((t + 1)*nBlocks); b++)
					fprintf_s(filePtr, "%15.3f ", OL[l]->conc[0].LayAP[b]);
				fprintf_s(filePtr, "\n");

				// Temperature
				for (size_t b = t * nBlocks; b < ((t + 1)*nBlocks); b++)
					fprintf_s(filePtr, "%15.3f ", OL[l]->conc[0].LayTemp[b]);
				fprintf_s(filePtr, "\n");
			}
		}

		fclose(filePtr);*/
	}

	for (size_t t = 0; t < RES.size(); t++)

		RES[t].TypeStage = OneLayerData::mod->stg[RES[t].stageNum - 1].TypeStg;

	return RES;
}
//------------------------------------------------------------------------------------------

	