#pragma once
#include "stdafx.h"
#include "MyCheckBox.h"

BEGIN_MESSAGE_MAP(CMyCheckBox , CButton)
	ON_WM_LBUTTONUP( )
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------
CMyCheckBox::CMyCheckBox(CDateTimeCtrl *src1, CDateTimeCtrl *src2)
{
	dtc1 = src1;
	dtc2 = src2;
}
//--------------------------------------------------------------------------------
void CMyCheckBox::SetDtc(int state)
{
	switch (!state)
	{
		case BST_CHECKED : dtc1->EnableWindow(TRUE); dtc2->EnableWindow(TRUE);
			break;
		case BST_UNCHECKED : dtc1->EnableWindow(FALSE); dtc2->EnableWindow(FALSE);
			break;
	}
}
//--------------------------------------------------------------------------------
void CMyCheckBox::OnLButtonUp( UINT nFlags, CPoint point )
{
	SetDtc( this->GetCheck() );
	return CButton::OnLButtonUp( nFlags, point );
}
//--------------------------------------------------------------------------------
