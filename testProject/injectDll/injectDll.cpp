// injectDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "injectDll.h"


// This is an example of an exported variable
INJECTDLL_API int ninjectDll=0;

// This is an example of an exported function.
extern "C" LRESULT CALLBACK DllHookGetMsg(int code,WPARAM wParam,LPARAM lParam)
{
	if (code < 0)
	{
		return CallNextHookEx(NULL,code,wParam,lParam);
	}
	if (code == HC_ACTION)
	{
		//MessageBox(NULL, TEXT("钩子回调函数"),TEXT("回调"), MB_OK);
	}
	return CallNextHookEx(NULL,code,wParam,lParam);
}

// This is the constructor of a class that has been exported.
// see injectDll.h for the class definition
CinjectDll::CinjectDll()
{
	return;
}
