// CResultDlg  dialog used for Show results

#pragma once
#include "ResultListCtrl.h"
#include "Util1_Options.h"
#include "webbrowser2.h"
#include <mshtml.h>
#include "WorkContext.h"
#include "resource.h"

class CResultDlg : public CDialog, public CUtil1_Options
{
private:
	// данные
	CResultListCtrl *m_pResultListCtrl;
	CWorkContext *pwc;
	CWebBrowser2 *m_pCtrlWeb; // IE

	// методы
	void InsertColumnInList();
	void FillListView(int row, int col, PTCHAR text);
	void GetPath(PTCHAR src_in, PTCHAR dest_out);
	PTCHAR strrpos(PTCHAR src, TCHAR cmp);
	HRESULT SetHtmlTagText(PTCHAR tagName, PTCHAR value);
	void SetIEText();
	void SetListViewText();
	void NavigateToDllResource();
	//
	void WriteToDatabase();
	void SqlCreateOrInsert(char* sql);
	void WriteTableEx(char* sql);
	void StrCatW2A(char* sql, PTCHAR adds, int lenW);

public:
	//CResultDlg();
	CResultDlg(CWorkContext *pWrkCtx);
	~CResultDlg();

// Dialog Data
	enum { IDD = IDD_RESULT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	//afx_msg void OnBtn1();
	DECLARE_MESSAGE_MAP()
public:
	void DocumentCompleteExplorer1(LPDISPATCH pDisp, VARIANT* URL);
	DECLARE_EVENTSINK_MAP()
};
