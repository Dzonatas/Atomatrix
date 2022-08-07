/***************************************
*
* EXIT.c
*
*******************/


#include "ATOMATRIX.h"



#if defined( MICROSOFT_win32 )
# include <windows.h>

void EXIT( unsigned  v )
	{
//	_flushall() ;
//	_exit(v) ;
	TerminateProcess( GetCurrentProcess() , v ) ;
	}

#else
# include <stdlib.h>
# include <signal.h>


void EXIT( unsigned  v )
	{
	signal(SIGALRM,SIG_IGN);
	signal(SIGHUP,SIG_IGN);
	signal(SIGINT,SIG_IGN);
	signal(SIGQUIT,SIG_IGN);
	exit(v) ;

	}

#endif
