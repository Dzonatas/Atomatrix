/***************************************
*
* aCRTL_localTime.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include "ATOMATRIX.h"


#include <time.h>


atom aCRTL_localTime( atom A , unsigned T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern atom		aError_RANGE ;
	extern atom		(*oInteger)(int) ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern void		(*aVarray_append_a_a)(atom,atom) ;
	struct tm*		lt;
	time_t			clocked;
	atom			z ;

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
	clocked		= (time_t) V[0]->datum ;
	lt		= localtime( &clocked );
	if( ! lt )
		{
		aError_RANGE->allusions++;
		return aError_RANGE;
		}
	z	= aVarray_atomize_a_n( 0 , 0 ) ;			// UPDATE: aVarray_atomize_a_n( 0, 9 ) ...
	aVarray_append_a_a( z , oInteger( lt->tm_sec ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_min ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_hour ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_mday ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_mon ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_year ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_wday ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_yday ) ) ;
	aVarray_append_a_a( z , oInteger( lt->tm_isdst ) ) ;
	return z ;
	}