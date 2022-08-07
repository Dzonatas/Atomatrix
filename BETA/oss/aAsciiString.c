// aAsciiString.c		Copyright 2004 (c) BALLARD

#include "ATOMATRIX.h"
#include "cmp_s.h"
#include "unicode.h"
#include "varray.h"


atom			aAsciiString ;
atom			aAsciiStringIterumDatum ;
atom			aAsciiStringIterinteger ;		// UPDATE: aAsciiStringIterintegral
atom			aAsciiStringIterumDatum ;


atom aAsciiString_atomize( string s )
	{
	extern atom		atomize(atom) ;
	atom			a ;

	( a = atomize( aAsciiString ) )->datum	= (void*) s ;
	return a ;
	}


atom aAsciiString_pregeneration( void )
	{
	extern atom		aAscii ;
	extern atom		atomine ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aAsciiString	= atomize( aAscii ) ;
	oFunction( aAsciiString , "atomize"  , aAsciiString_atomize ) ;
	aAsciiStringIterinteger	= atomize( atomine ) ;
	aAsciiStringIterumDatum	= atomize( atomine ) ;
	return aAsciiString ;
	}


atom aAsciiString_duplicatent( atom A )
	{
	extern atom		atomize(atom) ;
	extern string		string_duplicate(string) ;
	extern void *		allocm(unsigned) ;
	extern void *		allocz(unsigned) ;
	atom			a ;
	int			l ;
	string			s ;
	string			p ;
	string			r ;

	a	= atomize( aAsciiString ) ;
	l = 1 ;
	if( *( r = s = A->datum ) )
		{
		l++ ;
		while( *(++r) )
			l++;
		}
	else	{
		a->datum = allocz( 1 ) ;
		return a ;
		}
	a->datum = p = allocm( l ) ;
	*p = *s ;
	while( ( *(++p) = *(++s) ) ) ;
	
	return a ;
	}


atom aAsciiString_pAdd( atom A , atom B )
	{
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiString_atomize(string);
	extern atom		aUnicodedString_atomize(unicodes) ;
	extern string		add_s(string,string) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aAsciiString )						// UPDATE: optimize
		return aAsciiString_atomize( add_s( A->datum , B->datum ) ) ;
	if( B->precursor == aAsciiCharacter )						// UPDATE: optimize
		{
		byte		b[2] ;
		b[0]	= (byte)B->datum ;
		b[1]	= 0 ;
		return aAsciiString_atomize( add_s( A->datum , b ) ) ;
		}
	if( B->precursor == aUnicodedString )						// UPDATE: optimize
		{
		extern unicodes unicode_s(string) ;
		extern unicodes add_u_u(unicodes,unicodes) ;
		extern void dealloc(void*) ;
		unicodes	s = unicode_s( A->datum ) ;
		atom		a = aUnicodedString_atomize( add_u_u( s , B->datum ) ) ;
		dealloc( s ) ;
		return a ;
		}
	if( B->precursor == aUnicodedEntity )					// UPDATE: optimize
		{
		extern unicodes unicode_s(string) ;
		extern unicodes add_u_u(unicodes,unicodes) ;
		extern void dealloc(void*) ;
		unicodes	s = unicode_s( A->datum ) ;
		unicode		b[2] ;
		atom		a ;
		b[0]	= (unicode)B->datum ;
		b[1]	= 0 ;
		a =  aUnicodedString_atomize( add_u_u( s , b ) ) ;
		dealloc( s ) ;
		return a ;
		}
	if( B == aUndefined )
		{
		A->allusions++ ;
		return A ;
		}
/*
	if( A == aUndefined )
		{
		B->allusions++ ;
		return B ;
		}
*/
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}

atom aAsciiString_pAddEquate( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern string		add_s(string,string) ;
	extern void		dealloc(void*) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aError_RANGE ;
	extern unsigned		unicodes_length(unicodes) ;
	extern void*		allocm(unsigned) ;

	if( B->precursor == aAsciiString )
		{
		extern void *	allocr(void*,unsigned) ;
		string		s ;
		string		r ;
		unsigned	x = 0 ;
		unsigned	y = 0 ;

		if( *( r = s = B->datum ) )
			{
			x++ ;
			while( *(++s) )
				x++ ;
			}
		if( *( s = A->datum ) )
			{
			y++ ;
			while( *(++s) )
				y++ ;
			}
		s = (string)( A->datum = allocr( A->datum , x+y+1 ) ) + y ;
		if( ( *s = *r ) )
			while( ( *(++s) = *(++r) ) ) ;
		return A ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		string	s ;
		byte	b[2] ;
		b[0] = (byte)B->datum ;
		b[1] = 0 ;
		A->datum = add_s( s = A->datum , b ) ;
		dealloc( s ) ;
		return A ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		string	s ;
		byte	b[2] ;
		if( (unicode)B->datum > 127 )
			{
			extern unicodes unicode_s(string) ;
			extern void	intratomize(atom,atom) ;
			unicodes u = unicode_s( A->datum ) ;
			unicode		c[2] ;
			c[0] = (unicode)B->datum ;
			c[1] = 0 ;
			dealloc( A->datum ) ;
			intratomize( A , aUnicodedString ) ;
			A->datum = add_u_u( u , c ) ;
			dealloc( u ) ;
			return A ;
			}
		b[0] = (byte)((unicode)B->datum) ;
		b[1] = 0 ;
		A->datum = add_s( s = A->datum , b ) ;
		dealloc( s ) ;
		return A ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		extern unicodes unicode_s(string) ;
		extern void	intratomize(atom,atom) ;
		unicodes u = unicode_s( A->datum ) ;
		dealloc( A->datum ) ;
		intratomize( A , aUnicodedString ) ;
		A->datum = add_u_u( u , B->datum ) ;
		dealloc( u ) ;
		return A ;
		}
/*									// previous [non]conversion code
		unicodes	p ;
		unsigned	l = unicodes_length( p = (unicodes)B->datum ) ;
		string		m ;
		string		s = m = allocm( l + 1 ) ;
		while( *p )
			{
			if( *p > 127 )
				{
				dealloc( m ) ;
				aError_RANGE->allusions++ ;		// UPDATE: error RANGE or convert to aUnicodeString
				return aError_RANGE ;
				}
			*(s++) = (byte)*(p++) ;
			}
		*s = 0 ;
		A->datum = add_s( s = A->datum , m ) ;
		dealloc( s ) ;
		dealloc( m ) ;
		return A ;
		}
*/
	else
	if( B == aUndefined )
		{
		return A ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aAsciiString_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( *((string)A->datum) )
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aAsciiString_pCompareEqual( atom A , atom B )
	{
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aEquality ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiString )
		{
		string		a = A->datum ;
		string		b = B->datum ;
		while( *a && *b && *a == *b )
			a++ , b++ ;
		if( *a == *b )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *((string)A->datum) == (byte)B->datum && ( (byte)B->datum && ! ((string)A->datum)[1] ) )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)B->datum > 127 )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		if( *((string)A->datum) == (unicode)B->datum && ( (unicode)B->datum && ! ((string)A->datum)[1] ) )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedString )
		{
		string		a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b && *a == *b )
			a++ , b++ ;
		if( *a == *b )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}

	aUndefined->allusions++ ;
	return aUndefined ;
	}


/*static*/ atom aAsciiString$index( atom A , int T , atom* V )
	{
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oIndexInteger)(int) ;

	if( A->precursor != aAsciiString )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor == aAsciiString )
		{
		string		p ;
		string		z ;
		string		a = A->datum ;
		string		b = V[0]->datum ;
		if( ( ! b ) || ( ! a ) || ( ! *a ) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		p	= a ;
		z	= b ;
		while( *z )
			if( *z++ == *p++ )
				continue ;
			else	{
				z	= b ;
				if( *( p = ++a ) )
					while( *p && *z != *p )
						p++ ;
				if( ! *( a = p ) )
					break ;
				}
		if( ! *z )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		byte		b ;
		string		a ;
		if( (unicode)V[0]->datum > 127 )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		b	= (byte)((unicode)V[0]->datum) ;
		a	= A->datum ;
		while( *a && *a != b )
			a++ ;
		if( *a == b )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		byte		b ;
		string		a ;
		b	= (byte)V[0]->datum ;
		a	= A->datum ;
		while( *a && *a != b )
			a++ ;
		if( *a == b )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		string		p ;
		unicodes	z ;
		string		a = A->datum ;
		unicodes	b = V[0]->datum ;
		if( ( ! b ) || ( ! a ) || ( ! *a ) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		p	= a ;
		z	= b ;
		while( *z )
			if( *z++ == *p++ )
				continue ;
			else	{
				z	= b ;
				if( *( p = ++a ) )
					while( *p && *z != *p )
						p++ ;
				if( ! *( a = p ) )
					break ;
				}
		if( ! *z )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}

	aError_INVARG->allusions++;
	return aError_INVARG;
	}




static atom aAsciiString_pIndex( atom A , atom B )
	{
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string);
	extern unsigned		string_length(string) ;
	extern void*		allocz(unsigned) ;
	int			i ;
	int			l ;

	l	= string_length( A->datum ) ;
	i	= (int)B->datum ;
	if( i >= 0 && i < l )
		{						// UPDATE: aError_INVARG ?
		string	s ;
		s		= allocz( 2 ) ;	
		*s		= ((string)A->datum)[i] ;
		return	aAsciiString_atomize( s ) ;			// UPDATE: aAsciiCharacter_atomize
		}
	aError_RANGE->allusions++ ;
	return aError_RANGE ;
	}


/*static*/ atom aAsciiString_pSplice( atom A , atom B , atom C )
	{
	extern unsigned		string_length(string) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern void*		allocm(unsigned) ;
	string			a ;
	int			b ;
	int			c ;
	string			p ;
	int			i , l , max , maxf , maxt ;

	a	= A->datum ;
	b	= (int)B->datum ;
	c	= (int)C->datum ;
	l	= string_length( a ) ;
	if( l == 0 )
		return aAsciiString_atomize( string_duplicate( "" ) ) ;
#	define MAX(_1_,_2_)	(_1_>_2_?_1_:_2_)
#	define FITRANGE(_1,_2) if( _1 < 0 ) { if( (_1 = _2 + _1) < 0 ) _1 = 0; } 
	maxf	= b<0 ? MAX(-b,l) : MAX(b+1,l);
	maxt	= c<0 ? MAX(-c,l) : MAX(c+1,l);
	max	= MAX(maxf,maxt);
	FITRANGE(b,max); 
	FITRANGE(c,max);
	if( b <= c )
		{
		if( b >= l )
			return aAsciiString_atomize( string_duplicate( "" ) ) ;
		if( c >= l )
			c = l - 1;
		i = c - b;
		l = 0;
		p = (string) allocm( i + 2 );
		while( b <= c )
			*(p+l++) = *(a+b++) ;
		}
	else	{
		if( c >= l )
			return aAsciiString_atomize( string_duplicate( "" ) ) ;
		if( b >= l )
			b=l-1;
		i = b - c;
		l = 0;
		p = (string) allocm( i + 2 );
		while( b >= c )
			*(p+l++) = *(a+b--) ;
		}
	p[l]		= 0;
	return aAsciiString_atomize( p ) ;
	}


/*static*/ atom aAsciiString_pIterize( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aIterum ;
	extern atom		aAsciiStringIterumDatum ;
	extern atom		aAsciiStringIterinteger ;
	extern atom		aAsciiString_atomize(string) ;
	extern atom		aOprandIterate ;
	extern atom		oDestructor(atom,void*) ;
	extern string		string_duplicate(string) ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	extern void		aAsciiStringIterum_destructent(atom) ;
	extern atom		aAsciiStringIterum_pIterate(atom,atom) ;
	atom			z ;
	atom			y ;
	atom			x ;
	char			s[2] ;

	if( *((string)A->datum) )
		{
		s[1] = 0 ;
		s[0] = *(string)A->datum ;
		B->datum = aAsciiString_atomize( string_duplicate( s ) ) ;
		z	= atomize( aIterum ) ;
		oDestructor( z , aAsciiStringIterum_destructent ) ;
		( x = atomize( aAsciiStringIterumDatum ) )->datum = B ;
		interatomize( x , z ) ;
		interatomize( atomize( aAsciiStringIterinteger ) , z ) ;
		( y = atomize( aOprandIterate ) )->datum = aAsciiStringIterum_pIterate ;
		interatomize( y , z ) ;
		((atom)( z->datum = A ))->allusions++ ;
		return z ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aAsciiString_destructent( atom A )
	{
	extern void		dealloc(void*) ;

	if( A->datum )
		dealloc( A->datum ) ;
	A->datum	= 0 ;
	}



atom aAsciiString$string( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_INVOBJ ;

	if( A->precursor != aAsciiString )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}
	A->allusions++ ;
	return A ;
	}


static atom aAsciiStringIterum_pIterate( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern void		destruct(atom) ;
	extern string		string_duplicate(string) ;
	extern atom		aAsciiCharacter_atomize(byte) ;
	unsigned int		x ;
	atom			b ;

	if( ((string)((atom)A->datum)->datum )[( x = ++((unsigned int)(b=A->interatom->interatoms)->datum) ) ] )
		{
		atom	a ;
		b	= b->interatoms->datum ;
#ifdef _DEBUG
		if( (atom)b != B )
			_asm int 3 ;
#endif
		if( ( a = b->datum ) )
			if( --a->allusions == 0 )
				destruct( a ) ;
		B->datum = aAsciiCharacter_atomize( ((string)((atom)A->datum)->datum )[x] ) ;
		A->allusions++ ;
		return A ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aAsciiStringIterum_destructent( atom A )
	{
	extern void		destruct(atom) ;
	atom			a ;
	atom			b ;

	if( ( a = A->datum ) )
		{
		b = A->interatom->interatoms->interatoms->datum ;
		if( --a->allusions == 0 )
			destruct( a ) ;
		if( ( a = b->datum ) )
			{
			if( --a->allusions == 0 )
				destruct( a ) ;
			b->datum = 0 ;
			}
		}
	}



string aAsciiString_canonicalize( string S )
	{
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned) ;
	extern void*		allocr(void*,unsigned) ;
	string			s ;
	string			p ;
	string			pp ;
	string			a ;
	unsigned		c ;
	unsigned		z ;
	
	c	= 0 ;
	a	= S ;
	while( *a++ )
		c++ ;
	p = s	= allocm( c + 1 ) ;
	a	= S ;
	while( *a )
		if( *a < 128 && *a != '&' && *a != '<' && *a != '>' && ( *a >= ' ' || *a == '\n' || *a == '\t' ) )
			*(s++)	= (byte)(*(a++)) ;
		else	{
			unsigned	x ;
			
			pp	= p ;
			z	= 0 ;
			x	= *a ;
			while( x )
				z++ , x /= 10 ;
			s	= ( p = allocr( p , c += z + 3 ) ) + (unsigned)( s - pp ) ;

			*(s++)	= '&' ;
			*(s++)	= '#' ;
			s	+= z ;
			x	= *a ;
			while( x )
				{
				*(--s) = '0' + ( x % 10 ) ;
				x /= 10 ;
				}
			s	+= z ;
			*(s++)	= ';' ;
			a++ ;
			}
	*(s++)	= 0 ;
	return p ;
	}


string aAsciiString_decanonicalize( string S )
	{
	extern void*		allocm(unsigned) ;
	extern void*		allocr(void*,unsigned) ;
	string			s ;
	unsigned		c ;
	string			u ;
	string			p ;
	unsigned		x ;
	
	c	= 0 ;
	s	= S ;
	while( *s++ )
		c++ ;
	s	= S ;
	p = u	= allocm( ( c + 1 ) ) ;
	c	= 0 ;
	while( *s )
		if( *s != '&' )
			c++ , *(u++) = (byte) *(s++) ;
		else	{
			c++ ;
			if( *(++s) != '#' )
				{
				dealloc( p ) ;
				return 0 ; 
				}
			x	= 0 ;
			while( *(++s) != ';' )
				{
				x *= 10 ;			// UPDATE: optimize first pass
				if( *s < '0' || *s > '9' )
					{
					dealloc( p ) ;
					return 0 ;
					}
				x += (byte)( *s - '0' ) ;
				}
			if( x > 127 )
				{
				dealloc( p ) ;
				return 0 ; 
				}
			*(u++)	= (byte) x ;
			s++ ;
			}
	*u	= 0 ;
	return allocr( p , ( c + 1 ) ) ;			// UPDATE: optimize, allocr not needed if no conversion occurred
	}



static atom aAsciiString$length( atom A , int T , atom* V )
	{
	extern atom		aError_ARGC ;
	extern atom		aInteger_atomize(unsigned) ;
	unsigned		l ;
	string			s ;

	if( T )	{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	l	= 0 ;
	if( *( s = A->datum ) )
		do	l++ ;
		while( *++s ) ;
	return aInteger_atomize( l ) ;
        }





/*static*/ atom aAsciiString$endex( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString ;
	extern atom		aUndefined ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		(*oIndexInteger)(int) ;
	extern unsigned		string_length(string) ;
	extern unsigned		unicodes_length(unicodes) ;

	if( A->precursor != aAsciiString )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[0]->precursor == aAsciiString )
		{
		unsigned	m ;
		int		a ;
		string		mstr = V[0]->datum ;
		string		astr = A->datum ;
		if( (! mstr) || (!astr) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		m	= 0;			// UPDATE: optimize
		a	= string_length( astr )-1;
		while( a >= 0 && mstr[m] )
			{
			if(astr[a+m] && astr[a+m]==mstr[m])
				{
				m++;
				continue;
				}
			m=0;
			a--;
			}
		if( m == string_length(mstr) )
			return oIndexInteger( a ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		byte		c = (byte)V[0]->datum ;
		string		a = A->datum ;
		if( ! c )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		while( *a )
			a++ ;
		while( a >= (string)A->datum && c != *a )
			a-- ;
		if( c == *a && a >= (string)A->datum )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		unsigned	m ;
		int		a ;
		unicodes	mstr = V[0]->datum ;
		string		astr = A->datum ;
		if( (! mstr) || (!astr) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		m	= 0;			// UPDATE: optimize
		a	= string_length( astr )-1;
		while( a >= 0 && mstr[m] )
			{
			if(astr[a+m] && astr[a+m]==mstr[m])
				{
				m++;
				continue;
				}
			m=0;
			a--;
			}
		if( m == unicodes_length(mstr) )
			return oIndexInteger( a ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		byte		c ;
		string		a = A->datum ;
		if( (unicode)V[0]->datum > 127 || ! ( c = (byte)((unicode)V[0]->datum)) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		while( *a )
			a++ ;
		while( a >= (string)A->datum && c != *a )
			a-- ;
		if( c == *a && a >= (string)A->datum )
			return oIndexInteger( (unsigned)( a - (string)A->datum ) ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	aError_INVARG->allusions++;
	return aError_INVARG;
	}



static atom aAsciiString$lowercase( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern unsigned int	string_length(string) ;
	string			s;
	int			i;
	int			len;

	if( ! ( T == 0 || T == 1 ) )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	len	= string_length( A->datum ) ;
	if( T )	{
		if( V[0]->precursor == aInteger )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		if( (int) V[0]->datum < 0 )
			{ 
			aError_RANGE->allusions++;
			return aError_RANGE;
			}
		if( (int) V[0]->datum < len )
			len = (int) V[1]->datum ;
		}
	s	= string_duplicate( A->datum ) ;
	for( i = 0 ; i < len ; i++ )
		if( s[i] >= 'A' && s[i] <= 'Z' )
			s[i] |= 0x20 ;
	return aAsciiString_atomize( s ) ;
	}




atom aAsciiString$split( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	extern atom		aVarray_atomize_a_n(atom,int) ;
	extern void*		allocm(unsigned int) ;
	extern unsigned int	string_length(string) ;
	extern unsigned		unicodes_length(unicodes) ;
	atom*			v;
	int			lm;
	int			atchr;
	int			schr;
	string			sstr;
	string			atstr;
	atom			a ;
	int			x ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[0]->precursor == aAsciiString )
		{
		int		m;
		int		mlen;
		string		mstr;
		atstr	= A->datum ;
		mstr	= V[0]->datum ;

		lm	= 0;
		atchr	= 0;
		mlen	= string_length(mstr);

		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		x	= 0 ;

		while( atstr[atchr] )
			{
			m=0;
			while( mstr[m] && atstr[atchr+m] && mstr[m]==atstr[atchr+m] )
				m++;
			if( m == mlen )
				{
				sstr		= allocm( (atchr-lm)+1 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aAsciiString_atomize( sstr ) ;
				lm		= atchr+m;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm((atchr-lm)+1);
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aAsciiString_atomize( sstr ) ;
			}
		return a ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		int		m;
		int		mlen ;
		unicodes	mstr ;
		atstr	= A->datum ;
		mstr	= V[0]->datum ;

		lm	= 0;
		atchr	= 0;
		mlen	= unicodes_length(mstr);

		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		x	= 0 ;

		while( atstr[atchr] )
			{
			m=0;
			while( mstr[m] && atstr[atchr+m] && mstr[m]==atstr[atchr+m] )
				m++;
			if( m == mlen )
				{
				sstr		= allocm( (atchr-lm)+1 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aAsciiString_atomize( sstr ) ;
				lm		= atchr+m;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm((atchr-lm)+1);
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aAsciiString_atomize( sstr ) ;
			}
		return a ;
		}
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		byte	c = (byte)V[0]->datum ;
		atstr	= A->datum ;

		lm	= 0;
		atchr	= 0;

		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		x	= 0 ;

		while( atstr[atchr] )
			{
			if( atstr[atchr] == c || ! c )
//			m=0;
//			while( mstr[m] && atstr[atchr+m] && mstr[m]==atstr[atchr+m] )
//				m++;
//			if( m == mlen )
				{
				sstr		= allocm( (atchr-lm)+1 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aAsciiString_atomize( sstr ) ;
				if( c )
					lm		= atchr + 1 ;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm((atchr-lm)+1);
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aAsciiString_atomize( sstr ) ;
			}
		return a ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		byte	c ;
		atstr	= A->datum ;


		lm	= 0;
		atchr	= 0;

		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		x	= 0 ;

		if( (unicode)V[0]->datum  > 127 )
			return a ;
		c = (byte)((unicode)V[0]->datum) ;

		while( atstr[atchr] )
			{
			if( atstr[atchr] == c || ! c )
//			m=0;
//			while( mstr[m] && atstr[atchr+m] && mstr[m]==atstr[atchr+m] )
//				m++;
//			if( m == mlen )
				{
				sstr		= allocm( (atchr-lm)+1 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aAsciiString_atomize( sstr ) ;
				if( c )
					lm		= atchr + 1 ;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm((atchr-lm)+1);
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aAsciiString_atomize( sstr ) ;
			}
		return a ;
		}
	aError_INVARG->allusions++ ;
	return aError_INVARG ;
	}


atom aAsciiString$substring( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned int) ;
	extern unsigned int	string_length(string) ;
	int			f ;
	int			t ;
	int			l ;
	int			c ;
	string			s ;

	if( T < 1 || 2 < T )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	l	= string_length( A->datum ) ;
	t	= l ;
	f	= (int)V[0]->datum;
	if( f < 0 )
		f = l + f ;
	if( f > l )
		f = l - 1 ;
	if( f < 0 )
		f = 0 ;
	if( T == 2 )
		{
		if( V[1]->precursor != aInteger )
			{
			aError_INVARG->allusions++ ;
			return aError_INVARG ;
			}
		t	= (int)V[1]->datum ;
		if( t < 0 )
			t = l + t ;
		else
			t = f + t ;
		if( t < 0 )
			t = 0 ;
		if( t > l )
			t = l ;
		}
	s	= allocm( ( t - f < 0 ? f - t : t - f ) + 1 ) ;
	c	= 0 ;
	if( f < t )
		do	if( f < l )
				*(s+c++) = *(((string)A->datum)+f) ;
			while( ++f < t ) ;
	*(s+c)	= 0 ;
	return aAsciiString_atomize( s ) ;
	}



atom aAsciiString$uppercase( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern unsigned int	string_length(string) ;
	string			s;
	int			i;
	int			len;

	if( ! ( T == 0 || T == 1 ) )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	len	= string_length( A->datum ) ;
	if( T )	{
		if( V[0]->precursor != aInteger )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		if( (int) V[0]->datum < 0 )
			{
			aError_RANGE->allusions++;
			return aError_RANGE;
			}
		if( (int) V[0]->datum < len )
			len = (int) V[0]->datum ;
		}
	s	= string_duplicate( A->datum ) ;
	for( i = 0 ; i < len ; i++ )
		if( s[i] >= 'a' && s[i] <= 'z' )
			s[i] &= 0xDF ;
	return aAsciiString_atomize( s );
	}



atom aAsciiString$replace( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned int) ;
	extern void*		allocr(void*,unsigned int) ;
	extern unsigned int	string_length(string) ;
	int l,x,c,len,nlen,y,w;
	char *str=0,*p,*m,*s;
	string	u ;

	if( T != 2 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[0]->precursor == aAsciiString )
		m = V[0]->datum ;
	else
	if( V[0]->precursor == aUnicodedString )
		{
		unicodes	p ;
		unsigned	l = unicodes_length( p = (unicodes)V[0]->datum ) ;
		string		s = m = allocm( l + 1 ) ;
		while( *p )
			{
			if( *p > 127 )
				{
				dealloc( m ) ;
				return aAsciiString_atomize( string_duplicate( A->datum ) ) ;
				}
			*(s++) = (byte)*(p++) ;
			}
		*s = 0 ;
		}
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		m = allocz( 2 ) ;
		*m = (byte)V[0]->datum ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		if( (unicode)V[0]->datum > 127 )
			return aAsciiString_atomize( string_duplicate( A->datum ) ) ;
		m = allocz( 2 ) ;
		*m = (byte)((unicode)V[0]->datum) ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[1]->precursor == aAsciiString )
		u = V[1]->datum ;
	else
	if( V[1]->precursor == aUnicodedString )
		{
		unicodes	p ;
		unsigned	l = unicodes_length( p = (unicodes)V[1]->datum ) ;
		string		s = u = allocm( l + 1 ) ;
		while( *p )
			{
			if( *p > 127 )
				{
				dealloc( u ) ;
				if( V[0]->precursor != aAsciiString )
					dealloc( m ) ;
				return aAsciiString_atomize( string_duplicate( A->datum ) ) ;
				}
			*(s++) = (byte)*(p++) ;
			}
		*s = 0 ;
		}
	else
	if( V[1]->precursor == aAsciiCharacter )
		{
		u = allocz( 2 ) ;
		*u = (byte)V[0]->datum ;
		}
	else
	if( V[1]->precursor == aUnicodedEntity )
		{
		if( (unicode)V[1]->datum > 127 )
			{
			if( V[1]->precursor != aAsciiString )
				dealloc( m ) ;
			return aAsciiString_atomize( string_duplicate( A->datum ) ) ;
			}
		u = allocz( 2 ) ;
		*u = (byte)((unicode)V[1]->datum) ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	y	= 0;
	p	= A->datum ;
	w	= 0;
	len	= string_length(p);
	nlen	= string_length(V[1]->datum);
	for( l = 0 ; l < len ; l++ )
		{
		c=l;
		for( x = 0 ; p[c] == m[x] ; x++ , c++ )
			{
			if(m[x+1]==0)
				{
				if( str )
					str	= allocr( str , w+(l-y)+nlen+1 );
				else
					str	= allocm( w+(l-y)+nlen+1 );
				
				while(y<l)
					str[w++]=p[y++];
				s=V[1]->datum;
				if(s)
					while(*s)
						str[w++]=*(s++);
				l=c;
				y=c+1;
				break;
				}
			}
		}

	if( y < len )
		{
		if( str )
			str = allocr( str, w+(len-y)+1 );
		else
			str = allocm( w+(len-y)+1 );
		while( y < len )
			str[w++] = p[y++];
		}

	if( ! str )
		str	= allocm( 1 );

	str[w]=0;

	if( V[0]->precursor != aAsciiString )
		dealloc( m ) ;
	if( V[1]->precursor != aAsciiString )
		dealloc( u ) ;

	return aAsciiString_atomize( str );
	}


atom aAsciiString$join( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned) ;
	int			t ;
	unsigned		m ;
	unsigned		c ;
	string			s ;
	string			p ;
	string			a ;
	string			z ;
	atom			b ;
	atom*			v ;

	if( A->precursor != aAsciiString )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}
	if( ! T )
		{
		*( s = allocm( 1 ) ) = 0 ;
		return aAsciiString_atomize( s ) ;		// UPDATE: aAsciiCharacter
		}
	c	= 0 ;
	a	= A->datum ;
	while( *a++ )
		c++ ;
	m	= 0 ;
	v	= V ;
	if( ( t	= T ) > 1 )
		{
		while( --t )
			m += c ;
		t	= T ;
		}
	do	{
		if( ( b = *v )->precursor == aAsciiString )
			{
			p	= b->datum ;
			while( *p++ )
				m++ ;
			}
		v++ ;
		} while( --t ) ;
	s = z	= allocm( m + 1 ) ;
	if( ( b = (*(v=V)) )->precursor == aAsciiString )	// UPDATE: should preverify all elements as aAsciiString else use aError
		{
		p	= b->datum ;
		while( *p )
			*s++ = *p++ ;
		}
	if( ( t = T-1 ) ) do
		{
		a	= A->datum ;
		while( *a )
			*s++ = *a++ ;
		if( ( b = (*++v) )->precursor == aAsciiString )
			{
			p	= b->datum ;
			while( *p )
				*s++ = *p++ ;
			}
		} while( --t ) ;
	*s	= 0 ;
	return aAsciiString_atomize( z ) ;
	}


/*static*/ atom aAsciiString_pGreaterThan( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aUndefined ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;

	if( B->precursor == aAsciiString )
		{
		string		a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a <= *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a <= *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(string)A->datum < (byte)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)(*(string)A->datum) < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		string		a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b )
			{
			if( (unicode)*a <= *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( (unicode)*a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( (unicode)*a <= *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


/*static*/ atom aAsciiString_pLessThan( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;

	if( B->precursor == aAsciiString )
		{
		string		a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a >= *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a >= *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(string)A->datum < (byte)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)(*(string)A->datum) < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		string		a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b )
			{
			if( (unicode)*a >= *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( (unicode)*a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( (unicode)*a >= *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}

/*static*/ atom aAsciiString_pGreaterOrEqual( atom A , atom B )
	{
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiString )
		{
		string		a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a < *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a < *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(string)A->datum > (byte)B->datum )
			return aInteger_atomize( 1 ) ;
		if( *(string)A->datum == (byte)B->datum )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((string)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)(*(string)A->datum) > (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		if( (unicode)(*(string)A->datum) == (unicode)B->datum )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((string)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		string		a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b )
			{
			if( (unicode)*a < *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( (unicode)*a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( (unicode)*a < *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}



/*static*/ atom aAsciiString_pLessOrEqual( atom A , atom B )
	{
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiString )
		{
		string		a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a > *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a > *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(string)A->datum < (byte)B->datum )
			return aInteger_atomize( 1 ) ;
		if( *(string)A->datum == (byte)B->datum )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((string)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( (unicode)(*(string)A->datum) < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		if( (unicode)(*(string)A->datum) == (unicode)B->datum )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((string)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		string		a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b )
			{
			if( (unicode)*a > *b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( (unicode)*a == *b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( (unicode)*a > *b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}

	aUndefined->allusions++ ;
	return aUndefined ;
	}

void aAsciiString_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	atom			_a ;

	oDestructor( aAsciiString , aAsciiString_destructent ) ;
	oDuplicator( aAsciiString , aAsciiString_duplicatent ) ;
	atomethodize( aAsciiString , "string" , aAsciiString$string ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aAsciiString$##_1(atom,int,atom*) ;	\
		atomethodize( aAsciiString , #_1 , aAsciiString$##_1 ) ;		\
		}
//	__( code ) ;			// implement
	__( integer ) ;
	__( uppercase ) ;
	__( lowercase ) ;
	__( index ) ;
	__( endex ) ;
	__( join ) ;
	__( length ) ;
	__( split ) ;
	__( replace ) ;
	__( substring ) ;
#	undef __
#	define __(_1)			\
		{			\
		extern atom	aOprand##_1 ;	\
		(_a=atomize( aOprand##_1 ))->datum=aAsciiString_p##_1 ; \
		interatomize( _a , aAsciiString ) ;	\
		}
	__( Add ) ;
	__( AddEquate ) ;
	__( Condition ) ;
	__( CompareEqual ) ;
//	__( Equals ) ;
//	__( AddEquals ) ;
//	__( MinusEquals ) ;
	__( Index ) ;
//	__( PopLeft ) ;
//	__( PopRight ) ;
//	__( Pull ) ;
//	__( PushLeft ) ;
//	__( PushRight ) ;
	__( Splice ) ;
//	__( Split ) ;
//	__( Evaluate ) ;
	__( Iterize ) ;
//	__( Iterate ) ;
	__( GreaterThan ) ;
	__( GreaterOrEqual ) ;
	__( LessThan ) ;
	__( LessOrEqual ) ;

	oFunction( aAsciiString , "canonicalize"  , aAsciiString_canonicalize ) ;
	oFunction( aAsciiString , "decanonicalize"  , aAsciiString_decanonicalize ) ;
	}


#include <stdlib.h>

static atom aAsciiString$integer( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aInteger_atomize(unsigned) ;

	if( T )	{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aAsciiString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	return aInteger_atomize( atoi( A->datum ) ) ;
	}