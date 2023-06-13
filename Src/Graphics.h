#if !defined(AFX_GRAPHICS_H__205DB244_C7EE_4F50_97B7_DD957CE31A1B__INCLUDED_)
#define AFX_GRAPHICS_H__205DB244_C7EE_4F50_97B7_DD957CE31A1B__INCLUDED_

#include "GraphXY.h" 
#include "BitmapEx.h"
#define DEFAULT_BITMAP_FILE_EXTENSION						_T("bmp")
#define DEFAULT_BITMAP_FILE_NAME							_T("My Drawing")

struct MIC
{	
   double Height,
	      wConc,
		  wprd;
};

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Graphics.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Graphics dialog

class Graphics : public CDialog
{
public:
	int xPMax,
		 xPMin,
		 yPMax,
		 yPMin,
		 Num,
		 nOutFiles,
		 kTemp;

	double xMax,
			 xMin,
			 yMax,
			 yMin,
			 MicConc;

	CGraphXY GraphXY;
	
	vector<MIC> Mic;

	vector<double> RT;

	vector<vector<double>>	RC_L_Out,	// Stores the Layer Concentration
							WC_L_Out,	// Stores the Well Concentration
							WPRD;		// Stores the Water Production Rate

	vector<vector<vector<double>>>	C_L_Out,		// Concentration Profiles vs Radius and Layer	
									AD_L_Out,		// Adsoprtion Profiles vs Radius and Layer	
									SW_L_Out,		// Water Saturation vs Radius and Layer	
									FTemp_L_Out;	// Formation Temperature vs Radius and Layer

	CString Ptitle,
			Stage,
			title,
			xLabel,
			yLabel,
			PathNameReadFile;
	
	int logScale; // 0 = Log Scale, 1 = Linear Scale
	bool bTitle,
		  bMic,
		  bGraphs;
	
	//Background color
	COLORREF m_crBackgroundColor;
	
	//Bitmap stuff representing our drawing
	HBITMAP m_hDrawingSurface;
	BYTE* m_pDrawingSurfaceBits;
	BITMAPINFOHEADER BMIH;

public:
	void SetXYMaxMin();
	void SetNum(int);
	void UpdateInterface(CString);
	static const COLORREF m_crColors[8];
	bool CreateDIB();

// Construction
public:
	Graphics(CWnd* pParent = NULL);   // standard constructor
	virtual ~Graphics();
// Dialog Data
	//{{AFX_DATA(Graphics)
	enum { IDD = IDD_GRAPHICS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Graphics)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Graphics)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCancelMode();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DrawGraph(CDC *pDC, int, int, int, int);
	void DrawTics(CDC *pDC, CRect rect);
	void DrawAxis(CDC *, CRect);

private:
	CDC m_pMemDC;	
	CRect m_rcClient;
public:
	virtual 
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCopytoclipboard();
	afx_msg void OnSaveOutput();
	afx_msg void OnSaveImage();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICS_H__205DB244_C7EE_4F50_97B7_DD957CE31A1B__INCLUDED_)
