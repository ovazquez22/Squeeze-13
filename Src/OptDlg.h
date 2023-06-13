#if !defined(AFX_OPTDLG_H__0B634FE6_A262_4F23_B2C3_502887A50744__INCLUDED_)
#define AFX_OPTDLG_H__0B634FE6_A262_4F23_B2C3_502887A50744__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OptDlg dialog

class OptDlg : public CDialog
{
// Construction
public:
	OptDlg(CWnd* pParent = NULL);   // standard constructor
	CString snfile;
// Dialog Data
	//{{AFX_DATA(OptDlg)
	enum { IDD = IDD_OPT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(OptDlg)
	afx_msg void OnButtonOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTDLG_H__0B634FE6_A262_4F23_B2C3_502887A50744__INCLUDED_)
