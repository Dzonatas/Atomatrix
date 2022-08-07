/***************************************
*
* opBNEGATE.c
*
*******************/


#include "opcode.h"


void opBNEGATE(void)
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aOprandBinaryNegation ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	A = *(--E_SP) ;

	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandBinaryNegation )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	( *(E_SP++) = aIllogical )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
