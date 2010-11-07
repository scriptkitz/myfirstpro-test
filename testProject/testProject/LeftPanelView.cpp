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


BOOL CLeftPanelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}


BOOL CLeftPanelView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CListView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


void CLeftPanelView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(GetStyle() & LVS_REPORT);

	CListCtrl& listCtrl = GetListCtrl();

	// Insert a column. This override is the most convenient.
	listCtrl.InsertColumn(0, _T("Player Name"), LVCFMT_LEFT);

	// The other InsertColumn() override requires an initialized
	// LVCOLUMN structure.
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT;
	col.pszText = _T("Jersey Number");
	col.fmt = LVCFMT_LEFT;
	listCtrl.InsertColumn(1, &col);

	// Set reasonable widths for our columns
	listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	listCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
}
