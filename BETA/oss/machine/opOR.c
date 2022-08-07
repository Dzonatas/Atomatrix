/***************************************
*
* opOR.c
*
*******************/


#include "opcode.h"
#include "varray.h"


void opOR(void)
	{
	extern atom		aAsciiString ;
	extern atom		aInteger ;
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern void		(*destruct)(atom) ;
	int			i ;
									// UPDATE: progressive routine
	atom	A = *(E_SP-1) ;

	i	= 0 ;
	if( A == aUndefined )
		i = 1 ;
	else
	if( A->precursor == aInteger )
		{
		if( (int)A->datum == 0 )
			i = 1 ;
		}
	else
	if( A->precursor == aAsciiString )
		{
		if( ((string)A->datum)[0] == 0 )
			i = 1 ;
		}
	else
	if( A->precursor == aVarray )
		{
		if( ((varray)A->datum)->nelem == 0 )
			i = 1 ;
		}
	else	{
		atom	a ;
		a	= A ;
		while( a && a != aIllogical )
			a = a->precursor ;
		if( a )
			i = 1 ;
		}
	if( i ) {
		if( --A->allusions == 0 )
			destruct( A ) ;
		*(--E_SP) = aUndefined ;
		aUndefined->allusions++ ;
		E_PC++ ;
		return ;
		}
	E_PC	+= (*E_PC).i ;
	}
