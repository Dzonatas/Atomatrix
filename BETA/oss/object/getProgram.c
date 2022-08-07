/***************************************
*
* getProgram.c
*
*******************/


#include "ATOMATRIX.h"
#include "program.h"
#include "cmp_s.h"


atom getProgram( atom A, string N, string* P )
	{
	extern atom		aModuleXML ;
	extern atom		aModuleSerinteger ;
	extern atom		aProgram ;
	extern atom		aModule ;
	extern void		(*destruct)(atom) ;
	extern atom		loadModule(atom) ;
	extern atom		aModuleXML_atomorphize(atom) ;
	atom			a ;
	atom			r ;


	if( A->precursor == aModuleSerinteger )
		if( ( r = loadModule( A ) ) )
			{
			if( --r->allusions == 0 )
				destruct( r ) ;	// UPDATE: should return r
			return atominul ;
			}
	if( A->precursor == aModuleXML )
		if( ( r = aModuleXML_atomorphize( A ) ) )
			{
			if( --r->allusions == 0 )
				destruct( r ) ;	// UPDATE: should return r
			return atominul ;
			}
	a	= A->interatom ;
	while( a )
		{
		if( a->precursor == aProgram )
			if( cmp_s( ((program)a->datum)->name, N ) == 0 )
				{
				if( P )
					*P = ((program)a->datum)->psrc;
				return a;
				}
		a = a->interatoms;
		}

	return atominul;
	}
