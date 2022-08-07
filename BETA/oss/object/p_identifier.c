/***************************************
*
* p_identifier.c
*
*******************/


#include "ATOMATRIX.h"
#include "program.h"


atom p_identifier( atom A , unsigned T , atom* V )
	{
	extern atom		aProgram ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*str_dup)(string) ;

	if( A->precursor != aProgram )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	return aAsciiString_atomize( str_dup( ((program)A->datum)->name ) ) ;
	}
