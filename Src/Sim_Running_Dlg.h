#pragma once


// Sim_Running_Dlg dialog

class Sim_Running_Dlg : public CDialogEx
{
	DECLARE_DYNAMIC(Sim_Running_Dlg)

public:
	Sim_Running_Dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~Sim_Running_Dlg();
	void Update(bool, CString);

// Dialog Data
	enum { IDD = IDD_DIALOG_SIM_RUN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticTxt();
};
