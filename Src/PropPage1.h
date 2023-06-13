#if !defined(AFX_PROPPAGE1_H__77733E8C_0876_4C6F_863C_CC9216BB49C7__INCLUDED_)
#define AFX_PROPPAGE1_H__77733E8C_0876_4C6F_863C_CC9216BB49C7__INCLUDED_

#include "testgrid.h"
#include "EditSingleGrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropPage1 dialog

class PropPage1 : public CPropertyPage
{
	DECLARE_DYNCREATE(PropPage1)

// Construction
public:
	PropPage1();
	PropPage1(int);
	~PropPage1();

	void Allocate();
	void DeAllocate();

	PropPage1& operator=(const PropPage1&);
	void Equate(const PropPage1&);

	void UpdateFromEdits();

// Dialog Data
	//{{AFX_DATA(PropPage1)
	enum { IDD = IDD_PROPPAGE_LARGE1 };
	CButton m_All_Layers;
	//}}AFX_DATA
	
	int nLay,
		 Units,
		 iModel,
		 nSimTyp, // = 0 Single Phase, = 1 Two Phase 
		 nDisabled; // = 0 Disabled, 1 Not Disabled
		
	testgrid RockGrid,
				FluidGrid;
	
	EditSingleGrid m_Well_Rad,
						m_Max_Rad,
						m_Mu_Wat,
						m_Mu_Oil;

	vector<StaticLay> SLay;
	
	double WellRad,
			 MaxRad,
			 MUw,
			 MUo;
	
	CString PathName;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
	afx_msg void OnAllLayers();

	DECLARE_MESSAGE_MAP()
private:
	void UpdateControls();
	void UpdateFromListCtrl();
	void SetHeaderRowFluidGrid(int r);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE1_H__77733E8C_0876_4C6F_863C_CC9216BB49C7__INCLUDED_)
