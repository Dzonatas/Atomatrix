/***************************************
*
* o_precesses.c
*
*******************/


#include	"ATOMATRIX.h"
#include "object.h"


atom o_precesses( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oInteger)(int) ;
	extern object		MOBJ(atom) ;
	extern atom		loadModule(atom) ;
	atom			m ;
	ObjID			o ;
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
	if( ( m = V[0] )->precursor == aInteger )
		{
			_asm int 3 ;
//		if( ! COBJ( p = (ObjID)m->datum ) )
//			{
//			aError_INVARG->allusions++;
//			return aError_INVARG;
//			}
		}
	else
	if( m->precursor == aModuleXML )
		p	= MOBJ( m )->precursor ;
	else
	if( m->precursor == aModuleSerinteger )
		p	= MOBJ( m )->precursor ;
	else
	if( m->precursor && m->precursor->precursor == aModule )
		p	= ((object)m->datum)->precursor ;
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	o	= ((object)A->datum)->serinteger ;
	// UPDATE: p == o ? 0 !
	while( p )
		{
		if( p == A )
			return oInteger( 1 ) ;
		p = MOBJ( p )->precursor ;
		}
	return oInteger( 0 ) ;				// UPDATE: oInteger works for now
	}