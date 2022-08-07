/***************************************
* 
* gro.c
* 
*******************/


#include "ATOMATRIX.h"


atom			aError ;
atom			aError_VARNF ;
atom			aError_RANGE ;
atom			aError_TYPE ;
atom			aError_ARGC ;
atom			aError_INVOBJ ;
atom			aError_VERBNF ;
atom			aError_INVARG ;
atom			aError_EXISTS ;
atom			aError_PROGNF ;
atom			aError_COMP ;
atom			aError_RECR ;
atom			aError_PERM ;
atom			aError_CALL_FAILED ;
atom			aError_aUndefined ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
atom			atomine ;
atom			aIllogical ;
void*			(*oAtomatrixFunction)(string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oAtomical)(atom,string,atom) ;
atom			(*oFunction)(atom,string,void*) ;
atom			(*oDestructor)(atom,void*) ;
void			(*destruct)(atom) ;
atom			(*atomize)(atom) ;
void*			(*allocm)(unsigned) ;
void*			(*allocr)(void*,unsigned) ;
void			(*dealloc)(void*) ;
void			(*interatomize)(atom,atom) ;
void			(*intratomize)(atom,atom) ;
atom			(*atomethodize)(atom,string,atom(*)(atom,unsigned,atom*));
void			(*output)(string) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oLibraryAtomical)(atom,string) ;
atom			(*aAsciiString_atomize)(string) ;



void _lib preconstructor( atom A )
	{
	extern atom		oError(string);
	extern atom		aError_pUndefined(void) ;		// UPDATE: aError_undefined
	extern atom		aError_atomize_a(atom) ;

	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	oFunction		= oAtomatrixFunction( "oFunction" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	oAtomical( aAtomatrix , "aLibraryError" , A ) ;
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( atomine ) )
	__( aError ) ;
#	undef __
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( aError ) )
	__( aError_VARNF ) ;
	__( aError_RANGE ) ;
	__( aError_TYPE ) ;
	__( aError_ARGC ) ;
	__( aError_INVOBJ ) ;
	__( aError_VERBNF ) ;
	__( aError_INVARG ) ;
	__( aError_EXISTS ) ;
	__( aError_PROGNF ) ;
	__( aError_COMP ) ;
	__( aError_RECR ) ;
	__( aError_PERM ) ;
	__( aError_CALL_FAILED ) ;
	__( aError_aUndefined ) ;
	oFunction( A , "oError"			, oError ) ;
	oFunction( A , "aError_pUndefined"	, aError_pUndefined ) ;		// UPDATE: onto aError, not A
	}


void _lib constructor( atom A )
	{
	extern atom		aError_type(atom,unsigned,atom*);

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aIllogical ) ;
	___( oDestructor ) ;
	___( interatomize ) ;
	___( intratomize ) ;
	___( allocm ) ;
	___( allocr ) ;
	___( dealloc ) ;
	___( destruct ) ;
	___( atomethodize ) ;
	___( output ) ;
	___( oLibraryFunction ) ;
	___( oLibraryAtomical ) ;
	aAsciiString_atomize = oLibraryFunction( oLibraryAtomical( oLibraryAtomical( oAtomatrixAtomical( "aUnicode" ) , "ascii" ) , "string" ) , "atomize" ) ;
	intratomize( aError , aIllogical ) ;
	atomethodize( aError , "type"	,	aError_type ) ;
	oFunction( aError , "atomize_a"	, aError_atomize_a ) ;
	}


void _lib destructor( atom A )
	{
	destruct( aError ) ;
	}