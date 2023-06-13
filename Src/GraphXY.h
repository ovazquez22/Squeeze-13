// GraphXY.h: interface for the CGraphXY class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHXY_H__801F853F_B683_4EAF_90B2_F3B1894879CE__INCLUDED_)
#define AFX_GRAPHXY_H__801F853F_B683_4EAF_90B2_F3B1894879CE__INCLUDED_
/*
struct OutFile
{
    double **outArray;

	 int nPts,
		  nCols;

	 char NameFile[100];
};
*/

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGraphXY  
{
public:
	void SetMaxNumTimes(int, int, int, int, int);
	CGraphXY();
	CGraphXY(int, int, int, int, int);
	virtual ~CGraphXY();
	void AddElement();

	double *x,
			 **y,
			 ***y1,
			 *xP,
			 *yP;

	int nPts,
		 MaxNumTimes,
		 nLay,
		 nDat,
		 nFileNum;

	vector<OutFile> pFile;

private:
	void Allocate();
	void DeAllocate();
	void Equate(const CGraphXY& rhs);
};

#endif // !defined(AFX_GRAPHXY_H__801F853F_B683_4EAF_90B2_F3B1894879CE__INCLUDED_)
