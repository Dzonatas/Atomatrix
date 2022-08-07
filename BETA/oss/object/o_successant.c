/***************************************
*
* o_successant.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"


atom o_successant( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		POBJ(ObjID) ;
	extern atom		loadModule(atom) ;
	extern void		(*destruct)(atom) ;

	if( A->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
		}
	else
	if( A->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( A ) ) )
			return r ;
		}
	else
	if( ( ! A->precursor ) || A->precursor->precursor != aModule )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	return	((object)A->datum)->intraobject ;
	}
