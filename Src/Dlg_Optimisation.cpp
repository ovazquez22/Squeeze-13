// Dlg_Optimisation.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Dlg_Optimisation.h"


// Dlg_Optimisation dialog

Dlg_Optimisation::Dlg_Optimisation(CWnd* pParent /*=NULL*/)
	: CDialog(Dlg_Optimisation::IDD, pParent)
{
	nRows = 1;
	nCols = 5;
}

Dlg_Optimisation::~Dlg_Optimisation()
{
}

void Dlg_Optimisation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPTIMISATION, StageGrid);
}


BEGIN_MESSAGE_MAP(Dlg_Optimisation, CDialog)
END_MESSAGE_MAP()


// Dlg_Optimisation message handlers
BOOL Dlg_Optimisation::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString openFilter = "Input data files (*.dat)|*.dat|";
	char full[_MAX_PATH];

	/* Get the current working directory: */
	if (_getcwd(full, _MAX_PATH) == NULL)
		perror("_getcwd error");
	else
	{
		strcat(full, "\\test.dat");
	}

	CFileDialog openDialog(TRUE, "inp", full, OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	if (openDialog.DoModal() == IDOK)
	{	
		PathNameReadFile = openDialog.GetPathName();

		Mod.ReadFileSQZ5Radial(openDialog.GetFileName());		// read input data
		
		nRows = 1;

		for (int s = 0; s < Mod.nStg; s++)
		{
			if (Mod.stg[s].TypeStg != 3)
			{	
				if (Mod.stg[s].TypeStg == 1)
					nRows += 2;
				else
					nRows++;
			}
		}
	}
	
	nCols = 5;

	testgrid tempgrid(nRows, nCols);

	StageGrid = tempgrid;

	InitialiseGrid();

	Invalidate(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void Dlg_Optimisation::OnOK()
{
	CString openFilter = "Input data files (*.dat)|*.dat|";
	char full[_MAX_PATH];

	CFileDialog saveDialog(FALSE, "dat", PathNameReadFile, OFN_HIDEREADONLY |
		OFN_FILEMUSTEXIST, openFilter, this);

	UpdateFromEdits();

	if (saveDialog.DoModal() == IDOK)
	{
		Mod.WriteFileSQZ5Radial(saveDialog.GetFileName());
	}

	CDialog::OnOK();


}

void Dlg_Optimisation::InitialiseGrid()
{
	CString sItem;

	int r,
		numSt = 0,
		height,
		width;
	
	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_OPTIMISATION);
	pWnd->GetWindowRect(&rect);

	height = rect.Height();
	width = rect.Width() - 14;

	StageGrid.SetColEdit(4, testgrid::COMBO);
	StageGrid.comboList[0].RemoveAll();
	StageGrid.comboList[0].Add("Yes");
	StageGrid.comboList[0].Add("No");

	// HEADER
	StageGrid.SetRowFormat(0, testgrid::HEADER);
	StageGrid.colS[0][0] = "TypeStg";
	StageGrid.colS[1][0] = "Original\n(Hour/ppm)";
	StageGrid.colS[2][0] = "Low";
	StageGrid.colS[3][0] = "Upper";
	StageGrid.colS[4][0] = "Included";

	for (int r = 0; r < nRows; r++)
	{
		StageGrid.SetRowHeight(r, 20);
	}

	for (int c = 0; c < nCols; c++)
	{
		StageGrid.SetColWidth(c, int(width / nCols));
	}

	int s = 0;
	for (r = 0; r < nRows-1; r++)
	{	
		StageGrid.colS[2][r + 1] = "0.05";
		StageGrid.colS[3][r + 1] = "1.5";

		double Vol;
			
		if (Mod.stg[s].TypeStg == 2)
		{
			StageGrid.colS[0][r+1] = "Overflush";
						
			Vol = Mod.stg[s].Time * Mod.stg[s].Qwt * 60;
			sItem.Format((Vol <= (1e6 - 1)) ? "%.4g" : "%.2e", Vol);
			sItem.Replace(_T("e+000"), _T("e"));
			sItem.Replace(_T("e+00"), _T("e"));
			sItem.Replace(_T("e+0"), _T("e"));
			StageGrid.colS[1][r+1] = sItem;

			if (Mod.stg[s].objTime == 1)
			{
				StageGrid.colS[4][r + 1] = "Yes";
				StageGrid.colS[2][r + 1].Format("%.8g", Mod.SqzOpt.lX[Mod.stg[s].objIndexTime]);
				StageGrid.colS[3][r + 1].Format("%.8g", Mod.SqzOpt.uX[Mod.stg[s].objIndexTime]);
			}
			else
				StageGrid.colS[4][r + 1] = "No";
		}

		if (Mod.stg[s].TypeStg == 5)
		{
			StageGrid.colS[0][r+1] = "Shut-in";
			sItem.Format((Mod.stg[s].Time <= (1e6 - 1)) ? "%.4g" : "%.2e", Mod.stg[s].Time);
			sItem.Replace(_T("e+000"), _T("e"));
			sItem.Replace(_T("e+00"), _T("e"));
			sItem.Replace(_T("e+0"), _T("e"));
			StageGrid.colS[1][r+1] = sItem;

			if (Mod.stg[s].objTime == 1)
			{
				StageGrid.colS[4][r + 1] = "Yes";
				StageGrid.colS[2][r + 1].Format("%.8g", Mod.SqzOpt.lX[Mod.stg[s].objIndexTime]);
				StageGrid.colS[3][r + 1].Format("%.8g", Mod.SqzOpt.uX[Mod.stg[s].objIndexTime]);
			}
			else
				StageGrid.colS[4][r + 1] = "No";
		}

		if (Mod.stg[s].TypeStg == 1)
		{
			StageGrid.colS[0][r+1] = "Inj_Vol";
					
			Vol = Mod.stg[s].Time * Mod.stg[s].Qwt * 60;			
			sItem.Format((Vol <= (1e6 - 1)) ? "%.4g" : "%.2e", Vol);
			sItem.Replace(_T("e+000"), _T("e"));
			sItem.Replace(_T("e+00"), _T("e"));
			sItem.Replace(_T("e+0"), _T("e"));
			StageGrid.colS[1][r + 1] = sItem;
			if (Mod.stg[s].objTime == 1)
			{
				StageGrid.colS[4][r + 1] = "Yes";
				StageGrid.colS[2][r + 1].Format("%.8g", Mod.SqzOpt.lX[Mod.stg[s].objIndexTime]);
				StageGrid.colS[3][r + 1].Format("%.8g", Mod.SqzOpt.uX[Mod.stg[s].objIndexTime]);
			}
			else
				StageGrid.colS[4][r + 1] = "No";

			StageGrid.colS[0][r +2] = "Inj_SI";
			StageGrid.colS[1][r + 2].Format("%.8g", Mod.stg[s].InitialCw);
			if (Mod.stg[s].objSI == 1)
			{
				StageGrid.colS[4][r + 2] = "Yes";
				StageGrid.colS[2][r + 2].Format("%.8g", Mod.SqzOpt.lX[Mod.stg[s].objIndexSI]);
				StageGrid.colS[3][r + 2].Format("%.8g", Mod.SqzOpt.uX[Mod.stg[s].objIndexSI]);
			}
			else
			{
				StageGrid.colS[4][r + 2] = "No";
				StageGrid.colS[2][r + 2] = "0.05";
				StageGrid.colS[3][r + 2] = "1.5";
			}
			r++;
		}

		s++;
			
	}

	Invalidate(FALSE);
}


void Dlg_Optimisation::UpdateFromEdits()
{
	int nS = 0,
		nI = 0;

	for (int r = 1; r < nRows; r++)
	{
		if (StageGrid.colS[4][r] == "Yes") nI++;
	}

	if (nI != Mod.SqzOpt.nPar)
	{
		Mod.SqzOpt.lX.resize(nI);
		Mod.SqzOpt.uX.resize(nI);

		Mod.SqzOpt.nPar = nI;
	}
	
	for (int r = 0; r < Mod.nStg; r++)
	{
		Mod.stg[r].objTime = 0;
		Mod.stg[r].objSI = 0;
	}

	nI = 0;
	
	for (int r = 0; r < nRows - 1; r++)
	{
		double Vol;

		if (StageGrid.colS[0][r + 1] == "Overflush")
		{
			if (StageGrid.colS[4][r + 1] == "Yes")
			{	
				Mod.stg[nS].objTime = 1;
				Mod.stg[nS].objIndexTime = nI;
				
				Mod.SqzOpt.lX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				Mod.SqzOpt.uX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);

				nI++;
			}	
			
			nS++;
		}

		if (StageGrid.colS[0][r + 1] == "Shut-in")
		{
			if (StageGrid.colS[4][r + 1] == "Yes")
			{	
				Mod.stg[nS].objTime = 1;
				Mod.stg[nS].objIndexTime = nI;

				Mod.SqzOpt.lX.push_back(atof(StageGrid.colS[2][r + 1]));
				Mod.SqzOpt.uX.push_back(atof(StageGrid.colS[3][r + 1]));

				nI++;
			}
			
			nS++;
		}

		if (StageGrid.colS[0][r + 1] == "Inj_Vol")
		{
			if (StageGrid.colS[4][r + 1] == "Yes")
			{
				Mod.stg[nS].objTime = 1;
				Mod.stg[nS].objIndexTime = nI;

				Mod.SqzOpt.lX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				Mod.SqzOpt.uX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);

				nI++;
			}
		}
		
		if (StageGrid.colS[0][r + 1] == "Inj_SI")
		{	
			if (StageGrid.colS[4][r + 1] == "Yes")
			{
				Mod.stg[nS].objSI = 1;
				Mod.stg[nS].objIndexSI = nI;

				Mod.SqzOpt.lX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				Mod.SqzOpt.uX[Mod.stg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);

				nI++;
			}
			
			nS++;
		}

	}

}