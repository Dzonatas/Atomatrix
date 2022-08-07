/*****************************************
*
* o_get.c
*
**************************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include <windows.h>
#include <wininet.h>

struct	_arg
	{
	union	{
		LPCTSTR		s ;
		LPCWSTR		u ;
		}	p ;
	unsigned	t ;
	unsigned	u ;
	} ;


void* aURL_get__task( struct _arg* A )
	{
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern void		(*dealloc)(void*) ;
	extern unsigned		(*str_len)(string) ;
	extern string		(*str_cpy)(string,string) ;
	extern string		(*str_cat)(string,string) ;
	extern string		(*aAtomatrixSeriestring)(void) ;
	string			s ;
	void *			p ;
	HINTERNET		n ;
	HINTERNET		u ;
	DWORD			x ;
	DWORD			z ;
	DWORD			e ;
	int			y ;
	string			v ;

	v = allocm( str_len( aAtomatrixSeriestring() ) + 12 ) ;
	str_cpy( v , "ATOMATRIX/" ) ;
	str_cat( v , aAtomatrixSeriestring() ) ;

	n = InternetOpen( v , INTERNET_OPEN_TYPE_DIRECT , 0 , 0 , 0 ) ;
	dealloc( v ) ;
	if( A->t )
		{
		InternetSetOption( n , INTERNET_OPTION_CONNECT_TIMEOUT , &A->t , sizeof(DWORD) ) ;
		InternetSetOption( n , INTERNET_OPTION_RECEIVE_TIMEOUT , &A->t , sizeof(DWORD) ) ;
		InternetSetOption( n , INTERNET_OPTION_SEND_TIMEOUT ,    &A->t , sizeof(DWORD) ) ;
		}
	if( ! n )
		return atominul ;
	if( A->u )
		u = InternetOpenUrlW( n , A->p.u , 0 , 0 , INTERNET_FLAG_NO_CACHE_WRITE , 0 ) ;
	else
		u = InternetOpenUrl( n , A->p.s , 0 , 0 , INTERNET_FLAG_NO_CACHE_WRITE , 0 ) ;
	s	= allocm( z = 4096 ) ;
	p	= s ;
	y	= 0 ;
	do	{
		if( InternetReadFile( u , p , z , &x ) )
			{
			if( x == 0 )
				break ;
			y	+= x ;
			s	= allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		if( ERROR_INSUFFICIENT_BUFFER == ( e = GetLastError() ) )
			{
			z *= 2 ;
			s	= allocr( s , y + z ) ;
			p	= s + y ;
			continue ;
			}
		else
			break ;
		} while( 1 ) ;
	*(char*)p = 0 ;
	y++ ;
	InternetCloseHandle( u ) ;
	InternetCloseHandle( n ) ;
	return allocr( s , y ) ;
	}
	

atom o_get( atom A , int T , atom* V )
	{
	extern void*		(*taskSplice)(void*,void*) ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedString ;
	extern atom		aInteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		(*aAsciiString_atomize)(string) ;
	struct _arg		a ;

	if( T > 2 || ! T )
		{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}

	if( V[0]->precursor == aAsciiString )
		{
		a.p.s	= V[0]->datum ;
		a.u	= 0 ;
		}
	else
	if( V[0]->precursor == aUnicodedString )
		{
		a.p.u	= V[0]->datum ;
		a.u	= 1 ;
		}
	else	{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}

	if( T == 2 && V[1]->precursor != aInteger )
		{
		aError_INVARG->allusions++;
		return aError_INVARG;
		}
	if( T == 2 )
		a.t	= (unsigned)V[1]->datum ;
	else
		a.t	= 0 ;
	return aAsciiString_atomize( taskSplice( aURL_get__task , &a ) ) ;
	}