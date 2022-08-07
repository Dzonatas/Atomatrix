/***************************************
*
* Active_Follow.c
*
*******************/


#include "ATOMATRIX.h"


void Active_Follow( atom A , atom* V )
	{
	extern void		(*aVarray_append_v_a)(atom*,atom) ;

	while( A )
		{
		A->allusions++ ;
		aVarray_append_v_a( V , A ) ;
		if( A->intratom )
			Active_Follow( A->intratom , V ) ;
		A	= A->intratoms ;
		}
	}


/***************************************
*
* UPDATE:
* Function Active_Follow() could be not recursive.
*
***********************/