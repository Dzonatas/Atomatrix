// aXML.c			Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"

#include "unicode.h"

#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "objvar.h"
#include "varray.h"

#include "cmp_s.h"


atom				aXML ;
atom				aXML_aElement ;
atom				aXML_aData ;
atom				aXML_aAttribute ;			// UPDATE: should be like aXML with only element and data
atom				aXML_aTranslator ;			// datum is an address of a XML translation procedure for a datatype
atom				aXML_aDatatype ;			// datum is an atom reference to a textual identifier

atom				aXML_asciiBeginTag ;
atom				aXML_asciiEndTag ;
atom				aXML_asciiEqual ;
atom				aXML_asciiSpace ;
atom				aXML_asciiDiagonal ;
atom				aXML_asciiQuote ;


atom aXML_atomize( void )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	a	= atomize( aXML ) ;
	return a ;
	}


atom aXML_element_a_s( atom X , string S )
	{
	extern atom		atomize(atom) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;
	atom			x ;
#ifdef DEBUG
	if( X->precursor != aXML )
		_asm int 3 ;
#endif
	(atom)X->datum = ( ( ( x = atomize( aXML ) )->orbit = X )->interatom ? ( ((atom)X->datum)->interatoms = x ) : ( X->interatom = x ) ) ;
	(atom)x->datum = ( ( a = atomize( aXML_aElement ) )->orbit = x )->interatom = a ;
	interatomize( aAsciiString_atomize( string_duplicate( S ) ) , a ) ;
	return x ;
	}


atom aXML_element_s( string S )
	{
	extern atom		atomize(atom) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;
	atom			x ;

	x = atomize( aXML ) ;
	(atom)x->datum = ( ( a = atomize( aXML_aElement ) )->orbit = x )->interatom = a ;
	interatomize( aAsciiString_atomize( string_duplicate( S ) ) , a ) ;
	return x ;
	}


atom aXML_attribute_a_s_a( atom X , string S , atom A )
	{
	extern atom		atomize(atom) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;

	(atom)X->datum = ( ( ( a = atomize( aXML_aAttribute ) )->orbit = X )->interatom ? ( ((atom)X->datum)->interatoms = a ) : ( X->interatom = a ) ) ;
	( a->interatom = aAsciiString_atomize( string_duplicate( S ) ) )->orbit = a ;
	if( A->orbit )
		interatomize( A , atominul ) ;
	( a->interatom->interatoms = A )->orbit = a ;
	return X ;
	}


atom aXML_data_a( atom X )
	{
	extern atom		atomize(atom) ;
	atom			a ;
#ifdef DEBUG
	if( X->precursor != aXML )
		_asm int 3 ;
#endif
	return (atom)X->datum = ( ( ( a = atomize( aXML_aData ) )->orbit = X )->interatom ? ( ((atom)X->datum)->interatoms = a ) : ( X->interatom = a ) ) ;
	}

static void addEquate( atom A , atom B )
	{
	extern atom		aAsciiString_pAddEquate(atom,atom) ;
	extern void		destruct(atom) ;
	atom			a ;

	a = aAsciiString_pAddEquate( A , B ) ;
//	if( --a->allusions == 0 )
//		destruct( a ) ;
	}

static void aXML_iterateAndDocument( atom X , atom A )		// UPDATE: invoke translatomize procedures
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiString_pAddEquate(atom,atom) ;
	extern atom		aAsciiString_atomize(string) ;
	extern void		console_output(string) ;
	atom			e = X->interatom ;
	atom			a = e ;

#ifdef DEBUG
	if( e->precursor != aXML_aElement )
		_asm int 3 ;
#endif
	addEquate( A , aXML_asciiBeginTag ) ;
	addEquate( A , e->interatom ) ;		// UPDATE: invoke procedure on an aXML_aElement intratom instead of a->interatom
	if( ( a = a->interatoms ) && a->precursor == aXML_aAttribute )
		{
		atom	b = a->interatom ;
		do	{
			addEquate( A , aXML_asciiSpace ) ;
			addEquate( A , b ) ;
			addEquate( A , aXML_asciiEqual ) ;
			addEquate( A , aXML_asciiQuote ) ;
			b = b->interatoms ;
			addEquate( A , b ) ;
			addEquate( A , aXML_asciiQuote ) ;
			} while( ( a = a->interatoms ) && a->precursor == aXML_aAttribute ) ;
		if( ! a )
			{
			addEquate( A , aXML_asciiDiagonal ) ;
			addEquate( A , aXML_asciiEndTag ) ;
			return ;
			}
		}
	if( ! a )
		{
		addEquate( A , aXML_asciiDiagonal ) ;
		addEquate( A , aXML_asciiEndTag ) ;
		return ;
		}
	addEquate( A , aXML_asciiEndTag ) ;
	do	if( a->precursor == aXML )
			aXML_iterateAndDocument( a , A ) ;
		else
		if( a->precursor == aXML_aData )
			addEquate( A , a->datum ) ;
		while( ( a = a->interatoms ) ) ;
	addEquate( A , aXML_asciiBeginTag ) ;
	addEquate( A , aXML_asciiDiagonal ) ;
	addEquate( A , e->interatom ) ;		// UPDATE: invoke procedure on an aXML_aElement intratom instead of a->interatom
	addEquate( A , aXML_asciiEndTag ) ;
	}


atom aXML_atomize_a( atom X )
	{
	extern atom		aUndefined ;
	extern atom		(*oError)(string) ;
	atom			a ;
	atom			aa ;

//	_asm int 3 ;

	aa	= X ;
	do	{
		a = aa->interatom ;
		do	if( a->precursor == aXML_aTranslator )
				return ((atom(*)(atom))a->datum)( X ) ;
			while( ( a = a->interatoms ) ) ;
		} while( ( aa = aa->precursor ) ) ;
	if( X == aUndefined )
		{
		return aXML_element_s( "undefined" ) ;
		}
	return oError( "translation" ) ;		// aError_pUndefined() ;
	}


atom aXML_interatomize_a_a( atom X , atom A )
	{
	extern atom		(*oError)(string) ;
	atom			a ;
	atom			aa ;
	atom			x ;

//	_asm int 3 ;

	aa	= A ;
	do	{
		if( ( a = aa->interatom ) )
			do	if( a->precursor == aXML_aTranslator )
					{
					x = ((atom(*)(atom))a->datum)( A ) ;
					(atom)X->datum = ( ( x->orbit = X )->interatom ? ( ((atom)X->datum)->interatoms = x ) : ( X->interatom = x ) ) ;
					return x ;
					}
				while( ( a = a->interatoms ) ) ;
		} while( ( aa = aa->precursor ) ) ;
	return oError( "translation" ) ;		// aError_pUndefined() ;
	}


atom aXML_document_a( atom X )
	{
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
	extern void		iterateModule( atom  ) ;
	atom			a ;

	a	= aAsciiString_atomize( allocz( 1 ) ) ;		// UPDATE: aAsciiString_atomize(void), aAsciiString_atomize_s(string)
	aXML_iterateAndDocument( X , a ) ;
	return a ;
	}


atom aXML_translatomize_a_p( atom A , atom (*P)(atom) )
	{
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;
	atom			b ;

	a	= atomize( aXML_aTranslator ) ;
	a->datum= (void*) P ;
	if( A )	{
		b = atomize( aXML_aDatatype ) ;
		b->datum= A ;
		A->allusions++ ;
		interatomize( b , a ) ;
		}
	return a ;
	}


static string string_extract( string A , string B )
	{
	extern void *		allocm(unsigned) ;
	int			l;
	string			s;
	string			p;
	string			r;

	if( A )
		{
		l = 0 ;
		s = A ;
		while( s++ != B )
			l++ ;
		r = p = allocm( l + 1 ) ;
		s = A ;
		while( s != B ) 
			*(p++) = *(s++) ;
		*p = 0 ;
		}
	else	{
		r = allocm( 1 ) ;
		*r = 0 ;
		}
	return r ;
	}


#ifdef DEBUG

static void iterateModule( atom M )
	{
	extern atom		aAsciiString ;
	extern void		console_output(string) ;
	atom			a = M->interatom ;

	do 	if( a->precursor == aXML_aElement )
			{
			console_output( a->interatom->datum ) ;
			console_output( "|" ) ;
			}
		else
		if( a->precursor == aXML )
			{
			console_output( "<=" ) ;
			iterateModule( a ) ;
			console_output( "=>" ) ;
			}
		while( ( a = a->interatoms ) ) ;
	}

#endif

atom aXML_modulize( atom A )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiString_atomize(string) ;
	extern atom		atomize(atom) ;
	extern void*		allocz(unsigned) ;
	extern void		interatomize(atom,atom) ;
	extern void		intratomize(atom,atom) ;
	extern void		destruct(atom) ;
	extern atom		(*oError)(string) ;
	string			c ;
	string			d ;
	unsigned		t ;
	unsigned		s ;
	atom			a ;
	atom			m ;
	unsigned		l ;

#	define aXML_interatomize(_1,_2,_3) \
		interatomize( _3 , (atom)_1->datum = ( ( ( a = atomize( _2 ) )->orbit = _1 )->interatom ? ( ((atom)_1->datum)->interatoms = a ) : ( _1->interatom = a ) ) )

#	define aXML_interatomize_a(_1) \
		 ( ((atom)m->datum)->interatom->interatoms = _1 )->orbit = ((atom)m->datum)

#	define aXML_interatomizeData(_1,_2) \
		 ( (atom)_1->datum = ( ( ( a = atomize( aXML_aData ) )->orbit = _1 )->interatom ? ( ((atom)_1->datum)->interatoms = a ) : ( _1->interatom = a ) ) )->datum = _2

#ifdef DEBUG
	if( A->precursor != aAsciiString )
		return oError( "assertion failed aXML_modulize( ! aAsciiString ) " ) ;
#endif

	m	= atomize( aXML ) ;

	l	= 0 ;
	d	= 0 ;
	t	= 0 ;
	s	= 0 ;
	for( c = A->datum ; *c ; c++ )
		{
		if( t )	{
			if( s )	{
				if( *c == '\"' )		// end quote
					{
					aXML_interatomize_a( aAsciiString_atomize( string_extract( d , c ) ) ) ;
					d = 0 ;
					s = 0 ;
					continue ;
					}
				if( ! d )
					d = c ;
        			continue ;
				}
			if( *c == '>' )
				{
				if( t == 3 )
					{
					// end tag
					m = m->orbit ;			// UPDATE: error if ! m->orbit
					t = 0 ;
					d = 0 ;
					continue ;
					}
				if( t == 2 )
					{
					// end identifier
#ifdef DEBUG
					if( m->datum )
						_asm int 3 ;
#endif
					aXML_interatomize( m , aXML_aElement , aAsciiString_atomize( string_extract( d , c ) ) ) ;
					d = 0 ;
					t = 0 ;
					continue ;
					}
				// begin tag data
				// attribute
				d = 0 ;
				t = 0 ;
				continue ;
				}
			if( *c == '=' )
				{
				if( d )
					aXML_interatomize( m , aXML_aAttribute , aAsciiString_atomize( string_extract( d , c ) ) ) ;
				// attribute
				d = 0 ;
				continue ;
				}
			if( *c == '\"' )
				{
				s = 1 ;
				d = 0 ;
				continue ;
				}
			if( *c == '/' )
				{
				if( d == 0 && t == 4 )
					{
					t = 3 ;
					continue ;
					}
				if( *(c+1) != '>' )
					return oError( "not well-formed" ) ;
				if( t == 2 && d )
					aXML_interatomize( m , aXML_aElement , aAsciiString_atomize( string_extract( d , c ) ) ) ;
				c++ ;
				t = 0 ;
				m = m->orbit ;
				continue ;
				}
			if( *c == ' ' )
				{
				if( t == 2 )
					{
#ifdef DEBUG
					if( m->datum )
						_asm int 3 ;
#endif
					aXML_interatomize( m , aXML_aElement , aAsciiString_atomize( string_extract( d , c ) ) ) ;
					t = 1 ;
					}
				else
				if( d )
					aXML_interatomize( m , aXML_aAttribute , aAsciiString_atomize( string_extract( d , c ) ) ) ;
				d = 0 ;
				continue ;
				}
			if( t == 4 )
				{
				if( *c == '?' )
					{
					t = 0 ;
					d = c - 1 ;
					continue ;
					}
				if( *c == '!' )
					{
					t = 0 ;
					d = c - 1 ;
					continue ;
					}
				m->datum = ( ( ( a = atomize( aXML ) )->orbit = m )->interatom ? ( ((atom)m->datum)->interatoms = a ) : ( m->interatom = a ) ) ;
				m = a ;
				d = c ;		// begin tag name
				t = 2 ;
				}
			if( ! d )
				d = c ;
			continue ;
			}
		if( *c == '<' )
			{
			t = 4 ;
			if( d )	{
				aXML_interatomizeData( m , aAsciiString_atomize( string_extract( d , c ) ) ) ;
				d = 0 ;
				}
			continue ;
			}
		if( ! d )
			d = c ;
		continue ;
		}
	if( m->orbit )
		_asm int 3 ;								// UPDATE: error if m->orbit != atominul

	if( t )
		_asm int 3 ;								// UPDATE: error if m->orbit != atominul

	if( d )	{
		aXML_interatomizeData( m , aAsciiString_atomize( string_extract( d , c ) ) ) ;
		d = 0 ;
		}

//	iterateModule( m ) ;

	return m ;
	}




/*
	{
	t	?
		{
		s ?	{
			c == "\"" ?
				{
				attribute_equals( m , d ) ;
				s = 0 ;
				d = "" ;
				< ;
				}
			d += c ; // 3
			< ;
			}
		c == ">" ?
			{
			t == 3 ?
				{
				m = popr( z ) ;
				d = "" ;
				t = 0 ;
				< ;
				}
			t == 2 ?
				{
				m.identifier = d ;
				t = 0 ;
				d = "" ;
				< ;
				}
			attribute( m , d ) ;
			d = "" ;
			t = 0 ;
			< ;
			}
		c == "=" ?
			{
			attribute( m , d )  ;
			d = "" ;
			< ;
			}
		c == "\"" ?
			{
			attribute( m , d )  ;
			d = "" ;
			s = 1 ;
			< ;
			}
		c == "/" && ~~d && t == 4 ?
			{
			t = 3 ;
			< ; 
			}
		c == " " ?
			{
			t == 2 ?
				{
				m.identifier = d ;
				m addVariant( "attributes" ) ;
				m.attributes = {} ;
				t = 1 ;
				}
			,	attribute( m , d )  ;
			d = "" ;
			< ;
			}
		t == 4 ?
			{
			c == "?" ?
				{
				t = 0 ;
				d = "<!" ;
				< ;
				}
			c == "!" ?
				{
				t = 0 ;
				d = "<!" ;
				< ;
				}
			a = mm atomize() ;
			pushr( m.set , a )  ;
			a reintermodulize( m ) ;
			pushr( z , m ) ;
			m = a ;
			d = "" ;
			t = 2 ;
			a = 0 ;
			}
		d += c ;  // 1
		< ;
		}
	c == "<" ?
		{
		t = 4 ;
		d ? pushr( m.set , d )  ;
		d = "" ;
		< ;
		}
	d += c ;  // 2
	}
t == 0 && d ? pushr( m.set , d )  ;

*/


atom aXML_translatomize_XML( atom A )
	{
	extern atom		(*oError)(string) ;
	atom			a ;
	string			s ;
	atom			d ;

	if( ( a = A )->precursor != aXML )
		return oError( "not XML" ) ;
	s	= a->interatom->interatom->datum ;
	if( ( d = aXML_aDatatype->intratom ) )
		do	if( cmp_s( s , (string)((atom)d->datum)->datum ) == 0 )
				return ((atom (*)(atom))d->orbit->datum)( A ) ;
			while( d = d->intratoms ) ;
#ifdef DEBUG
	_asm int 3;
#endif
	return oError( "undefined" ) ;
	}





atom aXML_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		aAtomatrical ;
	extern atom		atomize(atom) ;
	extern atom		oAtomical(atom,string,atom) ;
	extern atom		oFunction(atom,string,void*) ;


	aXML	= atomize( atomine ) ;

	oAtomical( aXML		, "aElement" ,		aXML_aElement	= atomize( atomine ) ) ;
	oAtomical( aXML		, "aData" ,		aXML_aData	= atomize( aAtomatrical ) ) ;
	oAtomical( aXML		, "aAttribute" ,	aXML_aAttribute	= atomize( atomine ) ) ;
	oAtomical( aXML		, "aTranslator" ,	aXML_aTranslator= atomize( atomine ) ) ;
	oAtomical( aXML		, "aDatatype" ,		aXML_aDatatype	= atomize( aAtomatrical ) ) ;

	oFunction( aXML		, "modulize" ,		aXML_modulize	) ;
	oFunction( aXML		, "translatomize_XML" ,	aXML_translatomize_XML	) ;
	oFunction( aXML		, "translatomize_a_p" ,	aXML_translatomize_a_p	) ;

	oFunction( aXML		, "atomize" ,		aXML_atomize	) ;
	oFunction( aXML		, "atomize_a" ,		aXML_atomize_a	) ;
	oFunction( aXML		, "element_s" ,		aXML_element_s	) ;
	oFunction( aXML		, "data_a" ,		aXML_data_a	) ;
	oFunction( aXML		, "element_a_s" ,	aXML_element_a_s	) ;
	oFunction( aXML		, "attribute_a_s_a" ,	aXML_attribute_a_s_a	) ;
	oFunction( aXML		, "interatomize_a_a" ,	aXML_interatomize_a_a	) ;
	oFunction( aXML		, "document_a" ,	aXML_document_a	) ;
//	oFunction( aXML		, "attribute_a_s_s" ,	aXML_attribute_a_s_s	) ;

	return aXML ;
	}



void aXML_constructent( void )
	{
	extern string		string_duplicate(string) ;
	extern atom		aAsciiString_atomize(string) ;
//	extern atom		oDestructor(atom,void*) ;

//	oDestructor( aXML , aXML_destruct_a ) ;
	aXML_asciiBeginTag	= aAsciiString_atomize( string_duplicate( "<" ) ) ;	// UPDATE: aAsciiCharacter_atomize(...)
	aXML_asciiEndTag	= aAsciiString_atomize( string_duplicate( ">" ) ) ;
	aXML_asciiEqual 	= aAsciiString_atomize( string_duplicate( "=" ) ) ;
	aXML_asciiSpace 	= aAsciiString_atomize( string_duplicate( " " ) ) ;
	aXML_asciiDiagonal 	= aAsciiString_atomize( string_duplicate( "/" ) ) ;
	aXML_asciiQuote 	= aAsciiString_atomize( string_duplicate( "\"" ) ) ;
	return ;
	}


void aXML_destructor( void )
	{
	extern void		destruct(atom) ;

	destruct( aXML_aDatatype ) ;
	destruct( aXML_aTranslator ) ;
	destruct( aXML_aAttribute ) ;
	destruct( aXML_aData ) ;
	destruct( aXML_aElement ) ;
	destruct( aXML ) ;
	}
