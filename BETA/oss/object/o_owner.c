// o_owner.c			Copyrighted 2004 Ballard

#include	"ATOMATRIX.h"
#include "object.h"


atom o_principal( atom A , unsigned T , atom* V )
	{
	extern atom		aUndefined ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_INVARG ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PERM ;
	extern atom		POBJ(ObjID) ;
	extern int		p_principal_o(object) ;
	extern atom		loadModule(atom) ;
	object			o ;
	atom			a ;

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
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( ! T )
		{
		if( ( o = (object)A->datum )->principal == A )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! o->principal )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		o->principal->allusions++ ;
		return o->principal ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( ( a = V[0])->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( a ) ) )
			return r ;
		}
	else
	if( a->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( a ) ) )
			return r ;
		}
	else
	if( ( ! a->precursor ) || a->precursor->precursor != aModule )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( p_principal_o( o = ((object)A->datum) ) )
		{
		o->principal = a ;
		a->allusions++ ;
		return a ;
		}
	aError_PERM->allusions++;
	return aError_PERM;
	}
