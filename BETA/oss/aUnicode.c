// aUnicode.c			Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "unicode.h"
#include "cmp_s.h"

atom		aUnicode ;
atom		aAscii ;
atom		aUnicoded ;


atom aUnicode_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;
	extern atom		oAtomical(atom,string,atom) ;
	extern atom		aUnicodedEntity_pregeneration(void) ;
	extern atom		aUnicodedString_pregeneration(void) ;
	extern atom		aAsciiCharacter_pregeneration(void) ;
	extern atom		aAsciiString_pregeneration(void) ;

	aUnicode	= atomize( atomine ) ;

	oAtomical( aUnicode ,	"unicoded" ,	aUnicoded = atomize( atomine ) ) ;
	oAtomical( aUnicode ,	"ascii" ,	aAscii = atomize( aUnicoded ) ) ;
	oAtomical( aUnicoded ,	"string" ,	aUnicodedString_pregeneration() ) ;
	oAtomical( aUnicoded ,	"entity" ,	aUnicodedEntity_pregeneration() ) ;
	oAtomical( aAscii ,	"string" ,	aAsciiString_pregeneration() ) ;
	oAtomical( aAscii ,	"character" ,	aAsciiCharacter_pregeneration() ) ;
	return aUnicode ;
	}


atom aUnicode$entity( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aUnicodedEntity_atomize(unicode) ;
	extern atom		aAsciiCharacter_atomize(byte) ;

	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( (unsigned)V[0]->datum >= 0xFFFE )
		{
		aError_RANGE->allusions++ ;
		return aError_RANGE ;
		}
	if( (unsigned)V[0]->datum > 127 )
		return aUnicodedEntity_atomize( (unicode)V[0]->datum ) ;
	return aAsciiCharacter_atomize( (byte)V[0]->datum ) ;
	}


atom aUnicode$string( atom A , int T , atom* V )
	{
	typedef	unsigned short	unicode ;
	typedef	unicode *	unicodes ;
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	extern atom		aUnicodedEntity_atomize(unicode) ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiCharacter ;
	extern unsigned		string_length(string) ;
	extern void *		allocm(unsigned) ;		
//	extern void *		allocz(unsigned) ;
	unicodes		s ;
	unsigned		l ;
	unicodes		a ;
	string			b ;

	if( V[0]->precursor == aUnicodedString )
		{
		V[0]->allusions++ ;
		return V[0] ;
		}
	if( V[0]->precursor == aUnicodedEntity )		// UPDATE:
		{
		V[0]->allusions++ ;
		return V[0] ;
		}
	if( V[0]->precursor == aAsciiCharacter )		// UPDATE:
		{
		return aUnicodedEntity_atomize( (unicode) (byte) V[0]->datum ) ;
		}
	if( V[0]->precursor != aAsciiString )			// UPDATE: move routine to aAsciiString$unicode()
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	l	= string_length( b = V[0]->datum ) ;		// UPDATE: "1 to 2" overflow limitation

	a = s	= allocm( sizeof(unicode) + ( l * sizeof(unicode) ) ) ;
//	c	= 0 ;
	while( l-- )
		*(a++) = *(b++) ;
	*a = 0 ;

	return aUnicodedString_atomize( s ) ;
	}




/*
atom aInteger$string( atom A , unsigned T , atom* V )
	{
	string			p ;
	unsigned		r ;
	unsigned		z = 0 ;

	z	= 0 ;
	r	= ...

	if( ! r )
		z = 1 ;
	else
		while( r /= 10 )
			z++ ;
	*( p = ( allocm( z + 1 ) + z ) = 0 ;
	while( --z )
		{
		*--p = '0' + r % 10 ;
		r /= 10 ;
		} ;
	*--p	= '0' + r ;

	return aAsciiString_atomize( string_duplicate( n ) ) ;
	}
*/


atom aAscii_translatomize( atom X ) 
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aXML_aAttribute ;
	extern atom		(*oError)(string) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		aAsciiString_canonicalize(string) ;
	extern atom		aXML_element_s(string) ;
	extern atom		aXML_data_a(atom) ;
	extern atom		aXML_attribute_a_s_a(atom,string,atom) ;
	extern void*		allocm(unsigned) ;
        atom			a ;
	string			s ;

	if( X->precursor == aAsciiString )
		{
		atom	a ;
		aXML_data_a( a = aXML_element_s( "ascii" ) )->datum = aAsciiString_atomize( aAsciiString_canonicalize( X->datum ) ) ;
		return a ;
		}
	if( X->precursor == aAsciiCharacter )
		{
		atom	a ;
		string	p ;
		unsigned r ;
		unsigned z = 0 ;
		a = aXML_element_s( "ascii" ) ;
		if( ! ( r = (unsigned)X->datum ) )
			z = 1 ;
		else	{
			while( r /= 10 )
				z++ ;
			r = (unsigned) X->datum ;
			}
		*( p = ( ((string)allocm( z + 1 )) + z ) ) = 0 ;
		while( --z )
			{
			*--p = '0' + r % 10 ;
			r /= 10 ;
			} ;
		*--p	= '0' + r ;
		aXML_attribute_a_s_a( a , "n" , aAsciiString_atomize( p ) ) ;
		return a ;
		}
	a	= X->interatom ;
	s	= a->interatom->datum ;
	a	= a->interatoms ;
	if( cmp_s( s , "ascii" ) == 0 )
		{
		extern atom	aAsciiCharacter_atomize(byte) ;
		extern string	aAsciiString_decanonicalize(string) ;
		extern string	string_duplicate(string) ;
		if( a->precursor == aXML_aAttribute )
			{
			unsigned  x = 0 ;
			s = a->interatom->interatoms->datum ;
			if( *s )
				do	{
					x *= 10 ;			// UPDATE: optimize first pass
					if( *s < '0' || *s > '9' )
						return oError( "not well-formed XML" ) ;
					x += (byte)( *s - '0' ) ;	// UPDATE: test for overflow
					} while( *(++s) ) ;
			if( x > 127 )
				return oError( "not well-formed XML" ) ;
			return aAsciiCharacter_atomize( x ) ;
			}
		if( ! ( s = aAsciiString_decanonicalize( ((atom)a->datum)->datum ) ) )
				return oError( "not well-formed XML" ) ;
		return aAsciiString_atomize( s ) ;
		}
	return 0 ;
	}


atom aUnicoded_translatomize( atom X ) 
	{
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern string		aUnicodedString_canonicalize(string) ;
	extern atom		aXML_aAttribute ;
	extern atom		(*oError)(string) ;
	extern atom		aXML_element_s(string) ;
	extern atom		aXML_data_a(atom) ;
	extern atom		aXML_attribute_a_s_a(atom,string,atom) ;
	extern void*		allocm(unsigned) ;
        atom			a ;
	string			s ;
	
	if( X->precursor == aUnicodedString )
		{
		atom	a ;
		aXML_data_a( a = aXML_element_s( "unicode" ) )->datum = aAsciiString_atomize( aUnicodedString_canonicalize( X->datum ) ) ;
		return a ;
		}
	if( X->precursor == aUnicodedEntity )
		{
		atom	a ;
		string	p ;
		unsigned r ;
		unsigned z = 0 ;
		a = aXML_element_s( "unicode" ) ;
		if( ! ( r = (unsigned)X->datum ) )
			z = 1 ;
		else	{
			while( r /= 10 )
				z++ ;
			r = (unsigned) X->datum ;
			}
		*( p = ( ((string)allocm( z + 1 )) + z ) ) = 0 ;
		while( --z )
			{
			*--p = '0' + r % 10 ;
			r /= 10 ;
			} ;
		*--p	= '0' + r ;
		aXML_attribute_a_s_a( a , "n" , aAsciiString_atomize( p ) ) ;
		return a ;
		}
	a	= X->interatom ;
	s	= a->interatom->datum ;
	a	= a->interatoms ;
	if( cmp_s( s , "unicode" ) == 0 )
		{
		extern atom	aUnicodedString_atomize(unicodes) ;
		extern atom	aUnicodedEntity_atomize(unicode) ;
		extern unicodes	aUnicodedString_decanonicalize(string) ;
		extern unicodes	unicode_s(string) ;
		unicodes	u ;
		if( a->precursor == aXML_aAttribute )
			{
			string	  s = a->interatom->interatoms->datum ;
			unsigned  x = 0 ;
			if( *s )
				do	{
					x *= 10 ;			// UPDATE: optimize first pass
					if( *s < '0' || *s > '9' )
						return oError( "not well-formed XML" ) ;
					x += (byte)( *s - '0' ) ;	// UPDATE: test for overflow
					} while( *(++s) ) ;
			if( x >= 0xFFFE )
				return oError( "not well-formed XML" ) ;
			return aUnicodedEntity_atomize( x ) ;
			}
		if( ! ( u = aUnicodedString_decanonicalize( ((atom)a->datum)->datum ) ) )
				return oError( "not well-formed XML" ) ;
		return aUnicodedString_atomize( u ) ;
		}
	return 0 ;
	}


void aUnicode_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*)) ;
//	extern atom		oDestructor(atom,void*) ;
//	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	extern void		aUnicodedEntity_constructent(void) ;
	extern void		aUnicodedString_constructent(void) ;
	extern void		aAsciiCharacter_constructent(void) ;
	extern void		aAsciiString_constructent(void) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern atom		aXML_translatomize_a_p(atom,atom(*)(atom)) ;
	atom			a ;

#	undef __
#	define __(_1)							\
		{							\
		extern atom	aUnicode$##_1(atom,int,atom*) ;	\
		atomethodize( aUnicode , #_1 , aUnicode$##_1 ) ;	\
		}
	__( entity ) ;
	__( string ) ;

	aUnicodedEntity_constructent() ;
	aUnicodedString_constructent() ;
	aAsciiCharacter_constructent() ;
	aAsciiString_constructent() ;

	a	= aXML_translatomize_a_p( aAsciiString_atomize( string_duplicate( "unicode" ) ) , aUnicoded_translatomize ) ;
	interatomize( a , aUnicoded ) ;
	a	= aXML_translatomize_a_p( aAsciiString_atomize( string_duplicate( "ascii" )  ) , aAscii_translatomize ) ;
	interatomize( a , aAscii ) ;
	}


void aUnicode_destructor( void )
	{
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern void		destruct(atom) ;

	destruct( aAsciiString ) ;
	destruct( aAsciiCharacter ) ;
	destruct( aAscii ) ; 
	destruct( aUnicodedString ) ;
	destruct( aUnicodedEntity ) ;
	destruct( aUnicode ) ;
	destruct( aUnicoded ) ;
	}