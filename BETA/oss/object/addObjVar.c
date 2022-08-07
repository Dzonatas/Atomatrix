// addObjVar.c			Copyright (c) 2004 BALLARD


#include "ATOMATRIX.h"
#include "objvar.h"
#include "cmp_s.h"

#include "stdio.h"

atom addObjVar( atom A , string S , atom* V )
	{
	extern atom		aUndefined ;
	extern atom		aObjVar ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern string		(*str_dup)(string) ;
	extern void*		(*allocz)(unsigned int) ;
	atom			a ;
	objvar			v ;

	a			= atomize( aObjVar ) ;
	v = a->datum		= allocz( sizeof(struct _objvar) ) ;
	v->name			= str_dup( S ) ;		// UPDATE: should pool string data
	v->a			= a ;
	if( V )
		v->v		= *V ;
	else
		( v->v	= aUndefined )->allusions++ ;
	interatomize( a , A ) ;
	return a ;
	}


