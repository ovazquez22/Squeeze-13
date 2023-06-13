#pragma once
#include "afxwin.h"


// IsoMatchParDlg dialog

class IsoMatchParDlg : public CDialogEx
{
	DECLARE_DYNAMIC(IsoMatchParDlg)

public:
	Model M;
	IsoMatchParDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~IsoMatchParDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_ISO_PAR };
/*	CEdit	m_Edit_K_Orig,
			m_Edit_N_Orig,
			m_Edit_R_Orig,
			m_Edit_K_Up,
			m_Edit_N_Up,
			m_Edit_R_Up,
			m_Edit_K_Low,
			m_Edit_N_Low,
			m_Edit_R_Low;*/

	void UpdateControls();
	void UpdateEdits();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	
	CEdit m_Edit_K_Orig;
	CEdit m_Edit_K_Up;
	CEdit m_Edit_PopSize;
};
