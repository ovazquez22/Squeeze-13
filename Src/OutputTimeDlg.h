#if !defined(AFX_OUTPUTTIMEDLG_H__1D3950DD_E8B1_4C2F_BA37_A9EE7BE869DE__INCLUDED_)
#define AFX_OUTPUTTIMEDLG_H__1D3950DD_E8B1_4C2F_BA37_A9EE7BE869DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutputTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OutputTimeDlg dialog

class OutputTimeDlg : public CDialog
{
// Construction
public:
	OutputTimeDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OutputTimeDlg)
	enum { IDD = IDD_DLG_OUTPUT_TIME };
	CButton m_check_WatCut;
	CButton m_check_Wat_Tot_Flow;
	CButton m_check_Wat_Mass;
	CButton m_check_Wat_Flow_Lay;
	CButton m_check_Wat_Conc;
	CButton m_check_Wat_Cum;
	CButton m_check_Oil_Tot_Flow;
	CButton m_check_Oil_Mass;
	CButton m_check_Oil_Conc;
	CButton m_check_Oil_Cum;
	CButton m_check_Wat_Pen;
	CButton m_check_Oil_Pen;
	CButton m_check_Tot_Mass;

	//}}AFX_DATA

	int bRad, // 0 = Deactivated, 1 = Activated
		 bWC, // 0 = Deactivated, 1 = Activated 
       bTQw, // 0 = Deactivated, 1 = Activated 
		 bTQo, // 0 = Deactivated, 1 = Activated 
		 bTCw, // 0 = Deactivated, 1 = Activated 
		 bTCo, // 0 = Deactivated, 1 = Activated
		 bSIw, // 0 = Deactivated, 1 = Activated
		 bSIo, // 0 = Deactivated, 1 = Activated
		 bLQw, // 0 = Deactivated, 1 = Activated
		 bLQo, // 0 = Deactivated, 1 = Activated
		 bCPw, // 0 = Deactivated, 1 = Activated
		 bCPo, // 0 = Deactivated, 1 = Activated
		 bPw, // 0 = Deactivated, 1 = Activated
		 bPo, // 0 = Deactivated, 1 = Activated
		 bMass; // 0 = Deactivated, 1 = Activated

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OutputTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OutputTimeDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OUTPUTTIMEDLG_H__1D3950DD_E8B1_4C2F_BA37_A9EE7BE869DE__INCLUDED_)
