// ez1.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ez1.h"
#include "MainFrm.h"

#include "ez1Doc.h"
#include "ez1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cez1App

BEGIN_MESSAGE_MAP(Cez1App, CWinApp)
	ON_COMMAND(IDM_APP_ABOUT, &Cez1App::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	//ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Cez1App construction

Cez1App::Cez1App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only Cez1App object

Cez1App theApp;


// Cez1App initialization

BOOL Cez1App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Cez1Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(Cez1View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}


//------------------------------------------------------------------------------
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
private:
	CFont *m_pFont; // шрифт
 	COLORREF m_textColor; // цвет
	bool fInto; // признак того, что указатель мыши находится в определенном месте

public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	afx_msg int  OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnMouseMove( UINT, CPoint );
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	m_pFont = 0;
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

//------------------------------------------------------------------------------
// App command to run the dialog
void Cez1App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
//------------------------------------------------------------------------------
int CAboutDlg::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	m_pFont = new CFont;
	// Установим синий RGB-цвет текста
 	m_textColor = RGB(0, 0, 255);
	return 0;
}
//------------------------------------------------------------------------------
void CAboutDlg::OnMouseMove(UINT Flags, CPoint Loc)
{
	HCURSOR hCursor;
	if((Loc.x > 0) && (Loc.x < 150) && (Loc.y > 0) && (Loc.y < 150))
	{
		hCursor = LoadCursor(0,IDC_HAND);
		SetCursor(hCursor);
		fInto = 1;
	}
	else
	{
		fInto = 0;
	}
}
//------------------------------------------------------------------------------
void CAboutDlg::OnPaint()
{
	CPaintDC paintDC(this);
	CRgn rgn;
	rgn.CreateRectRgn(0,0,500,150);
	CBrush brush;
	brush.CreateSolidBrush(RGB(255,255,255));
	paintDC.FillRgn(&rgn,&brush);
	COLORREF oldColor = paintDC.SetTextColor(m_textColor);
	paintDC.TextOut(200, 40, L"Экспресс зачет");
	oldColor = paintDC.SetTextColor(oldColor);
	paintDC.TextOut(200, 90, L"Версия 1.01 (DEMO)");
}
//------------------------------------------------------------------------------
void CAboutDlg::OnLButtonUp( UINT, CPoint )
{
	if (fInto) ShellExecute(this->m_hWnd,L"open",L"\"C:\\Program Files\\Internet Explorer\\iexplore.exe\"",L"mynv.ru",0,SW_SHOWNORMAL);
}
// Cexpzach1App message handlers
