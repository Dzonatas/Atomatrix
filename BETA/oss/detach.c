/***************************************
*
* detach.c
*
*******************/


#include "ATOMATRIX.h"

#if defined( MICROSOFT_win32 )
# include <windows.h>
#elif defined( UNIX_freebsd ) || defined( UNIX_linux )
# include <dlfcn.h>
#else
# error
#endif


void detach( void * l )
{

#if defined( MICROSOFT_win32 )
FreeLibrary( l ) ;

#elif defined( UNIX_freebsd ) || defined( UNIX_linux )
dlclose( l );

#else
return;

#endif
}
