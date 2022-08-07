/***************************************
*
* o_active.c
*
*******************/


#include "ATOMATRIX.h"


atom o_active( atom A , unsigned T , atom* V )
	{
	extern atom		aModuleSerialed ;
	extern atom		(*aVarray_atomize_a_n)(atom,unsigned) ;
	extern void		Active_Follow(atom,atom*);
	atom			a ;

	a			= aVarray_atomize_a_n( 0 , 0 ) ;

	Active_Follow( aModuleSerialed->intratom , &a ) ;

	return a ;
	}
// UPDATE: query anywhere in hierarchy and limit result to such