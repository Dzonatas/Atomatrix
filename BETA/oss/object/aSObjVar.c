/***************************************
*
* aSObjVar.c
*
*******************/


#include "ATOMATRIX.h"
#include "objvar.h"
#include "sobjvar.h"
#include "varray.h"

atom				aSObjVar ;


atom oSObjVar( objvar V )
	{
	extern void *		(*allocz)(unsigned) ;
	extern atom		(*atomize)(atom) ;
	atom			a ;
	sobjvar			v ;

	v		= allocz( sizeof(struct _sobjvar) ) ;
	a		= atomize( aSObjVar ) ;
	a->datum	= v ;
	v->ov		= V;
	v->v		= &V->v;
	return a ;
	}


void sobjvar_destructor( atom A )
	{
	extern void		(*dealloc)(void*) ;

	if( A != aSObjVar )
		dealloc( A->datum ) ;
	A->datum	= 0 ;
	}


atom sobjvar_oprandEquals( atom A , atom B )
	{
	extern atom		aUndefined ;
	extern atom		aError_PERM ;
	extern atom		(*duplicate)(atom) ;
	extern void		(*destruct)(atom) ;
	extern int		p_write(atom) ;
	atom*			v ;

	if( ! p_write( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	v	= ((sobjvar)A->datum)->v ;
	if( *v == aUndefined )
		aUndefined->allusions-- ;
	else
	if( --(*v)->allusions == 0 )
		destruct( *v ) ;
	*v	= duplicate( B ) ;
	A->allusions++ ;
	return A ;
	}


atom sobjvar_oprandAddEquals( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern atom		aError_PERM ;
	extern atom		aOprandAddEquate ;
	extern int		p_rw(atom) ;
	atom*			v ;
	atom			a ;
	atom			aa ;

	if( ! p_rw( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	a	= *(v=((sobjvar)A->datum)->v) ;
	if( a == aUndefined )
		{
#ifdef DEBUG
		_asm int 3 ;					// UPDATE
#endif
		aIllogical->allusions++ ;
		return aIllogical ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandAddEquate )
				{
				if( ( a = ((oprand)aa->datum)( *v , B ) ) == *v )
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


atom sobjvar_oprandMinusEquals( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aOprandMinusEquate ;
	extern int		p_write(atom) ;
	atom*			v ;
	atom			a ;
	atom			aa ;

	if( ! p_rw( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	a	= *(v=((sobjvar)A->datum)->v) ;
	if( a == aUndefined )
		{
#ifdef DEBUG
		_asm int 3 ;					// UPDATE
#endif
		aIllogical->allusions++ ;
		return aIllogical ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandMinusEquate )
				{
				if( ( a = ((oprand)aa->datum)( *v , B ) ) == *v )
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


atom sobjvar_oprandEvaluate( atom A )
	{
	extern atom		aError_PERM ;
	extern int		p_read(atom) ;
	atom			a ;

	if( ! p_read( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	( a = *((sobjvar)A->datum)->v )->allusions++ ;
	return a ;
	}


atom sobjvar_oprandIndex( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aError_PERM ;
	extern atom		aError_RANGE ;
	extern atom		aIllogical ;
	extern atom		aOprandIndex ;
	extern atom		aVarray ;
	sobjvar			sov ;
	atom	a ;
	atom	aa ;

	if( ! p_read( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	if( a->precursor == aVarray )			// UPDATE: use aOprandIndex
		{
		varray		v ;
		unsigned int	i ;
		v	= a->datum ;
		i	= (unsigned int)B->datum ;
		if( i < v->nelem )
			{
			// UPDATE: use oSObjVar()
			sov->v	= v->ary + i ;
			A->allusions++ ;
			return A ;
			}
		aError_RANGE->allusions++ ;
		return aError_RANGE ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandIndex )
				return ((oprand)aa->datum)( *(sov->v) , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom sobjvar_oprandSplice( atom A , atom B , atom C )
	{
	typedef atom		(*oprand)(atom,atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aOprandSplice ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_read( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
		{
		aUndefined->allusions++ ;
		return aUndefined ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplice )
				return ((oprand)aa->datum)( *sov->v , B , C ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


atom sobjvar_oprandPopLeft( atom A )
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aError_PERM ;
	extern atom		aOprandPopLeft ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_rw( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopLeft )
				return ((oprand)aa->datum)( *sov->v ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom sobjvar_oprandPopRight( atom A )
	{
	typedef atom		(*oprand)(atom) ;
	extern atom		aOprandPopRight ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_rw( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPopRight )
				return ((oprand)aa->datum)( *sov->v ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom sobjvar_oprandPull( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandPull ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_rw( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
		{
		a->allusions++ ;
		return a ;
		}
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPull )
				return ((oprand)aa->datum)( *sov->v , B ) ;
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom sobjvar_oprandPushLeft( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandPushLeft ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_write( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
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
				((oprand)aa->datum)( *sov->v , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}

atom sobjvar_oprandPushRight( atom A , atom B )
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aOprandPushRight ;
	extern atom		aError_PERM ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	sobjvar			sov ;
	atom			a ;
	atom			aa ;

	if( ! p_write( A ) )
		{
		aError_PERM->allusions++ ;
		return aError_PERM ;
		}
	if( ( a = *((sov=(sobjvar)A->datum)->v) ) == aUndefined )
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
				((oprand)aa->datum)( *sov->v , B ) ;
				A->allusions++ ;
				return A ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	aIllogical->allusions++ ;
	return aIllogical ;
	}


void aSObjVar_pConstructent( atom A )
	{
	atom			a ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern atom		(*oDestructor)(atom,void*) ;

	oDestructor( aSObjVar , sobjvar_destructor ) ;

#	undef __
#	define __(_1)	\
		{ \
		extern atom aOprand##_1 ; \
		(a=atomize( aOprand##_1 ))->datum=sobjvar_oprand##_1 ; \
		interatomize( a , aSObjVar ) ; \
		}
	__( Equals ) ;
	__( AddEquals ) ;
	__( MinusEquals ) ;
	__( Evaluate ) ;
	__( Index ) ;
	__( Splice ) ;
	__( PushRight ) ;
	__( PushLeft ) ;
	__( PopLeft ) ;
	__( PopRight ) ;
	__( Pull ) ;
	}