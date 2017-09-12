// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "CopyLessEdit.h"
#include "quad_template.h"
typedef std::list<int> LISTINT; // ��� ������������� Radio Buttons

class CMainFrame : public CFrameWnd
{
private:
	// ������:
	int CreateRadioButtons(int ButtonsCount); // ������� ������ (�������������)
	void DestroyRadioButtons(int ButtonsCount); // ������ ������
	int GetRandomNumber(int MaxCount); // �������� ��������� ����� �� 0 �� MaxCount
	int GetLastParity(int src); // �������� ��������� ������
	int GetValByIndex(LISTINT *lst, int idx); // �������� �������� ������ �� �������
	int ExchangeBands(int first, int second); // ������������ ������� ���� ������ RB
	int ExchangeSequence(int MaxCount);  // ������������� ������ RB
	//BOOL IsEqual(LISTINT *s1, LISTINT *s2); // ������ ��������� ��������� ��� ���
	BOOL IsParity(int src); // ������ ��� ���
	void SetMainTitle(); // ����� ��������� �������� ����   -- �������� !!!!!!!!!!!!!!
	int KursListDialog(); // ��������� ������ ������ ����� � ������ ������
	int OptionsDialog(); // ������ ����� (������� ��� � ���������� ������������ � ��������������)
	int ArchDialog(); // ������ ������ � ������ ������� ������
	int CreateControls(); // ������� ��� �������� �� ����������� ������ Radio buttons
	int LoadXML_Kurs(); // ��������� �� XML ���� � ��������� TestItemList
	void copy_list(CAnswerList *dest, CAnswerList *src); // �������� ��������� ������ � ����� ����.
	int ShowTestItem(); // ������� Radio buttons � �������� �������� �������
	int SetRadioButtonsTextAndHwnd(); // ������ ����� ������ ������� � ��������� ������ � ������
	int CheckAnswer(); // �������� ������������ ������
	int HideTestItem(); // ������ Radio buttons
	int ResultDialog(); // ������ ������ ����������� � ������ ����������� HTML script
	void OnUpdateFrameTitle(BOOL bAddToTitle); // ���������, � ������� �� ������������ �� �������� ��������� (untitled)
	bool GetPlayContinue(); // ���������� ������� ��������� ����� ������ ��������
	int NewTestInvoke(LPCREATESTRUCT lpCreateStruct); // ������ �����
	ULONG DateToLong(int day, int month, int year); // ���� �������������
	//
	// ������:
	CTestItemList m_TestItemList; // ������ �������� � �������
	int m_mode; // ����� ������: 0 = ��������������, 1 = ������������
	PTCHAR m_pKursFileName; // �������� ����� ��� ���������� ����� (��������: "ex0.xml")
	int m_CurQueNum; // ������� ����� ������� � ����
	CString MainTitle; // ��������� ����
	int m_BiletNumber; // ����� ������ � 0
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


