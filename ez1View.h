// ez1View.h : interface of the Cez1View class
//


#pragma once


class Cez1View : public CView
{
protected: // create from serialization only
	Cez1View();
	DECLARE_DYNCREATE(Cez1View)

// Attributes
public:
	Cez1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cez1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ez1View.cpp
inline Cez1Doc* Cez1View::GetDocument() const
   { return reinterpret_cast<Cez1Doc*>(m_pDocument); }
#endif

