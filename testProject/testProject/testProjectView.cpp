
// testProjectView.cpp : CtestProjectView ���ʵ��
//

#include "stdafx.h"
#include "testProject.h"
#include "define.h"
#include "testProjectDoc.h"
#include "testProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//int m_exitproc = 0; //��Ϊ��ȫ�֣�view�����󣬴˱�����Ȼ���ڣ��ٴ�newһ��view���������ᱻ����ֵ�������ϴ�ֵ��
						//��Ҳ���ԣ���Ҫ��֤������ֵ����0��
						//�������������ȫ�ֵģ���Ϊ����view����ȫ�ֱ������ı��Ӱ������view����;
static char szSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM", "SEQ" };
// CtestProjectView

IMPLEMENT_DYNCREATE(CtestProjectView, CListView)

BEGIN_MESSAGE_MAP(CtestProjectView, CListView)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CtestProjectView ����/����

CtestProjectView::CtestProjectView():hthread(0)
{
	// TODO: �ڴ˴���ӹ�����룻
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
		//wait for m_hThread to be over��and wait for
		//QS_ALLINPUT��Any message is in the queue��
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
		ReleaseSemaphore(cursocktsema,1,NULL);//û�õ���WaitForSingleObject(cursocktsema)���ԡ���Ҫrelease����dll����298
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

	//ҽ�����ˣ����ơ����������//0��ʾҽ����ʱ���� 1��ʾҽ���ڣ�2��ʾҽԺҪ���ţ�
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
		if(WaitForSingleObject(pv->exesema,INFINITE)==WAIT_FAILED)//�Ҳ�����ȥ�ò��鱾����Ҫ�Ȳ���֪ͨд���ˣ���ȥ�ã�
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
		//�����°�������û= =��
		switch(pv->m_exitproc)//
		{
		case 0://��ʱ����Ҫ���Ż᣻
			*(int*)pv->lpBaseOffset = 0;
			break;
		case 2://��ʾ�°�����
			*(int*)pv->lpBaseOffset = 2;
			dh.cmd=0;
			break;
		}
		//ok������д�����ˡ����ҿ�����
		struct dataHeader* pdh = (struct dataHeader*)pv->lpDataOffset;
		
		//����д�ĸ�ʽ��û����= =��
		wchar_t bu[50]={0};
		swprintf_s(bu,50,TEXT("index:%d"),pdh->index);
		if (listCtrl)
		{
			listCtrl.InsertItem(0,bu);
		}
		//֪���˲��飬�����˿�ҩ�����������ڵ��Ŷ���
		pdh->cmd = dh.cmd;
		//MessageBox(0,TEXT("�����˿�ҩ��"),TEXT(""),0);
		//ҩ�����ã�֪ͨ������ҩ������
		if(ReleaseSemaphore(pv->cursocktsema,1,NULL)==0)
		{
			ErrorExit(TEXT("ReleaseSemaphore cursocktsema"));
		}
		if (pv->m_exitproc == 2)
		{
			//�˳��ˡ�������
			return 0;
		}
	}
}

BOOL CtestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	totalIndex =0;
	offsetgf =0;
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |= LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

// CtestProjectView ����

void CtestProjectView::OnDraw(CDC* /*pDC*/)
{
	CtestProjectDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CtestProjectView ���

#ifdef _DEBUG
void CtestProjectView::AssertValid() const
{
	CListView::AssertValid();
}

void CtestProjectView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CtestProjectDoc* CtestProjectView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CtestProjectDoc)));
	return (CtestProjectDoc*)m_pDocument;
}
#endif //_DEBUG


// CtestProjectView ��Ϣ�������


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
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));		//ע��������ʱ�����ÿ�����̵�ID��������ͬ���ź�������ֹ���Ź���
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
