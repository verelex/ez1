// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "CopyLessEdit.h"
#include "quad_template.h"
typedef std::list<int> LISTINT; // дл€ перемешивани€ Radio Buttons

class CMainFrame : public CFrameWnd
{
private:
	// методы:
	int CreateRadioButtons(int ButtonsCount); // создать кнопки (переключатели)
	void DestroyRadioButtons(int ButtonsCount); // убрать кнопки
	int GetRandomNumber(int MaxCount); // получить случайное число от 0 до MaxCount
	int GetLastParity(int src); // получить последнее чЄтное
	int GetValByIndex(LISTINT *lst, int idx); // получить значение списка по индексу
	int ExchangeBands(int first, int second); // перестановка позиций двух кнопок RB
	int ExchangeSequence(int MaxCount);  // перемешивание кнопок RB
	//BOOL IsEqual(LISTINT *s1, LISTINT *s2); // списки полностью совпадают или нет
	BOOL IsParity(int src); // чЄтное или нет
	void SetMainTitle(); // “екст заголовка главного окна   -- доделать !!!!!!!!!!!!!!
	int KursListDialog(); // Ќачальный диалог выбора курса и режима работы
	int OptionsDialog(); // диалог опций (задание ‘»ќ и должностей тестируемого и ответственного)
	int ArchDialog(); // диалог поиска в архиве сданных тестов
	int CreateControls(); // создает все контролы за исключением кнопок Radio buttons
	int LoadXML_Kurs(); // загружает из XML курс и заполн€ет TestItemList
	void copy_list(CAnswerList *dest, CAnswerList *src); //  опируем локальные данные в более глоб.
	int ShowTestItem(); // создать Radio buttons и показать варианты ответов
	int SetRadioButtonsTextAndHwnd(); // задать текст кнопок ответов и присвоить хэндлы в списке
	int CheckAnswer(); // проверка правильности ответа
	int HideTestItem(); // скрыть Radio buttons
	int ResultDialog(); // ƒиалог вывода результатов и печати посредством HTML script
	void OnUpdateFrameTitle(BOOL bAddToTitle); // заголовок, в котором не учавствовало бы название документа (untitled)
	bool GetPlayContinue(); // определ€ет условие окончани€ цикла показа вопросов
	int NewTestInvoke(LPCREATESTRUCT lpCreateStruct); // запуск теста
	ULONG DateToLong(int day, int month, int year); // дата григорианска€
	//
	// данные:
	CTestItemList m_TestItemList; // список вопросов и ответов
	int m_mode; // –ежим работы: 0 = самоподготовка, 1 = тестирование
	PTCHAR m_pKursFileName; // Ќазвание файла дл€ выбранного курса (например: "ex0.xml")
	int m_CurQueNum; // текущий номер вопроса с нул€
	CString MainTitle; // «аголовок окна
	int m_BiletNumber; // номер билета с 0
	//

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CReBar      *m_pRebar, m_wndRebarTop;
	CCopyLessEdit 	m_wndKursName, m_wndQNumberEdit, m_wndQTextEdit;
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar, m_wndToolBar2;
	CButton     m_wndButtonsArray[9];

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnOptions(); // menu Options
	afx_msg void OnNewTest();
	afx_msg void OnArch();
	afx_msg void OnApp1();
	afx_msg void OnApp2();
	afx_msg void OnNext();
	afx_msg void OnHelp();
	DECLARE_MESSAGE_MAP()
};


