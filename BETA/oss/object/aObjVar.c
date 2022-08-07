// aObjVar.c			Copyright (c) 2004 BALLARD


#include "ATOMATRIX.h"
#include "objvar.h"
#include "cmp_s.h"

#include "stdio.h"

atom				aObjVar ;


atom aObjVar_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;

	return aObjVar = atomize( atomine ) ;
	}


atom aObjVar_translatomize_a( atom X )
	{
	extern atom		aXML ;
	extern atom		aObjVar ;
	extern atom		aUndefined ;
	extern atom		aAsciiString ;
	extern atom		addObjVar(atom,string,atom*) ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*atomize)(atom) ;
	extern string		(*str_dup)(string) ;
	extern void*		(*allocz)(unsigned int) ;
	extern atom		(*aXML_translatomize_XML)(atom) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern atom		(*aXML_element_s)(string) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*destruct)(atom) ;
	extern int		illogicated(atom) ;
	atom			a ;
	atom			m ;
	atom			i = atominul ;
	atom			p = atominul ;
	atom			o = atominul ;
	atom			d = atominul ;
	atom			t = atominul ;
	string			s ;
	objvar			ov ;


	if( X->precursor == aObjVar )
		{
		extern atom (*oInteger)(unsigned) ;
		ov = (objvar) X->datum ;
		m = aXML_element_s( "variant" ) ;
		a = aAsciiString_atomize( str_dup( ov->name ) ) ;		// UPDATE: ov->name should be an atom
		aXML_interatomize_a_a( aXML_element_a_s( m , "identifier" ) , a ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		if( ov->principal )
			aXML_interatomize_a_a( aXML_element_a_s( m , "principal" ) , ov->principal ) ;
		if( ov->perms )
			{
			a = oInteger( ov->perms ) ;
			aXML_interatomize_a_a( aXML_element_a_s( m , "principle" ) , a ) ;
			if( --a->allusions == 0 )
				destruct( a ) ;
			}
		if( ov->v != aUndefined )
			aXML_interatomize_a_a( aXML_element_a_s( m , "source" ) , ov->v ) ;
		return m ;
		}
	if( ! ( a = X->interatom ) )
		return aError_pUndefined() ;
	do	if( a->precursor == aXML )
			{
			m = a->interatom ;
			s = m->interatom->datum ;
			if( ! ( m = m->interatoms ) )
				continue ;
			if( cmp_s( "identifier" , s ) == 0 )
				{
				while( m && m->precursor != aXML )
					m = m->interatoms ;
				i = m ;
				}
			else
			if( cmp_s( "principal" , s ) == 0 )
				{
				while( m && m->precursor != aXML )
					m = m->interatoms ;
				o = m ;
				}
			else
			if( cmp_s( "principle" , s ) == 0 )
				{
				while( m && m->precursor != aXML )
					m = m->interatoms ;
				p = m ;
				}
			else
			if( cmp_s( "source" , s ) == 0 )
				{
				while( m && m->precursor != aXML )
					m = m->interatoms ;
				d = m ;
				}
			}
		while( ( a = a->interatoms ) ) ;

	a			= atomize( aObjVar ) ;
	ov = a->datum		= allocz( sizeof( struct _objvar ) ) ;
	if( illogicated( m = aXML_translatomize_XML( i ) ) )
		{
		destruct( a ) ;
		return aError_pUndefined() ;
		}
	if( m->precursor != aAsciiString )
		_asm int 3 ;					// UPDATE: return error
	ov->name		= str_dup( m->datum ) ;		// UPDATE: should pool string data
	if( --m->allusions == 0 )
		destruct( m ) ;
	ov->a			= a ;
	if( o && illogicated( ov->principal = aXML_translatomize_XML( o ) ) )
		{
		destruct( a ) ;
		return aError_pUndefined() ;
		}
	if( p )	{
		extern atom aInteger ;
		if( illogicated( o = aXML_translatomize_XML( p ) ) )
			{
			destruct( a ) ;
			return aError_pUndefined() ;
			}
		if( o->precursor != aInteger )
			_asm int 3 ;
		ov->perms	= (unsigned short) o->datum ;
		destruct( o ) ;
		}
	if( d )
		{
		if( illogicated( ov->v = aXML_translatomize_XML( d ) ) )
			{
			destruct( a ) ;
			return aError_pUndefined() ;
			}
		}
	else
		( ov->v	= aUndefined )->allusions++ ;
	return a ;
	}


void aObjVar_destruct_a( atom A )
	{
	extern void		(*destruct)(atom) ;
	extern void		(*dealloc)(void*) ;
	objvar			v ;
	atom			a ;

	if( A->datum )
		{
		if( a=(v=A->datum)->v )
			if( --a->allusions == 0 )
				destruct( a );
		if( v->name )
			dealloc( v->name ) ;
		if( a = v->principal )
			if( --a->allusions == 0 )
				destruct( a ) ;
		dealloc( v ) ;
		}
	A->datum = 0 ;
	}



void aObjVar_constructent( void )
	{
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
	extern atom		(*oDestructor)(atom,void*) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*aXML_translatomize_a_p)(atom,atom(*)(atom)) ;
	extern atom		v_delete(atom,unsigned,atom*);
	extern atom		v_principal(atom,unsigned,atom*);
	extern atom		v_orbit(atom,unsigned,atom*);
	extern atom		v_security(atom,unsigned,atom*);
	extern atom		v_identifier(atom,unsigned,atom*);

	interatomize( aXML_translatomize_a_p( aAsciiString_atomize( str_dup( "variant" )  ) , aObjVar_translatomize_a ) , aObjVar ) ;

	atomethodize( aObjVar , "delete",	v_delete ) ;
	atomethodize( aObjVar , "identifier",	v_identifier ) ;
	atomethodize( aObjVar , "principal",	v_principal ) ;
	atomethodize( aObjVar , "security",	v_security ) ;
	atomethodize( aObjVar , "orbit",	v_orbit ) ;

	oDestructor( aObjVar , aObjVar_destruct_a ) ;
	}


void aObjVar_destructor( void )
	{
	extern void		(*destruct)(atom) ;

	destruct( aObjVar ) ;
	}