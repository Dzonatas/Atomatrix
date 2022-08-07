/***************************************
*
* opSEARCH.c
*
*******************/


#include "opcode.h"
#include "varray.h"


void opSEARCH(void)
	{
	extern atom		aModule ;
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
//	extern atom		aError_TYPE ;
//	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
	extern atom		oIndexInteger( int ) ;
	unsigned int		n ;
	atom*			v ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( B->precursor != aVarray )
		{
		( *(E_SP++)	= aUndefined )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		*E_SP	 = aUndefined ;
		aUndefined->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		return;
		}
	v = ((varray)B->datum)->ary ;
	n = ((varray)B->datum)->nelem ; 
	if( A->precursor && A->precursor->precursor == aModule )
		{
		while( n )
			{
			if( *v == A )
				break ;
			v++ , n-- ;
			}
		}
	else
	if( A->precursor == aInteger )
		{
		while( n )
			{
			if( *v == A )
				break ;
			if( (*v)->precursor == aInteger )
				{
				if( (*v)->datum == A->datum )
					break ;
				}
			v++ , n-- ;
			}
		}
	else
	if( A->precursor == aAsciiString )			// UPDATE: unicode
		{
		while( n )
			{
			if( *v == A )
				break ;
			if( (*v)->precursor == aAsciiString )
				{
				string	a	= A->datum ;
				string	b	= (*v)->datum ;
				if( cmp_s(a,b) == 0 )
					break;
				}
			v++ , n-- ;
			}
		}
	else
		n = 0 ;
	if( n )
		*(E_SP++) = oIndexInteger( ((varray)B->datum)->nelem - n ) ;
	else
		( *(E_SP++) = aUndefined )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
