/***************************************
*
* v_principal.c
*
*******************/


#include	"ATOMATRIX.h"
#include "object.h"
#include "objvar.h"


atom v_principal( atom A , int T , atom* V )
	{
	extern atom		aUndefined ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aObjVar ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PERM ;
	extern int		p_principal(objvar) ;
	extern atom		loadModule(atom) ;
	objvar			o ;
	atom			a ;

	if( A->precursor != aObjVar )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( ! T )
		{
		if( ( o = (objvar)A->datum)->principal == ( a = A->orbit ) )
			{
			a->allusions++ ;
			return a ;
			}
		// UPDATE: validate
		if( ! o->principal )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		( a = o->principal )->allusions++ ;
		return a ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( ( ! ( a = V[0] )->precursor ) || a->precursor->precursor != aModule )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( a->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( a ) ) )
			return r ;
		}
	if( a->precursor == aModuleSerinteger )		// UPDATE: aModuleSerinteger
		{
		atom	r ;
		if( ( r = loadModule( a ) ) )
			return r ;
		}
	if( p_principal( o = ((objvar)A->datum) ) )
		{
		( o->principal = a )->allusions++ ;
		return a ;
		}
	aError_PERM->allusions++;
	return aError_PERM;
	}