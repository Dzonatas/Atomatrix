/***************************************
*
* memory.c
*
*******************/


#include "ATOMATRIX.h"
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "tlsp.h"


#define MDBG 3

#ifdef MDBG
# include <crtdbg.h>
int	memsz = 0;
void *	memc	= (void *)0x00858D68 ;
FILE*	fmem ;

#endif

extern HANDLE	processHeap ;

void displayLastErrorText( void );


void * allocm( unsigned int sz )
	{
#	if MDBG == 1
	void * m	= malloc( sz ) ;
	memsz		+= _msize( m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 2
	extern DWORD	tlsIndex ;

	void * m = HeapAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0 , sz ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 3
	void * m	= HeapAlloc( processHeap , 0 , sz ) ;
	memsz		+= HeapSize( processHeap , 0 , m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	else
//	return HeapAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0 , sz ) ;
	return HeapAlloc( processHeap , 0 , sz ) ;

#	endif
	}


void * allocz( unsigned int sz )
	{
#	if MDBG == 1
	void * m	= calloc( 1 , sz ) ;
	memsz		+= _msize( m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 2
	extern DWORD	tlsIndex ;
	void * m = HeapAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , HEAP_ZERO_MEMORY , sz ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 3
	void * m	= HeapAlloc( processHeap , HEAP_ZERO_MEMORY , sz ) ;
	memsz		+= HeapSize( processHeap , 0 , m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	else
//	return HeapAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , HEAP_ZERO_MEMORY , sz ) ;
	return HeapAlloc( processHeap , HEAP_ZERO_MEMORY , sz ) ;

#	endif
	}


void * allocr( void * p , unsigned int sz )
	{
#	if MDBG == 1
	void * m ;
	memsz		-= _msize( p ) ;
	m		= realloc( p , sz ) ;
	memsz		+= _msize( m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 2
	extern DWORD	tlsIndex ;
	void * m = HeapReAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0 , p , sz ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	elif	MDBG == 3
	void * m ;
	memsz		-= HeapSize( processHeap , 0 , p ) ;
	m		= HeapReAlloc( processHeap , 0 , p , sz ) ;
	memsz		+= HeapSize( processHeap , 0 , m ) ;
	if( m == memc )
		_asm int 3 ;
	return m ;

#	else
//	return HeapReAlloc( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0 , p , sz ) ;
	return HeapReAlloc( processHeap , 0 , p , sz ) ;

#	endif
	}


void dealloc( void * p )
	{
	extern DWORD	tlsIndex ;
#	if MDBG == 1
	memsz		-= _msize( p );
	free( p ) ;

#	elif	MDBG == 2
	if( p == memc )
		_asm int 3 ;
	HeapFree( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0 , p ) ;

#	elif	MDBG == 3
	memsz		-= HeapSize( processHeap , 0 , p ) ;
	HeapFree( processHeap , 0 , p ) ;
	return ;

#	else
//	HeapFree( ( (tlsp)TlsGetValue( tlsIndex ) )->memory , 0, p ) ;
	HeapFree( processHeap , 0, p ) ;

#	endif
	}


int check_memory( void )
	{
#	if MDBG == 1
	return _CrtCheckMemory() ;

#	else
	return 1 ;

#	endif
	}


#	if MDBG == 1
_CrtMemState		cms ;

int _MCRTD_report( int T , string M , int *V )
	{
	extern void console_output(string);
	console_output( M ) ;
	fprintf( fmem , "%s" , M ) ;
	fflush( fmem ) ;
	if( T == _CRT_WARN )
		return 1 ;
	*V	= 1 ;
	return 1 ;
	}
#endif


void memory_constructor( void )
	{
#	if MDBG == 1
	int			f ;
	char			b[256] ;
	char			fn[256] ;

	GetEnvironmentVariable( "_ATOMATRIX", b, sizeof( b ) );
	sprintf( fn, "%smem.txt", b );

	fmem = fopen( fn, "w" );
	fprintf( fmem , "----\n" ) ;
	fflush( fmem ) ;

	_CrtSetReportHook( _MCRTD_report ) ;
	f = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) ;
	f |= _CRTDBG_ALLOC_MEM_DF ;
	f |= _CRTDBG_CHECK_ALWAYS_DF ;
	f |= _CRTDBG_CHECK_CRT_DF ;
	f |= _CRTDBG_DELAY_FREE_MEM_DF ;
	f &= ~_CRTDBG_LEAK_CHECK_DF ;
	_CrtSetDbgFlag( f ) ;
	_CrtSetReportMode( _CRT_WARN , _CRTDBG_MODE_DEBUG ) ;
	_CrtSetReportMode( _CRT_ERROR , _CRTDBG_MODE_DEBUG ) ;
	_CrtSetReportMode( _CRT_ASSERT , _CRTDBG_MODE_DEBUG ) ;
	_CrtMemCheckpoint( &cms ) ;

#	else	
	extern DWORD		tlsIndex ;
	( (tlsp)TlsGetValue( tlsIndex ) )->memory = HeapCreate( HEAP_NO_SERIALIZE , 0 , 0 ) ;
	return ;

#	endif
	}


void memory_destructor( void )
	{
#	if MDBG == 1
	char		s[40] ;

	_CrtMemDumpAllObjectsSince( &cms ) ;
	console_output( "memsz=" ) ;
	console_output( _ltoa( memsz , s , 10 ) ) ;
	console_output( "\n" ) ;

	fprintf( fmem , "memsz=%ld" , memsz ) ;
	fclose( fmem ) ;

#	else
	// HeapDestroy( ... ) in "dll.c"
	return ;

#	endif
	}

#include <lmerr.h>

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
		if( m = LoadLibraryEx( TEXT("netmsg.dll") , NULL , LOAD_LIBRARY_AS_DATAFILE ) )
			f |= FORMAT_MESSAGE_FROM_HMODULE ;
	l	= FormatMessageA( f , m , e , MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ) , (LPSTR) &p , 0 , NULL ) ;
	if( l )	{
		MessageBox( 0 , p , "ATOMATRIX" , MB_OK | MB_ICONERROR ) ;
		LocalFree( p ) ;
		}
	if( m )
		FreeLibrary( m ) ;
	}




atom aAtomatrix_memory( atom A , int T , atom* V )
	{

#ifdef MDBG
	extern atom		aInteger_atomize(unsigned) ;

	return aInteger_atomize( memsz ) ;

#else
	return 0 ;
#endif
	}


