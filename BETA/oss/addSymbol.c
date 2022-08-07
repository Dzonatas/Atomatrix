/***************************************
*
* addSymbol.c
*
*******************/


#include "ATOMATRIX.h"

#include "cmp_s.h"


atom addSymbol( atom A, string S )
	{
	extern atom		atomize(atom);
	extern atom		interatomize(atom,atom);
	extern atom		aSymbol;
	atom			a;

	a = A->interatom;
	while( a )
		{
		if( a->precursor == aSymbol )
			if( cmp_s( (string)a->datum , S ) == 0 )
				return atominul;
		a = a->interatoms;
		}
	a		= atomize( aSymbol );
	interatomize( a, A );
	a->datum	= S;
	return a;
	}




/*************************************
*
* UPDATE:
* addSymbol() should be oSymbolicASCII(atom,string)
*
***********************/