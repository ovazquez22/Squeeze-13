#if !defined(AFX_ISOTHERMFITTINGDLG_H__72CA949C_E24C_4C68_B372_CA6FA4CE8E44__INCLUDED_)
#define AFX_ISOTHERMFITTINGDLG_H__72CA949C_E24C_4C68_B372_CA6FA4CE8E44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoThermFittingDlg.h : header file
//
#include "Optimization.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// IsoThermFittingDlg dialog

class IsoThermFittingDlg : public CDialog
{
// Construction
public:
	IsoThermFittingDlg(CWnd* pParent = NULL);   // standard constructor
	Model M;
	void UpdateControls();
	void UpdateEdits();

// Dialog Data
	//{{AFX_DATA(IsoThermFittingDlg)
	enum { IDD = IDD_DIALOG_ISOTHERM_FITTING };
	CEdit	m_Edit_K_Orig;
	CEdit	m_Edit_N_Orig;
	CEdit	m_Edit_R_Orig;
	CEdit	m_Edit_nMin;
	CEdit	m_Edit_nMax;
	CEdit	m_Edit_KMin;
	CEdit	m_Edit_KMax;
	CEdit	m_Edit_rMin;
	CEdit	m_Edit_rMax;
	CEdit	m_Edit_TrainPer;
	CEdit	m_Edit_Generations;
	CEdit	m_Edit_Population;
	//}}AFX_DATA
	Optimization Opt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(IsoThermFittingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(IsoThermFittingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOTHERMFITTINGDLG_H__72CA949C_E24C_4C68_B372_CA6FA4CE8E44__INCLUDED_)
