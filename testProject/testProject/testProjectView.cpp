
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
}

CtestProjectView::~CtestProjectView()
{
	m_exitproc = 1;
	//Sleep(1000);
	ReleaseSemaphore(readsema,1,NULL);
	CloseHandle(readsema);
	UnmapViewOfFile(lpBaseOffset);
	CloseHandle(hsgFile);
}
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	Sleep(1000);
	CtestProjectView* pv = (CtestProjectView*)lpParameter;
	HANDLE hdr = OpenProcess(PROCESS_ALL_ACCESS,NULL, pv->GetDocument()->m_docSelPID);
	CListCtrl& listCtrl = pv->GetListCtrl();
	int ta = 0,err =0;
	while(1)
	{
		//pv->readsema = OpenSemaphore(SEMAPHORE_ALL_ACCESS,false,TEXT(READ_SEMAPHORE));
		if (pv->m_exitproc)
		{
			//�����ʱ�˳��߳��Ǹ����⡣�������������롣��
			return 0;
		}
		WaitForSingleObject(pv->readsema,INFINITE);
		int la = *(int*)pv->lpBaseOffset;
		HANDLE hd = *(HANDLE*)((char*)pv->lpBaseOffset+4);
		if (la != ta)
		{
			wchar_t bu[50]={0};
			
			static HANDLE tarhd=0;
			if (!tarhd)
			{
				if(DuplicateHandle(hdr,hd,GetCurrentProcess(),&tarhd,0, FALSE, DUPLICATE_SAME_ACCESS)==0)
				{
					wchar_t dd[5];
					err = GetLastError();
					_itow_s(err,dd,5,10);
					MessageBox(0,TEXT("DuplicateHandle"),dd,0);
				}
			}
			swprintf_s(bu,50,TEXT("index:%d handle:%d duplicateH:%d"),la,hd,tarhd);
			listCtrl.InsertItem(0,bu);
			//MessageBox(0,TEXT("aaaaaaaaaaaaaaa"),TEXT(""),0);
			if(ReleaseSemaphore(tarhd,1,NULL)==0)
			{
				err = GetLastError();
				MessageBox(0,TEXT("ReleaseSemaphore"),TEXT("1"),0);
			}

			ta = la;
		}
		if(ReleaseSemaphore(pv->readsema,1,NULL)==0)
		{
			err = GetLastError();
			MessageBox(0,TEXT("ReleaseSemaphore"),TEXT("22222"),0);
		}
		//CloseHandle(pv->readsema);
	}

	
	
}

BOOL CtestProjectView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	

	hsgFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,MAPPINGFILESIZE,TEXT("woleigecagaga"));
	lpBaseOffset = MapViewOfFile(hsgFile,FILE_MAP_ALL_ACCESS,0,0,0);

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
