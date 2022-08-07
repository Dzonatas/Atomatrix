// aProgram.c			Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "machine/executable.h"
#include "program.h"
#include "cmp_s.h"

atom				aProgram ;


atom aProgram_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		(*atomize)(atom) ;

	return aProgram = atomize( atomine ) ;
	}


atom aProgram_translatomize_a( atom X )
	{
	extern atom		aXML ;
	extern atom		aAsciiString ; 
	extern atom		(*atomize)(atom) ;
//	extern void		(*interatomize)(atom,atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*aXML_translatomize_XML)(atom) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern atom		(*aXML_element_s)(string) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*destruct)(atom) ;
	atom			a ;
	atom			m ;
	string			s ;
	atom			i = atominul ;
	atom			c = atominul ;
	program			p ;


	if( X->precursor == aProgram )
		{
		p = (program) X->datum ;
		m = aXML_element_s( "method" ) ;
		a = aAsciiString_atomize( str_dup( p->name ) ) ;		// UPDATE: pptr->name should be an atom
		aXML_interatomize_a_a( aXML_element_a_s( m , "identifier" ) , a ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		a = aAsciiString_atomize( str_dup( p->psrc ) ) ;		// UPDATE: pptr->psrc should be an atom
		aXML_interatomize_a_a( aXML_element_a_s( m , "source" ) , a ) ;
		if( --a->allusions == 0 )
			destruct( a ) ;
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
			if( cmp_s( "source" , s ) == 0 )
				{
				while( m && m->precursor != aXML )
					m = m->interatoms ;
				c = m ;
				}
			}
		while( ( a = a->interatoms ) ) ;
	a	= atomize( aProgram ) ;
	p = a->datum	= allocz( sizeof( struct _program ) ) ;
	p->a		= a ;
	m	= aXML_translatomize_XML( i ) ;
	if( m->precursor != aAsciiString )
		_asm int 3 ;					// UPDATE: return error
	p->name		= str_dup( m->datum ) ;
	if( --m->allusions == 0 )
		destruct( m ) ;
	if( c )	{
		extern void	(*destruct)(atom) ;
		m	= aXML_translatomize_XML( c ) ;
		p->psrc		= str_dup( m->datum ) ;
		destruct( m ) ;
		}
	else
		p->psrc	= str_dup( "" ) ;
	return a ;
	}


void aProgram_destruct_a( atom A )
	{
	extern void		(*dealloc)(void*) ;
	program			p ;
	if( A->datum )
		{
		p = A->datum ;
		if( p->name )
			dealloc( p->name ) ;
		if( p->exep )
			p->exep->prog = 0 ;
		if( p->psrc )
			dealloc( p->psrc ) ;
		dealloc( p ) ;
		}
	A->datum = 0 ;
	}


void aProgram_constructent( void )
	{
	extern atom		p_identifier(atom,unsigned,atom*);
	extern atom		p_inherit(atom,unsigned,atom*);
	extern atom		p_source(atom,unsigned,atom*);
	extern atom		p_orbit(atom,unsigned,atom*);
	extern atom		p_delete(atom,unsigned,atom*);
	extern atom		p_evaluate(atom,unsigned,atom*);
	extern atom		p_sleep(atom,unsigned,atom*);
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
	extern atom		(*oDestructor)(atom,void*) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*aXML_translatomize_a_p)(atom,atom(*)(atom)) ;

	interatomize( aXML_translatomize_a_p( aAsciiString_atomize( str_dup( "method" )  ) , aProgram_translatomize_a ) , aProgram ) ;

	atomethodize( aProgram , "inherit",	p_inherit ) ;
	atomethodize( aProgram , "evaluate",	p_evaluate ) ;
	atomethodize( aProgram , "delete",	p_delete ) ;
	atomethodize( aProgram , "orbit",	p_orbit ) ;
	atomethodize( aProgram , "identifier",	p_identifier ) ;
	atomethodize( aProgram , "sleep",	p_sleep ) ;
	atomethodize( aProgram , "source",	p_source ) ;

	oDestructor( aProgram , aProgram_destruct_a ) ;
	}


void aProgram_destructor( void )
	{
	extern void		(*destruct)(atom) ;

	destruct( aProgram ) ;
	}