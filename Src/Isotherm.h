#if !defined(AFX_ISOTHERM_H__36879F22_5F42_4287_B352_950F132BD08A__INCLUDED_)
#define AFX_ISOTHERM_H__36879F22_5F42_4287_B352_950F132BD08A__INCLUDED_

#include "TableGrid.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Isotherm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Isotherm dialog

class Isotherm : public CDialog
{
// Construction
public:
	TableGrid TabGr;
	Isotherm(CWnd* pParent = NULL);   // standard constructor
	void ReadFileTableIso(const char *NameFile);
	void ReadFieldFileTable(const char *File, int );
	void ReadFileTableRelPerm(const char *File);
	void ReadLabDataTable(const char *File);

	void SetPathName(const CString NameFile);

// Dialog Data
	//{{AFX_DATA(Isotherm)
	enum { IDD = IDD_ISO_TABLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	int NIso;		// Stores the number of concentration in the Iso Table
	
	double *Conc, 
			 *Adso,
			 *Time,
			 *Sw,
			 *Krw,
			 *Kro;

	CString PathName,
			  NameFile;
	
	BOOL bFileOpen,
		  bTab,
		  bRelPerm,
		  bIsoDer;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Isotherm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Isotherm)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOTHERM_H__36879F22_5F42_4287_B352_950F132BD08A__INCLUDED_)
