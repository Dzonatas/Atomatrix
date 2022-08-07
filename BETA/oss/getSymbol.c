/***************************************
*
* getSymbol.c
*
*******************/


#include "ATOMATRIX.h"

#include "cmp_s.h"


atom getSymbol( atom A, string S )
{
extern atom		aSymbol;
atom			a;

a = A->interatom;

while( a )
	{
	if( a->precursor == aSymbol )
		if( cmp_s( (string)a->datum , S ) == 0 )
			return a;
	a = a->interatoms;
	}

return atominul;
}
