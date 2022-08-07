#include "ATOMATRIX.h"
#include <windows.h>
#include "tlsp.h"


DWORD		tlsIndex ;
HINSTANCE	DLL_instance ;
HANDLE		mutex ;
HANDLE		processHeap ;
HANDLE		threadsEvent ;
tlsp		tlspp ;

BOOL WINAPI DllMain( HINSTANCE I , DWORD R , LPVOID V )
	{
	extern void console_hide(void) ;
	extern void dbgmsg( char * S , void * A , void * B , void * C ) ;
	if( R == DLL_THREAD_ATTACH )
		{
		tlsp	p ;
		p = HeapAlloc( processHeap , HEAP_ZERO_MEMORY , sizeof( struct _tlsp ) ) ;
		TlsSetValue( tlsIndex , p ) ;
		p->tlspp = tlspp ;
		tlspp	= p ;
		return 1 ;
		}
	else
	if( R == DLL_THREAD_DETACH )
		{
		tlsp	p = tlspp ;
		tlsp	t = (tlsp) TlsGetValue( tlsIndex ) ;
		if( p == t )
			tlspp = p->tlspp ;
		else	{
			while( p->tlspp != t )
				p = p->tlspp ;
			p->tlspp = t->tlspp ;
			}
		if( t->memory )
			HeapDestroy( t->memory ) ;
		if( t->detachEvent )
			SetEvent( t->detachEvent ) ;
		HeapFree( processHeap , 0 , t ) ;
		ReleaseMutex( mutex ) ;
		p	= tlspp ;
		while( p && ! p->created )
			p = p->tlspp ;
		if( ! p )
			SetEvent( threadsEvent ) ;
		return 1 ;
		}
	else
	if( R == DLL_PROCESS_ATTACH )
		{
		tlsp	p ;
		processHeap	= GetProcessHeap( ) ;
		if( ! ( mutex = CreateMutex( 0 , 1 , "ATOMATRIX.MUTEX" ) ) )  // UPDATE: validity of multiple processes 
			return 0 ;
		if( GetLastError( ) == ERROR_ALREADY_EXISTS )
			return 0 ;
		if( ( tlsIndex = TlsAlloc( ) ) == TLS_OUT_OF_INDEXES )
			return 0 ;
		p = HeapAlloc( processHeap , HEAP_ZERO_MEMORY , sizeof( struct _tlsp ) ) ;
		TlsSetValue( tlsIndex , p ) ;
		tlspp	= p ;
		threadsEvent	= CreateEvent( 0 , 1 , 0 , 0 ) ;
		DLL_instance = I ;
		return 1 ;
		}
	else
	if( R == DLL_PROCESS_DETACH )
		{
		tlsp	t = (tlsp) TlsGetValue( tlsIndex ) ;
		if( t->memory )
			HeapDestroy( t->memory ) ;
		if( tlsIndex )
			TlsFree( tlsIndex ) ;
		if( mutex ) 
			CloseHandle( mutex ) ;
		if( threadsEvent )
			CloseHandle( threadsEvent ) ;
		console_hide( ) ;
		return 1 ;
		}
	return 1 ;
	}