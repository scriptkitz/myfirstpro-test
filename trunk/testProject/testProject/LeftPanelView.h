#pragma once


// CLeftPanelView view

class CLeftPanelView : public CListView
{
	DECLARE_DYNCREATE(CLeftPanelView)

protected:
	CLeftPanelView();           // protected constructor used by dynamic creation
	virtual ~CLeftPanelView();

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


