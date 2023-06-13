#if !defined(AFX_AUTOBOXDLG_H__EC84856C_C763_4A6B_8524_2CEDECE6BE04__INCLUDED_)
#define AFX_AUTOBOXDLG_H__EC84856C_C763_4A6B_8524_2CEDECE6BE04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoBoxDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AutoBoxDlg dialog

class AutoBoxDlg : public CDialog
{
// Construction
public:
	AutoBoxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AutoBoxDlg)
	enum { IDD = IDD_ABOUTBOX };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AutoBoxDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AutoBoxDlg)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCancelMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSaveimage();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditnpopsize();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOBOXDLG_H__EC84856C_C763_4A6B_8524_2CEDECE6BE04__INCLUDED_)
