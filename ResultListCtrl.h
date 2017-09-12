// CResultListCtrl  subclassing
#pragma once
#include "MyEdit.h"

//typedef std::pair<std::wstring,std::wstring> PAIRSTRSTR;
//typedef std::list<PAIRSTRSTR> LISTPAIRSTRSTR;

class CResultListCtrl : public CListCtrl
{
private:
	//PTCHAR dest;
	//LISTPAIRSTRSTR *m_plpss;
	//LISTPAIRSTRSTR::iterator  m_iter;
	CMyEdit* edit;
	int editRow, editCol;
	//int m_mode; // =0 (просмотр результатов), =1 (выбор курсов при загрузке)
	int *mode_ptr; // pointer to flag подготовка/экзамен

public:
	//CResultListCtrl(PTCHAR str, LISTPAIRSTRSTR *plpss);//, CVxParentDialog *dlg, int *ptr);
	CResultListCtrl();

// Implementation
protected:
	afx_msg void OnLButtonDblClk( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};

