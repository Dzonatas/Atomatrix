/***************************************
*
* o_precessant.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

atom o_precessant( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_INVARG ;
	extern atom		POBJ(ObjID) ;
	extern atom		loadModule(atom) ;
	atom			m ;

	if( A->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
		}
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
	if( ( m = ((object)A->datum)->precursor ) )
		{
		m->allusions++ ;
		return m ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}