/*******************************************
*
* security.c
*
***********************/


#include "ATOMATRIX.h"
#include "perms.h"
#include "object.h"
#include "objvar.h"
#include "sobjvar.h"

int precesses( atom P , atom S )
	{
	extern atom		aModuleSerinteger ;
	extern atom		aModuleXML ;
	extern atom		loadModule( atom ) ;
	extern atom		oModuleLoad(unsigned,atom*) ;
	extern object		COBJ(ObjID) ;
	extern void		(*destruct)(atom) ;
	atom			p = S ;

	if( ! ( P && S ) )
		return 0 ;

	do	{
		if( p->precursor == aModuleXML )
			{
			extern atom aModuleXML_atomorphize(atom) ;
			atom	r ;
			if( ( r = aModuleXML_atomorphize( p ) ) )
				{
				destruct( r ) ;
				return 0 ;		// UPDATE: handle error, or redo return type
				}
			}
		if( p->precursor == aModuleSerinteger )
			{
			atom	r ;
			if( ( r = loadModule( p ) ) )
				{
				destruct( r ) ;
				return 0 ;
				}
			}
		if( ( p = ((object)p->datum)->precursor ) == P )
			return 1 ;
		} while( p ) ;

	return 0;
	}


int p_read( atom A )
	{
	extern atom		(*sframe_scope)(void) ;
	objvar			v ;
	atom			s ;

#ifdef _DEBUG
		{
		extern atom aSObjVar ;
		if( A->precursor != aSObjVar ) _asm int 3 ;
		}
#endif

	v	= ((sobjvar)A->datum)->ov ;
	if( v->perms & P_AR )
		return 1 ;
	if( v->principal == ( s = sframe_scope() ) )
		return 1 ;
	if( v->perms & P_TR	&& s == v->a->orbit )
			return 1 ;
	if( v->perms & P_OR	&& s == ((object)v->a->orbit->datum)->principal )
			return 1 ;
	if( v->perms & P_CHOWN	&& precesses( v->principal , s ) )
			return 1 ;
	if( ((object)s->datum)->perms & P_WIZARD )
		return 1 ;
	return 0 ;
	}


int p_write( atom A )
	{
	extern atom		(*sframe_scope)(void) ;
	objvar			v ;
	atom			s ;

#ifdef _DEBUG
		{
		extern atom aSObjVar ;
		if( A->precursor != aSObjVar ) _asm int 3 ;
		}
#endif

	v	= ((sobjvar)A->datum)->ov ;
	if( v->perms & P_AW )
		return 1 ;
	if( v->principal == ( s = sframe_scope() ) )
		return 1 ;
	if( v->perms & P_TW	&& s == v->a->orbit )
			return 1 ;
	if( v->perms & P_OW	&& s == ((object)v->a->orbit->datum)->principal )	// v->oo->principal
			return 1 ;
	if( v->perms & P_CHOWN	&& precesses( v->principal , s ) ) ;
			return 1 ;
	if( ((object)s->datum)->perms & P_WIZARD )
		return 1 ;
	return 0 ;
	}


int p_rw( atom A )
	{
	return ( p_read( A ) & p_write( A ) ) ;
	}


int p_principal( objvar V ) 
	{
	extern atom		(*sframe_scope)(void) ;
	atom			s ;

	if( V->principal == ( s = sframe_scope() ) )
		return 1 ;
	if( ((object)s->datum)->perms & P_WIZARD )
		return 1 ;
	return 0 ;
	}


int p_principal_o( object O ) 
	{
	extern atom		(*sframe_scope)(void) ;
	atom			s ;

	if( O->principal == ( s = sframe_scope() ) )
		return 1 ;
	if( ((object)s->datum)->perms & P_WIZARD )
		return 1 ;
	return 0 ;
	}
