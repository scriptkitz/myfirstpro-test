
// ChildFrm.h : CChildFrame ��Ľӿ�
//

#pragma once

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:
	CSplitterWnd m_leftWnd;
	CSplitterWnd m_topDownWnd;
// ����
public:

// ��д
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};