/***************************************
*
* o_successants.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"

atom o_successants( atom A , unsigned T , atom* V )
	{
	extern atom		aModule ;
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aError_ARGC ;
	extern atom		aError_INVARG ;
	extern atom		aError_INVOBJ ;
	extern atom		POBJ(ObjID) ;
	extern atom		(*aVarray_atomize_a_n)(atom,unsigned) ;
	extern atom		(*aVarray_append_a_a)(atom,atom) ;
	extern atom		loadModule(atom) ;
	extern void		(*destruct)(atom) ;
	atom			a ;
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
		aError_INVOBJ->allusions++ ;
		return aError_INVOBJ ;
		}
	if( T )	{
		aError_ARGC->allusions++;
		return aError_ARGC;
		}
	z	= aVarray_atomize_a_n( 0 , 0 ) ;
	a	= ((object)A->datum)->intraobject ;
	while( a )
		{
		if( a->precursor == aModuleSerinteger )
			{
			atom	r ;
			if( ( r = loadModule( a ) ) )
				destruct( r ) ;		// UPDATE: handle errant
			}
		if( a->precursor == aModuleXML )
			{
			extern atom aModuleXML_atomorphize(atom) ;
			atom	r ;
			if( ( r = aModuleXML_atomorphize( a ) ) )
				destruct( r ) ;
			}
		a->allusions++ ;
		aVarray_append_a_a( z , a ) ;
		a = ((object)a->datum)->intraobjects ;
		}
	return z ;
	}
