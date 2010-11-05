// LeftPanelView.cpp : implementation file
//

#include "stdafx.h"
#include "testProject.h"
#include "LeftPanelView.h"


// CLeftPanelView

IMPLEMENT_DYNCREATE(CLeftPanelView, CListView)

CLeftPanelView::CLeftPanelView()
{

}

CLeftPanelView::~CLeftPanelView()
{
}

BEGIN_MESSAGE_MAP(CLeftPanelView, CListView)
END_MESSAGE_MAP()


// CLeftPanelView diagnostics

#ifdef _DEBUG
void CLeftPanelView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CLeftPanelView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLeftPanelView message handlers
