// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
/* //20
#include <stdlib.h>
#include <string.h>
#include "define.h"

#define SIZE_BUF 28
*/
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
		//����Ҫ�����������ж϶�μ��أ���Ϊ���ܼ��ض��ٴ�����ֻ�����һ�Σ���һ�μ���ʱ�򡣡��Ǻǣ���Űɡ�
		//�����ε���loadlibrayҲֻ������һ�Σ�������ֱ��������дhookapi����ɡ�
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
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

