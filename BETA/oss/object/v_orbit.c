/***************************************
*
* v_orbit.c
*
*******************/


#include "ATOMATRIX.h"


atom v_orbit( atom A , unsigned T , atom* V )
	{
	extern atom		aObjVar ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	atom			a ;

	if( A->precursor != aObjVar )
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
