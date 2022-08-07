/***************************************
*
* addVariant.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom addVariant( atom A, string N, variant V )
	{
	extern atom		aVariable ;
	extern void		(*destruct)(atom) ;
	extern atom		(*atomize)(atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern void		(*interatomize)(atom,atom) ;
	atom			a;
	variable		v;

	a		= A->interatom ;
	while( a )
		{
		if( a->precursor == aVariable)
			if( cmp_s( ((variable)a->datum)->name, N) == 0 )
				return atominul ;
		a = a->interatoms;
		}
	a	= atomize( aVariable );
	interatomize( a, A );
	v = a->datum	= allocz( sizeof(struct _variable) ) ;
	v->name		= N ;
	if( V )	{
		v->v.Type	= V->Type ;
		v->v.v		= V->v ;
		}
	else	{
		v->v.Type	= 0 ;
		v->v.v.I	= 0 ;
		}
	return a;
	}
