
// testProject.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "testProject.h"
#include "MainFrm.h"
#include "ProcessListDlg.h"
#include "ChildFrm.h"
#include "testProjectDoc.h"
#include "testProjectView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CtestProjectApp

BEGIN_MESSAGE_MAP(CtestProjectApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CtestProjectApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND_RANGE(USER_INDEX,USER_INDEX+MAX_DOC_CONT,funTabBtnCmd)
	ON_UPDATE_COMMAND_UI_RANGE(USER_INDEX,USER_INDEX+MAX_DOC_CONT,funUpdateTabBtnCmd)
END_MESSAGE_MAP()


// CtestProjectApp ����

CtestProjectApp::CtestProjectApp()
{

	// TODO: �ڴ˴����ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CtestProjectApp ����

CtestProjectApp theApp;


// CtestProjectApp ��ʼ��

BOOL CtestProjectApp::InitInstance()
{
	//��������Ȩ�ޡ�����
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
	//

	CWinApp::InitInstance();
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_testProjectTYPE,
		RUNTIME_CLASS(CtestProjectDoc),
		RUNTIME_CLASS(CChildFrame), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CtestProjectView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������ MDI ��ܴ���
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� MDI Ӧ�ó����У���Ӧ������ m_pMainWnd ֮����������
	// ������/��
	m_pMainWnd->DragAcceptFiles();

	// ���á�DDE ִ�С�
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// ������׼������DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// �������ѳ�ʼ���������ʾ����������и���
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();
	for (int i=0;i<MAX_DOC_CONT;i++)
	{
		m_validIndex.push_back(USER_INDEX+i);
	}
	return TRUE;
}



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
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
// CtestProjectApp ��Ϣ��������

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
		 MessageBox(m_pMainWnd->GetSafeHwnd(),TEXT("�����㴴����...����"),TEXT("��ʾ��ܰ��"),0);
		 return false;
	 }
	 CProcessListDlg pdlg;
	 if (pdlg.DoModal() == IDCANCEL)
	 {
		 return FALSE;
	 }
	 //����������
	 testDoc->m_docSelPID = pdlg.m_selPID;
	 testDoc->SetTitle(pdlg.m_processName);
	 CString title = testDoc->GetTitle();
	 if (m_docBtnMap.find(title) != m_docBtnMap.end())
	 {
		 return false;
	 }
	 
	 
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
	 CtestProjectDoc *activDoc = (CtestProjectDoc *)mf->MDIGetActive()->GetActiveDocument();//��ȡ��ǰ��doc��
	 CDocTemplate *dt = activDoc->GetDocTemplate();//��ȡdocģ�塣
	 POSITION posdoc = dt->GetFirstDocPosition();//��ʼ����docģ�塣
	 while(posdoc)
	 {
		CtestProjectDoc* docs = (CtestProjectDoc*)dt->GetNextDoc(posdoc);
		if(tabId == docs->m_btnID)
		{
			POSITION pos = docs->GetFirstViewPosition();
			CtestProjectView* cv = (CtestProjectView*)docs->GetNextView(pos);
			mf->MDIActivate(cv->GetParentFrame());//����ѡ�еİ�ť��Ӧ��ID��doc��ID��ͬ��cv��parentframe--��childframe��
			break;
		}
		
	 }
	//���õ�ǰtabID��
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
		cbt->SetButtonStyle(oldstyle & ~BS_DEFPUSHBUTTON);//����UI��ʱ�����Ƿ�ť�кڱߣ��о��������ѿ�������
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