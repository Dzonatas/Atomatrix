/******************************
*
* aCRTL_random.c
*
*******************/


#include <stdlib.h>

#include	"ATOMATRIX.h"


atom aCRTL_random( atom A , unsigned T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_RANGE ;
	extern atom		aError_INVARG ;
	extern atom		(*oInteger)(int) ;

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
	if( 1 > (int) V[0]->datum )
		{
		aError_RANGE->allusions++ ;
		return aError_RANGE ;
		}
	return oInteger( rand() % (unsigned) V[0]->datum );
	}
