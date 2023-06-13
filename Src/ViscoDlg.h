#if !defined(AFX_VISCODLG_H__48317B9E_7584_4FE6_AEFE_8859939CF90D__INCLUDED_)
#define AFX_VISCODLG_H__48317B9E_7584_4FE6_AEFE_8859939CF90D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViscoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ViscoDlg dialog

class ViscoDlg : public CDialog
{
// Construction
public:
	ViscoDlg(CWnd* pParent = NULL);   // standard constructor
	void Allocate( int );
	void DeAllocate();
	void UpdateFromEdits();

// Dialog Data
	//{{AFX_DATA(ViscoDlg)
	enum { IDD = IDD_VISCODLG_DIALOG };
	CEdit	m_Edit_Wat_nv;
	CEdit	m_Edit_Wat_MaxMu;
	CEdit	m_Edit_Wat_Landa;
	CEdit	m_Edit_Wat_n;
	CButton m_Check_Wat_Shear;
	CEdit	m_Edit_Oil_nv;
	CEdit	m_Edit_Oil_MaxMu;
	CEdit	m_Edit_Oil_Landa;
	CEdit	m_Edit_Oil_n;
	CButton m_Check_Oil_Shear;
	//}}AFX_DATA
	
	int nComp, // Chosen component
		kTemp; // Temperature Calculations

	CompProp waterComp,
						oilComp;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViscoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	void UpdateControls();

protected:

	// Generated message map functions
	//{{AFX_MSG(ViscoDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnWaterShearThinning();
	afx_msg void OnOilShearThinning();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VISCODLG_H__48317B9E_7584_4FE6_AEFE_8859939CF90D__INCLUDED_)
