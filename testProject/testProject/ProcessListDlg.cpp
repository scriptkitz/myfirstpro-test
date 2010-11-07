// ProcessListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "testProject.h"
#include "ProcessListDlg.h"
#include "afxdialogex.h"
#include <Psapi.h>

// CProcessListDlg dialog

IMPLEMENT_DYNAMIC(CProcessListDlg, CDialogEx)

CProcessListDlg::CProcessListDlg(CWnd* pParent /*=NULL*/)
	:m_selPID(0),m_imglist(NULL),CDialogEx(CProcessListDlg::IDD, pParent)
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
END_MESSAGE_MAP()


// CProcessListDlg message handlers


BOOL CProcessListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	InitProcessList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


bool CProcessListDlg::InitProcessList(void)
{
	m_listProcessCtrl.DeleteAllItems();
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
	m_listProcessCtrl.SetImageList(m_imglist,LVSIL_SMALL );//必须用LVSIL_SMALL才显示从程序中加载的ICON。为虾米。
	m_listProcessCtrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_listProcessCtrl.InsertColumn(0,TEXT("  "),LVCFMT_LEFT,50);
	m_listProcessCtrl.InsertColumn(1,TEXT("PID"),LVCFMT_LEFT,90);
	m_listProcessCtrl.InsertColumn(2,TEXT("ProcessName"),LVCFMT_LEFT,200);
	m_listProcessCtrl.InsertColumn(3,TEXT("FilePath"),LVCFMT_LEFT,300);
	//使用psapi函数枚举进程。Toolhelp32适用于Windows98/Me/XP/2000，但不适用于Windows NT。 PSAPI适用于WindowsXP/2000/NT，但不适用于Windows98/Me 。
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
				sproName.Format(TEXT("打开失败！"));
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

				//使用SHgetFileInfo获取文件信息，包含文件图标信息。。很方便的函数。
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
		MessageBox(TEXT("没有选择进程!"));
		return;
	}
	CDialogEx::OnOK();
}


void CProcessListDlg::OnNMClickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	m_selPID = m_listProcessCtrl.GetItemData(pNMItemActivate->iItem);
	*pResult = 0;
}
