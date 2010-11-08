#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CProcessListDlg dialog

class CProcessListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CProcessListDlg)

public:
	CProcessListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProcessListDlg();

// Dialog Data
	enum { IDD = IDD_PROCESS_LIST };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_listProcessCtrl;
	CImageList *m_imglist;
	CString m_processName;
	int hookmodel; //0-- hook api, 1-- local loadlibray careteremotethread. 2.....
	DWORD m_selPID;
	bool InitProcessList(void);
	BOOL injectDll_localLoadLib(void);//
	BOOL injectDll_windowhook();
	afx_msg void OnBnClickedRefresh();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMClickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioHook();
	afx_msg void OnBnClickedRadioLRemote();
};
