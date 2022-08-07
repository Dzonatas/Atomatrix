/***************************************
*
* o_variance.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"


atom o_variance( atom A , unsigned T , atom* V )
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
//	extern atom		getObjVar(atom,string,objvar*) ;
	extern atom		aModule_variance_a_s(atom,string) ;
	extern atom		loadModule(atom) ;
	atom			a ;
//	atom			v ;
	string			s ;

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
	if( V[0]->precursor != aAsciiString )		// UPDATE: unicode
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	s	= V[0]->datum ;
	a	= aModule_variance_a_s( A , s ) ;
	if( a->precursor == aObjVar )
		{
		// UPDATE: use reference variant
		a->allusions++ ;
		return a ;
		}
	return a ;
	}
