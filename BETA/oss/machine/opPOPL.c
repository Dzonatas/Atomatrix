/***************************************
*
* opPOPL.c
*
*******************/


#include "opcode.h"
#include "objvar.h"
#include "sobjvar.h"


void opPOPL(void)
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aError_INVARG ;
	extern atom		aOprandPopLeft ;
	extern void		(*destruct)(atom) ;
	atom	a ;
	atom	aa ;

	atom	A = *(--E_SP) ;

	a	= A ;
	do	if( ( aa = a->interatom ) )
			do	if( aa->precursor == aOprandPopLeft )
					{
					*(E_SP++) =  ((oprand)aa->datum)( A ) ;
					if( --A->allusions == 0 )
						destruct( A ) ;
					return ;
					}
				while( ( aa = aa->interatoms ) ) ;
		while( ( a = a->precursor ) ) ;
	( *(E_SP++) = aError_INVARG )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
