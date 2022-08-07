/***************************************
*
* oError.c
*
*******************/


#include "ATOMATRIX.h"

atom			aLibraryVariable ;
atom			(*addAtom)(atom,string,atom) ;


atom aError_atomize_a( atom A )
	{
	extern atom		aError ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oAtomatrixAtomical)(string) ;
	extern void*		(*oLibraryFunction)(atom,string) ;
	atom 			a ;

	a	= atomize( aError ) ;
	if( ! addAtom )	
		{
		aLibraryVariable	= oAtomatrixAtomical( "aLibraryVariable" ) ;		// UPDATE: move to structor.c
		addAtom			= oLibraryFunction( aLibraryVariable , "addAtom" ) ;
		}
	addAtom( a , "message" , A ) ;
	A->allusions++ ;
	return a ;
	}



atom oError( string s )
	{
	extern atom		aError ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*oAtomatrixAtomical)(string) ;
	extern void*		(*oLibraryFunction)(atom,string) ;
	extern atom		(*aAsciiString_atomize)(string) ;
	atom 			a ;

	a	= atomize( aError ) ;
	if( ! addAtom )				// UPDATE:
		{
		aLibraryVariable	= oAtomatrixAtomical( "aLibraryVariable" ) ;
		addAtom			= oLibraryFunction( aLibraryVariable , "addAtom" ) ;
		}
	addAtom( a , "message" , aAsciiString_atomize( s ) ) ;
	return a ;
	}


atom aError_pUndefined( void )
	{
	extern atom		aError_aUndefined ;
	aError_aUndefined->allusions++ ;
	return aError_aUndefined ;
	}