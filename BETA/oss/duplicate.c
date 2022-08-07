/***************************************
*
* duplicate.c
*
*******************/


#include "ATOMATRIX.h"


atom duplicate( atom A )
	{
	extern atom		aDuplicator ;
	atom			(*f)(atom) ;
	atom 			a ;
	atom 			aa ;

	aa	= A ;
	while( aa )
		{
		a = aa->interatom ;
		while( a )
			{
			if( a->precursor == aDuplicator )
				{
				f = a->datum ;
				// if( f( A ) ) return ;
				return f( A ) ;
				}
			a = a->interatoms ;
			}
		aa = aa->precursor ;
		}
	A->allusions++ ;
	return A ;
	}


atom oDuplicator( atom A , void* B )
	{
	extern atom		aDuplicator ;
	extern atom		atomize(atom) ;
	extern void		interatomize(atom,atom) ;
	atom			a ;

	a		= atomize( aDuplicator ) ;
	a->datum	= B ;
	interatomize( a , A ) ;
	return a ;
	}
