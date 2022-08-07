/***************************************
*
* symbol.c
*
*******************/


#include "ATOMATRIX.h"

#if defined( MICROSOFT_win32 )
# include <windows.h>
#elif defined( UNIX_darwin )
# include <mach-o/dyld.h>
#elif defined( UNIX_freebsd ) || defined( UNIX_linux )
# include <dlfcn.h>
#else
# error
#endif

void * symbol( atom A , string name )
	{
	extern string		add_s(string,string);
	extern void		dealloc(void*);
	void *			s;

	#if defined( MICROSOFT_win32 ) && defined ( __BORLANDC__ )
	void *			ss;
	ss	= add_s( "_", name );
	s       = GetProcAddress( A->datum , ss );
	dealloc( ss ) ;
	if( s )
	        return s;

	#elif defined( MICROSOFT_win32 )
	s       = GetProcAddress( A->datum , name );
	if( s )
	        return s;

	#elif defined( UNIX_darwin )
	void *			ss;
	ss	= add_s( "_", name );
	s	= (void*) NSLookupSymbolInImage( A->datum , ss,
		        NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR
			| NSLOOKUPSYMBOLINIMAGE_OPTION_BIND
			);
	dealloc( ss ) ;
	if( s )
		if( s = NSAddressOfSymbol( s ) )
			return s;

	#elif defined( UNIX_freebsd ) || defined( UNIX_linux )
	s	= dlsym( A->datum , name );
	if( s )
		return s;

	#endif

	return (void*) 0;
	}
