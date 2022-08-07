/***************************************
*
* Add_Vars.c
*
*******************/


#include "ATOMATRIX.h"


void Add_Vars( short cnt )
	{
	extern atom		aUndefined ;
	extern atom		(*oVariant)(atom,atom) ;
	extern void*		(*allocm)(unsigned int) ;
	extern void*		(*allocr)(void*,unsigned int) ;
	extern atom*		E_VA;
	extern short		E_VC;
	short			oldcnt;
	short			x;

	if( cnt == 0 )
		return;
	oldcnt	= E_VC;
	E_VC	+= cnt;
	if( E_VA )
	        E_VA = allocr( E_VA , sizeof(struct _atom*) * E_VC );
	else
	        E_VA = allocm( sizeof(struct _atom*) * E_VC );
	for( x = oldcnt ; x < E_VC ; x++ )
		{  /* 1 */
		E_VA[x] = oVariant( 0 , 0 ) ;
		}
	}


/********************
*
* UPDATE:
* 1 - Optimize loop with incremental reference to E_VA
*
*************/