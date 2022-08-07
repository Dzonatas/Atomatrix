/***************************************
*
* opBRA.c
*
*******************/


#include "opcode.h"


void opBRA(void)
	{
	E_PC += (*E_PC).i ;
	}
