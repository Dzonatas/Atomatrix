/***************************************
*
* opPULL.c
*
*******************/


#include "opcode.h"
#include "objvar.h"
#include "sobjvar.h"


void opPULL(void)
	{
	typedef atom		(*oprand)(atom,atom) ;
	extern atom		aInteger ;
	extern atom		aUndefined ;
	extern atom		aError_INVARG ;
	extern atom		aOprandPull ;
	extern void		(*destruct)(atom) ;

	atom	B = *(--E_SP) ;
	atom	A = *(--E_SP) ;

	if( B->precursor == aInteger )			// UPDATE: test in *_aOprandPull() procedure
		{
		atom	a ;
		atom	aa ;
		a	= A ;
		do	{
			aa	= a->interatom ;
			while( aa )
				{
				if( aa->precursor == aOprandPull )
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
		}
	( *(E_SP++) = aError_INVARG )->allusions++ ;
	if( --A->allusions == 0 )
		destruct( A ) ;
	( *E_SP	= aUndefined )->allusions++ ;
	if( --B->allusions == 0 )
		destruct( B ) ;
	}
