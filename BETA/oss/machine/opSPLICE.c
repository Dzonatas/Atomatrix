/***************************************
*
* opSPLICE.c
*
*******************/


#include "opcode.h"


void opSPLICE(void)
	{
	typedef atom		(*oprand)(atom,atom,atom) ;
	extern atom		aInteger ;
	extern atom		aIllogical ;
	extern atom		aUndefined ;
	extern atom		aError_TYPE ;
	extern atom		aOprandSplice ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	C = *(--E_SP) ;
	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( B->precursor != aInteger && C->precursor != aInteger )		// UPDATE: do not check for aInteger, do such at aOprandSplice procedure
		{
		( *(E_SP++) = aError_TYPE )->allusions++ ;
		if( --A->allusions == 0 )
			destruct( A ) ;
		( *(E_SP++) = aUndefined )->allusions++ ;
		if( --B->allusions == 0 )
			destruct( B ) ;
		( *E_SP = aUndefined )->allusions++ ;
		if( --C->allusions == 0 )
			destruct( C ) ;
		return;
		}
	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandSplice )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A , B , C ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				( *E_SP = aUndefined )->allusions++ ;
				if( --B->allusions == 0 )
					destruct( B ) ;
				( *(E_SP+1) = aUndefined )->allusions++ ;
				if( --C->allusions == 0 )
					destruct( C ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		} while( ( a = a->precursor ) ) ;
	_asm int 3 ;
	( *E_SP++ = aIllogical )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *(E_SP+1) = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	( *(E_SP+2) = aUndefined )->allusions++ ;
	if( --C->allusions == 0 )
		destruct( C ) ;
	}
