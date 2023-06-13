#if !defined(AFX_BASICDLG_H__03D48E1C_7B33_4525_965F_68EDF73C752A__INCLUDED_)
#define AFX_BASICDLG_H__03D48E1C_7B33_4525_965F_68EDF73C752A__INCLUDED_

#include "Basic2Phase.h"	// Added by ClassView
#include "GraphicsBlock.h"
#include "Graphics.h"
#include "TempDlg.h"
#include "AutoBoxDlg.h"
#include "OptDlg.h"
#include "FileNotOpenDlg.h"
#include "DlgWarning.h"	
#include "IsoThermFittingDlg.h"
#include "Optimization.h"
#include "CheckListBoxGraphs.h"
#include "IsothermDerivationGraph.h"
#include "Sim_Running_Dlg.h"
#include "PSOParallel.h"
#include "Opt_Output.h"
#include "Model.h"

struct layer
{	
	double *TimeVol;
	double **Conc,
		   **AdsPPt,
		   **Temp,
		   **FTemp;
};

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BasicDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BasicDlg dialog

class BasicDlg : public CDialog
{
// Construction
public:
	
	Optimization Opt;
	PSOParallel Par,
				IsoPar;

	SqzOptimisation SqzOpt;

	vector<data_sqz> opt_data; // Stores the output from the optimisation run

	void HideGraph();
	void ShowGraph();
	void RelativePIDf(int);
	BasicDlg(CWnd* pParent = NULL);   // standard constructor
	void CreateDlg(CWnd* pParent);
	void UpdateRun(char *);
	void UpdateSingleGraphTime(int);
	int CheckLicense(int &);
	void Par_PSO_Opt();
	Particle RunPSO(Model&, int);
	Particle RunSecantOpt();
	double randval(double, double);
	void Parallel_Runs(vector<Particle> &, Model &, int);
	
	~BasicDlg();

	PropPage1 Page1;

	Basic2Phase Basic2PhaseDlg;

	Graphics *GraphTime,
				*GraphRad;

	GraphicsBlock GraphCross;
	
	AutoBoxDlg ABDlg;
	OptDlg ODlg;
	IsoThermFittingDlg IFDlg;
	CheckListBoxGraphs IsoGraphs;
	IsothermDerivationGraph IsoDerGrp;
	Opt_Output Dlg_PSO_Output;

//	DlgWarning *DlgWar;
	CRect DWRect;

	FileNotOpenDlg FNODlg;

	ProgressBar ProBar;

	int nGraphTime,
		 nGraphRad,
		 MNTcount;
	
	CString PathName,
			PathNameReadFile,
			TitleNameReadFile;
	
	long userTimeDelay;
	
	double	MaxAds,
			MaxConc,
			MaxAdsPPt; 
	
	Model Mod;

	int nL,
		nB,
		nSP_Pts,
		nF;

	FILE *fArc, *fGA, *fGARes, *fTest, *fRes;

	bool fail,
		ParRun; // To Update Graphs with Parallel run, not with output files

	void ChartInitialisation();

// Dialog Data
	//{{AFX_DATA(BasicDlg)
	enum { IDD = IDD_BASIC_DLG };
	CButton	m_Button_Graphs;
	CButton	m_Button_Warning;
	CButton	m_Button_MOD;
	CButton	m_Button_Close;
	CButton	m_Button_Graph_Cross;
	CButton	m_Button_Graph_Rad;
	CButton	m_Button_Graph_Time;
	CEdit	m_EditTitle2;
	CEdit	m_EditTitle1;
	CStatic	m_Static_AddedTime;
	CSliderCtrl	m_Slider_TimeDelay;
	CSliderCtrl	m_Slider_SP_TS;
	CButton	m_bAbortRun;
	CButton	m_bEditModel;
	CButton	m_bBasicParam;
	CButton	m_bRunModel;
	CButton	m_bReadFile;
	CEdit	m_editMod;
	CStatic	m_Static_Sim_Type;
	//}}AFX_DATA
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BasicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(BasicDlg)
	afx_msg void OnReadFile();
	afx_msg void OnEditModel();
	afx_msg void OnRunModel();
	afx_msg void OnGraphTime();
	afx_msg void OnGraphRad();
	afx_msg void OnGraphCross();
	virtual BOOL OnInitDialog();
	afx_msg void OnMod();
	afx_msg void OnBasicPara();
	afx_msg void OnAbortRun();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonPlay();
	afx_msg void OnHelp();
	afx_msg void OnButtonAbout();
	afx_msg void OnCancelMode();;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnEnChangeSlider_SP_TS();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	bool quitRun,
		 STOP,
		 GenBool,
		 check;

	Sim_Running_Dlg SRD;

	void UpdateGraphCross(CString);
	void UpdateGraphTime(int, CString);
	void FirstChartInit();
	void UpdateGraphRad(CString);
	void DeAllocate();
	void Allocate();
	void ReadOutputFile(int);
	double ** d2DArray(int, int);
	void freed2DArray(double **, int);
	double *** d3DArray(int, int, int);
	void freed3DArray(double ***, int, int);
	int ProcessMessageLoop();
	static int role;

	double yMinGT;

public:
	afx_msg void OnBnClickedButtonPhreeqc();
	afx_msg void OnBnClickedCheckGraphs();
	afx_msg void OnBnClickedButtonBack();
	afx_msg void OnBnClickedButtonFoward();
	afx_msg void OnButtonParOpt();
	afx_msg void OnBnClickedButtonIsoTest();
	afx_msg void OnBnClickedButtonMopso();
	afx_msg void OnBnClickedButtonParOpt2();
	afx_msg void OnBnClickedSensitivityCalculations();
	afx_msg void OnNMCustomdrawSliderSpTs(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnStnClickedStaticSimType();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonSecOpt();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASICDLG_H__03D48E1C_7B33_4525_965F_68EDF73C752A__INCLUDED_)
