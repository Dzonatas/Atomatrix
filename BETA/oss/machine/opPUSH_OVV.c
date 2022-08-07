/***************************************
*
* opPUSH_OVV.c
*
*******************/


#include "opcode.h"
#include "variant.h"  // needed by variable.h
#include "variable.h"


void opPUSH_OVV(void)
	{
	extern atom		aAsciiString ;
	extern atom		aModule ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aVarrayReferent ;
	extern atom		aIllogical ;
	extern atom		aError_TYPE ;
	extern atom		aError_INVOBJ ;
	extern void		(*destruct)(atom) ;
//	extern atom		(*POBJ)(ObjID) ;
	extern atom		(*getVariable_A_S)(atom,string) ;
	extern atom		(*objvar_a_s)(atom,string) ;
	atom			v ;
									// UPDATE: progressive routine
	atom	A = *(--E_SP) ;

	if( A->precursor != aAsciiString )
		{
		( *(E_SP++)	= aError_TYPE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		E_PC++;
		return;
		}
	v	= E_VA[E_PF->lvar + E_PC->i ]->datum ;
	if( ! v )
		{
		( *(E_SP++) = aError_INVOBJ )->allusions++ ;	// UPDATE: should not be INVOBJ
		if( --A->allusions == 0 )
			destruct( A ) ;
		E_PC++;
		return ;
		}
	if( v->precursor == aVarrayReferent )
		if( v->orbit )
			v = *(atom*)v->datum ;
		else	{
			( *(E_SP++) = aIllogical )->allusions++ ;
			if( --A->allusions == 0 )
				destruct( A ) ;
			E_PC++;
			return ;
			}
	if( v->precursor && v->precursor->precursor == aModule )
		*E_SP++ = objvar_a_s( v , A->datum ) ;
	else
//	if( v->precursor == aInteger )  // UPDATE:
//		_asm int 3 ;
//	else
		*E_SP++ = getVariable_A_S( v , A->datum ) ;
	E_PC++;
	if( --A->allusions == 0 )
		destruct( A ) ;
	}
