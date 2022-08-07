// aVarrayReferent.c		Copyright (c) 2004 BALLARD


#include "ATOMATRIX.h"
#include "varray.h"

atom			aVarrayReferent ;


atom aVarrayReferent_atomize_a_n( atom A , unsigned I )
	{
	extern atom		aVarray ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;
	atom*			v ;

#	ifdef _DEBUG
	if( A->precursor != aVarray )
		_asm int 3 ;

#	endif
	v	= ((varray)A->datum)->ary + I ;
	a	= A->interatom ;
	while( a )
		{
		if( a->precursor == aVarrayReferent && a->datum == v )
			{
			a->allusions++ ;
			return a ;
			}
		a = a->interatoms ;
		}
	a = atomize( aVarrayReferent ) ;
	a->datum = v ;
//	(*v)->allusions++ ;
	interatomize( a , A ) ;
	return a ;
	}


atom aVarrayReferent_pregeneration( void )
	{
	extern atom		atomine ;
	extern atom		atomize(atom) ;
	extern atom		oFunction(atom,string,void*) ;

	aVarrayReferent	= atomize( atomine ) ;
	oFunction( aVarrayReferent , "atomize_a_n",	aVarrayReferent_atomize_a_n ) ;
	return aVarrayReferent ;
	}


atom aVarrayReferent_pCondition( atom A )
	{
	extern atom		aOprandCondition ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	typedef atom		(*oprand)(atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
#ifdef DEBUG
	if( A == *tv ) _asm int 3 ;
#endif
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandCondition )
				return ((oprand)aa->datum)( *tv ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom aVarrayReferent_pEquals( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern void		destruct(atom) ;
	extern atom		duplicate(atom) ;
	atom*			tv ;

	
	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( *( tv = (atom*)A->datum ) == aUndefined )
		aUndefined->allusions-- ;
	else
	if( --(*tv)->allusions == 0 )
		destruct( *tv ) ;
	*tv	= duplicate( B ) ;
	A->allusions++ ;
	return A ;
	}

/*
atom aVarrayReferent_pAddEquals( atom A , atom B )
	{
	_asm int 3 ;
	}

atom aVarrayReferent_pMinusEquals( atom A , atom B )
	{
	_asm int 3 ;
	}
*/

atom aVarrayReferent_pIndex( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandIndex ;
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandIndex )
				return ((oprand)aa->datum)( *tv , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom aVarrayReferent_pPopLeft( atom A )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandPopLeft ;
	typedef atom		(*oprand)(atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopLeft )
				return ((oprand)aa->datum)( *tv ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom aVarrayReferent_pPopRight( atom A )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandPopRight ;
	typedef atom		(*oprand)(atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopRight )
				return ((oprand)aa->datum)( *tv ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom aVarrayReferent_pPull( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandPull ;
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPull )
				return ((oprand)aa->datum)( *tv , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom aVarrayReferent_pPushLeft( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandPushLeft ;
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPushLeft )
				{
				((oprand)aa->datum)( *tv , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom aVarrayReferent_pPushRight( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandPushRight ;
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPushRight )
				{
				((oprand)aa->datum)( *tv , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom aVarrayReferent_pSplice( atom A , atom B , atom C )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandSplice ;
	typedef atom		(*oprand)(atom,atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplice )
				return ((oprand)aa->datum)( *tv , B , C ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom aVarrayReferent_pSplit( atom A , atom B )
	{
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandSplit ;
	typedef atom		(*oprand)(atom,atom) ;
	atom			a ;
	atom			aa ;
	atom*			tv ;

	if( ! A->orbit )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( ( a = *( tv = (atom*)A->datum ) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplit )
				return ((oprand)aa->datum)( *tv , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom aVarrayReferent_pEvaluate( atom A )
	{
	extern atom		aUndefined ;
	atom			a ;

	if( A->orbit )
		a = *(atom*)A->datum ;
	else
		a = aUndefined ;
	a->allusions++ ;
	return a ;
	}


void aVarrayReferent_constructent( void )
	{

#	define __(_1)	\
		{	\
		extern atom aOprand##_1 ; \
		atom	_a ; \
		(_a=atomize( aOprand##_1 ))->datum=aVarrayReferent_p##_1 ; \
		interatomize( _a , aVarrayReferent ) ; \
		}
	__( Condition ) ;
	__( Equals ) ;
//	__( AddEquals ) ;
//	__( MinusEquals ) ;
	__( Index ) ;
	__( PopLeft ) ;
	__( PopRight ) ;
	__( Pull ) ;
	__( PushLeft ) ;
	__( PushRight ) ;
	__( Splice ) ;
	__( Split ) ;
	__( Evaluate ) ;
	}


void aVarrayReferent_destructor( void )
	{
	extern void		destruct(atom) ;

	destruct( aVarrayReferent ) ;
	}


