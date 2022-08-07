/***************************************
*
* interatomize.c
*
*******************/


#include "ATOMATRIX.h"


void interatomize( atom A , atom orbit )
	{
	atom		 w;

	if( orbit == A->orbit )
		return;
	if( A->orbit )
		{
		if( A->orbit->interatom == A )
			A->orbit->interatom = A->interatoms;
		else	{
			w = A->orbit->interatom;
			while( w->interatoms != A )
				w = w->interatoms;
			w->interatoms = A->interatoms;
			}
		A->orbit = atominul;
		}
	if( orbit == atominul )
		return;
	A->orbit		= orbit;
	A->interatoms		= orbit->interatom;
	orbit->interatom	= A;
	}


/***************************************
*
* UPDATE:
* Function interatomize() does not fully work as named.  This function
* only moves the _atom to interatom allusion.  Function interatomize()
* should include function atomize() and this should be named elsewise.
*
*******************/
