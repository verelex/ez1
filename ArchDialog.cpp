#include "StdAfx.h"
#include "ArchDialog.h"
#include "sqlite3.h"
#include "MyListCtrl.h"
#include "MyCheckBox.h"
#include <mshtml.h>

BEGIN_MESSAGE_MAP(CArchDlg , CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED( IDC_BUTTON_ARCH1, &CArchDlg::OnBtnArch1 )
END_MESSAGE_MAP()

//---------------------------------------------------------------------
BEGIN_EVENTSINK_MAP(CArchDlg, CDialog)
	ON_EVENT(CArchDlg, IDC_EXPLORER1, 259, CArchDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

//--------------------------------------------------------------------------------
CArchDlg::CArchDlg() : CDialog(CArchDlg::IDD)
{
	m_ieWB = new CWebBrowser2(); // first
	if (m_ieWB)
	{
		MyListCtrl = new CMyListCtrl(m_ieWB); // second
	} 
	else
	{
		MessageBoxExA("Error creating IE control");
	}
	
	DTCtrl1 = new CDateTimeCtrl();
	DTCtrl2 = new CDateTimeCtrl();
	BtnCheck = new CMyCheckBox(DTCtrl1,DTCtrl2);
	BtnCBLikeOrEq = new CButton();
}
//--------------------------------------------------------------------------------
CArchDlg::~CArchDlg()
{
	if (MyListCtrl) delete MyListCtrl;
	if (m_ieWB) delete m_ieWB;
	if (BtnCheck) delete BtnCheck;
	if (BtnCBLikeOrEq) delete BtnCBLikeOrEq;
	if (DTCtrl1) delete DTCtrl1;
	if (DTCtrl2) delete DTCtrl2;
}
//-----------------------------------------------------
void CArchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT_ARCH1,m_StrName);
	DDX_Text(pDX,IDC_EDIT_ARCH2,m_StrCompany);
	DDX_Text(pDX,IDC_COMBO_ARCH1,m_StrExam);
	DDX_Text(pDX,IDC_DATETIMEPICKER_ARCH1,m_StrDate1);
	DDX_Text(pDX,IDC_DATETIMEPICKER_ARCH2,m_StrDate2);
	DDX_Control(pDX, IDC_LIST_ARCH1, *MyListCtrl);
	DDX_Control(pDX, IDC_CHECK1, *BtnCheck);
	DDX_Control(pDX, IDC_CHECK2, *BtnCBLikeOrEq);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ARCH1, *DTCtrl1);
	DDX_Control(pDX, IDC_DATETIMEPICKER_ARCH2, *DTCtrl2);
	DDX_Control(pDX, IDC_EXPLORER1, *m_ieWB);
}
//-----------------------------------------------------
BOOL CArchDlg::OnInitDialog()
{
	MyListCtrl->SubclassDlgItem(IDC_LIST_ARCH1,this);
	BtnCheck->SubclassDlgItem(IDC_CHECK1,this);
	BtnCBLikeOrEq->SubclassDlgItem(IDC_CHECK2,this);
	DTCtrl1->SubclassDlgItem(IDC_DATETIMEPICKER_ARCH1,this);
	DTCtrl2->SubclassDlgItem(IDC_DATETIMEPICKER_ARCH2,this);
	m_ieWB->SubclassDlgItem(IDC_EXPLORER1,this);
	//
	CDialog::OnInitDialog();
	CenterWindow();
	InsertColumnInList();
	//
	//m_ieWB->Navigate(L"res://vxres1.dll/101", NULL, NULL, NULL,NULL);

	return TRUE;
}
//--------------------------------------------------------------------------------
void CArchDlg::OnBtnArch1() // SEARCH BUTTON
{
	UpdateData();
	BOOL b1 = FALSE;
	CString StrSQL;

	// определение установлен ли чекбокс "точное совпадение"
	b1 = ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck();

	MyListCtrl->DeleteAllItems();
	StrSQL.Insert(0,L"SELECT * FROM archive1 WHERE ");

	// определение доступности контролов задания даты
	CButton *mchb = (CButton*)GetDlgItem(IDC_CHECK1);
	int state1 = mchb->GetCheck();

	// если условия не заданы то поиск не производится т.е. без WHERE никак!
	if ( m_StrName.IsEmpty() && m_StrExam.IsEmpty() &&  m_StrCompany.IsEmpty() && !state1 )
		return;

	if (!m_StrName.IsEmpty()) // m_StrName задано
	{
		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"username = '");
		} 
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"username LIKE '%");
		}
		StrSQL.Insert(StrSQL.GetLength(),m_StrName.GetBuffer() );
		m_StrName.ReleaseBuffer();
		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"' ");
		}
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"%' ");
		}
	}

	if (!m_StrExam.IsEmpty()) // m_StrExam задано
	{
		if (!m_StrName.IsEmpty())
		{
			StrSQL.Insert(StrSQL.GetLength(),L"AND ");
		}

		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"kursname = '");
		} 
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"kursname LIKE '%");
		}
		StrSQL.Insert(StrSQL.GetLength(),m_StrExam.GetBuffer() );
		m_StrName.ReleaseBuffer();
		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"' ");
		} 
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"%' ");
		}
	}

	if (!m_StrCompany.IsEmpty()) // m_StrCompany задано
	{
		if (!m_StrName.IsEmpty() || !m_StrExam.IsEmpty())
		{
			StrSQL.Insert(StrSQL.GetLength(),L"AND ");
		}

		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"company = '");
		} 
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"company LIKE '%");
		}

		StrSQL.Insert(StrSQL.GetLength(),m_StrCompany.GetBuffer() );
		m_StrName.ReleaseBuffer();
		if (b1)
		{
			StrSQL.Insert(StrSQL.GetLength(),L"' ");
		} 
		else
		{
			StrSQL.Insert(StrSQL.GetLength(),L"%' ");
		}
	}

	//
	if (!m_StrDate1.IsEmpty() && (state1 == BST_CHECKED) ) // m_StrDate1 задано и не запрещено
	{
		if (!m_StrName.IsEmpty() || !m_StrExam.IsEmpty() || !m_StrCompany.IsEmpty())
		{
			StrSQL.Insert(StrSQL.GetLength(),L"AND ");
		}
		//-------
		// Q: зачем преобразовывать дату строкой в число?
		// A: так как BETWEEN применяется только с числами!
		//
		int date1[3], date2[3];
		CStringTo3IntArray(m_StrDate1, date1);
		CStringTo3IntArray(m_StrDate2, date2);
		ULONG ud1 = DateToLong(date1);
		ULONG ud2 = DateToLong(date2);
		//
		CString tmp;
		StrSQL.Insert(StrSQL.GetLength(),L"date1 BETWEEN ");
		tmp.Format(L"%d", ud1);
		StrSQL.Insert(StrSQL.GetLength(),tmp.GetBuffer());
		tmp.ReleaseBuffer();
		StrSQL.Insert(StrSQL.GetLength(),L" AND ");
		tmp.Format(L"%d", ud2);
		StrSQL.Insert(StrSQL.GetLength(),tmp.GetBuffer());
		tmp.ReleaseBuffer();
		tmp.Empty();
	}

	StrSQL.Insert(StrSQL.GetLength(),L";");
	LPTSTR pBuf = StrSQL.GetBuffer();
	StrSQL.ReleaseBuffer();
	int lenW = StrSQL.GetLength();
	char* str1 = new char[lenW+10];
	if (str1)
	{
		memset(str1,0,lenW+10);
		WideCharToMultiByte(CP_ACP, 0, pBuf, -1, str1, lenW, 0, 0);
		//MessageBoxA(0,str1,0,0);
		SqlSelect(str1);
		delete [] str1;
	}
}
//-----------------------------------------------------------------------
// Преобразование григорианской даты в порядковый номер
// дня юлианского периода
ULONG CArchDlg::DateToLong(int *i3)
//ULONG CArchDlg::DateToLong(int day, int month, int year)
{
	if(i3[1]>2)
	{
		i3[1]-=3;
	}
	else
	{
		i3[1]+=9;
		i3[2]--;
	}
	long c=i3[2]/100L;
	long ya=i3[2]-100L*c;

	return (146097L*c)/4L+(1461L*ya)/4L+(153L*i3[1]+2L)/5L+i3[0]+1721119L;
}
//-----------------------------------------------------------------------
// Преобразование порядкового номера дня юлианского периода
// в григорианскую дату
void CArchDlg::LongToDate(ULONG j, int& day, int& month, int& year)
{
	ULONG d,m,y;
	j-=1721119L;
	y=(4L*j-1L)/146097L;
	d=(4L*j-1L-146097L*y)/4L;
	j=(4L*d+3L)/1461L;
	d=(4L*d+7L-1461L*j)/4L;
	m=(5L*d-3L)/153L;
	d=(5L*d+2L-153L*m)/5L;
	y=100L*y+j;
	if (m<10)
	{
		m+=3;
	}
	else
	{
		m-=9; y++;
	}
	month=m; year=y; day=d;
}
//-----------------------------------------------------------------------------
CString CArchDlg::LongDateToCString(ULONG j)
{
	int day, month, year;
	LongToDate(j, day, month, year);
	CString s1;
	s1.Format(L"%.2d.%.2d.%.4d", day, month, year); // ex: "18.05.2010"
	return s1;
}
//--------------------------------------------------------------
void CArchDlg::CStringTo3IntArray(CString StrDate, int* i3)
{
	PTCHAR pbuf = new TCHAR[10];
	if (pbuf)
	{
		memset(pbuf,0,10);
		lstrcpyn(pbuf,StrDate.GetBuffer(),3); // почему 3 (на 1 больше)? хз
		i3[0] = _wtoi(pbuf);
		memset(pbuf,0,10);
		lstrcpyn(pbuf,StrDate.GetBuffer()+3,3);
		i3[1] = _wtoi(pbuf);
		memset(pbuf,0,10);
		lstrcpyn(pbuf,StrDate.GetBuffer()+6,5);
		i3[2] = _wtoi(pbuf);
		delete [] pbuf;
	}
}
//-----------------------------------------------------
HRESULT CArchDlg::SetHtmlTagText(PTCHAR tagName, PTCHAR value)
{
	HRESULT hr = E_FAIL;
	CComQIPtr<IHTMLDocument2> pDoc;
	CComQIPtr<IDispatch> pDisp;
	pDisp = m_ieWB->GetDocument();
	if(pDisp)
	{
		hr = pDisp->QueryInterface(IID_IHTMLDocument2, (void**)&pDoc);
		if(pDoc)
		{
			CComQIPtr<IHTMLElementCollection> pAll;
			hr = pDoc->get_all(&pAll);
			if (pAll)
			{
				CComQIPtr<IDispatch> pDsp;
				CComVariant v1(tagName);
				CComVariant v2;
				hr = pAll->item(v1, v2, &pDsp);
				CComQIPtr<IHTMLElement> m_pHtmlList;
				m_pHtmlList = pDsp;
				CComBSTR bsHtml = value;
				if (m_pHtmlList)
				{
					hr = m_pHtmlList->put_innerHTML(bsHtml);
				}
			}
		}
	}
	return hr;
}
//-----------------------------------------------------
void CArchDlg::SetIEText()
{
	PTCHAR arr1[] = { L"user_fio1", L"user_fio2", L"user_company", L"user_jobtitle", L"kursname", L"biletnum", L"test_date", L"test_time", L"max_err_cnt", L"rep_block_mdata", L"curr_err_cnt", L"result", L"admin_jobtitle", L"admin_fio" };
	char *arr2[] =  {  "username",   "username",   "company",       "userjobpost",    "kursname",  "biletnum",  "date1",      "time1",      "dopusk",       "databytes",        "userdop",       "res",     "admjobpost",      "admname"   };

	for (int i=0; i<14; i++)
	{
		SetHtmlTagText( arr1[i], DBGetTextByMemberName( arr2[i] ).GetBuffer() );
	}
}
//  [8/18/2010 user] -----------------------------------------------------------
ULONG CArchDlg::CStringDateToLong(CString sDate)
{
	int i3[3];
	CStringTo3IntArray(sDate, i3);
	return DateToLong(i3);
}
//--------------------------------------------------------------------------------
void CArchDlg::InsertColumnInList()
{
	// set up columns
	LV_COLUMN lvC;
	memset(&lvC,0,sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column

	LPWSTR headers[] = { L"#", L"Орг", L"ФИО",L"Должность",L"Курс",L"Билет",L"Дата",L"Время",L"Допуск",L"Ошибок",L"Итог",L"Админ",L"Должн. админа",L"табл" };
	int headerSize[] = { 30,      100,    100,          50,    100,      20,     30,      30,       50,       20,     50,     100,              50,    20  };

	for (int header=0; header<14; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<wchar_t *>(headers[header]);
		MyListCtrl->InsertColumn(header,&lvC);
	}
	MyListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}
//--------------------------------------------------------------------------------
void CArchDlg::FillListView(int row, int col, PTCHAR text)
{
	MyListCtrl->SetItemText( row, col, const_cast<wchar_t *>(text) );
}
//--------------------------------------------------------------------------------
/*void CArchDlg::InsertNew(int row)
{
	LV_ITEM pitem;
	memset(&pitem, 0, sizeof(LV_ITEM));
	pitem.mask=LVIF_TEXT;

	PTCHAR bufi = new TCHAR[MAX_PATH];
	if (bufi)
	{
		pitem.iItem = row; // row
		_itow_s(row+1,bufi,20,10);// номер строкой
		pitem.pszText = bufi;
		MyListCtrl->InsertItem(&pitem);

		FillListView(row,1,L"-");
		FillListView(row,2,L"-");
		FillListView(row,3,L"-");
		delete [] bufi;
	}
}*/
//--------------------------------------------------------------------------------
/*void CArchDlg::SqlCreateOrInsert(char *sql)
{
	sqlite3 *db;
	sqlite3_stmt * pStmt;

	// SQL инструкции
	MessageBoxExA(sql);

	// открытие (создание) БД
	// TODO: возможно лучше передавать в параметрах указатель на структуру в 
	// котрой помимо прочего будет содержаться название БД
	if (sqlite3_open("demo1.db", &db))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	// шаг 1 подготовка SQL инструкции к выполнению 
	//(компиляция ее в байт-код)
	if (sqlite3_prepare_v2(db, sql, -1, &pStmt, NULL))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_finalize(pStmt);
		sqlite3_close(db);
		return;
	}

	// шаг выполнения
	int rc = sqlite3_step(pStmt);

	// получение сообщения при ошибке
	if (rc != SQLITE_DONE)
		MessageBoxExA(sqlite3_errmsg(db));

	// шаг 4 завершение выполнения запроса
	sqlite3_finalize(pStmt);

	// закрытие БД
	sqlite3_close(db);
}*/
//--------------------------------------------------------------------------------
void CArchDlg::SqlSelect(char *sql)
{
	sqlite3 *db;
	sqlite3_stmt * pStmt;
	int coln, rc;
	ULONG uld;
	CString date1;

	// SQL инструкции
	//MessageBoxExA(sql);

	// открытие (создание) БД
	// TODO: возможно лучше передавать в параметрах указатель на структуру в 
	// котрой помимо прочего будет содержаться название БД (pDbOpenContext)
	if (sqlite3_open("demo1.db", &db))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}
	// выполнение SQL инструкций
	// шаг 1 подготовка SQL инструкции к выполнению 
	//(компиляция ее в байт-код)
	if (sqlite3_prepare_v2(db, sql, -1, &pStmt, NULL))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_finalize(pStmt);
		sqlite3_close(db);
		return;
	}
	// получения количества столбцов
	//coln = sqlite3_column_count(pStmt);

	//len = ...
	//sqlite3_bind_blob(pStmt, 1, (const void*)pdata, len, SQLITE_STATIC);
	// шаг 2 выполнение SQL инструкций
	while((rc = sqlite3_step(pStmt)) == SQLITE_ROW)
	{
		// тело цилка выполнится только для 
		// инструкции SELECT
		// для остальных запросов функция 
		// sqlite3_step вернет SQLITE_DONE
		//
		// получить число столбцов в текущей строке 
		// результата
		coln = sqlite3_data_count(pStmt);

		//
		LV_ITEM pitem;
		memset(&pitem, 0, sizeof(LV_ITEM));
		pitem.mask=LVIF_TEXT;
		pitem.pszText = L"?";
		MyListCtrl->InsertItem(&pitem);

		// шаг 3 получение данных
		for(int j=0; j<coln; j++)
		{
			// получение данных как текстовые строки
			// вне зависимости от объявленного типа
			// столбца (кроме INTEGER)!!!
			switch (sqlite3_column_type(pStmt, j))
			{
			case SQLITE_INTEGER: //MessageBoxExA("Save BLOB");
				uld = (ULONG)sqlite3_column_int64(pStmt, j);
				if ( j == 0 )
				{
					PTCHAR tp1 = new TCHAR[20];
					if (tp1)
					{
						_itow_s((int)uld, tp1, 15, 10);
						FillListView(0,j,tp1);
						delete [] tp1;
					}
				}
				else // primary key не преобразуем в дату
				{
					
					date1 = LongDateToCString(uld);
					FillListView(0,j,date1.GetBuffer());
					date1.ReleaseBuffer();
				}
				break;
			/*case SQLITE_BLOB: //MessageBoxExA("Save BLOB");
				cnt = sqlite3_column_bytes(pStmt, j);
				//SaveBlobToFile((BYTE*)sqlite3_column_blob(pStmt, j), cnt);
				//
				break;*/
			default: //MessageBoxExA((LPCSTR)sqlite3_column_text(pStmt, j));
				PTCHAR temp = new TCHAR[MAX_PATH];
				char* tmp;
				if (temp)
				{
					tmp = (char*)sqlite3_column_text(pStmt, j);
					MultiByteToWideChar(CP_ACP, 0, tmp, strlen(tmp)+1, temp, MAX_PATH);
					FillListView(0,j,temp);
					delete [] temp;
				}
				break;
			}
		}
	}
	// получение сообщения при ошибке
	if (rc != SQLITE_DONE)
		MessageBoxExA(sqlite3_errmsg(db));
	// шаг 4 завершение выполнения запроса
	sqlite3_finalize(pStmt);
	// закрытие БД
	sqlite3_close(db);
}
//  [8/20/2010 user]------------------------------------------------------
CString CArchDlg::DBGetTextByMemberName(char* MemberName)
{
	sqlite3 *db;
	sqlite3_stmt * pStmt;
	int coln, rc;
	ULONG uld1;
	CString cstrg1;

	// SQL инструкции
	char *sql_part1 = "SELECT ";
	char *sql_part2 = " FROM archive1 WHERE id = ";
	char sql[MAX_PATH];
	memset(sql,0,MAX_PATH);
	lstrcpyA(sql,sql_part1);
	lstrcatA(sql,MemberName);
	lstrcatA(sql,sql_part2);
	char *tmp1 = new char[MAX_PATH];
	if (tmp1)
	{
		memset(tmp1,0,MAX_PATH);
		int len = MyListCtrl->GetPKID().GetLength();
		WideCharToMultiByte(CP_ACP,0,MyListCtrl->GetPKID().GetBuffer(),len,tmp1,MAX_PATH,0,0);
		lstrcatA(sql,tmp1);
		delete [] tmp1;
	}

	// открытие (создание) БД
	// TODO: возможно лучше передавать в параметрах указатель на структуру в 
	// котрой помимо прочего будет содержаться название БД (pDbOpenContext)
	if (sqlite3_open("demo1.db", &db))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_close(db);
		return 0;
	}
	// выполнение SQL инструкций
	// шаг 1 подготовка SQL инструкции к выполнению 
	//(компиляция ее в байт-код)
	if (sqlite3_prepare_v2(db, sql, -1, &pStmt, NULL))
	{
		MessageBoxExA(sqlite3_errmsg(db));
		sqlite3_finalize(pStmt);
		sqlite3_close(db);
		return 0;
	}
	// получения количества столбцов
	//coln = sqlite3_column_count(pStmt);

	//len = ...
	//sqlite3_bind_blob(pStmt, 1, (const void*)pdata, len, SQLITE_STATIC);
	// шаг 2 выполнение SQL инструкций
	while((rc = sqlite3_step(pStmt)) == SQLITE_ROW)
	{
		// тело цилка выполнится только для 
		// инструкции SELECT
		// для остальных запросов функция 
		// sqlite3_step вернет SQLITE_DONE
		//
		// получить число столбцов в текущей строке 
		// результата
		coln = sqlite3_data_count(pStmt);

		// шаг 3 получение данных
		for(int j=0; j<coln; j++)
		{
			// получение данных как текстовые строки
			// вне зависимости от объявленного типа
			// столбца (кроме даты)
			if ( strcmp(MemberName,"date1") == 0 )
			{
				uld1 = (ULONG)sqlite3_column_int64(pStmt, j);
				cstrg1 = LongDateToCString(uld1);
			} 
			else
			{
				switch (sqlite3_column_type(pStmt, j))
				{
				default: //MessageBoxExA((LPCSTR)sqlite3_column_text(pStmt, j));
					cstrg1 = sqlite3_column_text(pStmt, j);
					break;
				}
			}
		}
	}
	// получение сообщения при ошибке
	if (rc != SQLITE_DONE)
		MessageBoxExA(sqlite3_errmsg(db));
	// шаг 4 завершение выполнения запроса
	sqlite3_finalize(pStmt);
	// закрытие БД
	sqlite3_close(db);
	//
	return cstrg1;
}
//  [8/25/2010 user]--------------------------------------------------------------
void CArchDlg::OnSize(UINT nType,int cx,int cy)
{
	CDialog::OnSize(nType,cx,cy);

	// Resize the list ctrl
	int cxl = cx - 25;
	if (MyListCtrl->m_hWnd) MyListCtrl->SetWindowPos(0,0,0,cxl,150,SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
	// Resize the ie web ctrl
	if (m_ieWB->m_hWnd) m_ieWB->SetWindowPos(0,0,219,cxl,cy-370,SWP_NOMOVE|SWP_NOZORDER|SWP_SHOWWINDOW);
}
//  [8/25/2010 user]---------------------------------------------------------
void CArchDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	SetIEText();
}
//
