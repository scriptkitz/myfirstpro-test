
// testProject.h : testProject Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CtestProjectApp:
// �йش����ʵ�֣������ testProject.cpp
//

class CtestProjectApp : public CWinApp
{
public:
	CtestProjectApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CtestProjectApp theApp;
