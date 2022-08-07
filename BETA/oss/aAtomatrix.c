// aAtomatrix.c

#include "ATOMATRIX.h"
#include "unicode.h"


atom aAtomatrix$attach( atom A , unsigned T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiCharacter ;
	extern atom		aDirectionAtomatrix ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern void		dealloc(void*) ;
	extern string 		add_s(string,string) ;
	extern void		construct(atom) ;
	extern atom		oLibrary(string) ;
	extern atom		oError(string) ;
	extern atom		aAsciiString_atomize(string) ;
	extern string		aUnicodedString_canonicalize(unicodes) ;
	extern void		interatomize(atom,atom) ;
	extern void		destruct(atom) ;
	string			s ;
	string			ss ;
	atom			a ;
	atom			b ;

	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	a	= *V ;
	if( a->precursor == aAsciiString )
		{
#ifdef _DEBUG
		ss	= add_s( a->datum , "\\2\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#else
		ss	= add_s( a->datum , "\\1\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#endif
		}
	else
	if( a->precursor != aAsciiCharacter )
		{
		char s[2] ;
		s[1] = (byte)a->datum ;
		s[0] = 0 ;
#ifdef _DEBUG
		ss	= add_s( s , "\\2\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#else
		ss	= add_s( s , "\\1\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#endif
		}
	else
	if( a->precursor != aUnicodedString )
		{
		string s = aUnicodedString_canonicalize( (unicodes)a->datum ) ;
#ifdef _DEBUG
		ss	= add_s( s , "\\2\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#else
		ss	= add_s( s , "\\1\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#endif
		dealloc( s ) ;
		}
	else
	if( a->precursor != aUnicodedEntity )
		{
		unicode		u[2] ;
		string		s ;
		u[0] = (unicode)a->datum ;
		u[1] = 0 ;
		s = aUnicodedString_canonicalize( u ) ;
#ifdef _DEBUG
		ss	= add_s( s , "\\2\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#else
		ss	= add_s( s , "\\1\\" ) ;			// UPDATE: DYNAMIC SUBVERSION
#endif
		dealloc( s ) ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	s	= add_s( ss , V[0]->datum ) ;
	dealloc( ss ) ;
	if( ( a	= oLibrary( s ) )->datum )
		{
		if( a->allusions == 1 )				// UPDATE: thread constructent
			{
			construct( a ) ;
			b = aAsciiString_atomize( s ) ;
			interatomize( b , a ) ;
			}
		else
			dealloc( s ) ;
		return a ;
		}
	if( --a->allusions == 0 )
		destruct( a ) ;
	ss	= add_s( "dynamic library '" , s ) ;
	dealloc( s ) ;
	s	= add_s( ss , "' illogical" ) ;
	dealloc( ss ) ;
	return oError( s ) ;
	}
