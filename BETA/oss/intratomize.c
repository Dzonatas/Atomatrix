/***************************************
*
* intratomize.c
*
*******************/

	
#include "ATOMATRIX.h"


void intratomize( atom A , atom precursor )
	{
	atom		a;

	if( precursor == A->precursor )
		return;
	if( A->precursor )
		{
		if( A->precursor->intratom == A )
			A->precursor->intratom = A->intratoms ;
		else	{
			a = A->precursor->intratom ;
			while( a->intratoms != A )
				a = a->intratoms ;
			a->intratoms = A->intratoms ;
			}
		}
	if( A->precursor = precursor )
		{
		A->intratoms		= precursor->intratom ;
		precursor->intratom	= A ;
		}
	}
