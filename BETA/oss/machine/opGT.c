/***************************************
*
* opGT.c
*
*******************/


#include "opcode.h"


void opGT( void )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aOprandGreaterThan ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	a	= A ;
	do	if( ( aa = a->interatom ) )
			do	if( aa->precursor == aOprandGreaterThan )
					{
					*(E_SP++) =  ((oprand)aa->datum)( A , B ) ;
					if( --A->allusions == 0 )
						destruct( A ) ;
					if( --B->allusions == 0 )
						destruct( B ) ;
					return ;
					}
				while( ( aa = aa->interatoms ) ) ;
		while( ( a = a->precursor ) ) ;
	( *(E_SP++) = aUndefined )->allusions++ ;			// UPDATE: aIllogical ?
	if( --A->allusions == 0 )
		destruct( A ) ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
