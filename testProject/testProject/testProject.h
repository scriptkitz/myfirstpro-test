
// testProject.h : testProject Ӧ�ó������ͷ�ļ�
//
#pragma once


#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#include <map>
#include <string>
#include <vector>
using namespace std;


#define MAX_DOC_CONT  10      //�������������ĵ���
#define USER_INDEX	  12340	 //tab��ť��ʼID

#define TAB_WIDTH     130	//������
#define TAB_HEIGHT    25

// CtestProjectApp:
// �йش����ʵ�֣������ testProject.cpp
//
typedef map<CString,CButton*> DOCBTN_MAP;

class CtestProjectDoc;
class CtestProjectApp : public CWinApp
{
public:
	CtestProjectApp();
	~CtestProjectApp();
	bool addDocTabBtn(CtestProjectDoc* testDoc);
	void delDocTabBtn(CtestProjectDoc* testDoc);

// ��д
public:
	virtual BOOL InitInstance();
	DOCBTN_MAP 	m_docBtnMap;// ������һ��title��Ӧһ��butn��
	vector<int> m_validIndex;//������õ�btnID
	vector<CString> m_BtnIDOrder;//����btn��˳�򡣡���Ϊmap�����������������˳��������ʾ�ҡ�;

	HANDLE hMutexDll; //��ֹ������loadlibrary��injectdll���µļ���һЩfilemapping��semaphore�ȳ�ͻ��
	HANDLE hMutex;	//��ֹ���ж��ʵ������
// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	
private:
	afx_msg void funTabBtnCmd(UINT tabId);
	afx_msg void funUpdateTabBtnCmd(CCmdUI*);

	int currentBtnID;
};

extern CtestProjectApp theApp;
