/***************************************
*
* opPOP.c
*
*******************/

#define _CRT_SECURE_NO_WARNINGS

#include "opcode.h"
#include "object.h"
#include "program.h"
#include <stdlib.h>


void opPOP(void)
	{
	extern atom		aUndefined ;
	extern atom		aIllogical ;
	extern void		(*output)(string) ;
	extern void		(*destruct)(atom) ;
	extern string		VarToStr(atom) ;
	atom			a ;

	atom	A	= *(--E_SP) ;

	if( A == aUndefined )
		return ;
	*E_SP	= aUndefined ;
	aUndefined->allusions++ ;
	if( --A->allusions == 0 )
		{
		destruct( A ) ;
		return ;
		}
	a	= A ;
	while( a != aIllogical && a->precursor )
		a = a->precursor ;
	if( a != aIllogical )
		return ;
		{
#if 0
		output( "[[Executed an illogic expression.]]\n" ) ;
#else
		char		s[255] ;
		char*		p ;
		unsigned	l ;
		if( ! E_PF->exea->prog )
			output( "[ Illogical ]\n" ) ;
		else	{
			output( "[ Illogical @ $module serialed( " ) ;
			output( _itoa( ((object)E_PF->exea->prog->a->orbit->datum)->serinteger , s , 10 ) ) ;
			output( " ) program( \"" ) ;
			output( E_PF->exea->prog->name ) ;
			output( "\" ) ] = " ) ;
			output( VarToStr( A ) ) ;
			output( "\n" ) ;
			}
		l = 0 ;
		p = E_PF->exea->linep[E_PC-E_PF->exea->exec-1] ;
		while( l < 250 && *p && *p != '\n' )
			s[l++] = *(p++) ;
		s[l] = 0 ;
		output( s ) ;
		if( l && s[l-1] != '\n' )
			output( "\n" ) ;
#endif
		}
	}


	//while( A->precursor != atomine )
	//	A = A->precursor ;

	//while( A = A->precusor