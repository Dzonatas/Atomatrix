/***************************************
*
* gro.c
*
*******************/

#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"

atom			aVariable ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
void*			(*oAtomatrixFunction)(string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oAtomical)(atom,string,atom) ;
atom			(*oFunction)(atom,string,void*) ;
atom			(*atomize)(atom) ;
void			(*destruct)(atom) ;
void			(*interatomize)(atom,atom) ;
atom			(*atomethodize)(atom,string,atom(*)(atom,int,atom*));
atom			(*oDestructor)(atom,void*) ;
void			(*dealloc)(void*) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oLibraryAtomical)(atom,string) ;
string			(*str_dup)(string) ;
void*			(*allocz)(unsigned int) ;
atom			atomine ;
atom			aIllogical ;
atom			aUndefined ;
atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString ;
atom			aAsciiCharacter ;
atom			aUnicodedString ;
atom			aUnicodedEntity ;
atom			(*aAsciiString_atomize)(string) ;
atom			aInteger ;
atom			(*oInteger)(int) ;


void _lib preconstructor( atom A )
	{
	extern atom		addAtom(atom,string,atom);
	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	oFunction		= oAtomatrixFunction( "oFunction" ) ;
	oAtomical( aAtomatrix , "aLibraryVariable" , A ) ;
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( atomine ) )
	__( aVariable ) ;
	oFunction( aVariable , "addAtom"	, addAtom ) ;
	}


void _lib destructor( atom A )
	{
	destruct( aVariable ) ;
	}


void variable_destructor( atom A )
	{
	variable	v	= A->datum ;
	if( v )
		{
		if( v->name )
			dealloc( v->name ) ;
		if( v->v.Type == v_ATOM )
			if( v->v.v.Object )
				if( --v->v.v.Object->allusions == 0 )
					destruct( v->v.v.Object ) ;
		dealloc( v ) ;
		}
	A->datum = 0 ;
	}


atom variable_oprandEvaluate( atom A )
	{
	variable v = A->datum;

	switch(v->v.Type) {
		case v_NOTHING:
			aUndefined->allusions++ ;
			return aUndefined ;
		case v_ATOM:
			if( v->v.v.Object )
				{
				v->v.v.Object->allusions++ ;
				return v->v.v.Object ;
				}
			aUndefined->allusions++ ;
			return aUndefined ;
		case v_INTEGER:
			return oInteger( v->v.v.I ) ;
		case v_WORD:
			return oInteger( v->v.v.Word ) ;
		case v_BYTE:
			return oInteger( v->v.v.Byte ) ;
		case v_LONG:
			return oInteger( v->v.v.Long ) ;
		case v_ASCIIZ:
			return aAsciiString_atomize( str_dup( v->v.v.Ptr ) );
			
		case v_ASCII:
			{
			string	s ;
			*( s = (char *)allocz( 2 ) )	= v->v.v.Byte ;
			return aAsciiString_atomize( s ) ;
			}
		}
	_asm int 3 ;
	aUndefined->allusions++ ;
	return aUndefined ;
	}


void _lib constructor( atom A )
	{
	extern atom		getAtom(atom,string,atom*);
	extern atom		addAtom(atom,string,atom);
	extern atom		addByte(atom,string,unsigned char);
	extern atom		getByte(atom,string,unsigned char *);
	extern atom		addWord(atom,string,unsigned short);
	extern atom		getWord(atom,string,unsigned short *);
	extern atom		addLong(atom,string,unsigned long);
	extern atom		getLong(atom,string,unsigned long *);
	extern atom		addInteger(atom,string,int);
	extern atom		getInteger(atom,string,int *);
	extern atom		addASCII(atom,string,char);
	extern atom		getASCII(atom,string,char*);
	extern atom		addVariant(atom,string,variant);
	extern atom		getVariable(atom,string,variable*);
	extern atom		getVariable_A_S(atom,string);
	extern atom		o_Set(atom,int,atom*);
	atom			_a ;

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aIllogical ) ;
	__( aUndefined ) ;
	___( interatomize ) ;
	___( oDestructor ) ;
	___( atomethodize ) ;
	___( dealloc ) ;
	___( destruct ) ;
	___( oLibraryFunction ) ;
	___( oLibraryAtomical ) ;
	___( str_dup ) ;
	___( allocz ) ;
	__( aInteger ) ;
	__( aUnicode ) ;
#	undef __
#	undef ___
#	define _(_1)		_a = _1 = oAtomatrixAtomical( #_1 )
#	define __(_1)		_1 = oLibraryAtomical( _a , #_1 )
#	define ___(_1)		_1 = oLibraryFunction( _a , #_1 )
	oDestructor( aVariable , variable_destructor ) ;
	atomethodize( aVariable, "Set",		o_Set);
	oFunction( A , "addByte"	, addByte ) ;
	oFunction( A , "getByte"	, getByte ) ;
	oFunction( A , "addWord"	, addWord ) ;
	oFunction( A , "getWord"	, getWord ) ;
	oFunction( A , "addLong"	, addLong ) ;
	oFunction( A , "getLong"	, getLong ) ;
	oFunction( A , "addInteger"	, addInteger ) ;
	oFunction( A , "getInteger"	, getInteger ) ;
	oFunction( A , "addASCII"	, addASCII ) ;
	oFunction( A , "getASCII"	, getASCII ) ;
	oFunction( A , "addAtom"	, addAtom ) ;
	oFunction( A , "getAtom"	, getAtom ) ;
	oFunction( A , "addVariant"	, addVariant ) ;
	oFunction( A , "getVariable"	, getVariable ) ;
	oFunction( A , "getVariable_A_S" , getVariable_A_S ) ;
#	undef __
#	define __(_1)	\
		(_a=atomize( oLibraryAtomical( oAtomatrixAtomical( "aLibraryMachine" ) , "aOprand" #_1 ) ))->datum=variable_oprand##_1 ; \
		interatomize( _a , aVariable ) ;
	__( Evaluate ) ;
	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize = oLibraryFunction( aAsciiString , "atomize" ) ;
	}
