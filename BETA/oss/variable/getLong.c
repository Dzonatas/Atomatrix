/***************************************
*
* getLong.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getLong( atom A, string name, unsigned long * value)
	{
	extern atom		aVariable ;
	variable		v;
	atom			a;

	a		= A->interatom ;
	while( a )
		{
		if( a->precursor == aVariable )
			if( cmp_s( ((variable)a->datum)->name, name ) == 0 )
				{
				v = a->datum;
				if( v->v.Type != v_LONG )
					return atominul;
				*value = v->v.v.Long;
				return a;
				}
		a = a->interatoms;
		}
	return atominul;
	}
