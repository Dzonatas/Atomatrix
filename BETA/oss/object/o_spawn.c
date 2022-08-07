/***************************************
*
* o_spawn.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

atom	o_spawn( atom A , unsigned T , atom* V )		// UPDATE: aModuleSerinteger
	{
	extern atom		aInteger ;
	extern atom		(*oInteger)(int) ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern int		isValid(ObjID);
	extern ObjID		Create_Obj(ObjID,ObjID);
	extern atom		sframe_scope(void) ;
	ObjID                   NewObj;

	if( T == 1 )
		{
		if( V[0]->precursor != aInteger )
			{
			aError_INVARG->allusions++;
			return aError_INVARG;
			}
		if( ( ! isValid( (ObjID)V[0]->datum ) ) || V[0]->datum == 0 )
			{
			aError_INVOBJ->allusions++;
			return aError_INVOBJ;
			}
		NewObj = Create_Obj( (ObjID)V[0]->datum , ((object)sframe_scope()->datum)->serinteger );

		return oInteger( NewObj );
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	return atominul ;
	}
