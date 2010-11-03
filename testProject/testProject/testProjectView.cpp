
// testProjectView.cpp : CtestProjectView 类的实现
//

#include "stdafx.h"
#include "testProject.h"

#include "testProjectDoc.h"
#include "testProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestProjectView

IMPLEMENT_DYNCREATE(CtestProjectView, CView)

BEGIN_MESSAGE_MAP(CtestProjectView, CView)
END_MESSAGE_MAP()

// CtestProjectView 构造/析构

CtestProjectView::CtestProjectView()
{
	// TODO: 在此处添加构造代码

}

CtestProjectView::~CtestProjectView()
{
}

BOOL CtestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CtestProjectView 绘制

void CtestProjectView::OnDraw(CDC* /*pDC*/)
{
	CtestProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CtestProjectView 诊断

#ifdef _DEBUG
void CtestProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CtestProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtestProjectDoc* CtestProjectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestProjectDoc)));
	return (CtestProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestProjectView 消息处理程序
