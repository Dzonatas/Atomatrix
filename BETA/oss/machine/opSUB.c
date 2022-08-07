/***************************************
*
* opSUB.c
*
*******************/


#include "opcode.h"


void opSUB(void)
	{
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( B == aUndefined )
		{
		( *(E_SP++) = aUndefined )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		return ;
		}
	else
	if( A == aUndefined )
		{
		( *(++E_SP) = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	else
	if( B->precursor != aInteger )
		{
		*(E_SP++)	= aError_TYPE ;
		aError_TYPE->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	else
	if( A->precursor != aInteger )
		{
		*(E_SP++)	= aError_TYPE ;
		aError_TYPE->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	*(E_SP++)	= oInteger( (int)A->datum - (int)B->datum ) ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
