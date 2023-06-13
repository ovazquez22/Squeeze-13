#pragma once

#include "GraphCtrl.h"

// CheckListBoxGraphs dialog


class CheckListBoxGraphs : public CDialogEx
{
	DECLARE_DYNAMIC(CheckListBoxGraphs)

public:
	CheckListBoxGraphs(CWnd* pParent = NULL);   // standard constructor
	virtual ~CheckListBoxGraphs();

// Dialog Data
	enum { IDD = IDD_CHECKLISTBOX_GRAPHS_DIALOG };
	CCheckListBox	m_CheckListIsotherms;

	CString openFile;
	CGraphCtrl m_Graph;

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListIsotherms();
	afx_msg void OnCheckchangeListIsotherms();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CEdit	m_Edit_xMax;
	CEdit	m_Edit_yMax;
	CEdit	m_Edit_xMin;
	CEdit	m_Edit_yMin;
	CButton	m_Check_YLogScale;
public:
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnBnClickedCheckYlogscale();
	afx_msg void OnBnClickedCancel();
};
