/***************************************
*
* addAtom.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom addAtom( atom A , string N , atom O )
	{
	extern atom		aVariable ;
	extern atom		(*atomize)(atom) ;
	extern atom		(*interatomize)(atom,atom) ;
	extern void *		(*allocz)(unsigned int) ;
	extern string		(*str_dup)(string) ;
	variable		v ;

	atom a	= A->interatom ;

	while( a )
		{
		if( a->precursor == aVariable )
			if( cmp_s( ((variable)a->datum)->name , N ) == 0 )
				return atominul ;
		a = a->interatoms ;
		}

	a	= atomize( aVariable ) ;
	interatomize( a , A ) ;

	v = a->datum		= allocz( sizeof(struct _variable) ) ;
	v->name			= str_dup( N ) ;
	v->v.Type		= v_ATOM ;
	v->v.v.Object		= O ;
	return a;
	}

/***************************************
*
* UPDATE:
*
* The string_duplicate() could be not required.
*
*******************/
