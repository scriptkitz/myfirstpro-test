// HexEditView.cpp : implementation file
//

#include "stdafx.h"
#include "testProject.h"
#include "HexEditView.h"


// CHexEditView

IMPLEMENT_DYNCREATE(CHexEditView, CEditView)

CHexEditView::CHexEditView()
{

}

CHexEditView::~CHexEditView()
{
}

BEGIN_MESSAGE_MAP(CHexEditView, CEditView)
END_MESSAGE_MAP()


// CHexEditView diagnostics

#ifdef _DEBUG
void CHexEditView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CHexEditView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHexEditView message handlers
