// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "detours.h"
#include <WinSock2.h>
#include <wchar.h>
#include <Strsafe.h>
/* //20
#include <stdlib.h>
#include <string.h>
#include "define.h"

#define SIZE_BUF 28
*/
#pragma comment(lib,"ws2_32.lib")
HANDLE hsgFile = NULL;
static LONG dwSlept = 0;
LPVOID lpBaseOffset =0, offsetgf=0;
struct dataHeader
{
	int type; //数据类型.1:send 2:recv 3:sendto 4:recvfrom 
	size_t size;//数据大小
	int typeIndex;//相关类型索引.
	int totalindex;//数据总索引
	char* offset;//数据偏移指针地址.

};
// Target pointer for the uninstrumented Sleep API.
int (WSAAPI * Real_send)(SOCKET s, const char * buf, int len, int flags) = send;
int (WSAAPI * Real_sendto)(SOCKET s, const char * buf, int len, int flags,const struct sockaddr * to, int tolen)  = sendto;
int (WSAAPI * Real_recv)(SOCKET s, char * buf, int len, int flags) = recv;
int (WSAAPI * Real_recvfrom)(SOCKET s,char * buf, int len, int flags,struct sockaddr * from, int * fromlen) = recvfrom;

// Detour function that replaces the Sleep API.

static int WSAAPI Catch_send(SOCKET s, const char * buf, int len, int flags)
{
	struct dataHeader dh;
	dh.type = 1;
	dh.size = len;
	dh.offset = (char*)offsetgf-lpBaseOffset;
	memcpy(offsetgf,&dh,sizeof(struct dataHeader));
	offsetgf = (char*)offsetgf + sizeof(struct dataHeader);
	memcpy(offsetgf,buf,len);
	offsetgf = (char*)offsetgf + len;
	return Real_send(s, buf, len, flags);
}
static int WSAAPI Catch_sendto(SOCKET s,const char * buf, int len, int flags, const struct sockaddr * to, int tolen)
{
	return Real_sendto(s,buf, len, flags,to, tolen);
}
static int WSAAPI Catch_recv(SOCKET s, char * buf, int len, int flags)
{
	return Real_recv(s, buf, len, flags);
}

static int WSAAPI Catch_recvfrom(SOCKET s,char * buf, int len, int flags,struct sockaddr * from, int * fromlen)
{
	return Real_recvfrom(s, buf, len, flags, from, fromlen);
}

// DllMain function attaches and detaches the TimedSleep detour to the
// Sleep target function.  The Sleep target function is referred to
// through the TrueSleep target pointer.


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	/* //20
	DWORD ch;
	TCHAR chmu[SIZE_BUF]={0};
	static HANDLE hMutex = NULL;
	*/
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//不需要创建互斥来判断多次加载，因为不管加载多少次这里只会调用一次，第一次加载时候。。呵呵，大概吧。
		//就算多次调用loadlibray也只会运行一次，这样就直接在这里写hookapi代码吧。
		/* //20
		ch = GetCurrentProcessId();//GetCurrentThreadId(); //GetCurrentProcess();
#ifdef UNICODE
		_itow_s((int)ch,chmu,SIZE_BUF,10);
#else
		_itoa_s((int)ch,chmu,SIZE_BUF,10);
#endif
		GetModuleHandle(NULL);
		wcscat_s(chmu,SIZE_BUF,TEXT(MYSTRING));
		MessageBox(0,chmu,TEXT(""),0);
		hMutex = CreateMutex(0,FALSE,chmu);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CloseHandle(hMutex);
			hMutex = NULL;
			return 0;
		}
		*/
		hsgFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,10240,TEXT("woleigecagaga"));
		lpBaseOffset = MapViewOfFile(hsgFile,FILE_MAP_ALL_ACCESS,0,0,0);
		offsetgf = lpBaseOffset;
		if (hsgFile == NULL)
		{
			MessageBox(0,TEXT(""),TEXT(""),0);
			return false;
		}
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)Real_send, Catch_send);
		DetourAttach(&(PVOID&)Real_sendto, Catch_sendto);
		DetourAttach(&(PVOID&)Real_recv, Catch_recv);
		DetourAttach(&(PVOID&)Real_recvfrom, Catch_recvfrom);
		DetourTransactionCommit();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourDetach(&(PVOID&)Real_send, Catch_send);
		DetourDetach(&(PVOID&)Real_sendto, Catch_sendto);
		DetourDetach(&(PVOID&)Real_recv, Catch_recv);
		DetourDetach(&(PVOID&)Real_recvfrom, Catch_recvfrom);
		DetourTransactionCommit();
		UnmapViewOfFile(lpBaseOffset);
		CloseHandle(hsgFile);
		break;
	}
	return TRUE;
}

