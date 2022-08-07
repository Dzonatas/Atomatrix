/***************************************
*
* opOBJ_FUNC.c
*
*******************/


#include "opcode.h"
#include "method.h"
#include "varray.h"

void opOBJ_FUNC(void)
	{
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aSignal_JUMP ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		aSignal_REPEAT_FUNCTION ;
	extern void		(*destruct)(atom) ;
	extern void		(*dealloc)(void*) ;
	sframe			tf;
	atom			B ;
	atom			A ;
	method			f;
	atom			a;
	atom			r ;

	if( ( B = ( tf = E_PF )->datum ) )
		{
		B = B->interatom->datum = *--E_SP ;
		( *E_SP = aUndefined )->allusions++ ;
		}
	A	= *--E_SP ;
	f = E_PC->a->datum ;
	a  = E_PC->a->orbit ;
	if( A->precursor == aVarray )
		r = (f->method)( a , ((varray)A->datum)->nelem , ((varray)A->datum)->ary ) ;
	else
		r = (f->method)( a , 0 , 0 ) ;
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
		tf->pc++ ;
		tf->datum = 0 ;
		return ;
		}
	if( r == aSignal_REPEAT_FUNCTION )
		{
		E_SP++ ;
		E_PC-- ;
		tf->datum = 0 ;
		return ;
		}
	if( r->precursor == aSignal_JUMP )
		{
		E_SP++ ;
		E_PF->stackbot++ ;
		tf->pc-- ;
//			tf->jmp	= (int)r->datum ;
//			destruct( r ) ;
		return;
		}
	}
