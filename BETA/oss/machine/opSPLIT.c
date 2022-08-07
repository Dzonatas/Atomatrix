/***************************************
*
* opSPLIT.c
*
*******************/


#include "opcode.h"
#include "varray.h"
#include "objvar.h"
#include "sobjvar.h"


void opSPLIT(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aOprandSplit ;
	extern void		(*destruct)(atom) ;
	atom	aa ;
	atom	a ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	a	= A ;
	do	if( ( aa = a->interatom ) )
			do	if( aa->precursor == aOprandSplit )
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
