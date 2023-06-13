/////////////////////////////////////////////////////////////////////////////
// CGraphLine	
// An Graph Line
/////////////////////////////////////////////////////////////////////////////
class CGraphLine
{
public:
	CGraphLine(); // Default constructor to keep CArray happy
	CGraphLine(LPCTSTR lpszName, COLORREF crColor, int nWidth, UINT uLower, UINT uUpper);	
	~CGraphLine();

public:
	void SetName(LPCTSTR lpszName) { m_strName = lpszName; }
	void SetRange(UINT uLower, UINT uUpper);
	void SetPen(int nWidth, COLORREF crColor);
	void SetPos(UINT uPos);
	void DrawBand(CDC* pDC, RECT& rcBand);
	void SetMaxMin(double, double, double, double, int, int, int, int);

public:
	UINT m_uLower;
	UINT m_uUpper;
	UINT m_uPosY;
	UINT m_uPreviousPosY;
	UINT m_uOffset;			
	CPen m_pen;
	CString m_strName;
	
	double	*X,
			*Y;  

	int nPts,
		xPMax,
		xPMin,
		yPMax,
		yPMin,
		Sq; // Square side

	double xMax,
		   xMin,
		   yMax,
		   yMin;

	bool check,
		 Point,		// Point
		 Circle,	// Circle 
		 Square,	// Field Data
		 logScale;  // True log scale

	COLORREF color;
};