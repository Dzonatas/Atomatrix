/***************************************
*
* p_inherit.c
*
*******************/


#include "ATOMATRIX.h"
#include "varray.h"
#include "program.h"


atom p_inherit( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aError_INVOBJ ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern void		(*destruct)(atom) ;
	extern atom		(*inherit)(atom);
	extern atom		(*duplicate)(atom);
	varray			va;
	atom			a ;
	atom			b ;
	atom			z ;

	if( A != aProgram )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	a		= aVarray_atomize_a_n( 0 , 0 ) ;	// UPDATE:
	va		= a->datum ;
	va->nelem	= T ;
	va->ary		= V ;
	b		= duplicate( a ) ;
	z		= inherit( b ) ;
	va->nelem	= 0 ;
	va->ary		= 0 ;
	if( --b->allusions == 0 )
		destruct( b ) ;
	if( --a->allusions == 0 )
		destruct( a ) ;
	return z ;
	}
