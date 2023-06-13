#if !defined(AFX_PROPPAGE3_H__C34E5CFF_09B1_47B0_AB46_D31BF84879AC__INCLUDED_)
#define AFX_PROPPAGE3_H__C34E5CFF_09B1_47B0_AB46_D31BF84879AC__INCLUDED_

#include "testgrid.h"
#include "StageDlg.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage3.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropPage3 dialog

class PropPage3 : public CPropertyPage
{
	DECLARE_DYNCREATE(PropPage3)

// Construction
public:
	void InitialiseGrid();
	void UpdateFromEdits();
	PropPage3();
	PropPage3(int, int, int);
	~PropPage3();
	PropPage3& operator=(const PropPage3&);

	int nStg,
		 nComp,
		 nLay,
		 Units,
		 iModel,
		 nDisabled, // = 0 Single Phase, = 1 Two Phase
		 NoTstep,
		 Header,		// Header = 0 no Headers, Header; = 1, Headers;
		 kTemp, // 0 = No Temperature, 1 = Temperature
		 m_TimeStep;
	//	 *compRole;

	StageDlg StgDlg;

	testgrid StageGrid;

	vector<Stg> Stage;

// Dialog Data
	//{{AFX_DATA(PropPage3)
	enum { IDD = IDD_PROPPAGE_LARGE3 };
	CButton	m_Headers;
	CEdit	m_Edit_NewStage_Pos;
	CButton	m_AutoTimeStep;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropPage3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateTimeVol();
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL OnSetActive();
	// Generated message map functions
	//{{AFX_MSG(PropPage3)
	afx_msg void OnUpdateRows();
	afx_msg void OnAutomaticTimeStep();
	virtual BOOL OnInitDialog();
	virtual BOOL OnKillActive();
	afx_msg void OnAddStage();
	afx_msg void OnRemoveStage();
	afx_msg void OnCheckHeaders();
	afx_msg void OnLowTimeStep();
	afx_msg void OnHighTimeStep();
	afx_msg void OnEnChangeStageGrid();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);

private:
	void SetHeaderRowStage(int, int);
	void UpdateControls();
	void DeAllocate();
	void Allocate();
	void Equate(const PropPage3& rhs);
	void UpdateST();
	
public:
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnStnClickedStaticStagegrid();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE3_H__C34E5CFF_09B1_47B0_AB46_D31BF84879AC__INCLUDED_)
