/***************************************
*
* o_variant.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"


atom o_variant( atom A , unsigned T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aObjVar ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_VARNF ;
	extern atom		POBJ(ObjID) ;
	extern atom		getObjVar(atom,string,objvar*) ;
	extern atom		loadModule(atom) ;
	atom			v ;

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
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor != aAsciiString )			// UPDATE: unicode
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( ( v = getObjVar( A , V[0]->datum , 0 ) ) )
		{
		// UPDATE: use reference variant
		v->allusions++ ;
		return v ;
		}
	aError_VARNF->allusions++ ;
	return aError_VARNF ;
	}
