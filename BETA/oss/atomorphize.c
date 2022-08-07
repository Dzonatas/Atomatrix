/***************************************
*
* atomorphize.c
*
*******************/


#include "ATOMATRIX.h"


atom atomorphize( string S )
	{
	extern atom		aAtomatrix ;
	extern atom		library(string) ;
	extern void		construct(atom) ;
	extern void		interatomize(atom,atom) ;

	return atominul ;
#if 0
	a	= library( S ) ;
	if( a )
		construct( a ) ;
	if( ! a->orbit )
		interatomize( a , aAtomatrix ) ;
	return a ;
#endif
	}
