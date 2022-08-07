/***************************************
*
* isChild.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"


int isChild( ObjID obj, ObjID ancestor )
	{
	extern atom		(*POBJ)(ObjID) ;
	extern object		(*MOBJ)(atom) ;
	extern object		(*COBJ)(ObjID) ;
	object			o ;
	atom			a ;

	o	= COBJ( obj ) ;
	a	= POBJ( ancestor ) ;
	if( ! ( o && a ) )
		return 0 ;
	do	if( o->precursor == a )
			return 1 ;
		while( ( o = MOBJ(o->precursor) ) ) ;
	return 0 ;
	}
