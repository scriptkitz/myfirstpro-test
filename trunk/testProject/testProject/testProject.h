
// testProject.h : testProject 应用程序的主头文件
//
#pragma once


#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#include <map>
#include <string>
#include <vector>
using namespace std;


#define MAX_DOC_CONT  5      //最多给创建几个文档啊
#define USER_INDEX	  12340	 //tab按钮开始ID

#define TAB_WIDTH     85	//。。。
#define TAB_HEIGHT    25

// CtestProjectApp:
// 有关此类的实现，请参阅 testProject.cpp
//
typedef map<CString,CButton*> DOCBTN_MAP;

class CtestProjectDoc;
class CtestProjectApp : public CWinApp
{
public:
	CtestProjectApp();
	~CtestProjectApp();
	bool addDocTabBtn(CtestProjectDoc* testDoc);

// 重写
public:
	virtual BOOL InitInstance();
	DOCBTN_MAP 	m_docBtnMap;
	vector<int> m_validIndex;
// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
private:
	afx_msg void funTabBtnCmd(UINT tabId);
	afx_msg void funUpdateTabBtnCmd(CCmdUI*);

	int currentBtnID;
public:
	void setBtnEnable(CString title);
};

extern CtestProjectApp theApp;
