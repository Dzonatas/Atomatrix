/***************************************
*
* gro.c
*
*******************/


#include "ATOMATRIX.h"


atom			aVariant ;

atom _lib		ATOMATRIX_ATOMICAL ;
atom			aAtomatrix ;
void*			(*oAtomatrixFunction)(string) ;
atom			(*oAtomatrixAtomical)(string) ;
atom			(*oAtomical)(atom,string,atom) ;
atom			(*oFunction)(atom,string,void*) ;
atom			(*oDestructor)(atom,void*) ;
atom			(*oDuplicator)(atom,void*) ;
void			(*destruct)(atom) ;
atom			(*atomize)(atom) ;
void*			(*allocm)(unsigned int) ;
void*			(*allocz)(unsigned int) ;
void			(*dealloc)(void*) ;
void			(*interatomize)(atom,atom) ;
string			(*str_dup)(string) ;
void*			(*oLibraryFunction)(atom,string) ;
atom			(*oLibraryAtomical)(atom,string) ;
atom			(*duplicate)(atom) ;
atom			atomine ;
atom			aUndefined ;
atom			aIllogical ;
unsigned int		(*str_len)(string) ;
string			(*add_s)(string,string) ;
atom			aUnicode ;
atom			aUnicoded ;
atom			aAscii ;
atom			aAsciiString ;
atom			aAsciiCharacter ;
atom			aUnicodedString ;
atom			aUnicodedEntity ;
atom			(*aAsciiString_atomize)(string) ;

atom			aLibraryMachine ;
atom			aOprandEquals ;
atom			aOprandAddEquals ;
atom			aOprandAddEquate ;
atom			aOprandMinusEquals ;
atom			aOprandMinusEquate ;
atom			aOprandIndex ;
atom			aOprandPopLeft ;
atom			aOprandPopRight ;
atom			aOprandPull ;
atom			aOprandPushLeft ;
atom			aOprandPushRight ;
atom			aOprandSplice ;
atom			aOprandSplit ;
atom			aOprandEvaluate ;

atom			aInteger ;
atom			(*oInteger)(int);

atom			aVarray ;
atom			(*aVarray_atomize_a_n)(atom,unsigned) ;
atom			(*aVarray_append_a_a)(atom,atom) ;
atom			(*aVarrayReferent_atomize_a_n)(atom,unsigned) ;


atom			aLibraryError ;
atom			aError_TYPE ;
atom			aError_RANGE ;


void _lib preconstructor( atom A )
	{
	aAtomatrix		= ATOMATRIX_ATOMICAL ;
	oAtomatrixFunction	= aAtomatrix->datum ;
	oAtomatrixAtomical	= oAtomatrixFunction( "oAtomatrixAtomical" ) ;
	oAtomical		= oAtomatrixFunction( "oAtomical" ) ;
	atomine			= oAtomatrixAtomical( "atomine" ) ;
	atomize			= oAtomatrixFunction( "atomize" ) ;
	oAtomical( aAtomatrix , "aLibraryVariant" , A ) ;
#	define __(_1)	\
		oAtomical( A , #_1 , _1 = atomize( atomine ) )
	__( aVariant ) ;
	}


void _lib destructor( atom A )
	{
	destruct( aVariant ) ;
	}


void variant_destructor( atom A )
	{
	atom			a ;

	if( A == aVariant)
		return ;
	if( a = A->datum )
		if( --a->allusions == 0 )
			destruct( a ) ;
	A->datum = 0 ;
	}


atom variant_duplicator( atom A )
	{
	A->allusions++ ;
	return A ;
	}


atom oVariant( atom A , atom B )
	{
	atom			a ;

	a	= atomize( aVariant ) ;
	if( A )
		interatomize( A , a ) ;
	a->datum = B ;
	return a ;
	}


atom variant_oprandEquals( atom A , atom B )
	{
	atom			a ;

	if( a = A->datum )
		if( --a->allusions == 0 )
			destruct( a ) ;
	if( B )	{
//		B->allusions++ ;			// UPDATE: make "oprand<-oprand" statement allusify but not duplicate
		A->datum = duplicate( B ) ;
		}
	else
		A->datum = 0 ;
	A->allusions++ ;
	return A ;
	}


atom variant_oprandAddEquals( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		_asm int 3 ;
		aIllogical->allusions++ ;
		return aIllogical ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandAddEquate )
				{
				if( ( a = ((oprand)aa->datum)( A->datum , B ) ) == A->datum )
					{
					A->allusions++ ;
					return A ;
					}
				return a ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandMinusEquals( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		_asm int 3 ;
		aIllogical->allusions++ ;
		return aIllogical ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandMinusEquate )
				{
				if( ( a = ((oprand)aa->datum)( A->datum , B ) ) == A->datum )
					{
					A->allusions++ ;
					return A ;
					}
				return a ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandIndex( atom A , atom B )
	{
	typedef atom (*oprand)(atom,atom) ;
	atom	a ;
	atom	aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandIndex )
				return ((oprand)aa->datum)( A->datum , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandPopLeft( atom A )
	{
	typedef atom (*oprand)(atom) ;
	atom	a ;
	atom	aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopLeft )
				return ((oprand)aa->datum)( A->datum ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandPopRight( atom A )
	{
	typedef atom	(*oprand)(atom) ;
	atom	a ;
	atom	aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopRight )
				return ((oprand)aa->datum)( A->datum ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandPull( atom A , atom B )
	{
	typedef atom	(*oprand)(atom,atom) ;
	atom	a ;
	atom	aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPull )
				return ((oprand)aa->datum)( A->datum , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandPushLeft( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPushLeft )
				{
				((oprand)aa->datum)( A->datum , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandPushRight( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPushRight )
				{
				((oprand)aa->datum)( A->datum , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandSplice( atom A , atom B , atom C )
	{
	typedef atom		(*oprand)(atom,atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplice )
				return ((oprand)aa->datum)( A->datum , B , C ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandSplit( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplit )
				return ((oprand)aa->datum)( A->datum , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom variant_oprandEvaluate( atom A )
	{
	typedef atom		(*oprand)(atom) ;
	atom			a ;
	atom			aa ;

	if( ! ( a = A->datum ) )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandEvaluate )
				return ((oprand)aa->datum)( A->datum ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	( a = A->datum )->allusions++ ;
	return a ;
	}


void _lib constructor( atom A )
	{
	atom			_a ;

#	undef __
#	define __(_1)		_1 = oAtomatrixAtomical( #_1 )
#	define ___(_1)		_1 = oAtomatrixFunction( #_1 )
	__( aUndefined		) ;
	__( aIllogical		) ;
	___( oFunction		) ;
	___( oDestructor		) ;
	___( oDuplicator		) ;
	___( interatomize		) ;
	___( allocm			) ;
	___( allocz			) ;
	___( dealloc			) ;
	___( destruct		) ;
	___( str_dup			) ;
	___( oLibraryFunction	) ;
	___( oLibraryAtomical	) ;
	___( duplicate		) ;
	___( str_len			) ;
	___( add_s			) ;
	__( aInteger		) ;
	__( aUnicode ) ;
#	undef __
#	undef ___
#	define _(_1)		_a = _1 = oAtomatrixAtomical( #_1 )
#	define __(_1)		_1 = oLibraryAtomical( _a , #_1 )
#	define ___(_1)		_1 = oLibraryFunction( _a , #_1 )
	_( aLibraryMachine ) ;
		__( aOprandEquals ) ;
		__( aOprandAddEquals ) ;
		__( aOprandAddEquate ) ;
		__( aOprandMinusEquals ) ;
		__( aOprandMinusEquate ) ;
		__( aOprandIndex ) ;
		__( aOprandPopLeft ) ;
		__( aOprandPopRight ) ;
		__( aOprandPull ) ;
		__( aOprandPushLeft ) ;
		__( aOprandPushRight ) ;
		__( aOprandSplice ) ;
		__( aOprandSplit ) ;
		__( aOprandEvaluate ) ;
	_( aLibraryError ) ;
		__( aError_TYPE   		) ;
		__( aError_RANGE		) ;
	oDestructor( aVariant , variant_destructor ) ;
	oDuplicator( aVariant , variant_duplicator ) ;
	oFunction( A , "oVariant"	, oVariant ) ;
#	undef __
#	define __(_1)	\
		(_a=atomize( aOprand##_1 ))->datum=variant_oprand##_1 ; \
		interatomize( _a , aVariant ) ;
	__( Equals ) ;
	__( AddEquals ) ;
	__( MinusEquals ) ;
	__( Index ) ;
	__( PopLeft ) ;
	__( PopRight ) ;
	__( Pull ) ;
	__( PushLeft ) ;
	__( PushRight ) ;
	__( Splice ) ;
	__( Split ) ;
	__( Evaluate ) ;

	aVarray			= oAtomatrixAtomical( "aVarray" ) ;
	aVarray_atomize_a_n	= oLibraryFunction( aVarray , "atomize_a_n" ) ;
	aVarray_append_a_a	= oLibraryFunction( aVarray , "append_a_a" ) ;
	aVarrayReferent_atomize_a_n	= oLibraryFunction( oLibraryAtomical( aVarray , "referent" ) , "atomize_a_n" ) ;

	aAscii		= oLibraryAtomical( aUnicode , "ascii" ) ;
	aUnicoded	= oLibraryAtomical( aUnicode , "unicoded" ) ;
	aAsciiString	= oLibraryAtomical( aAscii , "string" ) ;
	aAsciiCharacter	= oLibraryAtomical( aAscii , "character" ) ;
	aUnicodedString	= oLibraryAtomical( aUnicoded , "string" ) ;
	aUnicodedEntity	= oLibraryAtomical( aUnicoded , "entity" ) ;
	oInteger	= oLibraryFunction( aInteger , "atomize" ) ;
	aAsciiString_atomize = oLibraryFunction( aAsciiString , "atomize" ) ;
	}

