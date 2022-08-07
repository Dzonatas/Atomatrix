/*************************************************
*
* aWND_event.c
*
***********************/

#define no_init_all deprecated
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#include "ATOMATRIX.h"
#include "wnd.h"


#ifdef _DEBUG
void dbgmsg( char * S , void * A , void * B , void * C )
	{
	extern int sprintf( char*,const char*,...) ;
	DWORD	w ;
	char s[1024] ;
	sprintf( s , S , A , B , C ) ;
	WriteConsole( GetStdHandle( STD_OUTPUT_HANDLE ) , s , strlen( s ) , &w , 0 ) ;
	}
#endif


extern atom		aWND_EVENT ;

atom aWND_event( atom A , int T , atom* V )
	{
	extern atom		(*atomize)(atom) ;
	atom			a ;
	HANDLE			e ;

	e	= CreateEvent( 0 , 0 , 0 , 0 ) ;
	a	= atomize( aWND_EVENT ) ;
	a->datum = e ;
	return a ;
	}

atom aWND_EVENT_reset( atom A , int T , atom* V )
	{
	if( A == aWND_EVENT )
		return atominul ;
	ResetEvent( (HANDLE) A->datum ) ;
	return atominul ;
	}

atom aWND_EVENT_set( atom A , int T , atom* V )
	{
	if( A == aWND_EVENT )
		return atominul ;
	SetEvent( (HANDLE) A->datum ) ;
	return atominul ;
	}

struct	_arg
	{
	HANDLE			e ;
	} ;

void* aWND_EVENT_task( struct _arg* A  )
	{
	WaitForSingleObject( A->e , INFINITE ) ;
	return atominul ;
	}


atom aWND_EVENT_wait( atom A , int T , atom* V )
	{
	extern void*		(*taskSplice)(void*,void*) ;

	if( A == aWND_EVENT )
		return atominul ;
	if( WaitForSingleObject( A->datum , 0 ) == WAIT_TIMEOUT )
		{
		struct _arg		a ;
		a.e	= A->datum ;
		taskSplice( aWND_EVENT_task , &a ) ;
		}
	return atominul ;
	}


void aWND_EVENT_destruct( atom A )
	{
	if( A == aWND_EVENT )
		return ;
	CloseHandle( A->datum ) ;
	}

void aWND_EVENT_constructent( atom A )
	{
	extern atom		(*oDestructor)(atom,void(*)(atom)) ;
	extern atom		(*atomethodize)(atom,string,atom(*)(atom,int,atom*)) ;

#	define __(_1)							\
		{							\
		extern atom 		aWND_EVENT_##_1(atom,int,atom*) ;	\
		atomethodize( aWND_EVENT , #_1 , aWND_EVENT_##_1 ) ;		\
		}
	__( set ) ;
	__( wait ) ;
	__( reset ) ;

	oDestructor( aWND_EVENT , aWND_EVENT_destruct ) ;
	}