// aModuleXML.c			Copyright (c) 2004 BALLARD


#include "ATOMATRIX.h"

#include "unicode.h"

#include <stdio.h>
#include <stdlib.h>

#include "object.h"
#include "objvar.h"
#include "varray.h"
#include "program.h"

#include "cmp_s.h"

#ifdef MICROSOFT_win32
# define __	"\\"
#include <windows.h>
#else
# define __	"/"
#endif

#define DBMPATH		"db"__"m"
#define DBMPXML		".xml"

atom				aModuleXML ;
atom				aModuleXML_serial ;


/*
static int equal_a_a( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandCompareEqual ;
	extern atom		aEquality ;
	extern void		(*destruct)(atom) ;

	if( A == B )
		return 1 ;
	else	{
		atom			a ;
		atom			aa ;
		a	= A ;
		do	{
			aa	= a->interatom ;
			while( aa )
				{
				if( aa->precursor == aOprandCompareEqual )
					if( ( a = ((oprand)aa->datum)( A , B ) ) == aEquality )
						{
						--a->allusions ;
						return 1 ;
						}
					else	{
						if( --a->allusions == 0 )
							destruct( a ) ;
						return 0 ;
						}
				aa = aa->interatoms ;
				}
			} while( ( a = a->precursor ) ) ;
		return 0 ;
		}
	}
*/

atom aModuleXML_atomize_a( atom A )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandCompareEqual ;
	extern atom		aEquality ;
	extern void		(*destruct)(atom) ;
	extern atom		(*aError_pUndefined)(void) ;
	extern string		(*str_dup)(string) ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		aModuleXML ;
	atom			a ;
	atom			b ;
	atom			r ;
	a	= A ;
	do	if( ( b = a->interatom ) )
			do	if( b->precursor == aOprandCompareEqual )
					{
					if( ( a = aModuleXML_serial->intratom ) )
						do	if( ( r = ((oprand)b->datum)( A , a->datum ) ) == aEquality )
								{
								if( --r->allusions == 0 )
									destruct( r ) ;
								a->orbit->allusions++ ;
								return a->orbit ;
								}
							else	if( --r->allusions == 0 )
									destruct( r ) ;
							while( ( a = a->intratoms ) ) ;
					a = atomize( aModuleXML ) ;
					( a->interatom = b = atomize( aModuleXML_serial ) )->orbit = a ;
					b->datum = A ;			// UPDATE: localize or duplicate A to a
					A->allusions++ ;
					return a ;
					}
				while( ( b = b->interatoms ) ) ;
		while( ( a = a->precursor ) ) ;
	return aError_pUndefined() ;				// UPDATE: specify error
	}

/*
void aModuleXML_destructor( atom A )
	{
	extern void		(*dealloc)(void*) ;

	if( A->datum )
		{
		dealloc( A->datum ) ;
		A->datum = 0 ;
		}
	}

void aModuleXML_serialString_destructor( atom A )
	{
	extern void		(*dealloc)(void*) ;

	if( A->datum )
		{
		dealloc( A->datum ) ;
		A->datum = 0 ;
		}
	}
*/


void aModuleXML_preconstructent(void)
	{
	extern atom		aModule ;
	extern atom		(*atomize)(atom) ;
	
	aModuleXML			= atomize( aModule ) ;
	}


atom aModuleXML_translatomize( atom X )			// UPDATE: aModule_aSerial
	{
	extern atom		aXML ;
	extern atom		aXML_aAttribute ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*aXML_interatomize_a_a)(atom,atom) ;
	extern atom		(*aXML_element_s)(string) ;
	extern atom		(*aXML_translatomize_XML)(atom) ;
	extern void		(*destruct)(atom) ;
        atom			a ;
	atom			s ;

	if( X->precursor == aXML )
		{
		if( ( a = X->interatom ) )
			while( ( a = a->interatoms ) )
				if( a->precursor == aXML_aAttribute )			// UPDATE: verify attribute name
					return aModuleXML_atomize_a( a->interatom->interatoms ) ;
				else
				if( a->precursor == aXML )
					{
					extern atom aInteger ;
					extern atom oModule(unsigned) ;
					s = aXML_translatomize_XML( a ) ;
//					if( s->precursor == aInteger )		// UPDATE: aModule_aSerial should not test for aInteger
//						return oModule( (unsigned) s->datum ) ;
					a = aModuleXML_atomize_a( s ) ;
					if( --s->allusions == 0 )
						destruct( s ) ;
					return a ;
					}
		return aError_pUndefined() ;
		}
	s	= X->interatom ;
	while( s->precursor != aModuleXML_serial )
		s = s->interatoms ;
	if( ! s )
		return aError_pUndefined() ;
	aXML_interatomize_a_a( a = aXML_element_s( "serial" ) , s->datum ) ;		// UPDATE: collapse to an attribute if possible
	return a ;
	}



void aModuleXML_constructent(void)
	{
	extern atom		aAtomatrical ;
	extern string		(*str_dup)(string) ;
	extern atom		(*aXML_translatomize_a_p)(atom,atom(*)(atom)) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*atomize)(atom) ;

	aModuleXML_serial		= atomize( aAtomatrical ) ;

	interatomize( aXML_translatomize_a_p( aAsciiString_atomize( str_dup( "serial" )  ) , aModuleXML_translatomize ) , aModuleXML ) ;
	}


void aModuleXML_destructent(void)
	{
	extern void		(*destruct)(atom) ;

	destruct( aModuleXML_serial ) ;
	destruct( aModuleXML ) ;
	}


string static repositoryPath( void )
	{
	string			s ;
	unsigned		x ;
	string			p ;

#	ifdef MICROSOFT_win32

	extern void*		(*allocm)(unsigned) ;

	x	= GetEnvironmentVariable( "_ATOMATRIX" , 0 , 0 ) ;		// UPDATE: use direction_Atomatrix
	s	= allocm( x + sizeof( DBMPATH ) + 1 ) ;
	GetEnvironmentVariable( "_ATOMATRIX" , s , x ) ;

#	else

	extern string		(*str_dup)(string) ;
	extern unsigned		(*str_len)(string) ;

	s	= str_dup( getenv( "_ATOMATRIX" ) ) ;
	x	= str_len( s ) ;
	s	= allocr( s , x + sizeof( DBMPATH ) + 1 ) ;

#	endif

	p	= s + x - 1 ;
	x	= 0 ;
	while( ( *p = *(DBMPATH+x++) ) ) p++ ;

	return s ;
	}


atom static repositoryWrite_a_a( atom A , atom B )
	{
	extern atom		(*aError_pUndefined)(void) ;
	extern int		(*str_len)(string) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	extern void		(*dealloc)(void*) ;
	string			s ;
	string			p ;
	DWORD			x ;
//	DWORD			z ;
	unsigned		c ;
	HANDLE			f ;
	string			S ;
	string			D ;

#ifdef DEBUG
		{
		extern atom aAsciiString ;
		if( A->precursor != aAsciiString )
			_asm int 3 ;
		}
#endif DEBUG
	S	= (string)A->datum ;				// UPDATE: canonicalize
#ifdef DEBUG
		{
		extern atom aAsciiString ;
		if( B->precursor != aAsciiString )
			_asm int 3 ;
		}
#endif DEBUG
	D	= (string)B->datum ;

	s	= repositoryPath() ;
	c	= str_len( s ) ;
	s	= allocr( s , c + 100 ) ;		// UPDATE: 100 ?

	if( ( f	= CreateFile( s , 0 , 0 , 0 , OPEN_EXISTING , FILE_FLAG_BACKUP_SEMANTICS , 0 ) ) != INVALID_HANDLE_VALUE )
		CloseHandle( f ) ;
	else
	if( ! CreateDirectory( s , 0 ) )
		{
		dealloc( s ) ;
		return aError_pUndefined() ;
		}

	p	= s + c ;
	c	= 0 ;
	while( ( *p = *(__+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(S+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(DBMPXML+c++) ) ) p++ ;

	if( ( f	= CreateFile( s , GENERIC_WRITE , 0 , 0 , CREATE_ALWAYS , 0 , 0 ) ) == INVALID_HANDLE_VALUE )
		{
		dealloc( s ) ;
		return aError_pUndefined() ;
		}
	dealloc( s ) ;
	if( ! WriteFile( f , D  , str_len( D ) , &x , 0 ) )
		{
		CloseHandle( f ) ;
		return aError_pUndefined() ;
		}
	CloseHandle( f ) ;
	return atominul ;
	}


static atom repositoryRead_a( atom A )
	{
	extern atom		(*aError_pUndefined)(void) ;
	extern void*		(*allocm)(unsigned) ;
	extern void*		(*allocr)(void*,unsigned) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern unsigned		(*str_len)(string) ;
	extern string		(*str_dup)(string) ;
	extern void		(*dealloc)(void*) ;
	unsigned		c ;
	string			p ;
	string			s ;
	HANDLE			f ;
	DWORD			x ;
	DWORD			z ;
	DWORD			e ;
	string			S ;

#ifdef DEBUG
		{
		extern atom aAsciiString ;
		if( A->precursor != aAsciiString )
			_asm int 3 ;
		}
#endif DEBUG
	S	= A->datum ;				// UPDATE: canonicalize

	s	= repositoryPath() ;
	c	= str_len( s ) ;
	s	= allocr( s , c + 100 ) ;		// UPDATE: 100 ?
	p	= s + c ;
	c	= 0 ;
	while( ( *p = *(__+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(S+c++) ) ) p++ ;
	c	= 0 ;
	while( ( *p = *(DBMPXML+c++) ) ) p++ ;

	z	= 4096 ;
	f	= CreateFile( s , GENERIC_READ , 0 , 0 , OPEN_EXISTING , 0 , 0 ) ;
	dealloc( s ) ;
	if( f == INVALID_HANDLE_VALUE )
		return aError_pUndefined() ;
	s	= (string)allocm( z ) ;
	p	= s ;
	c	= 0 ;
	do	{
		if( ReadFile( f , p , z , &x , 0 ) )
			{
			if( x == 0 )
				break ;
			c	+= x ;
			s	= (string)allocr( s , c + z ) ;
			p	= s + c ;
			continue ;
			}
		if( ERROR_INSUFFICIENT_BUFFER == ( e = GetLastError() ) )
			{
			z *= 2 ;		// UPDATE: validity of max z
			s	= (string)allocr( s , c + z ) ;
			p	= s + c ;
			continue ;
			}
		else
			break ;
		} while( 1 ) ;
	*(char*)p = 0 ;
	c++ ;
	CloseHandle( f ) ;
	return aAsciiString_atomize( allocr( s , c ) ) ;
	}


int illogicated( atom A )
	{
	extern atom		aIllogical ;
	atom			a ;

	if( A == aIllogical )
		return 1 ;
	if( ( a = A->precursor ) )
		do	if( a == aIllogical )
				return 1 ;
			while( ( a = a->precursor ) ) ;
	return 0 ;
	}


static atom module_a( atom A )
	{
	extern atom	aModule ;
	extern atom	aXML ;
	extern atom	aXML_aAttribute ;
	extern atom	(*aXML_translatomize_XML)(atom) ;
	extern void	(*destruct)(atom) ;
	extern atom	(*aError_pUndefined)(void) ;

	if( A->precursor == aXML_aAttribute )
		return aModuleXML_atomize_a( A->interatom->interatoms ) ;
	if( A->precursor == aXML )
		{
		atom a = aXML_translatomize_XML( A ) ;
		atom b = a ;
		while( b && b->precursor != aModule )
			b = b->precursor ;
		if( ! b )
			{
			if( --a->allusions == 0 )
				destruct( a ) ;
			return aError_pUndefined() ;
			}
		return a ;
		}
	return aError_pUndefined() ;
	}


void aModuleXML_demorph_a( atom A )
	{
	extern void		(*destruct)(atom) ;
	extern void		(*dealloc)(void*) ;
	atom	a ;
	atom	b ;

	if( A->datum )
		{
		object	o = A->datum ;

#define _(_1)	if( _1 && --_1->allusions == 0 ) destruct( _1 ) ;
		_( o->interobject ) ;
		_( o->interobject ) ;
		_( o->intraobject ) ;
		_( o->intraobjects ) ;
		_( o->orbit ) ;
		_( o->precursor ) ;
		_( o->principal ) ;
#undef _
		dealloc( o ) ;
		A->datum = 0 ;
		}
	if( ( a = A->interatom ) )
		do	if( a->precursor != aModuleXML_serial )
				{
				if( --a->allusions == 0 )
					{
					b = a->interatoms ;
					destruct( a ) ;
					a = b ;
					continue ;
					}
				}
			while( ( a = a->interatoms ) ) ;
	}


atom aModuleXML_atomorphize( atom M )
	{
	extern atom		aXML ;
	extern atom		aXML_aElement ;
	extern atom		aXML_aAttribute ;
	extern atom		atomine ;
	extern atom		aModuleAlpha ;
	extern void*		(*allocz)(unsigned) ;
	extern void		(*dealloc)(void*) ;
	extern void		(*interatomize)(atom,atom) ;
	extern void		(*intratomize)(atom,atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*aXML_modulize)(atom) ;
	extern atom		(*aXML_translatomize_XML)(atom) ;
	atom			r ;
	atom			a ;
	atom			m ;
	object			o ;
	string			s ;

	if( ( a = M->interatom ) )
		while( a->precursor != aModuleXML_serial && ( a = a->interatoms ) ) ;
	if( ! a )
		return aError_pUndefined() ;
	if( illogicated( r = repositoryRead_a( a->datum ) ) )
		return r ;
	m	= aXML_modulize( r ) ;
	if( --r->allusions == 0 )
		destruct( r ) ;
	if( illogicated( m ) )
		return m ;

	a	= m->interatom ;
	while( a && a->precursor != aXML )
		a = a->interatoms ;
	if( ! a )
		return aError_pUndefined() ;
	m	= a ;

	if( ( ! ( a = ((atom)m->interatom) ) ) || a->precursor != aXML_aElement || cmp_s( (string)a->interatom->datum , "module" ) != 0 )
		return aError_pUndefined() ;

	M->datum = o		= allocz( sizeof(struct _object) ) ;

	do	if( a->precursor == aXML )
			{
			s	= ( r = a->interatom )->interatom->datum ;
			while( r && r->precursor != aXML && r->precursor != aXML_aAttribute )
				r = r->interatoms ;
			if( ! r )
				continue ;
			if( cmp_s( "precursor" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->precursor = r ;
				}
			else
			if( cmp_s( "intramodule" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->intraobject = r ;
				}
			else
			if( cmp_s( "intramodules" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->intraobjects = r ; 
				}
			else
			if( cmp_s( "orbit" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->orbit = r ; 
				}
			else
			if( cmp_s( "intermodule" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->interobject = r ; 
				}
			else
			if( cmp_s( "intermodules" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->interobjects = r ; 
				}
			else
			if( cmp_s( "principal" , s ) == 0 )
				{
				if( illogicated( r = module_a( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				o->principal = r ; 
				}
			else
			if( cmp_s( "principle" , s ) == 0 )
				{
				extern atom aInteger ;
				unsigned p = 0 ;
				if( illogicated( r = aXML_translatomize_XML( r ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				if( r->precursor != aInteger )
					{
					if( --r->allusions == 0 )
						destruct( r ) ;
					aModuleXML_demorph_a( M ) ;
					return aError_pUndefined() ;
					}
				o->perms = ( unsigned short ) r->datum ;
				if( --r->allusions == 0 )
					destruct( r ) ;
				}
			else
			if( cmp_s( "variant" , s ) == 0 )
				{
				if( illogicated( r = aXML_translatomize_XML( a ) ) )	// UPDATE: aXML_translatomize_a_a( m , a )   where element of m has datatype of a
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				interatomize( r , M ) ;
				}
			else
			if( cmp_s( "method" , s ) == 0 )
				{
				if( illogicated( r = aXML_translatomize_XML( a ) ) )
					{
					aModuleXML_demorph_a( M ) ;
					return r ;
					}
				interatomize( r , M ) ;
				}
			}
		while( ( a = a->interatoms ) ) ;

	intratomize( M , aModuleAlpha ) ;

	return 0 ;
	}



atom aModuleXML_alphaSave( atom M )
	{
	extern atom		aProgram ;
	extern atom		aModuleXML_serial ;
	extern atom		(*aError_pUndefined)(void) ;
	extern atom		(*destruct)(atom) ;
	extern atom		(*aXML_atomize)(void) ;
	extern atom		(*aXML_atomize_a)(atom) ;
	extern atom		(*aXML_element_a_s)(atom,string) ;
//	extern atom		(*aXML_attribute_a_s_s)(atom,string,string) ;
	extern atom		(*aXML_data_a)(atom) ;
	extern atom		(*aXML_document_a)(atom) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*aAsciiString_canonicalize)(string) ;
	atom			a ;
	atom			s ;
	atom			x ;
//	atom			e ;
//	atom			aa ;
//	program			pptr ;
	atom			z ;


	s	= M->interatom ;
	while( s->precursor != aModuleXML_serial )
		s = s->interatoms ;
	if( ! s )
		return aError_pUndefined() ;

	if( illogicated( x = aXML_atomize_a( M ) ) )
		return x ;
	if( illogicated( a = aXML_document_a( x ) ) )
		{
		if( --x->allusions == 0 )
			destruct( x ) ;
		return a ;
		}
	if( ( z = repositoryWrite_a_a( s->datum , a ) ) )
		{
		if( --a->allusions == 0 )
			destruct( a ) ;
		if( --x->allusions == 0 )
			destruct( x ) ;
		return z ;
		}
	if( --a->allusions == 0 )
		destruct( a ) ;
	if( --x->allusions == 0 )
		destruct( x ) ;
	return atominul ;
	}