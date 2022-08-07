/***********************************************
*
* structor.c
*
*************************/


#include "ATOMATRIX.h"


atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
atom			(*oAtomatrixAtomical)(string) ;
void*			(*oAtomatrixFunction)(string) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*atomethodize)(atom,string,atom(*)(atom,int,atom*));
void*			(*allocm)(unsigned int);
void*			(*allocr)(void*,unsigned int);
void			(*dealloc)(void) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oLibraryAtomical)(atom,string) ;
void*			(*taskSplice)(void*,void*) ;
string			(*str_cpy)(string,string);
string			(*str_cat)(string,string);
unsigned int		(*str_len)(string);
void			(*aAtomatrixSeriestring)(void) ;
atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString ;
atom			aAsciiCharacter ;
atom			aUnicodedEntity ;
atom			aUnicodedString ;
atom			(*aAsciiString_atomize)(string) ;
atom			aInteger ;
atom			(*oInteger)(int) ;

atom			aLibraryError ;
atom			aError;
atom    		aError_VARNF;
atom    		aError_RANGE;
atom    		aError_TYPE;
atom    		aError_ARGC;
atom    		aError_INVOBJ;
atom    		aError_VERBNF;
atom    		aError_INVARG;
atom    		aError_EXISTS;
atom    		aError_PROGNF;
atom    		aError_COMP;
atom    		aError_RECR;
atom    		aError_PERM;
atom    		aError_CALL_FAILED;


void _lib constructor( atom A )
	{
	extern atom		o_get(atom,int,atom*);

	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	atomethodize		= oAtomatrixFunction( "atomethodize" ) ;
	allocm			= oAtomatrixFunction( "allocm" ) ;
	allocr			= oAtomatrixFunction( "allocr" ) ;
	dealloc			= oAtomatrixFunction( "dealloc" ) ;
	oLibraryFunction	= oAtomatrixFunction( "oLibraryFunction" ) ;
	oLibraryAtomical	= oAtomatrixFunction( "oLibraryAtomical" ) ;
	taskSplice		= oAtomatrixFunction( "taskSplice" ) ;
	str_cpy			= oAtomatrixFunction( "str_cpy" ) ;
	str_len			= oAtomatrixFunction( "str_len" ) ;
	str_cat			= oAtomatrixFunction( "str_cat" ) ;
	aInteger		= oAtomatrixAtomical( "aInteger" ) ;
	aUnicode		= oAtomatrixAtomical( "aUnicode" ) ;

	aLibraryError		= oAtomatrixAtomical( "aLibraryError" ) ;
	aError_ARGC    		= oLibraryAtomical( aLibraryError , "aError_ARGC" ) ;
	aError_INVARG  		= oLibraryAtomical( aLibraryError , "aError_INVARG" ) ;

	atomethodize( A ,	"get" ,		o_get ) ;

	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize = oLibraryFunction( aAsciiString , "atomize" ) ;
	}


void _lib destructor( atom A )
	{
	return ;
	}
