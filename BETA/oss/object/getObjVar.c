/***************************************
*
* getObjVar.c
*
*******************/


#include "ATOMATRIX.h"
#include "object.h"
#include "objvar.h"
#include "cmp_s.h"


atom getObjVar( atom A, string N, objvar* V )
	{
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		loadModule(atom) ;
	extern void		(*destruct)(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	extern atom		aObjVar ;
	atom			a;

	if( A->precursor == aModuleSerinteger )
		{
		atom	r ;
		if( ( r = loadModule( A ) ) )
			{
			destruct( r ) ;
			return atominul ;
			}
		}
	if( A->precursor == aModuleXML )
		{
		atom	r ;
		if( ( r = aModuleXML_atomorphize( A ) ) )
			{
			destruct( r ) ;
			return atominul ;
			}
		}
	a	= A->interatom ;
	while( a )
		{
		if( a->precursor == aObjVar )
			if( cmp_s( ((objvar)a->datum)->name, N ) == 0 ) {
				if( V )
					*V	= (objvar) a->datum;
				return a;
				}
		a = a->interatoms;
		}
	return atominul;
	}
