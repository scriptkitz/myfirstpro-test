
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
	INT m_btnID;//��Ӧapp��İ�ť��ID��
	DWORD m_docSelPID; //����ѡ�еĽ����б���̵�ID��
	HHOOK m_hook;//�����WINDOWS HOOkע�뷽ʽ����ֵ��Ϊ0��
	HWINEVENTHOOK m_ehook;//EVENT HOOK...
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


