/***************************************
*
* iscontainedin.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"


int IsContainedIn( ObjID obj, ObjID loc )		// UPDATE: inline this code
	{
	extern object		(*COBJ)(ObjID) ;
	extern object		(*MOBJ)(atom) ;
	extern atom		(*POBJ)(ObjID) ;
	object			o ;
	atom			l ;

	o	= COBJ( obj ) ;
	l	= POBJ( loc ) ;
	if( ! ( o && l ) )
		return 0 ;
	do	{
		if( o->orbit == l )
			return 1 ;
		} while( ( o = MOBJ( o->orbit ) ) ) ;
	return 0 ;
	}
