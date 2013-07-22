
// PublishAide_1View.h : interface of the CPublishAide_1View class
//
#include "PublishAide_1Doc.h"
#include "PublishAide_1.h"
#pragma once


class CPublishAide_1View : public CHtmlView
{
protected: // create from serialization only
	CPublishAide_1View();
	DECLARE_DYNCREATE(CPublishAide_1View)

// Attributes
public:
	CPublishAide_1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CPublishAide_1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnDocumentComplete(LPCTSTR lpszURL);
	int GotoAnchor(BSTR* sNearlyID, BSTR* sLinkURL);
	int LoginFunction(void);
	int AuctionCategorySelect(void);
};

#ifndef _DEBUG  // debug version in PublishAide_1View.cpp
inline CPublishAide_1Doc* CPublishAide_1View::GetDocument() const
   { return reinterpret_cast<CPublishAide_1Doc*>(m_pDocument); }
#endif

