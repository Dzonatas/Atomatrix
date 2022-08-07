/***************************************
*
* POBJ.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

/*
atom COBJ_Follow(atom A, ObjID id)
	{

	while( A )
		{
		if( id == ((object)A->datum)->this)
			return A;
		if( A->intratom )
			{
			A = COBJ_Follow( A->intratom, id );
			if( A )
				return A;
			}
		A = A->intratoms;
		}
	return atominul;
	}
*/

object COBJ(ObjID id)
	{
	extern atom		oModuleLoad(unsigned,atom*) ;
	atom			a ;

	a = oModuleLoad( id , 0 ) ;
	if( a )
		return a->datum ;
	return (object) 0;
	}


atom POBJ(ObjID id)
	{
	extern atom		oModuleLoad(unsigned,atom*) ;

	return oModuleLoad( id , 0 ) ;
	}
