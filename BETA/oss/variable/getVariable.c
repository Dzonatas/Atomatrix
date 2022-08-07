/***************************************
*
* getVariable.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getVariable( atom A, string N, variable* V )
	{
	extern atom		aVariable ;
	atom			a;
	atom			aa = A;

	while( aa )
		{
		a = aa->interatom;
		while( a )
			{
			if( a->precursor == aVariable )
				if( cmp_s( ((variable)a->datum)->name, N ) == 0 ) {
					if( V )
						*V = (variable) a->datum;
					return a;
					}
			a = a->interatoms;
			}
		aa = aa->precursor;
		}

	return atominul;
	}
