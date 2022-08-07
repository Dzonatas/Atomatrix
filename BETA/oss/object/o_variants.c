/***************************************
*
* o_variants.c
*
*******************/


#include "ATOMATRIX.h"


atom o_variants( atom A , unsigned T , atom* V )
	{
	extern atom		aObjVar ;
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVOBJ ;
	extern atom		(*aVarray_atomize_a_n)(atom,int) ;
	extern atom		(*aVarray_append_a_a)(atom,atom) ;
	extern atom		loadModule(atom) ;
	atom			a;
	atom			z ;

	if( A->precursor == aModuleXML )
		{
		extern atom aModuleXML_atomorphize(atom) ;
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			return r ;
		}
	else
	if( A->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( A ) ) )
			return r ;
		}
	else
	if( ( ! A->precursor ) || A->precursor->precursor != aModule )
		{
		aError_INVOBJ->allusions++;
		return aError_INVOBJ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	z	= aVarray_atomize_a_n( 0 , 0 ) ;
	if( a = A->interatom )
		do if( a->precursor == aObjVar )
			{
			a->allusions++ ;
			aVarray_append_a_a( z , a ) ;
			}
		while( a = a->interatoms ) ;
	return z ;
	}
