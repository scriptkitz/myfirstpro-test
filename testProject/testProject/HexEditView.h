#pragma once


// CHexEditView view

class CHexEditView : public CEditView
{
	DECLARE_DYNCREATE(CHexEditView)

protected:
	CHexEditView();           // protected constructor used by dynamic creation
	virtual ~CHexEditView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


