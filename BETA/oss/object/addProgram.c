/***************************************
*
* addProgram.c
*
*******************/


#include "ATOMATRIX.h"
#include "program.h"
#include "cmp_s.h"


atom addProgram( atom A, string N, string S )
	{
	extern atom		aProgram ;
	extern atom		(*atomize)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern string		(*str_dup)(string) ;
	atom			a;
	program			p;

	a	= atomize( aProgram ) ;
	interatomize( a, A ) ;
	p = a->datum	= allocz( sizeof(struct _program) ) ;
	p->a		= a ;
	p->name		= str_dup( N ) ;
	p->psrc		= S ;
	return a ;
	}


