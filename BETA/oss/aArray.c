// aArray.c			Copyright (c) 2004 BALLARD

#if 0

#include "ATOMATRIX.h"


atom			aArray ;
atom			aArrayItem ;


atom insert( atom A , int Z )
	{
	extern atom		(*atomize)(atom) ;
	extern atom		aArray ;
	extern atom		aArrayItem ;
	atom			i ;
	int			z ;
	atom			a ;
	atom			b ;

#	ifdef _DEBUG
	if( A->precursor != aArray)
		_asm int 3 ;

#	endif
	a	= A->interatom ;
	if( Z == 0 )
		{
		i		= atomize( aArrayItem ) ;
		i->orbit	= A ;
		if( a )
			{
			i->interatoms	= A->interatom ;
			A->interatom	= i ;
			a->datum	= i ;
			}
		else	{
			A->interatom	= i ;
			A->datum	= i ;
			}
		return i ;
		}
	if( ! a )
		return atominul ;
	z	= 1 ;
	while( a->interatoms && z < Z )
		{
		a = a->interatoms ;
		z++ ;
		}
	if( z != Z )
		return atominul ;
	i		= atomize( aArrayItem ) ;
	i->orbit	= A ;
	i->datum	= a ;
	if( a->interatoms )
		{
		b		= a->interatoms ;
		i->interatoms	= b ;
		a->interatoms	= i ;
		b->datum	= i ;
		}
	else	{
		a->interatoms	= i ;
		A->datum	= i ;
		}
	return i ;
	}



atom append( atom A )
	{
	extern atom		(*atomize)(atom) ;
	extern atom		aArray ;
	extern atom		aArrayItem ;
	atom			i ;
	atom			a ;

#	ifdef _DEBUG
	if( A->precursor != aArray )
		_asm int 3 ;

#	endif
	i		= atomize( aArrayItem ) ;
	i->orbit	= A ;
	a		= A->datum ;
	A->datum	= i ;
	if( a )
		{
		i->datum	= a ;
		a->interatoms	= i ;
		}
	else	{
		A->interatom	= i ;
		}
	return i ;
	}


atom oArray( void )
	{
	extern atom		(*atomize)(atom) ;
	extern atom		aArray ;
	return atomize( aArray ) ;
	}


#endif