#if !defined(GRAPHCTRLH)
#define GRAPHCTRLH

#include <AfxTempl.h>
#include "CGraphLine.h"


struct RGB_Col {
    float r, g, b;
};

/////////////////////////////////////////////////////////////////////////////
// CGraphCtrl	
// An Graph like control with support for multiple lines
// Based on COscilloscopeCtrl Tanzim Husain [tanzim@mail.com], [http://www.codeproject.com/Articles/3799/COscilloscopeCtrl]
/////////////////////////////////////////////////////////////////////////////

class CGraphCtrl : public CWnd
{
// Construction
public:
	CGraphCtrl();

// Attributes
public:
	int nLines,
	
		xPMax,
		xPMin,
		yPMax,
		yPMin,
		
		xLabFont,	// x Label Font
		yLabFont,	// y Label Font

		xFont,	// x-Axis Font
		yFont;	// y-Axis Font

	double xMax,
		   xMin,
		   yMax,
		   yMin,
		   
		   cL,
		   cR,
		   cT,
		   cB;

	bool logScale, // True log scale
		 bTitle;

	CString xLabel,
			yLabel,
			title;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGraphCtrl)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnCopytoclipboard();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	CGraphLine *m_Lines;

public:
	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT uID);
	
	BOOL SetGridColor(COLORREF crGrid);
	BOOL SetBkColor(COLORREF crBk);	
	BOOL Update();
	
	
	void SetName(int nBand, LPCTSTR lpszName) { m_Lines[nBand].SetName(lpszName); }
	CString GetName(int nBand) {return m_Lines[nBand].m_strName; }
	void SetRange(int nBand, UINT uLower, UINT uUpper) { m_Lines[nBand].SetRange(uLower, uUpper); }
	void SetPen(int nBand, int nWidth, COLORREF crColor) { m_Lines[nBand].SetPen(nWidth, crColor); }
	void SetPos(int nBand, UINT uPos) { m_Lines[nBand].SetPos(uPos); }
	void DrawTitle(CDC *, CRect);
	void DrawBands();
	void DrawAxis(CDC *, CRect);
	void ReadOutputIsoDer(const char *);
	void ReadOutputIsoCheck(char *, int);
	void NiceTics(double &, double &, double &, int &);
	double niceNum(double range, int round);
	RGB_Col hslToRgb( float, float, float);	

private:
	BOOL InvalidateCtrl();

private:
	
	CDC* m_pMemDC;	
	CRect m_rcClient;		

	COLORREF m_crBackGround;
	COLORREF m_crGrid;		
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line
#endif // !defined(GRAPHCTRLH)