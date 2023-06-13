// PropPage7.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "PropPage7.h"
#include "afxdialogex.h"
#include "PropPage3.h"
#include "Sheet.h"

// PropPage7 dialog

IMPLEMENT_DYNAMIC(PropPage7, CPropertyPage)

PropPage7::PropPage7()
: CPropertyPage(PropPage7::IDD)
{
}

PropPage7::~PropPage7()
{
}

void PropPage7::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPTIMISATION2, StageGrid);
	DDX_Control(pDX, IDC_CHECK_OPER_CONS, m_Check_Op_Cons);
	DDX_Control(pDX, IDC_CHECK_ONLY_SI_VOL, m_Check_Only_SI_Vol);
	DDX_Control(pDX, IDC_CHECK_ONLY_TOT_VOL, m_Check_Only_TOT_Vol);
	DDX_Control(pDX, IDC_CHECK_FIXED_TARGET, m_Check_Fixed_Target);
}

BEGIN_MESSAGE_MAP(PropPage7, CPropertyPage)
	//{{AFX_MSG_MAP(PropPage5)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_ONLY_SI_VOL, &PropPage7::OnBnClickedCheckOnlySiVol)
	ON_BN_CLICKED(IDC_CHECK_ONLY_TOT_VOL, &PropPage7::OnBnClickedCheckOnlyTotVol)
	ON_BN_CLICKED(IDC_CHECK_FIXED_TARGET, &PropPage7::OnBnClickedCheckFixedTarget)
	ON_EN_CHANGE(IDC_EDIT_nPopSize, &PropPage7::OnEnChangeEditnpopsize)
	ON_EN_CHANGE(IDC_EDIT_BinsMT, &PropPage7::OnEnChangeEditBinsmt)
	ON_EN_CHANGE(IDC_EDIT_IterationsSecant, &PropPage7::OnEnChangeEditIterationssecant)
	ON_EN_CHANGE(IDC_EDIT_nBins, &PropPage7::OnEnChangeEditnbins)
	ON_STN_CLICKED(IDC_STATIC_OPTIMISATION2, &PropPage7::OnStnClickedStaticOptimisation2)
	ON_EN_CHANGE(IDC_EDIT4, &PropPage7::OnEnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT_nGen, &PropPage7::OnEnChangeEditngen)
	ON_EN_CHANGE(IDC_EDIT_SI_VOL, &PropPage7::OnEnChangeEditSiVol)
	ON_EN_CHANGE(IDC_EDIT_InjHourRate, &PropPage7::OnEnChangeEditInjhourrate)
	ON_BN_CLICKED(IDC_CHECK_OPER_CONS, &PropPage7::OnBnClickedCheckOperCons)
END_MESSAGE_MAP()

// PropPage7 message handlers
BOOL PropPage7::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	CRect rect;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_OPTIMISATION2);
	pWnd->GetWindowRect(&rect);

	height = rect.Height();
	width = rect.Width() - 14;

	InitialiseGrid();

	return TRUE;
}

void PropPage7::InitialiseGrid()
{
	CString sItem;

	int c,
		r,
		numSt = 0;

	StageGrid.SetColEdit(5, testgrid::COMBO);
	StageGrid.comboList[0].RemoveAll();
	StageGrid.comboList[0].Add("Yes");
	StageGrid.comboList[0].Add("No");

	// Read only Cols
	StageGrid.SetColEdit(0, testgrid::READ_ONLY);
	StageGrid.SetColEdit(1, testgrid::READ_ONLY);
	StageGrid.SetColEdit(2, testgrid::READ_ONLY);

	// Gray Cols
	StageGrid.SetColFormat(0, testgrid::HILITE);
	StageGrid.SetColFormat(1, testgrid::HILITE);
	StageGrid.SetColFormat(2, testgrid::HILITE);

	// HEADER
	StageGrid.SetRowFormat(0, testgrid::HEADER);
	StageGrid.colS[0][0] = "Stage";
	StageGrid.colS[1][0] = "TypeStg";
	StageGrid.colS[2][0] = "Original Value";
	StageGrid.colS[3][0] = "Low";
	StageGrid.colS[4][0] = "Upper";
	StageGrid.colS[5][0] = "Included";

	for (r = 0; r < nRows; r++)
	{
		StageGrid.SetRowHeight(r, 20);
	}

	StageGrid.SetRowHeight(0, 40);

	for (c = 0; c < nCols; c++)
	{
		StageGrid.SetColWidth(c, int(width / nCols));
	}

	int s = 0,
		count = 0;

	for (r = 0; r < nRows - 1; r++)
	{
		StageGrid.colS[3][r + 1] = "0.05";
		StageGrid.colS[4][r + 1] = "1.5";

		double Vol;

		if (P7SqzOpt->oStg[s].TypeStg == 1)
		{
			++count;

			StageGrid.colS[1][r + 1] = "Inj_Vol";

			Vol = P7SqzOpt->oStg[s].Time * P7SqzOpt->oStg[s].Qwt * 60;;
			sItem.Format((Vol <= (1e6 - 1)) ? "%.4g" : "%.2e", Vol);
			sItem.Replace(_T("e+000"), _T("e"));
			sItem.Replace(_T("e+00"), _T("e"));
			sItem.Replace(_T("e+0"), _T("e"));
			StageGrid.colS[2][r + 1] = sItem;

			if (P7SqzOpt->oStg[s].objTime == 1)
			{
				StageGrid.colS[5][r + 1] = "Yes";
				StageGrid.colS[3][r + 1].Format("%.8g", P7SqzOpt->lX[P7SqzOpt->oStg[s].objIndexTime]);
				StageGrid.colS[4][r + 1].Format("%.8g", P7SqzOpt->uX[P7SqzOpt->oStg[s].objIndexTime]);
			}
			else
				StageGrid.colS[5][r + 1] = "No";

			StageGrid.colS[1][r + 2] = "Inj_SI";
			StageGrid.colS[2][r + 2].Format("%.8g", P7SqzOpt->oStg[s].InitialCw[0]);
			if (P7SqzOpt->oStg[s].objSI == 1)
			{
				StageGrid.colS[5][r + 2] = "Yes";
				StageGrid.colS[3][r + 2].Format("%.8g", P7SqzOpt->lX[P7SqzOpt->oStg[s].objIndexSI]);
				StageGrid.colS[4][r + 2].Format("%.8g", P7SqzOpt->uX[P7SqzOpt->oStg[s].objIndexSI]);
			}
			else
			{
				StageGrid.colS[5][r + 2] = "No";
				StageGrid.colS[3][r + 2] = "0.05";
				StageGrid.colS[4][r + 2] = "1.5";
			}

			StageGrid.colS[0][r + 1].Format("%d", s + 1);
			StageGrid.colS[0][r + 2].Format("%d", s + 1);

			r++;

		}

		if (P7SqzOpt->oStg[s].TypeStg == 2)
		{
			StageGrid.colS[0][r + 1].Format("%d", s + 1);
			StageGrid.colS[1][r + 1] = "Overflush";

			Vol = P7SqzOpt->oStg[s].Time * P7SqzOpt->oStg[s].Qwt * 60;
			sItem.Format((Vol <= (1e6 - 1)) ? "%.4g" : "%.2e", Vol);
			sItem.Replace(_T("e+000"), _T("e"));
			sItem.Replace(_T("e+00"), _T("e"));
			sItem.Replace(_T("e+0"), _T("e"));
			StageGrid.colS[2][r + 1] = sItem;

			if (P7SqzOpt->oStg[s].objTime == 1)
			{
				StageGrid.colS[5][r + 1] = "Yes";
				StageGrid.colS[3][r + 1].Format("%.8g", P7SqzOpt->lX[P7SqzOpt->oStg[s].objIndexTime]);
				StageGrid.colS[4][r + 1].Format("%.8g", P7SqzOpt->uX[P7SqzOpt->oStg[s].objIndexTime]);
			}
			else
				StageGrid.colS[5][r + 1] = "No";


		}

		/*	if (P7SqzOpt->oStg[s].TypeStg == 5) // Shut-in
		{
		StageGrid.colS[0][r + 1].Format("%d", s + 1);
		StageGrid.colS[1][r + 1] = "Shut-in";
		sItem.Format((P7SqzOpt->oStg[s].Time <= (1e6 - 1)) ? "%.4g" : "%.2e", P7SqzOpt->oStg[s].Time);
		sItem.Replace(_T("e+000"), _T("e"));
		sItem.Replace(_T("e+00"), _T("e"));
		sItem.Replace(_T("e+0"), _T("e"));
		StageGrid.colS[2][r + 1] = sItem;

		if (P7SqzOpt->oStg[s].objTime == 1)
		{
		StageGrid.colS[5][r + 1] = "Yes";
		StageGrid.colS[3][r + 1].Format("%.8g", P7SqzOpt->lX[P7SqzOpt->oStg[s].objIndexTime]);
		StageGrid.colS[4][r + 1].Format("%.8g", P7SqzOpt->uX[P7SqzOpt->oStg[s].objIndexTime]);
		}
		else
		StageGrid.colS[5][r + 1] = "No";

		}*/

		s++;

	}

	UpdateControls();

	Invalidate(FALSE);
}

void PropPage7::UpdateControls()
{
	CString sn;

	// = 0, Time = 1 Water Volume Produced
	if (kPlot == 0 || kPlot == 2)
		sn = "Squeeze Lifetime Target Time (days)";

	if (kPlot == 1 || kPlot == 3)
	{
		if (Units == 0)
			sn = "Squeeze Lifetime Target Vol Prod Water (1000bbls)";
		if (Units == 1)
			sn = "Squeeze Lifetime Target Vol Prod Water (100m3)";
	}
	GetDlgItem(IDC_STATIC_T_V)->SetWindowText(sn);

	// = 0 Conventional, = 1 Metric
	if (Units == 0)
		sn = "Scale Inhibitor Cost (Neat Product), Currency/bbl";

	if (Units == 1)
		sn = "Scale Inhibitor Cost (Neat Product), Currency/m3";

	GetDlgItem(IDC_STATIC_SI_COST)->SetWindowText(sn);

	sn.Format("%.6g", P7SqzOpt->InjHourRate);
	GetDlgItem(IDC_EDIT_InjHourRate)->SetWindowText(sn);

	sn.Format("%.6g", P7SqzOpt->SIBblRate);
	GetDlgItem(IDC_EDIT_SICost)->SetWindowText(sn);

	sn.Format("%.6g", P7SqzOpt->MIC);
	GetDlgItem(IDC_EDIT_MIC)->SetWindowText(sn);

	sn.Format("%.6g", P7SqzOpt->Obj_T_V);
	GetDlgItem(IDC_EDIT_T_V)->SetWindowText(sn);

	sn.Format("%d", P7SqzOpt->nGen);
	GetDlgItem(IDC_EDIT_nGen)->SetWindowText(sn);

	sn.Format("%d", P7SqzOpt->nPopSize);
	GetDlgItem(IDC_EDIT_nPopSize)->SetWindowText(sn);

	sn.Format("%d", P7SqzOpt->BinsMT);
	GetDlgItem(IDC_EDIT_BinsMT)->SetWindowText(sn);

	sn.Format("%d", P7SqzOpt->IterationsSecant);
	GetDlgItem(IDC_EDIT_IterationsSecant)->SetWindowText(sn);

	sn.Format("%d", P7SqzOpt->nBins);
	GetDlgItem(IDC_EDIT_nBins)->SetWindowText(sn);

	// = 0 Conventional, = 1 Metric
	if (Units == 0)
		sn = "Total SI Volume, bbl";

	if (Units == 1)
		sn = "Total SI Volume, m3";

	GetDlgItem(IDC_CHECK_ONLY_SI_VOL)->SetWindowText(sn);
	sn.Format("%.6g", P7SqzOpt->TotSIVol);
	GetDlgItem(IDC_EDIT_SI_VOL)->SetWindowText(sn);

	// = 0 Conventional, = 1 Metric
	if (Units == 0)
		sn = "Total Inj Volume, bbl";

	if (Units == 1)
		sn = "Total Inj Volume, m3";

	GetDlgItem(IDC_CHECK_ONLY_TOT_VOL)->SetWindowText(sn);
	sn.Format("%.6g", P7SqzOpt->TotInjVol);
	GetDlgItem(IDC_EDIT_TOT_VOL)->SetWindowText(sn);

	m_Check_Op_Cons.SetCheck(P7SqzOpt->SensCalc ? BST_CHECKED : BST_UNCHECKED);
	m_Check_Only_SI_Vol.SetCheck(P7SqzOpt->Only_SI ? BST_CHECKED : BST_UNCHECKED);
	m_Check_Only_TOT_Vol.SetCheck(P7SqzOpt->Only_Tot_Vol ? BST_CHECKED : BST_UNCHECKED);
	m_Check_Fixed_Target.SetCheck(P7SqzOpt->Fixed_Target ? BST_CHECKED : BST_UNCHECKED);

	if (P7SqzOpt->Fixed_Target)
	{
		GetDlgItem(IDC_EDIT_nPopSize)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_nGen)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_BinsMT)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_IterationsSecant)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_nPopSize)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_nGen)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_BinsMT)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_IterationsSecant)->EnableWindow(0);
	}

	if (P7SqzOpt->SensCalc)
	{
		GetDlgItem(IDC_EDIT_SI_VOL)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_TOT_VOL)->EnableWindow(1);
		GetDlgItem(IDC_CHECK_ONLY_SI_VOL)->EnableWindow(1);
		GetDlgItem(IDC_CHECK_ONLY_TOT_VOL)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SI_VOL)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_TOT_VOL)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_ONLY_SI_VOL)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_ONLY_TOT_VOL)->EnableWindow(0);
		m_Check_Only_SI_Vol.SetCheck(BST_UNCHECKED);
		m_Check_Only_TOT_Vol.SetCheck(BST_UNCHECKED);
	}

}

void PropPage7::UpdateFromEdits()
{
	int nS = 0,
		nI = 0;

	for (int r = 1; r < nRows; r++)
	{
		if (StageGrid.colS[5][r] == "Yes")
		{
			nI++;
			P7SqzOpt->LabelNum.push_back(atoi(StageGrid.colS[0][r].GetBuffer()));
		}
	}

	P7SqzOpt->nPar = nI;

	if (P7SqzOpt->X.size() != nI)
	{
		P7SqzOpt->X.resize(nI);
		P7SqzOpt->lX.resize(nI);
		P7SqzOpt->uX.resize(nI);
		P7SqzOpt->Vol.resize(nI);
		P7SqzOpt->Label.resize(nI);
	}

	nI = 0;

	for (int r = 0; r < nRows - 1; r++)
	{
		if (StageGrid.colS[1][r + 1] == "Overflush")
		{
			if (StageGrid.colS[5][r + 1] == "Yes")
			{
				P7SqzOpt->oStg[nS].objTime = 1;
				P7SqzOpt->oStg[nS].objIndexTime = nI;

				P7SqzOpt->X[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]) / (P7SqzOpt->oStg[nS].Qwt * 60);
				P7SqzOpt->lX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);
				P7SqzOpt->uX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[4][r + 1]);
				P7SqzOpt->Vol[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				char intstr[3];
				//	itoa(nI + 1, intstr, 10);
				P7SqzOpt->Label[nI] = "OFVol" + StageGrid.colS[0][r + 1];

				nI++;
			}

			if (StageGrid.colS[5][r + 1] == "No")

				P7SqzOpt->oStg[nS].objTime = 0;

			nS++;
		}

		if (StageGrid.colS[1][r + 1] == "Shut-in")
		{
			if (StageGrid.colS[5][r + 1] == "Yes")
			{
				P7SqzOpt->oStg[nS].objTime = 1;
				P7SqzOpt->oStg[nS].objIndexTime = nI;

				P7SqzOpt->X[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				P7SqzOpt->lX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);
				P7SqzOpt->uX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[4][r + 1]);

				nI++;
			}

			if (StageGrid.colS[5][r + 1] == "No")

				P7SqzOpt->oStg[nS].objTime = 0;

			nS++;
		}

		if (StageGrid.colS[1][r + 1] == "Inj_Vol")
		{
			if (StageGrid.colS[5][r + 1] == "Yes")
			{
				P7SqzOpt->oStg[nS].objTime = 1;
				P7SqzOpt->oStg[nS].objIndexTime = nI;

				P7SqzOpt->X[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]) / (P7SqzOpt->oStg[nS].Qwt * 60);
				P7SqzOpt->lX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[3][r + 1]);
				P7SqzOpt->uX[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[4][r + 1]);
				P7SqzOpt->Vol[P7SqzOpt->oStg[nS].objIndexTime] = atof(StageGrid.colS[2][r + 1]);
				char intstr[3];
				//	itoa(P7SqzOpt->LabelNum[nI], intstr, 10);
				P7SqzOpt->Label[nI] = "InjVol" + StageGrid.colS[0][r + 1];
				nI++;
			}

			if (StageGrid.colS[5][r + 1] == "No")

				P7SqzOpt->oStg[nS].objTime = 0;
		}

		if (StageGrid.colS[1][r + 1] == "Inj_SI")
		{
			if (StageGrid.colS[5][r + 1] == "Yes")
			{
				P7SqzOpt->oStg[nS].objSI = 1;
				P7SqzOpt->oStg[nS].objIndexSI = nI;

				P7SqzOpt->X[P7SqzOpt->oStg[nS].objIndexSI] = atof(StageGrid.colS[2][r + 1]);
				P7SqzOpt->lX[P7SqzOpt->oStg[nS].objIndexSI] = atof(StageGrid.colS[3][r + 1]);
				P7SqzOpt->uX[P7SqzOpt->oStg[nS].objIndexSI] = atof(StageGrid.colS[4][r + 1]);
				char intstr[3];
				//	itoa(nI + 1, intstr, 10);
				P7SqzOpt->Label[nI] = "InjSI" + StageGrid.colS[0][r + 1];
				nI++;
			}

			if (StageGrid.colS[5][r + 1] == "No")

				P7SqzOpt->oStg[nS].objSI = 0;

			nS++;
		}


	}

	CString sn;

	GetDlgItem(IDC_EDIT_InjHourRate)->GetWindowTextA(sn);
	P7SqzOpt->InjHourRate = atof(sn);

	GetDlgItem(IDC_EDIT_SICost)->GetWindowTextA(sn);
	P7SqzOpt->SIBblRate = atof(sn);

	GetDlgItem(IDC_EDIT_MIC)->GetWindowTextA(sn);
	P7SqzOpt->MIC = atof(sn);

	GetDlgItem(IDC_EDIT_T_V)->GetWindowTextA(sn);
	P7SqzOpt->Obj_T_V = atof(sn);

	GetDlgItem(IDC_EDIT_nGen)->GetWindowTextA(sn);
	P7SqzOpt->nGen = atoi(sn);

	GetDlgItem(IDC_EDIT_nPopSize)->GetWindowTextA(sn);
	P7SqzOpt->nPopSize = atoi(sn);

	GetDlgItem(IDC_EDIT_BinsMT)->GetWindowTextA(sn);
	P7SqzOpt->BinsMT = atoi(sn);

	GetDlgItem(IDC_EDIT_IterationsSecant)->GetWindowTextA(sn);
	P7SqzOpt->IterationsSecant = atoi(sn);

	GetDlgItem(IDC_EDIT_nBins)->GetWindowTextA(sn);
	P7SqzOpt->nBins = atoi(sn);

	P7SqzOpt->SensCalc = (m_Check_Op_Cons.GetCheck() ? 1 : 0);
	P7SqzOpt->Only_SI = (m_Check_Only_SI_Vol.GetCheck() ? 1 : 0);
	P7SqzOpt->Only_Tot_Vol = (m_Check_Only_TOT_Vol.GetCheck() ? 1 : 0);
	P7SqzOpt->Fixed_Target = (m_Check_Fixed_Target.GetCheck() ? 1 : 0);

	GetDlgItem(IDC_EDIT_SI_VOL)->GetWindowTextA(sn);
	P7SqzOpt->TotSIVol = atof(sn);

	GetDlgItem(IDC_EDIT_TOT_VOL)->GetWindowTextA(sn);
	P7SqzOpt->TotInjVol = atof(sn);
}

BOOL PropPage7::OnSetActive()
{
	nCols = 6;
	nRows = 1;

	if (nStg != CSheet::ST.size())
	{
		nStg = CSheet::ST.size();
		P7SqzOpt->oStg.resize(nStg);
	}

	for (int s = 0; s < nStg; s++)
	{
		P7SqzOpt->oStg[s] = CSheet::ST[s];

		if (P7SqzOpt->oStg[s].TypeStg == 1) // Main Treatment
			nRows += 2;

		if (P7SqzOpt->oStg[s].TypeStg == 2) // Overflush
			nRows++;

		P7SqzOpt->oStg[s].TypeStg = CSheet::ST[s].TypeStg;
		P7SqzOpt->oStg[s].TimeStep = CSheet::ST[s].TimeStep;
		P7SqzOpt->oStg[s].Qwt = CSheet::ST[s].Qwt;
		P7SqzOpt->oStg[s].InitialCw = CSheet::ST[s].InitialCw;
		P7SqzOpt->oStg[s].objTime = CSheet::ST[s].objTime;
		P7SqzOpt->oStg[s].objIndexTime = CSheet::ST[s].objIndexTime;
		P7SqzOpt->oStg[s].objSI = CSheet::ST[s].objSI;
		P7SqzOpt->oStg[s].objIndexSI = CSheet::ST[s].objIndexSI;
	}

	kPlot = CSheet::kPlot;

	testgrid tempgrid(nRows, nCols);

	StageGrid = tempgrid;

	InitialiseGrid();

	return CPropertyPage::OnSetActive();
}

BOOL PropPage7::OnKillActive()
{
	UpdateFromEdits();

	if (CSheet::ST.size() != nStg)
		CSheet::ST.resize(nStg);

	for (int s = 0; s < nStg; s++)
	{
		CSheet::ST[s].objTime = P7SqzOpt->oStg[s].objTime;
		CSheet::ST[s].objIndexTime = P7SqzOpt->oStg[s].objIndexTime;
		CSheet::ST[s].objSI = P7SqzOpt->oStg[s].objSI;
		CSheet::ST[s].objIndexSI = P7SqzOpt->oStg[s].objIndexSI;
	}

	return CPropertyPage::OnKillActive();
}


void PropPage7::OnStnClickedStaticOptimisation2()
{
}


void PropPage7::OnEnChangeEdit4()
{
}


void PropPage7::OnEnChangeEditngen()
{
}

void PropPage7::OnEnChangeEditBinsmt()
{
}

void PropPage7::OnEnChangeEditSiVol()
{
}


void PropPage7::OnEnChangeEditInjhourrate()
{
}

void PropPage7::OnEnChangeEditIterationssecant()
{
}
void PropPage7::OnBnClickedRadio2()
{
}

void PropPage7::OnBnClickedCheckOnlySiVol()
{
}


void PropPage7::OnBnClickedCheckOnlyTotVol()
{
}



void PropPage7::OnBnClickedFixedTarget()
{
}



void PropPage7::OnBnClickedCheck2()
{
}



void PropPage7::OnBnClickedCheckFixedTarget()
{
	if (m_Check_Fixed_Target.GetCheck())
	{
		GetDlgItem(IDC_EDIT_nPopSize)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_nGen)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_BinsMT)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_IterationsSecant)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_nPopSize)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_nGen)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_BinsMT)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_IterationsSecant)->EnableWindow(0);
	}
}



void PropPage7::OnEnChangeEditnpopsize()
{
}



void PropPage7::OnEnChangeEditnbins()
{
}


void PropPage7::OnBnClickedCheckOperCons()
{
	if (m_Check_Op_Cons.GetCheck())
	{
		GetDlgItem(IDC_EDIT_SI_VOL)->EnableWindow(1);
		GetDlgItem(IDC_EDIT_TOT_VOL)->EnableWindow(1);
		GetDlgItem(IDC_CHECK_ONLY_SI_VOL)->EnableWindow(1);
		GetDlgItem(IDC_CHECK_ONLY_TOT_VOL)->EnableWindow(1);
	}
	else
	{
		GetDlgItem(IDC_EDIT_SI_VOL)->EnableWindow(0);
		GetDlgItem(IDC_EDIT_TOT_VOL)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_ONLY_SI_VOL)->EnableWindow(0);
		GetDlgItem(IDC_CHECK_ONLY_TOT_VOL)->EnableWindow(0);
		m_Check_Only_SI_Vol.SetCheck(BST_UNCHECKED);
		m_Check_Only_TOT_Vol.SetCheck(BST_UNCHECKED);
	}
}
