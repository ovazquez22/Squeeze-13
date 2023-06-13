#if !defined(AFX_SHEET_H__D04F8C8F_711d_4A7B_B143_9073FA6BD1A4__INCLUDED_)
#define AFX_SHEET_H__D04F8C8F_711d_4A7B_B143_9073FA6BD1A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Sheet.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CSheet

class CSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet)

// Construction
public:
	CSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
// Attributes
public:


// Operations
public:
	static vector<Stg> ST;
	static int kPlot;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEET_H__D04F8C8F_711d_4A7B_B143_9073FA6BD1A4__INCLUDED_)
