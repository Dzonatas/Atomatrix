/***************************************
*
* structor.c
*
*******************/

#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"


atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
atom			aUndefined ;
void*			(*oAtomatrixFunction)(string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oAtomical)(atom,string,atom) ;
atom			(*oFunction)(atom,string,void*) ;
atom			(*atomize)(atom) ;
void			(*destruct)(atom) ;
void			(*interatomize)(atom,atom) ;
atom			(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
atom			(*oDestructor)(atom,void*) ;
atom			(*oDuplicator)(atom,void*) ;
void			(*output)(string) ;
void*			(*allocz)(unsigned) ;
void*			(*allocm)(unsigned) ;
void*			(*allocr)(void*,unsigned) ;
void			(*dealloc)(void*) ;
string			(*str_dup)(string) ;
int			(*str_len)(string) ;
atom			(*oLibraryAtomical)(atom,string) ;
void*			(*oLibraryFunction)(atom,string) ;
string			(*add_s)(string,string) ;
atom			(*addAtom)(atom,string,atom) ;
void*			(*taskSplice)(void*,void*) ;
atom			atomine ;
atom			aInteger ;
atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString ;
atom			aAsciiCharacter ;
atom			aUnicodedEntity ;
atom			aUnicodedString ;
atom			(*aAsciiString_atomize)(string) ;

atom			aLibraryMachine ;
atom			aOprandAdd ;
atom			aOprandAddEquate ;
atom			aOprandCondition ;
atom			aOprandCompareEqual ;
atom			aOprandIndex ;
atom			aOprandSplice ;
atom			aEquality ;
atom			(*oIndexInteger)(int) ;

atom			aVarray;
atom			(*aVarray_atomize_a_n)(atom,int);
atom			(*aVarray_append_a_a)(atom,atom);

atom			(*oInteger)(int) ;

atom			aLibraryError ;
atom			aError_RANGE ;
atom			aError_TYPE ;
atom			aError_ARGC ;
atom			aError_INVARG ;
atom			aError_INVOBJ ;

atom			aLibraryVariable ;
atom			(*addAtom)(atom,string,atom) ;
atom			(*getVariable)(atom,string,variable*) ;



void _lib preconstructor( atom A )
	{
	extern void		aWND_preconstructent(atom) ;

	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	oFunction		= oAtomatrixFunction( "oFunction" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	aWND_preconstructent( A ) ;
	}


void _lib destructor( atom A )
	{
	extern void		aWND_destructent(atom) ;
	
	aWND_destructent( A ) ;
	}



void _lib constructor( atom A )
	{
	extern void		aWND_constructent(atom) ;
	atom			_a ;

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aUndefined ) ;
	___( oDestructor ) ;
	___( oDuplicator ) ;
	___( interatomize ) ;
	___( atomethodize ) ;
	___( output ) ;
	___( allocz ) ;
	___( allocm ) ;
	___( allocr ) ;
 	___( dealloc ) ;
	___( destruct ) ;
	___( str_dup ) ;
	___( str_len ) ;
	___( oLibraryAtomical ) ;
	___( oLibraryFunction ) ;
	___( add_s ) ;
	___( taskSplice ) ;
	__( aInteger ) ;
	__( aUnicode ) ;
#	undef __
#	undef ___
#	define _(_1)		_a = _1 = oAtomatrixAtomical( #_1 )
#	define __(_1)		_1 = oLibraryAtomical( _a , #_1 )
#	define ___(_1)		_1 = oLibraryFunction( _a , #_1 )
	_( aLibraryMachine ) ;
		__( aEquality ) ;
		__( aOprandAdd ) ;
		__( aOprandAddEquate ) ;
		__( aOprandCondition ) ;
		__( aOprandCompareEqual ) ;
		__( aOprandIndex ) ;
		__( aOprandSplice ) ;
		___( oIndexInteger ) ;
	_( aLibraryError ) ;
		__( aError_RANGE ) ;
		__( aError_TYPE ) ;
		__( aError_ARGC ) ;
		__( aError_INVARG ) ;
		__( aError_INVOBJ ) ;
	_( aLibraryVariable ) ;
		___( addAtom ) ;
		___( getVariable ) ;

	aVarray			= oAtomatrixAtomical( "aVarray" ) ;
	aVarray_atomize_a_n	= oLibraryFunction( aVarray , "atomize_a_n" ) ;
	aVarray_append_a_a	= oLibraryFunction( aVarray , "append_a_a" ) ;

	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize	= oLibraryFunction( aAsciiString , "atomize" ) ;
	aWND_constructent( A ) ;
	}
