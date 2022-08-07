/***************************************
*
* getASCII.c
*
*******************/


#include <stdlib.h>

#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getASCII( atom A, string name, char * value)
	{
	extern atom		aVariable ;
	extern void		(*destruct)(atom) ;
	variable		v ;
	atom			a ;

	a		= A->interatom;
	while( a )
		{
		if( a->precursor == aVariable )
			if( cmp_s( ((variable)a->datum)->name, name ) == 0 )
				{
				v = a->datum;
				if( v->v.Type != v_ASCII )
					return atominul;
				*value = v->v.v.Byte;
				return a;
				}
		a = a->interatoms;
		}
	return atominul;
	}
