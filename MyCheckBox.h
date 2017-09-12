
#pragma once

// CButton subclassing

class CMyCheckBox : public CButton
{
private:
	CDateTimeCtrl *dtc1, *dtc2;

private:
	void SetDtc(int state);

public:
	CMyCheckBox(CDateTimeCtrl *src1, CDateTimeCtrl *src2); // constructor

// Implementation
protected:
	afx_msg void OnLButtonUp(UINT, CPoint);
	DECLARE_MESSAGE_MAP()
};
