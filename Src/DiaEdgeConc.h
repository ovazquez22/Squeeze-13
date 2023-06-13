#if !defined(AFX_DIAEDGECONC_H__8B87BA65_3B86_49B2_A81D_27E58E6EF535__INCLUDED_)
#define AFX_DIAEDGECONC_H__8B87BA65_3B86_49B2_A81D_27E58E6EF535__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DiaEdgeConc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DiaEdgeConc dialog

class DiaEdgeConc : public CDialog
{
// Construction
public:
	DiaEdgeConc(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DiaEdgeConc)
	enum { IDD = IDD_DIALOG_EDGE_CONC };
	CStatic	m_Static_EdgeConc;
	//}}AFX_DATA
	
	int lay,
		 comp,
		 wo;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DiaEdgeConc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DiaEdgeConc)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIAEDGECONC_H__8B87BA65_3B86_49B2_A81D_27E58E6EF535__INCLUDED_)
