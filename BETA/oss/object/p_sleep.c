/***************************************
*
* p_sleep.c
*
*******************/


#include	"ATOMATRIX.h"

#ifdef MICROSOFT_win32
#include <windows.h>
//extern void* sleep(unsigned int);
#else
#include <unistd.h>
#endif

atom p_sleep( atom A , unsigned T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[0]->datum < 0 )
		{
		aError_RANGE->allusions++ ;
		return aError_RANGE ;
		}
#	ifdef MICROSOFT_win32
	 Sleep( (int) V[0]->datum * 1000 ) ;
#	else
	 sleep( (int) V[0]->datum ) ;
#	endif

	return atominul;
	}
