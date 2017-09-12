// ez1Doc.h : interface of the Cez1Doc class
//


#pragma once


class Cez1Doc : public CDocument
{
protected: // create from serialization only
	Cez1Doc();
	DECLARE_DYNCREATE(Cez1Doc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~Cez1Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


