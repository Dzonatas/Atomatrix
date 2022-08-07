/********************************************
*
* winmain.c
*
*****************************/


#include <windows.h>
#include <lmerr.h>
#include "include.h"


void	displayLastErrorText( void ) ;
int	atomatrix_process( void ) ;


HINSTANCE hinst ;

int WINAPI WinMain( HINSTANCE I , HINSTANCE P , LPSTR C , int S )
	{
	atomatrix_process() ;
	displayLastErrorText( ) ;
	return 0 ;
	}


void displayLastErrorText( void )
	{
	HMODULE		m ;
	LPSTR		p ;
	DWORD		l ;
	DWORD		f ;
	DWORD		e ;

	e	= GetLastError() ;
	f	= FORMAT_MESSAGE_ALLOCATE_BUFFER
		| FORMAT_MESSAGE_IGNORE_INSERTS
		| FORMAT_MESSAGE_FROM_SYSTEM ;
	m	= 0 ;
	if( e >= NERR_BASE && e <= MAX_NERR )
		if( m = LoadLibraryEx( TEXT("netmsg.dll") , NULL ,
			LOAD_LIBRARY_AS_DATAFILE ) ) 
				f |= FORMAT_MESSAGE_FROM_HMODULE ;
	l	= FormatMessageA( f , m , e ,
		MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ) ,
		(LPSTR) &p , 0 , NULL ) ;
	if( l )	{
		MessageBox( 0 , p , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
		LocalFree( p ) ;
		}
	if( m )
		FreeLibrary( m ) ;
	}