
// testProjectView.cpp : CtestProjectView ���ʵ��
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

// CtestProjectView ����/����

CtestProjectView::CtestProjectView()
{
	// TODO: �ڴ˴���ӹ������

}

CtestProjectView::~CtestProjectView()
{
}

BOOL CtestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CtestProjectView ����

void CtestProjectView::OnDraw(CDC* /*pDC*/)
{
	CtestProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CtestProjectView ���

#ifdef _DEBUG
void CtestProjectView::AssertValid() const
{
	CView::AssertValid();
}

void CtestProjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CtestProjectDoc* CtestProjectView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestProjectDoc)));
	return (CtestProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestProjectView ��Ϣ�������
