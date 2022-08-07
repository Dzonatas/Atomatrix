/***************************************
*
* opBRT.c
*
*******************/


#include "opcode.h"


void opBRT(void)
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aOprandCondition ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	A = *(--E_SP) ;

	if( ( a = A ) == aUndefined )
		{
		E_PC++ ;
		return ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandCondition )
				{
				if( ( a = ((oprand)aa->datum)( A ) ) == A )
					{
					E_PC += E_PC->i ;
					if( --A->allusions == 0 )
						destruct( A ) ;
					return ;
					}
				E_PC++ ;
				if( --a->allusions == 0 )
					destruct( a ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	// UPDATE: stop and report if aIllogical
	( *E_SP = aUndefined )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	E_PC	+= E_PC->i ;
	}