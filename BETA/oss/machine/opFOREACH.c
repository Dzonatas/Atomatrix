/***************************************
*
* opFOREACH.c
*
*******************/


#include "opcode.h"


void opFOREACH(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aOprandIterize ;
	extern atom		aIterum ;
	extern void		(*destruct)(atom) ;
	atom			A ;
	atom			v0 ;
	atom			a ;
	atom			aa ;

	if( ( a = (v0=E_VA[E_PF->lvar + (E_PC+1)->i])->datum ) )
		{
		if( --a->allusions == 0 )
			destruct( a ) ;
		v0->datum = 0 ;
		}
	A = a	= *--E_SP ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandIterize )
				{
				if( ( *(E_SP++) = ((oprand)aa->datum)( A , v0 ) )->precursor == aIterum )
					{
					if( --A->allusions == 0 )
						destruct( A ) ;
					E_PC	+= 3 ;
					return ;
					}
				E_SP-- ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				E_PC++ ;
				E_PC += (E_PC+1)->i ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	E_PC++ ;
	E_PC += (E_PC+1)->i ;
	return ;
	}
