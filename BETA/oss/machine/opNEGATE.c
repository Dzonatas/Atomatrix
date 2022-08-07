/***************************************
*
* opNEGATE.c
*
*******************/


#include "opcode.h"


void opNEGATE(void)
	{
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;

	atom	A = *(--E_SP)  ;

	if( A->precursor == aInteger )
		*(E_SP++) = oInteger( -(int)A->datum ) ;
	else	{
		*(E_SP++) = aUndefined ;
		aUndefined->allusions++ ;
		}
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
