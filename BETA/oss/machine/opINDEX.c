/***************************************
*
* opINDEX.c
*
*******************/


#include "opcode.h"


void opINDEX(void)
	{
	typedef atom	(*oprand)(atom,atom) ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aOprandIndex ;
	extern atom		aError_TYPE ;
	extern atom		aError_RANGE ;
	extern void		(*destruct)(atom) ;
	int			i ;
	atom			a ;
	atom			aa ;

	atom	B	= *(--E_SP) ;
	atom	A	= *(--E_SP) ;

	if( B->precursor != aInteger )				// UPDATE: no aInteger check
		{
		( *(E_SP++) = aError_TYPE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP	= aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	if( ( i = (int)B->datum ) < 0 )
		{
		( *(E_SP++) = aError_RANGE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandIndex )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A , B ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				( *E_SP = aUndefined )->allusions++ ;
				if( --B->allusions == 0 )
					destruct( B ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		a	= a->precursor ;
		} while( a ) ;
	( *(E_SP++) = aIllogical )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
