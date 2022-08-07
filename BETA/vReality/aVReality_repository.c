/***************************************
*
* aVReality_repository.c
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"

#ifdef MICROSOFT_win32
# include <windows.h>
# define __	"\\"
#else
# define __	"/"
#endif

#include <stdio.h>

unsigned	editSequence ;
struct	_arg
	{
	string		s ;
	} ;

#define DBMPATH		"db"__"m"
#define DBMPXML		".xml"

string static repositoryPath( void )
	{
	string			s ;
	unsigned		x ;
	string			p ;

#	ifdef MICROSOFT_win32

	extern void*		(*allocm)(unsigned) ;

	x	= GetEnvironmentVariable( "_ATOMATRIX" , 0 , 0 ) ;
	s	= allocm( x + sizeof( DBMPATH ) + 1 ) ;
	GetEnvironmentVariable( "_ATOMATRIX" , s , x ) ;

#	else

	extern string		(*str_dup)(string) ;
	extern unsigned		(*str_len)(string) ;

	s	= str_dup( getenv( "_ATOMATRIX" ) ) ;
	x	= str_len( s ) ;
	s	= allocr( s , x + sizeof( DBMPATH ) + 1 ) ;

#	endif

	p	= s + x - 1 ;
	x	= 0 ;
	while( ( *p = *(DBMPATH+x++) ) ) p++ ;

	return s ;
	}


atom static repositoryIndex( void )
	{
	extern void*		(*allocr)(void*,unsigned) ;
	extern atom		(*aVarray_atomize_a_n)(atom,unsigned) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern atom		(*aVarray_append_a_a)(atom,atom) ;
	extern unsigned		(*str_len)(string) ;
	extern string		(*str_dup)(string) ;
	extern void		(*dealloc)(void*) ;
	unsigned		c ;
	string			p ;
	string			s ;
	atom			a ;

#	ifdef MICROSOFT_win32
	HANDLE			h ;
	WIN32_FIND_DATA		d ;
#	endif

	s	= repositoryPath() ;
	c	= str_len( s ) ;
	s	= allocr( s , c + 100 ) ;		// UPDATE: 100 ?
	p	= s + c ;
	c	= 0 ;
	while( ( *p = *(__"*"DBMPXML+c++) ) ) p++ ;
	
	a	= aVarray_atomize_a_n( 0 , 0 ) ;
	if( ( h = FindFirstFile( s , &d ) ) != INVALID_HANDLE_VALUE )
		{
		do	{
			if( ! ( d.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) )
				{
				c = str_len( d.cFileName ) - sizeof( DBMPXML ) + 1 ;
				d.cFileName[c] = 0 ;
				aVarray_append_a_a( a , aAsciiString_atomize( str_dup( d.cFileName ) ) ) ;
				}
			} while( FindNextFile( h , &d ) ) ;
		FindClose( h ) ;
		}
	dealloc( s ) ;
	return a ;
	}


atom static repositoryRead( string S )
	{
	extern atom		(*aError_pUndefined)(void) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern unsigned		(*str_len)(string) ;
	extern string		(*str_dup)(string) ;
	extern void		(*dealloc)(void*) ;
	unsigned		c ;
	string			p ;
	string			s ;
	HANDLE			f ;

	DWORD			x ;
	DWORD			z ;
	DWORD			e ;

	s	= repositoryPath() ;
	c	= str_len( s ) ;
	s	= allocr( s , c + 100 ) ;		// UPDATE: 100 ?
	p	= s + c ;
	c	= 0 ;
	while( ( *p = *(__+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(S+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(DBMPXML+c++) ) ) p++ ;

	z	= 4096 ;
	f	= CreateFile( s , GENERIC_READ , 0 , 0 , OPEN_EXISTING , 0 , 0 ) ;
	dealloc( s ) ;
	if( f == INVALID_HANDLE_VALUE )
		return aError_pUndefined() ;
	s	= (string)allocm( z ) ;
	p	= s ;
	c	= 0 ;
	do	{
		if( ReadFile( f , p , z , &x , 0 ) )
			{
			if( x == 0 )
				break ;
			c	+= x ;
			s	= (string)allocr( s , c + z ) ;
			p	= s + c ;
			continue ;
			}
		if( ERROR_INSUFFICIENT_BUFFER == ( e = GetLastError() ) )
			{
			z *= 2 ;		// UPDATE: validity of max z
			s	= (string)allocr( s , c + z ) ;
			p	= s + c ;
			continue ;
			}
		else
			break ;
		} while( 1 ) ;
	*(char*)p = 0 ;
	c++ ;
	CloseHandle( f ) ;
	return aAsciiString_atomize( allocr( s , c ) ) ;
	}


atom static repositoryWrite( string S , string D )
	{
	extern atom		(*aError_pUndefined)(void) ;
	extern int		(*str_len)(string) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	extern void		(*dealloc)(void*) ;
	string			s ;
	string			p ;
	DWORD			x ;
	DWORD			z ;
	unsigned		c ;
	HANDLE			f ;

	s	= repositoryPath() ;
	c	= str_len( s ) ;
	s	= allocr( s , c + 100 ) ;		// UPDATE: 100 ?

	if( ( f	= CreateFile( s , 0 , 0 , 0 , OPEN_EXISTING , FILE_FLAG_BACKUP_SEMANTICS , 0 ) ) != INVALID_HANDLE_VALUE )
		CloseHandle( f ) ;
	else
	if( ! CreateDirectory( s , 0 ) )
		{
		dealloc( s ) ;
		return aError_pUndefined() ;
		}

	p	= s + c ;
	c	= 0 ;
	while( ( *p = *(__+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(S+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(DBMPXML+c++) ) ) p++ ;


	z	= 4096 ;
	if( ( f	= CreateFile( s , GENERIC_WRITE , 0 , 0 , CREATE_ALWAYS , 0 , 0 ) ) == INVALID_HANDLE_VALUE )
		{
		dealloc( s ) ;
		return aError_pUndefined() ;
		}
	dealloc( s ) ;
	if( ! WriteFile( f , D  , str_len( D ) , &x , 0 ) )
		{
		CloseHandle( f ) ;
		return aError_pUndefined() ;
		}
	CloseHandle( f ) ;
	return atominul ;
	}


atom aVReality_repository( atom A , int T , atom* V )
	{
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiCharacter ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		(*allocm)(unsigned) ;
	extern void		(*dealloc)(void*) ;
	extern string		(*aUnicodedString_canonicalize)(unicodes) ;
	string			s ;
//	string			t ;
	atom			a ;


	if( ! T )
		return repositoryIndex() ;
	if( (*V)->precursor == aAsciiString )
		s = (string)(*V)->datum ;
	else
	if( (*V)->precursor == aUnicodedString )
		s = aUnicodedString_canonicalize( (unicodes)(*V)->datum ) ;
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( T == 1 )
		{
		a	= repositoryRead( (string)(*V)->datum ) ;
		if( (*V)->precursor != aAsciiString )
			dealloc( s ) ;
		return a ;
		}
	if( T != 2 )
		{
		if( (*V)->precursor != aAsciiString )
			dealloc( s ) ;
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	if( V[1]->precursor == aAsciiString )
		s = (string)V[1]->datum ;
	else
	if( (*V)->precursor == aUnicodedString )
		s = aUnicodedString_canonicalize( (unicodes)(*V)->datum ) ;
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
		if( (*V)->precursor != aAsciiString )
			dealloc( s ) ;
	a	= repositoryWrite( (string) (*V)->datum , (string) V[1]->datum ) ;
	if( (*V)->precursor != aAsciiString )
		dealloc( s ) ;
//	if( V[1]->precursor != aAsciiString )
//		dealloc( t ) ;
	return a ;
	}
