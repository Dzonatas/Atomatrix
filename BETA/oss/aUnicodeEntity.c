// aUnicodeEntity.c		Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "unicode.h"

atom		aUnicodedEntity ;


atom aUnicodedEntity_atomize( unicode C )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	( a = atomize( aUnicodedEntity ) )->datum = (void*) C ;
	return a ;
	}


atom aUnicodedEntity_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		aUnicoded ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aUnicodedEntity = atomize( aUnicoded ) ;
	oFunction( aUnicodedEntity , "atomize"  , aUnicodedEntity_atomize ) ;
	return aUnicodedEntity ;
	}


atom aUnicodedEntity_duplicatent( atom A )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	a		= atomize( aUnicodedEntity ) ;
	a->datum	= A->datum ;
	return a ;
	}


atom aUnicodedEntity_pAddEquate( atom A , atom B )
	{
	extern atom		aInteger ;
	extern void		dealloc(void*) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aError_RANGE ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern void		intratomize(atom,atom) ;
	extern void*		allocm(unsigned) ;

	if( B->precursor == aInteger )
		{
		if( -65536 < (int)B->datum && (int)B->datum < 65536 )
			{
			unsigned c = (unsigned)A->datum + (int)B->datum ;
			if( 0 <= c && c < 65534 )
				{
				(unicode)A->datum = (unicode)c;
				return A ;			// UPDATE: allusion++ ?
				}
			}
		aError_RANGE->allusions++ ;
		return aError_RANGE ;
		}
	if( B->precursor == aUnicodedEntity )
		if( ! (unicode)A->datum )
			{
			(unicode)A->datum = (unicode)B->datum ;
			return A ;
			}
		else
		if( (unicode)B->datum )
			{
			unicodes s ;
			intratomize( A , aUnicodedString ) ;
			s = A->datum = allocm( sizeof(unicode) * 3 ) ;
			*s	= (unicode)A->datum ;
			*(s+1)	= (unicode)B->datum ;
			*(s+2)	= 0 ;
			return A ;
			}
		else
			return A ;
	if( B->precursor == aAsciiCharacter )			// UPDATE: intratomize to aAsciiCharacter
		if( ! (unicode)A->datum )
			{
			(unicode)A->datum = (byte)B->datum ;
			return A ;
			}
		else
		if( (unicode)B->datum )
			{
			unicodes s ;
			intratomize( A , aUnicodedString ) ;
			s = A->datum = allocm( sizeof(unicode) * 3 ) ;
			*s	= (unicode)A->datum ;
			*(s+1)	= (byte)B->datum ;
			*(s+2)	= 0 ;
			return A ;
			}
		else
			return A ;				
								// UPDATE: B->precursor == aAsciiString
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aUnicodedEntity_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( A->datum )				// UPDATE: test for precursor aUnicodedEntity
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aUnicodedEntity_pCompareEqual( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aEquality ;
	extern atom		aUndefined ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;

	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)B->datum == (unicode)A->datum )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( (unicode)B->datum == (byte)A->datum )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedString )
		{
		if( *((unicodes)B->datum) == (unicode)A->datum && ( ( ! *((unicodes)B->datum) ) || ! ((unicodes)B->datum)[1] )  )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	if( B->precursor == aAsciiString )
		{
		if( *((string)B->datum) == (unicode)A->datum && ( ( ! *((string)B->datum) ) || ! ((string)B->datum)[1] )  )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aUnicodedEntity_destructent( atom A )
	{
	A->datum	= 0 ;
	}



atom aUnicodedEntity$string( atom A , int T , atom* V )
	{
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern atom		aError_INVOBJ ;

	if( A->precursor != aUnicodedEntity )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}

	if( (unsigned)((unicode)A->datum) < 128 )			// UPDATE: unicoded strings
		{
		char	c[2] ;
		c[0]	= (byte)((unicode)A->datum) ;
		c[1]	= 0 ;
		return aAsciiString_atomize( string_duplicate( c ) ) ;
		}
	else	{
		char		c[64] ;
		unsigned	i ;
		string		p ;
		c[0]	= '&' ;
		c[1]	= '#' ;
		i	= (unsigned)A->datum ;
		p	= c+3 ;
		while( i )
			{
			i /= 10 ;
			p++ ;
			}
		*(p--)	= 0 ;
		*(p--)	= ';' ;
		i	= (unsigned)A->datum ;
		while( i )
			{
			*(p--) = '0' + ( i % 10 ) ;
			i /= 10 ;
			}
		return aAsciiString_atomize( string_duplicate( c ) ) ;
		}
	}


atom aUnicodedEntity$code( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;

	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}
	if( (unsigned)V[0]->datum >= 0xFFFE )
		{
		aError_RANGE->allusions++;
		return aError_RANGE ;
		}
	return aUnicodedEntity_atomize( (short)V[0]->datum ) ;		// UPDATE: return integer value
	}


atom aUnicodedEntity$uppercase( atom A , int T , atom* V )
	{								// UPDATE: verify T
	extern atom	aAsciiCharacter_atomize(byte) ;
	if( (unicode)A->datum >= 'a' && (unicode)A->datum <= 'z' )
		return aAsciiCharacter_atomize( (byte)( (unicode)A->datum & 0xFFDF ) ) ;
	A->allusions++ ;
	return A ;
	}


atom aUnicodedEntity$lowercase( atom A , int T , atom* V )
	{
	extern atom	aAsciiCharacter_atomize(byte) ;
	if( (unicode)A->datum >= 'A' && (unicode)A->datum <= 'Z' )
		return aAsciiCharacter_atomize( (byte)( (unicode)A->datum | 0x0020 ) ) ;
	A->allusions++ ;
	return A ;
	}


static atom aUnicodedEntity$integer( atom A , int T , atom* V )
	{
	extern atom	aInteger_atomize(unsigned) ;
	if( (unicode)A->datum >= '0' || (unicode)A->datum <= '9' )
		return aInteger_atomize( (unicode)A->datum - L'0' ) ;
	return aInteger_atomize( 0 ) ;					// UPDATE: aUndefined unless V[0] is aInteger value 0
	}


static atom aUnicodedEntity$index( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$index(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes		s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$index( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aUnicodedEntity$endex( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$endex(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes			s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$endex( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aUnicodedEntity$join( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$join(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes		s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$join( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aUnicodedEntity$length( atom A , int T , atom* V )
	{
	if( (byte)A->datum )
		return aInteger_atomize( 1 ) ;
	return aInteger_atomize( 0 ) ;					// UPDATE: aUndefined unless V[0] is aInteger value 0
	}


static atom aUnicodedEntity$replace( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$replace(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes		s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$replace( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aUnicodedEntity$substring( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$substring(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes		s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$substring( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


static atom aUnicodedEntity$split( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString$split(atom,int,atom*) ;
	extern atom		destruct(atom) ;
	extern void*		allocz(unsigned) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	atom			a ;
	atom			x ;
	unicodes		s ;
								// UPDATE: optimize
	s	= allocz( 4 ) ;
	*s	= (unicode)A->datum ;
	x	= aUnicodedString_atomize( s ) ;
	a	= aUnicodedString$split( x , T , V ) ;
	destruct( x ) ;
	return a ;
	}


atom aUnicodedEntity_pAdd( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aAsciiString_atomize(string);
	extern atom		aUnicodedString_atomize(unicodes) ;
	extern unicodes		add_u_u(unicodes,unicodes) ;
	extern unicodes		unicode_s( string ) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern void *		allocz(unisigned) ;

	if( B->precursor == aAsciiString )						// UPDATE: optimize
		{
		unicode		a[2] ;
		unicodes	s ;
		atom		z ;
		if( ! *(string)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		s	= unicode_s( B->datum ) ;
		if( ! (unicode)A->datum )
			return aUnicodedString_atomize( s ) ;
		a[0]	= (unicode)A->datum ;
		a[1]	= 0 ;
		z	= aUnicodedString_atomize( add_u_u( a , s ) ) ;
		dealloc( s ) ;
		return z ;
		}
	if( B->precursor == aAsciiCharacter )						// UPDATE: optimize
		{
		unicodes	s ;
		if( ! (byte)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! (unicode)A->datum )
			{
			B->allusions++ ;
			return B ;
			}
		s	= allocz( 6 ) ;
		s[0]	= (unicode)A->datum ;
		s[1]	= (unicode)((byte)B->datum) ;
		return aUnicodedString_atomize( s ) ;
		}
	if( B->precursor == aUnicodedString )						// UPDATE: optimize
		{
//		extern unicodes add_u_u(unicodes,unicodes) ;
		unicode		s[2] ;
		s[0]	= (unicode)A->datum ;
		s[1]	= 0 ;
		return aUnicodedString_atomize( add_u_u( s , B->datum ) ) ;
		}
	if( B->precursor == aUnicodedEntity )					// UPDATE: optimize
		{
//		extern unicodes add_u_u(unicodes,unicodes) ;
		extern void dealloc(void*) ;
		unicodes	s ;
		unicode		b[2] ;

		if( ! *(unicodes)B->datum )
			{
			A->allusions++ ;
			return A ;
			}
		if( ! (unicode)A->datum )
			{
			B->allusions++ ;
			return B ;
			}
		s	= allocz( sizeof(unicode) * 3 ) ;
		s[0]	= (unicode)A->datum ;
		b[1]	= (unicode)B->datum ;
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



static atom aUnicodedEntity_pIndex( atom A , atom B )
	{
	extern atom		aInteger ;
	extern atom		aError_RANGE ;
	extern atom		aUnicodedString_atomize(unicodes);
	extern unsigned		unicodes_length(unicodes) ;
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
static atom aUnicodedEntity_p##_1_( atom A , atom B )					\
	{										\
	extern atom		aUnicodedString_p##_1_(atom,atom) ;			\
	extern atom		destruct(atom) ;					\
	extern void*		allocz(unsigned) ;					\
	extern atom		aUnicodedString_atomize(unicoded) ;			\
	atom			a ;							\
	atom			x ;							\
	unicodes		s ;							\
											\
	s	= allocz( 4 ) ;								\
	*s	= (unicode)A->datum ;							\
	a	= aUnicodedString_p##_1_( x = aUnicodedString_atomize( s ) , B ) ;	\
	destruct( x ) ;									\
	return a ;									\
	}

__( Splice ) ;
__( Iterize ) ;
__( GreaterThan ) ;
__( GreaterOrEqual ) ;
__( LessThan ) ;
__( LessOrEqual ) ;


void aUnicodedEntity_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	atom			_a ;

	oDestructor( aUnicodedEntity , aUnicodedEntity_destructent ) ;
	oDuplicator( aUnicodedEntity , aUnicodedEntity_duplicatent ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aUnicodedEntity$##_1(atom,int,atom*) ;	\
		atomethodize( aUnicodedEntity , #_1 , aUnicodedEntity$##_1 ) ;	\
		}
	__( code ) ;
	__( string ) ;
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
		(_a=atomize( aOprand##_1 ))->datum=aUnicodedEntity_p##_1 ; \
		interatomize( _a , aUnicodedEntity ) ;	\
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