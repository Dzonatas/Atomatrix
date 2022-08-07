// aAsciiCharacter.c		Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "unicode.h"

atom		aAsciiCharacter ;


atom aAsciiCharacter_atomize( byte C )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	( a = atomize( aAsciiCharacter ) )->datum	= (void*) C ;
	return a ;
	}


atom aAsciiCharacter_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		aAscii ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aAsciiCharacter = atomize( aAscii ) ;
	oFunction( aAsciiCharacter , "atomize"  , aAsciiCharacter_atomize ) ;
	return aAsciiCharacter ;
	}




atom aAsciiCharacter_duplicatent( atom A )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	a		= atomize( aAsciiCharacter ) ;
	a->datum	= A->datum ;
	return a ;
	}


atom aAsciiCharacter_pAddEquate( atom A , atom B )
	{
	extern atom		aInteger ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern void		dealloc(void*) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern void*		allocz(unsigned) ;
	extern void		intratomize(atom,atom) ;
	extern string		string_duplicate(string) ;
	extern string		add_s(string,string) ;
	extern unicodes		add_u_u(unicodes,unicodes) ;
	extern unicodes		unicodes_duplicate(unicodes) ;

	if( B->precursor == aInteger )
		{						// UPDATE: move to another procedure
		if( -128 < (int)B->datum && (int)B->datum < 128 )
			{
			unsigned c = (unsigned)A->datum + (int)B->datum ;
			if( 0 <= c && c < 128 )
				{
				A->datum = (void*)c;
				return A ;
				}
			}
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		string		s ;
		if( ! (byte)B->precursor )
			return A ;
		if( ! (byte)A->precursor )
			{
			(byte)A->datum = (byte)B->datum ;
			return A ;
			}
		s	= allocz( 3 ) ;
		s[0] = (byte)A->datum ;
		s[1] = (byte)B->datum ;
		intratomize( A , aAsciiString ) ;
		A->datum = s ;
		return A ;
		}
	else
	if( B->precursor == aAsciiString )
		{
		byte		a[2] ;
		if( ! *((string)B->datum) )
			return A ;
		if( ! ( a[0] = (byte)A->datum ) )
			{
			if( ! ((string)B->datum)[1] )
				{
				(byte)A->datum = *(string)B->datum ;
				return A ;
				}
			intratomize( A , aAsciiString ) ;
			A->datum = string_duplicate( (string)B->datum ) ;
			return A ;
			}
		a[1] = 0 ;
		intratomize( A , aAsciiString ) ;
		A->datum = add_s( a , (string)B->datum ) ;
		return A ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		string		s ;
		if( ! (unicode)B->datum )
			return A ;
		if( ! (byte)A->datum )
			{
			if( (unicode)B->datum > 127 )
				{
				intratomize( A , aUnicodedEntity ) ;
				(unicode)A->datum = (unicode)B->datum ;
				return A ;
				}
			(byte)A->datum = (byte)((unicode)B->datum) ;
			return A ;
			}
		if( (unicode)B->datum > 127 )
			{
			unicodes s ;
			s	= allocz( sizeof(unicode) * 3 ) ;
			s[0]	= (unicode)((byte)A->datum) ;
			s[1]	= (unicode)B->datum ;
			intratomize( A , aUnicodedString ) ;
			A->datum = s ;
			return A ;
			}
		s	= allocz( 3 ) ;
		s[0]	= (byte)A->datum ;
		s[1]	= (byte)((unicode)B->datum) ;
		A->datum = s ;
		return A ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		unicode		s[2] ;
		if( ! *((unicodes)B->datum) )
			return A ;
		if( ! (byte)A->datum )
			{
			intratomize( A , aUnicodedString ) ;
			A->datum = unicodes_duplicate( B->datum ) ;
			return A ;
			}
		s[0]	= (unicode)((byte)A->datum) ;
		s[1]	= 0 ;
		intratomize( A , aUnicodedString ) ;
		A->datum = add_u_u( s , B->datum ) ;
		return A ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aAsciiCharacter_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( A->datum )				// UPDATE: test for precursor aAsciiCharacter
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aAsciiCharacter_pCompareEqual( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aEquality ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiCharacter )
		{
		if( (byte)B->datum == (byte)A->datum )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aAsciiString )
		{
		if( *((string)B->datum) == (byte)A->datum && ( ( ! *((string)B->datum) ) || ! ((string)B->datum)[1] )  )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)B->datum == (unicode)((byte)A->datum) )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedString )
		{
		if( *((unicodes)B->datum) == (unicode)((byte)A->datum) && ( ( ! *((unicodes)B->datum) ) || ! ((unicodes)B->datum)[1] )  )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aAsciiCharacter_destructent( atom A )
	{
	A->datum	= 0 ;
	}


atom aAsciiCharacter$code( atom A , int T , atom* V )
	{
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		aError_INVARG ;

	if( T )	{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}
	return aInteger_atomize( (unsigned)A->datum ) ;
	}


atom aAsciiCharacter$uppercase( atom A , int T , atom* V )
	{
	if( (byte)A->datum >= 'a' && (byte)A->datum <= 'z' )
		return aAsciiCharacter_atomize( (byte)A->datum & 0xDF ) ;
	A->allusions++ ;
	return A ;
	}


atom aAsciiCharacter$lowercase( atom A , int T , atom* V )
	{
	if( (byte)A->datum >= 'A' && (byte)A->datum <= 'Z' )
		return aAsciiCharacter_atomize( (byte)A->datum | 0x20 ) ;
	A->allusions++ ;
	return A ;
	}


static atom aAsciiCharacter$integer( atom A , int T , atom* V )
	{
	if( (byte)A->datum >= '0' || (byte)A->datum <= '9' )
		return aInteger_atomize( (byte)A->datum - '0' ) ;
	return aInteger_atomize( 0 ) ;					// UPDATE: aUndefined unless V[0] is aInteger value 0
	}


static atom aAsciiCharacter$index( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$index(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$index( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aAsciiCharacter$endex( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$endex(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$endex( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aAsciiCharacter$join( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$join(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$join( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aAsciiCharacter$length( atom A , int T , atom* V )
	{
	if( (byte)A->datum )
		return aInteger_atomize( 1 ) ;
	return aInteger_atomize( 0 ) ;					// UPDATE: aUndefined unless V[0] is aInteger value 0
	}


static atom aAsciiCharacter$replace( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$replace(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$replace( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aAsciiCharacter$substring( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$substring(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$substring( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}

static atom aAsciiCharacter$split( atom A , int T , atom* V )
	{
	atom			a ;
	atom			x ;
	string			s ;
	extern atom		aAsciiString$split(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aAsciiString_atomize(string) ;
								// UPDATE: optimize
	s	= allocz( 2 ) ;
	*s	= (byte)A->datum ;
	x	= aAsciiString_atomize( s ) ;
	a	= aAsciiString$split( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


atom aAsciiCharacter_pAdd( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiString_atomize(string);
	extern atom		aUnicodedString_atomize(unicodes) ;
	extern string		add_s(string,string) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aAsciiString )						// UPDATE: optimize
		{
		byte		a[2] ;
		if( ! *(string)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! (byte)A->datum )
			{
			B->allusions++ ;
			return B ;
			}
		a[0]	= (byte)A->datum ;
		a[1]	= 0 ;
		return aAsciiString_atomize( add_s( a , B->datum ) ) ;
		}
	if( B->precursor == aAsciiCharacter )						// UPDATE: optimize
		{
		string		s ;
		if( ! (byte)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! (byte)A->datum )
			{
			B->allusions++ ;
			return B ;
			}
		s	= allocz( 3 ) ;
		s[0]	= (byte)A->datum ;
		s[1]	= (byte)B->datum ;
		return aAsciiString_atomize( s ) ;
		}
	if( B->precursor == aUnicodedString )						// UPDATE: optimize
		{
		extern unicodes add_u_u(unicodes,unicodes) ;
		unicode		s[2] ;
		s[0]	= (unicode)((byte)A->datum) ;
		s[1]	= 0 ;
		return aUnicodedString_atomize( add_u_u( s , B->datum ) ) ;
		}
	if( B->precursor == aUnicodedEntity )					// UPDATE: optimize
		{
		extern unicodes add_u_u(unicodes,unicodes) ;
		extern void dealloc(void*) ;
		unicodes	s ;

		if( ! (unicode)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! (byte)A->datum )
			{
			B->allusions++ ;
			return B ;
			}
		s	= allocz( sizeof(unicode) * 3 ) ;
		s[0]	= (unicode)((byte)A->datum) ;
		s[1]	= (unicode)B->datum ;
		return  aUnicodedString_atomize( s ) ;
		}
	if( B == aUndefined )
		{
		A->allusions++ ;
		return A ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}



static atom aAsciiCharacter_pIndex( atom A , atom B )
	{
	extern atom		aInteger ;
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string);
	extern unsigned		string_length(string) ;
	extern void*		allocz(unsigned) ;

	if( B->precursor == aInteger && ! (unsigned)B->datum )
		{						// UPDATE: aError_INVARG ?
		A->allusions++ ;
		return A ;
		}
	aError_RANGE->allusions++ ;
	return aError_RANGE ;
	}


#undef __ 
#define __(_1_)										\
static atom aAsciiCharacter_p##_1_( atom A , atom B )					\
	{										\
	extern atom		aAsciiString_p##_1_(atom,atom) ;			\
	extern atom		destruct(atom) ;					\
	extern void*		allocz(unsigned) ;					\
	extern atom		aAsciiString_atomize(unicoded) ;			\
	atom			a ;							\
	atom			x ;							\
	string			s ;							\
											\
	s	= allocz( 2 ) ;								\
	*s	= (byte)A->datum ;							\
	a	= aAsciiString_p##_1_( x = aAsciiString_atomize( s ) , B ) ;		\
	destruct( x ) ;									\
	return a ;									\
	}

__( Splice ) ;
__( Iterize ) ;
__( GreaterThan ) ;
__( GreaterOrEqual ) ;
__( LessThan ) ;
__( LessOrEqual ) ;


void aAsciiCharacter_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	atom			_a ;

	oDestructor( aAsciiCharacter , aAsciiCharacter_destructent ) ;
	oDuplicator( aAsciiCharacter , aAsciiCharacter_duplicatent ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aAsciiCharacter$##_1(atom,int,atom*) ;	\
		atomethodize( aAsciiCharacter , #_1 , aAsciiCharacter$##_1 ) ;	\
		}
	__( code ) ;
	__( integer ) ;
	__( uppercase ) ;
	__( lowercase ) ;
	__( index ) ;
	__( endex ) ;
	__( length ) ;
	__( split ) ;
	__( replace ) ;
	__( substring ) ;

#	undef __
#	define __(_1)			\
		{			\
		extern atom	aOprand##_1 ;	\
		(_a=atomize( aOprand##_1 ))->datum=aAsciiCharacter_p##_1 ; \
		interatomize( _a , aAsciiCharacter ) ;	\
		}
	__( Add ) ;
	__( AddEquate ) ;
	__( Condition ) ;
	__( CompareEqual ) ;
	__( Index ) ;
	__( Splice ) ;
	__( Iterize ) ;
	__( GreaterThan ) ;
	__( GreaterOrEqual ) ;
	__( LessThan ) ;
	__( LessOrEqual ) ;

	}