// OV Grid DLLDlg.h : header file
//

#if !defined(AFX_OVGRIDDLLDLG_H__E48E1F34_57C2_416A_967B_053B5B23B1F0__INCLUDED_)
#define AFX_OVGRIDDLLDLG_H__E48E1F34_57C2_416A_967B_053B5B23B1F0__INCLUDED_

#include "testgrid.h"	// Added by ClassView
#include "ReturnableEdit.h"	// Added by ClassView
#include "Sheet.h"	// Added by ClassView
#include "PropPage1.h"
#include "PropPage2.h"
#include "PropPage3.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COVGridDLLDlg dialog

class COVGridDLLDlg : public CDialog
{
// Construction
public:
	COVGridDLLDlg(CWnd* pParent = NULL);	// standard constructor
	
	ReturnableEdit EditTest;
	
	testgrid gr;

// Dialog Data
	//{{AFX_DATA(COVGridDLLDlg)
	enum { IDD = IDD_OVGRIDDLL_DIALOG };
	CEdit	m_NumRows;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COVGridDLLDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COVGridDLLDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEditModel();
	//}}AFX_MSG
	afx_msg LRESULT OnListCtrlUpdate();
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OVGRIDDLLDLG_H__E48E1F34_57C2_416A_967B_053B5B23B1F0__INCLUDED_)
