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
LPVOID lpBaseOffset =0;
int offsetgf=0;
int totalIndex=0;
struct dataHeader dh;//
HANDLE writesema=0 ,readsema=0; //�ź������
HANDLE hMutex = NULL;

// Target pointer for the uninstrumented Sleep API.
int (WSAAPI * Real_send)(SOCKET s, const char * buf, int len, int flags) = send;
int (WSAAPI * Real_sendto)(SOCKET s, const char * buf, int len, int flags,const struct sockaddr * to, int tolen)  = sendto;
int (WSAAPI * Real_recv)(SOCKET s, char * buf, int len, int flags) = recv;
int (WSAAPI * Real_recvfrom)(SOCKET s,char * buf, int len, int flags,struct sockaddr * from, int * fromlen) = recvfrom;

// Detour function that replaces the Sleep API.

int tt = 0;
static void writeDataFM(SOCKET s,int method,const char* buf,int len)
{
	if( WAIT_FAILED == WaitForSingleObject(writesema,INFINITE))//��ֹ�Լ�д
	{
		ErrorExit(TEXT("WaitForSingleObject writesema"));
	}
	//MessageBox(0,TEXT("��ֹ�Լ�д"),TEXT("dll"),0);

	/*
	wchar_t slen[10];
	switch(method)
	{
	case 1:
		tt = len;
		_itow_s(tt,slen,10,10);
		MessageBox(0,TEXT("send"),slen,0);
		break;
	case 2:
		tt += len;
		_itow_s(tt,slen,10,10);
		MessageBox(0,TEXT("recv"),slen,0);
		break;
	case 3:
		break;
	case 4:
		break;
	}
	*/
	int ia = *(int*)lpBaseOffset;
	ia++;
	*(int*)lpBaseOffset = ia;
	*(int*)((char*)lpBaseOffset +4) = (int)writesema;
	if(ReleaseSemaphore(readsema,1,NULL)==0)//д���ˣ������Զ��ˡ��ͷš�
	{
		ErrorExit(TEXT("ReleaseSemaphore readsema"));
	}
	if(WAIT_FAILED == WaitForSingleObject(writesema,INFINITE))//��д�������𣬵ȴ�exe���ͷš�
	{
		ErrorExit(TEXT("WaitForSingleObject writesema2"));
	}
	//����д�����롣���Ǳ��Ѿ���д�����ݡ��������˸���������
	int bbt = *(int*)((char*)lpBaseOffset +8);
	*(int*)((char*)lpBaseOffset +8) = bbt+1;
	//if( WAIT_FAILED == WaitForSingleObject(readsema,INFINITE))//��ֹ����
	//{
	//	ErrorExit(TEXT("WaitForSingleObject readsema"));
	//}

	//MessageBox(0,TEXT("��ֹ����"),TEXT("dll"),0);

	//MessageBox(0,TEXT("ͨ��exe�ͷ���һ�Ρ�"),TEXT("dll"),0);
	if(0==ReleaseSemaphore(writesema,1,NULL))
	{
		ErrorExit(TEXT("ReleaseSemaphore writesema"));
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
	iRet = getsockname( s, (sockaddr*)&dh.localsock, &namelen );//��ȡ����IP�Ͷ˿�..
	if ( iRet != SOCKET_ERROR )
	{
		int sockType = 0;
		int optlen = 4;
		iRet = getsockopt( s, SOL_SOCKET, SO_TYPE, (char*)&sockType, &optlen );//��ȡ��������
		dh.sockType = sockType;
		//+++++++++++ ��ʾSocket�Ľ���ID���˿ڡ����͡�Ŀ��IP�Ͷ˿� ++++++++++++
		int peernamelen = sizeof(sockaddr_in);
		iRet = getpeername( s, (sockaddr*)&dh.remotesock, &peernamelen );//��ȡĿ��IP�Ͷ˿�..
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
	writeDataFM(s,1,buf,len);
	return Real_send(s, buf, len, flags);
}
static int WSAAPI Catch_sendto(SOCKET s,const char * buf, int len, int flags, const struct sockaddr * to, int tolen)
{
	writeDataFM(s,3,buf,len);
	return Real_sendto(s,buf, len, flags,to, tolen);
}
static int WSAAPI Catch_recv(SOCKET s, char * buf, int len, int flags)
{
	int retlen = Real_recv(s, buf, len, flags);
	writeDataFM(s,2,buf,retlen);
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
		//��һ���������ж��loadlibrary��Ҳֻ������һ��DLL_PROCESS_ATTACH������Ѿ����ؾͲ������������ˡ�
		//�ж�һ���Ƿ�Ϊexe������õ�loadlibrary���õĴ��롣�ǵĻ��Ͳ��ٴ���filemapping�ȡ�;
		Sleep(500);
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
			//MessageBox(0,TEXT("EXE�е�loadlibrary��"),chmu,0);
			return TRUE;
		}
		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(FILE_MAPPING_STRING));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		//MessageBox(0,chmu,TEXT("CreateFileMapping"),0);
		hsgFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_EXECUTE_READWRITE,0,MAPPINGFILESIZE,chmu);
		lpBaseOffset = MapViewOfFile(hsgFile,FILE_MAP_ALL_ACCESS,0,0,0);
		//
		//ע��������ʱ�����ÿ�����̵�ID��������ͬ���ź�������ֹ���Ź���;
		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(WRITE_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		writesema = CreateSemaphore(NULL,1,1,chmu);

		memset(chmu,0,M_SIZE_SEMA);
		wcscat_s(chmu,M_SIZE_SEMA,TEXT(READ_SEMAPHORE));
		wcscat_s(chmu,M_SIZE_SEMA,ids);	
		readsema =  CreateSemaphore(NULL,1,1,chmu);

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
		if (writesema)
		{
			ReleaseSemaphore(writesema,1,NULL);
			CloseHandle(writesema);
		}
		if (readsema)
		{
			ReleaseSemaphore(readsema,1,NULL);
			CloseHandle(readsema);
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

