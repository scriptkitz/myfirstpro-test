
// testProjectDoc.cpp : CtestProjectDoc 类的实现
//

#include "stdafx.h"
#include "testProject.h"
#include "testProjectDoc.h"
#include <TlHelp32.h>
#include "define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestProjectDoc

IMPLEMENT_DYNCREATE(CtestProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(CtestProjectDoc, CDocument)
END_MESSAGE_MAP()


// CtestProjectDoc 构造/析构

CtestProjectDoc::CtestProjectDoc():m_btnID(0),m_hook(0),m_docSelPID(0),m_ehook(0)
{
	// TODO: 在此添加一次性构造代码

}

CtestProjectDoc::~CtestProjectDoc()
{
	theApp.delDocTabBtn(this);
	if (m_hook)	//卸载钩子。。。
	{
		if (0 == UnhookWindowsHookEx(m_hook))
		{
			ErrorExit(TEXT("UnhookWindowsHookEx"));
		}
	}else if (m_ehook)
	{
		UnhookWinEvent(m_ehook);
	}
	else
	{
		if (m_docSelPID)//卸载dll模块
		{
			HANDLE ffm = NULL;
			HANDLE mmp = OpenProcess(PROCESS_ALL_ACCESS ,NULL,m_docSelPID);
			MODULEENTRY32 te32 = {sizeof(te32)};
			HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,m_docSelPID);
			if ( INVALID_HANDLE_VALUE == hThreadSnap)
			{
				ErrorExit(TEXT("CreateToolhelp32Snapshot"));
			}
			if( Module32First( hThreadSnap, &te32) )
			{
				do{
					if (wcscmp(te32.szModule,TEXT(INJECT_DLL_NAME))==0)
					{
						ffm = te32.modBaseAddr;
						break;
					}
				}while( Module32Next( hThreadSnap, &te32) );
			}
			CloseHandle(hThreadSnap);
			if (ffm)
			{
				LPTHREAD_START_ROUTINE ffw = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"FreeLibrary");
				HANDLE ch = CreateRemoteThread(mmp,0,0,ffw,ffm,0,0);
				if (!ch)
				{
					ErrorExit(TEXT("CreateRemoteThread"));
				}
			}
			CloseHandle(mmp);
		}
		
	}
}

BOOL CtestProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	if (!theApp.addDocTabBtn(this))
	{
		return FALSE;
	}
	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	return TRUE;
}




// CtestProjectDoc 序列化

void CtestProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CtestProjectDoc 诊断

#ifdef _DEBUG
void CtestProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CtestProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CtestProjectDoc 命令
