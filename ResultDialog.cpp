#include "StdAfx.h"
#include "ResultDialog.h"
#include "sqlite3.h"

BEGIN_MESSAGE_MAP(CResultDlg , CDialog)
	//ON_COMMAND( IDC_BUTTON1,  OnBtn1 )
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CResultDlg, CDialog)
	ON_EVENT(CResultDlg, IDC_EXPLORER1, 259, CResultDlg::DocumentCompleteExplorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

//--------------------------------------------------------------------------------
CResultDlg::CResultDlg(CWorkContext *pWrkCtx) : CDialog(CResultDlg ::IDD), CUtil1_Options()
{
	pwc = pWrkCtx;
	//
	m_pResultListCtrl = new CResultListCtrl();
	m_pCtrlWeb = new CWebBrowser2();
}
//--------------------------------------------------------------------------------
CResultDlg::~CResultDlg()
{
	if (m_pResultListCtrl) delete m_pResultListCtrl;
	if (m_pCtrlWeb) delete m_pCtrlWeb;
}
//--------------------------------------------------------------------------------
void CResultDlg::InsertColumnInList()
{
	// set up columns
	LV_COLUMN lvC;
	memset(&lvC,0,sizeof(LV_COLUMN));
	lvC.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvC.fmt = LVCFMT_LEFT;  // left-align the column

	LPWSTR headers[] = { L"№ ",L"Вопрос",L"Ответ",L"Результат" };
	int headerSize[] = { 30, 200, 200, 150 };
		
	for (int header=0; header<4; header++)
	{
		lvC.iSubItem = header;
		lvC.cx = headerSize[header]; // width of the column, in pixels
		lvC.pszText = const_cast<wchar_t *>(headers[header]);
		m_pResultListCtrl->InsertColumn(header,&lvC);
	}
	m_pResultListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
}
//-----------------------------------------------------
void CResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RESULT, *m_pResultListCtrl);
	DDX_Control(pDX, IDC_EXPLORER1, *m_pCtrlWeb);
}
//-----------------------------------------------------
PTCHAR CResultDlg::strrpos(PTCHAR src, TCHAR cmp)
{
	int len = lstrlen(src);
	for(int i=len; i>0; i--)
	{
		if(src[i] == cmp) return src+i;
	}
	return 0;
}
//-----------------------------------------------------
void CResultDlg::GetPath(PTCHAR src_in, PTCHAR dest_out)
{
	PTCHAR tmp = strrpos(src_in,'\\');
	if(tmp)
	{
		int len1 = lstrlen(src_in);
		int len2 = lstrlen(tmp);
		lstrcpyn(dest_out, src_in, len1 - len2 + 2); // +2 for including trailing slash + '\0'
	}
	else
		MessageBox(_T("GetPath() : символ '\\' не найден"));
}
//-----------------------------------------------------
HRESULT CResultDlg::SetHtmlTagText(PTCHAR tagName, PTCHAR value)
{
	HRESULT hr = E_FAIL;
	CComQIPtr<IHTMLDocument2> pDoc;
	CComQIPtr<IDispatch> pDisp;
	pDisp = m_pCtrlWeb->GetDocument();
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
void CResultDlg::SetIEText()
{
	PTCHAR name = new TCHAR[MAX_PATH];
	PTCHAR buf = new TCHAR[MAX_PATH];
	if (name && buf)
	{
		// set question text:
		for(int i=0; i<5; i++)
		{
			_itow_s(i+1,buf,20,10);// номер вопроса
			lstrcpy(name,_T("q_text"));
			lstrcat(name,buf);
			//item = row, subitem = col
			m_pResultListCtrl->GetItemText(i, 1, buf, MAX_PATH-1);
			SetHtmlTagText(name,buf);
		}
		// set answer text:
		for(int i=0; i<5; i++)
		{
			_itow_s(i+1,buf,20,10);// номер вопроса
			lstrcpy(name,_T("user_answ_text"));
			lstrcat(name,buf);
			//item = row, subitem = col
			m_pResultListCtrl->GetItemText(i, 2, buf, MAX_PATH-1);
			SetHtmlTagText(name,buf);
		}
		// set answer status:
		for(int i=0; i<5; i++)
		{
			_itow_s(i+1,buf,20,10);// номер вопроса
			lstrcpy(name,_T("answ_status"));
			lstrcat(name,buf);
			//item = row, subitem = col
			m_pResultListCtrl->GetItemText(i, 3, buf, MAX_PATH-1);
			SetHtmlTagText(name,buf);
		}
		// это надо делать в MainFrm когда создается диалог результ???????????????
		pwc->m_UserName = UserName;
		pwc->m_UserJobPos = UserJobTitle;
		pwc->m_ErrorsOK = 5;
		//pwc->m_Res = L"Сдано"; // ниже
		pwc->m_AdminName = AdminName;
		pwc->m_AdminJobPos = AdminJobTitle;
		// set admin & user info
		SetHtmlTagText( _T("user_fio1"),UserName.GetBuffer() );
		SetHtmlTagText( _T("user_fio2"),UserName.GetBuffer() );
		SetHtmlTagText( _T("user_jobtitle"),UserJobTitle.GetBuffer() );
		SetHtmlTagText( _T("admin_fio"),AdminName.GetBuffer() );
		SetHtmlTagText( _T("admin_jobtitle"),AdminJobTitle.GetBuffer() );
		SetHtmlTagText( _T("user_company"),pwc->m_UserCompany.GetBuffer() );
		SetHtmlTagText( _T("test_datetime"),pwc->m_DateTime.GetBuffer() );
		SetHtmlTagText( _T("kursname"),pwc->m_KursName.GetBuffer() );
		_itow_s(pwc->m_BiletNumber+1,buf,20,10);// номер билета
		SetHtmlTagText( _T("biletnum"), buf );
		SetHtmlTagText( _T("max_err_cnt"), L" 1 " ); // fix in future
		_itow_s(pwc->m_ErrorsCount,buf,20,10);
		SetHtmlTagText( _T("curr_err_cnt"), buf );
		// исправить т.к. вопросов может быть > 5
		if(pwc->m_ErrorsCount > 1)
		{
			lstrcpyn(buf,L"НЕ СДАНО",10);
		}
		else
		{
			lstrcpyn(buf,L"СДАНО",10);
		}
		SetHtmlTagText( _T("result"), buf );
		pwc->m_Res.Format(L"%s",buf);

		delete [] name;
		delete [] buf;
	}
}
//-----------------------------------------------------
BOOL CResultDlg::OnInitDialog()
{
	m_pResultListCtrl->SubclassDlgItem(IDC_LIST_RESULT,this);
	m_pCtrlWeb->SubclassDlgItem(IDC_EXPLORER1,this);
	//
	CDialog::OnInitDialog();
	CenterWindow();
	InsertColumnInList();

	// load admin & user info
	int r = LoadXML_Options();

	// таблица результатов
	SetListViewText();

	// ie navigate to resource html
	NavigateToDllResource();

	if (pwc->m_mode == 1) // если режим тестирования
	{
		// заполняем данные в html
		SetIEText();

		// запись в БД
		WriteToDatabase();
	}

	return TRUE;
}
//--------------------------------------------------------------------------------
void CResultDlg::FillListView(int row, int col, PTCHAR text)
{
	m_pResultListCtrl->SetItemText( row, col, const_cast<wchar_t *>(text) );
}
//--------------------------------------------------------------------------------
void CResultDlg::DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	SetIEText();
}
//--------------------------------------------------------------------------------
void CResultDlg::NavigateToDllResource()
{
	PTCHAR tmp1 = new TCHAR[MAX_PATH*2];
	PTCHAR tmp2 = new TCHAR[MAX_PATH*2];
	if(tmp1 && tmp2)
	{
		GetModuleFileName(0,tmp1,MAX_PATH);
		GetPath(tmp1, tmp2);
		if (pwc->m_mode == 1) // если режим тестирования
		{
			lstrcat(tmp2,_T("vxres1.dll/101"));
		}
		else // (m_mode == 0) режим самоподготовки
		{
			lstrcat(tmp2,_T("vxres1.dll/102"));
		}
		lstrcpy(tmp1,_T("res://"));
		lstrcat(tmp1,tmp2);
		//MessageBox(tmp1);
		m_pCtrlWeb->Navigate(tmp1, NULL, NULL, NULL,NULL);
		delete tmp1;
		delete tmp2;
	}
}
//--------------------------------------------------------------------------------
void CResultDlg::SetListViewText()
{
	// сколько вопросов показывать
	int MaxValueToShow = 0;
	if(pwc->m_mode == 0) // подготовка
	{
		MaxValueToShow = (int)pwc->m_pTIL->size(); // все вопросы
	}
	else // (m_mode == 1) // тестирование
	{
		MaxValueToShow = 5; // только 5 вопросов
	}
	
	PTCHAR buf = new TCHAR[32];
	if(buf)
	{
		LV_ITEM pitem; // insert items
		memset(&pitem, 0, sizeof(LV_ITEM));
		pitem.mask=LVIF_TEXT;

		int i = 0;
		// текст вопроса
		CTestItemList::iterator iter = pwc->m_pTIL->begin(); //на начало
		std::advance(iter, pwc->m_BiletNumber*5); // передвигаем итератор на индекс
		for(CTestItemList::iterator end=pwc->m_pTIL->end(); (iter!=end) && (i<MaxValueToShow); ++iter,i++)
		{
			// нумерация
			_itow_s(i+1,buf,20,10);// номер вопроса
			pitem.iItem = i; // row
			pitem.pszText = const_cast<wchar_t *>(buf);
			m_pResultListCtrl->InsertItem(&pitem);

			// текст вопроса
			FillListView(i,1,(const PTCHAR)iter->first.c_str()); // Текст вопроса
			
			// текст выбранного ответа
			CAnswerList::iterator answ_iter = iter->third.begin();
			std::advance(answ_iter, iter->fourth); // передвигаем итератор на индекс
			FillListView(i,2,(const PTCHAR)answ_iter->first.c_str()); // Текст выбранного ответа

			// вердикт
			if (answ_iter->second == 1)
			{
				lstrcpyn(buf,L"Правильный ответ",20);
			}
			else
			{
				lstrcpyn(buf,L"Неправильный ответ",20);
				pwc->m_ErrorsCount ++;
			}
			FillListView(i,3,buf);
		}
		delete [] buf;
	}
}
//--------------------------------------------------------------------------------
void CResultDlg::WriteToDatabase()
{
	char* qe1 = "INSERT INTO archive1 VALUES(NULL,'";
	char* qe2 = "','";
	char* qe3 = "');";
	char* sql = new char[4096];
	if (sql)
	{
		memset(sql,0,520);
		strcpy(sql,qe1);

		// организация
		PTCHAR tmp = pwc->m_UserCompany.GetBuffer(); pwc->m_UserCompany.ReleaseBuffer();
		int len = pwc->m_UserCompany.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// ФИО юзера
		tmp = pwc->m_UserName.GetBuffer(); pwc->m_UserName.ReleaseBuffer();
		len = pwc->m_UserName.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// должность юзера
		tmp = pwc->m_UserJobPos.GetBuffer(); pwc->m_UserJobPos.ReleaseBuffer();
		len = pwc->m_UserJobPos.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// название курса
		tmp = pwc->m_KursName.GetBuffer(); pwc->m_KursName.ReleaseBuffer();
		len = pwc->m_KursName.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// номер билета
		CString tmp1;
		tmp1.Format(L"%d",pwc->m_BiletNumber);
		tmp = tmp1.GetBuffer(); tmp1.ReleaseBuffer();
		len = tmp1.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// дата
		tmp1.Format(L"%d",pwc->m_Date);
		tmp = tmp1.GetBuffer(); tmp1.ReleaseBuffer();
		len = tmp1.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// время
		tmp = pwc->m_Time.GetBuffer(); pwc->m_Time.ReleaseBuffer();
		len = pwc->m_Time.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// допуск
		tmp1.Format(L"%d",pwc->m_ErrorsOK);
		tmp = tmp1.GetBuffer(); tmp1.ReleaseBuffer();
		len = tmp1.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// кол-во ошибок юзера
		tmp1.Format(L"%d",pwc->m_ErrorsCount);
		tmp = tmp1.GetBuffer(); tmp1.ReleaseBuffer();
		len = tmp1.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// результат
		tmp = pwc->m_Res.GetBuffer(); pwc->m_Res.ReleaseBuffer();
		len = pwc->m_Res.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// ФИО админа
		tmp = pwc->m_AdminName.GetBuffer(); pwc->m_AdminName.ReleaseBuffer();
		len = pwc->m_AdminName.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// должность админа
		tmp = pwc->m_AdminJobPos.GetBuffer(); pwc->m_AdminJobPos.ReleaseBuffer();
		len = pwc->m_AdminJobPos.GetLength();
		StrCatW2A(sql,tmp,len);
		strcat(sql,qe2);

		// таблица
		strcat(sql,"<TABLE borderColor=black cellSpacing=0 cellPadding=4 width=100% border=1><TBODY><TR><TD align=center><B>№ </B></TD><TD align=center><B>Вопрос </B></TD><TD align=center><B>Ответ </B></TD><TD align=center><B>Результат </B></TD></TR>");
		WriteTableEx(sql);
		strcat(sql,"</TBODY></TABLE>");
		strcat(sql,qe3);
		//
		SqlCreateOrInsert(sql);
		//
		delete [] sql;
	}
}
//  [8/26/2010 user]-----------------------------------------------------------
void CResultDlg::StrCatW2A(char* sql, PTCHAR adds, int lenW)
{
	char* str1 = new char[lenW+10];
	if (str1)
	{
		memset(str1,0,lenW+10);
		WideCharToMultiByte(CP_ACP, 0, adds, -1, str1, lenW, 0, 0);
		strcat(sql,str1);
		delete [] str1;
	}
}
//--------------------------------------------------------------------------------
void CResultDlg::SqlCreateOrInsert(char *sql)
{
	sqlite3 *db;
	sqlite3_stmt * pStmt;

	// SQL инструкции
	//MessageBoxExA(sql);

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
}
//  [8/26/2010 user]--------------------------------------------------
void CResultDlg::WriteTableEx(char* sql)
{
	// сколько вопросов показывать
	int MaxValueToShow = 0;
	if(pwc->m_mode == 0) // подготовка
	{
		MaxValueToShow = (int)pwc->m_pTIL->size(); // все вопросы
	}
	else // (m_mode == 1) // тестирование
	{
		MaxValueToShow = 5; // только 5 вопросов
	}
	//
	int i = 0;
	// текст вопроса
	CTestItemList::iterator iter = pwc->m_pTIL->begin(); //на начало
	std::advance(iter, pwc->m_BiletNumber*5); // передвигаем итератор на индекс
	for(CTestItemList::iterator end=pwc->m_pTIL->end(); (iter!=end) && (i<MaxValueToShow); ++iter,i++)
	{
		// нумерация
		CString tmp;
		tmp.Format(L"<TR><TD class=tdCenterBody>%d</TD>",i+1); // номер вопроса
		PTCHAR pt1 = tmp.GetBuffer(); tmp.ReleaseBuffer();
		int len = tmp.GetLength();
		StrCatW2A(sql,pt1,len);

		// текст вопроса
		PTCHAR ptc = (PTCHAR)iter->first.c_str(); // Текст вопроса
		int lenW = iter->first.length();
		char* str1 = new char[lenW+10];
		if (str1)
		{
			memset(str1,0,lenW+10);
			WideCharToMultiByte(CP_ACP, 0, ptc, -1, str1, lenW, 0, 0);
			//MessageBoxA(0,str1,0,0);
			strcat(sql,"<TD class=tdCenterBody>");
			strcat(sql,str1);
			strcat(sql,"</TD>");
			delete [] str1;
		}

		// текст выбранного ответа
		CAnswerList::iterator answ_iter = iter->third.begin();
		std::advance(answ_iter, iter->fourth); // передвигаем итератор на индекс
		PTCHAR pta = (PTCHAR)answ_iter->first.c_str(); // Текст выбранного ответа
		lenW = answ_iter->first.length();
		char* str2 = new char[lenW+10];
		if (str2)
		{
			memset(str2,0,lenW+10);
			WideCharToMultiByte(CP_ACP, 0, pta, -1, str2, lenW, 0, 0);
			strcat(sql,"<TD class=tdCenterBody>");
			strcat(sql,str2);
			strcat(sql,"</TD>");
			delete [] str2;
		}

		// вердикт
		strcat(sql,"<TD class=tdCenterBody>");
		if (answ_iter->second == 1)
		{
			strcat(sql,"Правильный ответ");
		}
		else
		{
			strcat(sql,"Неправильный ответ");
		}
		strcat(sql,"</TD></TR>");
	}
}
//-----------------------------------------------------------------------
