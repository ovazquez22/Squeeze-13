#if !defined(AFX_MUTSOLVDLG_H__A88C4219_0E00_4D9F_BCA3_1680B1672F89__INCLUDED_)
#define AFX_MUTSOLVDLG_H__A88C4219_0E00_4D9F_BCA3_1680B1672F89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MutSolvDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MutSolvDlg dialog

class MutSolvDlg : public CDialog
{
// Construction
public:
	MutSolvDlg(CWnd* pParent = NULL);   // standard constructor
	
	int MAdso,  // Adsorption Alteration (=0 No, =1 Yes) 
		 Irr,		// Irreversible Adsorption Alteration (=0 No, =1 Yes) 
		 ModAdd,	// = 0 Max Additive Adsortion depends on injected concentration
					// = 1 It is a fixed number
		 IsoTyp; // = 1 - Freundlich,
					// = 2 - Langmuir,
					// = 3 - Table,
	
	bool MS_BA; // FALSE for Bridging Agent, TRUE for Mutual Solvent 

	double Ads1,
			 Ads2,
			 NewSwi,
			 NewSor,
			 MaxAdso;
	
	CString PathName,
			  TableName;

// Dialog Data
	//{{AFX_DATA(MutSolvDlg)
	enum { IDD = IDD_MUTSOLV_DIALOG };
	CButton	m_Check_ModAdd;
	CEdit	m_Edit_MaxAdso;
	CEdit	m_Edit_New_Sro;
	CEdit	m_Edit_New_Swi;
	CEdit m_Ads1;
	CEdit m_Ads2;
	CEdit m_Table;
	CButton m_Check_MAdso;
	CButton m_Check_Irr;
	CButton m_Freundlich;
	CButton m_Langmuir;
	CButton m_Tab;
	int	m_Radio_IsoType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MutSolvDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MutSolvDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckMAdso();
	afx_msg void OnRadioFreundlich();
	afx_msg void OnRadioLangmuir();
	afx_msg void OnRadioTable();
	afx_msg void OnCheckIrr();
	afx_msg void OnCheckModadd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateEdits();
	void UpdateControls();
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MUTSOLVDLG_H__A88C4219_0E00_4D9F_BCA3_1680B1672F89__INCLUDED_)
