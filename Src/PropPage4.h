#if !defined(AFX_PROPPAGE4_H__B4768288_F1FC_4FE9_8AEA_B8FD73D03075__INCLUDED_)
#define AFX_PROPPAGE4_H__B4768288_F1FC_4FE9_8AEA_B8FD73D03075__INCLUDED_

#include "OutputTimeDlg.h"
#include "GraphParamDlg.h"	// Added by ClassView
#include "TableGrid.h"
#include "EditSingleGrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage4.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropPage4 dialog

class PropPage4 : public CPropertyPage
{
	DECLARE_DYNCREATE(PropPage4)

// Construction
public:
	void UpdateControls();
	void IntialiseOldFileGrid();
	void UpdateFromEdits();
	GraphParamDlg GPDlg;
	OutputTimeDlg OTDlg;
	PropPage4();
	~PropPage4();
	
	int iAnim, 
		 kPlot,
		 kTemp,
		 kCrange,
		 kGraf1,
		 kGraf2,
		 kGraf3,
		 nGraf2[MaxLay],
		 nGraf3[MaxLay],
		 nIters,
		 nFreq,
		 nLife,
		 nOlds,
		 nLay,
		 iModel,
		 nDisabled,
		 bCw, // 0 = Deactivated, 1 = Activated 
       bCo, // 0 = Deactivated, 1 = Activated 
		 bAdsW, // 0 = Deactivated, 1 = Activated 
		 bAdsO, // 0 = Deactivated, 1 = Activated 
		 bViscW, // 0 = Deactivated, 1 = Activated
		 bViscO, // 0 = Deactivated, 1 = Activated
		 bRFW, // 0 = Deactivated, 1 = Activated
		 bRFO, // 0 = Deactivated, 1 = Activated
		 bOutputFile, // 0 = Deactivated, 1 = Activated
		 bSW, // 0 = Deactivated, 1 = Activated
		 bFW, // 0 = Deactivated, 1 = Activated
       Units, // 0 = Metric, 1 = Conventional

		 bOutputFileTime; // 0 = Deactivated, 1 = Activated 
		 
//		 bPage4; // 0 = Deactivated, 1 = Activated
			 
	double TolItr,
			 GrafMIC;
	
	vector<double> Clife;

	char FileNm[MaxFileNm][128];

	TableGrid OldFileGrid;

// Dialog Data
	//{{AFX_DATA(PropPage4)
	enum { IDD = IDD_PROPPAGE_LARGE4 };
	int		m_Return_Curve_Type;
	BOOL   	m_Bool_Screen_Display;
	BOOL		m_Bool_IsCon;
	BOOL		m_Bool_IsTmp;
	CButton  m_Check_Cw;
	CButton  m_Check_Co;
	CButton  m_Check_AdsW;
	CButton  m_Check_AdsO;
	CButton  m_Check_ViscW;
	CButton  m_Check_ViscO;
	CButton  m_Check_RFW;
	CButton  m_Check_RFO;
	CButton  m_Check_OutputFile;
	CButton  m_Check_OutputFileTime;
	CButton  m_Check_SW;
	CButton  m_Check_FW;
	CButton m_bGraphPara;
	CButton m_bShowTable;
	CButton m_bInputTable;
	CButton m_bRemoveFileNum;
	CStatic m_Static_RetConc;
	CStatic m_Static_MaxIter;
	CStatic m_Static_MaxTim;
	CStatic m_Static_Tol;
	CStatic m_Static_GrPa;
	CButton m_Button_ScrDis;
	CButton m_Button_TimLin;
	CButton m_Button_TimLog;
	CButton m_Button_WatLin;
	CButton m_Button_WatLog;
	CButton m_Static_RetPlot;
	CEdit	m_GrafMic;
	CButton m_Button_Time;
	CButton m_Button_WatPro;
	//}}AFX_DATA

	EditSingleGrid	m_Edit_Num_Freq,
						m_Edit_TolItr,
						m_Edit_CLife[MaxClife],
						m_Edit_Num_Iters,
						m_Edit_FieldTable,
						m_Edit_FileNum,
						m_Edit_GrafMic;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PropPage4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PropPage4)
	afx_msg void OnButtonGraphPara();
	afx_msg void OnButtonShowTable();
	afx_msg void OnButtonInputTable();
	afx_msg void OnButtonRemoveTable();
	afx_msg void OnButtonRemoveFileNum();
	afx_msg void OnButtonOutputFileDistance();
	afx_msg void OnButtonOutputFileTime();
	afx_msg void OnKillfocusOutputFile();
	afx_msg void OnKillfocusOldFiles();
	virtual BOOL OnKillActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnButtonCw();
	afx_msg void OnButtonCo();
	afx_msg void OnButtonAdsW();
	afx_msg void OnButtonAdsO();
	afx_msg void OnButtonViscW();
	afx_msg void OnButtonViscO();
	afx_msg void OnButtonRFW();
	afx_msg void OnButtonRFO();
	afx_msg void OnButtonSw();
	afx_msg void OnButtonSo();
	afx_msg void OnTime_Lin();
	afx_msg void OnTime_Log();
	afx_msg void OnWatVol_Lin();
	afx_msg void OnWatVol_Log();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
public:
	afx_msg void OnBnClickedRcWatpro();
	afx_msg void OnBnClickedRcTime();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE4_H__B4768288_F1FC_4FE9_8AEA_B8FD73D03075__INCLUDED_)
