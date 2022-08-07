/***************************************
*
* aUnicodeCharacter.c
*
*******************/


#include "ATOMATRIX.h"

atom		aUnicodeCharacter ;


void aUnicodeCharacter_preconstructent( atom A )
	{
	extern atom		atomine ;
	extern atom		aAtomatrix ;
	extern atom		aUnicoded ;
	extern atom		oAtomical(atom,string,atom) ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;
	extern atom		aUnicodeCharacter_atomize(short) ;

#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( aUnicoded ) )
	__( aUnicodeCharacter ) ;
	oFunction( A , "aUnicodeCharacter_atomize"  , aUnicodeCharacter_atomize ) ;
	}


atom aUnicodeCharacter_duplicatent( atom A )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	a		= atomize( aUnicodeCharacter ) ;
	a->datum	= A->datum ;
	return a ;
	}


atom aUnicodeCharacter_pAddEquate( atom A , atom B )
	{
	extern atom		aInteger ;
	extern void		dealloc(void*) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aInteger )
		{
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
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aUnicodeCharacter_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( A->datum )				// UPDATE: test for precursor aUnicodeCharacter
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aUnicodeCharacter_pCompareEqual( atom A , atom B )
	{
	extern atom		aString ;
	extern atom		aEquality ;
	extern atom		aUndefined ;

	if( B->precursor == aUnicodeCharacter )
		{
		if( (char)B->datum == (char)A->datum )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aString )
		{
		if( *((string)B->datum) == (char)A->datum && ( ( ! *((string)B->datum) ) || ! ((string)B->datum)[1] )  )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aUnicodeCharacter_atomize( short C )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	a		= atomize( aUnicodeCharacter ) ;
	a->datum	= (void*) C ;
	return a ;
	}


void aUnicodeCharacter_destructent( atom A )
	{
	A->datum	= 0 ;
	}



atom aUnicodeCharacter__string( atom A , int T , atom* V )
	{
	extern atom		aString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern atom		aError_INVOBJ ;

	if( A->precursor != aUnicodeCharacter )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}

	if( (unsigned)A->datum < 128 )
		{
		char	c[2] ;
		c[0]	= (char)A->datum ;
		c[1]	= 0 ;
		return aString_atomize( string_duplicate( c ) ) ;
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
		return aString_atomize( string_duplicate( c ) ) ;
		}
	}


atom aUnicodeCharacter__code( atom A , int T , atom* V )
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
	return aUnicodeCharacter_atomize( (short)V[0]->datum ) ;
	}


atom aUnicodeCharacter__integer( atom A , int T , atom* V )
	{
	extern atom		oInteger(unsigned) ;
	extern atom		aError_INVARG ;

	if( T )	{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}
	return oInteger( (unsigned)A->datum ) ;
	}


void aUnicodeCharacter_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	atom			_a ;

	oDestructor( aUnicodeCharacter , aUnicodeCharacter_destructent ) ;
	oDuplicator( aUnicodeCharacter , aUnicodeCharacter_duplicatent ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aUnicodeCharacter__##_1(atom,int,atom*) ;	\
		atomethodize( aUnicodeCharacter , #_1 , aUnicodeCharacter__##_1 ) ;	\
		}
	__( code ) ;
	__( string ) ;
	__( integer ) ;
//	__( uppercase ) ;
//	__( lowercase ) ;
#	undef __
#	define __(_1)			\
		{			\
		extern atom	aOprand##_1 ;	\
		(_a=atomize( aOprand##_1 ))->datum=aUnicodeCharacter_p##_1 ; \
		interatomize( _a , aUnicodeCharacter ) ;	\
		}
	__( AddEquate ) ;
	__( Condition ) ;
	__( CompareEqual ) ;
	}