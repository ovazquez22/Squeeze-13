// OutFile.h: interface for the OutFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OUTFILE_H__50892D27_65AD_49B6_9885_6A560AA25F83__INCLUDED_)
#define AFX_OUTFILE_H__50892D27_65AD_49B6_9885_6A560AA25F83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class OutFile  
{
public:
	OutFile();
	virtual ~OutFile();
	void Equate(const OutFile& );
	void Allocate(void);
	void DeAllocate(void);

   double **outArray;

	int nPts, 
		 nCols;

	char NameFile[100];	

};

#endif // !defined(AFX_OUTFILE_H__50892D27_65AD_49B6_9885_6A560AA25F83__INCLUDED_)
