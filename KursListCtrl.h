// CKursListCtrl  subclassing
#pragma once
#ifndef LOADKURSLISTCTRL
#define LOADKURSLISTCTRL

typedef std::pair<std::wstring,std::wstring> PAIRSTRSTR;
typedef std::list<PAIRSTRSTR> LISTPAIRSTRSTR;

class CKursListCtrl : public CListCtrl
{
private:
	PTCHAR dest;  // pointer to  �������� ���������� �����
	LISTPAIRSTRSTR *m_plpss;  // pointer to  ������ ��� (�������� ����� | ��� �����)

public:
	CKursListCtrl(PTCHAR str_ptr, LISTPAIRSTRSTR *plpss);

// Implementation
protected:
	afx_msg void OnLButtonDblClk( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CKursListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK( )
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CKursListCtrl::CKursListCtrl(PTCHAR str_ptr, LISTPAIRSTRSTR *plpss)
{
	dest = str_ptr; // pointer to  �������� ���������� �����
	m_plpss = plpss; // pointer to  ������ ��� (�������� ����� | ��� �����) 
}
//--------------------------------------------------------------------------------
void CKursListCtrl::OnLButtonDblClk( UINT nFlags,CPoint point )
{
	LVHITTESTINFO hti;
	memset(&hti, 0, sizeof(hti));
	hti.pt.x = point.x;
	hti.pt.y = point.y;
	SubItemHitTest(&hti);

	// onlySelected=true if the item clicked is selected, and no others are
	bool onlySelected = (GetItemState(hti.iItem, LVIS_SELECTED) != 0);
	int NumItems = GetItemCount();
	for (int item=0; item<NumItems; item++)
	{
		if (item!=hti.iItem && GetItemState(item, LVIS_SELECTED)!=0)
		{
			onlySelected = false;
		}
	}
	bool ctrlKeyDown = ((nFlags&MK_CONTROL) != 0);
	bool shiftKeyDown = ((nFlags&MK_SHIFT) != 0);

	if (onlySelected && !ctrlKeyDown && !shiftKeyDown)
	{
		// deselect all items
		SetItemState(-1, 0, LVIS_SELECTED);

		PTCHAR str1 = new TCHAR[MAX_PATH];
		if(str1)
		{
			GetItemText(hti.iItem,hti.iSubItem,str1,MAX_PATH-1);
			for(LISTPAIRSTRSTR::iterator itr = m_plpss->begin(),end=m_plpss->end(); itr!=end; ++itr)
			{
				if( !wcscmp(itr->second.c_str(),str1) )
				{
					// ����������� ��� ����� � ��������� ������
					// dest = CMainFrm::m_pKursFileName
					lstrcpy(dest,itr->first.c_str());
					CDialog *ParentDlg = (CDialog*)this->GetParent();
					// ��������� ������ ����� ������ �����
					CWnd *BtnWnd = ParentDlg->GetDlgItem(IDB_BUTTON1); // ������ ������� ID_CANCEL
					BtnWnd->SendMessage(BM_CLICK);
				}
			}
			delete [] str1;
		}
	}
	else
	{
		CListCtrl::OnLButtonDblClk(nFlags,point);
	}
}
//--------------------------------------------------------------------------------
#endif
