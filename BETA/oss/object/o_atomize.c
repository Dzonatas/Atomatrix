/***************************************
*
* o_atomize.c
*
*******************/


#include "ATOMATRIX.h"
#include "unicode.h"
#include "object.h"


atom o_atomize( atom A , unsigned T , atom* V )
	{
	extern atom		aModuleAlpha ;
	extern atom		aError_INVARG ;
	extern atom		aUnicodedString ;
	extern atom		aUnicodedEntity ;
	extern atom		aAsciiString ;
	extern atom		aAsciiCharacter ;
	extern atom		(*atomize)(atom) ;
	extern void		(*dealloc)(void*) ;
	extern void*		(*allocz)(unsigned) ;
	extern string		(*aUnicodedString_canonicalize)(unicodes) ;
	extern atom		aModuleXML_atomize_a(atom) ;
	atom			a ;

	if( ! T )
		{
		a		= atomize( aModuleAlpha ) ;
		a->datum	= allocz( sizeof( struct _object ) ) ;
		return a ;
		}
	if( T > 1 )
		{
		aError_INVARG->allusions++ ;
		return aError_INVARG ;
		}
	return aModuleXML_atomize_a( *V ) ;
	}