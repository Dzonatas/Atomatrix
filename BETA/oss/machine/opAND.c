/***************************************
*
* opAND.c
*
*******************/


#include "opcode.h"


void opAND()
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aOprandCondition ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	A = *(E_SP-1) ;

	if( A == aUndefined )
		{
		E_PC += E_PC->i ;
		return ;
		}
	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandCondition )
				{
				if( ( a = ((oprand)aa->datum)( A ) ) == A )
					{
					( *(--E_SP) = aUndefined )->allusions++ ;
					if( --A->allusions == 0 )
						destruct( A ) ;
					E_PC++ ;
					return ;
					}
				if( --a->allusions == 0 )
					destruct( a ) ;
				E_PC += E_PC->i ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	// UPDATE: notify about aIllogical
	( *(E_SP-1) = aIllogical )->allusions++ ; 
	if( --A->allusions == 0 )
		destruct( A ) ;
	E_PC += E_PC->i ;
	}
