#pragma once

#include "GraphCtrl.h"
#include "Sim_Running_Dlg.h"

// IsothermDerivationGraph dialog

class IsothermDerivationGraph : public CDialogEx
{
	DECLARE_DYNAMIC(IsothermDerivationGraph)

public:
	IsothermDerivationGraph(CWnd* pParent = NULL);   // standard constructor
	virtual ~IsothermDerivationGraph();
	void WriteFileSQZ5Linear(const char*);

// Dialog Data
	enum { IDD = IDD_IOSTHERM_DERIVATION_GRAPH_DIALOG };

	CGraphCtrl m_Graph_Iso_Sim,
			   m_Graph_Iso;

	Model Mod;

private:
	double *xIso,
		   *yIso;
	
	CButton	m_Check_YLogScale,
			m_Check_Fix_Limits;

	CDC m_pMemDC;	
	CRect m_rcClient;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	int ProcessMessageLoop();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditXmin();
	afx_msg void OnEnChangeEditXmax();
	afx_msg void OnEnChangeEditYmin();
	afx_msg void OnEnChangeEditYmax();
	afx_msg void OnEnChangeEditXminSim();
	afx_msg void OnEnChangeEditXmaxSim();
	afx_msg void OnEnChangeEditYminSim();
	afx_msg void OnEnChangeEditYmaxSim();
	afx_msg void OnEnChangeEditIsoMove();
	afx_msg void OnBnClickedAutomatic();
	afx_msg void OnBnClickedValidateIso();
	afx_msg void OnBnClickedCheckYlog();
	afx_msg void OnBnClickedButtonValIso();
	afx_msg void OnBnClickedButtonSaveIso();
	afx_msg void OnBnClickedCheckFixLimits();
	afx_msg void OnBnClickedCancel();
};
