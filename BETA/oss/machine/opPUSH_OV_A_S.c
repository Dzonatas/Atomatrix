/***************************************
*
* opPUSH_OV_A_S.c
*
*******************/


#include "opcode.h"
#include "variant.h"  // needed by variable.h
#include "variable.h"


void opPUSH_OV_A_S()
	{
	extern atom		aInteger ;
	extern atom		aModule ;
	extern atom		aUndefined ;
	extern atom		aVarrayReferent ;
	extern atom		aIllogical ;
	extern atom		aError_INVOBJ ;
	extern atom		(*getVariable_A_S)(atom,string) ;
	extern atom		(*objvar_a_s)(atom,string) ;
	atom			a ;

	if( ! ( a = E_PC->a ) )
		{
		( *(E_SP++) = aError_INVOBJ )->allusions++ ;
		E_PC	+= 2 ;
		return ;
		}
	if( a->precursor == aVarrayReferent )
		if( a->orbit )
			a = *(atom*)a->datum ;
		else	{
			( *(E_SP++) = aIllogical )->allusions++ ;
			E_PC	+= 2 ;
			return ;
			}
	E_PC++ ;
	if( a->precursor && a->precursor->precursor == aModule )
		*E_SP++ = objvar_a_s( a , E_PC->a->datum ) ;
	else
	if( a->precursor == aInteger )
		_asm int 3 ;
	else
		*E_SP++ = getVariable_A_S( a , E_PC->a->datum ) ;
	E_PC++;
	}