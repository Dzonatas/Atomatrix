/***************************************
*
* opPUSH_OVVS.c
*
*******************/


#include "opcode.h"
#include "variant.h"  // needed by variable.h
#include "variable.h"


void opPUSH_OVVS()
	{
	extern atom		aInteger ;
	extern atom		aModule ;
	extern atom		aUndefined ;
	extern atom		aVarrayReferent ;
	extern atom		aIllogical ;
	extern atom		aError_INVOBJ ;
	extern atom		(*getVariable_A_S)(atom,string) ;
	extern atom		(*objvar_a_s)(atom,string) ;
	atom			v ;

	v	= E_VA[E_PF->lvar + E_PC->i]->datum ;
	if( ! v )
		{
		( *(E_SP++) = aError_INVOBJ )->allusions++ ;
		E_PC	+= 2 ;
		return ;
		}
	if( v->precursor == aVarrayReferent )
		if( v->orbit )
			v = *(atom*)v->datum ;
		else	{
			( *(E_SP++) = aIllogical )->allusions++ ;
			E_PC	+= 2 ;
			return ;
			}
	E_PC++ ;

	if( v->precursor && v->precursor->precursor == aModule )
		*E_SP++ = objvar_a_s( v , E_PC->a->datum ) ;
	else
	if( v->precursor == aInteger )
		_asm int 3 ;
	else
		*E_SP++ = getVariable_A_S( v , E_PC->a->datum ) ;
	E_PC++;
	}