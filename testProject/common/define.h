
#ifndef DEFINE_FILE_ONCE
#define DEFINE_FILE_ONCE

#include <strsafe.h>
#include "stdafx.h"

#pragma once

#define M_SIZE_SEMA 50
#define  MAPPINGFILESIZE 1024*100
#define  INJECT_DLL_NAME "injectDLL.dll"
#define READ_SEMAPHORE   "z_j_2010_dll_scriptkit_read"
#define WRITE_SEMAPHORE  "z_j_2010_dll_scriptkit_write"
#define MUTEX_STRING "Z_J_exe_2010_mutex_kk"
struct dataHeader
{
	bool tofrom;//true to exe,false to dll
	char sockType;//��������..char szSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM", "SEQ" };
	int method; //���ݹ���.1:send 2:recv 3:sendto 4:recvfrom 
	size_t size;//���ݴ�С
	//DWORD threadID;//
	//int index;//����������
	//int offset;//����ƫ��ָ���ַ.
	struct sockaddr_in  localsock;
	struct sockaddr_in  remotesock;
	
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