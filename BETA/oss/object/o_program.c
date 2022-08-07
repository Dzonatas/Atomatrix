/***************************************
*
* o_program.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "program.h"

// UPDATE: o_grammant
atom o_program( atom A , unsigned T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aProgram ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aError_PROGNF ;
	extern atom		POBJ(ObjID) ;
	extern atom		getProgram(atom,string,program*) ;
	extern atom		loadModule(atom) ;
	atom			p ;

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
	if( ( p = getProgram( A , V[0]->datum , 0 ) ) )
		{
		// UPDATE: use reference variant
		p->allusions++ ;
		return p ;
		}
	aError_PROGNF->allusions++ ;
	return aError_PROGNF ;
	}



