#if !defined(AFX_TABLEGRID_H__553056E6_A7F5_4BE4_A2AE_E5CA9C7A0AD3__INCLUDED_)
#define AFX_TABLEGRID_H__553056E6_A7F5_4BE4_A2AE_E5CA9C7A0AD3__INCLUDED_

#include "ReturnableEdit.h"
#include "ReturnableComboBox.h"

#define MAX_COLS 20

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TableGrid.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TableGrid window

class TableGrid : public CStatic
{
// Construction
public:
	TableGrid();
	TableGrid(int);
	TableGrid(int, int);
	TableGrid(int, double *, double *);
	TableGrid(int, double *, double *, double *);

	void InitGrid();
	void InitGridFile(double *, double *);
	void InitGridFileRelPerm(double *, double *, double *);

	void Allocate();
	void DeAllocate();
	TableGrid& operator=(const TableGrid&);
	void Equate(const TableGrid&);
	void Disabled();
	void Enabled();

// Attributes
public:
	ReturnableComboBox theBox;
	ReturnableEdit theEdit;
	
	enum CELL_EDIT
	{
		READ_ONLY,
		EDIT,
		COMBO,
		TABLE,
		FIELD
	};

	enum CELL_FORMAT
	{
		NORMAL,
		BLUE,
		HILITE,
		GRAY,
		HEADER
	};
	
	CELL_EDIT **cellEdit;
	CELL_FORMAT **cellFormat;

	int ** iList,	// index of list to use for combo box
		 nRows,
		 nCols,
		
	    *colWidth,
	    *colPos,

	    *rowHeight,
	    *rowPos;

	int cellC,		// to store last clicked cell's column
		cellR,		// to store last clicked cell's row
		nDesign;	// Selected Design 

	bool initialised,
		 bScroll;

	CScrollBar vScroller,
			   hScroller;
	
	CStringArray colS[MAX_COLS],
					 comboList[MAX_COLS];	// string arrays for combo boxes
	
	CString PathNameReadFile;

	int vScrollPos,
		hScrollPos;

	CRect clientRect,
		  scrollRect,
		  editRect;
	
	bool editHasFocus,
		  boxHasFocus;
	
	int TypeStage,
		 checkPIDf,
		 FieldData; // == 0 Width column width/nCols; = 1 two columns 60-40; = 2 width is input

// Operations
public:
	void DrawGrid(CDC* pDC, CRect rect);
	void CalculateColRowPos();
	void SetEditRect(int, int );
	void SetColWidth(int, int );
	void SetRowHeight(int, int );
	void SetColRowComboList(int, int, int);
	void SetColFormat(int, CELL_FORMAT);
	void SetRowFormat(int, CELL_FORMAT);
	void SetColEdit(int, CELL_EDIT);
	void SetRowEdit(int, CELL_EDIT);
	void SetCellText(int, int, CString);
	void UpdateGrid();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TableGrid)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TableGrid();

	// Generated message map functions
protected:
	//{{AFX_MSG(TableGrid)
	afx_msg void OnPaint();
	afx_msg LRESULT OnEditLostFocus(WPARAM w, LPARAM l);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg LRESULT OnListCtrlUpdate(WPARAM w, LPARAM l);
	afx_msg LRESULT OnBoxLostFocus(WPARAM w, LPARAM l);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSavedesign();
	afx_msg void OnExporttable();
	afx_msg void OnExportparetofront();

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABLEGRID_H__553056E6_A7F5_4BE4_A2AE_E5CA9C7A0AD3__INCLUDED_)
