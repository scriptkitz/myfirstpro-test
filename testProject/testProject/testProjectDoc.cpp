
// testProjectDoc.cpp : CtestProjectDoc ���ʵ��
//

#include "stdafx.h"
#include "testProject.h"

#include "testProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestProjectDoc

IMPLEMENT_DYNCREATE(CtestProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestProjectDoc, CDocument)
END_MESSAGE_MAP()


// CtestProjectDoc ����/����

CtestProjectDoc::CtestProjectDoc()
{
	// TODO: �ڴ�����һ���Թ������

}

CtestProjectDoc::~CtestProjectDoc()
{
}

BOOL CtestProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ��������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CtestProjectDoc ���л�

void CtestProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ����Ӵ洢����
	}
	else
	{
		// TODO: �ڴ����Ӽ��ش���
	}
}


// CtestProjectDoc ���

#ifdef _DEBUG
void CtestProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestProjectDoc ����