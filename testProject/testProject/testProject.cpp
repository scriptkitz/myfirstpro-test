
// testProject.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "testProject.h"
#include "MainFrm.h"
#include "ProcessListDlg.h"
#include "ChildFrm.h"
#include "testProjectDoc.h"
#include "testProjectView.h"
#include "define.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CtestProjectApp

BEGIN_MESSAGE_MAP(CtestProjectApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CtestProjectApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND_RANGE(USER_INDEX,USER_INDEX+MAX_DOC_CONT,funTabBtnCmd)
	ON_UPDATE_COMMAND_UI_RANGE(USER_INDEX,USER_INDEX+MAX_DOC_CONT,funUpdateTabBtnCmd)
END_MESSAGE_MAP()


// CtestProjectApp 构造

CtestProjectApp::CtestProjectApp():hMutex(0)
{
	if (hMutex)
	{
		ReleaseMutex(hMutex);
		CloseHandle(hMutex);
	}
	if (hMutexDll)
	{
		ReleaseMutex(hMutexDll);
		CloseHandle(hMutexDll);
	}
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CtestProjectApp 对象

CtestProjectApp theApp;


// CtestProjectApp 初始化

BOOL CtestProjectApp::InitInstance()
{
	//
	TCHAR chmu[M_SIZE_SEMA]={0},ids[10]={0};
	DWORD ch = GetCurrentProcessId();

	_itow_s((int)ch,ids,10,10);

	wcscat_s(chmu,M_SIZE_SEMA,TEXT(MUTEX_STRING));
	wcscat_s(chmu,M_SIZE_SEMA,ids);
	//
	hMutex = CreateMutex(0,FALSE,chmu);
	hMutexDll = CreateMutex(0,FALSE,TEXT(MUTEX_STRING));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutexDll);
		hMutex = NULL;
		MessageBox(0,TEXT("只能运行一个实例！"),TEXT(""),0);
		return FALSE;
	}
	//
	//程序提升权限。。。
	HANDLE ph;
	LUID lu;
	OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES,&ph);
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&lu);
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount=1;
	tp.Privileges[0].Luid = lu;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(ph,false,&tp,0,0,0);
	CloseHandle(ph);
	
	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_testProjectTYPE,
		RUNTIME_CLASS(CtestProjectDoc),
		RUNTIME_CLASS(CChildFrame), // 自定义 MDI 子框架
		RUNTIME_CLASS(CtestProjectView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// 创建主 MDI 框架窗口
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 MDI 应用程序中，这应在设置 m_pMainWnd 之后立即发生
	// 启用拖/放
	m_pMainWnd->DragAcceptFiles();

	// 启用“DDE 执行”
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 分析标准外壳命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 主窗口已初始化，因此显示它并对其进行更新
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	for (int i=0;i<MAX_DOC_CONT;i++)
	{
		m_validIndex.push_back(USER_INDEX+i);
	}
	return TRUE;
}



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CtestProjectApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
CtestProjectApp::~CtestProjectApp()
{
	DOCBTN_MAP::iterator bb = m_docBtnMap.begin();
	while (bb != m_docBtnMap.end())
	{
		delete bb->second;
		bb->second = NULL;
		bb++;
	}
	
}
// CtestProjectApp 消息处理程序

void CtestProjectApp::delDocTabBtn(CtestProjectDoc* testDoc)
{
	DOCBTN_MAP::iterator it = m_docBtnMap.find(testDoc->GetTitle());
	if( it == m_docBtnMap.end())
		return;
	delete it->second;
	m_docBtnMap.erase(it);
	
	vector<CString>::iterator iit = m_BtnIDOrder.begin();
	while (iit != m_BtnIDOrder.end())
	{
		if (*iit == testDoc->GetTitle())
		{
			m_BtnIDOrder.erase(iit);
			break;
		}
		iit++;
	}
	CRect rc;
	CMainFrame *mf = (CMainFrame*)m_pMainWnd;
	mf->m_wndDlgBar.GetClientRect(&rc);
	int btcount=0;
	m_validIndex.push_back(testDoc->m_btnID);
	iit = m_BtnIDOrder.begin();
	while(iit != m_BtnIDOrder.end())
	{
		m_docBtnMap[*iit]->MoveWindow(CRect(TAB_WIDTH*btcount,rc.bottom-TAB_HEIGHT,TAB_WIDTH*(btcount+1),rc.bottom),1);
		iit++;
		btcount++;
	}
}


bool CtestProjectApp::addDocTabBtn(CtestProjectDoc* testDoc)
{
	
	CMainFrame *mf = (CMainFrame*)m_pMainWnd;
	
	INT docount = m_docBtnMap.size();
	if (docount >= MAX_DOC_CONT)
	{
		 MessageBox(m_pMainWnd->GetSafeHwnd(),TEXT("不给你创建啦...哈。"),TEXT("提示温馨的"),0);
		 return false;
	}
	CProcessListDlg pdlg;
	if (pdlg.DoModal() == IDCANCEL)
	{
		 return FALSE;
	}
	//允许。。。
	CString title = pdlg.m_processName;
	testDoc->m_hook = pdlg.m_hook;
	testDoc->m_ehook = pdlg.m_ehook;
	if (m_docBtnMap.find(title) != m_docBtnMap.end())
	{
		//MessageBox(m_pMainWnd->m_hWnd,TEXT("不要重复注入!"),TEXT("警告"),0); 这里不再判断是否已经注入，根据进程名判断不准，已经在进程列表dlg判断了。
		//但是也不能不处理，因为m_docBtnMap的key是唯一的，title不能相同，否则会被替换掉，最后出错，所以这里吧title名字改一下。
		do 
		{
			title.Append(CString("-"));
		} while (m_docBtnMap.find(title) != m_docBtnMap.end());
		
		//return false;
	}
	
	testDoc->m_docSelPID = pdlg.m_selPID;
	testDoc->SetTitle(title);
	
	CButton *tbtn = new CButton();
	CRect rc;
	mf->m_wndDlgBar.GetClientRect(&rc);
	int btnid = m_validIndex[m_validIndex.size()-1];
	testDoc->m_btnID = btnid;
	tbtn->Create(title,WS_VISIBLE|BS_FLAT|BS_LEFT|BS_BOTTOM/*|BS_OWNERDRAW*/ ,CRect(TAB_WIDTH*docount,rc.bottom-TAB_HEIGHT,TAB_WIDTH*(docount+1),rc.bottom),&mf->m_wndDlgBar,btnid);
	m_validIndex.pop_back();
	m_BtnIDOrder.push_back(title);
	m_docBtnMap[title] = tbtn;
	currentBtnID = btnid;
	return true;
}

void CtestProjectApp::funTabBtnCmd(UINT tabId)
{
	 CMainFrame *mf = (CMainFrame*)m_pMainWnd;
	 CtestProjectDoc *activDoc = (CtestProjectDoc *)mf->MDIGetActive()->GetActiveDocument();//获取当前的doc。
	 CDocTemplate *dt = activDoc->GetDocTemplate();//获取doc模板。
	 POSITION posdoc = dt->GetFirstDocPosition();//开始遍历doc模板。
	 while(posdoc)
	 {
		CtestProjectDoc* docs = (CtestProjectDoc*)dt->GetNextDoc(posdoc);
		if(tabId == docs->m_btnID)
		{
			POSITION pos = docs->GetFirstViewPosition();
			CtestProjectView* cv = (CtestProjectView*)docs->GetNextView(pos);
			mf->MDIActivate(cv->GetParentFrame());//激活选中的按钮对应的ID的doc的ID相同的cv的parentframe--》childframe。
			break;
		}
		
	 }
	//设置当前tabID。
	currentBtnID = tabId;
	return;
}
void CtestProjectApp::funUpdateTabBtnCmd(CCmdUI* ui)
{
	CMainFrame *mf = (CMainFrame*)m_pMainWnd;
	CButton* cbt = (CButton*)mf->m_wndDlgBar.GetDlgItem(ui->m_nID);
	long oldstyle = cbt->GetButtonStyle();
	if (oldstyle & BS_DEFPUSHBUTTON)
	{
		cbt->SetButtonStyle(oldstyle & ~BS_DEFPUSHBUTTON);//更新UI的时候检测是否按钮有黑边，有就消掉。难看。哈。
	}
	if (ui->m_nID == currentBtnID)
	{
		
		ui->Enable(FALSE);
	}
	else
	{
		ui->Enable(TRUE);
	}
	return;
}