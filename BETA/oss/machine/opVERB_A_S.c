/***************************************
*
* opVERB.c
*
*******************/


#include "opcode.h"
#include "cmp_s.h"
#include "varray.h"
#include "method.h"

void opVERB_A_S(void)
	{
	extern atom		aMethod ;
	extern atom		aModule ;
	extern atom		aAsciiString ;
	extern atom		aVarray ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PROGNF ;
	extern atom		aError_CALL_FAILED ;
	extern atom		aSignal_EXEC_CONTINUE ;
	extern atom		aSignal_REPEAT_FUNCTION ;
	extern atom		aSignal_JUMP ;
	extern void		(*dealloc)(void*) ;
	extern void		(*destruct)(atom) ;
	extern atom		execute_a_s_a_(atom,string,atom);
	sframe			tf ;
	method			m ;
	atom			a ;
	atom			aa ;
	atom			r ;
	atom			o ;
	atom			A ;
	atom			B ;
	atom			C ;
	atom			D ;

	if( ( D = ( tf = E_PF )->datum ) )
		{
		D = D->interatom->datum = *--E_SP ;
		( *E_SP = aUndefined )->allusions++ ;
		}
	A = (E_PC++)->a ;
	B = (E_PC++)->a ;
	C = *--E_SP ;
	if( B->precursor != aAsciiString )			// UPDATE: unicode
		{
		( *(E_SP++) = aError_TYPE )->allusions++ ;
		if( --C->allusions == 0 )
			destruct( C ) ;
		return;
		}
	if( ( aa = o = A )->precursor && aa->precursor->precursor == aModule )
		{
		if( ! ( a = execute_a_s_a_( o , B->datum , C ) ) )
			{
			( *E_SP = aUndefined )->allusions++ ;
			if( --C->allusions == 0 )
				destruct( C ) ;
			return;
			}
		if( a == aError_PROGNF )
			--a->allusions ;
		else	{
			*(E_SP++) = a ;
			if( --C->allusions == 0 )
				destruct( C ) ;
			return;
			}
		}
	do	{
		a = aa->interatom ;
		while( a )
			{
			if( a->precursor == aMethod )
				{
				m = a->datum;
				if( cmp_s( m->name , (string)B->datum ) == 0 )
					{
					if( C->precursor == aVarray )
						r = (m->method)( o , ((varray)C->datum)->nelem , ((varray)C->datum)->ary ) ;	// UPDATE: r = (m->method)( o , C ) ; // pass any array type, not only aVarray
					else
						r = (m->method)( o , 0 , 0 ) ;
					if( D && --D->allusions == 0 )
						destruct( D ) ;
					if( r )	{
						*(E_SP++)	= r ;
						if( --C->allusions == 0 )
							destruct( C ) ;
						return;
						}
					if( ! ( r = tf->datum) )
						{
						( *(E_SP++) = aUndefined )->allusions++ ;
						if( --C->allusions == 0 )
							destruct( C ) ;
						return ;
						}
					if( r == aSignal_EXEC_CONTINUE )
						{
						( *(E_SP++) = aUndefined )->allusions++ ;
						if( --C->allusions == 0 )
							destruct( C ) ;
						r->allusions-- ;
						tf->datum = 0 ;
						return ;
						}
					if( r == aSignal_REPEAT_FUNCTION )
						{
						E_SP++ ;
						E_PC -= 3 ;
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
					_asm int 3 ;
					// UPDATE: aIllogical
					}
				}
			a = a->interatoms;
			}
		} while( ( aa = aa->precursor ) ) ;
	( *(E_SP++) = aError_PROGNF )->allusions++ ;
	if( --C->allusions == 0 )
		destruct( C ) ;
	}
