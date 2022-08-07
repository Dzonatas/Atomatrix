/***************************************
*
* getAtom.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getAtom(atom this, char *name, atom *value)
	{
	extern atom		aVariable ;
	atom			a;
	variable		v;

	a = this->interatom;

	while( a ) {
		if( a->precursor == aVariable )
			if( cmp_s( ((variable)a->datum)->name, name) == 0 ) {
				v = a->datum;
				if(v->v.Type != v_ATOM)
					return atominul;
				*value = v->v.v.Object;
				return a;
				}
		a = a->interatoms;
		}
	return atominul;
	}


/***************************************
*
* UPDATE:
*
* returned atom could need allusions increased
*
*******************/
