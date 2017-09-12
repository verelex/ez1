// ez1Doc.cpp : implementation of the Cez1Doc class
//

#include "stdafx.h"
#include "ez1.h"

#include "ez1Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cez1Doc

IMPLEMENT_DYNCREATE(Cez1Doc, CDocument)

BEGIN_MESSAGE_MAP(Cez1Doc, CDocument)
END_MESSAGE_MAP()


// Cez1Doc construction/destruction

Cez1Doc::Cez1Doc()
{
	// TODO: add one-time construction code here

}

Cez1Doc::~Cez1Doc()
{
}

BOOL Cez1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// Cez1Doc serialization

void Cez1Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// Cez1Doc diagnostics

#ifdef _DEBUG
void Cez1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void Cez1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Cez1Doc commands
