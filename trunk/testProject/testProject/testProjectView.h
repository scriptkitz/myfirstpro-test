
// testProjectView.h : CtestProjectView 类的接口
//


#pragma once


class CtestProjectView : public CListView//CView
{
protected: // 仅从序列化创建
	CtestProjectView();
	DECLARE_DYNCREATE(CtestProjectView)

// 属性
public:
	CtestProjectDoc* GetDocument() const;

// 操作
public:
	HANDLE hsgFile;
	LPVOID lpBaseOffset;
	int offsetgf;
	int totalIndex;
	HANDLE readsema;
	HANDLE writesema;
	HANDLE hthread;
	int m_exitproc;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 实现
public:
	virtual ~CtestProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};

#ifndef _DEBUG  // testProjectView.cpp 中的调试版本
inline CtestProjectDoc* CtestProjectView::GetDocument() const
   { return reinterpret_cast<CtestProjectDoc*>(m_pDocument); }
#endif

