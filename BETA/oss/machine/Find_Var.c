/***************************************
*
* Find_Var.c
*
*******************/


#include "ATOMATRIX.h"
#include "sframe.h"
#include "executable.h"

#include "cmp_s.h"

atom Find_Var( string S )
	{
	extern atom*		E_VA ;
	extern sframe		E_PF ;
	int			i ;

	int	n = E_PF->exea->varcnt ;
	atom*	s = E_PF->exea->varnames ;

	for( i = 0 ; i < n ; i++ , s++ )
		if( *s )
			if( cmp_s( (string)(*s)->datum , S ) == 0 )
				return E_VA[E_PF->lvar+i];
	return 0 ;
	}


// UPDATE; function not used