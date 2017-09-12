// CMyEdit  subclassing

#include "stdafx.h"
#include "CopyLessEdit.h"

BEGIN_MESSAGE_MAP(CCopyLessEdit, CEdit)
	ON_WM_KEYDOWN( )
	ON_WM_RBUTTONDOWN( )
END_MESSAGE_MAP()

//--------------------------------------------------------------------------------
void CCopyLessEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL || nChar == VK_LCONTROL || nChar == VK_RCONTROL)
	{
		ShowWindow(SW_HIDE);
		ShowWindow(SW_SHOW);
	}
}
//--------------------------------------------------------------------------------
void CCopyLessEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}
//--------------------------------------------------------------------------------
void CCopyLessEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	ShowWindow(SW_HIDE);
	ShowWindow(SW_SHOW);
}
//--------------------------------------------------------------------------------
