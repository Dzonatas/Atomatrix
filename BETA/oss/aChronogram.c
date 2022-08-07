// aChronogram.c

#include "ATOMATRIX.h"
#include "unicode.h"

atom				aChronogram ;			// datum = base 36 format +year:day:milliseconds for Earth daytime or planetarySymbol+year:day:milliseconds

atom aChronogram_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;

	return aChronogram = atomize( atomine ) ;		// UPDATE: aChronological should be precursor
	}


atom aCronogram_atomize_utc( int Y , byte MM , byte DD , byte HH , byte MN , byte SS , unsigned MS )
	{
	extern void*		allocm(unsigned) ;
	atom			a ;
	unsigned		x ;
	string			p ;
	unsigned		y , d , m ;
	unsigned		y0 , d0 , m0 ;
	unsigned		c[12] = { 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ;
	string			q = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
	unsigned		l ;
	

	if( Y < 0 )
		_asm int 3 ;				// UPDATE: handle negative dates

	a	= atomize( aChronogram ) ;
	m	= MS + ( ( HH - 1 ) * 3600 + MN * 60 + SS ) * 1000 ;

	d	= ( DD - 1 ) ;
	for( l = 1 ; l < MM ; l++ )
		d += c[l-1] ;
	if( MM > 2 && ( ( Y % 4 ) == 0 && ((( Y % 100 ) != 0 ) || ( ( Y % 400 ) == 0 ) ) ) )
		d++ ;

	y = 0 ;
	x = Y ;
	y0	= 0 ;
	do	y0++ ;
		while( x /= 36 ) ;
	d0	= 0 ;
	x	= d ;
	do	d0++ ;
		while( x /= 36 ) ;
	m0	= 0 ;
	x	= m ;
	do	m0++ ;
		while( x /= 36 ) ;
	a->datum= p = allocm( y0 + d0 + m0 + 4 ) ;	// "+YYYY:DDD:MMM"	, base 36 , 3 extra + 1 nullity
	*p	= '+' ;
	x	= Y ;
	p	+= y0 ;
	do	*(p--) = q[ x % 36 ]  ;
		while( x /= 36 ) ;
	*(p += y0 + 1 ) = ':' ;
	x	= d ;
	p	+= d0 ;
	do	*(p--) = q[ x % 36 ] ;
		while( x /= 36 ) ;
	*(p += d0 + 1) = ':' ;
	x	= m ;
	p	+= m0 ;
	do	*(p--) = q[ x % 36 ] ;
		while( x /= 36 ) ;
	*(p += m0 + 1) = 0 ;
	return	a ;
	}


atom aChronogram$simplify( atom A , int T , atom* V )
	{
	extern atom		atomine ;
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		(*addAtom)(atom,string,atom) ;
	extern string		string_duplicate(string) ;
	extern atom		aAsciiString_atomize(string) ;
	atom			a ;
	string			p ;
	unsigned		x ;
	unsigned		l ;
	unsigned		m ;
	unsigned		c[2][12] =
		{
		{ 31 , 28 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } ,
		{ 31 , 29 , 31 , 30 , 31 , 30 , 31 , 31 , 30 , 31 , 30 , 31 } 
		};

	if( A->precursor != aChronogram )
		{
		extern atom aError_INVOBJ ;
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	p	= A->datum ;


	if( *(p++) != '+' ) _asm int 3 ;		// UPDATE: handle negative dates
	a	= atomize( atomine ) ;
	addAtom( a , "yearOfPlanet" , aAsciiString_atomize( string_duplicate( "Earth" ) ) ) ;
	x	= 0 ;
	do	{
		x *= 36 ;
		x += *p < 'A' ? *p - '0' : *p - 'A' + 10 ;
		} while( *(++p) != ':' ) ;
	addAtom( a , "year" , aInteger_atomize( x ) ) ;
	l = ( ( x % 4 ) == 0 && ((( x % 100 ) != 0 ) || ( ( x % 400 ) == 0 ) ) ) ? 1 : 0 ;
	p++ ;
	x	= 0 ;
	do	{
		x *= 36 ;
		x += *p < 'A' ? *p - '0' : *p - 'A' + 10 ;
		} while( *(++p) != ':' ) ;
	addAtom( a , "dayOfYear" , aInteger_atomize( x ) ) ;
	m	= 0 ;
	while( m < 12 && c[l][m] < x )
		x -= c[l][m++] ;
	addAtom( a , "day" , aInteger_atomize( x + 1 ) ) ;
	addAtom( a , "month" , aInteger_atomize( m + 1 ) ) ;
	p++ ;
	x	= 0 ;
	do	{
		x *= 36 ;
		x += *p < 'A' ? *p - '0' : *p - 'A' + 10 ;
		} while( *(++p) ) ;
	addAtom( a , "millisecondsOfDay" , aInteger_atomize( x ) ) ;
	addAtom( a , "milliseconds" , aInteger_atomize( x % 1000 ) ) ;
	x	/= 1000 ;
	addAtom( a , "seconds" , aInteger_atomize( x % 60 ) ) ;
	x	/= 60 ;
	addAtom( a , "minutes" , aInteger_atomize( x % 60 ) ) ;
	x	/= 60 ;
	addAtom( a , "afternoon" , aInteger_atomize( x > 11 ? 1 : 0 ) ) ;
	addAtom( a , "hour" , aInteger_atomize( x > 11 ? ( x - 11 ) : ( x + 1 ) ) ) ;
	addAtom( a , "hours" , aInteger_atomize( x + 1 ) ) ;
	return a ;
        }


atom aChronogram$unicode( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString_atomize(unicodes) ;
	unicodes		u ;
	unsigned		l ;
	string			p ;
	atom			a ;

	if( A->precursor != aChronogram )
		{
		extern atom aError_INVOBJ ;
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	l	= 0 ;
	p	= A->datum ;
	while( *(p++) ) l++ ;
	u = ( a = aUnicodedString_atomize( allocm( ( l + 2 ) * sizeof(unicode) ) ) )->datum ;
	*u	= 9793 ;	// Earth symbol : circle with plus on top
	p	= A->datum ;
	while( *(++u) = (unicode)*(p++) ) ;
	return a ;
        }


atom aChronogram_translatomize( atom X )
	{
	extern atom		aXML_aAttribute ;
	extern atom		(*oError)(string) ;
	extern string		string_duplicate(string) ;
//	extern void		dealloc(void*) ;
//	extern unicodes		aUnicodedString_decanonicalize(string) ;
        atom			a ;
	string			s ;
//	unicodes		u ;
	
	a	= X->interatom ;
	a	= a->interatoms ;
	if( a->precursor == aXML_aAttribute )
		s	= a->interatom->interatoms->datum ;
	else
		s	= ((atom)a->datum)->datum ;
//	u	= aUnicodedString_decanonicalize( s ) ;
//	if( ! u )
//		{
//		dealloc( u ) ;
//		return oError( "not well-formed XML" ) ;
//		}
	a	= atomize( aChronogram ) ;
	a->datum= string_duplicate( s ) ;
	return a ;
	}


void aChronogram_destruct_a( atom A )
	{
	extern void		dealloc(void*) ;

	if( A->datum ) 
		dealloc( A->datum ) ;
	A->datum = 0 ;
	}


void aChronogram_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*)) ;
	extern atom		oDestructor(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	extern atom		aXML_translatomize_a_p(atom,atom(*)(atom)) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	atom			a ;

#	undef __
#	define __(_1)								\
		{								\
		extern atom	aChronogram$##_1(atom,int,atom*) ;		\
		atomethodize( aChronogram , #_1 , aChronogram$##_1 ) ;	\
		}
	__( simplify ) ;
	__( unicode ) ;
	oDestructor( aChronogram , aChronogram_destruct_a ) ;
	a	= aXML_translatomize_a_p( aAsciiString_atomize( string_duplicate( "chronogram" ) ) , aChronogram_translatomize ) ;
	interatomize( a , aChronogram ) ;
	}


void aChronogram_destructor( void )
	{
	extern void		destruct(atom) ;

	destruct( aChronogram ) ;
	}