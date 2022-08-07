/***************************************
*
* opPUSHL.c
*
*******************/


#include "opcode.h"
#include "varray.h"
#include "objvar.h"
#include "sobjvar.h"

void opPUSHL(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aUndefined ;
	extern atom		aError_INVARG ;
	extern atom		aOprandPushLeft ;
	extern void		(*destruct)(atom) ;
	atom			a ;
	atom			aa ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	a	= A ;
	do	{
		aa	= a->interatom ;
		while( aa )
			{
			if( aa->precursor == aOprandPushLeft )
				{
				*(E_SP++) =  ((oprand)aa->datum)( A , B ) ;
				if( --A->allusions == 0 )
					destruct( A ) ;
				( *E_SP	= aUndefined )->allusions++ ;
				if( --B->allusions == 0 )
					destruct( B ) ;
				return ;
				}
			aa = aa->interatoms ;
			}
		a	= a->precursor ;
		} while( a ) ;
	( *(E_SP++) = aError_INVARG )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP = aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
