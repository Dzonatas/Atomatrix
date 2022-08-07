/***************************************
*
* addByte.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom addByte( atom A, string name, unsigned char value )
	{
	extern atom		aVariable ;
	extern void		(*destruct)(atom) ;
	extern atom		(*atomize)(atom) ;
	extern void*		(*allocz)(unsigned int) ;
	extern void		(*interatomize)(atom,atom) ;
	variable		v;
	atom			a;

	a		= A->interatom ;
	while( a )
		{
		if( a->precursor == aVariable )
			if( cmp_s( ((variable)a->datum)->name, name) == 0 )
				return atominul;
		a = a->interatoms;
		}
	a	= atomize( aVariable );
	interatomize( a, A );
	v = a->datum		= allocz( sizeof(struct _variable) );
	v->name			= name;
	v->v.Type		= v_BYTE;
	v->v.v.Byte		= value;
	return a;
	}
