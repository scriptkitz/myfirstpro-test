
#ifndef DEFINE_FILE_ONCE
#define DEFINE_FILE_ONCE

#include <strsafe.h>
#include "stdafx.h"

#pragma once

#define M_SIZE_SEMA 50 //信号量字符串长度;
#define  MAPPINGFILESIZE 1024*100 //createfilemapping大小;
#define  INJECT_DLL_NAME "injectDLL.dll" //dll文件名；
#define DLL_SEMAPHORE	"zj_dll_scriptkit_sema_" //DLL socket用信号量，领号排队，讲文明啊哈
#define READ_SEMAPHORE   "z_j_2010_comm_scriptkit_read"  //读信号量；
#define WRITE_SEMAPHORE  "z_j_2010_comm_scriptkit_write" //写信号量；
#define MUTEX_STRING "Z_J_exe_2010_mutex_kk"	//互斥量
#define FILE_MAPPING_STRING "zj_file_mapping_"
struct dataHeader
{
	char sockType;//链接类型..char szSockType[6][6] = { "NUL", "TCP", "UDP", "RAW", "RDM", "SEQ" };
	int cmd;	//命令。。exe对dll发送的命令。 0表示退出命令，要退出了。1表示不处理，直接通过，2表示拦截这次发送或者接收。3表示修改，修改这次的数据内容。
	int method; //数据功能.1:send 2:recv 3:sendto 4:recvfrom 
	size_t size;//数据大小
	int index;//
	struct sockaddr_in  localsock; //目标ip，端口
	struct sockaddr_in  remotesock;//目标的连接的ip，端口
};

inline void ErrorExit(LPTSTR lpszFunction) //= =，图个方便，写这里，但多个文件引用，引起重复定义编译错误，= =，懒的分开放了，直接写为inline，内联函数，谁调用谁新增代码= =，
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