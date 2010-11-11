
// testProjectDoc.h : CtestProjectDoc 类的接口
//


#pragma once


class CtestProjectDoc : public CDocument
{
protected: // 仅从序列化创建
	CtestProjectDoc();
	DECLARE_DYNCREATE(CtestProjectDoc)

// 属性
public:

// 操作
public:
	INT m_btnID;//对应app类的按钮的ID。
	DWORD m_docSelPID; //保存选中的进程列表进程的ID。
	HHOOK m_hook;//如果是WINDOWS HOOk注入方式，此值不为0。
	HWINEVENTHOOK m_ehook;//EVENT HOOK...
// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~CtestProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


