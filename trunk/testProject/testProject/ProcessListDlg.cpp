// ProcessListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testProject.h"
#include "ProcessListDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include <strsafe.h>
// CProcessListDlg dialog
#define  INJECT_DLL_NAME "injectDLL.dll"
IMPLEMENT_DYNAMIC(CProcessListDlg, CDialogEx)

CProcessListDlg::CProcessListDlg(CWnd* pParent /*=NULL*/)
	:m_selPID(0),m_imglist(NULL),hookmodel(0),CDialogEx(CProcessListDlg::IDD, pParent)
{

}

CProcessListDlg::~CProcessListDlg()
{
	m_imglist->DeleteImageList();
	delete m_imglist;
}

void CProcessListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_listProcessCtrl);
}


BEGIN_MESSAGE_MAP(CProcessListDlg, CDialogEx)
	ON_BN_CLICKED(IDC_REFRESH, &CProcessListDlg::OnBnClickedRefresh)
	ON_BN_CLICKED(IDOK, &CProcessListDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_PROCESS, &CProcessListDlg::OnNMClickListProcess)
	ON_BN_CLICKED(IDC_RADIO_HOOK, &CProcessListDlg::OnBnClickedRadioHook)
	ON_BN_CLICKED(IDC_RADIO_L_REMOTE, &CProcessListDlg::OnBnClickedRadioLRemote)
END_MESSAGE_MAP()


// CProcessListDlg message handlers


BOOL CProcessListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CButton* bt = (CButton*)GetDlgItem(IDC_RADIO_HOOK);
	bt->SetCheck(true);
	UpdateData(FALSE);
	InitProcessList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


bool CProcessListDlg::InitProcessList(void)
{
	m_listProcessCtrl.DeleteAllItems();
	m_selPID = 0;
	CRect rc;
	GetClientRect(&rc);
	if(m_imglist)
	{
		m_imglist->DeleteImageList();
		delete m_imglist;
	}
	m_imglist=new CImageList();
	if(m_imglist->Create(24,24,ILC_COLOR32,0,1)==0)
		MessageBox(TEXT("a1"));
	if(m_imglist->Add(LoadIcon(0,IDI_APPLICATION))==-1)
		MessageBox(TEXT("a2"));
	m_listProcessCtrl.SetImageList(m_imglist,LVSIL_SMALL );//������LVSIL_SMALL����ʾ�ӳ����м��ص�ICON��ΪϺ�ס�
	m_listProcessCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listProcessCtrl.InsertColumn(0,TEXT("  "),LVCFMT_LEFT,50);
	m_listProcessCtrl.InsertColumn(1,TEXT("PID"),LVCFMT_LEFT,90);
	m_listProcessCtrl.InsertColumn(2,TEXT("ProcessName"),LVCFMT_LEFT,200);
	m_listProcessCtrl.InsertColumn(3,TEXT("FilePath"),LVCFMT_LEFT,300);
	//ʹ��psapi����ö�ٽ��̡�Toolhelp32������Windows98/Me/XP/2000������������Windows NT�� PSAPI������WindowsXP/2000/NT������������Windows98/Me ��
	DWORD dwPIDs[1024];
	DWORD retByte;
	if(!EnumProcesses(dwPIDs,sizeof(dwPIDs),&retByte))
	{
		MessageBox(TEXT("EnumProcesses Error!!"));
		return FALSE;
	}
	// TODO:  Add extra initialization here
	int proIndex=0,procID=0;
	CString  sproNum,sproName,sproinfo;
	int imgindex =0;SHFILEINFO   info;
	while(proIndex<(int)(retByte/sizeof(DWORD)))
	{
		procID = dwPIDs[proIndex];

		HANDLE hProc  = OpenProcess(PROCESS_ALL_ACCESS ,NULL,procID);
		if(NULL==hProc)
		{
			if (procID == 0)
				sproName.Format(TEXT("System Idle Process"));
			else
				sproName.Format(TEXT("��ʧ�ܣ�"));
		}
		else
		{
			TCHAR fname[256];
			if(GetModuleBaseName(hProc,0,fname,256)==0)
			{
				if(procID == 4)
					sproName.Format(TEXT("%s"),TEXT("System"));
			}
			else
			{
				sproName.Format(TEXT("%s"),fname);

				TCHAR fpath[256];
				GetModuleFileNameEx(hProc,0,fpath,256);
				sproinfo.Format(TEXT("%s"),fpath);

				//ʹ��SHgetFileInfo��ȡ�ļ���Ϣ�������ļ�ͼ����Ϣ�����ܷ���ĺ�����
				if(SHGetFileInfo(sproinfo,0,&info,sizeof(&info),SHGFI_DISPLAYNAME|SHGFI_ICON))
				{
					imgindex   =   m_imglist->Add(info.hIcon);
					DestroyIcon(info.hIcon);
				}

			}
		}
		sproNum.Format(TEXT("%d"),proIndex);
		int ind = m_listProcessCtrl.InsertItem(0,sproNum,imgindex);
		sproNum.Format(TEXT("%d   (0x%X)"),procID,procID);
		m_listProcessCtrl.SetItemText(ind,1,sproNum);
		m_listProcessCtrl.SetItemData(ind,procID);

		m_listProcessCtrl.SetItemText(ind,2,sproName);
		m_listProcessCtrl.SetItemText(ind,3,sproinfo);








		//.............................................
		if (hProc)
		{
			CloseHandle(hProc);
		}
		sproNum.Empty();
		sproinfo.Empty();
		proIndex++;
	}
	return true;
}


void CProcessListDlg::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	InitProcessList();
}


void CProcessListDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if(!m_selPID)
	{
		MessageBox(TEXT("û��ѡ�����!"));
		return;
	}
	switch(hookmodel)
	{
	case 0:
		if (!injectDll_windowhook())
		{
			MessageBox(TEXT("injectDll_windowhook Error!"));
			return;
		}
		break;
	case 1:
		if (!injectDll_localLoadLib())
		{
			MessageBox(TEXT("injectDll_localLoadLib Error!"));
			return;
		}
		break;
	}
	CDialogEx::OnOK();
}
void ErrorExit(LPTSTR lpszFunction) 
{ 
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError(); 

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, 
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)); 
	StringCchPrintf((LPTSTR)lpDisplayBuf, 
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"), 
		lpszFunction, dw, lpMsgBuf); 
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	//ExitProcess(dw); 
}
BOOL CProcessListDlg::injectDll_localLoadLib(void)
{

	DWORD thid=0;
	LPVOID p_rmtDllName;
	WCHAR filename[256]={0};
	GetModuleFileName(NULL,filename,256);
	CString fcs = filename;
	int ind = fcs.ReverseFind(TEXT('\\'));
	fcs = fcs.Left(ind);
	fcs.Append(TEXT("\\")); 
	fcs.Append(TEXT(INJECT_DLL_NAME));
	HANDLE hProc  = OpenProcess(PROCESS_ALL_ACCESS ,NULL,m_selPID);
	p_rmtDllName = VirtualAllocEx(hProc,0,256*2+2,MEM_COMMIT|MEM_TOP_DOWN ,PAGE_READWRITE);
	if (!p_rmtDllName)
	{
		ErrorExit(TEXT(""));
		MessageBox(L"VirtualAllocEx����.!");
		return false;
	}
	SIZE_T writeByte;
	if(!WriteProcessMemory(hProc,p_rmtDllName,fcs.GetBuffer(),256*2+2,&writeByte))
	{
		ErrorExit(TEXT(""));
		MessageBox(L"WriteProcessMemory����.");
		return false;
	}
	fcs.ReleaseBuffer();
	LPTHREAD_START_ROUTINE ffw = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("kernel32.dll")),"LoadLibraryW");
	HANDLE ch = CreateRemoteThread(hProc,0,0,ffw,p_rmtDllName,0,&thid);
	if (!ch)
	{
		MessageBox(L"CreateRemoteThread ����.");
		return false;
	}
	DWORD rthe = WaitForSingleObject(ch,INFINITE);
	return true;
}
BOOL CProcessListDlg::injectDll_windowhook()
{
	// TODO: Add your control notification handler code here

	HHOOK m_hook;
	//::SetForegroundWindow(selWnd);
	//dwThreadID = GetWindowThreadProcessId(selWnd,&dwProcessID);
	HOOKPROC hkprcSysMsg; 
	static HINSTANCE hinstDLL;
	typedef   int   (CALLBACK*   LPFNDLLFUNC)(DWORD); 
	hinstDLL = LoadLibrary(TEXT(INJECT_DLL_NAME)); 
	if (hinstDLL == NULL)
	{
		ErrorExit(TEXT(""));
		return FALSE;
	}
	hkprcSysMsg = (HOOKPROC)GetProcAddress(hinstDLL, "DllHookGetMsg");
	//��ȡָ������ID�Ķ�Ӧ�����߳�ID��
	DWORD dwThreadID = 0;
	THREADENTRY32 te32 = {sizeof(te32)};
	HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if( Thread32First( hThreadSnap, &te32) )
	{
		do{
			if( m_selPID == te32.th32OwnerProcessID )
			{
				dwThreadID = te32.th32ThreadID;
				break;
			}
		}while( Thread32Next( hThreadSnap, &te32) );
	}

	m_hook = SetWindowsHookEx(WH_GETMESSAGE,(HOOKPROC)hkprcSysMsg,hinstDLL,dwThreadID);
	if (!m_hook)
	{
		ErrorExit(TEXT(""));
		FreeLibrary(hinstDLL);
		return FALSE;
	}
	FreeLibrary(hinstDLL);
	return TRUE;
}
void CProcessListDlg::OnNMClickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_selPID = m_listProcessCtrl.GetItemData(pNMItemActivate->iItem);
	m_processName = m_listProcessCtrl.GetItemText(pNMItemActivate->iItem,2);
	*pResult = 0;
}

void CProcessListDlg::OnBnClickedRadioHook()
{
	// TODO: Add your control notification handler code here
	hookmodel = 0;
}

void CProcessListDlg::OnBnClickedRadioLRemote()
{
	// TODO: Add your control notification handler code here
	hookmodel = 1;
}