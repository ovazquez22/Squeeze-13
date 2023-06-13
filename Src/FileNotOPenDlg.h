#if !defined(AFX_FILENOTOPENDLG_H__EA971E35_4D76_449B_B708_CB6137259E09__INCLUDED_)
#define AFX_FILENOTOPENDLG_H__EA971E35_4D76_449B_B708_CB6137259E09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNotOPenDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// FileNotOPenDlg dialog

class FileNotOpenDlg : public CDialog
{
// Construction
public:
	FileNotOpenDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(FileNotOPenDlg)
	enum { IDD = IDD_FILEOPEN_DLG };
	CStatic	m_Static_FileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FileNotOPenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FileNotOPenDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENOTOPENDLG_H__EA971E35_4D76_449B_B708_CB6137259E09__INCLUDED_)
