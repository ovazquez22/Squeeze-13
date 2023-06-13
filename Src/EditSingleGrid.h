#if !defined(AFX_EDITSINGLEGRID_H__608A8DBA_BA47_4AB4_879C_A123BD565685__INCLUDED_)
#define AFX_EDITSINGLEGRID_H__608A8DBA_BA47_4AB4_879C_A123BD565685__INCLUDED_

#include "ReturnableEdit.h"
#include "Isotherm.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditSingleGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditSingleGrid window

class EditSingleGrid : public CStatic
{
// Construction
public:
	EditSingleGrid();

// Attributes
public:

// Operations
public:
	double Value;
	bool cellEdit,
		  bFieldTab;
	
	CString FTFileName,
			  PathName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditSingleGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~EditSingleGrid();
	ReturnableEdit theEdit;
	
	Isotherm FieldTable; // Table Isotherm

	CRect clientRect,
			editRect;

	CString sEdit;
	
	bool editHasFocus;
	// Generated message map functions
protected:
	//{{AFX_MSG(EditSingleGrid)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSINGLEGRID_H__608A8DBA_BA47_4AB4_879C_A123BD565685__INCLUDED_)
