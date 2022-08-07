#include "ATOMATRIX.h"
#include <windows.h>


HINSTANCE	dllInstance ;


BOOL WINAPI DllMain( HINSTANCE I , DWORD R , LPVOID V )
	{
	if( R == DLL_PROCESS_ATTACH )
		dllInstance = I ;
	return 1 ;
	}
