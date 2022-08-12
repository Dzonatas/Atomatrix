/*******************************************
*
* atomatrix_process.c
*
**************************/


#include "include.h"
#include <windows.h>


#define BUFSZ	512
#ifdef _DEBUG
# define ATOMATRIX_OSS_PATH	"OSS\\Debug\\oss.dll"
#else
# define ATOMATRIX_OSS_PATH	"OSS\\Release\\oss.dll"
#endif

char* direction_ATOMATRIX( void )
	{
	unsigned int	i ;
	unsigned int	z ;
	unsigned int	c ;
	static char*	s ;

	if( s )
		return s ;
	z	= GetCurrentDirectory( 0 , 0 ) ;
	if( z < 10 )
		{
		SetLastError( 10 ) ;
		return (char*) 0 ;
		}
	s	= HeapAlloc( GetProcessHeap() , 0 , z ) ;
	GetCurrentDirectory( z , s ) ;
	c = i	= 0 ;
#ifndef _DEBUG
	while( ++i < z )
		{
		if( *(s+c++) == '\\' )
		 if( *(s+c++) == 'A')
		  if( *(s+c++) == 'T')
		   if( *(s+c++) == 'O')
		    if( *(s+c++) == 'M')
		     if( *(s+c++) == 'A')
		      if( *(s+c++) == 'T')
		       if( *(s+c++) == 'R')
			if( *(s+c++) == 'I')
			 if( *(s+c++) == 'X')
			  if( *(s+c++) == '\\')
				  i = z ;
		}
#else
	while (++i < z)
		{
		if (*(s + c++) == '\\')
			if (*(s + c++) == 'B')
				if (*(s + c++) == 'E')
					if (*(s + c++) == 'T')
						if (*(s + c++) == 'A')
							if (*(s + c++) == '\\')
								i = z;
		}
#endif
	if( i == z + 1 )
		*(s+c++) = 0 ;
	else	{
		c = z ;
		while( c && *(s+--c) != '\\' ) ;
		if( ! c )
			{
			SetLastError( 10 ) ;
			return (char*) 0 ;
			}
		*(s+ ++c) = 0 ;
		c++ ;
		}
	s	= (char*) HeapReAlloc( GetProcessHeap() , 0 , s , c ) ;
	return s ;
	}


char* direction_ATOMATRIX_OSS( void )
	{
	unsigned int		z ;
	unsigned int		c ;
	char*			s ;
	char*			p ;
	char*			pp ;

	s = pp	= direction_ATOMATRIX() ;
	z	= 1 ;
	while( *(pp++) )
		z++ ;
	p = pp	= (char*) HeapAlloc( GetProcessHeap() , 0 , z + sizeof( ATOMATRIX_OSS_PATH ) ) ;
	for( c = 0 ; *(s+c) ; pp++ )
		*pp = *(s+c++) ;
	s	= ATOMATRIX_OSS_PATH ;
	for( c = 0 ; *(s+c) ; pp++ )
		*pp = *(s+c++) ;
	*pp = 0 ;
	return p ;
	}


int atomatrix_process( void )
	{
	unsigned int	(*c)(char*) ;
	void*		l ;
	char*		p1 ;
	char*		p2 ;

	if( 0 == ( p1 = direction_ATOMATRIX() ) )
		return 0 ;
	if( 0 == ( p2 = direction_ATOMATRIX_OSS() ) )
		return 0 ;
	if( 0 == ( l = LoadLibrary( p2 ) ) ) 
		return 0 ;
	HeapFree( GetProcessHeap() , 0 , p2 ) ;
	SetEnvironmentVariable( "_ATOMATRIX" , p1 ) ;
	HeapFree( GetProcessHeap() , 0 , p1 ) ;

#	if defined ( __BORLANDC__ )
	if( 0 == ( c = (void*) GetProcAddress( l , "_atomatrix" ) ) )
		return 0 ;

#	else
	if( 0 == ( c = (void*) GetProcAddress( l , "atomatrix" ) ) )
		return 0 ;

#	endif

	(c)("COPYRIGHT BALLARD") ;

	FreeLibrary( l ) ;

	ExitProcess( 0 ) ;
	}