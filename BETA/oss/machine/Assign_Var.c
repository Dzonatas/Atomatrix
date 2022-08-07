/***************************************
*
* Assign_Var.c
*
*******************/


#include "ATOMATRIX.h"
#include "sframe.h"
#include "executable.h"

#include "cmp_s.h"


void Assign_Var( string N , atom A )
	{
	extern void		(*destruct)(atom) ;
	extern sframe		E_PF ;
	extern atom*		E_VA ;
	int			i;
	
	int	n = E_PF->exea->varcnt ;
	atom*	s = E_PF->exea->varnames ;

	for( i = 0 ; i < n ; i++ , s++ )
		{
		if( *s )
		        if( cmp_s( (string)(*s)->datum , N ) == 0 )
				{
				atom	v ;
				atom	a ;
				v	= E_VA[E_PF->lvar+i] ;
				if( a = v->datum )
					if( --a->allusions == 0 )
						destruct( a ) ;
				v->datum = A ;
				// UPDATE: use aOprandEquals
				return ;
				}
		}
	if( A )
		if( --A->allusions == 0 )
			destruct( A ) ;
	}
