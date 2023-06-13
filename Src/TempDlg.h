#if !defined(AFX_TEMPDLG_H__947F6DF3_7D5A_4913_A386_07594DD5855D__INCLUDED_)
#define AFX_TEMPDLG_H__947F6DF3_7D5A_4913_A386_07594DD5855D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TempDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TempDlg dialog

class TempDlg : public CDialog
{
// Construction
public:
	TempDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(TempDlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TempDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(TempDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEMPDLG_H__947F6DF3_7D5A_4913_A386_07594DD5855D__INCLUDED_)
