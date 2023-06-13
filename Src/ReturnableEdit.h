#if !defined(AFX_RETURNABLEEDIT_H__671C44F6_B50F_4D1B_AD02_3120DE2B9E42__INCLUDED_)
#define AFX_RETURNABLEEDIT_H__671C44F6_B50F_4D1B_AD02_3120DE2B9E42__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReturnableEdit.h : header file
//

#define UDM_EDIT_LOST_FOCUS		1002
#define IDC_EDIT_FOR_JCGRID		2224


/////////////////////////////////////////////////////////////////////////////
// ReturnableEdit window

class ReturnableEdit : public CEdit
{
// Construction
public:
	ReturnableEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReturnableEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ReturnableEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(ReturnableEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RETURNABLEEDIT_H__671C44F6_B50F_4D1B_AD02_3120DE2B9E42__INCLUDED_)
