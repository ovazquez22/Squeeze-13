#pragma once

#include "GraphCtrl.h"
#include "afxwin.h"
#include "TableGrid.h"

struct data_sqz
{
	vector<double> xVal;
	vector<double> dat;
};

// Opt_Output dialog

class Opt_Output : public CDialogEx
{
	DECLARE_DYNAMIC(Opt_Output)

public:
	Opt_Output(CWnd* pParent = NULL);   // standard constructor
	virtual ~Opt_Output();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPT_OUTPUT };
	CGraphCtrl m_Graph;

public:	
	int nPar,			// Parameter space dimension
		Fixed_Target;   // =0 for normal PSO Optimization and =1 for Secant Optimization	

	string	SavePathName,	// Pathname for Design Input file save 
			InputPathName;	// Pathname for Optimisatin Design Input file 

	vector<int>		parInd;		// Stores index of Pareto Front
	vector<data_sqz>	opt_data;	// label per parameter X
	vector<string>	Label;	// label per parameter X
	vector<double>	X,		// Stores Base Base Design Values  
					Vol;	// Stores the Volume of X

	TableGrid PSOGrid,
			  PSOGridHeader;

	void InitialiseGraph();
	void InitPSOGrid();
	void UpdateGraph(vector<double>, vector<double>, int, int);
	void WriteOptimisationFileModels(vector<double>, CString);
	void Pareto_Front(int, int);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	
// Implementation
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_comboList_Xaxis,
			  m_comboList_Yaxis;
	afx_msg void OnCbnCloseupComboXaxis();
	afx_msg void OnCbnCloseupComboYaxis();
	CScrollBar m_hScrollBar;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CScrollBar m_vScrollBar;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnCustom(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSavedesign(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnExporttable(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnExportparetofront(WPARAM wparam, LPARAM lparam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
