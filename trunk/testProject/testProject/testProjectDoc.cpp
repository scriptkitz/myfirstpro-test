
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

CtestProjectDoc::CtestProjectDoc():m_btnID(0),m_hook(0),m_docSelPID(0)
{
	// TODO: �ڴ����һ���Թ������

}

CtestProjectDoc::~CtestProjectDoc()
{
	theApp.delDocTabBtn(this);
	if (m_hook)
	{
		if (NULL == UnhookWindowsHookEx(m_hook))
		{
			MessageBox(NULL,TEXT("UnhookWindowsHookEx Error"),TEXT(""),0);
		}
	}
}

BOOL CtestProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!theApp.addDocTabBtn(this))
	{
		return FALSE;
	}
	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	return TRUE;
}




// CtestProjectDoc ���л�

void CtestProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
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
