/***************************************
*
* o_load.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

atom o_load( atom A , unsigned T , atom* V )			// update: $module serialed() same ?
	{
	extern atom		aInteger ;
	extern atom		aModule ;
	extern atom		aError_INVARG ;
	extern atom		aError_EXISTS ;
	extern atom		POBJ(ObjID);
	atom			a;

	if( A != aModule )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	a	= POBJ( (ObjID)V[0]->datum );
	if( ! a )
		a = aError_EXISTS;
	a->allusions++;
	return a;
	}
