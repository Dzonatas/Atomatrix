/***************************************
*
* atomethodize.c
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"
#include "method.h"
#include "cmp_s.h"


void atomethodize( atom A, string S , atom (*M)(atom,unsigned,atom*) )
	{
	extern atom		aMethod ;
	extern atom		atomize(atom);
	extern void		interatomize(atom,atom);
	extern void *		allocz(unsigned int);
	atom			a;
	method			m;

	a = A->interatom;

	while( a )
		{
		if( a->precursor == aMethod ) 
			if( cmp_s( ((method)a->datum)->name, S ) == 0 )
				return;
		a = a->interatoms;
		}

	a = atomize( aMethod );
	interatomize( a, A );

	m 		= allocz( sizeof(struct _method) );
	a->datum 	= m;
	m->name		= S ;
	m->method	= M ;
	}
