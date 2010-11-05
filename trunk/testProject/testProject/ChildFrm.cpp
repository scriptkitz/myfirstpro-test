
// ChildFrm.cpp : CChildFrame ���ʵ��
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

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ���

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

// CChildFrame ��Ϣ�������


void CChildFrame::ActivateFrame(int nCmdShow)
{
	// TODO: Add your specialized code here and/or call the base class
	nCmdShow = SW_SHOWMAXIMIZED; //�����Ӵ������ÿ�Ρ�
	
	CMDIChildWnd::ActivateFrame(nCmdShow);
	//ModifyStyle(WS_MAXIMIZEBOX|WS_MINIMIZEBOX|WS_MAXIMIZE|WS_MINIMIZE,0);//����󻯺���С����Ŧ
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

	//�ָ����һ�д�ģ���2��С�Ĵ��ڡ���
	BOOL ret = m_leftWnd.CreateStatic(this,1,2);
	m_leftWnd.CreateView(0,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	m_topDownWnd.CreateStatic(&m_leftWnd,2,1,WS_CHILD|WS_VISIBLE,m_leftWnd.IdFromRowCol(0,1));  //��������һ�������ǹؼ�����ʾ�ұ�һ�鴰�ڵ�ID��
	m_topDownWnd.CreateView(0,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	m_topDownWnd.CreateView(1,0,RUNTIME_CLASS(CtestProjectView),CSize(100,100),pContext);
	return ret; //CMDIChildWnd::OnCreateClient(lpcs, pContext); //���˰���SplitterWnd�����ɣ�ԭ�������һ�����⡣��Ҫ����Ĭ�ϵ�OnCreateClient�ˡ�
	//Ĭ�ϵ�OnCreateClient�����CframeWnd��CreateView������һ����һ��view��
}
