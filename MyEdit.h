// CMyEdit  subclassing
#pragma once
class CMyListCtrl;

class CMyEdit : public CEdit
{
// Implementation
private:
	CMyListCtrl *mlc;
	int ic, jc;

public:
	void SetMLCitems(CMyListCtrl *mlcr, int x, int y);

protected:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus( CWnd* );
	afx_msg void OnRButtonDown( UINT, CPoint );
	DECLARE_MESSAGE_MAP()
};
