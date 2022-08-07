/***************************************
*
* opIS.c
*
*******************/


#include "opcode.h"


void opIS(void)
	{
//	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		(*oInteger)(int) ;
	extern void		(*destruct)(atom) ;
//	extern int		(*isChild)(ObjID,ObjID) ;
	int			i ;
	atom			a ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

//	if( A->precursor == aInteger && B->precursor == aInteger )
//		i = isChild( (int)A->datum , (int)B->datum ) ;
//	else	{
		a	= A ;
		while( a && a != B )
			a = a->precursor ;
		if( a )
			i = 1 ;
		else
			i = 0 ;
//		}
	*(E_SP++) = oInteger( i ) ;				// UPDATE: use aEquality or something like that
	if( --A->allusions == 0 )
		destruct( A ) ;
	*E_SP	 = aUndefined ;
	aUndefined->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}