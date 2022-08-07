/***************************************
*
* execCreate.c
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"
#include "executable.h"


atom execCreate()
	{
	extern atom		aExecutable ;
	extern atom		(*atomize)(atom) ;
	extern void*		(*allocz)(unsigned int) ;
	executable		e ;
	atom			a ;

	e			= allocz( sizeof(struct _executable) ) ;
	e->a = a		= atomize( aExecutable ) ;
	a->datum		= e ;
	return a ;
	}
