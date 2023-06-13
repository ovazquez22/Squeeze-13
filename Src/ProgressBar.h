#pragma once


// ProgressBar dialog

class ProgressBar : public CDialog
{
	DECLARE_DYNAMIC(ProgressBar)

public:
	ProgressBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~ProgressBar();

// Dialog Data
	enum { IDD = IDD_PROGRESS_BAR };
	CProgressCtrl	m_ProgressBar;
	CString openFile;
	int iProBar;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
