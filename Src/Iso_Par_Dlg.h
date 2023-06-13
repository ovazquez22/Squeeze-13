#pragma once
#include "afxwin.h"


// Iso_Par_Dlg dialog

class Iso_Par_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Iso_Par_Dlg)

public:
	Iso_Par_Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~Iso_Par_Dlg();

	void UpdateControls();
	void UpdateEdits();

// Dialog Data
	enum { IDD = IDD_DIA_ISO_PAR };
	Model M;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_edit_Pop_size;
	CEdit m_edit_Num_Gen;
	CEdit m_Edit_K_Orig;
	CEdit m_Edit_K_Up;
	CEdit m_Edit_K_Low;
	CEdit m_Edit_N_Orig;
	CEdit m_Edit_N_Up;
	CEdit m_Edit_N_Low;
	CEdit m_Edit_R_Orig;
	CEdit m_Edit_R_Up;
	CEdit m_Edit_R_Low;
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedCancel();
};
