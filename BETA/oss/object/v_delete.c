/***************************************
*
* v_delete.c
*
*******************/


#include	"ATOMATRIX.h"


atom v_delete( atom A , unsigned T , atom* V )
	{
	extern atom		aObjVar ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PERM ;
	extern void		(*destruct)(atom) ;

	if( A->precursor != aObjVar )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	destruct( A ) ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}
