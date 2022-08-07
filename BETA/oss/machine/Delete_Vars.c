/***************************************
*
* Delete_Vars.c
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"


void Delete_Vars( short cnt )
	{
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern void		(*dealloc)(void*) ;
	extern void		(*destruct)(atom) ;
	extern atom*		E_VA ;
	extern short		E_VC ;
	int			x ;
	atom*			a ;

	if( cnt == 0 )
		return;
	if( ! E_VA )
		return;
	a	= E_VA + ( E_VC -= ( x = cnt ) ) ;
	while( x-- )
		{
		if( *a )
			{
			if( --(*a)->allusions == 0 )
				destruct( *a ) ;
			*a = 0 ;
			}
		a++ ;
		}
/*
	for( x = E_VC - cnt ; x < E_VC ; x++ )
		if( E_VA[x] )
			{
			if( --E_VA[x]->allusions == 0 )
				destruct( E_VA[x] ) ;
			E_VA[x] = 0 ;
			}
*/
	if( E_VC )
		E_VA = allocr( E_VA , sizeof(struct _atom*) * E_VC ) ;
	else	{
		dealloc( E_VA ); 
		E_VA	= 0 ; 
		}
	}