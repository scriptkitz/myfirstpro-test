
// testProjectView.h : CtestProjectView ��Ľӿ�
//


#pragma once


class CtestProjectView : public CView
{
protected: // �������л�����
	CtestProjectView();
	DECLARE_DYNCREATE(CtestProjectView)

// ����
public:
	CtestProjectDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// ʵ��
public:
	virtual ~CtestProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // testProjectView.cpp �еĵ��԰汾
inline CtestProjectDoc* CtestProjectView::GetDocument() const
   { return reinterpret_cast<CtestProjectDoc*>(m_pDocument); }
#endif

