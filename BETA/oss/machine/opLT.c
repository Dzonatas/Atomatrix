/***************************************
*
* opLT.c
*
*******************/


#include "opcode.h"


void opLT(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aOprandLessThan ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	a	= A ;
	do	if( ( aa = a->interatom ) )
			do	if( aa->precursor == aOprandLessThan )
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
