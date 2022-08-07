/***************************************
*
* opEVAL.c
*
*******************/


#include <stdlib.h>


#include "opcode.h"


void opEVAL(void)
	{
	typedef	atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aOprandEvaluate ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	A = *(E_SP-1) ;

	if( ( a = A ) == aUndefined )
		return ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandEvaluate )
				{
				*(E_SP-1) =  ((oprand)aa->datum)( A ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	}
