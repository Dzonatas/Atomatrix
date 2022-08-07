/***************************************
*
* p_orbit.c
*
*******************/


#include "ATOMATRIX.h"


atom p_orbit( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	atom			a ;

	if( A->precursor != aProgram )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	( a = A->orbit )->allusions++ ;
	return a ;
	}
