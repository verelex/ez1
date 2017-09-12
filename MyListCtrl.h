// CMyListCtrl  subclassing

#pragma once
#ifndef MYLISTCTRL
#define MYLISTCTRL

#include "webbrowser2.h"
#include "MyEdit.h"
#include <list>

typedef std::list< std::pair<std::wstring,std::wstring> > LISTPAIRSTRSTR;

class CMyListCtrl : public CListCtrl
{
private:
	PTCHAR dest;
	LISTPAIRSTRSTR *m_plpss;
	LISTPAIRSTRSTR::iterator  m_iter;
	CMyEdit* edit;
	int editRow, editCol;
	int m_mode; // =0 (просмотр результатов), =1 (выбор курсов при загрузке)
	BOOL *m_FlagPointer;
	HANDLE hData;
	CString m_PKID;
	void SetPKID(CString s);
	//
	CWebBrowser2 *m_ieBrowser; // IE
	void NavigateToDllResource();
	void GetPath(PTCHAR src_in, PTCHAR dest_out);
	PTCHAR strrpos(PTCHAR src, TCHAR cmp);
	// drag-drop
	CImageList* m_pDragImage;	//For creating and managing the drag-image
	BOOL		m_bDragging;	//T during a drag operation
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON

public:
	//CMyListCtrl(PTCHAR str, LISTPAIRSTRSTR *plpss);
	CMyListCtrl(CWebBrowser2 *pwb);
	void ExportDataToBuffer(char* buf);
	int GetDataSize();
	void SetFlagPointer(BOOL *pb) { m_FlagPointer = pb; };
	void SetHasChangesFlag();
	CString GetPKID(void);
	// make private!
	void ReportError();
	void DropItemOnList();

// Implementation
protected:
	afx_msg void OnBeginDrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg void OnLButtonDblClk( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};

#endif
