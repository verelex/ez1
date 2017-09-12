#include "StdAfx.h"
#include "OptionsDialog.h"
#include "resource.h"

BEGIN_MESSAGE_MAP(COptionsDlg , CDialog)
	//ON_COMMAND( IDOK, OnButtonOK )
	ON_BN_CLICKED(IDOK, &COptionsDlg::OnBnClickedOk)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------
COptionsDlg::COptionsDlg() : CDialog(COptionsDlg ::IDD)
{
}
//--------------------------------------------------------------------------------
void COptionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX,IDC_EDIT1,AdminName);
	DDX_Text(pDX,IDC_EDIT2,AdminJobTitle);
	DDX_Text(pDX,IDC_EDIT3,UserName);
	DDX_Text(pDX,IDC_EDIT4,UserJobTitle);
}
//--------------------------------------------------------------------------------
BOOL COptionsDlg::OnInitDialog()
{
	// Загружаем значения из XML файла
	LoadXML_Options();
	CDialog::OnInitDialog();
	CenterWindow();
	return TRUE;
}
//--------------------------------------------------------------------------------
void COptionsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	// Сохраняем значения в XML файл
	SaveXML_Options();
	MessageBox(L"Изменения сохранены");
	CDialog::OnCancel(); // закрыть диалог
}
//--------------------------------------------------------------------------------
int COptionsDlg::SaveXML_Options()
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
	CComVariant putVal_1(AdminName);
	hr = pAdminName->put_nodeTypedValue(putVal_1);
	CComBSTR strAdminJobTitle(L"opt/admin/jobTitle");
	CComPtr<IXMLDOMNode> pAdminJobTitle;
	hr = pDoc->selectSingleNode(strAdminJobTitle,&pAdminJobTitle);
	CComVariant putVal_2(AdminJobTitle);
	hr = pAdminJobTitle->put_nodeTypedValue(putVal_2);
	CComBSTR strUserName(L"opt/user/name");
	CComPtr<IXMLDOMNode> pUserName;
	hr = pDoc->selectSingleNode(strUserName,&pUserName);
	CComVariant putVal_3(UserName);
	hr = pUserName->put_nodeTypedValue(putVal_3);
	CComBSTR strUserJobTitle(L"opt/user/jobTitle");
	CComPtr<IXMLDOMNode> pUserJobTitle;
	hr = pDoc->selectSingleNode(strUserJobTitle,&pUserJobTitle);
	CComVariant putVal_4(UserJobTitle);
	hr = pUserJobTitle->put_nodeTypedValue(putVal_4);
	// save XML document
	hr = pDoc->save( CComVariant(L"options.xml") );
	if (CheckCOMResult(hr, (PVOID)-1, 1, L"Unable to save XML document to file"))
		return 1;

	if (pDoc) pDoc.Release();
	return 0;
}
//--------------------------------------------------------------------------------
