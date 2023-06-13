#pragma once
#

class Parallel
{
public:
	Parallel(string, string);
	~Parallel();

	char UnitSpec;

	int nLayer,
		nBlocks,
		iModel,
		kPlot;

	vector <double> Rad,
					conc,
					ap,
					temp,			
					LayHeight,
					fX,
					fY;

	vector<string> oldModels;

	vector<TotConcOut> TOTAL;

	void RunSim(const char *NameFile);
	void PrintOutFile(const char *fname, vector<TotConcOut> TOTAL);
};

