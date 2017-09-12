// CArchDlg  dialog used for search results in archive

#pragma once
#include "resource.h"
class CMyListCtrl;
class CMyCheckBox;
class CWebBrowser2;

class CArchDlg : public CDialog
{
private:
	CMyListCtrl *MyListCtrl;
	CMyCheckBox *BtnCheck;
	CButton *BtnCBLikeOrEq;
	CDateTimeCtrl *DTCtrl1;
	CDateTimeCtrl *DTCtrl2;
	CString m_StrName, m_StrCompany, m_StrExam, m_StrDate1, m_StrDate2;
	//
	CWebBrowser2 *m_ieWB; // IE
	void SetIEText();
	HRESULT SetHtmlTagText(PTCHAR tagName, PTCHAR value);
	//
	void CStringTo3IntArray(CString StrDate, int* i3);
	ULONG DateToLong(int* i3);
	ULONG CStringDateToLong(CString sDate);
	void LongToDate(ULONG j, int& day, int& month, int& year);
	CString LongDateToCString(ULONG j);
	//
	CString DBGetTextByMemberName(char* s);

public:
	CArchDlg();	// standard constructor
	~CArchDlg();
	//void SqlCreateOrInsert(char *sql);
	void SqlSelect(char *sql);
	void InsertColumnInList();
	void FillListView(int row, int col, PTCHAR text);
	//void InsertNew(int row);

// Dialog Data
	enum { IDD = IDD_ARCH1_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	// Generated message map functions
	afx_msg void OnSize(UINT nType,int cx,int cy);
	afx_msg void OnBtnArch1();
	DECLARE_MESSAGE_MAP()

public:
	DECLARE_EVENTSINK_MAP()
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
};
