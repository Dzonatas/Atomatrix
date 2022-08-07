/***************************************
*
* opTVERB.c
*
*******************/


#include "opcode.h"
#include "varray.h"
#include "method.h"


void opTVERB(void)
	{
	extern atom		aAsciiString ;
	extern atom		aMethod ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aError_PROGNF ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		aSignal_REPEAT_FUNCTION ;
	extern atom		aSignal_JUMP ;
	extern void		(*destruct)(atom) ;
	extern atom		execute_a_s_a_(atom,string,atom);
	extern atom		(*POBJ)(ObjID) ;
	sframe			tf ;
	atom			a ;
	atom			aa ;
	atom			r ;
//	atom			o ;
	method			m ;
	atom			C ;
	atom			B ;
	atom			A ;

	if( ( C = ( tf = E_PF )->datum ) )
		{
		C = C->interatom->datum = *--E_SP ;
		( *E_SP = aUndefined )->allusions++ ;
		}
	B	= *--E_SP ;
	A	= *--E_SP ;
	if( A->precursor != aAsciiString )			// UPDATE: unicode
		{
		( *(E_SP++) = aError_TYPE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
//	aa = o	= POBJ( E_PF->this ) ;
	if( ! ( a = execute_a_s_a_( E_PF->scope , A->datum , B ) ) )
		{
		( *E_SP = aUndefined )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *(E_SP+1) = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	if( a == aError_PROGNF )
		--a->allusions ;
	else	{
		*(E_SP++) = a ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *(E_SP) = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	aa = E_PF->scope ;
	do	{
		a = aa->interatom;
		while( a )
			{
			if( a->precursor == aMethod )
				{
				m = a->datum;
				if( cmp_s( m->name , (string)A->datum ) == 0 )
					{
					if( B->precursor == aVarray )
						r = (m->method)( E_PF->scope , ((varray)B->datum)->nelem , ((varray)B->datum)->ary ) ;
					else
						r = (m->method)( E_PF->scope , 0 , 0 ) ;
					if( C && --C->allusions == 0 )
						destruct( C ) ;
					if( r )	{
						*(E_SP++)	= r ;
						if( --A->allusions == 0 )
							destruct( A ) ;
						if( --B->allusions == 0 )
							destruct( B ) ;
//						E_PC++ ;
						return;
						}
					if( ! ( r = tf->datum) )
						{
						( *(E_SP++) = aUndefined )->allusions++ ;
						if( --A->allusions == 0 )
							destruct( A ) ;
						if( --B->allusions == 0 )
							destruct( B ) ;
//						E_PC++ ;
						return ;
						}
					if( r == aSignal_EXEC_CONTINUE )
						{
						( *(E_SP++) = aUndefined )->allusions++ ;
						if( --A->allusions == 0 )
							destruct( A ) ;
						if( --B->allusions == 0 )
							destruct( B ) ;
						r->allusions-- ;
//						tf->pc++ ;
						tf->datum = 0 ;
						return ;
						}
					if( r == aSignal_REPEAT_FUNCTION )
						{
						E_SP += 2 ;
						E_PC-- ;
						r->allusions-- ;
						tf->datum = 0 ;
						return ;
						}
					if( r->precursor == aSignal_JUMP )
						{
						E_SP += 2 ;
						E_PF->stackbot += 2 ;
						tf->pc-- ;
						return;
						}
					_asm int 3 ;
					return ;
					}
				}
			a = a->interatoms;
			}
		} while( ( aa = aa->precursor ) ) ;
	( *(E_SP++) = aError_PROGNF )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
