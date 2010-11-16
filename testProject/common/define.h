
#ifndef DEFINE_FILE_ONCE
#define DEFINE_FILE_ONCE

#include <strsafe.h>
#include "stdafx.h"

#pragma once

#define M_SIZE_SEMA 50 //�ź����ַ�������;
#define  MAPPINGFILESIZE 1024*100 //createfilemapping��С;
#define  INJECT_DLL_NAME "injectDLL.dll" //dll�ļ�����
#define DLL_SEMAPHORE	"zj_dll_scriptkit_sema_" //DLL socket���ź���������Ŷӣ�����������
#define READ_SEMAPHORE   "z_j_2010_comm_scriptkit_read"  //���ź�����
#define WRITE_SEMAPHORE  "z_j_2010_comm_scriptkit_write" //д�ź�����
#define MUTEX_STRING "Z_J_exe_2010_mutex_kk"	//������
#define FILE_MAPPING_STRING "zj_file_mapping_"
struct dataHeader
{
	char sockType;//��������..char szSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM", "SEQ" };
	int cmd;	//�����exe��dll���͵���� 0��ʾ�˳����Ҫ�˳��ˡ�1��ʾ������ֱ��ͨ����2��ʾ������η��ͻ��߽��ա�3��ʾ�޸ģ��޸���ε��������ݡ�
	int method; //���ݹ���.1:send 2:recv 3:sendto 4:recvfrom 
	size_t size;//���ݴ�С
	int index;//
	struct sockaddr_in  localsock; //Ŀ��ip���˿�
	struct sockaddr_in  remotesock;//Ŀ������ӵ�ip���˿�
};

inline void ErrorExit(LPTSTR lpszFunction) //= =��ͼ�����㣬д���������ļ����ã������ظ�����������= =�����ķֿ����ˣ�ֱ��дΪinline������������˭����˭��������= =��
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
#endif