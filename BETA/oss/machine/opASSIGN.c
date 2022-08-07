/***************************************
*
* opASSIGN.c
*
*******************/


#include "opcode.h"


void opASSIGN(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandEquals ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( ( a = A ) == aUndefined )
		{
		( *(++E_SP) = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandEquals )
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
_asm int 3 ;
	( *(E_SP++) = aIllogical )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
