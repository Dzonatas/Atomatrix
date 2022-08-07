/***************************************
*
* opFUNC.c
*
*******************/


#include "opcode.h"
#include "varray.h"


void opFUNC(void)
	{
	extern atom		aUndefined ;
	extern atom		aVarray ;
	extern atom		aSignal_JUMP ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		aSignal_REPEAT_FUNCTION ;
	extern void		(*destruct)(atom) ;
	sframe			tf;
	atom			A ;
	atom			B ;
	atom			r ;

	if( ( B = ( tf = E_PF )->datum ) )
		{
		B = B->interatom->datum = *--E_SP ;
		( *E_SP = aUndefined )->allusions++ ;
		}
	A	= *(--E_SP) ;
	if( A->precursor == aVarray )
		r = (E_PC->func)(((varray)A->datum)->nelem, ((varray)A->datum)->ary ) ; 
	else
		r = (E_PC->func)(0,0) ;
	if( B && --B->allusions == 0 )
		destruct( B ) ;
	if( r )	{
		*(E_SP++)	= r ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		E_PC++ ;
		return;
		}
	if( ! ( r = tf->datum) )
		{
		( *(E_SP++) = aUndefined )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		E_PC++ ;
		return ;
		}
	if( r == aSignal_EXEC_CONTINUE )
		{
		( *(E_SP++) = aUndefined )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		r->allusions-- ;
		tf->pc++ ;
		tf->datum = 0 ;
		return ;
		}
	if( r == aSignal_REPEAT_FUNCTION )
		{
		E_SP++ ;
		E_PC-- ;
		r->allusions-- ;
		tf->datum = 0 ;
		return ;
		}
	if( r->precursor == aSignal_JUMP )
		{
		E_SP++ ;
		E_PF->stackbot++ ;
		tf->pc-- ;
		return;
		}
	}
