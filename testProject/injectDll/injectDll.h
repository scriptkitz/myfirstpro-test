// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the INJECTDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// INJECTDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef INJECTDLL_EXPORTS
#define INJECTDLL_API __declspec(dllexport)
#else
#define INJECTDLL_API __declspec(dllimport)
#endif

// This class is exported from the injectDll.dll
extern "C" LRESULT CALLBACK DllHookGetMsg(int code,WPARAM wParam,LPARAM lParam);

class INJECTDLL_API CinjectDll {
public:
	CinjectDll(void);
	// TODO: add your methods here.
};

extern INJECTDLL_API int ninjectDll;


