
// testProjectView.cpp : CtestProjectView 类的实现
//

#include "stdafx.h"
#include "testProject.h"
#include "define.h"
#include "testProjectDoc.h"
#include "testProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//int m_exitproc = 0; //因为是全局，view析构后，此变量依然存在，再次new一个view，变量不会被赋初值，仍是上次值。
						//用也可以，但要保证析构后值清理0；
						//这个变量不能用全局的，因为其他view公用全局变量，改变会影响所以view。。;
static char szSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM", "SEQ" };
// CtestProjectView

IMPLEMENT_DYNCREATE(CtestProjectView, CListView)

BEGIN_MESSAGE_MAP(CtestProjectView, CListView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CtestProjectView 构造/析构

CtestProjectView::CtestProjectView():hthread(0)
{
	// TODO: 在此处添加构造代码；
	m_exitproc = 0;
	exesema = 0;
	cursocktsema = 0;
	lpBaseOffset = 0;
	hsgFile = 0;
}

CtestProjectView::~CtestProjectView()
{
	DWORD dwRet = 0;
	MSG msg;
	m_exitproc = 2;
	
	if(ReleaseSemaphore(exesema,1,NULL)!=0)
	{
		if(WaitForSingleObject(exitsema,INFINITE)==WAIT_FAILED)
		{
			ErrorExit(TEXT("WaitForSingleObject"));
		}
	}
	while (TRUE)
	{
		//wait for m_hThread to be over，and wait for
		//QS_ALLINPUT（Any message is in the queue）
		dwRet = MsgWaitForMultipleObjects (1, &hthread,   FALSE, INFINITE, QS_ALLINPUT);
		switch(dwRet)
		{
		case WAIT_OBJECT_0: 
			break; //break the loop
		case WAIT_OBJECT_0 + 1:
			//get the message from Queue
			//and dispatch it to specific window
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			DispatchMessage(&msg);
			continue;
		default:
			break; // unexpected failure
		}
		break;
	}
	/*
	if (exitsema)
	{
		ReleaseSemaphore(exitsema,1,NULL);
		CloseHandle(exitsema);
	}
	if (exesema)
	{
		ReleaseSemaphore(exesema,1,NULL);
		CloseHandle(exesema);
	}
	if (cursocktsema)
	{
		ReleaseSemaphore(cursocktsema,1,NULL);//没用到：WaitForSingleObject(cursocktsema)所以。不要release不让dll出错298
		CloseHandle(cursocktsema);
	}
	if (lpBaseOffset)
	{
		UnmapViewOfFile(lpBaseOffset);
	}
	if (hsgFile)
	{
		CloseHandle(hsgFile);
	}
	*/
}
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	
	CtestProjectView* pv = (CtestProjectView*)lpParameter;
	DWORD pid = pv->GetDocument()->m_docSelPID;

	//医生来了，挂牌“出诊”。。；//0表示医生临时不在 1表示医生在，2表示医院要关门；
	*(int*)pv->lpBaseOffset = 1;
	pv->m_exitproc = 1;
	if(WaitForSingleObject(pv->exesema,INFINITE)==WAIT_FAILED)
	{
		ErrorExit(TEXT("WaitForSingleObject"));
		return 0;
	}
	if(WaitForSingleObject(pv->cursocktsema,INFINITE)==WAIT_FAILED)
	{
		ErrorExit(TEXT("WaitForSingleObject"));
		return 0;
	}
	HANDLE hdr = OpenProcess(PROCESS_ALL_ACCESS,NULL,pid);
	CListCtrl& listCtrl = pv->GetListCtrl();
	int la = 0;
	HANDLE tarhd=0;
	struct dataHeader dh;
	while(1)
	{
		if(WaitForSingleObject(pv->exesema,INFINITE)==WAIT_FAILED)//我不主动去拿病情本，我要等病人通知写好了，再去拿；
		{
			ErrorExit(TEXT("WaitForSingleObject"));
			return 0;
		}
		if(WaitForSingleObject(pv->exitsema,1000)==WAIT_TIMEOUT)
		{
			*(int*)pv->lpBaseOffset = 2;
			dh.cmd=0;
			//ErrorExit(TEXT("88888888888888888"));
		}
		if(ReleaseSemaphore(pv->exitsema,1,NULL)==0)
		{
			ErrorExit(TEXT("ReleaseSemaphore cursocktsema"));
		}
		dh.cmd = 1;
		//听听下班铃响了没= =。
		switch(pv->m_exitproc)//
		{
		case 0://临时有事要出门会；
			*(int*)pv->lpBaseOffset = 0;
			break;
		case 2://表示下班啦；
			*(int*)pv->lpBaseOffset = 2;
			dh.cmd=0;
			break;
		}
		//ok，病人写好了了。。我看看；
		struct dataHeader* pdh = (struct dataHeader*)pv->lpDataOffset;
		
		//看看写的格式有没错误= =；
		wchar_t bu[50]={0};
		swprintf_s(bu,50,TEXT("index:%d"),pdh->index);
		if (listCtrl)
		{
			listCtrl.InsertItem(0,bu);
		}
		//知道了病情，给病人开药方，病人正在等着读；
		pdh->cmd = dh.cmd;
		//MessageBox(0,TEXT("给病人开药方"),TEXT(""),0);
		//药方开好，通知病人拿药方。。
		if(ReleaseSemaphore(pv->cursocktsema,1,NULL)==0)
		{
			ErrorExit(TEXT("ReleaseSemaphore cursocktsema"));
		}
		if (pv->m_exitproc == 2)
		{
			//退出了。。。；
			return 0;
		}
	}
}

BOOL CtestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	totalIndex =0;
	offsetgf =0;
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

// CtestProjectView 绘制

void CtestProjectView::OnDraw(CDC* /*pDC*/)
{
	CtestProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CtestProjectView 诊断

#ifdef _DEBUG
void CtestProjectView::AssertValid() const
{
	CListView::AssertValid();
}

void CtestProjectView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CtestProjectDoc* CtestProjectView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestProjectDoc)));
	return (CtestProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestProjectView 消息处理程序


void CtestProjectView::OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/)
{
	// TODO: Add your specialized code here and/or call the base class
	
	ASSERT(GetStyle() & LVS_REPORT);

	CListCtrl& listCtrl = GetListCtrl();

	// Insert a column. This override is the most convenient.
	listCtrl.InsertColumn(0, _T("Player Name"), LVCFMT_LEFT);

	// The other InsertColumn() override requires an initialized
	// LVCOLUMN structure.
	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_TEXT;
	col.pszText = _T("Jersey Number");
	col.fmt = LVCFMT_LEFT;
	listCtrl.InsertColumn(1, &col);

	// Set reasonable widths for our columns
	listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
	listCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
	if (!hthread)
	{
		TCHAR chmu[M_SIZE_SEMA],ids[10];
		DWORD ch = GetDocument()->m_docSelPID;
		memset(chmu,0,M_SIZE_SEMA);
		memset(ids,0,10);
		_itow_s((int)ch,ids,10,10);

		wcscat_s(chmu,M_SIZE_SEMA,TEXT(FILE_MAPPING_STRING));
		wcscat_s(chmu,M_SIZE_SEMA,ids);
	//	MessageBox(chmu,TEXT("CreateFileMapping"),0);
		hsgFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,MAPPINGFILESIZE,chmu);
		lpBaseOffset = MapViewOfFile(hsgFile,FILE_MAP_ALL_ACCESS,0,0,0);
		lpDataOffset = (char*)lpBaseOffset+4;
		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));		//注入多个进程时候根据每个进程的ID，创建不同的信号量。防止串号哈。
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		cursocktsema = CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(WRITE_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		exesema =  CreateSemaphore(NULL,1,1,chmu);

		exitsema =  CreateSemaphore(NULL,1,1,NULL);
		hthread = CreateThread(NULL,NULL,ThreadProc,this,0,0);
	}
}


void CtestProjectView::OnDestroy()
{
	CListView::OnDestroy();
	// TODO: Add your message handler code here
}
