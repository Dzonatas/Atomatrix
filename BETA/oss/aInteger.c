// aInteger.c		Copyright (c) 2004 BALLARD


#include "ATOMATRIX.h"


atom			aInteger ;


atom aInteger_atomize( unsigned N )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	( a = atomize( aInteger ) )->datum = (void*) N ;
	return a ;
	}


atom aInteger_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aInteger = atomize( atomine ) ;
	oFunction( aInteger ,	"atomize" , aInteger_atomize ) ;
	return aInteger ;
	}


atom aInteger_duplicator( atom A ) 
	{
	atom			a ;

	( a = atomize( aInteger ) )->datum = A->datum ;
	return a ;
	}


atom aInteger_pAdd( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( A->precursor == aInteger )
		{
		if( B->precursor == aInteger )
			return aInteger_atomize( (int)A->datum + (int)B->datum ) ;
		else
		if( B == aUndefined )
			{
			A->allusions++ ;
			return A ;
			}
		}
	else
	if( A == aUndefined )
		{
		B->allusions++ ;
		return B ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pDivide( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		if( B->datum == (void*)1 )
			{
			A->allusions++ ;
			return A ;
			}
		else
		if( B->datum )
			return aInteger_atomize( (int) A->datum / (int) B->datum ) ;
		else	{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
	else
	if( B == aUndefined )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pAddEquate( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		{
		((int)A->datum)	+= (int)B->datum ;
		return A ;
		}
	if( B == aUndefined )
		return A ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pMinusEquate( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		{
		((int)A->datum)	-= (int)B->datum ;
		return A ;
		}
	if( B == aUndefined )
		return A ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pCompareEqual( atom A , atom B )
	{
	extern atom		aEquality ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		{
		if( (int)A->datum == (int)B->datum )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aInteger_pCondition( atom A )
	{
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( (int)A->datum )
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aInteger_pBinaryAnd( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned int)A->datum & (unsigned int)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pBinaryOr( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned int)A->datum | (unsigned int)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aInteger_pGreaterThan( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned)A->datum > (unsigned)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}

atom aInteger_pLessThan( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned)A->datum < (unsigned)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}

atom aInteger_pGreaterOrEqual( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned)A->datum >= (unsigned)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}

atom aInteger_pLessOrEqual( atom A , atom B )
	{
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		return aInteger_atomize( (unsigned)A->datum <= (unsigned)B->datum ) ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}

atom aInteger_pBinaryNegation( atom A )
	{
	return aInteger_atomize( ~(int)A->datum ) ;
	}



atom aInteger$string( atom A , unsigned T , atom* V )
	{
	extern atom		aError_INVOBJ ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	char			n[20] ;			// UPDATE: dynamical
	char*			p ;
	int			r ;
	unsigned		z ;

	if( A->precursor != aInteger )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}

	z	= 0 ;

	p	= n ;

	if( ( r = (int) A->datum ) < 0 )
		r = -r , *(p++) = '-' ;
	while( r /= 10 )
		z++ ;

	*( 1 + ( p += z ) ) = 0 ;
	if( ( r = (int) A->datum ) < 0 )
		r = -r ;
	while( z-- )
		{
		*(p--) = '0' + r % 10 ;
		r /= 10 ;
		} ;
	*p	= '0' + r ;
	if( (int) A->datum < 0 )
		*--p = '-' ;

	return aAsciiString_atomize( string_duplicate( n ) ) ;
	}


atom aInteger$integer( atom A , unsigned T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_INVOBJ ;

	if( A->precursor != aInteger )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	A->allusions++ ;
	return A ;
	}


atom aInteger_translatomize( atom X )
	{
	extern atom		aXML_aAttribute ;
	extern atom		(*oError)(string) ;
	extern string		string_duplicate(string) ;
	extern void*		allocm(unsigned) ;
	extern atom		aXML_element_s(string) ;
	extern atom		aXML_attribute_a_s_a(atom,string,atom) ;
	extern atom		aAsciiString_atomize(string) ;
        atom			a ;
	string			s ;
	unsigned		x = 0 ;

	if( X->precursor == aInteger )
		{
		string	p ;
		unsigned r = (unsigned)X->datum ;
		unsigned z = 1 ;
		a = aXML_element_s( "integer" ) ;
		while( r /= 10 )
			z++ ;
		r = (unsigned) X->datum ;
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
	a	= a->interatoms ;
	if( a->precursor == aXML_aAttribute )
		s	= a->interatom->interatoms->datum ;
	else
		s	= ((atom)a->datum)->datum ;
	if( *s )
		do	{
			x *= 10 ;			// UPDATE: optimize first pass
			if( *s < '0' || *s > '9' )
				return oError( "not well-formed XML" ) ;
			x += (byte)( *s - '0' ) ;	// UPDATE: test for overflow
			} while( *(++s) ) ;
	( a = atomize( aInteger ) )->datum= (void*) x ;
	return a ;
	}


void aInteger_constructent( void )
	{
	extern atom		oDuplicator(atom,void*) ;
	extern void		atomethodize(atom,string,atom(*)(atom,unsigned,atom*)) ;
	extern void		interatomize(atom,atom) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern atom		aXML_translatomize_a_p(atom,atom(*)(atom)) ;
	atom			a ;

	oDuplicator( aInteger , aInteger_duplicator ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aInteger$##_1(atom,unsigned,atom*) ;	\
		atomethodize( aInteger , #_1 , aInteger$##_1 ) ;	\
		}
	__( integer ) ;
	__( string ) ;
#	undef __
#	define __(_1)	\
		{ \
		extern atom	aOprand##_1 ; \
		(a=atomize( aOprand##_1 ))->datum=aInteger_p##_1 ; \
		interatomize( a , aInteger ) ; \
		}
	__( Add ) ;
	__( Divide ) ;
	__( Condition ) ;
	__( CompareEqual ) ;
	__( AddEquate ) ;
	__( MinusEquate ) ;
	__( BinaryAnd ) ;
	__( BinaryOr ) ;
	__( BinaryNegation ) ;
	__( GreaterThan ) ;
	__( LessThan ) ;
	__( GreaterOrEqual ) ;
	__( LessOrEqual ) ;

	a	= aXML_translatomize_a_p( aAsciiString_atomize( string_duplicate( "integer" ) ) , aInteger_translatomize ) ;
	interatomize( a , aInteger ) ;
	}


void aInteger_destructor( void )
	{
	extern void		destruct(atom) ;
	destruct( aInteger ) ;
	}