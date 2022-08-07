/***************************************
*
* isValid.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

int isValid(ObjID oid)
	{
	extern object		COBJ(ObjID);

	if( COBJ(oid) == (object) 0 )
		return 0;

	return 1;
	}
