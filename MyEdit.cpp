// CMyEdit  subclassing

#include "stdafx.h"
#include "MyEdit.h"

BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	ON_WM_KEYDOWN( )
	ON_WM_KILLFOCUS( )
	ON_WM_RBUTTONDOWN( )
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CMyEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL || nChar == VK_LCONTROL || nChar == VK_RCONTROL)
	{
		SendMessage(WM_CLOSE,nChar,nFlags);
	}
}
//--------------------------------------------------------------------------------
void CMyEdit::OnKillFocus(CWnd* w)
{
	// return focus to the parent
	CWnd *wndz = GetParent();
	wndz->SetFocus();
	DestroyWindow();
	CEdit::OnKillFocus(w);
}
//--------------------------------------------------------------------------------
void CMyEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	OnKillFocus(0);
}
//--------------------------------------------------------------------------------
void CMyEdit::SetMLCitems(CMyListCtrl *mlcr, int x, int y)
{
	mlc = mlcr;
	ic = x;
	jc = y;
}
//--------------------------------------------------------------------------------
