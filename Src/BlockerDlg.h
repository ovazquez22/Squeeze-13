#if !defined(AFX_BLOCKERDLG_H__50193B6A_53A2_4C00_A2D3_BADF0F48E3D0__INCLUDED_)
#define AFX_BLOCKERDLG_H__50193B6A_53A2_4C00_A2D3_BADF0F48E3D0__INCLUDED_

#include "TableGrid.h"
#include "RFLayer.h"	// Added by ClassView

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// BlockerDlg dialog

class BlockerDlg : public CDialog
{
// Construction
public:
	BlockerDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(BlockerDlg)
	enum { IDD = IDD_BLOCKERDLG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	RFLayer rfDlg;
	
	TableGrid WatRFGrid,
				 OilRFGrid;

	int nLay;

	bool check_FD;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(BlockerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

private:
	void UpdateControls();
	void UpdateFromEdits();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(BlockerDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKERDLG_H__50193B6A_53A2_4C00_A2D3_BADF0F48E3D0__INCLUDED_)
