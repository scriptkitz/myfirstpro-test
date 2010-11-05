
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "testProject.h"
#include "testProjectView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_SHOWMAXIMIZED; //激活子窗口最大化每次。
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
	//ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_MAXIMIZE|WS_MINIMIZE,0);//想最大化和最小化按纽
}


BOOL CChildFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	WORD iid = LOWORD(wParam);
	
	return CMDIChildWnd::OnCommand(wParam, lParam);
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	//分割成左一列大的，右2行小的窗口。。
	BOOL ret = m_leftWnd.CreateStatic(this,1,2);
	m_leftWnd.CreateView(0,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	m_topDownWnd.CreateStatic(&m_leftWnd,2,1,WS_CHILD|WS_VISIBLE,m_leftWnd.IdFromRowCol(0,1));  //这里的最后一个参数是关键。表示右边一块窗口的ID。
	m_topDownWnd.CreateView(0,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	m_topDownWnd.CreateView(1,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	return ret; //CMDIChildWnd::OnCreateClient(lpcs, pContext); //搞了半天SplitterWnd都不成，原来是最后一句问题。不要调用默认的OnCreateClient了。
	//默认的OnCreateClient会调用CframeWnd的CreateView，创建一个单一的view。
}
