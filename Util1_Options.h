// CUtil1_Options  XML load options
#pragma once

class CUtil1_Options
{
	public:
		CString AdminName,AdminJobTitle,UserName,UserJobTitle;

		// чтобы можно было загрузить XML не вызывая диалог
		int LoadXML_Options();
		int CheckCOMResult(HRESULT hr, PVOID spXMLDOMp, int ires, CString str);
};
