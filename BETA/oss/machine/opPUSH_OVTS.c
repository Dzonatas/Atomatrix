/***************************************
*
* opPUSH_OVTS.c
*
*******************/


#include "opcode.h"


void opPUSH_OVTS(void)
	{
	extern atom		(*objvar_a_s)(atom,string) ;

	*E_SP++ = objvar_a_s( E_PF->scope , (*E_PC++).a->datum ) ;
	}