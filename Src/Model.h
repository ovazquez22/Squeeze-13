#ifndef Model_h
#define Model_h

#include <string>
#include <vector>
using namespace std;

//------------------------------------------------------------------------------------------
struct StaticLay 
{
	double	HLay,
			Phi,
			KAbs;

	int Type, // Corey = 0, Table Rel.Perm = 1
		nRP; // Number of entries in the Rel Perm Table

	double	A,
			Alpha,
			B,
			Beta,
			Swi, // Initial Water Saturation
			Srw, // Residual Water
			Sro, // Residual Oil
			NewSwi, // New Irrducible Water Saturation for Surfactant
			NewSor; // New Irrducible Oil Saturation for Surfactant

	vector<double>	RpSw,
					RpKrw,
					RpKro;

	CString RelPermTabFile;

	StaticLay() { KAbs = 1; };
};
//------------------------------------------------------------------------------------------
struct CompProp
{
	int Role,		// ( Tracer=0, Inhibitor=1, Viscosifier=2, Blocker=3, Surfactant=4, Ca=5, Na=6, Bridging=7 )
		MTrans,		// Mass Transfer ( NO = 0, EQU = 1 )
		Shear,		// 0 = No Shear, = 1 Yes Shear
		nLay,
		IsoTypAct,	// = 1 - Freundlich, = 2 - Langmuir,
		MAdso,		// MAdso (= 0  No Adsorption modification, = 1 Adsorption modification)
		IrrAdso,	// (= 0  Reversible Adsorption modification, = 1 Irreversible Adsorption modification)
		ModAdd,		// = 0, Maximum Additive Adsorption depends on the Initial Concentration
					// = 1, Maximum Additive Adsorption depends is splicitly input
		IsoTypeAds;

	double	MaxMU,		// Max. Viscosity
			nPowMU,		// Power of Viscosity function
			MaxC,		// It stores the Maximum Concentration 
			MaxCVis,	// It stores the Maximum Concentration for Viscosity calculations
			K,			// Mass Transfer Ratio between Oil and Water: Co = K * Cw
			Landa,		// Carreau Model
			n,			// Power Law
			Low_Ads1,
			Low_Ads2,
			High_Ads1,
			High_Ads2,
			Low_Conc,
			High_Conc,
			MaxAdso,	// Maximum Additive Adsorption 
			coreConc;	// Initial Concentration in the Core/Reservoir

	CompProp() { Role = 1; };
};
//------------------------------------------------------------------------------------------
struct Iso 
{
	int	IsoTyp,
	TypeAds,
	nIso;

	vector<double>	Ciso,
					ADiso;

	double	k,
		n,
		r2,
		Cps1,
		Cps2,
		Tpc,
		r4;

	CString IsoTabFile;

	Iso() { IsoTyp = 1; TypeAds = 1;  k = 501.1000; n = 0.4000; r2 = 0.1000; };
	~Iso();		// destuctor to free the memory
};

//------------------------------------------------------------------------------------------
struct OptStg
{
	int TypeStg,	// = 1 injection/preflush,
	// = 2 overflush,
	// = 3 back production,	
	// = 5 shut-in.

	objTime,		// = 0: no; = 1 yes, included in optimisation 
	objIndexTime,	// Index for lower and upper for objectives values
	objSI,			// = 0: no; = 1 yes, included in optimisation 
	objIndexSI;		// Index for lower and upper for objectives values

	double	InitialCw,	// per water components
			Qwt,
			Time,
			TimeStep,
			TI;

	OptStg(){ TypeStg = 1;  objTime = 0; objIndexTime = objIndexSI =- 1; objSI = 0; InitialCw = 100000; Qwt = 1; Time = 60; TI = 20; };
};
//------------------------------------------------------------------------------------------
struct Stg 
{
	int TypeStg,	// = 1 injection/preflush,
					// = 2 overflush,
					// = 3 back production,	
					// = 5 shut-in.
		nChage,		// Indicator of whether nOpen, nMethod, nLayOp and PIDf change in the new Stg:
					//	= 0: no
					// = 1: yes (this may be true even when nOpen & nMethod unchanged).
		nOpen,		// Number of open Lays at a specific Stg (nOpen < nLay; if all open, nOpen = 0).
		nMethod,	// Specification of which method is to be used in calculating the Lay flowrates:
					// = 0: calculating in terms of EffPerm,
					// = 1: calculating in terms of PIDf.
					// = 2: calculating in terms of EffPerm and KH product.
		BarPsi,

		objTime,		// = 0: no; = 1 yes, included in optimisation 
		objIndexTime,	// Index for lower and upper for objectives values
		objSI,			// = 0: no; = 1 yes, included in optimisation 
		objIndexSI,			// Index for lower and upper for objectives values
		objFlowRate,		// = 0: no; = 1 yes, included in optimisation 
		objIndexFlowRate;	// Index for lower and upper for objectives values

	double	Qwt,
			Qot,
			TimeStep, // Time Step
			Time,
			TI,
			watFrac,
			LinDisp;

	vector<int>		nLayOp;   // Indices of open Lays.;

	vector<double>	InitialCw,	// per water components
					InitialCo;

	vector<double>	PIDF,	// per Layer
					QwLay,
					LayPres,
					ProdFw;	

	Stg(){ objTime = 0;  objSI = 0; TimeStep = 0.01; nChage = 0;  nMethod = 0; };
	~Stg();
};
//------------------------------------------------------------------------------------------
struct SqzOptimisation
{
	double	MIC,			// MIC for squeeze lifetime 
			InjHourRate,	// Injection hourly rate
			SIBblRate,		// SI cost per barrel/m3			
			TotInjVol,		// Total Injected Volume Operational Constraints
			TotSIVol,		// Total Injected SI Volume Operational Constraints
			Obj_T_V;		// Target Squeeze Lifetime

	vector<string>	Label;

	vector<int> LabelNum;

	vector<double>	X,		// Original Time
		Vol,	// Original Volume
		uX,		// Upper limit of paramter
		lX,		// lower limit of parameter
		uObj,	// Upper limit of objective
		lObj,	// lower limit of objective
		Obj;	// lower limit of parameter

	int nPopSize,	// population size
		nGen,		// Number of generations
		nPar,		// Number of parameters
		nBins,		// Number of Bins in Sensitivity Calculations
		SensCalc,	// Conventional Sensitivity Calcualtions = 0, fixed volume of SI and Total Vol Inj = 1; = -1 No Sensitivity Calculations
		GrafMIC,	// MIC optimisation
		Only_SI,		// PSO dumping conditions considering only SI Inj Volume = 0, = 1 Not included 
		Only_Tot_Vol,	// PSO dumping conditions considering Tot Inj Volume = 0, = 1 Not included
		Fixed_Target,   // =0 for normal PSO Optimization and =1 for Secant Optimization
		BinsMT,            // Number of Bins for Secant Optimisation 
		IterationsSecant;  // Number of Iterations for Secant Optimisation

	vector<Stg> oStg;

	SqzOptimisation() {
		MIC = 10; Obj_T_V = 100; nPopSize = 20; nGen = 5; nPar = 1;  nBins = 5; Label.resize(nPar); X.resize(nPar); Vol.resize(nPar); uX.resize(nPar); lX.resize(nPar); InjHourRate = 8625;	SIBblRate = 430; TotInjVol = 1000; TotSIVol = 100; BinsMT = 10; IterationsSecant = 10;
	};
};
//------------------------------------------------------------------------------------------

struct Temp
{
	int IsoType,	// Isotherm Type at Hight Temperature
		watTemp, // 0 = No Temperature Water Viscosity dependence, = 1 Yes Temperature Water Viscosity dependence   
		oilTemp; // 0 = No Temperature Oil Viscosity dependence, = 1 Yes Temperature Oil Viscosity dependence   

	double	Ftemp, // Formation temperature at the beginning of squeeze treatment (°C).
			Kcr,   // Reservoir heat conductivity (conventional: Btu/ft-day-°C; metric: Btu/m-day-°C).
			Kcs,   // Formation underburden / overburden heat conductivity (conventional: Btu/ft-day-°C; metric: Btu/m-day-°C).

			SHr,   // Reservoir rock heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
			RHOr,  // Rock grain density (conventional: lb/ft3; metric: kg/m3).

			SHw,   // Aqueous phase heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
			RHOw,  // Aqueous phase density at formation conditions (conventional: lb/ft3; metric: kg/m3).

			SHo,   // Non-aqueous phase heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
			RHOo,  // Non-aqueous phase density at formation conditions (conventional: lb/ft3; metric: kg/m3).

			API,	// Oil API Gravity 
			Ads1,
			Ads2;

	Temp() {
		Ftemp = 111; Kcr = 45; Kcs = 45; SHr = 0.42; /* Btu/lbC */ SHw = 1.805; /* Btu/lbC */ SHo = 0.9181; /* Btu/lbC */ RHOw = 62.35; /* lb/ft3 */ RHOr = 165.3; /* lb/ft3 */ RHOo = 49.9; /* lb/ft3 */ IsoType = 1; Ads1 = 500; Ads2 = 0.3; watTemp = 1; oilTemp = 1; API = 21;
	};

};
//------------------------------------------------------------------------------------------
// this structure is used to read output from "conc.out"
struct OneConcOut
{
	int stage,
		TypeStage;
	double
	time,
	TotSI,
	Temp;
	vector <double> Rad;	// length = nLay
	vector <double> LayConc;
	vector <double> LayAP;
	vector <double> LayTemp;
	//LaySI,
	//WellSI,
	//Wprd;
};
//------------------------------------------------------------------------------------------
// this structure is used to save the total output for multilayered system
struct TotConcOut
{
	int stageNum,	// Index of timestep in the output file 
		TypeStage;	// Type of stage determined by index stageNum
	double 	time,
			TotSI,
			Twl;
	vector <double> LaySI;	// length = nLay
	vector <double> LayTemp;
	vector <double> WellSI;
	vector <double> Wprd;
};
//------------------------------------------------------------------------------------------
// this structure is used to save the output from old models
struct OutFile
{
	CString NameFile;
	vector <double> RT;	// length = nLay
	vector <double> RC;
};
//------------------------------------------------------------------------------------------
// this structure is used to store all output from single optismisation run;
struct Particle
{
	vector<double>	TimeVol,
					watConc;
	
	int nGrid,			// Number of the Grid, similar to Eclipse
		density;		// Stores the density in the Particle cuboid in the object space

	vector<double>	X,			// Stores the parameters of the Optimisation
					Objs,		// Stores the objectives function value
					CObjs,		// Stores the coordinates in the objective space
					Vol_Life,	// Volume of Water protected
					Tim_Life;	// Time of Squeeze Treatment

	double 	Obj,		// Stores the objective function value
			probability;// Stores the proability of the leader in the Archive to be selected

};
//------------------------------------------------------------------------------------------
// this structure is used to store the Objectives Grid space for leaders quality control;
struct Grid
{
	vector<int> depth;		// this is the number of recursive subdivisions of the objective space carried out in order to
	// divide the objective space into a grid for the purposes of diversity maintenance.Values of 
	// between 3 and 6 are useful, depending on number of objectives.	
	vector<double> delta;	// Stores the subdisivisions in the objective parameter space
};
//------------------------------------------------------------------------------------------
struct Model 
{
	string suff;	// suffix for creating the single layer files
	char UnitSpec;	// was "int" before - ILYA

	FILE	*fRes,
			*filePtrAll;

	CString ReadFileName,
			PathName,
			FieldTabFileName;

	bool forecast; // Set True for forecast, False for PSO

	int nSimTyp,	// = 0 Single Phase, = 1 Two Phase 
		nSim,		// = 0 Single Simulation, = 1 Optimisation, = 2 Isotherm Matching
		iModel,
		nBlocks,
		nLay,
		nStg,
		nComp,		// Number of Components
		kChem,
		kTemp,
		IsoSpec,
		nIters,
		nFreq,
		NoTstep,
		IsCon,
		IsTmp,
		nLife,
		kPlot,
		kCrange,
		kGraf1,
		kGraf2,
		kGraf3,
		nPts,
		iAnim,
		nOlds,
		nDebug,
		WellFric,			// 0 = No Well Friction, = 1 Well Friction
		nOpt,				// == 0 Isotherm Derivation, == 1 Design Optimisation; == -1 Run Single Model
		nComponentsWater;	// Number of water components

	unsigned long*	DataPos,
					nObjs,		// Number of objectives
					nPareto,	// It stores the number of particles in the Pareto Front
					nArc;		// It stores the number of leader particles in the archive
	
	vector<int> nGraf2,
				nGraf3;

	vector<double>	du,
					RT,
					RC,
					Time,
					RTemp,
					CumProd;		// SI Mass Production

	vector<CompProp> wComp,
					 oComp;

	vector<vector<double>>	Cd,		// Covariance Matrix
							iCd;	// Inverse of Covariance Matrix

	double	dr,
			radiusWell,
			radiusMax,
			CumInj,	// SI Mass Injection
			TotalInhMass,
			TotalProd_T_V, // Total Produced Time or Volume
			MUw,
			MUo,
			TolSQ5,
			GrafMIC,
			Cmin,
			Cmax,
			WellDia,
			FluidRho,
			e,
			Obj_T_V,
			TotInjVol,		// Total Injected Volume Operational Constraints
			TotSIVol,		// Total Injected SI Volume Operational Constraints;
			fMax,			// Maximum Likelihood 
			xL,				// Core Length
			Dcor,			// Core Diameter
			phi,			// Core Porosity
			rho,			// Core Density
			VI,				// Injected Volume
			VD,				// Dead Volume
			CI,				// Injected Concentration
			Cshut;			// Shut-in Concentration

	CString title1, title2;

	bool Max; // TRUE maximise, FALSE minimise

	vector<StaticLay> SLay;
						
	Iso Iso;
	vector<Stg> stg;
	Temp Temp;

	vector<int> StgType,				// Stores the Stage Type 
				StgNum;					// Stores the Stage Num 

	vector<double> fX;					// Stores field data
	vector<double> fY;					// Stores field data
	vector<double> IsoTV;
	vector<double> IsoC;
	vector<double> Clife;
	vector<double> TVlife;
	vector<double> Ciso;				// Stores Conc Isotherm derivation 
	vector<double> Giso;				// Stores Ads Isotherm derivation 
	vector<double> LaySpacing;

	vector<vector<vector<double>>>	C_L_Out,		// Concentration Profiles vs Radius and Layer	
									AD_L_Out,		// Adsoprtion Profiles vs Radius and Layer	
									SW_L_Out,		// Water Saturation vs Radius and Layer	
									FTemp_L_Out;	// Formation Temperature vs Radius and Layer

	vector<vector<double>>	RC_L_Out,	// Stores the Layer Concentration
							WC_L_Out,	// Stores the Well Concentration
							WPRD;		// Stores the Water Production Rate

	vector<string> single_layer_fn;			// file names (input) for the separate layers; this is defined in WriteSingleLayers
	vector<string> optimisation_model_fn;	// file names (input) for Optimisation Models; this is defined in WriteOptmisationModels
	vector<CString> FileNm;				// file names of old output files to be plotted

	SqzOptimisation SqzOpt;					// Stores all the input for an optimisation, including secant method and sensitivity studies

	Model(){
		nSim = 0;  nBlocks = 10;  nStg = 1; stg.resize(nStg); nComponentsWater = 1; stg[0].InitialCw.resize(nComponentsWater); nLay = 1;  SLay.resize(nLay);  nComponentsWater = 1;  wComp.resize(nComponentsWater); oComp.resize(nComponentsWater);  nOpt = 1; Temp.Ftemp = 0; nObjs = 3; nPareto = 0; nArc = 0; SqzOpt.nPopSize = 20; SqzOpt.nGen = 5; SqzOpt.nPar = 1; SqzOpt.SensCalc = 1;  SqzOpt.Only_SI = 1;  SqzOpt.Only_Tot_Vol = 1; TotInjVol = 1000;  TotSIVol = 10; SqzOpt.Fixed_Target = 1;  SqzOpt.BinsMT = 10; SqzOpt.IterationsSecant = 10; forecast = false; Max = FALSE; WellFric = 0; WellDia = 0.1; FluidRho = 63; e = 0.00015; rho = 2.65; VI = 10; VD = 10; CI = 10; Cshut = 10; IsoSpec = 0;
	};
	~Model();
	void ConvertUnits();
	void DeConvertUnits();
	double RunSQV_SP(Particle &);
	void LinearCore_SP();
	void ReadFileSQZ5Linear(const char*);
	void ReadFileSQZ5Isotherm(const char* NameFile);
	void IsothermDerivation(const char* NameFile);
	double fP(double C, double Cold, double Cupstr, double G, double Gold, double Qr, double Vw, double Vr, double dT);
	double fdPdG(double Vr);
	double fdPdC(double Qr, double Vw, double dT);
	double fQ(double C, double Cold, double G, double Gold, double dT);
	double Geq(double Cmo);
	double dGeqdC(double Cmo);
	double fdQdG(double dT);
	double fdQdC(double C);
	double R(int Iblock, double dR);
	void ReadFileSQZ5Radial(const char *fname);		// function for reading from file "fname"
	void PrintReturn(vector<double> RT, vector<double> RC, CString, double, Particle);
	void ReadFileSQZ5Optimisation(const char *fname);
	void ReadFileSQZ5Optimisation_Secant(const char *fname);
	void WriteFileSQZ5Radial(const char *fname);
	void WriteSingleLayers(const char *fname);			// function for writing separate layers to file "fname"
	void WriteOptimisationModels(const char *fname, vector<Particle*> d, int);	// function for writing Optimisation models to file "fname"
	void WriteIsothermOptimisationModel(const char *NameFile); // Subroutine to write Isotherm Matching in Optimisation Format, $P...
	void WriteOptimisationFileModels(const char *NameFile, vector<Particle> &p);
	void WriteOptimisationFileModels_Secant(const char *NameFile, vector<Particle> &p, int nP);
	unsigned long * GetPositionsFile(FILE* pFile, const char* lpszSearchString, unsigned long ulCurrentPosition, unsigned long &n);
	unsigned long FileSearch(FILE*, const char*, unsigned long);
	vector<vector<double>> cholesky(vector<vector<double>>, int n); // Calculates Choleski Decomposition of a matrix
	void show_matrix(vector<double> A, int n);
	vector<vector<double>>  mult(vector<vector<double>>, vector<vector<double>>, int, int, int); // Multiply two matrices
	void PrepLikelihood();
	void Prep_du(); // Generate Random error observations
	double Likelihood();
	void calculateInverse(vector< vector<double> >&);
	void print(vector< vector<double> >);
	void Print_Output(CString);
	double luDecomposition(vector<vector<double>>, int);
	vector<vector<double>> inverse(vector< vector<double> >);
	double randval(double, double);
	void Check_NR(double Cold, double Gold, double Cupstr, double dt, double Qr, double Vw, double Vr);
};
//------------------------------------------------------------------------------------------
// this structure is used to store all output from a single layer;
// array of <OneLayerData> will be used in the end to create the total output
struct OneLayerData
{
	vector<OneConcOut> conc;	// conc[t] corresponds to time step "t"; conc stores concentrations for the given layer
	static const Model *mod;	// multi-layer model (from input data) - for stage & layer flow rates

	int nLay,
		nSim;		// = 0 Single Simulation, = 1 Optimisation, = 2 Isotherm Matching

	double	GrafMIC,
			Obj;

	void ReadOutput(const char *fname, const char *fname1, int l);	// read "conc.out" file to "conc" vector
	
	//void ReadMod(const char *fname);	// read the input single layer model

	static vector<TotConcOut> WriteTotalConc(const char *fname, const vector<OneLayerData*> &OL);	
								// write total concentration from the array of single layers OL
								// to the file "fname" and return the same result as
								// vector<TotConcOut>[t], where "t" are time steps;
								// if fname == 0, then output to the file is skipped, but the
								// vector is still properly returned
};

//------------------------------------------------------------------------------------------
// this structure is used to store all output from a single run;
struct OneRun
{	
	vector<OneConcOut> conc;
	Model mod;
	Particle P;
};
//------------------------------------------------------------------------------------------
class Exception : public std::exception
{
protected:
	string msg;
public:
	Exception() : msg(""){};
	Exception(string s) : msg(s){};
	~Exception() {};
	const char *what() const { return msg.c_str(); };
};
//------------------------------------------------------------------------------------------

#endif