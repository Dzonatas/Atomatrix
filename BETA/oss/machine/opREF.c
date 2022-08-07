/***************************************
*
* opREF.c
*
*******************/


#include "opcode.h"
#include "varray.h"


void opREF(void)
	{
	extern atom		aVarray ;
	extern atom		aUndefined ;
	extern atom		aVarrayReferent ;
	extern atom		(*aVarrayReferent_atomize_a_n)(atom,int) ;
	extern void		(*destruct)(atom) ;
	extern void		(*interatomize)(atom,atom) ;
	unsigned int		i ;
	avei			x ;
	avei			y ;
	avei			z ;
	atom			v ;
	unsigned int		n ;
	atom			a ;

	x	= ( y = E_PC + 1 ) ;
	z	= ( E_PC += E_PC->i ) ;
	while( ++y < z )
		{
		v	= E_VA[E_PF->lvar + y->i] ;
		if( a = v->datum )
			if( --a->allusions == 0 )
				destruct( a ) ;
		v->datum = 0 ;
		}
	y	= x + 1 ;
	v	= E_VA[E_PF->lvar + x->i]->datum ;
	if( ! v )
		return ;
	if( v->precursor == aVarrayReferent )
		{
		if( ! v->orbit )
			return ;
		v	= *(atom*)v->datum ;
		}
	if( v->precursor != aVarray )
		return ;
	n	= ((varray)v->datum)->nelem ;
	for( i = 0 ; i < n && y < z ; i++ , y++ )
		E_VA[E_PF->lvar + y->i]->datum = aVarrayReferent_atomize_a_n( v , i ) ;
	}