/***************************************
*
* opLVERB.c
*
*******************/


#include "opcode.h"
#include "object.h"


void opLVERB(void)
	{
	extern atom		aAsciiString ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aError_PROGNF ;
	extern atom		aError_INVOBJ ;
	extern object		(*COBJ)(ObjID) ;
	extern void		(*destruct)(atom) ;
	extern atom		Find_Program_By_Location(atom,string);
	extern atom		execute_a_s_a_(atom,string,atom);
	atom			a ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( A->precursor != aAsciiString )
		{
		( *(E_SP++) = aError_TYPE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
#ifdef _DEBUG
		{
		extern atom aModule ;
		if( ( ! E_PF->scope->precursor ) || E_PF->scope->precursor->precursor != aModule ) _asm int 3 ;
//		if( E_PF->scope->precursor != aModule ) _asm int 3 ;
		}
#endif
	if( ! ( a = ((object)E_PF->scope->datum)->orbit ) )
		{
		( *(E_SP++) = aError_PROGNF )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	if( ! ( Find_Program_By_Location( a , A->datum ) ) )
		{
		( *(E_SP++) = aError_PROGNF )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	if( ( a = execute_a_s_a_( a , A->datum , B ) ) )
		{
		*E_SP++ = a ;				// UPDATE: verify sp
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	( *(E_SP++) = aUndefined )->allusions++ ;  	// UPDATE: verify sp
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}


/***************************************
*
* UPDATE:
* procedure opLVERB implements an idea that functionally works yet requires
* extra features to fully implement.  This procedure implements locational function
* progression if of comparison to precursoral function progression.
*
*******************/
