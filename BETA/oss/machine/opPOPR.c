/***************************************
*
* opPOPR.c
*
*******************/


#include "opcode.h"
#include "objvar.h"
#include "sobjvar.h"


void opPOPR(void)
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aError_INVARG ;
	extern atom		aOprandPopRight ;
	extern void		(*destruct)(atom) ;
	atom	a ;
	atom	aa ;

	atom	A = *(--E_SP) ;

	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopRight )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		a	= a->precursor ;
		} while( a ) ;
	( *(E_SP++)	= aError_INVARG )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
