/***************************************
*
* opNOT.c
*
*******************/


#include "opcode.h"
#include "varray.h"


void opNOT(void)
	{
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
//	extern atom		aVarrayReferent ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
									// UPDATE: progressive routine
	atom	A = *(--E_SP) ;

	if( A == aUndefined )
		{
		aUndefined->allusions-- ;
		*(E_SP++)	= oInteger( 1 ) ;  // NOTE #1
		return;
		}
	else
	if( A->precursor == aInteger )
		{
		*(E_SP++)	= oInteger( A->datum == 0 ) ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		return;
		}
	else
	if( A->precursor == aAsciiString )
		{
		if( *((string)A->datum) == 0 )
			{
			*(E_SP++)	= oInteger( 1 ) ;
			if( --A->allusions == 0 )
				destruct( A ) ;
			return;
			}
		*(E_SP++)	= oInteger( 0 ) ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		return;
		}
	else
	if( A->precursor == aVarray )
		{
		if( ((varray)A->datum)->nelem == 0 )
			{
			*(E_SP++)	= oInteger( 1 ) ;
			if( --A->allusions == 0 )
				destruct( A ) ;
			return; 
			}
		*(E_SP++)	= oInteger( 0 ) ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		return;
		}
	else	{
		atom	a ;
		a	= A ;
		while( a && a != aIllogical )
			a = a->precursor ;
		if( a )
			*(E_SP++)	= oInteger( 1 ) ;
		else
			*(E_SP++)	= oInteger( 0 ) ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		}
	}


/*****************************************************
*
* UPDATE:
* 1 - The expression " ~~ undefined " should return aUndefined
*     yet this does return 1 for compatibility.
*
*************************/