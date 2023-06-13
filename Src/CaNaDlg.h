#if !defined(AFX_CANADLG_H__1C659826_48BB_4D10_9A3E_2C2A45CF19D4__INCLUDED_)
#define AFX_CANADLG_H__1C659826_48BB_4D10_9A3E_2C2A45CF19D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CaNaDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CaNaDlg dialog

class CaNaDlg : public CDialog
{
// Construction
public:
	CaNaDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CaNaDlg)
	enum { IDD = IDD_CANA_DIALOG };
	CEdit m_Low_Ads1;
	CEdit m_Low_Ads2;
	CEdit m_High_Ads1;
	CEdit	m_High_Ads2;
	CEdit	m_Ca_coreConc;
	CEdit	m_Na_coreConc;
	CEdit	m_High_Na;
	CEdit	m_Low_Na;
	CButton m_Freundlich;
	CButton m_Langmuir;
	int	m_Radio_IsoType;
	//}}AFX_DATA

	int IsoTyp; // = 1 - Freundlich,
					// = 2 - Langmuir,
					// = 3 - a table of isotherm data,

	double Low_Ads1,
			 Low_Ads2,
			 High_Ads1,
			 High_Ads2,
			 CacoreConc,
			 NacoreConc,
			 Low_Na,
			 High_Na;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CaNaDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CaNaDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioFreundlich();
	afx_msg void OnRadioLangmuir();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateEdits();
	void UpdateControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CANADLG_H__1C659826_48BB_4D10_9A3E_2C2A45CF19D4__INCLUDED_)
