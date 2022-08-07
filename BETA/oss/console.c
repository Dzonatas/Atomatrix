/*************************************************
*
* console.c
*
***********************/


#include "ATOMATRIX.h"
#include <windows.h>
#include <stdio.h>

/*
void dbgmsg( char * S , void * A , void * B , void * C )
	{
	DWORD	w ;
	char s[1024] ;
	sprintf( s , S , A , B , C ) ;
	WriteConsole( GetStdHandle( STD_OUTPUT_HANDLE ) , s , strlen( s ) , &w , 0 ) ;
	}
*/

int	console ;

void console_show( void )
	{
	if( ! console )
		{
		AllocConsole( ) ;
		console = 1 ;
		}
	}

void console_hide( void )
	{
	if( console )
		{
#if DEBUG	
		extern string console_input( void ) ;
		extern void console_output( string T ) ;
		console_output( "\nPRESS ANY KEY TO CLOSE > " ) ;
		console_input() ;		// UPDATE: debug
#endif
		FreeConsole( ) ;
		console = 0 ;
		}
	}


void console_output( string T )
	{
	DWORD		w ;
	if( ! console )
		console_show() ;
	WriteConsole( GetStdHandle( STD_OUTPUT_HANDLE ) , T , strlen( T ) , &w , 0 ) ;
	}


string console_input( void )
	{
	extern void *	allocm(unsigned int) ;
	extern void *	allocr(void*,unsigned int) ;
	extern void	dealloc(void*);
	int		z ;
	int		l ;
	string		s ;
	string		p ;
	char		c ;
	char		b[2] ;
	DWORD		r ;

	if( ! console )
		console_show() ;
	z	= 10 ;
	p = s	= allocm( z ) ;
	l	= 0 ;
	do	{
		if( ( ! ReadConsole( GetStdHandle( STD_INPUT_HANDLE ) , &b , 1 , &r , 0 ) ) || r != 1 )
			{
			dealloc( s ) ;
			return 0 ;
			}
		c = b[0] ;
		if( c == 13 )
			continue ;
//			c = 10 ;
		if( c == 8 )
			{
			if( ! l )
				continue ;
			console_output( "\b" ) ;
			l-- ;
			p-- ;
			continue ;
			}
		if( c == 9 )
			{
			// UPDATE: do tab
			}
		if( ++l == z )
			{
			s = allocr( s , z += 10 ) ;
			p = s + l - 1 ;
			}
		*(p++)	= c ;
		} while( c != 10 ) ;
	*( ( s = allocr( s , l + 1 ) ) + l ) = 0 ;
	return s ;
	}