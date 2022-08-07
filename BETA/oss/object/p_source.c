/***************************************
*
* p_source.c
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include "machine/executable.h"
#include "program.h"


atom p_source( atom A , int T , atom* V )
	{
	extern atom		aUnicodedString ;
	extern atom		aAsciiString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiCharacter ;
	extern atom		aProgram ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		(*aAsciiString_atomize)(string) ;
	extern string		(*aUnicodedString_canonicalize)(unicodes) ;
	extern string		(*str_dup)(string) ;
	extern void		(*dealloc)(void*) ;
	extern void*		(*allocz)(unsigned) ;
	program			p ;
	atom			s ;

	if( A->precursor != aProgram )
		{
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( ! T )
		return aAsciiString_atomize( str_dup( ((program)A->datum)->psrc ) ) ;
	if( T != 1 )
		{
		aError_ARGC->allusions++ ;
		return aError_ARGC ;
		}
	if( ( s = V[0] )->precursor == aAsciiString )
		{
		p	= (program) A->datum ;		// UPDATE: verify security
		if( p->psrc )
			dealloc( p->psrc ) ;
		p->psrc	= str_dup( s->datum ) ;
		if( p->exep )
			{
			p->exep->prog = 0 ;
			p->exep = 0 ;
			}
		s->allusions++ ;
		return s ;
		}
	if( s->precursor == aAsciiCharacter )
		{
		p	= (program) A->datum ;		// UPDATE: verify security
		if( p->psrc )
			dealloc( p->psrc ) ;
		p->psrc	= allocz( 2 ) ;
		*p->psrc= (byte)s->datum ;
		if( p->exep )
			{
			p->exep->prog = 0 ;
			p->exep = 0 ;
			}
		s->allusions++ ;
		return s ;
		}
	if( ( s = V[0] )->precursor == aUnicodedString )
		{
		string	ss = aUnicodedString_canonicalize( (unicodes)s->datum ) ;
		p	= (program) A->datum ;		// UPDATE: verify security
		if( p->psrc )
			dealloc( p->psrc ) ;
		p->psrc	= ss ;
		if( p->exep )
			{
			p->exep->prog = 0 ;
			p->exep = 0 ;
			}
		return aAsciiString_atomize( ss ) ;
		}
	if( ( s = V[0] )->precursor == aUnicodedString )
		{
		string	ss ;
		unicode	u[2] ;
		u[0]	= (unicode)s->datum ;
		u[0]	= 0 ;
		ss = aUnicodedString_canonicalize( u ) ;	// UPDATE: use aUnicodedEntity_canonicalize( (unicode)s->datum )
		p	= (program) A->datum ;		// UPDATE: verify security
		if( p->psrc )
			dealloc( p->psrc ) ;
		p->psrc	= ss ;
		if( p->exep )
			{
			p->exep->prog = 0 ;
			p->exep = 0 ;
			}
		return aAsciiString_atomize( ss ) ;
		}
	aError_INVARG->allusions++ ;
	return aError_INVARG ;
	}
