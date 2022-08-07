/***************************************
*
* o_intermodulance.c
*
*******************/


#include	"ATOMATRIX.h"
#include "object.h"


atom o_intermodulance( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleSerinteger ;
	extern atom		aInteger ;
	extern atom		aModuleXML ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oInteger)(int) ;
	extern object		MOBJ(atom) ;
	extern atom		loadModule(atom) ;
	atom			m ;
	ObjID			o ;
	ObjID			p ;

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
	if( ( m = V[0] )->precursor == aInteger )
		_asm int 3 ;
//		{
//		if( ! COBJ( p = (ObjID)m->datum ) )
//			{
//			aError_INVARG->allusions++;
//			return aError_INVARG;
//			}
//		}
	if( ( m = V[0] )->precursor == aModuleXML )
		{
		extern atomaModuleXML_atomorhize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( m ) ) )
			return r ;
		}
	else
	if( m->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( m ) ) )
			return r ;
		}
	else
	if( m->precursor != aModule )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	else
		p	= ((object)m->datum)->serinteger ;		// UPDATE: needed ? nah
	o	= ((object)A->datum)->serinteger ;
	do	{
		if( ( m = ((object)m->datum)->orbit ) == A )
			return oInteger( 1 ) ;
		} while( m && MOBJ( ((object)m->datum)->orbit ) ) ;
	return oInteger( 0 ) ;				// UPDATE: oInteger works for now
	}
