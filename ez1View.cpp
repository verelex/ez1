// ez1View.cpp : implementation of the Cez1View class
//

#include "stdafx.h"
#include "ez1.h"

#include "ez1Doc.h"
#include "ez1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cez1View

IMPLEMENT_DYNCREATE(Cez1View, CView)

BEGIN_MESSAGE_MAP(Cez1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cez1View construction/destruction

Cez1View::Cez1View()
{
	// TODO: add construction code here

}

Cez1View::~Cez1View()
{
}

BOOL Cez1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cez1View drawing

void Cez1View::OnDraw(CDC* /*pDC*/)
{
	Cez1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// Cez1View printing

BOOL Cez1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cez1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cez1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cez1View diagnostics

#ifdef _DEBUG
void Cez1View::AssertValid() const
{
	CView::AssertValid();
}

void Cez1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cez1Doc* Cez1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cez1Doc)));
	return (Cez1Doc*)m_pDocument;
}
#endif //_DEBUG


// Cez1View message handlers
