/***************************************
*
* o_addProgram.c
*
*******************/


#include	"ATOMATRIX.h"


atom o_addProgram( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_EXISTS ;
	extern atom		aError_INVOBJ ;
	extern atom		addProgram(atom,string,string) ;
	extern atom		getProgram(atom,string,string*) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	atom			a ;
	string			s ;

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
	if( V[0]->precursor != aAsciiString )		// UPDATE: unicode
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	s	= (*V)->datum ;
	while( *s && 32 < *s && *s < 127 ) s++ ;
	if( *s )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( getProgram( A , s = (*V)->datum , 0 ) )
		{
		aError_EXISTS->allusions++ ;
		return aError_EXISTS ;
		}
	a	= addProgram( A , s , 0 ) ;		// UPDATE: verify security
	a->allusions++ ;
	return a ;
	}
