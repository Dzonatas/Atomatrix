/***************************************
*
* opPUSH_ON_ARY.c
*
*******************/


#include "opcode.h"


void opPUSH_ON_ARY(void)
	{
	extern void		(*aVarray_append_a_a)(atom,atom) ;

	atom	A = *--E_SP ;

	aVarray_append_a_a( *(E_SP-1) , A ) ;
	}