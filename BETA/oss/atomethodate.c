/***************************************
*
* atomethodate.c
*
*******************/


#include "ATOMATRIX.h"
#include "method.h"
#include "cmp_s.h"


atom atomethodate( atom A , string N , unsigned T , atom* V )
	{
	extern atom		aIllogical ;
	extern atom		aMethod ;
	method			m ;
	atom 			a ;
	atom 			aa ;
	
	aa	= A ;
	while( aa )
		{
		a = aa->interatom;
		while( a )
			{
			if( a->precursor == aMethod )
				{
				m = a->datum;
				if( cmp_s( m->name , N ) == 0 )
					return (m->method)( A , T , V ) ;
				}
			a = a->interatoms;
			}
		aa = aa->precursor;
		}
	aIllogical->allusions++ ;
	return aIllogical ;
	}
