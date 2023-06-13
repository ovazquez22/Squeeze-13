#if !defined(AFX_PROPPAGE2_H__A269B43F_6B95_406C_80F0_3748ED6E66AB__INCLUDED_)
#define AFX_PROPPAGE2_H__A269B43F_6B95_406C_80F0_3748ED6E66AB__INCLUDED_

#include "testgrid.h"	// Added by ClassView
#include "testgrid.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PropPage2 dialog

class PropPage2 : public CPropertyPage
{
	DECLARE_DYNCREATE(PropPage2)

// Construction
public:
	PropPage2(int, int);
	testgrid WaterGrid,
				OilGrid;

	PropPage2();
	
	~PropPage2();
	PropPage2& operator=(const PropPage2&);
	void Equate(const PropPage2&);

// Dialog Data
	//{{AFX_DATA(PropPage2)
	enum { IDD = IDD_PROPPAGE_LARGE2 };
	CButton m_All_Layers;
	//}}AFX_DATA
	
	testgrid	MassTransGrid;
	
	int nLay,
		 TempnLay,
		 nLayMem,
		 nComp,
		 IsoSpec,
		 iModel,
		 nDisabled, // = 0 Disabled, 1 Not Disabled;
		 kTemp;  // 0 = No Temperature, 1 = Temperature

	double MassRatio;

	vector<vector<Iso>> wIso,	// nLayers*nComponentsWater (l, cw)
						oIso;	// nLayers*nComponentsOil (l, co)
		
	vector<CompProp>	waterComp,
						oilComp;
	
	CString PathName;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(PropPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(PropPage2)
	virtual BOOL OnInitDialog(); 
	afx_msg void OnAllLayers();
	//}}AFX_MSG
	afx_msg LRESULT OnBoxLostFocus(WPARAM w, LPARAM l);
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	DECLARE_MESSAGE_MAP()
	
private:
	void UpdateControls();
	void UpdateFromEdits();
	void DeAllocate();
	void Allocate();
	void UpdateGrids();
	void SetHeaderRowWaterGrid(int r);
	void SetHeaderRowWaterGridSingle(int r);
	void SetHeaderRowOilGrid(int r);
	void SetHeaderRowMassTransGrid(int r);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPPAGE2_H__A269B43F_6B95_406C_80F0_3748ED6E66AB__INCLUDED_)
