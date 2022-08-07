/***************************************
*
* v_identifier.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"


atom v_identifier( atom A , int T , atom* V )
	{
	extern atom		aObjVar ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*str_dup)(string) ;

	if( A->precursor != aObjVar )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	return aAsciiString_atomize( str_dup( ((objvar)A->datum)->name ) ) ;
	}
