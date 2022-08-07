/***************************************
*
* o_serialed.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"


atom o_serialed( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aUndefined ;
	extern atom		POBJ(ObjID) ;
	atom			a ;

	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( ! ( a = POBJ( (ObjID)V[0]->datum ) ) )
		a = aUndefined ;
	a->allusions++ ;
	return a ;
	}