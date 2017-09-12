// CResultListCtrl  subclassing
#pragma once
#include "stdafx.h"
#include "ResultListCtrl.h"
#include "MyEdit.h"

BEGIN_MESSAGE_MAP(CResultListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK( )
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
CResultListCtrl::CResultListCtrl() : CListCtrl()
{
//	m_mode = 0;
}
//--------------------------------------------------------------------------------
/*CResultListCtrl::CResultListCtrl(PTCHAR str, LISTPAIRSTRSTR *plpss)
{
	//m_mode = 1;
	//dest = str;
	//m_plpss = plpss;
}*/
//--------------------------------------------------------------------------------
void CResultListCtrl::OnLButtonDblClk( UINT nFlags,CPoint point )
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

			// get the bounding box for the field to be edited
			CRect bounds;
			memset(&bounds, 0, sizeof(bounds));
			GetSubItemRect(hti.iItem, hti.iSubItem, LVIR_LABEL, bounds);

			// create the edit box
			edit = new CMyEdit;
			bounds.right = bounds.left+200;
			bounds.bottom = bounds.top+200;
			edit->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,bounds,this,0);
			
			// set edit box text to what that field's value is
			PTCHAR str1 = new TCHAR[MAX_PATH];
			GetItemText(hti.iItem,hti.iSubItem,str1,MAX_PATH-1);
			edit->SetWindowTextW(str1);
			if (str1) delete str1;

			edit->ShowWindow(SW_SHOWNORMAL);
			edit->UpdateWindow();
			edit->SetFocus();
		//}
	}
	else
	{
		CListCtrl::OnLButtonDblClk(nFlags,point);
	}
}
//--------------------------------------------------------------------------------
