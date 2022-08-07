/***************************************
*
* opMUL.c
*
*******************/


#include "opcode.h"


void opMUL(void)
	{
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( A == aUndefined || B == aUndefined )
		{
		*(E_SP++)	 = aUndefined ;
		aUndefined->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		*E_SP	 = aUndefined ;
		aUndefined->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	if( B->precursor != aInteger )
		{
		*(E_SP++)	= aError_TYPE ;
		aError_TYPE->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		*E_SP	 = aUndefined ;
		aUndefined->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	if( A->precursor != aInteger )
		{
		*(E_SP++)	= aError_TYPE ;
		aError_TYPE->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		*E_SP	 = aUndefined ;
		aUndefined->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	*(E_SP++) = oInteger( (int) A->datum * (int) B->datum ) ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	*E_SP	 = aUndefined ;
	aUndefined->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
