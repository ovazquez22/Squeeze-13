#pragma once


// Dlg_Optimisation dialog
#include "testgrid.h"	// Added by ClassView

class Dlg_Optimisation : public CDialog
{
public:
	Dlg_Optimisation(CWnd* pParent = NULL);   // standard constructor
	virtual ~Dlg_Optimisation();

// Dialog Data
	enum { IDD = IDD_OPTIMISATION_DLG };
	int nRows,
		nCols;

	testgrid StageGrid;

	Model Mod;

	CString PathNameReadFile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	virtual BOOL OnInitDialog();

	void InitialiseGrid();
	void UpdateFromEdits();

	DECLARE_MESSAGE_MAP()
};
