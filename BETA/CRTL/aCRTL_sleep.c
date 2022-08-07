/******************************
*
* aCRTL_random.c
*
*******************/

#include	"ATOMATRIX.h"
#include	<windows.h>


void* aCRTL_sleep__task( void* D )
	{
	Sleep( (unsigned)D * 1000 ) ;
	return 0 ;
	}


atom aCRTL_sleep( atom A , unsigned T , atom* V )
	{
	extern void*		(*taskSplice)(void*,void*) ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( (*V)->precursor != aInteger )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	taskSplice( aCRTL_sleep__task , (*V)->datum ) ;
	return atominul ;
	}