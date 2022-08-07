/***************************************
*
* opARY_ITER.c
*
*******************/


#include "opcode.h"


void opARY_ITER(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aIterum ;
	extern void		(*destruct)(atom) ;

	atom	A	= *--E_SP ;
	atom	v0	= E_VA[E_PF->lvar + E_PC->i] ;

	if( ( *(E_SP++) = ((oprand)A->interatom->datum)( A , v0 ) )->precursor == aIterum )
		{
		if( --A->allusions == 0 )
			destruct( A ) ;
		E_PC	+= 2 ;
		return ;
		}
	E_SP-- ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	E_PC += (E_PC+1)->i ;
	}
