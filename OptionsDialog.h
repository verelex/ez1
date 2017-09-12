// COPtionsDlg  dialog used for Show Optionss
#pragma once
#include "resource.h"
#include "Util1_Options.h"

class COptionsDlg : public CDialog, public CUtil1_Options
{
public:
	COptionsDlg();
	int SaveXML_Options();

// Dialog Data
	enum { IDD = IDD_OPTIONS_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	virtual BOOL OnInitDialog();

// Implementation
protected:
	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
};
