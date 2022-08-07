/***************************************
*
* opPUSH_VAR.c
*
*******************/


#include "opcode.h"


void opPUSH_VAR(void)
	{
	extern atom		aVarrayReferent ;
	extern atom		aUndefined ;
	atom			v ;
	atom			a ;

	v	= E_VA[E_PF->lvar + E_PC[0].i] ;
	if( a = v->datum )
		if( a->precursor == aVarrayReferent )
			{
			if( ! a->orbit )
				a = aUndefined ;
			a->allusions++ ;
			*(E_SP++)	= a ;
			E_PC++ ;
			return ;
			}
	v->allusions++ ;
	*(E_SP++)	= v ;
	E_PC++ ;
	}
