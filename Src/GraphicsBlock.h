#if !defined(AFX_GRAPHICSBLOCK_H__1D053E9C_19DE_4EA0_BD15_F6A8F9B66261__INCLUDED_)
#define AFX_GRAPHICSBLOCK_H__1D053E9C_19DE_4EA0_BD15_F6A8F9B66261__INCLUDED_

#include "GraphXY.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GraphicsBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GraphicsBlock dialog

class GraphicsBlock : public CDialog
{
public:
	CRect clientRect;
	int xPMax,
		 xPMin,
		 yPMax,
		 yPMin,
		 Num,
		 nLay,
		 nBlocks;

	double xMax,
			 xMin,
			 yMax,
			 yMin,
			 *layHeight;
//			 *x,
//			 **y;


	CGraphXY GraphXY;

	CString Stage,
			  title,
			  xLabel,
			  yLabel;
	
	bool bGraphs;

public:
	void SetXYMaxMin(int, int);
	void SetNum(int);
	void UpdateInterface(CString);
	static const COLORREF m_crColors[8];
// Construction
public:
	GraphicsBlock(CWnd* pParent = NULL);   // standard constructor
	~GraphicsBlock();
// Dialog Data
	//{{AFX_DATA(GraphicsBlock)
	enum { IDD = IDD_GRAPHICS_BLOCK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GraphicsBlock)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GraphicsBlock)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CopyChartToClipboard();
	void DeAllocate();
	void Allocate();
	void DrawBlocks(CDC *pDC, CRect rect, int);
	void DrawGraphicsBlocks(CDC *pDC, int, int, int);
	void DrawTics(CDC *pDC, CRect rect);
	void DrawAxis(CDC *, CRect);

private:
	CDC m_pMemDC;	
	CRect m_rcClient;
public:
	virtual 
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCopytoclipboard();
	afx_msg void OnSaveImage();

public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICSBLOCK_H__1D053E9C_19DE_4EA0_BD15_F6A8F9B66261__INCLUDED_)
