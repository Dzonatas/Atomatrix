/***************************************
*
* opPUSH_OV
*
*******************/


#include "opcode.h"
#include "variant.h"  // needed by variable.h
#include "variable.h"


void opPUSH_OV(void)
	{
	extern atom		aAsciiString ;
	extern atom		aModule ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern void		(*destruct)(atom) ;
	extern atom		(*getVariable_A_S)(atom,string) ;
	extern atom		(*objvar_a_s)(atom,string) ;
	atom			a ;
									// UPDATE: progressive routine
	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( B->precursor != aAsciiString )				// UPDATE: unicode
		( a = aError_TYPE )->allusions++ ;
	else
	if( A->precursor && A->precursor->precursor == aModule )
		*E_SP++ = objvar_a_s( A , (string)B->datum ) ;
	else
		*E_SP++ = getVariable_A_S( A , B->datum ) ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP	= aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}


