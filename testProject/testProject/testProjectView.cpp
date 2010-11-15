
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
	readsema = 0;
	writesema = 0;
	lpBaseOffset = 0;
	hsgFile = 0;
}

CtestProjectView::~CtestProjectView()
{
	if (readsema)
	{
		ReleaseSemaphore(readsema,1,NULL);
		CloseHandle(readsema);
	}
	
	if (writesema)
	{
		ReleaseSemaphore(writesema,1,NULL);//没用到：WaitForSingleObject(writesema)所以。不要release不让dll出错298
		CloseHandle(writesema);
	}
	if (lpBaseOffset)
	{
		UnmapViewOfFile(lpBaseOffset);
	}
	if (hsgFile)
	{
		CloseHandle(hsgFile);
	}
	
}
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	
	CtestProjectView* pv = (CtestProjectView*)lpParameter;
	DWORD pid = pv->GetDocument()->m_docSelPID;
	if(WaitForSingleObject(pv->readsema,INFINITE)==WAIT_FAILED)
	{
		ErrorExit(TEXT("WaitForSingleObject"));
		return 0;
	}
	HANDLE hdr = OpenProcess(PROCESS_ALL_ACCESS,NULL,pid);
	CListCtrl& listCtrl = pv->GetListCtrl();
	int ta = 0,err =0,la =0,ddt=0;
	HANDLE hd=0;
	HANDLE tarhd=0;
	while(1)
	{
		if(WaitForSingleObject(pv->readsema,INFINITE)==WAIT_FAILED)
		{
			ErrorExit(TEXT("WaitForSingleObject"));
			return 0;
		}

		la = *(int*)pv->lpBaseOffset;
		hd = *(HANDLE*)((char*)pv->lpBaseOffset+4);

		if (la != ta)
		{
			wchar_t bu[50]={0};
			/*
			if (!tarhd)
			{
				//static HANDLE tarhd=0;//这里变量声明这样导致了一个郁闷的错误，加载一个进程没问题，第二时候会导致锁死，最终原因就是这个变量，staic了。擦。闷。已经放上边了，并去掉了static；
				if(DuplicateHandle(hdr,hd,GetCurrentProcess(),&tarhd,0, FALSE, DUPLICATE_SAME_ACCESS)==0)// 啊哈，没必要复制目标的句柄啦，因为创建的是有名字的信号量，全局的，所以直接用自己的句柄释放一样的。
				{
					ErrorExit(TEXT("DuplicateHandle"));
				}
			}
			*/
			swprintf_s(bu,50,TEXT("index:%d handle:%d ddt:%d"),la,hd,ddt);
			listCtrl.InsertItem(0,bu);
			//在下面写入数据，回写吧，让dll读的数据。;
			if (pv->m_exitproc == 1)
			{
				//如果线程要退出。。。通知dll也要退出啦。。。；写入数据来通知dll。。唉。；

				*(int*)((char*)pv->lpBaseOffset+8) = -1;
				if(ReleaseSemaphore(pv->writesema,1,NULL)==0)
				{
					ErrorExit(TEXT("ReleaseSemaphore1"));
				}
				return 0;
			}
			*(int*)((char*)pv->lpBaseOffset+8) = la;
			//MessageBox(0,TEXT("aaaaaaaaaaaaaaa"),TEXT(""),0);
			if(ReleaseSemaphore(pv->writesema,1,NULL)==0)
			{
				ErrorExit(TEXT("ReleaseSemaphore1"));
			}

			ta = la;
		}
		//if(ReleaseSemaphore(pv->readsema,1,NULL)==0)
		//{
		//	ErrorExit(TEXT("ReleaseSemaphore2"));
		//}
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

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(WRITE_SEMAPHORE));		//注入多个进程时候根据每个进程的ID，创建不同的信号量。防止串号哈。
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		writesema = CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		readsema =  CreateSemaphore(NULL,1,1,chmu);
		//readsema = OpenSemaphore(SEMAPHORE_ALL_ACCESS,NULL,chmu);
		//MessageBox(chmu,TEXT("bbbbbbbbbbbbb"),0);
		hthread = CreateThread(NULL,NULL,ThreadProc,this,0,0);
	}
}


void CtestProjectView::OnDestroy()
{
	DWORD dwRet = 0;
	MSG msg;
	// 唉，看来也不能放这个函数里，这个只有active的view才会响应这个onDestroy。后台的视图类都不会调用。擦。
	m_exitproc = 1;
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
	CListView::OnDestroy();
	// TODO: Add your message handler code here
}
