/***************************************
*
* opEQ.c
*
*******************/


#include "opcode.h"


void opEQ(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandCompareEqual ;
	extern atom		aEquality ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( A == B )
		{
		( *(E_SP++) = aEquality )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		*E_SP	= aUndefined ;
		aUndefined->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandCompareEqual )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A , B ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				( *(E_SP) = aUndefined )->allusions++ ;
				if( --B->allusions == 0 )
					destruct( B ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	( *(E_SP++) = aIllogical )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
