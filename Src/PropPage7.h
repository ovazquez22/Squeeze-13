#pragma once


// PropPage7 dialog
#include "testgrid.h"

class PropPage7 : public CPropertyPage
{
	DECLARE_DYNAMIC(PropPage7)

public:
	PropPage7();
	virtual ~PropPage7();
	virtual BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_PROPPAGE_LARGE7 };
	int nStg,
		nRows,
		nCols,
		height,
		width,
		Units, // = 0 Conventional, = 1 Metric
		kPlot;
	
	SqzOptimisation *P7SqzOpt;

	testgrid StageGrid;

	void InitialiseGrid();
	void UpdateFromEdits();
	void UpdateControls();

	CButton m_Check_Op_Cons,
			m_Check_Only_SI_Vol,
			m_Check_Only_TOT_Vol,
		    m_Check_Fixed_Target;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnSetActive();
	virtual BOOL OnKillActive();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedCheckOnlySiVol();
	afx_msg void OnBnClickedCheckOnlyTotVol();
	afx_msg void OnBnClickedFixedTarget();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheckFixedTarget();
	afx_msg void OnEnChangeEditnpopsize();
	afx_msg void OnEnChangeEditBinsmt();
	afx_msg void OnEnChangeEditIterationssecant();
	afx_msg void OnEnChangeEditnbins();
	afx_msg void OnStnClickedStaticOptimisation2();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEditngen();
	afx_msg void OnEnChangeEditSiVol();
	afx_msg void OnEnChangeEditInjhourrate();
	afx_msg void OnBnClickedCheckOperCons();
};
