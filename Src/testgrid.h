#if !defined(AFX_TESTGRID_H__9DAC16DC_0673_4D26_9257_EE1FB96E3981__INCLUDED_)
#define AFX_TESTGRID_H__9DAC16DC_0673_4D26_9257_EE1FB96E3981__INCLUDED_

#include "ReturnableEdit.h"	// Added by ClassView
#include "ReturnableComboBox.h"	// Added by ClassView
#include "Isotherm.h"
#include "StageDlg.h"
#include "ViscoDlg.h"
#include "BlockerDlg.h"
#include "MutSolvDlg.h"
#include "CaNaDlg.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// testgrid.h : header file
//

#define MAX_COLS 20

/////////////////////////////////////////////////////////////////////////////
// testgrid window

class testgrid : public CStatic
{
// Construction
public:
	testgrid();
	testgrid(int, int);
	testgrid(int, int, int, int);
	void InitGrid();
	testgrid& operator=(const testgrid&);
	void Equate(const testgrid&);
	
// Attributes
public:
	enum CELL_EDIT
	{
		READ_ONLY,
		EDIT,
		COMBO,
		ISO,
		BICOND,
		FTV,
		TABLE
	};

	enum CELL_FORMAT
	{
		NORMAL,
		HILITE,
		GRAY,
		HEADER
	};
	
	enum PAGE
	{
		PP1,
		PP2,
		PP3
	};

	PAGE Tab;

	CELL_EDIT **cellEdit;
	CELL_FORMAT **cellFormat;

	CStringArray colS[MAX_COLS];	// string arrays for cell values
	CStringArray comboList[MAX_COLS];	// string arrays for combo boxes
	
	int ** iList,	// index of list to use for combo box
		 nRows,
		 nCols,
		 nL,
		 nC,
		
	    *colWidth,
	    *colPos,

	    *rowHeight,
	    *rowPos,
	    ** hmerge,	// 0 = no merge, 1 = merge with next cell, -1 = with previous cell, etc.
	    ** vmerge;	// as above, but vertical

	CScrollBar vScroller;

	int vScrollPos,
		 Units,
		 Header,
		 kTemp;

	CRect clientRect,
			scrollRect,
			editRect;
	
//	CEdit **Grids;

	CEdit cell;
	
	bool editHasFocus,
		  boxHasFocus;

	StageDlg StgDlg;
	
	ViscoDlg VisDlg;
	
	BlockerDlg BloDlg;

	MutSolvDlg MSDlg;
	
	CaNaDlg CaNaDlg;

	Isotherm IsoTable, // Table Isotherm
			   RelPermTable; // Table Relative Permeability

	int iModel,
		 nDisabled; // = 0 Disabled, 1 Not Disabled
	
	static void set_compRole(int *Role){ for(int i=0;i<10;i++) *(compRole+i) = *(Role+i);};
	static void set_compRole(int c, int Role){ compRole[c] = Role;};
	static int get_compRole(int c){ return compRole[c];};
	static int get_compAdTy(int r){ return compAdTy[r];};

private:
	void OpenIsothermTable();
	void OpenRelPermTable();
	void ReadFileTableIso(const char * );
	int cellC;	// to store last clicked cell's column
	int cellR;	// to store last clicked cell's row
	bool initialised;
	int static compRole[MaxComp];
	int static compAdTy[MaxComp];

// Operations
public:
	void Allocate();
	void DeAllocate();
	void DrawGrid(CDC* pDC, CRect rect);
	void CalculateColRowPos();
	void SetEditRect(int, int );
	void SetColWidth(int, int );
	void SetRowHeight(int, int );
	void SetColFormat(int c, CELL_FORMAT fmt);
	void SetRowFormat(int r, CELL_FORMAT fmt);
	void SetColEdit(int c, CELL_EDIT edit);
	void SetRowEdit(int r, CELL_EDIT edit);
	void SetCellText(int c, int r, CString txt);
	void Merge(int c1, int c2, int r1, int r2);
	void UnMerge(int c1, int c2, int r1, int r2);
	void SetColRowComboList(int c, int r, int list);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(testgrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Disabled();
	void Copy( const testgrid& rhs);
	ReturnableComboBox theBox;
	ReturnableEdit theEdit;
	virtual ~testgrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(testgrid)
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
	afx_msg LRESULT OnBoxLostFocus(WPARAM w, LPARAM l);
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTGRID_H__9DAC16DC_0673_4D26_9257_EE1FB96E3981__INCLUDED_)
