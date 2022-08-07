/***************************************
*
* atomatrix.c
*
*******************/


#include "ATOMATRIX.h"
#include <windows.h>
#include <process.h>
#include "tlsp.h"
#include "task.h"
#include "program.h"

#ifdef _DEBUG
void SetThreadName(DWORD,LPCSTR) ;
#endif

#include "fibers.h"	// for WINXP



unsigned int _lib atomatrix(void)
	{
	extern void		atomatrix_thread(void*) ;
	extern HANDLE		threadsEvent ;


#ifdef _DEBUG
	SetThreadName( -1 , "ATOMATRIX()" ) ;
#endif


	_beginthread( atomatrix_thread , 0 , 0 ) ;
	WaitForSingleObject( threadsEvent , INFINITE ) ;  // CRT requires first thread

	return 0 ;
	}


HANDLE			taskEvent ;
LPVOID			premier_fiber ;

task			previous_task ;
task			current_task ;



void atomatrix_thread( void * D )
	{
	extern void		constructor(void) ;
	extern void		destructor(void) ;
	extern void		memory_constructor(void) ;
	extern void		memory_destructor(void) ;
	extern VOID CALLBACK	atomatrix_task(LPVOID) ;
	extern string		string_duplicate(string) ;
	extern void *		allocz(unsigned) ;
	extern void		dealloc(void*) ;
	extern void		destruct(atom) ;
	extern void		end_threads(void) ;
	extern void		(*function)(void) ;
	extern atom		(*oModule)(unsigned) ;
	extern atom		(*getProgram)(atom,string,atom) ;
	extern DWORD		tlsIndex ;
	task			tasks ;
	task			t ;
	task			p ;
	atom			a ;
	atom			x ;

#ifdef _DEBUG
	SetThreadName( -1 , "ATOMATRIX VE" ) ;
#endif
	( (tlsp)TlsGetValue( tlsIndex ) )->created = 1 ;

	taskEvent	= CreateEvent( 0 , 1 , 0 , 0 ) ;

	memory_constructor() ;
	function = 0;
	constructor();


					// win32 (NT/XP/98) can use fibers to handle mulitple tasks with C and C++.
					// yet, Standard C/C++ setjmp() and longjmp() functions do not manage C++ context switches
					// therefore, we use fibers for C/C++ or setjmp()/longjmp() only for C
					// however, setjmp()/longjmp() means no dynamic C++ loads
	premier_fiber	= ConvertThreadToFiber( 0 ) ;
	t = tasks	= allocz( sizeof( struct _task ) ) ;
	t->fiber	= CreateFiber( 0 , atomatrix_task , t ) ;
	t->taskp	= t ;
	t->taskn	= t ;
	a		= oModule( 1 ) ;
	x		= getProgram( a , "_auto" , 0 ) ;
	t->programme	= x ;
	while( t )
		{
		SwitchToFiber( ( current_task = t )->fiber ) ;   // UPDATE: does asm optimize correctly ?
		if( ! ( p = previous_task )->ended )
			t = p->taskp ;
		else	{
			DeleteFiber( p->fiber ) ;
			p->taskp->taskn	= p->taskn ;
			if( p == ( t = p->taskn->taskp = p->taskp ) )
				t = 0 ;
			else
			if( p == tasks )
				tasks = t ;
			if( p->programme )
				if( --p->programme->allusions == 0)
					destruct( p->programme ) ;
			if( p->args )
				if( --p->args->allusions == 0 )
					destruct( p->args ) ;
			dealloc( p ) ;				// UPDATE: the aMacroprocessity should dealloc task
			if( ! t )
				break ;
			}
		if( ( p = t )->thread )
			{
			while( ( t = t->taskp )->thread )
				if( p == t )
					WaitForSingleObject( taskEvent , INFINITE ) ;
			ResetEvent( taskEvent ) ;
			}
		}

	destructor() ;
	end_threads() ;
	memory_destructor() ;

	return ;
	}

#ifdef _DEBUG
void dbgmsg( char * S , void * A , void * B , void * C )
	{
	extern void console_output( string ) ;
	extern int sprintf( char*,const char*,...) ;
	char s[1024] ;
	sprintf( s , S , A , B , C ) ;
	console_output( s ) ;
	}
#endif

void yield_task( void )
	{
	task			p ;
	task			t ;

	if( ( t = ( p = current_task )->taskp )->thread )
		{
		do if( p == t )
			WaitForSingleObject( taskEvent , INFINITE ) ;
			while( ( t = t->taskp )->thread ) ;
		ResetEvent( taskEvent ) ;
		}
	SwitchToFiber( ( current_task = t )->fiber ) ;
	}


VOID CALLBACK atomatrix_task( LPVOID D )
	{
	extern void		(*function)(void) ;
//	t->ss	= allocm( sizeof(struct _atom*) * STACKSIZE );
//	t->sp	= t->ss ;
//	t->aMacroprocessity	=
	if( function )
		function();
	previous_task		= D ;
	previous_task->ended	= 1 ; 
	SwitchToFiber( premier_fiber ) ;
	}

task new_task( void )
	{
	extern void *		allocz(unsigned) ;
	task			t ;
	task			p  = current_task ;
	task			n  = current_task->taskn ;

	t		= allocz( sizeof( struct _task ) ) ;
	t->fiber	= CreateFiber( 0 , atomatrix_task , t ) ;
	( t->taskp = p )->taskn = t ;
	( t->taskn = n )->taskp	= t ;
	return t ;
	}


task getTask( void ) 
	{
	return current_task ;
	}

struct _task_thread
	{
	unsigned		n ;
	task			t ;
	HANDLE			thread ;
	void *			(*function)() ;
	void *			result ;
	} ;

unsigned _stdcall task_thread( struct _task_thread * T )
	{
	extern DWORD		tlsIndex ;
#ifdef _DEBUG
	char	abc[2] ;	
	abc[0] = 'A' + T->n ;
	abc[1] = 0 ;
	SetThreadName( -1 , abc ) ;
#endif
	( (tlsp)TlsGetValue( tlsIndex ) )->detachEvent = taskEvent ;
	while( T->function )
		{
		T->result = (T->function)( T->result ) ;
		if( T->t )
			{
			T->t->thread = 0 ;
			T->function = 0 ;
			}
		SetEvent( taskEvent ) ;
		T->t	= 0 ;
		SuspendThread( T->thread ) ;
		}
	return 0 ;
	}

struct _task_thread **	threads = 0 ;
unsigned		nThreads = 0 ;

void* taskSplice( void* F , void* A )
	{
	unsigned i ;
	unsigned n ;
	extern void	(*yieldTask)(void) ;
	extern void *	allocz(unsigned) ;
	extern void *	allocm(unsigned) ;
	extern void *	allocr(void*,unsigned) ;
	struct _task_thread *	t ;

	if( ! threads )
		threads = allocm( sizeof( struct _task_thread * ) ) ;
	n	= 0 ;
	while( n < nThreads && threads[n]->t )
		n++ ;
	if( n == nThreads )
		{
		nThreads++ ;
		threads = allocr( threads , sizeof( struct _task_thread * ) * nThreads ) ;
		threads[n] = allocz( sizeof( struct _task_thread ) ) ;
		threads[n]->thread =  (HANDLE) _beginthreadex( 0 , 0 , task_thread , threads[n] , CREATE_SUSPENDED , &i ) ;
		threads[n]->n = n ;
		}
	t		= threads[n] ;
	t->t		= current_task ;
	t->function	= F ;
	t->result	= A ;
	current_task->thread	= 1 ;
	n=ResumeThread( t->thread ) ;
	yieldTask( ) ;
	return t->result ;
	}

void end_threads( void )
	{
	extern void	dealloc(void*) ;
	unsigned	n = 0 ;
	while( n < nThreads )
		{
		threads[n]->function = 0 ;
		ResumeThread( threads[n]->thread ) ;
		n++ ;
		}
	SwitchToThread() ;
	n	= 0 ;
	while( n < nThreads )
		{
		if( WaitForSingleObject( threads[n]->thread , 1000 ) == WAIT_TIMEOUT )
			TerminateThread( threads[n]->thread , 0 ) ;
		CloseHandle( threads[n]->thread ) ;
		dealloc( threads[n] ) ;
		n++ ;
		}
	dealloc( threads ) ;
	}


#ifdef _DEBUG

typedef struct tagTHREADNAME_INFO
	{
	DWORD dwType ;		// must be 0x1000
	LPCSTR szName ;		// pointer to name (in user addr space)
	DWORD dwThreadID ;	// thread ID (-1=caller thread)
	DWORD dwFlags ;		// reserved for future use, must be zero
	} THREADNAME_INFO ;

void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
	{
	THREADNAME_INFO info;
	info.dwType	= 0x1000 ;
	info.szName	= szThreadName ;
	info.dwThreadID	= dwThreadID ;
	info.dwFlags	= 0;
	__try	{
		RaiseException( 0x406D1388 , 0 , sizeof(info)/sizeof(DWORD) , (DWORD*)&info );
		} __except( EXCEPTION_CONTINUE_EXECUTION ) { }
	}

#endif