/***************************************
*
* opPUSH_OVT.c
*
*******************/


#include "opcode.h"


void opPUSH_OVT(void)
	{
	extern atom		aAsciiString ;
	extern atom		aError_TYPE ;
	extern void		(*destruct)(atom) ;
	extern atom		(*objvar_a_s)(atom,string) ;
									// UPDATE: progressive routine
	atom	A = *--E_SP ;

	if( A->precursor != aAsciiString )
		( *E_SP++ = aError_TYPE )->allusions++ ;
	else
		*E_SP++ = objvar_a_s( E_PF->scope , (string)A->datum ) ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
