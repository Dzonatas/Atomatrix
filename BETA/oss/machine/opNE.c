/***************************************
*
* opNE.c
*
*******************/


#include "opcode.h"


void opNE(void)
	{
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	int			i;
									// UPDATE: progressive routine
	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( A->precursor == aInteger
		&& B->precursor == aInteger )
		i	= ( (int)A->datum != (int) B->datum) ;
	else
	if( A->precursor == aAsciiString
		&& B->precursor == aAsciiString )
		{
		string	a	= A->datum ;
		string	b	= B->datum ;
		i	= ( cmp_s(a,b) != 0 ) ;
		}
	else	{
		// UPDATE: undef != undef should return undef
		i	= ( A != B ) ;
		}
	*(E_SP++)	= oInteger( i ) ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	*E_SP	= aUndefined ;
	aUndefined->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
