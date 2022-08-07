/***************************************
*
* getVariable_A_S.c
*
*******************/


#include "ATOMATRIX.h"
#include "variant.h"
#include "variable.h"
#include "cmp_s.h"


atom getVariable_A_S( atom A , string N )
	{
	extern atom		aUndefined ;
	extern atom		aVariable ;
	atom			a ;

	atom			aa = A ;

	while( aa )
		{
		a = aa->interatom ;
		while( a )
			{
			if( a->precursor == aVariable )
				if( cmp_s( ((variable)a->datum)->name, N ) == 0 )
					{
					a->allusions++ ;
					return a ;
					}
			a = a->interatoms ;
			}
		aa = aa->precursor ;
		}

	aUndefined->allusions++ ;	// UPDATE: return aError_VARNF if A != aUndefined
	return aUndefined ;
	}
