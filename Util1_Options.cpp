#include "stdafx.h"
#include "Util1_Options.h"

int CUtil1_Options::LoadXML_Options()
{
	CoInitialize(0);
	CComPtr<IXMLDOMDocument> pDoc;
	HRESULT hr = pDoc.CoCreateInstance(__uuidof(DOMDocument));
	if (CheckCOMResult(hr, pDoc.p, 1, L"Unable to create XML parser object"))
		return 1;

	// load XML document
	VARIANT_BOOL bSuccess = false;
	hr = pDoc->load(CComVariant(L"options.xml"), &bSuccess);
	if (CheckCOMResult(hr, (PVOID)-1, bSuccess, L"Unable to load XML document into the parser"))
		return 1;

	// select node (Admin name)
	CComBSTR strAdminName(L"opt/admin/name");
	CComPtr<IXMLDOMNode> pAdminName;
	hr = pDoc->selectSingleNode(strAdminName,&pAdminName);
	CComVariant retVal(VT_EMPTY);
	hr = pAdminName->get_nodeTypedValue(&retVal);
	AdminName.Insert(0,retVal.bstrVal);
	// select node (Admin JobTitle)
	CComBSTR strAdminJobTitle(L"opt/admin/jobTitle");
	CComPtr<IXMLDOMNode> pAdminJobTitle;
	hr = pDoc->selectSingleNode(strAdminJobTitle,&pAdminJobTitle);
	hr = pAdminJobTitle->get_nodeTypedValue(&retVal);
	AdminJobTitle.Insert(0,retVal.bstrVal);
	// select node (User name)
	CComBSTR strUserName(L"opt/user/name");
	CComPtr<IXMLDOMNode> pUserName;
	hr = pDoc->selectSingleNode(strUserName,&pUserName);
	hr = pUserName->get_nodeTypedValue(&retVal);
	UserName.Insert(0,retVal.bstrVal);
	// select node (User JobTitle)
	CComBSTR strUserJobTitle(L"opt/user/jobTitle");
	CComPtr<IXMLDOMNode> pUserJobTitle;
	hr = pDoc->selectSingleNode(strUserJobTitle,&pUserJobTitle);
	hr = pUserJobTitle->get_nodeTypedValue(&retVal);
	UserJobTitle.Insert(0,retVal.bstrVal);

	if (pDoc) pDoc.Release();
	return 0;
}
//--------------------------------------------------------------------------------
int CUtil1_Options::CheckCOMResult(HRESULT hr, PVOID spXMLDOMp, int ires, CString str)
{
	if ( (hr != S_OK) || (spXMLDOMp == 0) || (ires == 0) )
	{
		MessageBox(0,str,0,MB_OK|MB_ICONERROR);
		return 1;
	}
	return 0;
}
