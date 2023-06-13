// Isotherm.cpp : implementation file
//

#include "stdafx.h"
#include "OV Grid DLL.h"
#include "Isotherm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Isotherm dialog


Isotherm::Isotherm(CWnd* pParent /*=NULL*/)
	: CDialog(Isotherm::IDD, pParent)
{
	//{{AFX_DATA_INIT(Isotherm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bFileOpen = TRUE;
	bTab = TRUE;
	bRelPerm = FALSE;
	NIso = 0;
}


void Isotherm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Isotherm)
	DDX_Control(pDX, IDC_ISO_TABLE, TabGr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Isotherm, CDialog)
	//{{AFX_MSG_MAP(Isotherm)
	ON_BN_CLICKED(IDSAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Isotherm message handlers

void Isotherm::ReadFileTableIso(const char *File)
{
	FILE *filePtr;
	
	NameFile = File;

   char buffer[128];
	CString sItem;
	
	sItem.Format("%s","Isotherms\\");
	
	sItem += NameFile;

	if ( (filePtr = fopen(PathName + sItem,"r")) == NULL )
   {   
		sItem.Format( "The file %s was not opened.\nMake sure is in the right directory.", NameFile );
		AfxMessageBox(sItem);
		bFileOpen = FALSE;
	}
	else
   {  
		printf( "The file %s was opened\n", NameFile );
 	
		/* Set pointer to beginning of file: */
		fseek( filePtr, 0L, SEEK_SET );

		// validate the SQUEEZE data file and check the version
		fgets(buffer,128,filePtr);

		fscanf(filePtr,"%d \n",&NIso);
		
		fgets(buffer,128,filePtr);

		Conc = new double [NIso];

		Adso = new double [NIso];
		
		for( int i = 0 ; i < NIso ; i++)
			
			fscanf(filePtr,"%lf %lf \n",&Conc[i], &Adso[i] );

		TableGrid Temp(NIso+1, Conc, Adso);

		TabGr = Temp;
		
		fclose( filePtr );

		delete [] Conc;
		delete [] Adso;
	}
}

void Isotherm::ReadFieldFileTable(const char *File, int iModel)
{
	FILE *filePtr;
	
	NameFile = File;

   char buffer[128];
	CString sItem;

	if (iModel == 4)
	
		sItem.Format("%s","Lab Data\\");

	else
		
		sItem.Format("%s","Field Data\\");

	sItem += NameFile;

	if ( (filePtr = fopen(PathName + sItem,"r")) == NULL )
   {   
		sItem.Format( "The file %s was not opened.\nMake sure is in the right directory.", NameFile );
		AfxMessageBox(sItem);
		bFileOpen = FALSE;
	}
	else
   {  
		printf( "The file %s was opened\n", NameFile );
 	
		/* Set pointer to beginning of file: */
		fseek( filePtr, 0L, SEEK_SET );

		// validate the SQUEEZE data file and check the version
		fgets(buffer,128,filePtr);

		fscanf(filePtr,"%d \n",&NIso);
		
		fgets(buffer,128,filePtr);

		Time = new double [NIso];

		Conc = new double [NIso];
		
		for( int i = 0 ; i < NIso ; i++)
			
			fscanf(filePtr,"%lf %lf \n",&Time[i], &Conc[i] );

		TableGrid Temp(NIso+1, Time, Conc);

		TabGr = Temp;
		
		fclose( filePtr );
		
		delete [] Time;
		delete [] Conc;
	}
}

void Isotherm::ReadLabDataTable(const char *File)
{
	FILE *filePtr;
	
	NameFile = File;

   char buffer[128];
	CString sItem;
	
	sItem.Format("%s","Lab Data\\");
	
	sItem += NameFile;

	if ( (filePtr = fopen(PathName + sItem,"r")) == NULL )
   {   
		sItem.Format( "The file %s was not opened.\nMake sure is in the right directory.", NameFile );
		AfxMessageBox(sItem);
		bFileOpen = FALSE;
	}
	else
   {  
		printf( "The file %s was opened\n", NameFile );
 	
		/* Set pointer to beginning of file: */
		fseek( filePtr, 0L, SEEK_SET );

		// validate the SQUEEZE data file and check the version
		fgets(buffer,128,filePtr);

		fscanf(filePtr,"%d \n",&NIso);
		
		fgets(buffer,128,filePtr);

		Time = new double [NIso];

		Conc = new double [NIso];
		
		for( int i = 0 ; i < NIso ; i++)
			
			fscanf(filePtr,"%lf %lf \n",&Time[i], &Conc[i] );

		TableGrid Temp(NIso+1, Time, Conc);

		TabGr = Temp;
		
		fclose( filePtr );
		
		delete [] Time;
		delete [] Conc;
	}
}

void Isotherm::ReadFileTableRelPerm(const char *File)
{
	FILE *filePtr;
	
	NameFile = File;

   char buffer[128];
	CString sItem;
	
	sItem.Format("%s","Relative Permeability\\");
	
	sItem += NameFile;

	if ( (filePtr = fopen(PathName + sItem,"r")) == NULL )
   {   
		sItem.Format( "The file %s was not opened.\nMake sure is in the right directory.", NameFile );
		AfxMessageBox(sItem);
		bFileOpen = FALSE;
	}
	else
   {  
		printf( "The file %s was opened\n", NameFile );
 	
		/* Set pointer to beginning of file: */
		fseek( filePtr, 0L, SEEK_SET );

		// validate the SQUEEZE data file and check the version
		fgets(buffer,128,filePtr);

		fscanf(filePtr,"%d \n",&NIso);
		
		fgets(buffer,128,filePtr);

		Sw = new double [NIso];

		Krw = new double [NIso];

		Kro = new double [NIso];
		
		for( int i = 0 ; i < NIso ; i++)
			
			fscanf(filePtr,"%lf %lf %lf\n",&Sw[i], &Krw[i], &Kro[i] );

		TableGrid Temp(NIso+1, Sw, Krw, Kro);

		TabGr = Temp;
		
		fclose( filePtr );

		delete [] Sw;
		delete [] Krw;
		delete [] Kro;
	}
}

void Isotherm::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void Isotherm::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void Isotherm::OnSave() 
{
	// TODO: Add your control notification handler code here
	FILE* filePtr;  
	
	CString openFilter,
			  Root,
			  Ext;
	if ( bRelPerm ) // Relative Permeability
	{
		openFilter = "Relative Permeability (*.txt)|*.txt|All Files (*.*)|*.*||";
		Root = "Relative Permeability\\";
		Ext = "txt";
	}
	else
	{
		if ( bTab == TRUE ) // Isotherm 
		{	
			openFilter = "Adsorption Isotherm (*.txt)|*.txt|All Files (*.*)|*.*||";
			Root = "Isotherms\\";
			Ext = "txt";
		}
		
		if ( bTab == FALSE ) // File Data
		{	
			if ( bIsoDer == TRUE )
			{	
				openFilter = "Lab Data (*.txt)|*.txt|All Files (*.*)|*.*||";
				Root = "Lab Data\\";
				Ext = "txt";
			}
			else
			{	
				openFilter = "Field Data (*.txt)|*.txt|All Files (*.*)|*.*||";
				Root = "Field Data\\";
				Ext = "txt";	
			}
		}
	}

	CFileDialog saveDialog(FALSE, Ext, PathName+Root+NameFile, OFN_HIDEREADONLY|
		OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, openFilter, this);
	
	if( saveDialog.DoModal() == IDOK )
	{	
		CString sItem;
		
		NameFile = saveDialog.GetFileName();		
		
		if ( bRelPerm ) // Relative Permeability
			
			sItem.Format("%s","Relative Permeability\\");

		else
		{
			if ( bTab == TRUE ) // Isotherm 
				
				sItem.Format("%s","Isotherms\\");
			
			if ( bTab == FALSE ) // File Data
			{	
				
				if ( bIsoDer == TRUE )

					sItem.Format("%s","Lab Data\\");

				else
					
					sItem.Format("%s","Field Data\\");
			
			}

		}

		sItem = PathName + sItem + NameFile;

 		if ( (filePtr=fopen(sItem, "w")) == NULL)
		{
			printf("Couldn't write to this file.\n");
		}			 

		// output the text which the user has entered 

		fprintf( filePtr, "NIso\n"); // distinguishes between SQUEEZE III and SQUEEZE IV data files 
		fprintf( filePtr, "%d\n", NIso);
		
		if ( bRelPerm ) // Relative Permeability
		{
			fprintf( filePtr, "Sw		Krw		Kro\n");

			for( int i = 1 ; i <= NIso ; i++ )
				
				fprintf( filePtr, "%s\t\t%s \t\t%s\n", TabGr.colS[0][i], TabGr.colS[1][i], TabGr.colS[2][i]);

		}
		else
		{
			if ( bTab == TRUE ) // Isotherm 
				
				fprintf( filePtr, "Ciso(ppm)    ADiso(mg/L rock)\n");
			
			if ( bTab == FALSE ) // File Data

				fprintf( filePtr, "Time			Conc(ppm)\n");
			
			for( int i = 1 ; i <= NIso ; i++ )
			{	
				fprintf( filePtr, "%s\t\t%s \n", TabGr.colS[0][i], TabGr.colS[1][i]);
			}
		}

		fclose(filePtr);

	}
	
	CDialog::OnOK();
}


void Isotherm::SetPathName(const CString NameFile)
{
	PathName = NameFile;
}