/***************************************
*
* opASSIGN_ME.c
*
*******************/


#include "opcode.h"


void opASSIGN_ME(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aOprandMinusEquals ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandMinusEquals )
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



/************************************************
*
* UPDATE:
* 1 - If more than one allusion of A then atomize( aInteger ) and
*     set A to equate " A -= B ".  ( perhaps?? )
*
**************************/