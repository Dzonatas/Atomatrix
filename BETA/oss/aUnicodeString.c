// aUnicodeString.c		Copyright (c) 2004 BALLARD

#include "ATOMATRIX.h"
#include "cmp_s.h"
#include "unicode.h"
#include "varray.h"

atom			aUnicodedString ;
atom			aUnicodedStringIterumDatum ;
atom			aUnicodedStringIterinteger ;		// UPDATE: aUnicodedStringIterintegral
atom			aUnicodedStringIterumDatum ;


atom aUnicodedString_atomize( unicodes S )
	{
	extern atom		aUnicodedString ;
	extern atom		atomize(atom) ;
	atom			a ;

	( a = atomize( aUnicodedString ) )->datum	= (void*) S ;
	return a ;
	}


atom aUnicodedString_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		aUnicoded ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aUnicodedString = atomize( aUnicoded ) ;
	oFunction( aUnicodedString , "atomize"  , aUnicodedString_atomize ) ;
	aUnicodedStringIterinteger	= atomize( atomine ) ;
	aUnicodedStringIterumDatum	= atomize( atomine ) ;
	return aUnicodedString ;
	}


unicodes unicode_s( string S )				// UPDATE: temporary helper procedure -- optimize inline
	{
	unicodes		s ;
	unsigned		l ;
	unicodes		a ;
	string			b ;
	extern unsigned		string_length(string) ;
	extern void*		allocm(unsigned) ;

	l	= string_length( b = S ) ;		// UPDATE: "1 to 2" overflow limitation

	a = s	= allocm( sizeof(unicode) + ( l * sizeof(unicode) ) ) ;
	while( l-- )
		*(a++) = *(b++) ;
	*a = 0 ;

	return s ;
	}


unicodes unicodes_duplicate( unicodes A )
	{
	extern void *		allocm(unsigned) ;
	int			l ;
	unicodes		s ;
	unicodes		p ;
	unicodes		r ;

	if( A )
		{
		l = 0 ;
		s = (short*)A ;
		while( *(s++) )
			l++;
		r = p = allocm( ( l + 1 ) * sizeof(unicode) );
		s = A;
		while( *s ) 
			*(p++) = *(s++);
		*p = 0;
		}
	else	{
		r = allocm( sizeof(unicode) );
		*r = 0;
		}

	return r;
	}


static unicodes unicodes_concatenate( unicodes A , unicodes B )
	{
	unicodes		a ;
	unicodes		b ;

	if( A && B )
		{
		a = A;
		b = B;
		while( *a )
			a++;
		while( *b )
			*(a++) = *(b++);
		*a = 0;
		}

	return A;
	}


unicodes add_u_u( unicodes str1 , unicodes str2 )
	{
	extern void *	allocm(unsigned) ;
	unicodes	p ;
	unicodes	s ;
	unsigned	size		= 1;

	if( str1 )
		{
		s	= str1;
		while( *(s++) )
			size++;
		}
	else
		str1 = L"";

	if( str2 )
		{
		s	= str2;
		while( *(s++) )
			size++;
		}
	else
		str2 = L"";

	p = (unicodes) allocm( size * sizeof(unicode) );

	s	= str1;
	while( *s )
		*p++ = *s++;

	s	= str2;
	while( *s )
		*p++ = *s++;

	*p++	= 0;

	return p-size ;
	}



unsigned unicodes_length( unicodes A )
	{
	unicodes		a ;
	unsigned		l ;

	l = 0 ;

	if( A )
		{
		a = A ;
		while( *(a++) )
			l++ ;
		}

	return l;
	}


atom aUnicodedString_duplicatent( atom A )
	{
	extern atom		atomize(atom) ;
	extern unicodes		unicodes_duplicate(unicodes) ;
	atom			a ;

	a		= atomize( aUnicodedString ) ;
	a->datum	= unicodes_duplicate( A->datum ) ;
	return a ;
	}


atom aUnicodedString_pAdd( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aUnicodedString_atomize(unicodes);
	extern unicodes		add_u_u(unicodes,unicodes) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern void		dealloc(void*) ;

	if( A->precursor == aUnicodedString )
		{
		if( B->precursor == aUnicodedString )
			return aUnicodedString_atomize( add_u_u( A->datum , B->datum ) ) ;
		else
		if( B->precursor == aUnicodedEntity )
			{
			unicode b[2] ;
			b[1] = 0 ;
			b[0] = (unicode)B->datum ;
			return aUnicodedString_atomize( add_u_u( A->datum , b ) ) ;
			}
		else
		if( B->precursor == aAsciiCharacter )
			{
			unicode b[2] ;
			b[1] = 0 ;
			b[0] = (byte)B->datum ;
			return aUnicodedString_atomize( add_u_u( A->datum , b ) ) ;
			}
		else
		if( B->precursor == aAsciiString )
			{
			unicodes s ;
			atom	a = aUnicodedString_atomize( add_u_u( A->datum , s = unicode_s( B->datum ) ) ) ;
			dealloc( s ) ;
			return a ;
			}
		else
		if( B == aUndefined )
			{
			A->allusions++ ;
			return A ;
			}
		}
	else
	if( A == aUndefined )		// UPDATE: should not be aUndefined
		{
		B->allusions++ ;
		return B ;
		}
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aUnicodedString_pAddEquate( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern unicodes		add_u_u(unicodes,unicodes) ;
	extern void		dealloc(void*) ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;

	if( B->precursor == aUnicodedString )
		{
		unicodes	s ;
		A->datum = add_u_u( s = A->datum , B->datum ) ;
		dealloc( s ) ;
		return A ;
		}
	else
	if( B->precursor == aUnicodedEntity )			// UPDATE: make character end with nullity or include add_u_u code with optimization
		{
		unicode		b[2] ;
		unicodes	s ;
		b[1] = 0 ;
		b[0] = (unicode)B->datum ;
		A->datum = add_u_u( s = A->datum , b ) ;
		dealloc( s ) ;
		return A ;
		}
	else
	if( B->precursor == aAsciiCharacter )			// UPDATE: make character end with nullity ...
		{
		unicode		b[2] ;
		unicodes	s ;
		b[1] = 0 ;
		b[0] = (byte)B->datum ;
		A->datum = add_u_u( s = A->datum , b ) ;
		dealloc( s ) ;
		return A ;
		}
	else
	if( B->precursor == aAsciiString )
		{
		unicodes	s ;
		unicodes	b ;
		A->datum = add_u_u( s = A->datum , b = unicode_s( B->datum ) ) ;
		dealloc( s ) ;
		dealloc( b ) ;
		return A ;
		}
	else
	if( B == aUndefined )
		return A ;
	aError_TYPE->allusions++ ;
	return aError_TYPE ;
	}


atom aUnicodedString_pCondition( atom A )
	{
	extern atom		aUndefined ;

	if( *((unicodes)A->datum) )
		return A ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aUnicodedString_pCompareEqual( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aEquality ;
	extern atom		aUndefined ;

	if( B->precursor == aUnicodedString )
		{
		unicodes	a = A->datum ;
		unicodes	b = B->datum ;
		while( *a && *b && *a == *b )
			a++ , b++ ;
		if( *a == *b )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aAsciiString )
		{
		unicodes	a = A->datum ;
		string		b = B->datum ;
		while( *a && *b && *a < 128 && *a == *b )
			a++ , b++ ;
		if( *a < 128 && *a == *b )
			{
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		unicodes	a = A->datum ;
		unicode		b = (unicode)B->datum ;
		if( *a == b )
			{
			if( b && a[1] )
				{
				aUndefined->allusions++ ;
				return aUndefined ;
				}
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		unicodes	a = A->datum ;
		byte		b = (byte)B->datum ;
		if( *a == b )
			{
			if( b && a[1] )
				{
				aUndefined->allusions++ ;
				return aUndefined ;
				}
			aEquality->allusions++ ;
			return aEquality ;
			}
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


atom aUnicodedString_pIndex( atom A , atom B )
	{
	extern atom		aError_RANGE ;
	extern atom		aUnicodedEntity_atomize(unicode);
	extern unsigned		unicodes_length(unicodes) ;
	extern void*		allocz(unsigned) ;
	int			i ;
	int			l ;

	l	= unicodes_length( A->datum ) ;
	i	= (int)B->datum ;
	if( i >= 0 && i < l )
		return	aUnicodedEntity_atomize( ((unicodes)A->datum)[i] ) ;
	aError_RANGE->allusions++ ;
	return aError_RANGE ;
	}


atom aUnicodedString_pSplice( atom A , atom B , atom C )
	{
	extern unsigned		unicodes_length(unicodes) ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	extern unicodes		unicodes_duplicate(unicodes) ;
	extern void*		allocm(unsigned) ;
	extern void*		allocz(unsigned) ;
	unicodes		a ;
	int			b ;
	int			c ;
	unicodes		p ;
	int			i , l , max , maxf , maxt ;

	a	= A->datum ;
	b	= (int)B->datum ;
	c	= (int)C->datum ;
	l	= unicodes_length( a ) ;
	if( l == 0 )
		return aUnicodedString_atomize( allocz( sizeof(unicode) ) ) ;
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
			return aUnicodedString_atomize( allocz( sizeof(unicode) ) ) ;
		if( c >= l )
			c = l - 1;
		i = c - b;
		l = 0;
		p = (unicodes) allocm( ( i + 2 ) * sizeof(unicode) );
		while( b <= c )
			*(p+l++) = *(a+b++) ;
		}
	else	{
		if( c >= l )
			return aUnicodedString_atomize( allocz( sizeof(unicode) ) ) ;
		if( b >= l )
			b=l-1;
		i = b - c;
		l = 0;
		p = (unicodes) allocm( ( i + 2 ) * sizeof(unicode) );
		while( b >= c )
			*(p+l++) = *(a+b--) ;
		}
	p[l]		= 0;
	return aUnicodedString_atomize( p ) ;
	}


atom aUnicodedString_pIterize( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aIterum ;
	extern atom		aUnicodedStringIterumDatum ;
	extern atom		aUnicodedStringIterinteger ;
	extern atom		aUnicodedEntity_atomize(unicode) ;
	extern atom		aOprandIterate ;
	extern atom		oDestructor(atom,void*) ;
	extern unicodes		unicodes_duplicate(unicodes) ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	extern void		aUnicodedStringIterum_destructent(atom) ;
	extern atom		aUnicodedStringIterum_pIterate(atom,atom) ;
	atom			z ;
	atom			y ;
	atom			x ;

	if( *((unicodes)A->datum) )
		{
		B->datum = aUnicodedEntity_atomize( *(unicodes)A->datum ) ;
		z	= atomize( aIterum ) ;
		oDestructor( z , aUnicodedStringIterum_destructent ) ;
		( x = atomize( aUnicodedStringIterumDatum ) )->datum = B ;
		interatomize( x , z ) ;
		interatomize( atomize( aUnicodedStringIterinteger ) , z ) ;
		( y = atomize( aOprandIterate ) )->datum = aUnicodedStringIterum_pIterate ;
		interatomize( y , z ) ;
		((atom)( z->datum = A ))->allusions++ ;
		return z ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aUnicodedString_destructent( atom A )
	{
	extern void		dealloc(void*) ;

	if( A->datum )
		dealloc( A->datum ) ;
	A->datum	= 0 ;
	}



atom aUnicodedString$string( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
	extern atom		aError_INVOBJ ;

	if( A->precursor != aUnicodedString )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}
	A->allusions++ ;
	return A ;
	}



string aUnicodedString_canonicalize( unicodes S )
	{
	extern atom		aAsciiString ;
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned) ;
	extern void*		allocr(void*,unsigned) ;
	string			s ;
	string			p ;
	string			pp ;
	unicodes		a ;
	unsigned		c ;
	unsigned		z ;
	
	c	= 0 ;
	a	= S ;
	while( *a++ )
		c++ ;
	p = s	= allocm( c + 1 ) ;
	a	= S ;
	while( *a )
		if( *a < 128 && *a != L'&' && ( *a >= L' ' || *a == L'\n' || *a == L'\t' ) )
			*(s++)	= (byte)((unicode)*(a++)) ;
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


atom aUnicodedString$canonicalize( atom A , int T , atom* V )
	{
	extern atom		aError_INVARG ;
	extern atom		aAsciiString_atomize(string) ;
	
	if( T || A->precursor != aUnicodedString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}
	return aAsciiString_atomize( aUnicodedString_canonicalize( A->datum ) ) ;
	}


unicodes aUnicodedString_decanonicalize( string S )
	{
	extern void*		allocm(unsigned) ;
	extern void*		allocr(void*,unsigned) ;
	string			s ;
	unsigned		c ;
	unicodes		u ;
	unicodes		p ;
	unsigned		x ;
	
	c	= 0 ;
	s	= S ;
	while( *s++ )
		c++ ;
	s	= S ;
	p = u	= allocm( ( c + 1 ) * sizeof(unicode) ) ;
	c	= 0 ;
	while( *s )
		if( *s != '&' )
			c++ , *(u++) = (unicode) *(s++) ;
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
			if( x >= 0xFFFE )
				{
				dealloc( p ) ;
				return 0 ; 
				}
			*(u++)	= (unicode) x ;
			s++ ;
			}
	*u	= 0 ;
	return allocr( p , ( c + 1 ) * sizeof(unicode) ) ;
	}


atom aUnicodedString$decanonicalize( atom A , int T , atom* V )
	{
	extern atom		aError_INVARG ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedString_atomize(unicodes) ;
	unicodes		u ;
	
	if( T != 1 || A != aUnicodedString || V[0]->precursor != aAsciiString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	u	= aUnicodedString_decanonicalize( V[0]->datum ) ;
	if( ! u )
		{
		aError_INVARG->allusions++;
		return aError_INVARG ;
		}
	return aUnicodedString_atomize( u ) ;
	}


atom aUnicodedStringIterum_pIterate( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern void		destruct(atom) ;
	extern unicodes		unicodes_duplicate(unicodes) ;
	extern atom		aUnicodedEntity_atomize(unicode) ;
	unsigned		x ;
	atom			b ;

	if( ((unicodes)((atom)A->datum)->datum )[( x = ++((unsigned int)(b=A->interatom->interatoms)->datum) ) ] )
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
		B->datum = aUnicodedEntity_atomize( ((unicodes)((atom)A->datum)->datum )[x] ) ;
		A->allusions++ ;
		return A ;
		}
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aUnicodedStringIterum_destructent( atom A )
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



/*static*/ atom aUnicodedString$index( atom A , int T , atom* V )
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

	if( A->precursor != aUnicodedString )
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
		unicodes	p ;
		string		z ;
		unicodes	a = A->datum ;
		string		b = V[0]->datum ;
		if( ( ! b ) || ( ! a ) || ( ! *a ) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		p	= a ;
		z	= b ;
		while( *z )
			if( (unicode)*z++ == *p++ )
				continue ;
			else	{
				z	= b ;
				if( *( p = ++a ) )
					while( *p && (unicode)*z != *p )
						p++ ;
				if( ! *( a = p ) )
					break ;
				}
		if( ! *z )
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		unicode		b ;
		unicodes	a ;
		b	= (unicode)V[0]->datum ;
		a	= A->datum ;
		while( *a && *a != b )
			a++ ;
		if( *a == b )
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		unicode		b ;
		unicodes	a ;
		b	= (unicode)((byte)V[0]->datum) ;
		a	= A->datum ;
		while( *a && *a != b )
			a++ ;
		if( *a == b )
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		unicodes	p ;
		unicodes	z ;
		unicodes	a = A->datum ;
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
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}

	aError_INVARG->allusions++;
	return aError_INVARG;
	}






static atom aUnicodedString$length( atom A , int T , atom* V )
	{
	extern atom		aError_ARGC ;
	extern atom		aInteger_atomize(unsigned) ;
	unsigned		l ;
	unicodes		s ;

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





/*static*/ atom aUnicodedString$endex( atom A , int T , atom* V )
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

	if( A->precursor != aUnicodedString )
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
		unicodes	astr = A->datum ;
		if( (! mstr) || (!astr) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		m	= 0;			// UPDATE: optimize
		a	= unicodes_length( astr )-1 ;
		while( a >= 0 && mstr[m] )
			{
			if(astr[a+m] && astr[a+m] == mstr[m])
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
		unicode		c = (unicode)((byte)V[0]->datum) ;
		unicodes	a = A->datum ;
		if( ! c )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		while( *a )
			a++ ;
		while( a >= (unicodes)A->datum && c != *a )
			a-- ;
		if( c == *a && a >= (unicodes)A->datum )
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		unsigned	m ;
		int		a ;
		unicodes	mstr = V[0]->datum ;
		unicodes	astr = A->datum ;
		if( (! mstr) || (!astr) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		m	= 0;			// UPDATE: optimize
		a	= unicodes_length( astr )-1 ;
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
		unicode		c ;
		unicodes	a = A->datum ;
		if( ! ( c = (unicode)V[0]->datum) )
			{
			aUndefined->allusions++ ;
			return aUndefined ;
			}
		while( *a )
			a++ ;
		while( a >= (unicodes)A->datum && c != *a )
			a-- ;
		if( c == *a && a >= (unicodes)A->datum )
			return oIndexInteger( (unsigned)( a - (unicodes)A->datum ) / 2 ) ;
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	aError_INVARG->allusions++;
	return aError_INVARG;
	}



static atom aUnicodedString$lowercase( atom A , int T , atom* V )
	{
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern unsigned int	string_length(string) ;
	unicodes		s;
	int			i;
	int			len;

	if( ! ( T == 0 || T == 1 ) )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aUnicodedString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	len	= unicodes_length( A->datum ) ;
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
	s	= unicodes_duplicate( A->datum ) ;
	for( i = 0 ; i < len ; i++ )
		if( s[i] >= L'A' && s[i] <= L'Z' )
			s[i] |= 0x0020 ;
	return aUnicodedString_atomize( s ) ;
	}




atom aUnicodedString$split( atom A , int T , atom* V )
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
	unicodes		sstr;
	unicodes		atstr;
	atom			a ;
	int			x ;

	if( T != 1 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( A->precursor != aUnicodedString )
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
				sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aUnicodedString_atomize( sstr ) ;
				lm		= atchr+m;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aUnicodedString_atomize( sstr ) ;
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
				sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aUnicodedString_atomize( sstr ) ;
				lm		= atchr+m;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aUnicodedString_atomize( sstr ) ;
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
				sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aUnicodedString_atomize( sstr ) ;
				if( c )
					lm		= atchr + 1 ;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aUnicodedString_atomize( sstr ) ;
			}
		return a ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		unicode	c ;
		atstr	= A->datum ;


		lm	= 0;
		atchr	= 0;

		a	= aVarray_atomize_a_n( 0 , 0 ) ;
		x	= 0 ;

		c	= (unicode)V[0]->datum ;

		while( atstr[atchr] )
			{
			if( atstr[atchr] == c || ! c )
//			m=0;
//			while( mstr[m] && atstr[atchr+m] && mstr[m]==atstr[atchr+m] )
//				m++;
//			if( m == mlen )
				{
				sstr		= allocm( ( (atchr-lm)+1 ) * 2 );
				schr		= 0;
				while( lm < atchr )
					sstr[schr++]=atstr[lm++];
				sstr[schr]	= 0;
				aVarray_atomize_a_n( a , x+1 ) ;
				v = ((varray)a->datum)->ary + x++ ;
				*v		= aUnicodedString_atomize( sstr ) ;
				if( c )
					lm		= atchr + 1 ;
				}
			atchr++;
			}
		if(atchr!=lm)
			{
			sstr		= allocm( ( (atchr-lm)+1 )  * 2 );
			schr		= 0;
			while( lm < atchr )
				sstr[schr++] = atstr[lm++];
			sstr[schr]	= 0;
			aVarray_atomize_a_n( a , x+1 ) ;
			v = ((varray)a->datum)->ary + x++ ;
			*v		= aUnicodedString_atomize( sstr ) ;
			}
		return a ;
		}
	aError_INVARG->allusions++ ;
	return aError_INVARG ;
	}


atom aUnicodedString$substring( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
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
	unicodes		s ;

	if( T < 1 || 2 < T )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aUnicodedString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	if( V[0]->precursor != aInteger )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	l	= unicodes_length( A->datum ) ;
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
	s	= allocm( ( ( t - f < 0 ? f - t : t - f ) + 1 ) * 2 ) ;
	c	= 0 ;
	if( f < t )
		do	if( f < l )
				s[c++] = (((unicodes)A->datum)[f]) ;
			while( ++f < t ) ;
	*(s+c)	= 0 ;
	return aUnicodedString_atomize( s ) ;
	}



atom aUnicodedString$uppercase( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_RANGE ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		string_duplicate(string) ;
	extern unsigned int	string_length(string) ;
	unicodes		s;
	int			i;
	int			len;

	if( ! ( T == 0 || T == 1 ) )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( A->precursor != aUnicodedString )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	len	= unicodes_length( A->datum ) ;
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
	s	= unicodes_duplicate( A->datum ) ;
	for( i = 0 ; i < len ; i++ )
		if( s[i] >= L'a' && s[i] <= L'z' )
			s[i] &= 0xFFDF ;
	return aUnicodedString_atomize( s );
	}



atom aUnicodedString$replace( atom A , int T , atom* V )
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
	unicode *str=0,*p,*m,*s;
	unicodes	u ;

	if( T != 2 )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( A->precursor != aUnicodedString )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[0]->precursor == aAsciiString )
		m = unicode_s( (string)V[0]->datum ) ;
	else
	if( V[0]->precursor == aUnicodedString )
		m = V[0]->datum ;
	else
	if( V[0]->precursor == aAsciiCharacter )
		{
		m = allocz( 4 ) ;
		*m = (unicode)((byte)V[0]->datum) ;
		}
	else
	if( V[0]->precursor == aUnicodedEntity )
		{
		m = allocz( 4 ) ;
		*m = (unicode)V[0]->datum ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( V[1]->precursor == aAsciiString )
		u = unicode_s( V[1]->datum ) ;
	else
	if( V[1]->precursor == aUnicodedString )
		u = V[1]->datum ; 
	else
	if( V[1]->precursor == aAsciiCharacter )
		{
		u = allocz( 4 ) ;
		*u = (unicode)((byte)V[0]->datum) ;
		}
	else
	if( V[1]->precursor == aUnicodedEntity )
		{
		u = allocz( 4 ) ;
		*u = (unicode)V[1]->datum ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	y	= 0;
	p	= A->datum ;
	w	= 0;
	len	= unicodes_length(p);
	nlen	= unicodes_length(V[1]->datum);
	for( l = 0 ; l < len ; l++ )
		{
		c=l;
		for( x = 0 ; p[c] == m[x] ; x++ , c++ )
			{
			if(m[x+1]==0)
				{
				if( str )
					str	= allocr( str , ( w+(l-y)+nlen+1 ) * 2 );
				else
					str	= allocm( ( w+(l-y)+nlen+1 ) * 2 );
				
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
			str = allocr( str, ( w+(len-y)+1 ) * 2 );
		else
			str = allocm( ( w+(len-y)+1 ) * 2 );
		while( y < len )
			str[w++] = p[y++];
		}

	if( ! str )
		str	= allocm( 2 );

	str[w]=0;

	if( V[0]->precursor != aUnicodedString )
		dealloc( m ) ;
	if( V[1]->precursor != aUnicodedString )
		dealloc( u ) ;

	return aUnicodedString_atomize( str );
	}


atom aUnicodedString$join( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
//	extern atom		aAsciiString_atomize(string) ;
	extern void*		allocm(unsigned) ;
	int			t ;
	unsigned		m ;
	unsigned		c ;
	unicodes		s ;
	unicodes		p ;
	unicodes		a ;
	unicodes		z ;
	atom			b ;
	atom*			v ;

	if( A->precursor != aUnicodedString )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ ;
		}
	if( ! T )
		{
		*( s = allocm( 2 ) ) = 0 ;
		return aUnicodedString_atomize( s ) ;		// UPDATE: aAsciiCharacter
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
		if( ( b = *v )->precursor == aUnicodedString )
			{
			p	= b->datum ;
			while( *p++ )
				m++ ;
			}
		v++ ;
		} while( --t ) ;
	s = z	= allocm( ( m + 1 ) * 2 ) ;
	if( ( b = (*(v=V)) )->precursor == aUnicodedString )	// UPDATE: should preverify all elements as aUnicodedString else use aError
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
		if( ( b = (*++v) )->precursor == aUnicodedString )
			{
			p	= b->datum ;
			while( *p )
				*s++ = *p++ ;
			}
		} while( --t ) ;
	*s	= 0 ;
	return aUnicodedString_atomize( z ) ;
	}


/*static*/ atom aUnicodedString_pGreaterThan( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aUndefined ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;

	if( B->precursor == aAsciiString )
		{
		unicodes	a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a <= (unicode)*b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == (unicode)*b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a <= (unicode)*b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(unicodes)A->datum < (unicode)((byte)B->datum) )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( *(unicodes)A->datum < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		unicodes	a = A->datum ;
		unicodes	b = B->datum ;
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
	aUndefined->allusions++ ;
	return aUndefined ;
	}


/*static*/ atom aUnicodedString_pLessThan( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;

	if( B->precursor == aAsciiString )
		{
		unicodes	a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a >= (unicode)*b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == (unicode)*b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a >= (unicode)*b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(unicodes)A->datum < (unicode)((byte)B->datum) )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( *(unicodes)A->datum < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		unicodes	a = A->datum ;
		unicodes	b = B->datum ;
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
	aUndefined->allusions++ ;
	return aUndefined ;
	}

/*static*/ atom aUnicodedString_pGreaterOrEqual( atom A , atom B )
	{
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiString ;
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiString )
		{
		unicodes	a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a < (unicode)*b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == (unicode)*b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a < (unicode)*b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(unicodes)A->datum > (unicode)((byte)B->datum) )
			return aInteger_atomize( 1 ) ;
		if( *(unicodes)A->datum == (unicode)((byte)B->datum) )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((unicodes)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( *(unicodes)A->datum > (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		if( *(unicodes)A->datum == (unicode)B->datum )
			if( ! (unicode)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((unicodes)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		unicodes	a = A->datum ;
		unicodes	b = B->datum ;
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
	aUndefined->allusions++ ;
	return aUndefined ;
	}



/*static*/ atom aUnicodedString_pLessOrEqual( atom A , atom B )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aAsciiCharacter ;
	extern atom		aUnicodedEntity ;
	extern atom		aInteger_atomize(unsigned) ;
	extern atom		aUndefined ;

	if( B->precursor == aAsciiString )
		{
		unicodes	a = A->datum ;
		string		b = B->datum ;
		while( *a && *b )
			{
			if( *a > (unicode)*b )
				return aInteger_atomize( 0 ) ;
			a++ , b++ ;
			}
		if( *a == (unicode)*b )
			{
			return aInteger_atomize( 1 ) ;
			}
		if( *a > (unicode)*b )
			return aInteger_atomize( 0 ) ;
		return aInteger_atomize( 1 ) ;				// UPDATE: return aInequality ;
		}
	else
	if( B->precursor == aAsciiCharacter )
		{
		if( *(unicodes)A->datum < (unicode)((byte)B->datum) )
			return aInteger_atomize( 1 ) ;
		if( *(unicodes)A->datum == (unicode)((byte)B->datum) )
			if( ! (byte)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((unicodes)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedEntity )
		{
		if( *(unicodes)A->datum < (unicode)B->datum )
			return aInteger_atomize( 1 ) ;
		if( *(unicodes)A->datum == (unicode)B->datum )
			if( ! (unicode)B->datum )
				return aInteger_atomize( 1 ) ;
			else
			if( ! ((unicodes)A->datum)[1] )
				return aInteger_atomize( 1 ) ;
		}
	else
	if( B->precursor == aUnicodedString )
		{
		unicodes	a = A->datum ;
		unicodes	b = B->datum ;
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

	aUndefined->allusions++ ;
	return aUndefined ;
	}


void aUnicodedString_constructent( void )
	{
	extern atom		atomize(atom) ;
	extern atom		atomethodize(atom,string,atom(*)(atom,int,atom*));
	extern atom		oDestructor(atom,void*) ;
	extern atom		oDuplicator(atom,void*) ;
	extern void		interatomize(atom,atom) ;
	extern atom		oFunction(atom,string,void*) ;
	atom			_a ;

	oDestructor( aUnicodedString , aUnicodedString_destructent ) ;
	oDuplicator( aUnicodedString , aUnicodedString_duplicatent ) ;
#	undef __
#	define __(_1)							\
		{							\
		extern atom	aUnicodedString$##_1(atom,int,atom*) ;	\
		atomethodize( aUnicodedString , #_1 , aUnicodedString$##_1 ) ;		\
		}
	__( string ) ;
	__( canonicalize ) ;
	__( decanonicalize ) ;
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
		(_a=atomize( aOprand##_1 ))->datum=aUnicodedString_p##_1 ; \
		interatomize( _a , aUnicodedString ) ;	\
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

	oFunction( aUnicodedString , "canonicalize" , aUnicodedString_canonicalize ) ;
	oFunction( aUnicodedString , "decanonicalize" , aUnicodedString_decanonicalize ) ;
	}


#include <stdlib.h>

static atom aUnicodedString$integer( atom A , int T , atom* V )
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
	return aInteger_atomize( _wtoi( A->datum ) ) ;
	}