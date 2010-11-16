// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "detours.h"
#include <WinSock2.h>
#include <wchar.h>
#include <Strsafe.h>
#include "define.h"
#include <stdlib.h>
/* //20
#include <stdlib.h>
#include <string.h>
#include "define.h"

#define SIZE_BUF 28
*/
#pragma comment(lib,"ws2_32.lib")
HANDLE hsgFile = NULL;
LPVOID lpBaseOffset =0,lpDataOffset=0;
int offsetgf=0;
int totalIndex=0;
struct dataHeader dh;//
HANDLE exesema=0 ,cursocktsema=0,socketsema =0; //信号量句柄
HANDLE hMutex = NULL;
bool exitprosng = false;

// Target pointer for the uninstrumented Sleep API.
int (WSAAPI * Real_send)(SOCKET s, const char * buf, int len, int flags) = send;
int (WSAAPI * Real_sendto)(SOCKET s, const char * buf, int len, int flags,const struct sockaddr * to, int tolen)  = sendto;
int (WSAAPI * Real_recv)(SOCKET s, char * buf, int len, int flags) = recv;
int (WSAAPI * Real_recvfrom)(SOCKET s,char * buf, int len, int flags,struct sockaddr * from, int * fromlen) = recvfrom;

// Detour function that replaces the Sleep API.

int tt = 0;
static void writeDataFM(SOCKET s,int method,const char* buf,int &len) // len是会被修改返回的。所以不要改乱
{
	//看看有没医院要关门的牌子。；
	if (exitprosng)
	{
		return;//要关门了？不取号了。走人；
	}
	//取号占位哈。。哥排队。哥文明；
	if( WAIT_FAILED == WaitForSingleObject(socketsema,INFINITE))
	{
		ErrorExit(TEXT("WaitForSingleObject socketsema"));
	}
	//看看有没医院要关门的牌子。。；
	if (exitprosng)
	{
		ReleaseSemaphore(socketsema,1,NULL);//丢了号。走人；
		return;
	}
	//看看小窗口，有医生出诊么，如果没有医生在，判断一下是临时不在，还是医院要关门了。；
	int hasdct = *(int*)lpBaseOffset;//0表示医生临时不在 1表示医生在，2表示医院要关门。
	switch(hasdct)
	{
	case 0:	//0表示医生临时不在。那么就走人吧，不等了，让位子，请下一位。
		ReleaseSemaphore(socketsema,1,NULL);
		return;
		break;
	case 2:	//2表示医院要关门，那么写个大牌子(exitprosng),通知所有取了号(调用了WaitForSingleObject的)人，丢了号，走人吧,没取号的，不要取了。要关门了。并自己丢号走人。
		exitprosng = true;
		ReleaseSemaphore(socketsema,1,NULL);
		return;
		break;
	}
	//OK，有医生在，而且在等待读。写病情。；
	struct dataHeader* phd = (struct dataHeader*)lpDataOffset;
	phd->index++;
	//dh.index++;
	if(ReleaseSemaphore(exesema,1,NULL)==0)//写完了，给医生读。释放。
	{
		exitprosng = true;
		//ErrorExit(TEXT("ReleaseSemaphore exesema")); 这里由于exe关闭时候冲突时候出错，但这里会导致崩溃。先屏蔽了把。
		return;
	}
	else if(WAIT_FAILED == WaitForSingleObject(cursocktsema,INFINITE))////病人等待医生返回信息才能去开药； 挂起，等待exe来释放。
	{
		ErrorExit(TEXT("WaitForSingleObject cursocktsema"));
	}
	//医生通知药方写好。读药方。。
	memcpy(&dh,lpDataOffset, sizeof(struct dataHeader));
	switch(dh.cmd)
	{
		//命令。。exe对dll发送的命令。 0表示退出命令，要退出了。1表示不处理，直接通过，2表示拦截这次发送或者接收。3表示修改，修改这次的数据内容。
	case 0:
		exitprosng = true;
		break;
	}
	//看看医生的命令。。；
	//OK。。下一位。。。
	if(0==ReleaseSemaphore(socketsema,1,NULL))
	{
		ErrorExit(TEXT("ReleaseSemaphore socketsema"));
	}

	/*
	totalIndex++;
	int iRet=0;
	dh.method = method;
	dh.size = len;
	dh.index = totalIndex;
	dh.tofrom = true;
	//=================
	int namelen = sizeof(sockaddr_in);
	iRet = getsockname( s, (sockaddr*)&dh.localsock, &namelen );//获取本机IP和端口..
	if ( iRet != SOCKET_ERROR )
	{
		int sockType = 0;
		int optlen = 4;
		iRet = getsockopt( s, SOL_SOCKET, SO_TYPE, (char*)&sockType, &optlen );//获取链接类型
		dh.sockType = sockType;
		//+++++++++++ 显示Socket的进程ID、端口、类型、目的IP和端口 ++++++++++++
		int peernamelen = sizeof(sockaddr_in);
		iRet = getpeername( s, (sockaddr*)&dh.remotesock, &peernamelen );//获取目的IP和端口..
	}
	//===================
	dh.offset = offsetgf+sizeof(struct dataHeader);
	
	dh.threadID = GetCurrentThreadId();
	memcpy(LPVOID(offsetgf + (char*)lpBaseOffset),&dh,sizeof(struct dataHeader));

	offsetgf = offsetgf + sizeof(struct dataHeader);

	memcpy(LPVOID(offsetgf+(char*)lpBaseOffset),buf,len);

	offsetgf = offsetgf + len;
	*(int*)(offsetgf+(char*)lpBaseOffset) = -1;
	
	ReleaseSemaphore(hos,1,NULL);
	CloseHandle(hos);
	int count = 100;
	while(1)
	{
		HANDLE hos = OpenSemaphore(SEMAPHORE_ALL_ACCESS,false,TEXT(MYSTRING));
		WaitForSingleObject(hos,INFINITE);
		memcpy(&dh,LPVOID((char*)lpBaseOffset + (offsetgf - sizeof(struct dataHeader) -len) ),sizeof(struct dataHeader));
		if ((dh.index == totalIndex+1 && dh.tofrom == false)|| count==0)
		{
			offsetgf = offsetgf -dh.size -  sizeof(struct dataHeader);
			ReleaseSemaphore(hos,1,NULL);
			CloseHandle(hos);
			break;
		}
		ReleaseSemaphore(hos,1,NULL);
		CloseHandle(hos);
	}
	*/
}
static int WSAAPI Catch_send(SOCKET s, const char * buf, int len, int flags)
{
	writeDataFM(s,1,buf,len);//可以修改发送的内容。。。。
	return Real_send(s, buf, len, flags);
}
static int WSAAPI Catch_sendto(SOCKET s,const char * buf, int len, int flags, const struct sockaddr * to, int tolen)
{
	writeDataFM(s,3,buf,len);//可以修改发送的内容。。。。
	return Real_sendto(s,buf, len, flags,to, tolen);
}
static int WSAAPI Catch_recv(SOCKET s, char * buf, int len, int flags)
{
	//应该先吧返回的内容放在自己buf里，然后处理完了在放回他的buf。
	char *sbuf = new char[len];
	int retlen = Real_recv(s, sbuf, len, flags);
	writeDataFM(s,2,sbuf,retlen);
	if (retlen > len)
	{
		retlen = len;
		MessageBox(0,TEXT("接收的数据buffer过小啦。"),TEXT("错误"),0);
	}
	memcpy_s(buf,len,sbuf,retlen);
	return retlen;
}

static int WSAAPI Catch_recvfrom(SOCKET s,char * buf, int len, int flags,struct sockaddr * from, int * fromlen)
{
	int retlen = Real_recvfrom(s, buf, len, flags, from, fromlen);
	writeDataFM(s,4,buf,retlen);
	return retlen;
}

// DllMain function attaches and detaches the TimedSleep detour to the
// Sleep target function.  The Sleep target function is referred to
// through the TrueSleep target pointer.


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	DWORD ch;
	TCHAR chmu[M_SIZE_SEMA]={0},ids[10]={0};
	
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		//多一个进程运行多次loadlibrary，也只会运行一次DLL_PROCESS_ATTACH。如果已经加载就不会运行这里了。
		//判断一下是否为exe程序调用的loadlibrary调用的代码。是的话就不再创建filemapping等。;
		//Sleep(500);
		ch = GetCurrentProcessId();

		_itow_s((int)ch,ids,10,10);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(MUTEX_STRING));
		wcscat_s(chmu,M_SIZE_SEMA,ids);
		//MessageBox(0,chmu,TEXT(""),0);
		hMutex = CreateMutex(0,FALSE,chmu);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
			hMutex = NULL;
			//MessageBox(0,TEXT("EXE中的loadlibrary！"),chmu,0);
			return TRUE;
		}
		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(FILE_MAPPING_STRING));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		//MessageBox(0,chmu,TEXT("CreateFileMapping"),0);
		hsgFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,MAPPINGFILESIZE,chmu);
		lpBaseOffset = MapViewOfFile(hsgFile,FILE_MAP_ALL_ACCESS,0,0,0);
		lpDataOffset = (char*)lpBaseOffset+4; //前4位的int留着用来判断exe是否已经退出注入。
		//
		//注入多个进程时候根据每个进程的ID，创建不同的信号量。防止串号哈。;
		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(WRITE_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		exesema = CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		cursocktsema =  CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(DLL_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		socketsema =  CreateSemaphore(NULL,1,1,chmu);
		//MessageBox(0,chmu,TEXT("bbbbbbbbbbbbb"),0);
		if (hsgFile == NULL)
		{
			MessageBox(0,TEXT("CreateFileMapping"),TEXT("error"),0);
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
		if (hMutex)
		{
			ReleaseMutex(hMutex);
			CloseHandle(hMutex);
		}
		if (exesema)
		{
			ReleaseSemaphore(exesema,1,NULL);
			CloseHandle(exesema);
		}
		if (cursocktsema)
		{
			ReleaseSemaphore(cursocktsema,1,NULL);
			CloseHandle(cursocktsema);
		}
		if (socketsema)
		{
			ReleaseSemaphore(socketsema,1,NULL);
			CloseHandle(socketsema);
		}
		if (lpBaseOffset)
		{
			UnmapViewOfFile(lpBaseOffset);
		}
		if (hsgFile)
		{
			CloseHandle(hsgFile);
		}
		break;
	}
	return TRUE;
}

