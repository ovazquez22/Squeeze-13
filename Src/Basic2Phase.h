#if !defined(AFX_BASIC2PHASE_H__0B10B84B_0E2D_4197_BAA9_7DDAAC2C4C40__INCLUDED_)
#define AFX_BASIC2PHASE_H__0B10B84B_0E2D_4197_BAA9_7DDAAC2C4C40__INCLUDED_

#include "Sheet.h"	// Added by ClassView
#include "PropPage1.h"
#include "PropPage2.h"
#include "PropPage3.h"
#include "PropPage4.h"
#include "PropPage5.h"
#include "PropPage7.h"
#include "testgrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Basic2Phase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Basic2Phase dialog

class Basic2Phase : public CDialog
{
// Construction
public:
	void LaySpacingGridDraw();
	void UpdateFromEdits();
	void UpdateControls();
	Basic2Phase(CWnd* pParent = NULL);   // standard constructor
	~Basic2Phase(); 
// Dialog Data
	//{{AFX_DATA(Basic2Phase)
	enum { IDD = IDD_BASIC_2PHASE };
	CEdit	m_Edit_FluidRho;
	CStatic	m_Static_FluidRho;
	CEdit	m_Edit_WellDia;
	CEdit	m_Edit_TubeRough;	
	CEdit m_Edit_FormTemp;
   CEdit m_Edit_AqHeCap;
   CEdit m_Edit_NAHeCap;
   CEdit m_Edit_FormThCond;
   CEdit m_Edit_ThCondRes;
   CEdit m_Edit_RoGrDen;
   CEdit m_Edit_AqPhDen;
   CEdit m_Edit_NAPhDen;
   CEdit m_Edit_RoHeCap;
	CStatic	m_Static_WellDia;
	CStatic	m_Static_TubeRough;
	CEdit	m_VolInj;
	CEdit	m_TotVol;
	CEdit	m_Rho;
	CEdit	m_InhCon;
	CEdit	m_CShut;
	CStatic m_Static_VolInj;
	CStatic m_Static_TotVol;
	CStatic m_Static_InhCon;
	CStatic m_Static_Rho;
	CStatic m_Static_CShut;	
	CButton m_Isotherm;
	CEdit	m_EditTitle1;
	CEdit	m_EditTitle2;
	CButton	m_Metric;
	CButton	m_Conventional;
	CButton	m_TwoPhase;
	CButton	m_SinglePhase;
	CEdit	m_Num_Comp;
	CEdit	m_Num_Stages;
	CEdit	m_Num_Layers;
	CEdit	m_Num_Blocks,
			m_API_Oil;
	int		m_Units;
	int		m_ModelType;
	int		m_Radio_Simulation;
	CButton	m_Check_WellFriction;
	CButton	m_Check_Temperature,
			m_Check_WatTemp,
			m_Check_OilTemp;
	testgrid LaySpacingGrid;
	CStatic m_Static_FormTemp;
	CStatic m_Static_FormThCond;
	CStatic m_Static_ThCondRes;
	CStatic m_Static_AqHeCap;
	CStatic m_Static_RoGrDen;
	CStatic m_Static_AqPhDen;
	CStatic m_Static_RoHeCap;
	
	//}}AFX_DATA
	
	int nB,
		nLay,
		nComp,
		nSt, // 0 = Metric, 1 = Conventional
		nWF, // 0 = No Well Friction, = 1 Well Friction
		nTemp, // 0 = No Temperature Calculations, = 1 Temperature Calculations
		watTemp,
		oilTemp;

	double CShut,
		    VolInj,
			 TotVol,
			 Rho,
			 InhCon,
			 WellDia,
			 FluidRho,
			 e,
			 Ftemp, // Formation temperature at the beginning of squeeze treatment (°C).
			 Kcr,   // Reservoir heat conductivity (conventional: Btu/ft-day-°C; metric: Btu/m-day-°C).
			 Kcs,   // Formation underburden / overburden heat conductivity (conventional: Btu/ft-day-°C;
					  // metric: Btu/m-day-°C).
			 SHr,   // Reservoir rock heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
		     SHw,   // Aqueous phase heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
			 RHOw,  // Aqueous phase density at formation conditions (conventional: lb/ft3; metric: kg/m3).
			 SHo,   // Aqueous phase heat capacity (conventional: Btu/lb-°C; metric: Btu/kg-°C).
			 RHOo,  // Aqueous phase density at formation conditions (conventional: lb/ft3; metric: kg/m3).
			 RHOr, 
			 API;	// Oil API

	vector<double> LaySpacing;

	CString EditTitle1,
			  EditTitle2;

	EditSingleGrid	m_Edit_IsoTable;

	bool unit,
		 check;
	
//	CTemperature T;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Basic2Phase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Basic2Phase)
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	afx_msg void OnRadial();
	afx_msg void OnLinear();
	afx_msg void OnIsotherm();
	afx_msg void OnIsothermDerivation();
	afx_msg void OnAnalitical();
	afx_msg void OnMetric();
	afx_msg void OnConventional();
	afx_msg void OnCheckWellfriction();
	afx_msg void OnKillfocusNumLayers();
	afx_msg void OnCheckTemperature();
	afx_msg void OnInputEffluentData();
	afx_msg void OnShowData();
	afx_msg void OnSinglePhase();
	afx_msg void OnTwoPhase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckWatVisc();
	afx_msg void OnBnClickedCheckOilVisc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASIC2PHASE_H__0B10B84B_0E2D_4197_BAA9_7DDAAC2C4C40__INCLUDED_)
