
// testProjectDoc.h : CtestProjectDoc ��Ľӿ�
//


#pragma once


class CtestProjectDoc : public CDocument
{
protected: // �������л�����
	CtestProjectDoc();
	DECLARE_DYNCREATE(CtestProjectDoc)

// ����
public:

// ����
public:
	INT m_btnID;
	DWORD m_docSelPID;
// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CtestProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


