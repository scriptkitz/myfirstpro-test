// injectDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "injectDll.h"


// This is an example of an exported variable
INJECTDLL_API int ninjectDll=0;

// This is an example of an exported function.
INJECTDLL_API int fninjectDll(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see injectDll.h for the class definition
CinjectDll::CinjectDll()
{
	return;
}
