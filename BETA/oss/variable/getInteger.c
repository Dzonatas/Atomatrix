/***************************************
*
* getInteger.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getInteger( atom A, string name, int * value)
	{
	extern atom		aVariable ;
	variable		v;
	atom			a;

	a		= A->interatom ;
	while( a )
		{
		if( a->precursor == aVariable)
			if( cmp_s( ((variable)a->datum)->name, name ) == 0 )
				{
				v = a->datum;
				if( v->v.Type != v_INTEGER )
					return atominul;
				*value = v->v.v.I;
				return a;
				}
		a = a->interatoms;
		}
	return atominul;
	}
