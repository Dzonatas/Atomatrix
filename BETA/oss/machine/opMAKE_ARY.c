/***************************************
*
* opMAKE_ARY.c
*
*******************/


#include "opcode.h"


void opMAKE_ARY(void)
	{
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	*(E_SP++)	= aVarray_atomize_a_n( 0 , 0 ) ;
	}
