// CKursListDlg  dialog used for Show KursLists
#pragma once
#include "KursListCtrl.h"
#include "Util1_Options.h"

class CKursListDlg : public CDialog, public CUtil1_Options
{
private:
	LISTPAIRSTRSTR lpss; // ������ ��� (�������� ����� | ��� �����)
	CKursListCtrl *m_KursListCtrl; // �������
	CToolTipCtrl* m_pToolTip; // ���������
	int *mode_ptr; // pointer to flag ����������/�������

public:
	CKursListDlg(PTCHAR pkfn);
	~CKursListDlg();
	INT_PTR DoModal(int* mode); // ����������
	void InsertColumnInList(); // ������������� �������� ������� (��������)
	void FillListView(int row, int col, PTCHAR text); // ��������� ������� (������)
	int LoadXML_KursList(); // ��������� �� XML ������ ��� (�������� ����� | ��� �����)
	int SetMode(int imode); // set flag ����������/������� [������������� CMainFrame::m_mode]
	void ChooseMode(); // �������� Radio Buttons (����� ������ ����� � ������� ������ ������ � ������ �� �������� ����� ������ �������)

// Dialog Data
	enum { IDD = IDD_LISTKURS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Implementation
protected:
	afx_msg void OnItemDrawNotify( NMHDR*, LRESULT* ); // ������ ������ �������
	afx_msg void OnBnClickedButton1(); // ��������� ������
	DECLARE_MESSAGE_MAP()
};

//--------------------------------------------------------------------------------
CKursListDlg::CKursListDlg(PTCHAR pkfn) : CDialog(CKursListDlg ::IDD)
{
	//KursName = str1;
	m_KursListCtrl = new CKursListCtrl(pkfn, &lpss);
	m_pToolTip = new CToolTipCtrl();
}
//--------------------------------------------------------------------------------
CKursListDlg::~CKursListDlg()
{
	if (m_KursListCtrl) delete m_KursListCtrl;
	if (m_pToolTip) delete m_pToolTip;
}
//--------------------------------------------------------------------------------
INT_PTR CKursListDlg::DoModal(int* pmode) // ��������� �� CMainFrame::mode
{
	mode_ptr = pmode;
	return CDialog::DoModal();
}
//--------------------------------------------------------------------------------
int CKursListDlg::SetMode(int imode)
{
	*mode_ptr = imode; // ������������� CMainFrame::mode
	return 0;
}
//--------------------------------------------------------------------------------
void CKursListDlg::ChooseMode() // ����� ������ ����� � ������� ������ ������ � ������ �� �������� ����� ������ �������
{
	CWnd *pWnd1 = CKursListDlg::GetDlgItem(IDC_RADIO1);
	CButton *pBtn1 = static_cast<CButton*>(pWnd1);
	if (pBtn1->GetCheck() == BST_CHECKED)
	{
		SetMode(0);
	}
	CWnd *pWnd2 = CKursListDlg::GetDlgItem(IDC_RADIO2);
	CButton *pBtn2 = static_cast<CButton*>(pWnd2);
	if (pBtn2->GetCheck() == BST_CHECKED)
	{
		SetMode(1);
	}
}
//--------------------------------------------------------------------------------
int CKursListDlg::LoadXML_KursList() // ��������� ������ ��� (�������� ����� | ��� �����)
{
	CoInitialize(0);
	CComPtr<IXMLDOMDocument> pDoc;
	HRESULT hr = pDoc.CoCreateInstance(__uuidof(DOMDocument));
	if (CheckCOMResult(hr, pDoc.p, 1, L"Unable to create XML parser object"))
		return 1;

	// load XML document
	VARIANT_BOOL bSuccess = false;
	hr = pDoc->load(CComVariant(L"kurslist.xml"), &bSuccess);
	if (CheckCOMResult(hr, (PVOID)-1, bSuccess, L"Unable to load XML document into the parser"))
		return 1;

	// list of curses
	CComBSTR strList(L"curses/curs");
	CComPtr<IXMLDOMNodeList> pList;
	hr = pDoc->selectNodes(strList,&pList); // list [question with answers]

	// get list length
	long listLength = 0; // ���������� ������
	hr = pList->get_length(&listLength);
	if (CheckCOMResult(hr, (PVOID)-1, listLength, L"Unable to locate 'curses/curs' XML node list"))
		return 1;

	for(int idx = 0; idx < listLength; idx++) // �� ���� ����� <curs>
	{
		CComPtr<IXMLDOMNode> pQ;
		hr = pList->get_item(idx,&pQ); // select node (kurs + child nodes)
		CComPtr<IXMLDOMNodeList> pQChildList;
		hr = pQ->get_childNodes(&pQChildList); // ��� �������� ��� <curses/curs> ����
		//
		long childListLength = 0; // kurs child nodes count
		//get kurs child list length 
		hr = pQChildList->get_length(&childListLength);
		if (CheckCOMResult(hr, (PVOID)-1, childListLength, L"Unable to locate 'curses/curs' child XML nodes list"))
			return 1;
		//
		PAIRSTRSTR pss; // ���� ������� ��������� ��������� ������� ����� <curs>
		for(int jdx = 0; jdx < childListLength; jdx++) // �� ���� �������� �����, � ������: <file>,<about>
		{
			CComPtr<IXMLDOMNode> pQChildNode; // each kurs child node
			hr = pQChildList->get_item(jdx,&pQChildNode); // select node (child nodes of <curs>)

			BSTR XMLTagName; // ���������� ��� ���� (���)
			hr = pQChildNode->get_baseName(&XMLTagName);

			CComVariant retVal(VT_EMPTY);
			hr = pQChildNode->get_nodeTypedValue(&retVal);
			if (!wcscmp(XMLTagName,L"file"))
			{
				pss.first.insert(0,retVal.bstrVal);
			}
			if (!wcscmp(XMLTagName,L"about"))
			{
				pss.second.insert(0,retVal.bstrVal);
			}
		}
		lpss.push_back(pss);
	}

	if (pDoc) pDoc.Release();
	return 0;
}
//--------------------------------------------------------------------------------
void CKursListDlg::InsertColumnInList()
{
	// set up columns
	LV_COLUMN lvC;
	memset(&lvC,0,sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column

	LPWSTR headers[] = { L"� ",L"�������� �����" };
	int headerSize[] = { 30, 400 };

	for (int header=0; header<2; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<wchar_t *>(headers[header]);
		m_KursListCtrl->InsertColumn(header,&lvC);
	}
	m_KursListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}
//--------------------------------------------------------------------------------
void CKursListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDD_LISTKURS_DLG,*m_KursListCtrl);
}
//--------------------------------------------------------------------------------
BOOL CKursListDlg::OnInitDialog()
{
	//����� ������ radio button (��������������)
	CWnd *pWnd1 = CKursListDlg::GetDlgItem(IDC_RADIO1);
	CButton *pBtn1 = static_cast<CButton*>(pWnd1);
	pBtn1->SetCheck(BST_CHECKED);
	//
	m_KursListCtrl->SubclassDlgItem(IDC_LIST_KURS,this);
	CDialog::OnInitDialog();
	CenterWindow();
	InsertColumnInList();
	LoadXML_KursList();
	// tooltip
	m_pToolTip->Create(this);
	m_pToolTip->AddTool(GetDlgItem(IDC_LIST_KURS),L"������� ���� ��� ������ �����");
	// insert items
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask=LVIF_TEXT;
	//
	PTCHAR buf = new TCHAR[32];
	if(buf)
	{
		int i=0;
		for(LISTPAIRSTRSTR::iterator itr = lpss.begin(),end=lpss.end(); itr!=end; ++itr)
		{
			_itow_s(i+1,buf,20,10);// ����� �������
			pitem.iItem = i; // row
			pitem.pszText = const_cast<wchar_t *>(buf);
			m_KursListCtrl->InsertItem(&pitem);
			// �������� �����
			FillListView(i,1,(const PTCHAR)itr._Mynode()->_Myval.second.c_str());
			++i;
		}
		delete [] buf;
	}
	return TRUE;
}
//--------------------------------------------------------------------------------
BOOL CKursListDlg::PreTranslateMessage(MSG* pMsg)
{
	if (NULL != m_pToolTip)
	{
		if(pMsg->message >= WM_MOUSEFIRST && pMsg->message <= WM_MOUSELAST)
			m_pToolTip->RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//--------------------------------------------------------------------------------
void CKursListDlg::FillListView(int row, int col, PTCHAR text)
{
	m_KursListCtrl->SetItemText( row, col, const_cast<wchar_t *>(text) );
}
//--------------------------------------------------------------------------------
void CKursListDlg::OnItemDrawNotify(NMHDR* pNmhdr, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pNcd = (NMLVCUSTOMDRAW*) pNmhdr;
	switch( pNcd->nmcd.dwDrawStage )
	{
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT:
			pNcd->clrText = RGB(0,0,0);
			if( pNcd->nmcd.dwItemSpec & 1)
				pNcd->clrTextBk = RGB(240,255,255);
			else 
				pNcd->clrTextBk = RGB(255,240,255);
			*pResult = CDRF_NEWFONT;
			break;
	}
}
//--------------------------------------------------------------------------------
void CKursListDlg::OnBnClickedButton1() // ��� ������ ������ �� �������������
{
/*	if(!wcscmp(KursName,L"empty"))
	{
		MessageBox(L"���� �� ������!");
	}
	else
	{*/
		ChooseMode();
		OnCancel();
	//}
}
//--------------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CKursListDlg , CDialog)
	//ON_COMMAND( IDOK, OnClickOK )
	ON_NOTIFY( NM_CUSTOMDRAW, IDC_LIST_KURS, OnItemDrawNotify )
	ON_BN_CLICKED(IDB_BUTTON1, &CKursListDlg::OnBnClickedButton1)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------
