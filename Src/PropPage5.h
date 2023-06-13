#if !defined(AFX_PROPPAGE5_H__2FF45E86_6F6B_4F45_97D2_480DBA6CA220__INCLUDED_)
#define AFX_PROPPAGE5_H__2FF45E86_6F6B_4F45_97D2_480DBA6CA220__INCLUDED_

#include "EditSingleGrid.h"
#include "AnaliticalLinear.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage5.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropPage5 dialog

class PropPage5 : public CPropertyPage
{
	DECLARE_DYNCREATE(PropPage5)

// Construction
public:
	PropPage5();
	~PropPage5();
	void UpdateControls();
	void UpdateFromEdits();

// Dialog Data
	//{{AFX_DATA(PropPage5)
	enum { IDD = IDD_PROPPAGE_LARGE5 };
	int m_Kiso;
	int m_nSpace;
	
	CAnaliticalLinear AnLinModel;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropPage5)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

	EditSingleGrid	m_Edit_CLife[MaxClife],
						m_Edit_Phi,
					   m_Edit_CI,
						m_Edit_VolInj,
						m_Edit_VolOve,
						m_Edit_N,
						m_Edit_K,
						m_Edit_CSE,
						m_Edit_MinVol,
						m_Edit_MaxVol,
						m_Edit_NumRet,
						m_Edit_MaxIter,
						m_Edit_Tol;
	
// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropPage5)
	afx_msg void OnNK();
	afx_msg void OnCSE();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE5_H__2FF45E86_6F6B_4F45_97D2_480DBA6CA220__INCLUDED_)
