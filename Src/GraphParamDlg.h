#if !defined(AFX_GRAPHPARAMDLG_H__4686BB9E_274F_4B2A_838E_809651DBE835__INCLUDED_)
#define AFX_GRAPHPARAMDLG_H__4686BB9E_274F_4B2A_838E_809651DBE835__INCLUDED_

#include "TableGrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphParamDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GraphParamDlg dialog

class GraphParamDlg : public CDialog
{
// Construction
public:
	void UpdateFromEdits();
	void EnableGraph2();
	void EnableGraph3();
	void UpdateControls();
	GraphParamDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GraphParamDlg)
	enum { IDD = IDD_GRAPHPARAMDLG_DIALOG };
	CButton	m_Button_Rad_Inh;
	CButton	m_Button_Rad_Ads;
	CButton	m_Button_Rad_Temp;
	CEdit	m_Edit_MinConc;
	CEdit	m_Edit_MaxConc;
	CButton	m_Check_MinMaxConc;
	int		m_OptGraf1;
	CButton	m_Check_Graph1;
	CButton	m_Check_Graph2;
	CButton	m_Check_Graph3;
	CButton  m_check_extra;
	CButton  m_extra_out;
	CButton  m_extra_out2;
	//}}AFX_DATA
	
	int nLay,
		 kCrange,
		 kGraf1,
		 kGraf2,
		 kGraf3,
		 nGraf2[MaxLay],
		 nGraf3[MaxLay],
		 nDebug;

	double MaxConc,
			 MinConc;
	
	TableGrid Graph2Grid,
				 Graph3Grid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GraphParamDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GraphParamDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckGraph1();
	afx_msg void OnCheckMinmaxcon();
	afx_msg void OnCheckGraph2();
	afx_msg void OnCheckGraph3();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCheckExtraOutput();
	afx_msg void OnRadioExtraout();
	afx_msg void OnRadioExtraout2();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHPARAMDLG_H__4686BB9E_274F_4B2A_838E_809651DBE835__INCLUDED_)
