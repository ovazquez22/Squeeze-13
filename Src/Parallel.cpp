#include "stdafx.h"
#include "Parallel.h"


Parallel::Parallel(string full, string outputfile)
{	
	try
	{
		string input,
			   concout,
			   input_short;
		size_t i;

		_chdir(full.c_str());
		input = "input.dat";
		concout = "conc_output.txt";

		i = input.find_last_of(".");
		if (i < string::npos)
			input_short = input.substr(0, i);
		else
			input_short = input;

	/*	i = full.find_last_of("\\");
		concout = full.substr(0, i);
		concout += "\\Exec\\conc.out";*/

		Model Mod;
		Mod.suff = "_run_";
		Mod.ReadFileSQZ5Radial(input.c_str());		// read input data
		Mod.WriteSingleLayers(input.c_str());		// write input for each separate layer

		nLayer = Mod.nLay;
		nBlocks = Mod.nBlocks;
		iModel = Mod.iModel;
		kPlot = Mod.kPlot;
		UnitSpec = Mod.UnitSpec;

		vector<OneRun*> OLD(Mod.nLay);
		
	//	data.resize(Mod.nLay);

		for (int i = 0; i < Mod.nLay; i++)
		{
			OLD[i] = new OneRun;		// to be deleted manually!
			/*OLD[i]->nLay = 1;
			OLD[i]->nSim = 0;*/
			LayHeight.push_back(Mod.SLay[i].HLay);
		}
		
		fX = Mod.fX;
		fY = Mod.fY;
		oldModels = Mod.oldModels;
		
		JobsPool JP(OLD);
		JP.finput = input_short;
		JP.fconc = "conc";		// these file names should be provided by USER (to be implemented)
		JP.fdist = "dist";
		JP.suff = "_run_";		// suffix

		// define the number of processes (threads);
		// this number can be modified by USER (to be implemented)
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		int nproc = sysinfo.dwNumberOfProcessors;

		cout << "Number of processors: " << nproc << "\r\n";	// TEST; DELETE if necessary

		nproc -= 1;

		if (nproc < 1)
			throw Exception("Number of threads should be greater than zero");

		vector<thread*> threads(nproc);
		for (int j = 0; j < nproc; j++)
			threads[j] = new thread(&JobsPool::Run, &JP);	// start nproc threads

		// the threads are running here!

		for (int j = 0; j < nproc; j++)
		{
			threads[j]->join();		// wait for all threads to finish
			delete threads[j];		// and delete them
		}

		if (JP.E)
			rethrow_exception(JP.E);	// rethrow exception from the worker thread

		TOTAL = JP.WriteOutput(concout.c_str());		// write the final files
		
		PrintOutFile(outputfile.c_str(), TOTAL);

		for (int b = 0; b < Mod.nBlocks; b++)

			Rad = OLD[0]->conc[0].Rad;

		for (int t = 0; t < TOTAL.size(); t++)
		{
			for (int l = 0; l < Mod.nLay; l++)
			{	
				for (size_t b = t * nBlocks; b < ((t + 1)*nBlocks); b++)
				{
					conc.push_back(OLD[l]->conc[0].LayConc[b]);
					ap.push_back(OLD[l]->conc[0].LayAP[b]);
					temp.push_back(OLD[l]->conc[0].LayTemp[b]);
				}
			}
		}

		// clean the memory
		for (int i = 0; i < Mod.nLay; i++)
		{	
			delete OLD[i];
		}

	}
	catch (Exception &e)
	{
		cout << e.what();
	}
}


Parallel::~Parallel()
{
/*	for (int i = 0; i < nLayer; i++)

		delete data[i];*/
		
}

//----------------------------------------------------------------------------------------
void Parallel::RunSim(const char *NameFile)
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
void Parallel::PrintOutFile(const char *fname, vector<TotConcOut> TOTAL)
{
	char T[20];

	FILE *fOut;
	size_t err;

	string tempLfname;
	tempLfname = fname;
	size_t p = tempLfname.find_last_of(".");
	if (p < string::npos)
		tempLfname = tempLfname.substr(0, p);
	else
		tempLfname = tempLfname;

	tempLfname += ".out";

	if ((err = fopen_s(&fOut, tempLfname.c_str(), "w")) != 0)
		printf("Error opening file %tempLfnames for writting\r\n", tempLfname);
	else
		printf("File %s was opened for writting\r\n", tempLfname);


	fprintf_s(fOut, "Squeeze VI - Two Phase Near WellBore Squeeze Treatment Simulator\n\n");

	fprintf_s(fOut, "%s", OneLayerData::mod->title1);
	fprintf_s(fOut, "%s\n", OneLayerData::mod->title2);

	if (OneLayerData::mod->nLay > 1)
		fprintf_s(fOut, "This is the MULTI-LAYER RADIAL model.\n\n");
	else
		fprintf_s(fOut, "This is the SINGLE-LAYER RADIAL model.\n\n");

	fprintf_s(fOut, "Well Geometry:\n\n");

	fprintf_s(fOut, "          %5.2f feet        %d  annuli, each  %5.4f feet of radius\n",
		OneLayerData::mod->radiusWell, OneLayerData::mod->nBlocks, (OneLayerData::mod->radiusMax - OneLayerData::mod->radiusWell) / OneLayerData::mod->nBlocks);

	fprintf_s(fOut, "          |<------>|\n");
	fprintf_s(fOut, "                                                   \\    \\    \\    \\\n");
	fprintf_s(fOut, " (        +        )    )    )   . . . . . . . . .  |    |    |    |\n");
	fprintf_s(fOut, "                                                   /    /    /    /\n");
	fprintf_s(fOut, " |<-- Well Bore -->|\n");
	fprintf_s(fOut, "          |<-------------------  %5.2f feet ---------------------->|\n\n", OneLayerData::mod->radiusMax);

	fprintf_s(fOut, "NO RESERVOIR TEMPERATURE SIMULATION\n\n");

	fprintf_s(fOut, "Stage   Duration       Well Mode    Well  Rate");
	fprintf_s(fOut, "    Well Conc.");
	fprintf_s(fOut, "     Well Temp. \n");
	fprintf_s(fOut, " No.   (Hours/days)                  (Bpm/Bpd)");
	fprintf_s(fOut, "      (ppm)");
	fprintf_s(fOut, "         (deg C)\n");
	fprintf_s(fOut, "-----  ------------  -------------  ----------");
	fprintf_s(fOut, "  ---------------");
	fprintf_s(fOut, "  ---------- \n");

	double	TotalInhInj = 0,
		TotalInhPro = 0;

	for (int j = 0; j< OneLayerData::mod->nStg; j++)
	{
		if (OneLayerData::mod->stg[j].TypeStg == 1) strcpy_s(T, "Inh.Injection");

		if (OneLayerData::mod->stg[j].TypeStg == 2) strcpy_s(T, "Water Inject");

		if (OneLayerData::mod->stg[j].TypeStg == 3) strcpy_s(T, "Production");

		if (OneLayerData::mod->stg[j].TypeStg == 5) strcpy_s(T, "Shut-in");

		fprintf_s(fOut, "%3d    %12.3f  %-13s  %10.3f", j + 1, OneLayerData::mod->stg[j].Time, T, OneLayerData::mod->stg[j].Qwt);
		fprintf_s(fOut, "  %15.3f ", OneLayerData::mod->stg[j].InitialCw);
		fprintf_s(fOut, "%12.3f\n", OneLayerData::mod->Temp.Ftemp);

		if (OneLayerData::mod->stg[j].TypeStg == 1)

			TotalInhInj += OneLayerData::mod->stg[j].InitialCw * OneLayerData::mod->stg[j].Qwt * OneLayerData::mod->stg[j].Time *60.0;

	}

	fprintf_s(fOut, "\n Simulating ...\n\n");

	if (OneLayerData::mod->IsCon == 0 && OneLayerData::mod->iModel == 3) // No in situ concentrations versus distance 
	{
		fprintf_s(fOut, "	Elapsed Time");
		fprintf_s(fOut, "	Well Concentration  Cumulative Production     Recovered   Well Bottomhole Temp.");
		fprintf_s(fOut, "\n");

		fprintf_s(fOut, "       (days)");
		fprintf_s(fOut, "              (ppm)             (ppm.bbls)          (percent)        (deg C)");
		fprintf_s(fOut, "\n");
		fprintf_s(fOut, "    ------------");
		fprintf_s(fOut, "    ------------------  ---------------------     ---------   --------------------");
		fprintf_s(fOut, "\n");
	}

	if (OneLayerData::mod->IsCon == 0)
	{
		for (size_t t = 0; t < TOTAL.size(); t++)
		{	
			if (OneLayerData::mod->stg[TOTAL[t].stageNum - 1].TypeStg == 3)

				TotalInhPro += TOTAL[t].TotSI * OneLayerData::mod->stg[TOTAL[t].stageNum - 1].Qwt * OneLayerData::mod->stg[TOTAL[t].stageNum - 1].TimeStep * double(OneLayerData::mod->nFreq);

			fprintf_s(fOut, " %12.4f", TOTAL[t].time);

			fprintf_s(fOut, "    %18.3f", TOTAL[t].TotSI);

			fprintf_s(fOut, "    %18.2f  %15.3f", TotalInhPro, (TotalInhInj <= 0 ? 0.0 : 100 * TotalInhPro / TotalInhInj));

			fprintf_s(fOut, "    %12.3f", TOTAL[t].Twl);

			fprintf_s(fOut, "\n");
		}

	}

	if (OneLayerData::mod->IsCon == 1) // In situ concentrations versus distance 
	{
		for (size_t t = 0; t < TOTAL.size(); t++)
		{
			for (size_t l = 0; l < OneLayerData::mod->nLay; l++)
			{
				fprintf_s(fOut, "\nInhibitor Concentrations for Layer  %d\n\n", l + 1);

				fprintf_s(fOut, "At time t =   %12.4f days, the in-situ inhibitor concentration profile looks like:\n\n", TOTAL[t].time / 86400);

				fprintf_s(fOut, "         Distance from well      ");

				fprintf_s(fOut, "Inhibitor Concentration         ");
				fprintf_s(fOut, "\n");

				fprintf_s(fOut, "              (feet)          Mobile Phase (ppm)   Adsorped Phase (ppm)");
				fprintf_s(fOut, "\n");

				fprintf_s(fOut, "         ------------------ ");
				fprintf_s(fOut, "  ------------------   --------------------   ");
				fprintf_s(fOut, "\n");

				for (size_t b = 0; b < OneLayerData::mod->nBlocks; b++)
				{
					fprintf_s(fOut, "              %12.4f", 0);

					fprintf_s(fOut, "    %15.4f        %15.4f      \n", 0, 0);
				}
			}
		}
	}

	fprintf_s(fOut, "\n ... simulation complete.\n\n");
	fprintf_s(fOut, "A total of %15.2f ppm.bbls produced,\n", TotalInhPro);
	fprintf_s(fOut, "\t from  %15.2f ppm.bbls injected: %6.3f percenatge returned.\n\n", TotalInhInj, (TotalInhInj <= 0 ? 0.0 : 100 * TotalInhPro / TotalInhInj));

	string st = "days";

	if (OneLayerData::mod->kPlot == 1)
	{
		st = "(1000bbls)";

		if (OneLayerData::mod->UnitSpec == 'M' || OneLayerData::mod->UnitSpec == 'm')

			st = "(100m3)";
	}
	for (size_t w = 0; w < OneLayerData::mod->nLife; w++)
	{
		fprintf_s(fOut, "Inhibitor lifetime(from start of treatment) above \n");

		if (OneLayerData::mod->TVlife[w] > 0)

			fprintf_s(fOut, "\t%11.4f ppm was %11.4f %s.\n\n", OneLayerData::mod->Clife[w], OneLayerData::mod->TVlife[w], st.c_str());

		else

			fprintf_s(fOut, "\tNever crosses \t%11.4f ppm \n\n", OneLayerData::mod->Clife[w]);
	}

	fclose(fOut);
}