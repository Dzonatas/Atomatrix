/***************************************
*
* o_addVariant.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"
#include "perms.h"

#include "ctype.h"


atom o_addVariant( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_EXISTS ;
	extern atom		getObjVar(atom,string,objvar*) ;
	extern atom		addObjVar(atom,string,objvar*) ;
	extern atom		(*sframe_scope)(void) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	extern atom		aModule_variance_a_s(atom,string) ;
//	char			s[100];
	string			p ;
	int			i ;
	atom			v ;
	atom			a ;
	objvar			ov ;

	if( A->precursor == aModuleXML )
		{
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
	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( (v=V[0])->precursor != aAsciiString )		// UPDATE: unicode
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	i	= 0 ;
	p	= v->datum ;
	while( *p && 32 < *p && *p < 127 ) p++ ;
	if( *p )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	p	= v->datum ;
	if( getObjVar( A , p , 0 ) )
		{
		aError_EXISTS->allusions++;
		return aError_EXISTS;
		}
	a		= addObjVar( A , p , 0 ) ;		// UPDATE: verify security
	ov		= a->datum ;
	ov->perms	= (P_OR|P_TR|P_OW|P_TW);
	( ov->principal	= sframe_scope() )->allusions++ ;
	ov->flags	= 0 ;
	a->allusions++ ;
	return a ;
	}