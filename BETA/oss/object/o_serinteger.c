/******************************
*
* o_serinteger.c
*
*********************/


#include "ATOMATRIX.h"
#include "object.h"


atom o_serinteger( atom A , int T , atom* V )
	{
	extern atom		aModuleSerialed ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oInteger)(int) ;

	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( A->precursor == aModuleSerinteger )
		return oInteger( (unsigned)A->datum ) ;
	if( A->precursor != aModuleSerialed )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	return oInteger( ((object)A->datum)->serinteger ) ;
	}
