/***************************************
*
* library.c
*
*******************/


#include "ATOMATRIX.h"
#include "cmp_s.h"

#if defined( MICROSOFT_win32 )
# include <windows.h>

#elif defined( UNIX_darwin )
# include <mach-o/dyld.h>

#elif defined( UNIX_freebsd ) || defined( UNIX_linux )
# include <dlfcn.h>

#endif

/*
struct _loadp				// structure to manage single load of dynamic executables
	{
	string		path ;		// path to dynamic executable
	unsigned	attached ;
	struct _loadp *	loadpp ;
	}				// threads ???? tls ???? UPDATE::: uses win32 fibers instead


typedef struct _loadp	loadpp ;
*/

atom			aLibrary ;
atom			aLibraryError ;
atom			aLibraryVariable ;
atom			aLibraryVariant ;
atom			aLibraryObject ;
atom			aLibraryMachine ;
atom			aDirectionLibrent ;

void library_preconstructor( atom A )
	{
	extern atom		aAtomatrix ;
	extern atom		aLibrary ;
	extern atom		atomize(atom) ;
	extern void		dealloc(void*) ;
	extern string		add_s(string,string) ;
	extern void *		symbol(atom,string) ;
	extern void		interatomize(atom,atom) ;
	extern string		direction_ATOMATRIX ;
	extern atom		oDestructor(atom,void*) ;
	extern atom		oConstructor(atom,void*) ;
	atom*			atomatrixp ;
	void			(*f)(atom) ;
	void *			l ;
	string			s ;
	string			ss ;
	string			name ;

	name	= A->interatom->datum ;

#	if defined( MICROSOFT_win32 )
	s	= add_s( name , DSOEXT ) ;
	ss      = add_s( direction_ATOMATRIX , s ) ;
	l       = (void*) LoadLibrary( ss ) ;

#	elif defined( UNIX_darwin )
	s	= add_s( name , DSOEXT ) ;
	ss      = add_s( direction_ATOMATRIX , s ) ;
	l	= (void*) NSAddImage( ss , NSADDIMAGE_OPTION_RETURN_ON_ERROR ) ;

#	elif defined( UNIX_freebsd ) || defined( UNIX_linux )
	s	= add_s( name , DSOEXT ) ;
	ss      = add_s( direction_ATOMATRIX , s ) ;
	l	= dlopen( ss , RTLD_LAZY ) ;

#	endif

	dealloc(s);
	dealloc(ss);

	if( ! l )
		return ;
	A->datum = l ;
	if( ( atomatrixp = symbol( A , "ATOMATRIX_ATOMICAL" ) ) )
		*atomatrixp = aAtomatrix ;
	if( ( f = symbol( A , "constructor" ) ) )
		oConstructor( A , f ) ;
	if( ( f = symbol( A , "destructor" ) ) )
		oDestructor( A , f ) ;
	if( ( f = symbol( A , "preconstructor" ) ) )
		f( A ) ;
	return ;
	}



void library_destructor( atom A )
	{
	extern atom		aLibrary ;

	if( A == aLibrary )
		return ;

#	if defined( MICROSOFT_win32 )
	if( A->datum )
		FreeLibrary( A->datum ) ;

#	elif defined( UNIX_freebsd ) || defined( UNIX_linux )
	if( A->datum )
		dlclose( A->datum );

#	else
#	error

#	endif
	A->datum = 0 ;
	}


atom oLibrary( string S )
	{
	extern atom		aAtomatrix ;
	extern void		interatomize(atom,atom) ;
	atom			l ;
	atom			s ;
	atom			a ;

	l	= aLibrary->intratom ;
	while( l )
		{
		a	= l->interatom ;
		while( a )
			{
			if( a->precursor == aDirectionLibrent )
				{
				if( cmp_s(((string)a->datum),S) == 0 )
					{
					l->allusions++ ;
					return l ;
					}
				}
			a	= a->interatoms ;
			}
		l	= l->intratoms ;
		}
	l		= atomize( aLibrary ) ;
	s		= atomize( aDirectionLibrent ) ;
	s->datum	= S ;
	interatomize( s , l ) ;
	interatomize( l , aAtomatrix ) ;
	library_preconstructor( l ) ;
	return l ;
	}

#ifdef _DEBUG
#define		SUBVERSION	"\\2"
#else
#define		SUBVERSION	"\\1"
#endif

void libraries_preconstructor( void )
	{
	extern atom		atomine ;
	extern atom		aAtomatrix ;
	extern atom		oAtomical(atom,string,atom) ;
	extern void		construct(atom) ;
	extern atom		oDestructor(atom,void*) ;

	aLibrary		= atomize( atomine ) ;
	aDirectionLibrent	= atomize( atomine ) ;
	oAtomical( aAtomatrix , "aLibrary" , aLibrary ) ;
	oDestructor( aLibrary , library_destructor ) ;
	aLibraryMachine		= oLibrary( "OSS\\machine" SUBVERSION "\\machine" ) ;
	aLibraryError		= oLibrary( "OSS\\error" SUBVERSION "\\error" ) ;
	aLibraryVariable	= oLibrary( "OSS\\variable" SUBVERSION "\\variable" ) ;
	aLibraryVariant		= oLibrary( "OSS\\variant" SUBVERSION "\\variant" ) ;
	aLibraryObject		= oLibrary( "OSS\\object" SUBVERSION "\\object" ) ;
	}


void libraries_constructor( void )
	{
	construct( aLibraryError ) ;
	construct( aLibraryVariable ) ;
	construct( aLibraryVariant ) ;
	construct( aLibraryMachine ) ;
	construct( aLibraryObject ) ;
	}


void libraries_destructor( void )
	{
	extern atom		aAtomatrix ;
	extern void		destruct(atom) ;
	atom			a ;
	atom			b ;

	a	= aLibrary->intratom ;
	do	{
		if( a->precursor == aLibrary && a->orbit != aAtomatrix )
			{
			b = a ;
			a = a->intratoms ;
			destruct( b ) ;
			}
		else
			a = a->intratoms ;
		} while( a ) ;
	destruct( aLibraryMachine ) ;
	destruct( aLibraryObject ) ;
	destruct( aLibraryVariant ) ;
	destruct( aLibraryVariable ) ;
	destruct( aLibraryError ) ;
	destruct( aLibrary ) ;
	}