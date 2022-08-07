/***************************************
*
* construct.c
*
*******************/


#include "ATOMATRIX.h"


void construct(atom A)
	{
	extern atom		aConstructor ;
	void			(*f)(atom) ;
	atom 			a ;
	atom 			aa ;

	aa =	A ;
	while( aa )
		{
		a = aa->interatom ;
		while( a )
			{
			if( a->precursor == aConstructor )
				{
				f = a->datum;
				f( A );
				return ;
				}
			a = a->interatoms;
			}
		aa = aa->precursor;
		}
	}


/***************************************
*
* UPDATE: construct()
* 1 - another redesign of this function would work
* ? - perhaps incremental construction
*
*******************/


atom oConstructor( atom A , void* B )
	{
	extern atom		aConstructor ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;

	a		= atomize( aConstructor ) ;
	a->datum	= B ;
	interatomize( a , A ) ;
	return a ;
	}


