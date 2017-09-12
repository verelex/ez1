// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "ez1.h"
#include "MainFrm.h"
#include "KursListDialog.h"
#include "OptionsDialog.h"
#include "ResultDialog.h"
#include "ArchDialog.h"
#include <afxpriv.h> // ����� ��� ���������� AfxSetWindowText
#include <locale>

#define NOT_SELECTED 777 // �� ���� �� radio buttons �� �������!

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(IDM_OPTIONS, OnOptions)
	ON_COMMAND(IDM_NEWTEST, OnNewTest)
	ON_COMMAND(IDM_ARCH, OnArch)
	ON_COMMAND(ID_APP_1, &CMainFrame::OnApp1)
	ON_COMMAND(ID_APP_2, &CMainFrame::OnApp2)
	ON_COMMAND(ID_NEXTBTN, &CMainFrame::OnNext)
	ON_COMMAND(ID_HELPBTN, &CMainFrame::OnHelp)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};
//----------------------------------------------------------------------------
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	m_pRebar = 0;
	m_mode = 0; // �� ��������� ����� ��������������
	//memset(m_wndButtonsArray,0,sizeof(CButton)*9);
	m_pKursFileName = new TCHAR[MAX_PATH]; // �������� ����� ��� ���������� �����
	m_BiletNumber = 0; //������ ����� �� ���������
}
//----------------------------------------------------------------------------
CMainFrame::~CMainFrame()
{
	if(m_pKursFileName) delete [] m_pKursFileName;
}
//----------------------------------------------------------------------------
int CMainFrame::KursListDialog()
{
	CKursListDlg kurslistDlg(m_pKursFileName); // ���������� � ��������
	kurslistDlg.DoModal(&m_mode);
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::OnOptions()
{
	OptionsDialog();
}
//----------------------------------------------------------------------------
int CMainFrame::NewTestInvoke(LPCREATESTRUCT lpCreateStruct = 0)
{
	// �������� ������ ��������
	m_TestItemList.clear();
	// ����� �����
	lstrcpyW(m_pKursFileName, L"");
	KursListDialog(); // ����������� ��������: m_pKursFileName [��� �����(� ��������� ������) ��� ��������]

	SetMainTitle();
	// ���� ������ ��� ����������
	if (lpCreateStruct)
	{
		if (CFrameWnd::OnCreate(lpCreateStruct) == -1) return -1; // ������
		CreateControls();
	}

	//// ��������� �� XML ���� � ���������:   1. TestItemList
	//										  2. m_wndKursName (������ ���� �������! �� �������������)
	if(LoadXML_Kurs() == 1) return 0; // ������
	//
	m_CurQueNum = 0; // ��������� ����� �������
	if(m_mode) // ���� ������������ ���������� ����� ������ (� �������)
	{
		int MaxBiletNumber = (int)m_TestItemList.size()/5 - 1;
		m_BiletNumber = GetRandomNumber(MaxBiletNumber);
		m_CurQueNum = m_BiletNumber * 5; // ����� �������
	}
	//
	ShowTestItem(); // �������� ������
	//
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::OnNewTest()
{
	NewTestInvoke();
}
//----------------------------------------------------------------------------
int CMainFrame::OptionsDialog()
{
	COptionsDlg optionsDlg;
	optionsDlg.DoModal();
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::SetMainTitle() // ����� ��������� �������� ����
{
	PTCHAR tmp = new TCHAR[MAX_PATH*2];
	if(tmp)
	{
		lstrcpy(tmp,L"�������� ����� 1.0 - ");
		if(m_mode == 1) // ���� ����� ������������
		{
			lstrcat(tmp,L"����: ");
			CUtil1_Options utilopt;
			utilopt.LoadXML_Options(); // ��������� ������ � �����������
			lstrcat(tmp,utilopt.UserName.GetBuffer()); // ����� ������ ��� ������������
		}
		else // ����� ��������������
		{
			lstrcat(tmp,L"��������������");
		}
		CString UserCompany;
		// ��������
		UserCompany.Insert(0,L" - ��� \"���� � ������\""); // �������� �����������
		lstrcat(tmp,UserCompany.GetBuffer()); // = load from dongle
		MainTitle.Empty();
		MainTitle.Format(L"%s",tmp);
		OnUpdateFrameTitle(0);
		delete [] tmp;
	}
}
//----------------------------------------------------------------------------
BOOL CMainFrame::IsParity(int src) // ������ ��� ���
{
	if((src % 2) == 0) return TRUE;
	return FALSE;
}
//----------------------------------------------------------------------------
/*BOOL CMainFrame::IsEqual(LISTINT *s1, LISTINT *s2) // ������ ��������� ��������� ��� ���
{
	int ravno=0;
	size_t size=0;
	size = s1->size();
	if(size == s2->size()) // ������ ��������
	{
		//�������� ���� ������
		for(LISTINT::iterator iter1=s1->begin(),iter2=s2->begin(),end=s1->end(); iter1!=end; ++iter1,++iter2)
		{
			if(iter1._Mynode()->_Myval == iter2._Mynode()->_Myval) ravno++;
		}
		if(ravno == size) return TRUE;
	}
	return FALSE;
}*/
//----------------------------------------------------------------------------
int CMainFrame::GetRandomNumber(int MaxCount)
{
	//srand(GetTickCount()); // init random ------- ������� � OnCreate()
	return (int)( ( (double)rand() / (double) RAND_MAX) * MaxCount );
}
//----------------------------------------------------------------------------
int CMainFrame::ExchangeSequence(int MaxCount) // �������������
{
	LISTINT DefList; // ������ ������������� ��������
	LISTINT::iterator iter;
	// ��������� ������ ������������� ��������
	for(int i=0; i<MaxCount; i++)
	{
		DefList.push_back(i);
	}
	LISTINT RndList; // ������ ��������� ��������
	int RndInt;
	// ��������� ������ ��������� ��������
	for(int i=0,m=MaxCount-1; i<MaxCount; i++,m--)
	{
		int rnd = GetRandomNumber(m); // �������� ��������� ��������
		iter = DefList.begin(); // �� ������ ������ ������������� ��������
		std::advance(iter, rnd); // ����������� �������� �� ������
		RndInt = iter._Mynode()->_Myval; // ����� ��������
		DefList.erase(iter); // ������� ���� �� �������
		RndList.push_back(RndInt); // ������� � ������ ��������� ��������
	}
	// ������������ Radio Buttons
	int max = GetLastParity(MaxCount);
	for(int i=0,j=0; i<max/2; i++, j+=2)
	{
		ExchangeBands(GetValByIndex(&RndList,j),GetValByIndex(&RndList,j+1));
	}
	// ���� ����� ���������� ��������� �� ������, ��
	// ������ ��������� � ��������
	if(!IsParity(MaxCount))
	{
		ExchangeBands(GetValByIndex(&RndList,0),GetValByIndex(&RndList,MaxCount-1));
	}
	return 0;
}
//----------------------------------------------------------------------------
int CMainFrame::GetValByIndex(LISTINT *lst, int idx) // idx starts 0
{
	LISTINT::iterator iter = lst->begin(); // �� ������ ������
	std::advance(iter, idx); // ����������� �������� �� ������
	return iter._Mynode()->_Myval; // ����� ��������
}
//----------------------------------------------------------------------------
int CMainFrame::GetLastParity(int src) // ��������� ������
{
	if(src < 2)
	{
		MessageBox(L"GetLastParity(num < 2)");
		return -1;
	}
	if( (src % 2) == 0 ) // ���� ������� �� 2 ��� �������
	{
		return src;
	}
	else
	{
		return src-1;
	}
	return 2; // �����������
}
//----------------------------------------------------------------------------
int CMainFrame::CreateRadioButtons(int ButtonsCount)
{
	// create rebar
	if(m_pRebar == 0) m_pRebar = new CReBar();
	else return 2; // ��������� ������� ���������� �������� (��� ����������)

	BOOL b;
	if(m_pRebar)
	{
		b = m_pRebar->Create(this);
	}
	if(!b)
	{
		TRACE0("Failed to create rebar control\n");
		return 1;      // fail to create
	}

	//int ButtonsCount = 5; // ���������� ������ (���������� 5)
	if(!m_wndButtonsArray[0]) // ���� ������ �� �������
	{
		for(int i=0; i<ButtonsCount; i++) // ������� ������
		{
			// i - �� ������ �����
			PTCHAR tmp = new TCHAR[100];
			_itow_s(i,tmp,100,10);
			m_wndButtonsArray[i].Create(tmp,WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON|BS_MULTILINE,CRect(0, 0, 100, 40),this,0);
			delete tmp;
			m_pRebar->AddBar(&m_wndButtonsArray[i],0,0,RBBS_BREAK|RBBS_NOGRIPPER);
		}
		ExchangeSequence(ButtonsCount); // �������������
	}
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::DestroyRadioButtons(int ButtonsCount)
{
	if(m_wndButtonsArray[0])
	{
		for(int i=0; i<ButtonsCount; i++)
		{
			m_wndButtonsArray[i].DestroyWindow();
		}
	}
	if(m_pRebar)
	{
		m_pRebar->DestroyWindow();
		delete m_pRebar;
		m_pRebar = 0;
	}
	this->RecalcLayout();
}
//----------------------------------------------------------------------------
int CMainFrame::ExchangeBands(int first, int second)
{
	CReBarCtrl& refReBarCtrl = m_pRebar->GetReBarCtrl();
	int BandCount = (int) refReBarCtrl.GetBandCount();
	if(first==second || BandCount==0 || first>BandCount || second>BandCount) return 1;
	LPREBARBANDINFO prbbi1 = new REBARBANDINFO;
	LPREBARBANDINFO prbbi2 = new REBARBANDINFO;
	BOOL b;
	prbbi1->cbSize = prbbi2->cbSize = sizeof(REBARBANDINFO);
	prbbi1->fMask = prbbi2->fMask = RBBIM_CHILD;
	// get
	b = refReBarCtrl.GetBandInfo(first,prbbi1);
	b = refReBarCtrl.GetBandInfo(second,prbbi2);
	// set
	b = refReBarCtrl.SetBandInfo(first,prbbi2);
	b = refReBarCtrl.SetBandInfo(second,prbbi1);
	refReBarCtrl.ShowBand(first); // fix; �.�. �� ��������� ��������� ��������. ������?
	if(prbbi1) delete prbbi1;
	if(prbbi2) delete prbbi2;
	return 0;
}
//----------------------------------------------------------------------------
int CMainFrame::CreateControls()
{
	// create rebar
	if (!m_wndRebarTop.Create(this))
	{
		TRACE0("Failed to create TOP rebar\n");
		return -1;      // fail to create
	}
	// create edit for show kurs name
	if (!m_wndKursName.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY|WS_VSCROLL,CRect(0, 0, 100, 20),this,0))
	{
		TRACE0("Failed to create edit for show kurs name\n"); // fail to create
		return -1;      
	}
	// add edit to rebar
	m_wndRebarTop.AddBar(&m_wndKursName,0,0,RBBS_BREAK|RBBS_NOGRIPPER);
	//
	// create edit for show question number
	if (!m_wndQNumberEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY|WS_VSCROLL,CRect(0, 0, 100, 20),this,0))
	{
		TRACE0("Failed to create edit for show question number\n"); // fail to create
		return -1;      
	}
	// add edit to rebar
	m_wndRebarTop.AddBar(&m_wndQNumberEdit,0,0,RBBS_BREAK|RBBS_NOGRIPPER);
	//m_wndQNumberEdit.SetWindowTextW(L"����� �������");

	// create edit for question text
	if (!m_wndQTextEdit.Create(WS_CHILD|WS_VISIBLE|ES_MULTILINE|ES_READONLY|WS_VSCROLL,CRect(0, 0, 100, 60),this,0))
	{
		TRACE0("Failed to create edit for question text\n"); // fail to create
		return -1;      
	}
	// add edit to rebar
	m_wndRebarTop.AddBar(&m_wndQTextEdit,0,0,RBBS_BREAK|RBBS_NOGRIPPER);
	//m_wndQTextEdit.SetWindowTextW(L"����� �������");


	// create statusbar
	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	//toolbar 1
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//toolbar 2
	if (!m_wndToolBar2.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar2.LoadToolBar(IDR_TOOLBAR2))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar2);
	return 0;
}
//----------------------------------------------------------------------------
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	srand(GetTickCount()); // init random

	return NewTestInvoke(lpCreateStruct);
}
//----------------------------------------------------------------------------
int CMainFrame::ShowTestItem()
{
	CString VoprStr;
	CTestItemList::iterator iter = m_TestItemList.begin(); //�� ������
	if(m_mode) // ���� ������������
	{
		VoprStr.Format(L"�����: %d,   ������: %d",m_BiletNumber+1,m_CurQueNum-m_BiletNumber*5+1);
	}
	else // ��������������
	{
		VoprStr.Format(L"������: %d",m_CurQueNum+1);
	}
	//
	std::advance(iter, m_CurQueNum); // ����������� �������� �� ������
	// ����� �������
	m_wndQNumberEdit.SetWindowTextW(VoprStr);
	// ����� �������
	m_wndQTextEdit.SetWindowTextW(iter->first.c_str());
	// ����� ������
	// ??????????????????
	// �������� ������� CString?
	//
	// ����� �������
	int size = (int)iter->third.size(); // ���������� ��������� ������
	CreateRadioButtons(size);
	SetRadioButtonsTextAndHwnd();
	return size;
}
//----------------------------------------------------------------------------
int CMainFrame::HideTestItem()
{
	// ����� �������
	CTestItemList::iterator iter = m_TestItemList.begin(); //�� ������
	std::advance(iter, m_CurQueNum); // ����������� �������� �� ������
	int size = (int)iter->third.size(); // ���������� ��������� ������
	DestroyRadioButtons(size);
	return size;
}
//----------------------------------------------------------------------------
int CMainFrame::SetRadioButtonsTextAndHwnd()
{
	CTestItemList::iterator iter = m_TestItemList.begin(); //�� ������
	std::advance(iter, m_CurQueNum); // ����������� �������� �� ������
	int j=0;
	for(CAnswerList::iterator i=iter->third.begin(),end=iter->third.end(); i!=end; ++i)
	{
		// ����������� CAnswer.third ����� ������
		i->third = m_wndButtonsArray[j];
		// ����������� ����� ������
		m_wndButtonsArray[j].SetWindowTextW(i->first.c_str());
		j++;
	}
	return 0;
}
//----------------------------------------------------------------------------
int CMainFrame::CheckAnswer()
{
	int r = NOT_SELECTED; // ���� �� ���� radio button �� �������
	LRESULT state;
	CTestItemList::iterator iter = m_TestItemList.begin(); //�� ������
	std::advance(iter, m_CurQueNum); // ����������� �������� �� ������
	int AskedAnswerNumber = 0;
	for(CAnswerList::iterator i=iter->third.begin(),end=iter->third.end(); i!=end; ++i)
	{
		state = ::SendMessage(i->third,BM_GETSTATE,0,0);
		if ( state != BST_UNCHECKED ) // ���� ������� (������� ������) ������
		{
			r = 0; // �������� �����
			iter->fourth = AskedAnswerNumber; // ���������� ����� ���������� ������
			// ���� ������� ������������ ��� �������� ������ ����������, �� ���������
			if(i->second)
			{
				return 1; // ������ �����
			}
		}
		AskedAnswerNumber++;
	}
	return r;
}
//----------------------------------------------------------------------------
int CMainFrame::LoadXML_Kurs() // ��������� �� XML ���� � ��������� TestItemList
{
	CUtil1_Options utilopt;
	// init COM, create parser MSXML:
	HRESULT hr = CoInitialize(0);
	CComPtr<IXMLDOMDocument> pDoc;
	hr = pDoc.CoCreateInstance(__uuidof(DOMDocument));
	if (utilopt.CheckCOMResult(hr, pDoc.p, 1, L"Unable to create XML parser object"))
		return 1;
	
	// load XML document
	VARIANT_BOOL bSuccess = false;
	hr = pDoc->load(CComVariant(m_pKursFileName), &bSuccess);
	if (utilopt.CheckCOMResult(hr, (PVOID)-1, bSuccess, L"Unable to load XML document into the parser"))
		return 1;

	// select node (�������� �����)
	CComBSTR strKursName(L"my:field/my:curs");
	CComPtr<IXMLDOMNode> pKursName;
	hr = pDoc->selectSingleNode(strKursName,&pKursName);
	CComVariant retVal(VT_EMPTY);
	hr = pKursName->get_nodeTypedValue(&retVal);
	// ������������� � rebar -> edit
	m_wndKursName.SetWindowTextW((LPCTSTR)retVal.pbstrVal); // �������� �����

	// select list
	CComBSTR strQs(L"my:field/my:temicursov/my:temacursa/my:questions/my:question");
	CComPtr<IXMLDOMNodeList> pQsList;
	hr = pDoc->selectNodes(strQs,&pQsList); // list [question with answers]
	if (pDoc) pDoc.Release();

	// get list length
	long listLength = 0; // ���������� ��������
	hr = pQsList->get_length(&listLength);
	if (utilopt.CheckCOMResult(hr, (PVOID)-1, listLength, L"Unable to locate 'my:question' XML node list"))
		return 1;

	for(int idx = 0; idx < listLength; idx++) // �� ���� ����� <my:question> (��������)
	{
		CComPtr<IXMLDOMNode> pQ; // each question with answers
		hr = pQsList->get_item(idx,&pQ); // select node (q + child nodes)
		// get question child list
		CComPtr<IXMLDOMNodeList> pQChildList;
		hr = pQ->get_childNodes(&pQChildList); // ��� �������� ��� <my:question> ����
		//
		long childListLength = 0; // question child nodes count
		//get q child list length 
		hr = pQChildList->get_length(&childListLength);
		if (utilopt.CheckCOMResult(hr, (PVOID)-1, childListLength, L"Unable to locate 'my:question' child XML nodes list"))
			return 1;

		CTestItem TestItem; // ���� ������� ��������� ��������� ������� ������� � ��������
		for(int jdx = 0; jdx < childListLength; jdx++) // �� ���� �������� �����, � ������: <my:qtext>,<my:answer>,<my:qhelp>
		{
			CComPtr<IXMLDOMNode> pQChildNode; // each question child node
			hr = pQChildList->get_item(jdx,&pQChildNode); // select node (child nodes of q)

			BSTR XMLTagName; // ���������� ��� ���� (���)
			hr = pQChildNode->get_baseName(&XMLTagName);

			CComVariant retVal(VT_EMPTY);
			hr = pQChildNode->get_nodeTypedValue(&retVal);
			if (!wcscmp(XMLTagName,L"qtext"))
			{
				TestItem.first.insert(0,retVal.bstrVal);
			}
			if (!wcscmp(XMLTagName,L"qhelp"))
			{
				TestItem.second.insert(0,retVal.bstrVal);
			}
			if (!wcscmp(XMLTagName,L"answer"))
			{
				CAnswerList AnswerList; // ���� ������� [answer](���������)
				CAnswer Answer; // ���� ������� [astatus] � [atext]
				CComPtr<IXMLDOMNodeList> pQLastChildList; // ����, �������� ��� <my:answer>
				hr = pQChildNode->get_childNodes(&pQLastChildList);
				// �������� ���� (������ ���) [answer]
				for (int kdx=0; kdx<2; kdx++)
				{
					CComVariant varValue(VT_EMPTY);
					CComPtr<IXMLDOMNode> pQNodeEnd;
					hr = pQLastChildList->get_item(kdx,&pQNodeEnd);
					hr = pQNodeEnd->get_nodeTypedValue(&varValue);
					if (kdx == 0) // ������ ������� (������� ������������ - ������)
					{
						if (!wcscmp(varValue.bstrVal,L"���������� �����")) { Answer.second = 1; }
						else { Answer.second = 0; }
					}
					else // ������ ������� (������� ������ - ������)
					{
						Answer.first.insert(0,varValue.bstrVal);
					}
				}
				AnswerList.push_back(Answer); // ��������� � ������ �������
				copy_list(&TestItem.third,&AnswerList); // �������� ��������� ������ � ����� ����.
			}
		}
		m_TestItemList.push_back(TestItem); // ��������� TestItemList
	}
	//MessageBox(0,L"xex",0,0);
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::copy_list(CAnswerList *dest, CAnswerList *src)
{
	for(CAnswerList::iterator iter=src->begin(),end=src->end(); iter!=end; ++iter)
	{
		dest->push_back(iter._Mynode()->_Myval);
	}
}
//----------------------------------------------------------------------------
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}
//----------------------------------------------------------------------------
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
//----------------------------------------------------------------------------
// CMainFrame message handlers
void CMainFrame::OnApp1()
{
	CreateRadioButtons(5);
}
//----------------------------------------------------------------------------
void CMainFrame::OnApp2()
{
	DestroyRadioButtons(5);
}
//----------------------------------------------------------------------------
void CMainFrame::OnNext()
{
	switch( CheckAnswer() ) // if(CheckAnswer()==NOT_SELECTED) MessageBox(...);
	{
		case 0: //MessageBox(L"�����������!",0,MB_OK|MB_ICONERROR);
			break;

		case 1: //MessageBox(L"���������",L"OK",MB_OK|MB_ICONINFORMATION);
			break;

		case NOT_SELECTED: MessageBox(L"�� ���� ������� �� ������!",0,MB_OK|MB_ICONERROR);
			return; // ����� ������ �� ������
	}
	//
	HideTestItem();
	//
	//if( m_CurQueNum < (int)(m_TestItemList.size()-1) ) // -1 �.�. ��������� m_CurQueNum � ����
	if( GetPlayContinue() )
	{
		m_CurQueNum++; // ��������� ������
		ShowTestItem();
	}
	else
	{
		//MessageBox(L"����, ����� �����������");
		// TODO: ������ � ��
		// ������� ��� �������: �������� �����, ����� �������, ����� �������
		//m_wndKursName.SetWindowTextW(L""); // �������� ����� - ������� � ResultDialog() �.�. �������� ����� ���
		m_wndQNumberEdit.SetWindowTextW(L"");
		m_wndQTextEdit.SetWindowTextW(L"");
		//
		ResultDialog();
	}
}
//----------------------------------------------------------------------------
bool CMainFrame::GetPlayContinue()
{
	if(m_mode) // ������������
	{
		return ( m_CurQueNum < m_BiletNumber*5+4 ); // +4 �.�. ��������� m_BiletNumber � ����
	}
	else
	{
		return ( m_CurQueNum < (int)(m_TestItemList.size()-1) ); // -1 �.�. ��������� m_CurQueNum � ����
	}
}
//----------------------------------------------------------------------------
int CMainFrame::ResultDialog()
{
	CWorkContext wc;
	wc.m_pTIL = &m_TestItemList;
	wc.m_mode = m_mode;
	wc.m_BiletNumber = m_BiletNumber;
	wc.m_ErrorsCount = 0;
	m_wndKursName.GetWindowTextW(wc.m_KursName);
	m_wndKursName.SetWindowTextW(L"");
	// set date time
	CTime theTime;
	theTime = CTime::GetCurrentTime();
	_wsetlocale(LC_ALL, L"Russian");
	wc.m_DateTime = theTime.Format(L"%d %B %Y�. - %H:%M");
	wc.m_Date = DateToLong(theTime.GetDay(), theTime.GetMonth(), theTime.GetYear());
	wc.m_Time = theTime.Format(L"%H:%M");
	//
	CResultDlg resultDlg(&wc);
	resultDlg.DoModal(); // ���������� ���� � ������������
	return 0;
}
//----------------------------------------------------------------------------
void CMainFrame::OnHelp()
{
	MessageBox(L"Help");
}
//----------------------------------------------------------------------------
void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
	AfxSetWindowText(this->m_hWnd,MainTitle);
}
//----------------------------------------------------------------------------
void CMainFrame::OnArch()
{
	ArchDialog();
}
//----------------------------------------------------------------------------
int CMainFrame::ArchDialog()
{
	CArchDlg archDlg;
	archDlg.DoModal();
	return 0;
}
//  [8/26/2010 user]-----------------------------------------------------------
ULONG CMainFrame::DateToLong(int day, int month, int year)
{
	if(month>2)
	{
		month-=3;
	}
	else
	{
		month+=9;
		year--;
	}
	long c=year/100L;
	long ya=year-100L*c;

	return (146097L*c)/4L+(1461L*ya)/4L+(153L*month+2L)/5L+day+1721119L;
}
//-----------------------------------------------------------------------------
