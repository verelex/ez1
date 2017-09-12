// CCopyLessEdit  subclassing
#pragma once

class CCopyLessEdit : public CEdit
{
// Implementation
protected:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown( UINT, CPoint );
	afx_msg void OnLButtonDown( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};
