
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
END_MESSAGE_MAP()

// CtestProjectView ����/����

CtestProjectView::CtestProjectView():hthread(0)
{
	// TODO: �ڴ˴���ӹ�����룻
	m_exitproc = 0;
	readsema = 0;
	writesema = 0;
	lpBaseOffset = 0;
	hsgFile = 0;
}

CtestProjectView::~CtestProjectView()
{
	m_exitproc = 1;
	if (readsema)
	{
		ReleaseSemaphore(readsema,1,NULL);
		CloseHandle(readsema);
	}
	if (readsema)
	{
		ReleaseSemaphore(writesema,1,NULL);
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
	Sleep(1000);
	CtestProjectView* pv = (CtestProjectView*)lpParameter;
	DWORD pid = pv->GetDocument()->m_docSelPID;
	HANDLE hdr = OpenProcess(PROCESS_ALL_ACCESS,NULL,pid);
	CListCtrl& listCtrl = pv->GetListCtrl();
	int ta = 0,err =0,la =0;
	HANDLE hd=0;
	HANDLE tarhd=0;
	while(1)
	{
		if(WaitForSingleObject(pv->readsema,INFINITE)==WAIT_FAILED)
		{
			ErrorExit(TEXT("WaitForSingleObject"));
			return 0;
		}
		if (pv->m_exitproc != 1 && pv->m_exitproc != 0)
		{
			return 0;
		}
		try
		{
			la = *(int*)pv->lpBaseOffset;
			hd = *(HANDLE*)((char*)pv->lpBaseOffset+4);
		}
		catch (CException* e)
		{
			return 0;
		}
		
		if (la != ta)
		{
			wchar_t bu[50]={0};
			if (!tarhd)
			{
				//static HANDLE tarhd=0;//���������������������һ�����ƵĴ��󣬼���һ������û���⣬�ڶ�ʱ��ᵼ������������ԭ��������������staic�ˡ������ơ��Ѿ����ϱ��ˣ���ȥ����static��
				if(DuplicateHandle(hdr,hd,GetCurrentProcess(),&tarhd,0, FALSE, DUPLICATE_SAME_ACCESS)==0)
				{
					ErrorExit(TEXT("DuplicateHandle"));
				}
			}
			swprintf_s(bu,50,TEXT("index:%d handle:%d duplicateH:%d"),la,hd,tarhd);
			listCtrl.InsertItem(0,bu);
			//MessageBox(0,TEXT("aaaaaaaaaaaaaaa"),TEXT(""),0);
			if(ReleaseSemaphore(tarhd,1,NULL)==0)
			{
				ErrorExit(TEXT("ReleaseSemaphore1"));
			}

			ta = la;
		}
		if(ReleaseSemaphore(pv->readsema,1,NULL)==0)
		{
			ErrorExit(TEXT("ReleaseSemaphore2"));
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

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(WRITE_SEMAPHORE));		//ע��������ʱ�����ÿ�����̵�ID��������ͬ���ź�������ֹ���Ź���
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		writesema = CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		readsema =  CreateSemaphore(NULL,1,1,chmu);
		//MessageBox(chmu,TEXT("bbbbbbbbbbbbb"),0);
		hthread = CreateThread(NULL,NULL,ThreadProc,this,0,0);
	}
}
