#if !defined(AFX_RETURNABLECOMBOBOX_H__8AAE8066_488D_40C0_9CBA_ABB3F809151C__INCLUDED_)
#define AFX_RETURNABLECOMBOBOX_H__8AAE8066_488D_40C0_9CBA_ABB3F809151C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReturnableComboBox.h : header file
//
#define UDM_BOX_LOST_FOCUS		1004
#define IDC_BOX_FOR_JCGRID    2226

/////////////////////////////////////////////////////////////////////////////
// ReturnableComboBox window

class ReturnableComboBox : public CComboBox
{
// Construction
public:
	ReturnableComboBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ReturnableComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~ReturnableComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(ReturnableComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RETURNABLECOMBOBOX_H__8AAE8066_488D_40C0_9CBA_ABB3F809151C__INCLUDED_)
