
// testProjectDoc.cpp : CtestProjectDoc 类的实现
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


// CtestProjectDoc 构造/析构

CtestProjectDoc::CtestProjectDoc():m_btnID(0),m_hook(0),m_docSelPID(0)
{
	// TODO: 在此添加一次性构造代码

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
	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	return TRUE;
}




// CtestProjectDoc 序列化

void CtestProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CtestProjectDoc 诊断

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


// CtestProjectDoc 命令
