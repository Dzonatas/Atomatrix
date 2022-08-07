/***************************************
*
* p_delete.c
*
*******************/


#include	"ATOMATRIX.h"


atom p_delete( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern void		(*destruct)(atom) ;

	if( A->precursor != aProgram  )
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
