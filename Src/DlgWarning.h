#if !defined(AFX_DLGWARNING_H__4BC49231_0598_45C5_8A72_F901DFAC7172__INCLUDED_)
#define AFX_DLGWARNING_H__4BC49231_0598_45C5_8A72_F901DFAC7172__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgWarning.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DlgWarning dialog

class DlgWarning : public CDialog
{
// Construction
public:
	void WriteWarnings();
	DlgWarning(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DlgWarning)
	enum { IDD = IDD_WARNINGS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	bool bPhi,
		  bKabs,
		  bHLay,
		  bSrw,
		  bSro,
		  bSwi;
	
	int width,
		 height;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DlgWarning)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DlgWarning)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGWARNING_H__4BC49231_0598_45C5_8A72_F901DFAC7172__INCLUDED_)
