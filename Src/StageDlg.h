#if !defined(AFX_STAGEDLG_H__CC8C3C8B_890E_4B34_B66C_2F7EDB191415__INCLUDED_)
#define AFX_STAGEDLG_H__CC8C3C8B_890E_4B34_B66C_2F7EDB191415__INCLUDED_

#include "TableGrid.h"	// Added by ClassView
#include "Schedule.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StageDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StageDlg dialog

class StageDlg : public CDialog
{
// Construction
public:
	void UpdateFromEdits();
	void UpdateControls();
	void DeAllocate();
	void Allocate(int, int, int);//, int *);
	StageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StageDlg)
	enum { IDD = IDD_STAGE_DLG };
	CButton	m_AllPres;
	CButton	m_AllLayOpen;
	CButton	m_AllComp;
	int		m_Check_KH;
	int		m_Check_Bar;
	CButton	m_KH;
	CButton	m_PIDF;
	CButton	m_LayPres;
	CButton	m_Bar;
	CButton	m_Psi;
	//}}AFX_DATA
	int nSt,
		 nStg,
		 nComp,
		 nLay,
		 nDisabled, // = 0 Disabled, 1 Not Disabled
		 nWF, // 0 = No Well Friction, = 1 Well Friction
		 checkOpenLayers,
		 checkPLsame;

	bool Enable;
		  
	TableGrid LayGrid,
				 CompGrid;
	
	vector<int> compRole;

	vector<Stg> Stage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(StageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckLayOpen();
	afx_msg void OnCheckAllComp();
	afx_msg void OnKh();
	afx_msg void OnLaypres();
	afx_msg void OnPidf();
	afx_msg void OnCheckPress();
	//}}AFX_MSG
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STAGEDLG_H__CC8C3C8B_890E_4B34_B66C_2F7EDB191415__INCLUDED_)
